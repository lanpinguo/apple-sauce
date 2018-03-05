/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     ofdpa_client.c
*
* @purpose      OF-DPA Client-only non-RPC API implementation
*
* @component    OF-DPA
*
* @comments     This file is only used by the client code.
*
* @create       26 Apr 2013
*
* @end
*
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <syslog.h>
#include "ofdpa_api.h"
#include "datapath.h"
#ifdef OFDPA_RPC
#include "rpcclt_api.h"
#include "rpc_sockmap.h"
#else
#include "system_config_api.h"
#include "ofdpa_util.h"
#include "rpcsrv_task.h"
#endif

#ifdef OFDPA_RPC

#define OFDPA_CLIENT_LOG_FILE    "/tmp/ofdpa_client.log"
#define OFDPA_CLIENT_CFG_FILE    "/etc/ofdpa_cfg.txt"
#define OFDPA_CLIENT_DEBUG_TOKEN "debugMsgLvl"

/* Maximum number of packets queued in a UNIX datagram socket. */
#define DRIVER_SOCKET_Q_MAX  1024

/* UNIX datagram socket "address" for client and server. */
#define OFDPA_CLIENT_ADDR   "/tmp/ofdpa_pkt_client"
#define OFDPA_SERVER_ADDR   "/tmp/ofdpa_pkt_server"

/* Minimum and maximum ethernet frame lengths for packet transmit API.
 * Lengths include Ethernet header and payload, and do not include the
 * 4-byte CRC trailer. We assume the hardware automatically pads Ethernet
 * frames to the minimum length required on the wire. */
#define OFDPA_MIN_PKT_LEN 14
#define OFDPA_MAX_PKT_LEN 12288


struct ofdpaClientHandle_s
{
  rpccltProcParms_t *proc;
  int                   initialized;
} ofdpaClientHandle =
{
  .initialized = 0,
};
#endif

struct ofdpaSocketHandle_s
{
  int     eventSockFd;
  int     pktSockFd;
} ofdpaSocketHandle =
{
  .eventSockFd = -1,
  .pktSockFd = -1,
};

/* socket is created as a blocking socket */
static
int dpaClientEventSocketCreate(void)
{
  struct sockaddr_un clsockaddr;
  int addrlen;
  int sockfd;
  int retval;
  unsigned int rcvSize = (50 * 1024);   /* bytes */

  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    return -1;
  }

  memset(&clsockaddr, 0, sizeof(clsockaddr));
  clsockaddr.sun_family = AF_UNIX;
  snprintf(clsockaddr.sun_path, sizeof(clsockaddr.sun_path) - 1, OFDPA_EVENT_CLIENT_ADDR);
  addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(clsockaddr.sun_path);
  unlink((const char *)clsockaddr.sun_path);     /* remove old socket file if it exists */

  retval = bind(sockfd, (const struct sockaddr *)&clsockaddr, addrlen);
  if (retval < 0)
  {
    close(sockfd);
    return -1;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvSize, sizeof(rcvSize)) == -1)
  {
    close(sockfd);
    return -1;
  }

  return sockfd;
}

/* socket is created as a blocking socket */
static
int dpaClientPktSocketCreate(void)
{
  struct sockaddr_un clsockaddr;
  int addrlen;
  int sockfd;
  int retval;
  unsigned int rcvSize = (50 * 1024);   /* bytes */

  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    return -1;
  }

  memset(&clsockaddr, 0, sizeof(clsockaddr));
  clsockaddr.sun_family = AF_UNIX;
  snprintf(clsockaddr.sun_path, sizeof(clsockaddr.sun_path) - 1, "%s", OFDPA_CLIENT_ADDR);
  addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(clsockaddr.sun_path);
  unlink((const char *)clsockaddr.sun_path);     /* remove old socket file if it exists */

  retval = bind(sockfd, (const struct sockaddr *)&clsockaddr, addrlen);
  if (retval < 0)
  {
    close(sockfd);
    return -1;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvSize, sizeof(rcvSize)) == -1)
  {
    close(sockfd);
    return -1;
  }

  return sockfd;
}

OFDPA_ERROR_t ofdpaClientInitialize(char *clientName)
{
  int rc;

#ifndef OFDPA_RPC
  void *threadPtr;

  rc = systemMappingInit();
  if (0 != rc)
  {
    return OFDPA_E_FAIL;
  }
  printf("Starting rpcsrvTask.\r\n");
  threadPtr = dpaThreadCreate("rpcsrvTask", 250, rpcsrvTask, NULL);
  if (NULL == threadPtr)
  {
    printf("Failed starting rpcsrvTask.\r\n");
    return OFDPA_E_FAIL;
  }
#else
  int rargc;
  char *rargv[RPCCLT_START_ARGS_TOTAL + 1];
  char client_sock_id_str[8];
  char server_sock_id_str[8];
  char client_inst_num[8];

  if (NULL == clientName)
  {
    return OFDPA_E_PARAM;
  }

  if (0 != ofdpaClientHandle.initialized)
  {
    return OFDPA_E_EXISTS;
  }

  ofdpaClientHandle.proc = rpccltProcPtrGet();

  /* Create server and client-side socket id string using sockmap table entry
   * for this instance
   */

  snprintf(client_sock_id_str, sizeof(client_sock_id_str)-1,
           "%d", getpid());

  snprintf(server_sock_id_str, sizeof(server_sock_id_str)-1,
           "%d", RPC_SOCKMAP_SINGLE_SRVR_SOCKID);

  snprintf(client_inst_num, sizeof(client_inst_num),
           "%d", 1);

  /* Set up RPC client invocation arg list */
  rargc = RPCCLT_START_ARGS_TOTAL;

  /* caller's executable file name */
  rargv[RPCCLT_START_ARGS_REQ_PROC] = clientName;

  /* process instance number */
  rargv[RPCCLT_START_ARGS_INST] = client_inst_num;

  /* client-side socket id */
  rargv[RPCCLT_START_ARGS_SOCKID] = client_sock_id_str;

  /* server-side socket id */
  rargv[RPCCLT_START_ARGS_SRVRSOCKID] = server_sock_id_str;

  rargv[RPCCLT_START_ARGS_LOGFILE] = OFDPA_CLIENT_LOG_FILE;

  rargv[RPCCLT_START_ARGS_CFGFILE] = OFDPA_CLIENT_CFG_FILE;

  /* config parm msglvl token name */
  rargv[RPCCLT_START_ARGS_CFGPARM] = OFDPA_CLIENT_DEBUG_TOKEN;

  rargv[rargc] = NULL;  /* last in list (required) */

  rpccltLogprintf(ofdpaClientHandle.proc, RPCCLT_MSGLVL_DETAIL, "%s: Calling rpccltCommSetup.\r\n", __FUNCTION__);

  rc = rpccltCommSetup(rargc, rargv);
  if (rc != RPCCLT_RC_OK)
  {
    return OFDPA_E_FAIL;
  }

  ofdpaClientHandle.initialized = 1;
#endif

  return OFDPA_E_NONE;
}



int ofdpaCltLogPrintf(int priority, char *fmt, ...)
{
#ifndef OFDPA_RPC
  va_list       ap;

  va_start(ap, fmt);
  return ofdpavLog(priority, fmt, ap);
#else
  ofdpa_buffdesc message;
  va_list ap;
  char buf[200];
  int  rc = 0;

  va_start(ap, fmt);

  rc = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (rc < 0)
  {
    return rc;
  }

  if (rc > 0)
  {
    if (sizeof(buf) == rc)
    {
      buf[sizeof(buf) - 1] = '\0';
    }

    message.pstart = buf;
    message.size = rc + 1;

    //ofdpaCltLogBuf(priority, message);
  }

  return rc;
#endif
}

int ofdpaCltDebugPrintf(const char *functionName, ofdpaComponentIds_t component, ofdpaDebugLevels_t verbosity, const char *format, ...)
{
  va_list       ap;

#ifndef OFDPA_RPC
  va_start(ap, format);
  return ofdpaDebugvPrintf(functionName, component, verbosity, format, ap);
#else
  ofdpa_buffdesc function;
  ofdpa_buffdesc message;
  char buf[200];
  char fname[100];
  int  rc = 0;

  va_start(ap, format);

  rc = vsnprintf(buf, sizeof(buf), format, ap);
  va_end(ap);

  if (rc < 0)
  {
    return rc;
  }

  if (rc > 0)
  {
    if (sizeof(buf) == rc)
    {
      buf[sizeof(buf) - 1] = '\0';
    }

    message.pstart = buf;
    message.size = rc + 1;

    strncpy(fname, functionName, sizeof(fname) - 1);
    fname[sizeof(fname) - 1] = '\0';    /* Guarantee null termination */
    function.pstart = fname;
    function.size   = strlen(fname) + 1;

    //rc = ofdpaCltDebugBuf(function, component, verbosity, message);
  }

  return rc;
#endif
}

OFDPA_ERROR_t ofdpaPktReceive(struct timeval *timeout, ofdpaPacket_t *pkt)
{
  return OFDPA_E_FAIL;
}

OFDPA_ERROR_t ofdpaEventReceive(struct timeval *timeout)
{
  int ofdpaEventClientFd;
  dpaEventMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

  ofdpaEventClientFd = ofdpaClientEventSockFdGet();
  if (ofdpaEventClientFd < 0)
  {
    return OFDPA_E_FAIL;
  }

  if (timeout)
  {
    if ((timeout->tv_sec == 0) && (timeout->tv_usec == 0))
    {
      /* set socket to non-blocking for this read */
      flags |= MSG_DONTWAIT;
    }
    else
    {
      /* blocking socket with a timeout */
      rv = setsockopt(ofdpaEventClientFd, SOL_SOCKET, SO_RCVTIMEO, (char *)timeout,
                      sizeof(struct timeval));
      if (rv < 0)
      {
        ofdpaCltLogPrintf(LOG_DEBUG,
                          "Failed to set packet receive timeout. Error %d.\r\n", rv);
        return OFDPA_E_FAIL;
      }
    }
  }
  else
  {
    /* blocking socket with no timeout. Make sure there is no timeout configured
     * on the socket from previous call. */
    rv = setsockopt(ofdpaEventClientFd, SOL_SOCKET, SO_RCVTIMEO, NULL, 0);
  }

  recvBytes = recvfrom(ofdpaEventClientFd, &msg, sizeof(msg), flags, 0, 0);

  if (recvBytes < 0)
  {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
    {
      /* Normal if no packets waiting to be received and caller didn't block. */
      return OFDPA_E_TIMEOUT;
    }
    ofdpaCltLogPrintf(LOG_DEBUG,
                      "Failed to receive packet. recvfrom() returned %d. errno %s.\r\n",
                      recvBytes, strerror(errno));
    return OFDPA_E_FAIL;
  }

  /* drain any additional event messages from socket */
  flags = MSG_DONTWAIT;
  do
  {
    recvBytes = recvfrom(ofdpaEventClientFd, &msg, sizeof(msg), flags, 0, 0);
  } while(recvBytes > 0);

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaClientEventSockBind(void)
{
  ofdpaSocketHandle.eventSockFd = dpaClientEventSocketCreate();

  if (ofdpaSocketHandle.eventSockFd < 0)
  {
    ofdpaSocketHandle.eventSockFd = -1;
    return OFDPA_E_FAIL;
  }
  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaClientPktSockBind(void)
{
  ofdpaSocketHandle.pktSockFd = dpaClientPktSocketCreate();

  if (ofdpaSocketHandle.pktSockFd < 0)
  {
    ofdpaSocketHandle.pktSockFd = -1;
    return OFDPA_E_FAIL;
  }
  return OFDPA_E_NONE;
}

int ofdpaClientEventSockFdGet(void)
{
  return ofdpaSocketHandle.eventSockFd;
}

int ofdpaClientPktSockFdGet(void)
{
  return ofdpaSocketHandle.pktSockFd;
}
