/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2015
*
**********************************************************************
*
* @filename  ofdpa_util.h
*
* @purpose      Utility functions
*
* @component    OFDPA
*
* @comments
*
* @create    5-Apr-2013
*
* @end
*
**********************************************************************/

#ifndef _INCLUDE_OFDPA_UTIL_H
#define _INCLUDE_OFDPA_UTIL_H

#include <stdint.h>
#include <stdarg.h>
#include <sys/time.h>
#include <pthread.h>
#include "ofdpa_datatypes.h"
#include <stdio.h>

#define TIMER_TICK_1000MSEC 1000
#define TIMER_TICK_100MSEC 100
#define TIMER_TICK_1MSEC   1

/* Relocate these */
#define OFDPA_NOT_SUPPORTED 0

#define OFDPA_MAX_VLANS 4095

typedef struct ofdpa_enetHeader_s
{
  ofdpaMacAddr_t dest;
  ofdpaMacAddr_t src;
} ofdpa_enetHeader_t;

#define OFDPA_ENET_HDR_SIZE    sizeof(ofdpa_enetHeader_t)

/* Ethernet Tagged */

typedef struct ofdpa_enetTag_s
{
  uint16_t tpid;                        /* Tag Protocol ID - 0x8100 if 802.1Q Tagged */
  uint16_t tci;                         /* Tag Control Information */
} ofdpa_8021QTag_t;

#define OFDPA_8021Q_VLAN_PROTOCOL_ID                 0x8100

#define OFDPA_8021Q_ENCAPS_HDR_SIZE    sizeof(ofdpa_8021QTag_t)

typedef enum
{
  PERIODIC_MODE = 0,
  NON_PERIODIC_MODE = 1,
} TIMER_MODE;

typedef enum
{
  VLAN_ADD_PORT_NOTIFY = 0,
  VLAN_DELETE_PORT_NOTIFY = 1,
} VLAN_PORT_NOTIFIY_t;

typedef struct ofdpaRWlock_s
{
  pthread_rwlock_t lock;
} ofdpaRWlock_t;

typedef struct ofdpaRWlockAttr_s
{
  pthread_rwlockattr_t attr;
} ofdpaRWlockAttr_t;

extern const char *versionBuf;

typedef struct ofdpaTimespec_t
{
  uint32_t days;
  uint32_t hours;
  uint32_t minutes;
  uint32_t seconds;
} ofdpaTimespec;

#define DPA_SEM_FOREVER   (-1)
#define DPA_SEM_BINARY    1
#define DPA_SEM_COUNTING  0

/* Interface Mask macros */
/* Number of entries per mask byte */
#define OFDPA_INTF_MASK_UNIT               (sizeof(uint8_t) * 8)

/* Number of bytes in mask */
#define OFDPA_INTF_INDICES   ((100) / OFDPA_INTF_MASK_UNIT + 1)

/* Interface storage */
typedef struct
{
  uint8_t   value[OFDPA_INTF_INDICES];
} OFDPA_INTF_MASK_t;

/* ISMASKBITSET returns 0 if the interface k is not set in mask j */
#define OFDPA_INTF_ISMASKBITSET(j, k)                              \
        ((j).value[((k-1)/(8*sizeof(uint8_t)))]                    \
                         & ( 1 << ((k-1) % (8*sizeof(uint8_t)))) )

/* SETMASKBIT turns on bit index # k in mask j. */
#define OFDPA_INTF_SETMASKBIT(j, k)                                \
            ((j).value[((k-1)/(8*sizeof(uint8_t)))]                \
                         |= 1 << ((k-1) % (8*sizeof(uint8_t))))


/* CLRMASKBIT turns off bit index # k in mask j. */
#define OFDPA_INTF_CLRMASKBIT(j, k)                                \
           ((j).value[((k-1)/(8*sizeof(uint8_t)))]                 \
                        &= ~(1 << ((k-1) % (8*sizeof(uint8_t)))))


/* CRC  algorithm Types */
typedef enum
{
  CRC_16,     /* Default CRC          */
  CRC_CCITT,  /* CRC_CCITT LSB First  */
  CRC_32,     /* 32 bit CRC           */
} CRC_TYPE_t;


/*
Warning 506: Constant value Boolean
Info 774: Boolean within 'if' always evaluates to True
Info 737: Loss of sign in promotion from int to unsigned int
*/

#define OFDPA_DEBUG_PRINTF(__component, __verbosity, __format, ...)  ofdpaDebugPrintf(__FUNCTION__, __component, __verbosity, __format,##__VA_ARGS__);




uint32_t dpaUpTimeSeconds(void);
uint32_t dpaUpTimeMilliSeconds(void);
void dpaUpTime(ofdpaTimespec *ts);

void *dpaSemCreate(char *desc, int binary, int initial_count);
void dpaSemDestroy(void *sem);
int dpaSemTake(void *sem, int usec);
int dpaSemGive(void *sem);

void *dpaThreadCreate(char *name,  int prio, void (f)(void *), void *arg);
void dpaThreadExit(int rc);

int ofdpaDebugPrintf(const char *functionName, ofdpaComponentIds_t component, ofdpaDebugLevels_t verbosity, const char *format, ...);
int ofdpaDebugvPrintf(const char *functionName, ofdpaComponentIds_t component, ofdpaDebugLevels_t verbosity, const char *format, va_list ap);

const char *ofdpaDebugComponentNameGet(ofdpaComponentIds_t component);
int ofdpaDebugComponentEnable(ofdpaComponentIds_t component);
int ofdpaDebugComponentIsEnabled(ofdpaComponentIds_t component);
int ofdpaDebugComponentReset(ofdpaComponentIds_t component);
void ofdpaDebugComponentResetAll(void);

int ofdpaDebugVerbositySet(ofdpaDebugLevels_t verbosity);
ofdpaDebugLevels_t ofdpaDebugVerbosityGet(void);

char *ofdpaMacAddrToString(ofdpaMacAddr_t *mac, char *buffer);

/* logging priorities are defined in <sys/syslog.h> */

int ofdpaLog(int priority, const char *format, ...);
int ofdpavLog(int priority, const char *format, va_list ap);

/* Timer APIs */
OFDPA_ERROR_t dpaTimerAdd(void *handler, timer_t *timerid, int timerduration, int periodic, void *param);
OFDPA_ERROR_t dpaTimerDelete(timer_t timerid);
OFDPA_ERROR_t dpaTimerSet(timer_t timerid, int timerduration, int periodic);
OFDPA_ERROR_t dpaTimerGet(timer_t timerid, int *remaining, int *interval);

/* Read-Write lock APIs */
OFDPA_ERROR_t dpaRWlockInit(ofdpaRWlock_t *rwlock, const ofdpaRWlockAttr_t *attr);
OFDPA_ERROR_t dpaRWlockDestroy(ofdpaRWlock_t *rwlock);
OFDPA_ERROR_t dpaRWlockReadlock(ofdpaRWlock_t *rwlock);
OFDPA_ERROR_t dpaRWlockWritelock(ofdpaRWlock_t *rwlock);
OFDPA_ERROR_t dpaRWlockUnlock(ofdpaRWlock_t *rwlock);

uint64_t dpaHtonll(uint64_t hostlong);
uint64_t dpaNtohll(uint64_t netlong);

uint32_t dpaDataOffsetGet(uint8_t *data);

OFDPA_ERROR_t crcCalculate(uint32_t startingCrc, uint8_t *data, int len, CRC_TYPE_t type, uint32_t *endingCrc);

#endif  /* _INCLUDE_OFDPA_UTIL_H */
