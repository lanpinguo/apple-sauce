/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpcclt_support.c
*
* @purpose   RPC client wrapper support functions.
*
* @component luaweb
*
* @comments
*
* @end
*
**********************************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "rpc_common.h"
#include "rpcclt_msg.h"
#include "rpcclt_support.h"

/*
 *********************
 * Internal Functions
 *********************
 */

/* calculate next parmData[] offset, aligned on defined boundary */
static __inline__ uint32_t _rpccltMsgNextDataOffsetGet(rpccomDevmsg_t *pmsg)
{
  uint8_t     *pnext;

  pnext = (uint8_t *)pmsg + pmsg->hdr.totLen;
  pnext = RPC_DEVMSG_PTR_ALIGN(pnext);
  return (uint32_t)(pnext - pmsg->parmData);
}


/*
 ***************************
 * Message Worker Functions
 ***************************
 */

OFDPA_ERROR_t rpccltMsgAlloc(rpccltProcParms_t *proc,
                       rpccomDevmsg_t **ppmsg)
{
  *ppmsg = (rpccomDevmsg_t *)calloc(1, sizeof(rpccomDevmsg_t));
  return (*ppmsg != NULL) ? OFDPA_E_NONE : OFDPA_E_FAIL;
}

void rpccltMsgFree(rpccltProcParms_t *proc,
                   rpccomDevmsg_t **ppmsg)
{
  if ((ppmsg != NULL) && (*ppmsg != NULL))
  {
    free(*ppmsg);
    *ppmsg = NULL;
  }
}

void rpccltMsgBegin(rpccltProcParms_t *proc,
                    rpccomDevmsg_t *pmsg, RPCCOM_DEVMSG_GRP_t group,
                    uint32_t type, RPCCOM_DEVMSG_FLAGS_t flags)
{
  rpcDevmsgHdr_t     *phdr;

  if (pmsg == NULL)
  {
    return;     /* should not occur if caller checked rpccltMsgAlloc() rc */
  }
  phdr = &pmsg->hdr;

  rpccltLogprintf(proc, RPCCLT_MSGLVL_DETAIL,
            "%s: group=%d type=%u flags=%d msglvl=%d",
            __FUNCTION__, group, type, flags, rpccltDebugMsglvlGet());

  phdr->group = group;
  phdr->type = type;
  phdr->seq = ++(proc->msgseq);                 /* pre-increment before send to compare same with response */
  phdr->flags = (uint32_t)flags;
  phdr->shmemKeyPartial = proc->sockid;
  phdr->maxParms = RPC_DEVMSG_PARMS_MAX;
  phdr->rpcRes = RPCCOM_DEVMSG_RRC_OK;       /* only change if error occurs */
  phdr->totLen = sizeof(*phdr);                 /* init to fixed-size header portion */
}

void rpccltMsgParmAdd(rpccltProcParms_t *proc,
                      rpccomDevmsg_t *pmsg, int32_t fieldSize,
                      uint8_t *parmVal, BOOL isBufferArg)
{
  rpcDevmsgHdr_t *phdr;
  rpcParmEnt_t   *pe;
  uint32_t        offset;
  int32_t         newParmLen;
#if RPC_USE_SHMEM
  int             shmemOffset;
  BOOL            isShmemParm = FALSE;
#endif /* RPC_USE_SHMEM */

  if (pmsg == NULL)
  {
    return;     /* should not occur if caller checked rpccltMsgAlloc() rc */
  }
  phdr = &pmsg->hdr;

  if (phdr->numParms >= phdr->maxParms)
  {
    /* too many parameters for msg hdr capacity */
    phdr->rpcRes = RPCCOM_DEVMSG_RRC_ENUMPARMS;
    return;
  }

  if (phdr->rpcRes != RPCCOM_DEVMSG_RRC_OK)
  {
    return;     /* previous error occurred for this msg -- bail out */
  }

#if RPC_USE_SHMEM
  /* if parm size exceeds shared memory threshold
   * change the destination ptr to the shared
   * memory area and store the shmem ptr itself
   * in the msg for this parm
   *
   * NOTE: Skip this section if parmVal is being
   *       passed as a null ptr.
   */
  if ((parmVal != NULL) &&
      (isBufferArg == TRUE) &&
      (fieldSize > RPC_DEVMSG_PARM_SHMEM_THRESH))
  {
    /* treat this as a shared memory parameter */
    isShmemParm = TRUE;

    /* obtain ptr to shmem data offset area */
    pe = rpccltShmemParmAreaGet(fieldSize);
    if (pe == NULL)
    {
      /* parm data too long */
      phdr->rpcRes = RPCCOM_DEVMSG_RRC_EDATALEN;
      return;
    }

    /* copy caller's data to shared memory area */
    memcpy(&pe->dataStart, parmVal, fieldSize);
    rpccltLogprintf(proc, RPCCLT_MSGLVL_DETAIL,
                    "%s: shmem memcpy done: addr=%p len=%u bytes",
                    __FUNCTION__, &pe->dataStart, fieldSize);
    pe->len = fieldSize;

    /* change the variables to store the shmem area relative offset
     * as the msg parm data below
     */
    shmemOffset = rpccltShmemAddrNormalize((int8_t *)pe);
    fieldSize = sizeof(shmemOffset);
    parmVal = (uint8_t *)&shmemOffset;
  }
#endif /* RPC_USE_SHMEM */

  /* check if enough room is available to copy the parm length value
   * and all of the parm data (or the shmem ptr)
   *
   * NOTE: Must ensure the parm length field is properly aligned.
   */
  offset = _rpccltMsgNextDataOffsetGet(pmsg);
  newParmLen = sizeof(parmlen_t) + fieldSize;
  rpccltLogprintf(proc, RPCCLT_MSGLVL_DETAIL,
                  "%s: totLen=%u offset=%u fieldSize=%d newParmLen=%u parmVal=%p",
                  __FUNCTION__, phdr->totLen, offset, fieldSize, newParmLen, parmVal);
  if ((offset + newParmLen) > RPC_DEVMSG_DATA_MAX)
  {
    /* parm data too long */
    phdr->rpcRes = RPCCOM_DEVMSG_RRC_EDATALEN;
    return;
  }

  /* store the parm data offset in the msg header */
  phdr->parmOffset[phdr->numParms] = offset;

  /* treat the parameter as an abstract sequence of bytes
   * and copy the parm data into the msg buffer
   *
   * for a a shared memory parm, the caller's parm data
   * was already copied into the shared memory area,
   * so the shmem data address is what's copied into
   * the msg buffer here
   *
   * NOTE: This ignores endianness, so cannot be used
   *       to communicate to a different CPU.
   */
  pe = (rpcParmEnt_t *)&pmsg->parmData[offset];  /* start of next data write location */
  pe->len = fieldSize;

  /* if a null ptr was passed instead of an actual parameter
   * value, set a special flag value to indicate same to the
   * RPC server-side processing
   *
   * must not copy any data into the msg buffer (leave
   * initial buffer content as is)
   */
  if (parmVal == NULL)
  {
    pe->len = RPCCOM_DEVMSG_PARM_QUAL_NULLPTR;
  }
  else
  {
    /* copy the caller's data to the msg */
    memcpy(&pe->dataStart, parmVal, fieldSize); /* copy actual parm data into field (if any) */

#if RPC_USE_SHMEM
    if (isShmemParm == TRUE)
    {
      /* overwrite the parm length with a special flag value
       * that indicates the parm "data" is actually a shmem offset
       */
      pe->len = RPCCOM_DEVMSG_PARM_QUAL_SHMEM;
    }
#endif /* RPC_USE_SHMEM */
  }

  /* update msg header fields */
  phdr->totLen = RPCCOM_DEVMSG_MSGLEN_CALC(pmsg, offset) + newParmLen;
  phdr->numParms++;
}

void rpccltMsgEnd(rpccltProcParms_t *proc, rpccomDevmsg_t *pmsg)
{
  int32_t         rrc    = RPCCOM_DEVMSG_RRC_OK;
  rpcDevmsgHdr_t *phdr;
  uint32_t        offset;
  uint32_t        msgLen = 0;
  uint32_t        crc32  = 0, *pcrc = NULL;

  if (pmsg == NULL)
  {
    return;     /* should not occur if caller checked rpccltMsgAlloc() rc */
  }
  phdr = &pmsg->hdr;

  if (phdr->rpcRes != RPCCOM_DEVMSG_RRC_OK)
  {
    return;     /* previous error occurred for this msg -- bail out */
  }

  do
  {
    /* calculate checksum and attach to the end of the msg after last
     * parmData[] byte, aligned to next 4-byte boundary (i.e. adjusted
     * totLen).
     */
    offset = _rpccltMsgNextDataOffsetGet(pmsg);
    /* check if enough room is available for alignment */
    if (offset > RPC_DEVMSG_DATA_MAX)
    {
      /* parm data too long after alignment -- no room to append crc32 value */
      rrc = RPCCOM_DEVMSG_RRC_EDATALEN;
      break;
    }

    /* update total length in msg hdr before computing crc value */
    msgLen = RPCCOM_DEVMSG_MSGLEN_CALC(pmsg, offset);
    phdr->totLen = msgLen + RPCCOM_DEVMSG_CRC_SIZE;


    /* calc crc value over all of the final msg except the crc32 field itself,
     * then append the crc32 value to the end of the msg data
     */
    crc32 = rpccomCrc32((uint8_t *)pmsg, msgLen);
    pcrc = (uint32_t *)&pmsg->parmData[offset];
    *pcrc = crc32;

  } while (0);

  if (rrc != RPCCOM_DEVMSG_RRC_OK)
  {
    phdr->rpcRes = rrc;
  }

  rpccltLogprintf(proc, RPCCLT_MSGLVL_DETAIL,
                  "%s: pmsg=%p offset=%u msgLen=%u totLen=%u "
                  "crc=0x%8.8x pcrc=%p rrc=%d",
                  __FUNCTION__, pmsg, offset, msgLen, phdr->totLen, crc32, pcrc, rrc);
}

OFDPA_ERROR_t rpccltMsgSend(rpccltProcParms_t *proc, rpccomDevmsg_t *pmsg)
{
  rpcDevmsgHdr_t       *phdr;
  rpccltDevmsg_t          devmsg;

  if (pmsg == NULL)
  {
    return OFDPA_E_FAIL;
  }
  phdr = &pmsg->hdr;

  if (phdr->rpcRes != RPCCOM_DEVMSG_RRC_OK)
  {
    return OFDPA_E_FAIL;        /* previous error occurred for this msg -- bail out */
  }

  /* set up msg container struct */
  memset(&devmsg, 0, sizeof(devmsg));
  rpccltMsgFlagsConvert(phdr->flags, &devmsg.mflags);
  devmsg.bufsiz = sizeof(*pmsg);
  devmsg.mlen = (int)pmsg->hdr.totLen;
  devmsg.bufptr = (int8_t *)pmsg;

  rpccltMsgDump(proc, pmsg, "Message contents sent:");

  if (0 > rpccltDevmsgSend(&devmsg))
  {
    return OFDPA_E_FAIL;
  }

  rpccltMsgDump(proc, pmsg, "Message contents received:");

  return OFDPA_E_NONE;
}

void rpccltMsgDump(rpccltProcParms_t *proc, rpccomDevmsg_t *pmsg, int8_t *title)
{
#define RPCCLT_MAX_DATA    8
  int                           msglvl = RPCCLT_MSGLVL_MSGDATA;
  rpcDevmsgHdr_t               *phdr;
  int                           i, j, jmax;
  uint8_t                       buf[(RPCCLT_MAX_DATA*3)+1];
  uint8_t                       strbuf[RPCCLT_MAX_DATA+1];
  uint8_t                       tmp[8];
  uint8_t                      *pdata;
  RPCCOM_DEVMSG_PARM_OVERLAY_t *pover;

  if (pmsg == NULL)
  {
    return;     /* should not occur if caller checked rpccltMsgAlloc() rc */
  }
  phdr = &pmsg->hdr;

  if (title != NULL)
  {
    rpccltLogprintf(proc, msglvl, "%s", title);
  }
  rpccltLogprintf(proc, msglvl,
                  "--------------------------------------------------");
  rpccltLogprintf(proc, msglvl,
                  "mlen=%u grp=%u type=%u seq=0x%8.8x flgs=%u rrc=%d maxp=%u nump=%u",
                  phdr->totLen, phdr->group, phdr->type, phdr->seq, phdr->flags,
                  phdr->rpcRes, phdr->maxParms, phdr->numParms);
  for (i = 0; i < phdr->numParms; i++)
  {
    pover = (RPCCOM_DEVMSG_PARM_OVERLAY_t *)&pmsg->parmData[phdr->parmOffset[i]];
    pdata = pover->u.DT_uint8_t;

    memset(buf, 0, sizeof(buf));
    memset(strbuf, 0, sizeof(strbuf));

    if (pover->len >= 0)
    {
      jmax = (pover->len < RPCCLT_MAX_DATA) ? pover->len : RPCCLT_MAX_DATA;
    }
#if RPC_USE_SHMEM
    else if (pover->len == RPCCOM_DEVMSG_PARM_QUAL_SHMEM)
    {
      jmax = sizeof(int);
    }
#endif /* RPC_USE_SHMEM */
    else if (pover->len == RPCCOM_DEVMSG_PARM_QUAL_NULLPTR)
    {
      /* null output ptr */
      jmax = 0;
      strcat(buf, "<<null ptr>>");
    }
    else
    {
      /* unexpected length -- leave buf empty */
      jmax = 0;
    }
    for (j = 0; j < jmax; j++)
    {
      sprintf(tmp, "%2.2x ", *pdata++);
      strcat(buf, tmp);
    }

    /* create potential string view of the data (may not be meaningful in all cases) */
    pdata = pover->u.DT_uint8_t;
    if (pover->len >= 0)
    {
      jmax = strlen(pdata);
      jmax = (jmax < RPCCLT_MAX_DATA) ? jmax : RPCCLT_MAX_DATA;
      for (j = 0; j < jmax; j++)
      {
        if (*pdata < ' ' || *pdata > '~')
        {
          break;
        }
        strbuf[j] = *pdata++;
      }
    }

    if (strlen(strbuf) > 0)
    {
      rpccltLogprintf(proc, msglvl,
                      "[%02d]: offset=%u len=%d data: %s -->  %s",
                      i, phdr->parmOffset[i], pover->len, buf, strbuf);
    }
    else
    {
      rpccltLogprintf(proc, msglvl,
                      "[%02d]: offset=%u len=%d data: %s",
                      i, phdr->parmOffset[i], pover->len, buf);
    }
  }
  rpccltLogprintf(proc, msglvl, "----- End of Message -----");
#undef RPCCLT_MAX_DATA
}

void rpccltMsgFlagsConvert(RPCCOM_DEVMSG_FLAGS_t flags,
                           rpccltDevmsgFlg_e *msgflags)
{
  rpccltDevmsgFlg_e       tmp;

  if (msgflags != NULL)
  {
    tmp = RPCCLT_DEVMSG_FLG_NONE;

    if ((flags & RPCCOM_DEVMSG_FLAGS_NOWAIT) != 0)
    {
      tmp |= RPCCLT_DEVMSG_FLG_NOWAIT;
    }

    *msgflags = tmp;
  }
}

/*
 * NOTE: All variable args passed to this function must be of type rpccltVaParm_t.
 *
 * NOTE: Only modifies incoming *fretval if wrapper function returns OFDPA_E_NONE.
 */
void rpccltSuppWrapperCommon(int fid, va_list args, RPCCOM_DEVMSG_GRP_t msgGrp, rpccltFtableEnt_t **ftable)
{
  rpccltProcParms_t   *proc = rpccltProcPtrGet();
  rpccomDevmsg_t      *pmsg = NULL;
  RPCCOM_DEVMSG_RRC_t  rrc  = RPCCOM_DEVMSG_RRC_ERROR;
  OFDPA_ERROR_t        rc   = OFDPA_E_NONE;
  uint32_t             i;
  rpcParmEnt_t        *pd;
  va_list              ap;
  rpccltFtableEnt_t   *pfti;
  int32_t              fieldSize;
  rpccltVaParm_t       pparm;
  uint8_t             *pdest;
  ofdpa_buffdesc      *pbufd;
  uint8_t             *pval;
  BOOL                 isOutputParm;
  BOOL                 isBufferArg;
#if RPC_USE_SHMEM
  int                  shmemOffset;
#endif /* RPC_USE_SHMEM */

  /* set up msg header and variable parm data */
  if (rpccltMsgAlloc(proc, &pmsg) != OFDPA_E_NONE)
  {
    rpccltLogprintf(proc, RPCCLT_MSGLVL_ACTIVE,
                    "%s (inst=%d grp=%d): Msg alloc failed! (pmsg=%x)",
                    __FUNCTION__, proc->inst, msgGrp, pmsg);
    return;
  }

#if RPC_USE_SHMEM
  /* re-init the shared memory area */
  rpccltShmemParmInit();
#endif /* RPC_USE_SHMEM */

  /* point to this function's entry in the input parm table */
  pfti = ftable[fid];

  rpccltMsgBegin(proc, pmsg, msgGrp, fid, pfti->flags);

  va_copy(ap, args);                    /* init arg ptr to start of variable args list */
  for (i = 0; i < pfti->numParms; i++)
  {
    isOutputParm = (RPCCLT_OUTPARMSMASK_FLAG_GET(pfti, i) != 0) ? TRUE : FALSE;
    isBufferArg = FALSE;
    pparm = va_arg(ap, rpccltVaParm_t);      /* read next parm ptr */
    pval = (uint8_t *)pparm;
    fieldSize = pfti->parmIdx[i].fieldSize;
    if (fieldSize == RPCCOM_DEVMSG_PARM_BUFFER_ARG)
    {
      /* this is a buffer parameter containing both a buffer length
       * and starting address
       *
       * examples:
       *    xxxx(uint32_t UnitIndex, yyy_buffdesc *pbufd)
       *    xxxx(uint32_t UnitIndex, yyy_buffdesc bufd)
       *
       * use size field from buffer descriptor as the size of the
       * parm field in the msg buffer
       *
       * NOTE: It is possible for the caller to pass a null ptr here
       *       so need to handle it accordingly.
       */
      pbufd = *(ofdpa_buffdesc **)pparm;
      if (pbufd == NULL)
      {
        /* caller passed null ptr -- no data to send/recv thru RPC */
        fieldSize = 0;
        pval = (uint8_t *)NULL;
        rpccltLogprintf(proc, RPCCLT_MSGLVL_DETAIL,
                        "%s (inst=%d grp=%d): buffer parm ptr is null: pbufd=%p bufSize=%u bufp=%p",
                        __FUNCTION__, proc->inst, msgGrp, pbufd, fieldSize, pval);
      }
      else
      {
        /* buffer descriptor field access technique depends on whether this
         * parameter is defined as a buffer descriptor or a pointer to one,
         * as indicated by the isOutputParm flag
         */
        if (isOutputParm == FALSE)
        {
          /* API parm defined as buffdesc (not a pointer), so don't
           * dereference pparm for access to the buffer descriptor fields
           */
          pbufd = (ofdpa_buffdesc *)pparm;
        }

        isBufferArg = TRUE;
        fieldSize = pbufd->size;
        pval = pbufd->pstart;
        rpccltLogprintf(proc, RPCCLT_MSGLVL_DETAIL,
                        "%s (inst=%d grp=%d): buffer parm: pbufd=%p bufSize=%u bufp=%p val=%s",
                        __FUNCTION__, proc->inst, msgGrp, pbufd, pbufd->size, pbufd->pstart, pval);
      }
    }
    else if (isOutputParm == TRUE)
    {
      /* for any output parm (other than the buffer descriptors handled above),
       * treat the incoming parameter value as a pointer and dereference it
       * to obtain the initial data value for the function call.
       *
       * NOTE: Caller can pass a null ptr for the data parm (i.e. when permitted
       *       by the API) which will end up in pval.
       */
      pval = *(uint8_t **)pparm;
    }
    rpccltMsgParmAdd(proc, pmsg, fieldSize, pval, isBufferArg);
  } /* endfor i */

  rpccltMsgEnd(proc, pmsg);

  rrc = pmsg->hdr.rpcRes;

  rpccltLogprintf(proc, RPCCLT_MSGLVL_VERBOSE,
                  "%s (inst=%d grp=%d): Built message for: group=%d type=%d len=%d rrc=%d",
                  __FUNCTION__, proc->inst, msgGrp, pmsg->hdr.group, pmsg->hdr.type, pmsg->hdr.totLen,
                  pmsg->hdr.rpcRes);

  if (rrc != RPCCOM_DEVMSG_RRC_OK)
  {
    va_end (ap);
    return;
  }

  /* send msg to device server and wait for response */
  rc = rpccltMsgSend(proc, pmsg);

  /* handle output from operation */
  if (rc == OFDPA_E_NONE)
  {
    pfti = ftable[fid];

    /* collect output parm data into caller's buffer */
    va_copy(ap, args);                                 /* init arg ptr to start of variable args list */
    for (i = 0; i < pfti->numParms; i++)
    {
      pparm = va_arg(ap, rpccltVaParm_t);    /* read next parm ptr */
      if (RPCCLT_OUTPARMSMASK_FLAG_GET(pfti, i) != 0)
      {
        /* assume any parameter passed by reference (i.e. pointer args)
         * is a potential output parameter and copy its contents from
         * the msg buffer back to the caller's arg location
         *
         * NOTE: If the parameter is input only, its original msg request
         *       value and length is still preserved in the response msg,
         *       so this copies the same content on top of itself in the
         *       caller's environment. This is necessary since the rpcclt
         *       has no way of knowing the intended usage of a pointer
         *       arg.
         */
        pd = (rpcParmEnt_t *)&pmsg->parmData[pmsg->hdr.parmOffset[i]];
        pbufd = NULL;
        if (pfti->parmIdx[i].fieldSize == RPCCOM_DEVMSG_PARM_BUFFER_ARG)
        {
          /* this is a buffer parameter containing both a buffer length
           * and starting address
           *
           * adjust the values used to control copying the msg data
           * to the caller's destination location
           */
#if RPC_USE_SHMEM
          if (pd->len == RPCCOM_DEVMSG_PARM_QUAL_SHMEM)
          {
            /* this parm uses shared memory
             *
             * compute shmem address by combining the relative offset
             * contained in the msg data with the shmem base address
             */
            memcpy(&shmemOffset, &pd->dataStart, sizeof(shmemOffset));
            pd = (rpcParmEnt_t *)rpccltShmemAddrAbsolute(shmemOffset);
          }
#endif /* RPC_USE_SHMEM */
          pbufd = *(ofdpa_buffdesc **)pparm;
          if (pbufd == NULL)
          {
            /* caller passed null ptr -- no data to send/recv thru RPC */
            pdest = (uint8_t *)NULL;
          }
          else
          {
            pbufd->size = pd->len;
            pdest = pbufd->pstart;
          }
        }
        else
        {
          /* "normal" parameter (i.e. a pointer to something to be byte-copied) */
          pdest = *(uint8_t **)pparm;
        }

        if (pdest != NULL)
        {
          /* only copy output data if a non-null ptr was provided by the caller */
          memcpy(pdest, &pd->dataStart, pd->len);

          if (i > 0)
          {
            rpccltLogprintf(proc, RPCCLT_MSGLVL_VERBOSE,
                            "%s (inst=%d grp=%d): Copied output (parm %d): pd=%p pd->len=%d "
                            "pd->buf=%s pbufd=%p pdest=%p",
                            __FUNCTION__, proc->inst, msgGrp, i, pd, pd->len, &pd->dataStart,
                            pbufd, pdest);
          }
          else
          {
            rpccltLogprintf(proc, RPCCLT_MSGLVL_VERBOSE,
                            "%s (inst=%d grp=%d): Copied output (parm %d): pd=%p pd->len=%d "
                            "pd->buf@=%p pbufd=%p pdest=%p",
                            __FUNCTION__, proc->inst, msgGrp, i, pd, pd->len, &pd->dataStart,
                            pbufd, pdest);
          }
        }
      }
    } /* endfor i */
  }

  rpccltMsgFree(proc, &pmsg);
  va_end (ap);
}
