/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
* ********************************************************************
*
* @filename   dot1ag_y1731pm.h
* @purpose    Y1731 PM
* @component  DOT1AG/Performance Management
* @comments   none
* @create     13-Dec-2013
*
***********************************************************************/
#ifndef INCLUDE_DOT1AG_Y1731_PM_H
#define INCLUDE_DOT1AG_Y1731_PM_H

#include "ofdpa_util.h"
#include "dot1ag_exports.h"
#include "dot1ag_proto.h"
#include "dot1ag_cnfgr.h"


typedef enum
{
  Y1731_BIN_FLAG_NONE   =  0x00,
  Y1731_BIN_FLAG_15M    =  0x01,
  Y1731_BIN_FLAG_24H    =  0x02,
} y1731BinModeFlags_t;

#define MAX_LM_INSTANCE OFDPA_DOT1AG_MAX_MEP_COUNT
#define MAX_DM_INSTANCE OFDPA_DOT1AG_MAX_MEP_COUNT


typedef struct y1731PmCFM_s
{
  /* Semaphores */
  void          *y1731PmMsgQSema;
  ofdpaRWlock_t  y1731PmCfgRWLock;
  /* Internal Event Message Queue */
  void          *y1731PmEventQueue;
  void          *y1731PmTaskId;
  timer_t        oneSecondTimer;
} y1731PmCFM_t;

extern y1731PmCFM_t y1731PmCFM;

extern y1731CfmMepProlmTable_t   y1731CfmMepProlmTable[OFDPA_DOT1AG_MAX_MEP_COUNT];
extern y1731CfmMepOndemlmTable_t y1731CfmMepOndemlmTable[OFDPA_DOT1AG_MAX_MEP_COUNT][8];
extern y1731CfmMepDmTable_t      y1731CfmMepDmTable[OFDPA_DOT1AG_MAX_MEP_COUNT][8];

#define Y1731PM_EVENT_MSG_COUNT  1024
#define Y1731PM_TASK_NAME        "y1731PMTask"
#define Y1731PM_QUEUE_EVENT      "y1731PmEventQueue"
typedef struct y1731PmMsg_s
{
  uint32_t event;
} y1731PmMsg_t;

typedef enum
{
  /***************************************************************/
  /* Events shared with all                                      */
  /***************************************************************/
  /*100*/y1731PmEventOneSecondTimer = 0,
} y1731PmEvents_t;

OFDPA_ERROR_t y1731PmThreadCreate(void);
void    y1731PmTask(void *arg);
void    y1731OneSecondTimerCallback(timer_t timerCtrlBlk, void* ptrData);
OFDPA_ERROR_t y1731PmUpdate();
void    y1731PmProcessLmBin(uint32_t curTimeStamp);
OFDPA_ERROR_t y1731PmGetLmBinPrim(uint16_t lmIdx, y1731CfmMepLmPrim *lmPrim, OFDPA_BOOL *isValid);
void    y1731PmGetLmBinEvent(OFDPA_BOOL isFrameValid, uint32_t totalFrame, uint32_t lostFrame, OFDPA_BOOL defectSecond,
                                uint32_t *frameLossRate, OFDPA_BOOL *valid);
void    y1731PmCalcLmBin(uint16_t lmIdx, uint8_t binMode, uint32_t N_FLR, OFDPA_BOOL N_valid, uint32_t N_TF, uint32_t N_LF,
                                uint32_t F_FLR, OFDPA_BOOL F_valid, uint32_t F_TF, uint32_t F_LF);
void    y1731PmMoveCurrentLmBintoHistory(uint16_t lmIdx, uint8_t binMode, uint32_t curTimeStamp);
void    y1731PmProcessDmBin(uint32_t curTimeStamp);
OFDPA_ERROR_t y1731PmGetDmBinPrim(uint16_t dmIdx, uint8_t priority, y1731CfmMepDmPrim *dmPrim, OFDPA_BOOL *isValid);
void    y1731PmGetDmBinEvent(y1731CfmMepDmPrim *dmPrim, uint32_t *N_FD, uint32_t *F_FD, uint32_t *B_FD, OFDPA_BOOL *isValid);
void    y1731PmCalcDmBin(uint16_t dmIdx, uint8_t priority, uint8_t binMode,
                                uint32_t N_FD, uint32_t F_FD, uint32_t B_FD);
void    y1731PmCalcDmFDV(uint32_t FD_pre, uint32_t FD, uint32_t *FDV);
void    y1731PmMoveCurrentDmBintoHistory(uint16_t dmIdx, uint8_t priority,
                                uint8_t binMode, uint32_t curTimeStamp);
OFDPA_ERROR_t y1731PmCheckTimeIsFixedValue(uint8_t binMode, uint32_t timeStamp, OFDPA_BOOL *isFixed);
void    y1731PmCalcLoopRange(OFDPA_BOOL is15mBinEnabled, OFDPA_BOOL is24hBinEnabled,
                                uint8_t *start, uint8_t *end);
void    y1731PmClearCurrentLmBin(uint16_t lmIdx, uint8_t binMode);
void    y1731PmClearCurrentDmBin(uint16_t dmIdx, uint8_t priority, uint8_t binMode);
OFDPA_ERROR_t y1731PmDumpLm(uint32_t intIfNum, uint32_t mdIndex,
                        uint32_t maIndex, uint32_t mepId,
                        uint32_t priorityClass, uint8_t binMode);
OFDPA_ERROR_t y1731PmDumpDm(uint32_t intIfNum, uint32_t mdIndex,
                        uint32_t maIndex, uint32_t mepId,
                        uint32_t pri, uint8_t binMode);
OFDPA_ERROR_t y1731PmGetProLmCounters(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex, uint32_t mepId, y1731CfmMepProlmTable_t *prolmEntry);
OFDPA_ERROR_t y1731PmGetProDmCounters(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex, uint32_t mepId, uint32_t pri, y1731CfmMepDmTable_t *dmEntry);
OFDPA_ERROR_t y1731PmGetOdLmCounters(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex, uint32_t mepId, uint32_t priorityClass, y1731CfmMepOndemlmTable_t *odlmEntry);
OFDPA_ERROR_t y1731PmGetOdDmCounters(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex, uint32_t mepId, uint32_t pri, y1731CfmMepDmTable_t *dmEntry);
#endif
