/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpcclt_msg_api.c
*
* @purpose   RPC client messaging support
*
* @component rpcclt
*
* @comments
*
* @create    10-Sep-2011
*
* @end
*
**********************************************************************/

#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "ofdpa_datatypes.h"
#include "rpc_common.h"
#include "rpc_sockmap.h"
#include "rpcclt_api.h"
#include "rpcclt_msg.h"

/* instantiate proc parms data struct */
static rpccltProcParms_t rpccltProcParms;


/* wait for data to be received from a socket */
static int _recvData(int sockfd, ofdpa_buffdesc bufd, int timeout)
{
  fd_set fds;
  int nbytes = RPCCLT_RC_ERR;             /* init to something non-successful */
  int n;
  BOOL done;

  do
  {
    done = TRUE;
    /* init fd set with specified socket fd */
    FD_ZERO(&fds);
    FD_SET(sockfd, &fds);

    /* NOTE: Decided not to timeout this call in the RPC layer.  The Process
     *       Manager will detect any hangs and restart the processes, as needed.
     *       Besides, the socket state ended up mostly unusable when a timeout
     *       did occur, so more work would be needed to clean up the communication
     *       path between RPC client and server.
     */

    n = select(sockfd+1, &fds, NULL, NULL, NULL);

    /* check select result */
    switch (n)
    {
      case -1:
        /* abnormal termination of select call */
        if (errno == EINTR)
        {
          done = FALSE;
        }
        else
        {
          nbytes = RPCCLT_RC_ERR;
        }
        break;

      default:
        /* data arrived from socket, so receive it normally
         *
         * NOTE: Did not set non-blocking socket option earlier,
         *       but this recv call won't block now since data
         *       is available.
         *
         *       using MSG_TRUNC flag here so we can detect if msg was truncated
         *       due to rx buffer being too small
         */
        do
        {
          nbytes = recv(sockfd, bufd.pstart, bufd.size, MSG_TRUNC);
        } while (nbytes < 0 && errno == EINTR);
        break;
    }
  } while (done == FALSE);
  return nbytes;
}

/* retrieves proc ptr */
rpccltProcParms_t *rpccltProcPtrGet(void)
{
  return &rpccltProcParms;
}

/* set rpc timeout value */
void rpccltTimeoutSet(int val)
{
  rpccltProcParms.clienttimout = (val >= 0) ? val : RPCCLT_SOCK_RECV_TIMEOUT;
  return;
}

/* initialize communication path with switchdrvr device
 *
 * the following set of startup arguments are expected
 * in the argv[] array:
 *
 *  0: requesting process name
 *  1: process instance number
 *  2: RPC client socket id
 *  3: RPC server socket id
 *  4: log file name (fully-qualified path) - or NULL ptr if not used
 *  5: config file name (fully-qualified path) - or NULL ptr if not used
 *  6: config parm message level token name - or NULL ptr if not used
 *
 * the message level can be modified at any time by writing to the specified
 * config file the token string followed by the message level value with a
 * single space in between, for example:
 *
 *      debugMsgLvl 4
 *
 */
int rpccltCommSetup(int argc, char *argv[])
{
  rpccltProcParms_t *proc = rpccltProcPtrGet();
  struct sockaddr_un cliaddr;
  int clilen;

  if (argc != RPCCLT_START_ARGS_TOTAL)
  {
    return RPCCLT_RC_ERR;
  }

  /* clear out the proc data struct */
  memset(proc, 0, sizeof(*proc));

  /* obtain parms from the argv list
   * (see function prolog for expected order)
   */
  proc->inst = atoi(argv[RPCCLT_START_ARGS_INST]);
  proc->sockid = atoi(argv[RPCCLT_START_ARGS_SOCKID]);
  proc->srvrsockid = atoi(argv[RPCCLT_START_ARGS_SRVRSOCKID]);
  if (argv[RPCCLT_START_ARGS_LOGFILE] != NULL)
  {
    strncpy(proc->logfile, argv[RPCCLT_START_ARGS_LOGFILE], sizeof(proc->logfile) - 1);
  }
  if (argv[RPCCLT_START_ARGS_CFGFILE] != NULL)
  {
    strncpy(proc->cfgfile, argv[RPCCLT_START_ARGS_CFGFILE], sizeof(proc->cfgfile) - 1);
  }
  if (argv[RPCCLT_START_ARGS_CFGPARM] != NULL)
  {
    strncpy(proc->cfgmsglvlname, argv[RPCCLT_START_ARGS_CFGPARM], sizeof(proc->cfgmsglvlname) - 1);
  }

  /* initialize other proc data struct fields */
  proc->pid = getpid();
  proc->msgseq = (uint32_t)time(NULL);
  proc->singlesrvr = RPC_SINGLE_SRVR_MODE;
  proc->debugMsgLvl = RPCCLT_DEBUG_MSGLVL_INIT;
  proc->logfd = RPCCLT_FD_UNINIT;
  proc->sockfd = RPCCLT_SOCK_UNINIT;
  proc->clienttimout = RPCCLT_SOCK_RECV_TIMEOUT;

  /* if using single server mode, replace the srvsockid with the
   * common id used for all external processes
   */
  if (proc->singlesrvr != 0)
  {
    proc->srvrsockid = RPC_SOCKMAP_SINGLE_SRVR_SOCKID;
  }

  rpccltLogprintf(proc, RPCCLT_MSGLVL_ALWAYS,
                  "Entering %s: inst=%d msgseq=0x%8.8x", __FUNCTION__, proc->inst, proc->msgseq);

#if RPC_USE_SHMEM
  /* init shared memory related fields */
  proc->shmid = RPCCLT_SHMID_UNALLOC;
  proc->shmsize = RPC_SHMEM_SIZE;
  proc->shmdata = NULL;

  /* create a shared memory area for message parameter data overflow */
  if (RPCCLT_RC_OK != rpccltShmemCreate())
  {
    return RPCCLT_RC_ERR;
  }
#endif /* RPC_USE_SHMEM */

  /*
   * bind to the device server socket using specific address of this client
   * (we connect() to this socket during message send processing)
   */

  if (RPCCLT_SOCK_UNINIT == proc->sockfd)
  {
    /* set up client address struct (i.e. "return address" for the socket) */
    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    snprintf(cliaddr.sun_path, sizeof(cliaddr.sun_path)-1,
             RPCCLT_SOCK_CLI_TMPL, proc->sockid);
    clilen = offsetof(struct sockaddr_un, sun_path) + strlen(cliaddr.sun_path);
    memcpy(proc->cliaddr, cliaddr.sun_path, sizeof(proc->cliaddr)-1); /* leaves last byte null */

    /* open a Unix-domain datagram socket and bind our client addr to it */
    if (0 > (proc->sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)))
    {
      rpccltLogprintf(proc, RPCCLT_MSGLVL_ACTIVE,
                      "Cannot open datagram socket: "
                      "inst=%d addr=%s errno=%s",
                      proc->inst, cliaddr.sun_path, strerror(errno));
      return RPCCLT_RC_ERR;
    }

    unlink((const char *)cliaddr.sun_path);     /* remove old socket file if it exists */
    if (0 > bind(proc->sockfd, (const struct sockaddr *)&cliaddr, clilen))
    {
      rpccltLogprintf(proc, RPCCLT_MSGLVL_ACTIVE,
                      "Cannot bind local address to datagram socket: "
                      "inst=%d addr=%s errno=%s",
                      proc->inst, cliaddr.sun_path, strerror(errno));
      close(proc->sockfd);
      proc->sockfd = RPCCLT_SOCK_UNINIT;
      return RPCCLT_RC_ERR;
    }
  }

  rpccltLogprintf(proc, RPCCLT_MSGLVL_ACTIVE,
                  "Created datagram socket: "
                  "inst=%d sockid=%d sockfd=%d cliaddr=%s",
                  proc->inst, proc->sockid, proc->sockfd, proc->cliaddr);

  rpccltLogprintf(proc, RPCCLT_MSGLVL_ALWAYS,
                  "Leaving %s: inst=%d", __FUNCTION__, proc->inst);

  return RPCCLT_RC_OK;
}

/* de-initialize communication path with switchdrvr device */
int rpccltCommTeardown(void)
{
  rpccltProcParms_t *proc = rpccltProcPtrGet();
  int tmp_sockfd;

  rpccltLogprintf(proc, RPCCLT_MSGLVL_ALWAYS,
                  "Entering %s: inst=%d", __FUNCTION__, proc->inst);

  /* save temp value for debug msg below */
  tmp_sockfd = proc->sockfd;

  /* close client Unix-domain socket */
  if (RPCCLT_SOCK_UNINIT != proc->sockfd)
  {
    close(proc->sockfd);
    proc->sockfd = RPCCLT_SOCK_UNINIT;
    proc->connected = 0;
    unlink((const char *)proc->cliaddr);        /* remove the socket file */

    rpccltLogprintf(proc, RPCCLT_MSGLVL_ACTIVE,
                    "Closed datagram socket: "
                    "inst=%d sockid=%d sockfd=%d cliaddr=%s",
                    proc->inst, proc->sockid, tmp_sockfd, proc->cliaddr);
  }

#if RPC_USE_SHMEM
  /* disconnect from and delete the shared memory segment for this client */
  if (RPCCLT_RC_OK != rpccltShmemDelete())
  {
    return RPCCLT_RC_ERR;
  }
#endif /* RPC_USE_SHMEM */

  rpccltLogprintf(proc, RPCCLT_MSGLVL_ALWAYS,
                  "Leaving %s: inst=%d", __FUNCTION__, proc->inst);

  /* ensure that the log file is closed
   * (each process instance has its own descriptor)
   */
  rpccltDebugLogFileClose();

  return RPCCLT_RC_OK;
}

/* send a message to switchdrvr device and optionally wait for reply
 *
 * returns:
 *    0 - success (including send with NOWAIT option)
 *   -1 - error, message not sent
 *   -2 - error, message sent but response not received
 */
int rpccltDevmsgSend(rpccltDevmsg_t *msg)
{
  rpccltProcParms_t  *proc = rpccltProcPtrGet();
  ofdpa_buffdesc      bufd;
  int                 nbytes;
  struct sockaddr_un  srvaddr;
  rpcDevmsgHdr_t     *phdr;
  int                 srvlen;
  int                 err;
  BOOL                done;

  /* use this opportunity to update the debug message level
   * to the latest value specified by the user in the
   * Lua Magnet config file
   */
  rpccltDebugMsglvlUpdate();

  /* establish a specific connection with the server if not yet connected */
  if (0 == proc->connected)
  {
    /* even though using datagram (connectionless) service, call connect()
     * here to identify the server addr as the only allowed endpoint of
     * this socket
     *
     * this requires the server side to create its socket address, so
     * keep trying for a finite amount of time before giving up
     *
     * the proc->singlesrvr field indicates whether sharing a single server
     * socket or mapping 1:1 between client and server (sockid is usually
     * the same on both sides for each lua_magnet instance)
     *
     * NOTE: as an added benefit, this allows us to use send() and recv()
     *       instead of sendto() and recvfrom()
     */

    /* set up server address struct */
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sun_family = AF_UNIX;
    snprintf(srvaddr.sun_path, sizeof(srvaddr.sun_path)-1,
             RPCCLT_SOCK_SRV_TMPL, proc->srvrsockid);
    srvlen = offsetof(struct sockaddr_un, sun_path) + strlen(srvaddr.sun_path);

    do
    {
      done = TRUE;
      if (0 > connect(proc->sockfd, (const struct sockaddr *)&srvaddr, srvlen))
      {
        err = errno;                        /* preserve original errno */
        rpccltLogprintf(proc, RPCCLT_MSGLVL_ACTIVE,
                      "Error connecting to server socket: "
                      "inst=%d srvrsockid=%d sockfd=%d srvaddr=%s errno=%s",
                      proc->inst, proc->srvrsockid, proc->sockfd, srvaddr.sun_path, strerror(err));
        if (err == EINTR)
        {
          done = FALSE;
        }
        else
        {
          proc->ctrs.mconnfail++;
          /* give server side a moment to catch up */
          if (err == ENOENT)
          {
            sleep(1);                               /* seconds */
          }
          return RPCCLT_RC_ERR;
        }
      }
    } while (done == FALSE);
    proc->connected = 1;
  }

  do
  {
    done = TRUE;
    /* send the message on the socket connected to the device */
    if (msg->mlen != send(proc->sockfd, msg->bufptr, msg->mlen, 0))
    {
      /* error sending message on socket -- log first occurrence and maintain count */
      err = errno;                        /* preserve original errno */
      if (err == EINTR)
      {
        done = FALSE;
      }
      else
      {
        if (0 == proc->ctrs.msendfail)
        {
          rpccltLogprintf(proc, RPCCLT_MSGLVL_ACTIVE,
                      "Error sending message to device socket: "
                      "inst=%d srvrsockid=%d sockfd=%d errno=%s",
                      proc->inst, proc->srvrsockid, proc->sockfd, strerror(errno));
        }
        proc->ctrs.msendfail++;
        errno = err;
        return RPCCLT_RC_ERR;
      }
    }
  } while (done == FALSE);
  proc->ctrs.msendok++;

  phdr = (rpcDevmsgHdr_t *)msg->bufptr;
  rpccltLogprintf(proc, RPCCLT_MSGLVL_BASIC,
                  "Sent message to device socket: "
                  "inst=%d srvrsockid=%d sockfd=%d mlen=%d group=%u type=%u rrc=%d",
                  proc->inst, proc->srvrsockid, proc->sockfd, msg->mlen,
                  phdr->group, phdr->type, phdr->rpcRes);

  /* if message NOWAIT flag is set, return to caller without
   * waiting for the server to respond
   */
  if (0 != (msg->mflags & RPCCLT_DEVMSG_FLG_NOWAIT))
  {
    proc->ctrs.mnowait++;
    return RPCCLT_RC_OK;
  }

  /* wait for the server to respond (overwrite caller's buffer with msg)
   */
  bufd.pstart = msg->bufptr;
  bufd.size = msg->bufsiz - 1;
  nbytes = _recvData(proc->sockfd, bufd, proc->clienttimout);
  err = errno;                        /* preserve original errno */

  if (0 > nbytes)
  {
    if (0 == proc->ctrs.mrecvfail)
    {
      rpccltLogprintf(proc, RPCCLT_MSGLVL_EXCEPT,
                      "Error receiving message from device socket: "
                      "inst=%d srvrsockid=%d sockfd=%d errno=%s",
                      proc->inst, proc->srvrsockid, proc->sockfd, strerror(errno));
    }
    proc->ctrs.mrecvfail++;
    errno = err;
    return RPCCLT_RC_ERR2;

  }
  else if (bufd.size < nbytes)
  {
    /* receive msg was truncated by the socket -- msg buffer is too small */
    if (0 == proc->ctrs.mrecvtrunc)
    {
      rpccltLogprintf(proc, RPCCLT_MSGLVL_EXCEPT,
                      "Truncated message received from device socket (discarded): "
                      "inst=%d sockid=%d srvrsockfd=%d bufsiz=%d msglen=%d",
                      proc->inst, proc->srvrsockid, proc->sockfd, bufd.size, nbytes);
    }
    proc->ctrs.mrecvtrunc++;
    proc->ctrs.mrecvfail++;
    errno = err;
    return RPCCLT_RC_ERR2;
  }
  else if (phdr->seq != proc->msgseq)
  {
    /* receive msg did not contain expected sequence number */
    if (0 == proc->ctrs.mrecvseqerr)
    {
      rpccltLogprintf(proc, RPCCLT_MSGLVL_EXCEPT,
                      "Sequence number mismatch in message received from device socket (discarded): "
                      "inst=%d srvrsockid=%d sockfd=%d expected=0x%8.8x actual=0x%8.8x",
                      proc->inst, proc->srvrsockid, proc->sockfd, proc->msgseq, phdr->seq);
    }
    proc->ctrs.mrecvseqerr++;
    proc->ctrs.mrecvfail++;
    errno = err;
    return RPCCLT_RC_ERR2;
  }

  /* msg was received successfully -- terminate buffer and return to caller */
  *(msg->bufptr + nbytes) = '\0';
  msg->mlen = nbytes;
  proc->ctrs.mrecvok++;

  rpccltLogprintf(proc, RPCCLT_MSGLVL_BASIC,
                  "Received message from device socket: "
                  "inst=%d srvrsockid=%d sockfd=%d mlen=%d group=%u type=%u seq=0x%8.8x rrc=%d",
                  proc->inst, proc->srvrsockid, proc->sockfd, msg->mlen,
                  phdr->group, phdr->type, phdr->seq,
                  phdr->rpcRes);

  return RPCCLT_RC_OK;
}
