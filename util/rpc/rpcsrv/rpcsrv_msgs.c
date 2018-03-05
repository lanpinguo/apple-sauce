/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2000-2014
*
**********************************************************************
*
* @filename     rpcsrv_msgs.c
*
* @purpose      RPC Server socket message handler.
*
* @component    rpcsrv
*
* @comments     none
*
* @create       19-Sep-2010
*
* @end
*
**********************************************************************/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "ofdpa_datatypes.h"
#include "rpc_common.h"
#include "rpcsrv_support.h"
#include "rpcsrv_util.h"
#include "rpcsrv_msgs.h"

/* OFDPA message group headers */
#include "rpccom_ofdpa.h"
#include "rpcsrv_ofdpa.h"

/*********************************************************************
*
* @purpose  Process message received from Unix domain socket
*
* @param    *pcliaddr   @b{(input)}  Ptr to requesting client's address
* @param    *buf        @b{(input)}  Ptr to buffer containing received message
* @param    bufsiz      @b{(input)}  Overall size of buffer area
* @param    *pmlen      @b{(output)} Ptr to received message length (perhaps truncated)
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments An OFDPA_E_NONE return value does not imply that the received
*           message was good, only that it was handled properly by
*           this function.  Check rc in message parm[0] data area
*           to determine whether it succeeded or not.
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t rpcsrvMsgRxProcess(struct sockaddr_un *pcliaddr,
                                 uint8_t *buf, int bufsiz,
                                 int *pmlen)
{
  BOOL            needFailMsg = FALSE;
  rpcDevmsgHdr_t *phdr;
  uint32_t       *pcrc;
  uint32_t        crc32;
  int             mlen;                 /* local copy -- must set *pmlen if value is changed */

  /* init */
  phdr = (rpcDevmsgHdr_t *)buf;
  mlen = *pmlen;

  /* init response results for success (overwrite if failure occurs) */
  phdr->rpcRes = RPCCOM_DEVMSG_RRC_OK;

  if (mlen > bufsiz)
  {
    /* receive msg was truncated by the socket -- msg buffer is too small */
    phdr->rpcRes = RPCCOM_DEVMSG_RRC_EDATALEN;
    needFailMsg = TRUE;
  }
  else if (mlen < sizeof(*phdr))
  {
    /* not enough data to process the msg header, so
     * build own 'failed' msg response in the buffer
     */
    phdr->rpcRes = RPCCOM_DEVMSG_RRC_NOHDR;
    needFailMsg = TRUE;
  }
  else
  {
    /* calculate crc32 of received msg (excluding appended crc32 field)
     *
     * adjust msg lengths to strip off crc32 value from request msg
     * so that response msg crc32 is properly calculated and stored
     *
     * NOTE: The operators = and -= are of equal precedence and
     *       each associates from right-to-left.
     */
    *pmlen = mlen -= RPCCOM_DEVMSG_CRC_SIZE;
    crc32 = rpccomCrc32(buf, mlen);
    phdr->totLen -= RPCCOM_DEVMSG_CRC_SIZE;  /* adjust AFTER calculating crc32 */

    /* verify msg CRC is valid and that received msg length
     * matches the total length stored in msg hdr
     */
    pcrc = (uint32_t *)(buf + mlen);
    if ((*pcrc != crc32) || (mlen != phdr->totLen))
    {
      /* msg protocol error */
      phdr->rpcRes = RPCCOM_DEVMSG_RRC_ESETUP;
      needFailMsg = TRUE;
    }
    else
    {
      /* msg received successfully
       *
       * give control to group-specific function to process it further
       */

      /* terminate msg buffer data as a precaution
       *
       * NOTE: This overwrites first byte of crc32 value in msg buffer.
       */
      *(buf + mlen) = '\0';

      /* invoke the appropriate msg group function */
      switch (phdr->group)
      {
        case RPCCOM_DEVMSG_GRP_OFDPA:
          rpcsrvMsgOfdpaRxProcess(buf, bufsiz, pmlen);
          break;

        default:
          /* unrecognized group */
          phdr->rpcRes = RPCCOM_DEVMSG_RRC_BADGROUP;
          break;

      } /* endswitch */
    }
  }

  if (needFailMsg == TRUE)
  {
    /* build a special failure msg in the buffer, using a
     * group and type of 0 (only sending the header portion
     * in this response)
     *
     * make sure phdr->rpcRes was set prior to here by the
     * corresponding error path
     *
     * mainly want to send something to kick the client out of
     * its recv() wait state
     */
    memset(phdr, 0, sizeof(*phdr));
    *pmlen = mlen = sizeof(*phdr);
    phdr->totLen = mlen + RPCCOM_DEVMSG_CRC_SIZE;
  }

  /* update total length in msg hdr before computing crc value */
  phdr->totLen = mlen + RPCCOM_DEVMSG_CRC_SIZE;

  /* calc crc value over entire msg (except for crc32 field itself),
   * then append the crc32 value to the end of the msg contents
   *
   * the final msg length includes the crc32 field
   */
  crc32 = rpccomCrc32(buf, mlen);
  pcrc = (uint32_t *)(buf + mlen);
  *pcrc = crc32;

  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Process API group request message received from Unix domain socket
*
* @param    msgGrp      @b{(input)}  Message group identifier
* @param    **ftable    @b{(input)}  Group-specific function table ptr
* @param    *buf        @b{(input)}  Ptr to buffer containing received message
* @param    bufsiz      @b{(input)}  Overall size of buffer area
* @param    *pmlen      @b{(output)} Ptr to received message length (perhaps truncated)
*
* @returns  void
*
* @comments Can assume caller has checked validity of input parameters.
*
* @comments The processing result is recorded in the message header
*           rpcRes.  The API function return code is provided in
*           the message parm[0] data area.
*
* @end
*
*********************************************************************/
void rpcsrvMsgApiRxProcess(RPCCOM_DEVMSG_GRP_t msgGrp, rpcsrvFtableEnt_t **ftable,
                           uint8_t *buf, int bufsiz, int *pmlen)
{
  rpccomDevmsg_t               *pmsg = (rpccomDevmsg_t *)buf;
  rpcDevmsgHdr_t               *phdr = (rpcDevmsgHdr_t *)buf;
  uint8_t                       rcBuf[RPCSRV_API_RC_LEN_MAX];
  ofdpa_buffdesc                bufd[RPC_DEVMSG_PARMS_MAX];
  ofdpa_buffdesc               *pparm[RPC_DEVMSG_PARMS_MAX];
  RPCCOM_DEVMSG_PARM_OVERLAY_t *pover;
  rpcsrvFtableEnt_t            *pfte;
  uint32_t                      numParms;
  int                           p;

  if (RPCSRV_MSGLVL_ACTIVE(RPCSRV_MSGLVL_5))
  {
    rpcsrvPrintf("Entering %s: grp=%d bufp=%p bufsiz=%d mlen=%d",
                 __FUNCTION__, msgGrp, buf, bufsiz, *pmlen);
  }

  /* make sure number of parameters indicated in msg
   * does not exceed defined maximum
   */
  numParms = phdr->numParms;
  if (numParms > RPC_DEVMSG_PARMS_MAX)
  {
    phdr->rpcRes = RPCCOM_DEVMSG_RRC_ESETUP;
    return;
  }

  pfte = ftable[phdr->type];

  memset(rcBuf, 0, sizeof(rcBuf));
  memset(bufd, 0, sizeof(bufd));
  memset(pparm, 0, sizeof(pparm));
  for (p = 0; p < numParms; p++)
  {
    pparm[p] = &bufd[p];
    pover = (RPCCOM_DEVMSG_PARM_OVERLAY_t *)&pmsg->parmData[phdr->parmOffset[p]];
    if (pover->len == RPCCOM_DEVMSG_PARM_QUAL_NULLPTR)
    {
      /* parm pointer being passed as NULL
       *
       * use NULL for this parameter's data ptr in the usmdb API call;
       * there is no data to extract from the RPC message, nor is there
       * a buffer descriptor to pass to the next layer
       */
      pparm[p]->size = 0;
      pparm[p]->pstart = NULL;
    }
    else
    {
      pparm[p]->size = pover->len;
      pparm[p]->pstart = pover->u.DT_int8_t;
    }
  }

  /* execute specific RPC server function (it fills in rcBuf with
   * the API return value)
   */
  (pfte->srvfn)(numParms, pparm, rcBuf);

  /* set USMDB function return value in msg parm[0] data area using the
   * expected size of the return value provided in the request msg
   */
  pover = (RPCCOM_DEVMSG_PARM_OVERLAY_t *)&pmsg->parmData[phdr->parmOffset[RPC_DEVMSG_IDX_RETVAL]];
  if (pover->len >= 0)
  {
    memcpy(pover->u.DT_uint8_t, rcBuf, pover->len);
  }

  /* set resultant buffer length into msg data area for BUFFER_ARG parms */
  for (p = RPC_DEVMSG_IDX_FIRSTPARM; p < numParms; p++)
  {
    if (pfte->parmSize[p] == RPCCOM_DEVMSG_PARM_BUFFER_ARG)
    {
      pover = (RPCCOM_DEVMSG_PARM_OVERLAY_t *)&pmsg->parmData[phdr->parmOffset[p]];
      {
        /* NOTE: This also covers case where (pover->len == RPCCOM_DEVMSG_PARM_QUAL_NULLPTR) */
        pover->len = pparm[p]->size;
      }
    }
  }

  phdr->rpcRes = RPCCOM_DEVMSG_RRC_OK;

  if (RPCSRV_MSGLVL_ACTIVE(RPCSRV_MSGLVL_5))
  {
    rpcsrvPrintf("Leaving %s: grp=%d bufp=%p bufsiz=%d mlen=%d fid=%d numParms=%d rrc=%d",
                 __FUNCTION__, msgGrp, buf, bufsiz, *pmlen, phdr->type, numParms, phdr->rpcRes);
  }
}

/*********************************************************************
*
* @purpose  Process OFDPA request message received from Unix domain socket
*
* @param    *buf        @b{(input)}  Ptr to buffer containing received message
* @param    bufsiz      @b{(input)}  Overall size of buffer area
* @param    *pmlen      @b{(output)} Ptr to received message length (perhaps truncated)
*
* @returns  void
*
* @comments Can assume caller has checked validity of input parameters.
*
* @comments The processing result is recorded in the message header
*           rpcRes.  The API function return code is provided in
*           the message parm[0] data area.
*
* @end
*
*********************************************************************/
void rpcsrvMsgOfdpaRxProcess(uint8_t *buf, int bufsiz, int *pmlen)
{
  static BOOL                  ftableIsInit = FALSE;
  RPCCOM_DEVMSG_GRP_t          msgGrp = RPCCOM_DEVMSG_GRP_OFDPA;
  rpcsrvFtableEnt_t            **ftable = rpcsrv_ofdpa_ftable_g;
  rpcDevmsgHdr_t               *phdr = (rpcDevmsgHdr_t *)buf;

  /* initialize the server-side RPC OFDPA function table */
  if (ftableIsInit == FALSE)
  {
    rpcsrvOfdpaFtableInit();
    ftableIsInit = TRUE;
    rpcsrvPrintf("%s: Initialized RPC server OFDPA function table.", __FUNCTION__);
  }

  /* check msg type within range */
  if ((phdr->type == RPCCOM_OFDPA_FID_NONE) ||
      (phdr->type >= RPCCOM_OFDPA_FID_TOTAL))
  {
    phdr->rpcRes = RPCCOM_DEVMSG_RRC_BADTYPE;
    return;
  }

  /* invoke common function to handle the RPC request message */
  rpcsrvMsgApiRxProcess(msgGrp, ftable, buf, bufsiz, pmlen);
}
