/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2000-2014
*
**********************************************************************
*
* @filename     rpcsrv_task.c
*
* @purpose      RPC Server task
*
* @component    rpcsrv
*
* @comments
*
* @create       19-Sep-2010
*
* @end
*
**********************************************************************/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "rpc_common.h"
#include "rpcsrv_util.h"
#include "rpcsrv_msgs.h"
#include "rpcsrv_task.h"
#include "rpc_sockmap.h"

/* internal task stats are global to allow access by devshell dump function */
rpcsrvTaskStats_t   rpcsrvTaskStats_g;

/*********************************************************************
*
* @purpose  Creates Unix domain socket and binds to its server address
*
* @param    pctrl       @b{(input)}  Ptr to task control struct
* @param    *perrcode   @b{(output)} Output location to store system call errno
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments The errcode is only updated when OFDPA_E_NONE is not returned.
*
* @end
*
*********************************************************************/
static OFDPA_ERROR_t rpcsrvSockCreate(rpcsrvTaskCtrl_t *pctrl,
                                      int *perrcode)
{
  int ret = -1;

  rpcsrvPrintf("%s ...\r\n", __FUNCTION__);

  /* open a Unix-domain datagram socket and bind our server addr to it */
  if (pctrl->sockfd < 0)
  {
    pctrl->sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (pctrl->sockfd < 0)
    {
      pctrl->pstats->msockfail++;
      pctrl->sockfd = RPCSRV_SOCK_UNINIT;
      pctrl->isbound = FALSE;
      if (perrcode != NULL)
      {
        *perrcode = errno;
      }
      return OFDPA_E_FAIL;
    }
  }

  rpcsrvPrintf("%s: socket created.\r\n", __FUNCTION__);

  /* have socket descriptor, now try to bind */
  if (pctrl->isbound == FALSE)
  {
    /* remove old socket file in case it exists (otherwise bind will fail) */
    unlink(pctrl->srvaddr.sun_path);

    if ((ret = bind(pctrl->sockfd, (struct sockaddr *)&pctrl->srvaddr, pctrl->srvlen)) < 0)
    {
      pctrl->pstats->mbindfail++;
    }
    else
    {
      rpcsrvPrintf("%s: socket bound.\r\n", __FUNCTION__);

      /* Change access permissions to allow Lighttpd, which is
       * running as a special user, access to the server socket.
       */
      if ((ret = chmod(pctrl->srvaddr.sun_path, RPCSRV_SOCK_SRV_MODE)) < 0)
      {
        rpcsrvPrintf("%s: chmod failed.\r\n", __FUNCTION__);

        pctrl->pstats->mpermfail++;
      }
    }

    /* check for errors in the bind process */
    if (ret < 0)
    {
      /* free up socket descriptor -- will get a new one on next attempt */
      close(pctrl->sockfd);
      pctrl->sockfd = RPCSRV_SOCK_UNINIT;
      if (perrcode != NULL)
      {
        *perrcode = errno;
      }
      return OFDPA_E_FAIL;
    }

    pctrl->isbound = TRUE;
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  RPC Server application task wrapper
*
* @param    argc        @b{(input)}  Number of entries in argv[] list
* @param    argv[]      @b{(input)}  List of task arguments
*
* @returns  void
*
* @comments Handles setup for the task function.
*
* @comments The required argv[] parameters are:
*             none
*
* @end
*
*********************************************************************/
void rpcsrvTask(void *arg)
{
  static rpcsrvTaskCtrl_t  ctrl;
  rpcsrvTaskCtrl_t        *pctrl;
  int                      clilen;
  struct sockaddr_un       cliaddr;
  int                      err;
  int                      mlen;
  rpcDevmsgHdr_t          *phdr;

  /* declared static to avoid stack issues */
  static uint8_t buf[RPCSRV_SOCK_MSG_MAX];
  int            bufsiz = sizeof(buf);

  /* initialize static structures */
  memset(&ctrl, 0, sizeof(ctrl));
  memset(&rpcsrvTaskStats_g, 0, sizeof(rpcsrvTaskStats_g));

  /* init control struct */
  pctrl = &ctrl;
  pctrl->sockfd = RPCSRV_SOCK_UNINIT;
  pctrl->isbound = FALSE;

  pctrl->inst   = 0;
  pctrl->sockid = RPC_SOCKMAP_SINGLE_SRVR_SOCKID;
  pctrl->pstats = &rpcsrvTaskStats_g;

  /* initialize server address struct */
  pctrl->srvaddr.sun_family = AF_UNIX;
  pctrl->srvlen = offsetof(struct sockaddr_un, sun_path) +
    snprintf(pctrl->srvaddr.sun_path, sizeof(pctrl->srvaddr.sun_path) - 1,
             RPCSRV_SOCK_SRV_TMPL, pctrl->sockid);

  rpcsrvPrintf("%s: sun_path = \"%s\".\r\n", __FUNCTION__, pctrl->srvaddr.sun_path);
  rpcsrvPrintf("%s: srvlen = %d.\r\n", __FUNCTION__, pctrl->srvlen);

  /* loop forever in this task */
  while (1)
  {
    /*
     * Create the device server socket using its well-known address.
     *
     * (The FastCGI web clients use this socket to communicate with the
     * switchdrvr device from an external process).
     *
     * NOTE: Restart the socket connection if it is down.
     */
    if (pctrl->sockfd < 0)
    {
      if (rpcsrvSockCreate(pctrl, &err) != OFDPA_E_NONE)
      {
        rpcsrvPrintf("Cannot open server socket: inst=%d sockid=%d errno=%d (%s)",
                     pctrl->inst, pctrl->sockid, err, strerror(err));

        /* wait before trying again */
        sleep(RPCSRV_SOCK_RETRY_WAIT);
        continue;
      }
    }

    /*
     * Wait for a new message from a client process, then
     * handle the message and send back a response (unless the
     * specific message type does not require one).
     *
     * Using MSG_TRUNC flag so we can detect if msg was truncated
     * due to rx buffer being too small.
     *
     * NOTE:
     * This task is implemented as an "iterative server" in that it
     * does not fork() a child process to handle the work.
     *
     */
    memset(&cliaddr, 0, sizeof(cliaddr));
    clilen = sizeof(cliaddr);

    rpcsrvPrintf("%s: About to wait for socket ...\r\n", __FUNCTION__);

    mlen = recvfrom(pctrl->sockfd, buf, bufsiz, MSG_TRUNC, (struct sockaddr *)&cliaddr, &clilen);

    rpcsrvPrintf("%s: received %d bytes.\r\n", __FUNCTION__, mlen);

    /* null terminate client address field */
    cliaddr.sun_path[sizeof(cliaddr.sun_path) - 1] = '\0';

    /* process the request */
    if (mlen < 0)
    {
      if ((pctrl->pstats->mrecvfail % RPCSRV_LOGMSG_THRESH) == 0)
      {
        rpcsrvPrintf("Server socket receive error: inst=%d sockid=%d total_seen=%u errno=%d (%s)",
                     pctrl->inst, pctrl->sockid, pctrl->pstats->mrecvfail+1, errno, strerror(errno));
      }
      pctrl->pstats->mrecvfail++;
      continue;                         /* cannot do anything else with this event */
    }
    else if (mlen > bufsiz)
    {
      /* receive msg was truncated by the socket -- msg buffer is too small */
      if ((pctrl->pstats->mrecvtrunc % RPCSRV_LOGMSG_THRESH) == 0)
      {
        rpcsrvPrintf("Truncated message received on server socket: "
                     "inst=%d sockid=%d total_seen=%u bufsiz=%d msglen=%d cliaddr=%s",
                     pctrl->inst, pctrl->sockid, pctrl->pstats->mrecvtrunc+1, bufsiz, mlen,
                     cliaddr.sun_path);
      }
      pctrl->pstats->mrecvtrunc++;

      /* NOTE: Continue processing this msg. Let Rx proc
       *       function also detect and handle this condition
       *       internally.
       */
    }

    /* process the received message
     *
     * NOTE: Must still respond to the client (unless the
     *       NOWAIT message flag is set), even if the message
     *       receive processing fails.  Otherwise, client will
     *       hang waiting on its recv() from the socket.
     */
    /* init working pointers to msg buffer */
    phdr = (rpcDevmsgHdr_t *)buf;

    if (rpcsrvMsgRxProcess(&cliaddr, buf, bufsiz, &mlen) != OFDPA_E_NONE)
    {
      /* received msg not processed successfully */
      if ((pctrl->pstats->mrecvprocerr % RPCSRV_LOGMSG_THRESH) == 0)
      {
        rpcsrvPrintf("Server socket message processing error: "
                     "inst=%d sockid=%d total_seen=%u msglen=%d cliaddr=%s -- group=%u type=%u seq=0x%8.8x",
                     pctrl->inst, pctrl->sockid, pctrl->pstats->mrecvprocerr+1, mlen,
                     cliaddr.sun_path, phdr->group, phdr->type, phdr->seq);
      }
      pctrl->pstats->mrecvprocerr++;
      pctrl->pstats->mrecvfail++;
    }
    else
    {
      pctrl->pstats->mrecvok++;
    }

    /* send response back to requesting client */
    if (sendto(pctrl->sockfd, buf, mlen, 0, (struct sockaddr *)&cliaddr, clilen) != mlen)
    {
      if ((pctrl->pstats->msendfail % RPCSRV_LOGMSG_THRESH) == 0)
      {
        rpcsrvPrintf("Server socket send error: inst=%d sockid=%d total_seen=%u errno=%d (%s)",
                     pctrl->inst, pctrl->sockid, pctrl->pstats->msendfail+1, errno, strerror(errno));
      }
      pctrl->pstats->msendfail++;
    }
    else
    {
      pctrl->pstats->msendok++;
    }

  } /* endwhile */

  /* clean up socket resources upon exit from this task */
  if (pctrl->sockfd >= 0)
  {
    close(pctrl->sockfd);
    pctrl->sockfd = RPCSRV_SOCK_UNINIT;
    unlink(pctrl->srvaddr.sun_path);
  }
}

/*********************************************************************
*
* @purpose  Debug function to display rpcsrvTask internal statistics
*
* @param    inst        @b{(input)}  Task instance number
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t rpcsrvTaskStatsDump(void)
{
  uint32_t           mode = RPCSRV_MSGLVL_ALWAYS; /* always display output */
  rpcsrvTaskStats_t *pstats;

  /* NOTE: Accessing global data without semaphore protection
   *       so treat as read-only area
   */
  pstats = &rpcsrvTaskStats_g;

  RPCSRV_PRT(mode, "\nCurrent rpcsrvTask() statistics:\n");
  RPCSRV_PRT(mode, "   Send OK................... %u\n", pstats->msendok);
  RPCSRV_PRT(mode, "   Recv OK................... %u\n", pstats->mrecvok);
  RPCSRV_PRT(mode, "   Send Fail................. %u\n", pstats->msendfail);
  RPCSRV_PRT(mode, "   Recv Fail................. %u\n", pstats->mrecvfail);
  RPCSRV_PRT(mode, "   Recv Truncated............ %u\n", pstats->mrecvtrunc);
  RPCSRV_PRT(mode, "   Socket Descr Fail......... %u\n", pstats->msockfail);
  RPCSRV_PRT(mode, "   Socket Bind Fail.......... %u\n", pstats->mbindfail);
  RPCSRV_PRT(mode, "   Socket Permissions Fail... %u\n", pstats->mpermfail);
  RPCSRV_PRT(mode, "\n");
  return OFDPA_E_NONE;
}
