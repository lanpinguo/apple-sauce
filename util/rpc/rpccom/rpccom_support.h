/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpccom_support.h
*
* @purpose   RPC common wrapper support header file.
*
* @component mgmt
*
* @comments
*
* @create    19-Sep-2010
*
* @end
*
**********************************************************************/

#ifndef _RPCCOM_SUPPORT_H
#define _RPCCOM_SUPPORT_H

#include <stdint.h>
#include "rpc_common.h"

/* calculate current message length */
#define RPCCOM_DEVMSG_MSGLEN_CALC(_pmsg, _offset) \
  (uint32_t)((void *)(&(_pmsg)->parmData[(_offset)]) - (void *)(_pmsg))

/* message CRC field size */
#define RPCCOM_DEVMSG_CRC_SIZE       sizeof(uint32_t)

/* message parameter flags */
typedef enum
{
  RPCCOM_DEVMSG_PARM_BUFFER_ARG      = -100, /* obtain buffer info from parm buffer descriptor */
} RPCCOM_DEVMSG_PARM_t;

/* common socket message parameter data overlay definition */
typedef struct
{
  int32_t               len;                   /* length of parameter data (or negative val for flag) */
  union                                         /* supported parm data types */
  {
    /* complete list of data types defined for all msg parameters */
    int8_t   DT_int8_t[1];
    uint8_t  DT_uint8_t[1];
    uint32_t DT_uint32_t;
  } u;
} RPCCOM_DEVMSG_PARM_OVERLAY_t;

/* socket message RPC return code (RRC) values */
typedef enum
{
  RPCCOM_DEVMSG_RRC_OK               = 0,    /* success */
  RPCCOM_DEVMSG_RRC_ERROR            = -1,   /* general error */
  RPCCOM_DEVMSG_RRC_TIMEOUT          = -2,   /* socket timeout occurred */
  RPCCOM_DEVMSG_RRC_ESETUP           = -3,   /* data setup error */
  RPCCOM_DEVMSG_RRC_EDATALEN         = -4,   /* incorrect msg data length */
  RPCCOM_DEVMSG_RRC_ENUMPARMS        = -5,   /* too many function parameters */
  RPCCOM_DEVMSG_RRC_NOHDR            = -6,   /* msg header missing or incomplete */
  RPCCOM_DEVMSG_RRC_BADGROUP         = -7,   /* invalid group id used */
  RPCCOM_DEVMSG_RRC_BADTYPE          = -8,   /* invalid type id used */
  RPCCOM_DEVMSG_RRC_ESHMEM           = -9    /* shared memory error */

} RPCCOM_DEVMSG_RRC_t;

/* socket message groups */
typedef enum
{
  RPCCOM_DEVMSG_GRP_USMDB            = 1,    /* USMDB functions   */
  RPCCOM_DEVMSG_GRP_OPENAPI          = 2,    /* OPENAPI functions */
  RPCCOM_DEVMSG_GRP_OFDPA            = 3     /* OFDPA functions   */
} RPCCOM_DEVMSG_GRP_t;

/* socket message flags */
typedef enum
{
  RPCCOM_DEVMSG_FLAGS_NONE           = 0,
  RPCCOM_DEVMSG_FLAGS_NOWAIT         = (1 << 0)    /* message does not require response */

} RPCCOM_DEVMSG_FLAGS_t;

/* socket message parm qualifiers */
typedef enum
{
#if RPC_USE_SHMEM
  RPCCOM_DEVMSG_PARM_QUAL_SHMEM      = -1,   /* parm entry resides in shared memory */
#else
  RPCCOM_DEVMSG_PARM_RSVD1           = -1,   /* reserved -- do not use (was shared memory flag) */
#endif /* RPC_USE_SHMEM */
  RPCCOM_DEVMSG_PARM_QUAL_NULLPTR    = -2    /* parm entry represents NULL value */
} RPCCOM_DEVMSG_PARM_QUAL_t;

/* socket message structure
 *
 * the crc32 value gets stored after the last parmData byte, aligned
 * to the next 4-byte boundary; the parmData[] array is sized to
 * include the CRC
 *
 * NOTE: Do not use sizeof(parmData) to determine the max data length
 *       allowed, since it includes room for the CRC value.  Use
 *       RPC_DEVMSG_DATA_MAX instead.
 */
typedef struct
{
  rpcDevmsgHdr_t hdr;                   /* fixed-size device msg header */
                                        /* parameter data area + CRC */
  uint8_t        parmData[RPC_DEVMSG_DATA_MAX + RPCCOM_DEVMSG_CRC_SIZE];
} rpccomDevmsg_t;

/* public functions */
extern uint32_t rpccomCrc32(uint8_t *pblk, uint32_t blklen);

#endif /* _RPCCOM_SUPPORT_H */
