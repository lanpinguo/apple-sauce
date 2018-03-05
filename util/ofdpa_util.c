/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     ofdpa_util.c
*
* @purpose      Utility functions
*
* @component    OFDPA
*
* @comments     none
*
* @create       04/05/2013
*
* @end
*
**********************************************************************/
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include "sal_thread.h"
#include "sync.h"
//#include "shared/util.h"
#include "ofdpa_util.h"

/*
 * Note: per component debug levels are not currently implemented.
 */
static struct
{
  ofdpaComponentIds_t compId;
  char               *compName;
  int                 compDebugEnabled;
  ofdpaDebugLevels_t  compDebugVerbosity;
} ofdpaComponents[] =
{
  { OFDPA_COMPONENT_API,         "API",      0, OFDPA_DEBUG_ALWAYS },
  { OFDPA_COMPONENT_MAPPING,     "Mapping",  0, OFDPA_DEBUG_ALWAYS },
  { OFDPA_COMPONENT_RPC,         "RPC",      0, OFDPA_DEBUG_ALWAYS },
  { OFDPA_COMPONENT_OFDB,        "OFDB",     0, OFDPA_DEBUG_ALWAYS },
  { OFDPA_COMPONENT_DATAPATH,    "Datapath", 0, OFDPA_DEBUG_ALWAYS },
  { OFDPA_COMPONENT_G8131,       "G8131",    0, OFDPA_DEBUG_ALWAYS },
  { OFDPA_COMPONENT_Y1731,       "Y1731",    0, OFDPA_DEBUG_ALWAYS },
};

static ofdpaDebugLevels_t debugVerbosity = OFDPA_DEBUG_ALWAYS; /* Default to the minimum level. */

#define SECONDS_PER_DAY        86400
#define SECONDS_PER_HOUR       3600
#define SECONDS_PER_MINUTE     60

static
void _dpaConvertRawUpTime(uint32_t rawTime, ofdpaTimespec *ts)
{
  ts->days    = rawTime / SECONDS_PER_DAY;
  rawTime     = rawTime % SECONDS_PER_DAY;
  ts->hours   = rawTime / SECONDS_PER_HOUR;
  rawTime     = rawTime % SECONDS_PER_HOUR;
  ts->minutes = rawTime / SECONDS_PER_MINUTE;
  rawTime     = rawTime % SECONDS_PER_MINUTE;
  ts->seconds = rawTime;
}

/**************************************************************************
*
* @purpose  Retrieve number of seconds since last reset
*
* @param    void
*
* @returns  rawtime
*
* @comments    none.
*
* @end
*
*************************************************************************/
uint32_t dpaUpTimeSeconds(void)
{
  struct timespec tp;
  int rc;

  rc = clock_gettime(CLOCK_MONOTONIC, &tp);
  if (rc < 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                       "Error returned from clock_gettime(): rc = %d\r\n",
                       rc);
    memset(&tp, 0, sizeof(tp));
  }
  return(tp.tv_sec);
}

/**************************************************************************
*
* @purpose  Retrieve number of milliseconds since last reset
*
* @param    void
*
* @returns  rawtime
*
* @comments    none.
*
* @end
*
*************************************************************************/
uint32_t dpaUpTimeMilliSeconds(void)
{
  struct timespec tp;
  int rc;

  rc = clock_gettime(CLOCK_MONOTONIC, &tp);
  if (rc < 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                       "Error returned from clock_gettime(): rc = %d\r\n",
                       rc);
    memset(&tp, 0, sizeof(tp));
  }
  return(( tp.tv_sec * 1000 ) + ( tp.tv_nsec / 1000000 ));
}

/**************************************************************************
*
* @purpose  Retrieve number of seconds since last reset and return in number
*           of days, hours, minutes, seconds
*
* @param    ts @b{(input)}    ptr to ofdpaTimespec structure
*
* @returns  void
*
* @comments    none.
*
* @end
*
*************************************************************************/
void dpaUpTime(ofdpaTimespec *ts)
{
  uint32_t rawTime;

  rawTime = dpaUpTimeSeconds();

  _dpaConvertRawUpTime(rawTime, ts);
}

void *dpaSemCreate(char *desc, int binary, int initial_count)
{
  return(sal_sem_create(desc, binary, initial_count));
}

void dpaSemDestroy(void *sem)
{
  sal_sem_destroy(sem);
}

int dpaSemTake(void *sem, int usec)
{
  return(sal_sem_take(sem, usec));
}

int dpaSemGive(void *sem)
{
  return(sal_sem_give(sem));
}

/*
 * Function: dpaThreadCreate
 *
 * Purpose:
 *    Abstraction for task creation.
 * Parameters:
 *    name - name of task
 *    func - address of function to call
 *    arg - argument passed to func.
 * Returns:
 *    Pointer to opaque thread identifier.
 * Notes:
 *    The stack size is fixed at the Linux kernel stacksize (8KB)
 */
void *dpaThreadCreate(char *name,  int prio, void (f)(void *), void *arg)
{
  return(sal_thread_create(name, SAL_THREAD_STKSZ, prio, f, arg));
}

/*
 * Function: dpaThreadExit
 *
 * Purpose:
 *    Exit the calling thread
 * Parameters:
 *    rc - return code from thread.
 * Notes:
 *    Never returns.
 */
void dpaThreadExit(int rc)
{
  sal_thread_exit(rc);
}

/*
 * Debugging output functions
 */

int ofdpaFlushedPrintf(const char *format, va_list ap)
{
  int rc = vfprintf(stdout, format, ap);
  fflush(stdout);
  return rc;
}

int ofdpaDebugPrintf(const char *functionName, ofdpaComponentIds_t component, ofdpaDebugLevels_t verbosity, const char *format, ...)
{
  va_list ap;
  int     rc;

  va_start(ap, format);
  rc = ofdpaDebugvPrintf(functionName, component, verbosity, format, ap);
  va_end(ap);

  return rc;
}

int ofdpaDebugvPrintf(const char *functionName, ofdpaComponentIds_t component, ofdpaDebugLevels_t verbosity, const char *format, va_list ap)
{
  unsigned char newFormat[400];

  if ((OFDPA_DEBUG_ALWAYS == verbosity) ||
      ((ofdpaDebugComponentIsEnabled(component)) &&
       (verbosity <= ofdpaDebugVerbosityGet())))
  {
    if (sizeof(newFormat) == snprintf(newFormat, sizeof(newFormat), "%s: %s", functionName, format))
    {
      newFormat[sizeof(newFormat) - 1] = '\0';
    }
    return ofdpaFlushedPrintf(newFormat, ap);
  }

  return 0;
}

static int ofdpaComponentIndexGet(ofdpaComponentIds_t component)
{
  int i;

  if ((component >= OFDPA_COMPONENT_FIRST) &&
      (component <  OFDPA_COMPONENT_MAX))
  {
    for (i = 0; i < (sizeof(ofdpaComponents) / sizeof(ofdpaComponents[0])); i++)
    {
      if (ofdpaComponents[i].compId == component)
      {
        return i;
      }
    }
  }

  return -1;
}

const char *ofdpaDebugComponentNameGet(ofdpaComponentIds_t component)
{
  int index = ofdpaComponentIndexGet(component);

  if (index < 0)
  {
    return NULL;
  }
  else
  {
    return ofdpaComponents[index].compName;
  }
}

int ofdpaDebugComponentEnable(ofdpaComponentIds_t component)
{
  int index = ofdpaComponentIndexGet(component);

  if (index < 0)
  {
    return 1;
  }

  ofdpaComponents[index].compDebugEnabled = 1;

  return 0;
}

int ofdpaDebugComponentDisable(ofdpaComponentIds_t component)
{
  int index = ofdpaComponentIndexGet(component);

  if (index < 0)
  {
    return 1;
  }

  ofdpaComponents[index].compDebugEnabled = 0;

  return 0;
}

void level2DebugInfoEnable(void)
{
	unsigned char j;

	for (j = OFDPA_COMPONENT_FIRST; j < OFDPA_COMPONENT_MAX; j++)
	{
		ofdpaDebugComponentEnable(j);
	}

	(void)ofdpaDebugVerbositySet(OFDPA_DEBUG_VERBOSE);
}

int ofdpaDebugComponentIsEnabled(ofdpaComponentIds_t component)
{
  int index = ofdpaComponentIndexGet(component);

  if (index < 0)
  {
    return 0;
  }

  return ofdpaComponents[index].compDebugEnabled;
}

int ofdpaDebugComponentReset(ofdpaComponentIds_t component)
{
  int index = ofdpaComponentIndexGet(component);

  if (index < 0)
  {
    return 1;
  }

  ofdpaComponents[index].compDebugEnabled = 0;

  return 0;
}

void ofdpaDebugComponentResetAll(void)
{
  int i;

  for (i = 0; i < (sizeof(ofdpaComponents) / sizeof(ofdpaComponents[0])); i++)
  {
    ofdpaComponents[i].compDebugEnabled = 0;
  }
}

int ofdpaDebugVerbositySet(ofdpaDebugLevels_t verbosity)
{

  if (verbosity >= OFDPA_DEBUG_MAX)
  {
    return 1;
  }
  /* OK to set verbosity higher than the max. */
  debugVerbosity = verbosity;

  return 0;
}

ofdpaDebugLevels_t ofdpaDebugVerbosityGet(void)
{
  return debugVerbosity;
}


/*
 * Logging functions.
 */

int ofdpaLog(int priority, const char *format, ...)
{
  va_list ap;
  int     rc;

  va_start(ap, format);
  rc = ofdpavLog(priority, format, ap);
  va_end(ap);

  return rc;
}

int ofdpavLog(int priority, const char *format, va_list ap)
{
  return ofdpaFlushedPrintf(format, ap);
}

char *ofdpaMacAddrToString(ofdpaMacAddr_t *mac, char *buffer)
{
  if ((mac == NULL) || (buffer == NULL))
  {
    return(NULL);
  }

  sprintf(buffer, "%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
          mac->addr[0], mac->addr[1], mac->addr[2],
          mac->addr[3], mac->addr[4], mac->addr[5]);

  return(buffer);
}

/* This define can be adjusted to modify the units of the timer functions.  Currently it
 * is milliseconds. */
#define TICKS_PER_SECOND 1000

/*********************************************************************
* purpose  function used to create and arm a timer
*
*
* param    handler   function pointer to handle the callback
* param    timerid   timerid of the timer which is created
* param    timeinsec time after which callback is required
* param    periodic  mode specifing if the timer must be
*                    periodic or oneshot
* param    param     arguments passed from the calling function
*
* return   OFDPA_E_NONE
* return   OFDPA_E_FAIL
*
* @end
*********************************************************************/
OFDPA_ERROR_t dpaTimerAdd(void *handler, timer_t *timerid, int timerduration, int periodic, void *param)
{
  struct sigevent sigtime;
  struct timespec monotonic_time;
  clockid_t       clock;
  int             ret = OFDPA_E_NONE;

  /* populate the structures in-order to create the timer */
  memset(&sigtime, 0, sizeof(sigtime));
  sigtime.sigev_notify = SIGEV_THREAD;
  sigtime.sigev_notify_attributes = NULL;
  sigtime.sigev_notify_function = handler;
  sigtime.sigev_value.sival_ptr = param;
  /* monotonic clock is used if available else realtime clock is used */
  if (!clock_gettime(CLOCK_MONOTONIC, &monotonic_time))
  {
    clock = CLOCK_MONOTONIC;
  }
  else
  {
    clock = CLOCK_REALTIME;
  }

  /* timer is created */
  if (timer_create(clock, &sigtime, timerid) == 0)
  {
    ret = dpaTimerSet(*timerid, timerduration, (periodic ? timerduration : 0));
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                       "Timer creation Error\r\n", 0);
    ret = OFDPA_E_FAIL;
  }
  return ret;
}

/*********************************************************************
* purpose  function used to destroy a timer
*
*
* param    timerid timer id of the timer which needs to be destroyed
*
* return   OFDPA_E_NONE
* return   OFDPA_E_FAIL
*
* @end
*********************************************************************/
OFDPA_ERROR_t dpaTimerDelete(timer_t timerid)
{
  OFDPA_ERROR_t ret = OFDPA_E_NONE;
  if (!(timer_delete(timerid)))
  {
    ret = OFDPA_E_FAIL;
  }
  return ret;
}

/*********************************************************************
* purpose  function used to set/reset a timer
*
*
* param    timerid       - timer id of the timer which needs to be set/reset
* param    timerduration - time after which callback is required
* param    interval      - interval between timer ticks
*
* return   OFDPA_E_NONE
* return   OFDPA_E_FAIL
*
* @end
*********************************************************************/
OFDPA_ERROR_t dpaTimerSet(timer_t timerid, int timerduration, int interval)
{
  OFDPA_ERROR_t     ret = OFDPA_E_NONE;
  struct itimerspec timerval;

  timerval.it_value.tv_sec  = timerduration / TICKS_PER_SECOND;
  timerval.it_value.tv_nsec = (long)(timerduration % TICKS_PER_SECOND) * (1000000000L / TICKS_PER_SECOND);

  if (interval) /* timer is triggered periodically */
  {
    timerval.it_interval.tv_sec  = interval / TICKS_PER_SECOND;
    timerval.it_interval.tv_nsec = (long)(interval % TICKS_PER_SECOND) * (1000000000L / TICKS_PER_SECOND);
  }
  else /* timer is triggered only once, but timer is not destroyed */
  {
    timerval.it_interval.tv_sec  = 0;
    timerval.it_interval.tv_nsec = 0;
  }

  if (timer_settime(timerid, 0, &timerval, NULL) != 0)
  {
    perror("time_settime error!");
    ret = OFDPA_E_FAIL;
  }
  return ret;
}

/*********************************************************************
* purpose  function used to get a timer
*
*
* param    timerid   - timer id of the timer
* param    remaining - time until next callback
* param    interval  - interval between timer ticks
*
* return   OFDPA_E_NONE
* return   OFDPA_E_FAIL
*
* @end
*********************************************************************/
OFDPA_ERROR_t dpaTimerGet(timer_t timerid, int *remaining, int *interval)
{
  struct itimerspec curr_value;

  if (0 != timer_gettime(timerid, &curr_value))
  {
    return OFDPA_E_FAIL;
  }

  *remaining = (curr_value.it_value.tv_sec    * TICKS_PER_SECOND) + (curr_value.it_value.tv_nsec    / (1000000000L / TICKS_PER_SECOND));
  *interval  = (curr_value.it_interval.tv_sec * TICKS_PER_SECOND) + (curr_value.it_interval.tv_nsec / (1000000000L / TICKS_PER_SECOND));

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpaRWlockInit(ofdpaRWlock_t *rwlock, const ofdpaRWlockAttr_t *rwlockattr)
{
  int rv = 0;
  if (NULL == rwlock)
  {
    return OFDPA_E_PARAM;
  }

  if (NULL == rwlockattr)
  {
    rv = pthread_rwlock_init(&(rwlock->lock), NULL);
  }
  else
  {
    rv = pthread_rwlock_init(&(rwlock->lock), &(rwlockattr->attr));
  }

  if (rv)
  {
    return OFDPA_E_ERROR;
  }
  else
  {
    return OFDPA_E_NONE;
  }
}

OFDPA_ERROR_t dpaRWlockDestroy(ofdpaRWlock_t *rwlock)
{
  int rv = 0;

  if (NULL == rwlock)
  {
    return OFDPA_E_PARAM;
  }

  rv = pthread_rwlock_destroy(&(rwlock->lock));

  if (rv)
  {
    return OFDPA_E_ERROR;
  }
  else
  {
    return OFDPA_E_NONE;
  }
}

OFDPA_ERROR_t dpaRWlockReadlock(ofdpaRWlock_t *rwlock)
{
  int rv = 0;

  if (NULL == rwlock)
  {
    return OFDPA_E_PARAM;
  }

  rv = pthread_rwlock_rdlock(&(rwlock->lock));

  if (rv)
  {
    return OFDPA_E_ERROR;
  }
  else
  {
    return OFDPA_E_NONE;
  }
}

OFDPA_ERROR_t dpaRWlockWritelock(ofdpaRWlock_t *rwlock)
{
  int rv = 0;

  if (NULL == rwlock)
  {
    return OFDPA_E_PARAM;
  }

  rv = pthread_rwlock_wrlock(&(rwlock->lock));

  if (rv)
  {
    return OFDPA_E_ERROR;
  }
  else
  {
    return OFDPA_E_NONE;
  }
}

OFDPA_ERROR_t dpaRWlockUnlock(ofdpaRWlock_t *rwlock)
{
  int rv = 0;

  if (NULL == rwlock)
  {
    return OFDPA_E_PARAM;
  }

  rv = pthread_rwlock_unlock(&(rwlock->lock));

  if (rv)
  {
    return OFDPA_E_ERROR;
  }
  else
  {
    return OFDPA_E_NONE;
  }
}

uint64_t dpaHtonll(uint64_t hostlong)
{
  uint64_t rval;
  uint8_t *data = (uint8_t *)&rval;

  data[0] = hostlong >> 56;
  data[1] = hostlong >> 48;
  data[2] = hostlong >> 40;
  data[3] = hostlong >> 32;
  data[4] = hostlong >> 24;
  data[5] = hostlong >> 16;
  data[6] = hostlong >> 8;
  data[7] = hostlong >> 0;

  return rval;
}

uint64_t dpaNtohll(uint64_t netlong)
{
  return dpaHtonll(netlong);
}

/*********************************************************************
* @purpose  calculate where the actual data packet header starts devoid of L2 headers
*
* @param    *data       @b{(input)} Pointer to pdu that is received
*
* @returns  offset      @b{(output)} Number of bytes of header
*
* @notes    check if packet is vlan tagged or LLC&SNAP or ethernet enacpsulations
* @notes                offset is dst(6)+src(6)+encaphrd(x)+protocoltype(2)
* @notes    vlan tagged offset is 6+6+4+2 = 18
* @notes    LLC&SNAP    offset is 6+6+8+2 = 22
* @notes    Ether encap offset is 6+6+0+2 = 14
*
* @end
*********************************************************************/
uint32_t dpaDataOffsetGet(uint8_t *data)
{
  uint16_t  protocol_type;
  uint8_t  *pEtype;
  uint32_t  offset = OFDPA_ENET_HDR_SIZE;
  uint16_t  temp16;

  /* Determine the protocol type */
  pEtype = data + OFDPA_ENET_HDR_SIZE;
  memcpy(&protocol_type, pEtype, sizeof(protocol_type)); /* Endian */

  temp16 = ntohs(protocol_type);
  protocol_type = temp16;
  while ((OFDPA_8021Q_VLAN_PROTOCOL_ID == protocol_type) ||
         (0x88a8                       == protocol_type))
  {
    /* This is an Vlan tagged Frame -- skip over tag */
    pEtype += OFDPA_8021Q_ENCAPS_HDR_SIZE;
    memcpy(&protocol_type, pEtype, sizeof(protocol_type)); /* Endian */

    temp16 = ntohs(protocol_type);
    protocol_type = temp16;

    offset += 4;
  }

  if (protocol_type <= 1500)
  {
    /* This is a LLC & SNAP Encapsulated Ethernet Frame */
    pEtype += 2;                        /* skip over length field */
    offset += 2;
    if (0x3 == (pEtype[2] & 0x3))       /* if unnumbered LLC, header is 3 bytes */
    {
      offset += 3;
    }
    else                                /* all other LLC headers are 4 bytes */
    {
      offset += 4;
    }
    if (((0xAA == pEtype[0]) ||         /* If SNAP */
         (0xAB == pEtype[0])) &&
        ((0xAA == pEtype[1]) ||
         (0xAB == pEtype[1])))
    {
      offset += 5;                      /* SNAP is 5 bytes */
    }
  }
  else
  {
    /* This is Ethernet encapsulation RFC894 -- skip over Ethertype */
    offset += 2;
  }
  return offset;
}

OFDPA_ERROR_t crcCalculate(uint32_t startingCrc, uint8_t *data, int len, CRC_TYPE_t type, uint32_t *endingCrc)
{
  /*TODO*/
  return OFDPA_E_NONE;
}
