/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpcclt_debug.c
*
* @purpose   RPC client debugging/logging support
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

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include "rpc_common.h"
#include "rpcclt_api.h"
#include "rpcclt_msg.h"


/* output a message to the RPC client log file */
void rpccltLogprintf(rpccltProcParms_t *proc, int msglvl, int8_t *fmt, ...)
{
  FILE *fd;
  time_t curtime;
  int8_t timestamp[RPCCLT_LOG_DATESIZE+1];
  size_t num;
  va_list ap;

  /* check msglvl designation for this message vs. current run-time setting */
  if (proc->debugMsgLvl < msglvl)
  {
    return;
  }

  rpccltDebugLogFileOpen();

  if (RPCCLT_FD_UNINIT == proc->logfd)
  {
    return;
  }

  curtime = time(NULL);
  num = strftime(timestamp, sizeof(timestamp)-1, "%Y-%m-%d %H:%M:%S", localtime(&curtime));
  timestamp[num] = '\0';

  /* a complete log message has timestamp, process ID, and ends with a newline */
  fd = proc->logfd;
  va_start(ap, fmt);
  fprintf(fd, "<%d> %s [%05d]: ", msglvl, timestamp, (int)proc->pid);
  vfprintf(fd, fmt, ap);
  fprintf(fd, "\n");
  fflush(fd);
  va_end(ap);

  /* close the log file after each message is written */
  rpccltDebugLogFileClose();
}

/* reads config file to obtain desired debug message level
 * (returns current message level if an error occurs)
 */
int rpccltDebugMsglvlCfgRead(void)
{
  rpccltProcParms_t *proc = rpccltProcPtrGet();
  FILE *fp;
  int8_t buf[RPCCLT_CFGPARM_MAX+1];
  int8_t name[RPCCLT_CFGPARM_MAX+1];
  int n, tmp;
  int val;

  val = rpccltDebugMsglvlGet();

  if ((strlen(proc->cfgfile) > 0) &&
      (strlen(proc->cfgmsglvlname) > 0))
  {
    if ((fp = fopen(proc->cfgfile, "r")) != NULL)
    {
      while (fgets(buf, sizeof(buf), fp) != NULL)
      {
        n = sscanf(buf, "%s %d", name, &tmp);
        if (n == 2)
        {
          if (strncmp(name, proc->cfgmsglvlname, sizeof(name)) == 0)
          {
            /* found the config entry in the file */
            val = tmp;
            break;
          }
        }
      } /* endwhile */

      fclose(fp);
    }
  }

  return val;
}

/* updates current debug message level from user-configured value */
void rpccltDebugMsglvlUpdate(void)
{
  int val;

  val = rpccltDebugMsglvlCfgRead();
  rpccltDebugMsglvlSet(val);
}

/* gets debug message level */
int rpccltDebugMsglvlGet(void)
{
  rpccltProcParms_t *proc = rpccltProcPtrGet();
  return proc->debugMsgLvl;
}

/* sets debug message level */
void rpccltDebugMsglvlSet(int msglvl)
{
  rpccltProcParms_t *proc = rpccltProcPtrGet();
  proc->debugMsgLvl = msglvl;
}

/* open the log file */
void rpccltDebugLogFileOpen(void)
{
  rpccltProcParms_t *proc = rpccltProcPtrGet();

  /* open the log file (if one was specified) */
  if (strlen(proc->logfile) > 0)
  {
    /* make sure file is closed so we don't orphan the old file descriptor */
    rpccltDebugLogFileClose();

    proc->logfd = fopen(proc->logfile, "a");
    if (NULL == proc->logfd)
    {
      proc->logfd = stderr;
    }
  }
}

/* close the log file */
void rpccltDebugLogFileClose(void)
{
  rpccltProcParms_t *proc = rpccltProcPtrGet();

  if (proc->logfd != RPCCLT_FD_UNINIT)
  {
    /* don't close stderr (let the OS do it) */
    if (stderr != proc->logfd)
    {
      fclose(proc->logfd);
    }
    proc->logfd = RPCCLT_FD_UNINIT;
  }
}
