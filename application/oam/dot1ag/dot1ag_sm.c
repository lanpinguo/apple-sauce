/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2000-2016
*
**********************************************************************
*
* @filename     dot1ag_sm.c
*
* @purpose      File for dot1ag state machine events
*
* @component    DOT1AG
*
* @comments
*
* @create       Feb'16th, 2009
*
* @end
*
**********************************************************************/

#include <stdlib.h>
#include "ofdb_api.h"
#include "dot1ag_api.h"
#include "dot1ag_exports.h"
#include "dot1ag_sm.h"
#include "dot1ag_y1731pm.h"
#include "dot1ag.h"
#include "dot1ag_pdu.h"
#include "dot1ag_procs.h"
#include "dot1ag_debug.h"
#include "dot1ag_ctrl.h"
#include "g8131_api.h"

extern uint32_t dot1agMicrosecondsGet(void);
extern uint32_t dot1agCentisecondsGet(void);

static OFDPA_ERROR_t dot1agCfmMepCCIStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmMepCCRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmRMepStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmRMepErrStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmMepXConStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmLBIStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmLBIRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmLBRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmLTIRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmLTRRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmLTRTStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t dot1agCfmMepFNGStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmProLMStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmOndemLMMStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmLMRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmDMMStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmDMRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmMepAISSendStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmMepAISRcvStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmMepLCKSendStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmMepLCKRcvStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmMepCSFSendStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmMepCSFRcvStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg);
static OFDPA_ERROR_t y1731CfmMepDefectStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg, y1731_defect_type_t defType);

void dot1agCfmCciTimerCallback(void *handle);
void dot1agCfmRmepTimerCallback(void *handle);
void dot1agCfmRmepErrTimerCallback(void *handle);
void dot1agCfmXConCCMTimerCallback(void *handle);
void dot1agCfmLbiTxTimerCallback(void *handle);
void dot1agCfmLbiTimerCallback(void *handle);
void dot1agCfmLtfTimerCallback(void *handle);
void dot1agCfmFngTimerCallback(void *handle);

void y1731CfmProlmTimerCallback(void *handle);
void y1731CfmOndemlmTimerCallback(void *handle);
void y1731CfmDmmTimerCallback(void *handle);
void y1731CfmAisSendTimerCallback(void *handle);
void y1731CfmAisRcvTimerCallback(void *handle);
void y1731CfmLckSendTimerCallback(void *handle);
void y1731CfmLckRcvTimerCallback(void *handle);
void y1731CfmCsfSendTimerCallback(void *handle);
void y1731CfmCsfRcvTimerCallback(void *handle);
void y1731DefectUNMTimerCallback(void *handle);
void y1731DefectUNPTimerCallback(void *handle);
void y1731DefectMMGTimerCallback(void *handle);
void y1731DefectUNLTimerCallback(void *handle);
void y1731DefectUNPTRTimerCallback(void *handle);

/* CFM Service level Connectivity Check Initiator state Machine */
dot1agMEPCciStates_t  dot1agSMCCITable[DOT1AG_MEP_CCI_EVENTS_MAX][DOT1AG_MEP_CCI_DEFAULT] =
{
/*Events*/               /*DOT1AG_MEP_CCI_IDLE*/  /*DOT1AG_MEP_CCI_WAITING*/
/*CCIBegin*/            { DOT1AG_MEP_CCI_IDLE,     DOT1AG_MEP_CCI_DEFAULT},
/*CCINotMepActive*/     { DOT1AG_MEP_CCI_IDLE,     DOT1AG_MEP_CCI_IDLE},
/*CCIEnabled*/          { DOT1AG_MEP_CCI_WAITING,  DOT1AG_MEP_CCI_DEFAULT},
/*CCIMacStatusChanged*/ { DOT1AG_MEP_CCI_IDLE,     DOT1AG_MEP_CCI_WAITING},
/*CCIWhileZero*/        { DOT1AG_MEP_CCI_DEFAULT,  DOT1AG_MEP_CCI_WAITING},
/*CCINotEnabled*/       { DOT1AG_MEP_CCI_DEFAULT,  DOT1AG_MEP_CCI_IDLE}
};

/* CFM Service level MEP Connectivity Check Receiver state Machine */
dot1agMEPCcrStates_t  dot1agSMMepCCRTable[DOT1AG_MEP_CCR_EVENTS_MAX][DOT1AG_MEP_CCR_DEFAULT] =
{
/*Events*/          /*CCR_RESET*/            /*CCR_IDLE*/                  /*RECORD_EQUAL*/        /*RECORD_LOW*/
/*CCRBegin*/       {DOT1AG_MEP_CCR_RESET,   DOT1AG_MEP_CCR_DEFAULT,      DOT1AG_MEP_CCR_DEFAULT, DOT1AG_MEP_CCR_DEFAULT},
/*CCRNotMepActive*/{DOT1AG_MEP_CCR_RESET,   DOT1AG_MEP_CCR_RESET,        DOT1AG_MEP_CCR_RESET,   DOT1AG_MEP_CCR_RESET},
/*CCRCCMRcvdEqual*/{DOT1AG_MEP_CCR_DEFAULT, DOT1AG_MEP_CCR_RECORD_EQUAL, DOT1AG_MEP_CCR_DEFAULT, DOT1AG_MEP_CCR_DEFAULT},
/*CCRCCMRcvdLow*/  {DOT1AG_MEP_CCR_DEFAULT, DOT1AG_MEP_CCR_RECORD_LOW,   DOT1AG_MEP_CCR_DEFAULT, DOT1AG_MEP_CCR_DEFAULT}
};

/* CFM Service level RMEP state Machine */
dot1agRmepStates_t  dot1agSMRmepTable[DOT1AG_RMEP_EVENTS_MAX][DOT1AG_RMEP_DEFAULT] =
{
/*Events*/               /*RMEP_IDLE*/        /*RMEP_START*/        /*RMEP_OK*/          /*RMEP_FAILED*/
/*RmepBegin*/           {DOT1AG_RMEP_IDLE,    DOT1AG_RMEP_DEFAULT,  DOT1AG_RMEP_DEFAULT, DOT1AG_RMEP_DEFAULT},
/*RmepNotMepActive*/    {DOT1AG_RMEP_IDLE,    DOT1AG_RMEP_IDLE,     DOT1AG_RMEP_IDLE,    DOT1AG_RMEP_IDLE},
/*dot1agRmepNot
EnableRmepDefect*/      {DOT1AG_RMEP_IDLE,    DOT1AG_RMEP_IDLE,     DOT1AG_RMEP_IDLE,    DOT1AG_RMEP_IDLE},
/*dot1agRmeprCCMRecvd */{DOT1AG_RMEP_DEFAULT, DOT1AG_RMEP_DEFAULT,  DOT1AG_RMEP_DEFAULT, DOT1AG_RMEP_OK     },
/*RmeprCCMRecvdWhile
NotZero */              {DOT1AG_RMEP_DEFAULT, DOT1AG_RMEP_OK,       DOT1AG_RMEP_OK,      DOT1AG_RMEP_OK},
/*RmepWhileZero*/       {DOT1AG_RMEP_DEFAULT, DOT1AG_RMEP_FAILED,   DOT1AG_RMEP_FAILED,  DOT1AG_RMEP_DEFAULT}
};

/* CFM Service level RMEP Error state Machine */
dot1agRmepErrStates_t  dot1agSMRmepErrTable[DOT1AG_RMEP_ERR_EVENTS_MAX][DOT1AG_RMEP_ERRCCM_DEFAULT] =
{
/*Events*/               /*RMEP_ERRCCM_IDLE*/        /*RMEP_ERRCCM_NO_DEFECT*/    /*RMEP_ERRCCM_DEFECT*/
/*RmepErrBegin*/        {DOT1AG_RMEP_ERRCCM_IDLE,    DOT1AG_RMEP_ERRCCM_DEFAULT, DOT1AG_RMEP_ERRCCM_DEFAULT},
/*RmepErrNotMepActive*/ {DOT1AG_RMEP_ERRCCM_IDLE,    DOT1AG_RMEP_ERRCCM_IDLE, DOT1AG_RMEP_ERRCCM_IDLE},
/*RmepErrCCMRecvd*/     {DOT1AG_RMEP_ERRCCM_DEFAULT, DOT1AG_RMEP_ERRCCM_DEFECT,  DOT1AG_RMEP_ERRCCM_DEFAULT},
/*RmepErrCCMRecvdWhile
  NotZero*/             {DOT1AG_RMEP_ERRCCM_DEFAULT, DOT1AG_RMEP_ERRCCM_DEFAULT, DOT1AG_RMEP_ERRCCM_DEFECT },
/*RmepErrCCMWhileZero*/ {DOT1AG_RMEP_ERRCCM_DEFAULT, DOT1AG_RMEP_ERRCCM_DEFAULT, DOT1AG_RMEP_ERRCCM_NO_DEFECT}
};

/* CFM Service level Cross Connect state Machine - 20.24 */
dot1agXConMepStates_t  dot1agSMXConMepTable[DOT1AG_MEP_XCON_EVENTS_MAX][DOT1AG_MEP_XCON_DEFAULT] =
{
/*Events*/              /*XCON_IDLE*/          /*XCON_NO_DEFECT*/          /*XCON_DEFECT*/
/*XConMepBegin*/     {DOT1AG_MEP_XCON_IDLE,    DOT1AG_MEP_XCON_DEFAULT,  DOT1AG_MEP_XCON_DEFAULT},
/*XConNotNMepActive*/{DOT1AG_MEP_XCON_IDLE,    DOT1AG_MEP_XCON_IDLE,  DOT1AG_MEP_XCON_IDLE},
/*XConCCMRecvd*/     {DOT1AG_MEP_XCON_DEFAULT, DOT1AG_MEP_XCON_DEFECT,   DOT1AG_MEP_XCON_DEFAULT},
/*XConCCMRecvd
WhileNotZero*/       {DOT1AG_MEP_XCON_DEFAULT, DOT1AG_MEP_XCON_DEFAULT,  DOT1AG_MEP_XCON_DEFECT },
/*XConCCMWhileZero*/ {DOT1AG_MEP_XCON_DEFAULT, DOT1AG_MEP_XCON_DEFAULT,  DOT1AG_MEP_XCON_NO_DEFECT}
};

/* CFM Service level Loopback Responder state Machine - 20.27 */
dot1agLBRStates_t  dot1agSMLBRTable[DOT1AG_LBR_EVENTS_MAX][DOT1AG_LBR_DEFAULT] =
{
/*Events*/            /*LR_IDLE*/           /*LR_RESPOND*/
/*LBRBegin*/       {DOT1AG_LBR_IDLE,    DOT1AG_LBR_DEFAULT},
/*LBRNotMepActive*/{DOT1AG_LBR_IDLE,    DOT1AG_LBR_IDLE},
/*LBRLBMRecvd*/    {DOT1AG_LBR_RESPOND, DOT1AG_LBR_DEFAULT}
};

/* CFM Service level Loopback Initiator state Machine - 20.31 */
dot1agLBIStates_t  dot1agSMLBITable[DOT1AG_LBI_EVENTS_MAX][DOT1AG_LBI_DEFAULT] =
{
/*Events*/                    /*LBI_IDLE*/    /*LBI_STARTING*/     /*LBI_XMITTING*/    /*LBI_XMIT*/        /*LBI_WAITING*/
/*dot1agLBIBegin*/          {DOT1AG_LBI_IDLE,     DOT1AG_LBI_DEFAULT,  DOT1AG_LBI_DEFAULT,  DOT1AG_LBI_DEFAULT,  DOT1AG_LBI_DEFAULT},
/*dot1agLBINotMepActive*/   {DOT1AG_LBI_IDLE,     DOT1AG_LBI_IDLE,     DOT1AG_LBI_IDLE,     DOT1AG_LBI_IDLE,     DOT1AG_LBI_IDLE},
/*dot1agLBILBMsToSend*/     {DOT1AG_LBI_STARTING, DOT1AG_LBI_XMITTING, DOT1AG_LBI_XMITTING, DOT1AG_LBI_XMITTING, DOT1AG_LBI_DEFAULT},
/*dot1agLBITestStop*/       {DOT1AG_LBI_DEFAULT,  DOT1AG_LBI_DEFAULT,  DOT1AG_LBI_IDLE,     DOT1AG_LBI_IDLE,  DOT1AG_LBI_IDLE}
};

/* CFM Service level Loopback Initiator Responder state Machine - 20.32 */
dot1agLBRStates_t  dot1agSMLBIRTable[DOT1AG_LBIR_EVENTS_MAX][DOT1AG_LBIR_DEFAULT] =
{
/*Events*/             /*LBR_IDLE*/         /*LBR_RESPOND*/
/*LBIRBegin*/       {DOT1AG_LBIR_IDLE,    DOT1AG_LBIR_DEFAULT},
/*LBIRNotMepActive*/{DOT1AG_LBIR_IDLE,    DOT1AG_LBIR_IDLE},
/*LBIRLBRRecvd*/    {DOT1AG_LBIR_RESPOND, DOT1AG_LBIR_DEFAULT}
};

/* CFM Service level FNG State Machine - 20.36 */
dot1agMepFNGStates_t  dot1agSMMepFNGTable[DOT1AG_FNG_EVENTS_MAX][DOT1AG_FNG_DEFAULT] =
{
/*Events*/                /*FNG_RESET*/        /*FNG_DEFECT*/    /*FNG_REPORT_DEFECT*/ /*FNG_DEFECT_REPORTED*/ /*FNG_DEFECT_CLEARING*/
/*FNGBegin*/              {DOT1AG_FNG_RESET,  DOT1AG_FNG_DEFAULT,  DOT1AG_FNG_DEFAULT,   DOT1AG_FNG_DEFAULT,   DOT1AG_FNG_DEFAULT},
/*FNGNotMepActive*/       {DOT1AG_FNG_RESET,  DOT1AG_FNG_RESET,    DOT1AG_FNG_RESET,     DOT1AG_FNG_RESET,     DOT1AG_FNG_RESET},
/*FNGMaDefectIndication*/ {DOT1AG_FNG_DEFECT, DOT1AG_FNG_DEFAULT,  DOT1AG_FNG_DEFAULT,   DOT1AG_FNG_DEFAULT,   DOT1AG_FNG_DEFECT_REPORTED},
/*FNGNotMaDefectIndicatn*/{DOT1AG_FNG_DEFAULT, DOT1AG_FNG_RESET,    DOT1AG_FNG_DEFAULT,   DOT1AG_FNG_DEFECT_CLEARING,   DOT1AG_FNG_DEFAULT},
/*FNGMaDefectIndicatn
  FNGWhileZero*/          {DOT1AG_FNG_DEFECT, DOT1AG_FNG_REPORT_DEFECT,    DOT1AG_FNG_DEFAULT,   DOT1AG_FNG_DEFAULT,  DOT1AG_FNG_DEFAULT},
/*FNGMaDefect
IndicatnHighDefectPrio*/  {DOT1AG_FNG_DEFAULT, DOT1AG_FNG_DEFAULT,  DOT1AG_FNG_DEFAULT, DOT1AG_FNG_REPORT_DEFECT, DOT1AG_FNG_DEFAULT},
/*FNGNotMaDefect
IndicatnFNGWhileZero*/    {DOT1AG_FNG_DEFAULT, DOT1AG_FNG_DEFAULT,  DOT1AG_FNG_DEFAULT, DOT1AG_FNG_DEFECT_CLEARING, DOT1AG_FNG_RESET}
};

/* CFM Service level Linktrace Initiator Receieve state Machine - 20.40 */
dot1agLTIRStates_t  dot1agSMLTIRTable[DOT1AG_LTIR_EVENTS_MAX][DOT1AG_LTIR_DEFAULT] =
{
/*Events*/             /*LTIR_IDLE*/       /*LTIR_RESPOND*/
/*LTIRBegin*/        {DOT1AG_LTIR_IDLE,    DOT1AG_LTIR_DEFAULT},
/*LTIRNotMepActive*/ {DOT1AG_LTIR_IDLE,    DOT1AG_LTIR_IDLE},
/*LTIRLTRRecvd*/     {DOT1AG_LTIR_RESPOND, DOT1AG_LTIR_DEFAULT}
};

/* CFM Service level Linktrace Receiver state Machine - 20.44 */
dot1agLTRRStates_t  dot1agSMLTRRTable[DOT1AG_LTRR_EVENTS_MAX][DOT1AG_LTRR_DEFAULT] =
{
/*Events*/               /*LTM_IDLE*/       /*LTM_RESPOND*/
/*LTMRBegin*/        {DOT1AG_LTRR_IDLE,    DOT1AG_LTRR_DEFAULT},
/*LTMRNotMepActive*/ {DOT1AG_LTRR_IDLE,    DOT1AG_LTRR_IDLE},
/*LTMRLTMRecvd*/     {DOT1AG_LTRR_RESPOND, DOT1AG_LTRR_DEFAULT}
};

/* CFM Service level Linktrace Transmitter state Machine - 20.45 */
dot1agLTRTStates_t  dot1agSMLTRTTable[DOT1AG_LTRT_EVENTS_MAX][DOT1AG_LTRT_DEFAULT] =
{
/*Events*/                  /*LTR_IDLE*/         /*LTR_WAITING*/       /*LTR_XMITTING*/
/*LTRTBegin*/           {DOT1AG_LTRT_IDLE,    DOT1AG_LTRT_DEFAULT,   DOT1AG_LTRT_DEFAULT},
/*LTRTNotMepActive*/    {DOT1AG_LTRT_IDLE,    DOT1AG_LTRT_IDLE,      DOT1AG_LTRT_IDLE},
/*LTRTLtfWhileZero*/    {DOT1AG_LTRT_DEFAULT, DOT1AG_LTRT_XMITTING,  DOT1AG_LTRT_DEFAULT},
/*LTRTnPendingLTRs*/    {DOT1AG_LTRT_DEFAULT, DOT1AG_LTRT_DEFAULT,   DOT1AG_LTRT_XMITTING},
/*LTRTnPendingLTRsZero*/{DOT1AG_LTRT_DEFAULT, DOT1AG_LTRT_DEFAULT,   DOT1AG_LTRT_WAITING}
};

/* CFM Service level ProLm state Machine */
y1731MepProlmStates_t  y1731SMProlmTable[Y1731_PROLM_EVENTS_MAX][Y1731_PROLM_DEFAULT] =
{
/*Events*/                      /*Y1731_PROLM_DISABLE*/  /*Y1731_PROLM_ENABLE*/
/*y1731PROLMBegin*/             {Y1731_PROLM_DISABLE,    Y1731_PROLM_DEFAULT},
/*y1731PROLMNotMepActive*/      {Y1731_PROLM_DISABLE,    Y1731_PROLM_DISABLE},
/*y1731PROLMCCMRecvd*/          {Y1731_PROLM_DEFAULT,    Y1731_PROLM_ENABLE},
/*y1731PROLMLMRRecvd*/          {Y1731_PROLM_DEFAULT,    Y1731_PROLM_ENABLE},
/*y1731PROLMSLRRecvd*/          {Y1731_PROLM_DEFAULT,    Y1731_PROLM_ENABLE},
/*y1731PROLMWhileZero*/         {Y1731_PROLM_DISABLE,    Y1731_PROLM_ENABLE},
/*y1731PROLMEnabled*/           {Y1731_PROLM_ENABLE,     Y1731_PROLM_DEFAULT},
/*y1731PROLMNotEnabled*/        {Y1731_PROLM_DEFAULT,    Y1731_PROLM_DISABLE},
};

/* CFM Service level OndemLm INIT state Machine */
y1731OndemLMMStates_t  y1731SMOndemLMMTable[Y1731_ONDEMLMM_EVENTS_MAX][Y1731_ONDEMLMM_DEFAULT] =
{
/*Events*/                       /*Y1731_ONDEMLMM_IDLE*/    /*Y1731_ONDEMLMM_XMIT*/
/*y1731ONDEMLMMBegin*/           {Y1731_ONDEMLMM_IDLE,      Y1731_ONDEMLMM_DEFAULT},
/*y1731ONDEMLMMNotMepActive*/    {Y1731_ONDEMLMM_IDLE,      Y1731_ONDEMLMM_IDLE},
/*y1731ONDEMLMMStart*/           {Y1731_ONDEMLMM_XMIT,      Y1731_ONDEMLMM_DEFAULT},
/*y1731ONDEMLMMStop*/            {Y1731_ONDEMLMM_DEFAULT,   Y1731_ONDEMLMM_IDLE},
/*y1731ONDEMLMMWhileZero*/       {Y1731_ONDEMLMM_DEFAULT,   Y1731_ONDEMLMM_XMIT},
/*y1731ONDEMLMMLMRRecvd*/        {Y1731_ONDEMLMM_DEFAULT,   Y1731_ONDEMLMM_XMIT},
/*y1731ONDEMLMMSLRRecvd*/        {Y1731_ONDEMLMM_DEFAULT,   Y1731_ONDEMLMM_XMIT},
};

/* CFM Service level LM RESP state Machine */
y1731LMRStates_t  y1731SMLMRTable[Y1731_LMR_EVENTS_MAX][Y1731_LMR_DEFAULT] =
{
/*Events*/                      /*Y1731_LMR_IDLE*/  /*Y1731_LMR_RESPOND*/
/*y1731LMRBegin*/          {Y1731_LMR_IDLE,    Y1731_LMR_DEFAULT},
/*y1731LMRNotMepActive*/   {Y1731_LMR_IDLE,    Y1731_LMR_IDLE},
/*y1731LMRLMMRecvd*/       {Y1731_LMR_RESPOND, Y1731_LMR_DEFAULT},
/*y1731LMRSLMRecvd*/       {Y1731_LMR_RESPOND, Y1731_LMR_DEFAULT}
};

/* CFM Service level DM INIT state Machine */
y1731DMMStates_t  y1731SMDMMTable[Y1731_DMM_EVENTS_MAX][Y1731_DMM_DEFAULT] =
{
/*Events*/                 /*Y1731_DMM_IDLE*/    /*Y1731_DMM_XMIT*/
/*y1731DMMBegin*/          {Y1731_DMM_IDLE,      Y1731_DMM_DEFAULT},
/*y1731DMMNotMepActive*/   {Y1731_DMM_IDLE,      Y1731_DMM_IDLE},
/*y1731DMMStart*/          {Y1731_DMM_XMIT,      Y1731_DMM_DEFAULT},
/*y1731DMMStop*/           {Y1731_DMM_DEFAULT,   Y1731_DMM_IDLE},
/*y1731DMMWhileZero*/      {Y1731_DMM_DEFAULT,   Y1731_DMM_XMIT},
/*y1731DMMDMRRecvd*/       {Y1731_DMM_DEFAULT,   Y1731_DMM_XMIT},
};

/* CFM Service level DM RESP state Machine */
y1731DMRStates_t  y1731SMDMRTable[Y1731_DMR_EVENTS_MAX][Y1731_DMR_DEFAULT] =
{
/*Events*/                 /*Y1731_DMR_IDLE*/    /*Y1731_DMR_RESPOND*/
/*y1731DMRBegin*/          {Y1731_DMR_IDLE,      Y1731_DMR_DEFAULT},
/*y1731DMRNotMepActive*/   {Y1731_DMR_IDLE,      Y1731_DMR_IDLE},
/*y1731DMRDMMRecvd*/       {Y1731_DMR_RESPOND,   Y1731_DMR_DEFAULT}
};

/* CFM Service level AIS send state Machine */
y1731AISSendStates_t  y1731SMAisSendTable[Y1731_AIS_SEND_EVENTS_MAX][Y1731_AIS_SEND_DEFAULT] =
{
/*Events*/                    /*Y1731_AIS_SEND_STOP*/   /*Y1731_AIS_SEND_START*/
/*y1731AisSendBegin*/          {Y1731_AIS_SEND_STOP,     Y1731_AIS_SEND_STOP},
/*y1731AisSendNotMepActive*/   {Y1731_AIS_SEND_STOP,     Y1731_AIS_SEND_STOP},
/*y1731AisSendWhileZero*/      {Y1731_AIS_SEND_STOP,     Y1731_AIS_SEND_START},
/*y1731AisSendStart*/          {Y1731_AIS_SEND_START,    Y1731_AIS_SEND_START},
/*y1731AisSendStop*/           {Y1731_AIS_SEND_STOP,     Y1731_AIS_SEND_STOP},
};

/* CFM Service level AIS receive state Machine */
y1731AISRcvStates_t  y1731SMAisRcvTable[Y1731_AIS_RCV_EVENTS_MAX][Y1731_AIS_RCV_DEFAULT] =
{
/*Events*/                    /*Y1731_AIS_RCV_IDLE*/   /*Y1731_AIS_RCV_WAITING*/
/*y1731AisRcvBegin*/          {Y1731_AIS_RCV_IDLE,      Y1731_AIS_RCV_DEFAULT},
/*y1731AisRcvNotMepActive*/   {Y1731_AIS_RCV_IDLE,      Y1731_AIS_RCV_IDLE},
/*y1731AisRcvWhileZero*/      {Y1731_AIS_RCV_IDLE,      Y1731_AIS_RCV_IDLE},
/*y1731AisRcvReceived*/       {Y1731_AIS_RCV_WAITING,   Y1731_AIS_RCV_WAITING}
};

/* CFM Service level LCK send state Machine */
y1731LCKSendStates_t  y1731SMLckSendTable[Y1731_LCK_SEND_EVENTS_MAX][Y1731_LCK_SEND_DEFAULT] =
{
/*Events*/                    /*Y1731_LCK_SEND_STOP*/   /*Y1731_LCK_SEND_START*/
/*y1731LckSendBegin*/          {Y1731_LCK_SEND_STOP,     Y1731_LCK_SEND_STOP},
/*y1731LckSendNotMepActive*/   {Y1731_LCK_SEND_STOP,     Y1731_LCK_SEND_STOP},
/*y1731LckSendWhileZero*/      {Y1731_LCK_SEND_STOP,     Y1731_LCK_SEND_START},
/*y1731LckSendStart*/          {Y1731_LCK_SEND_START,    Y1731_LCK_SEND_START},
/*y1731LckSendStop*/           {Y1731_LCK_SEND_STOP,     Y1731_LCK_SEND_STOP},
};

/* CFM Service level LCK receive state Machine */
y1731LCKRcvStates_t  y1731SMLckRcvTable[Y1731_LCK_RCV_EVENTS_MAX][Y1731_LCK_RCV_DEFAULT] =
{
/*Events*/                    /*Y1731_LCK_RCV_IDLE*/   /*Y1731_LCK_RCV_WAITING*/
/*y1731LckRcvBegin*/          {Y1731_LCK_RCV_IDLE,      Y1731_LCK_RCV_DEFAULT},
/*y1731LckRcvNotMepActive*/   {Y1731_LCK_RCV_IDLE,      Y1731_LCK_RCV_IDLE},
/*y1731LckRcvWhileZero*/      {Y1731_LCK_RCV_IDLE,      Y1731_LCK_RCV_IDLE},
/*y1731LckRcvReceived*/       {Y1731_LCK_RCV_WAITING,   Y1731_LCK_RCV_WAITING}
};

/* CFM Service level CSF send state Machine */
y1731CSFSendStates_t  y1731SMCsfSendTable[Y1731_CSF_SEND_EVENTS_MAX][Y1731_CSF_SEND_DEFAULT] =
{
/*Events*/                    /*Y1731_CSF_SEND_STOP*/   /*Y1731_CSF_SEND_START*/
/*y1731CsfSendBegin*/          {Y1731_CSF_SEND_STOP,     Y1731_CSF_SEND_STOP},
/*y1731CsfSendNotMepActive*/   {Y1731_CSF_SEND_STOP,     Y1731_CSF_SEND_STOP},
/*y1731CsfSendWhileZero*/      {Y1731_CSF_SEND_STOP,     Y1731_CSF_SEND_START},
/*y1731CsfSendStart*/          {Y1731_CSF_SEND_START,    Y1731_CSF_SEND_START},
/*y1731CsfSendStop*/           {Y1731_CSF_SEND_STOP,     Y1731_CSF_SEND_STOP},
};

/* CFM Service level CSF receive state Machine */
y1731CSFRcvStates_t  y1731SMCsfRcvTable[Y1731_CSF_RCV_EVENTS_MAX][Y1731_CSF_RCV_DEFAULT] =
{
/*Events*/                    /*Y1731_CSF_RCV_IDLE*/   /*Y1731_CSF_RCV_WAITING*/
/*y1731CsfRcvBegin*/          {Y1731_CSF_RCV_IDLE,      Y1731_CSF_RCV_DEFAULT},
/*y1731CsfRcvNotMepActive*/   {Y1731_CSF_RCV_IDLE,      Y1731_CSF_RCV_IDLE},
/*y1731CsfRcvWhileZero*/      {Y1731_CSF_RCV_IDLE,      Y1731_CSF_RCV_IDLE},
/*y1731CsfRcvReceived*/       {Y1731_CSF_RCV_WAITING,   Y1731_CSF_RCV_WAITING}
};

/* CFM Service level Y.1731 d*(dUNPtr/dUNL/dMMG/dUNP/dUNM) state machine */
y1731MepDefectStates_t  y1731defectTable[Y1731_MEP_UNPTR_EVENTS_MAX][Y1731_MEP_DEFECT_DEFAULT] =
{
/*Events*/              /*Y1731_MEP_DEFECT_IDLE*/  /*Y1731_MEP_DEFECT_NO_DEFECT*/ /*Y1731_MEP_DEFECT_DEFECT*/
/*y1731d*MepBegin*/     {Y1731_MEP_DEFECT_IDLE,    Y1731_MEP_DEFECT_DEFAULT,      Y1731_MEP_DEFECT_DEFAULT},
/*y1731d*NotNMepActive*/{Y1731_MEP_DEFECT_IDLE,    Y1731_MEP_DEFECT_IDLE,         Y1731_MEP_DEFECT_IDLE},
/*y1731d*CCMRecvd*/     {Y1731_MEP_DEFECT_DEFAULT, Y1731_MEP_DEFECT_DEFECT,       Y1731_MEP_DEFECT_DEFAULT},
/*y1731d*CCMRecvd
WhileNotZero*/          {Y1731_MEP_DEFECT_DEFAULT, Y1731_MEP_DEFECT_DEFAULT,      Y1731_MEP_DEFECT_DEFECT},
/*y1731d*CCMWhileZero*/ {Y1731_MEP_DEFECT_DEFAULT, Y1731_MEP_DEFECT_DEFAULT,      Y1731_MEP_DEFECT_NO_DEFECT}
};

/*********************************************************************
* @purpose  This is the classifier which dispatches the received
*           dot1ag event to a particular state machine
*
* @param   eventMsg  @b{(input)) holds the SM event related data
*
* @returns OFDPA_E_NONE
* @returns OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmStateMachineClassifier(dot1agCfmSMEventMsg_t *eventMsg,
                                              OFDPA_BOOL hwEnabled)
{
  OFDPA_ERROR_t    rc = OFDPA_E_FAIL;
  dot1agSMEvents_t dot1agEvent;

  if (eventMsg == NULL)
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = eventMsg->event;

  if (dot1agEvent > dot1agCCIEventsStart && dot1agEvent < dot1agCCIEventsEnd)
  {
    if (hwEnabled == OFDPA_FALSE)
      rc = dot1agCfmMepCCIStateMachine(eventMsg);
    else
      rc = OFDPA_E_NONE;
  }
  else if (dot1agEvent > dot1agCCREventsStart && dot1agEvent < dot1agCCREventsEnd)
  {
    if (hwEnabled == OFDPA_FALSE)
      rc = dot1agCfmMepCCRStateMachine(eventMsg);     
    else
      rc = OFDPA_E_NONE;
  }
  else if (dot1agEvent > dot1agRmepEventsStart && dot1agEvent < dot1agRmepEventsEnd)
  {
    if (hwEnabled == OFDPA_FALSE)
      /* Modified by JiaoYong, 2016/4/21 不让状态机运行*/      
      //rc = dot1agCfmRMepStateMachine(eventMsg);
      rc = OFDPA_E_NONE;
    else
      rc = OFDPA_E_NONE;
  }
  else if (dot1agEvent > dot1agRmepErrEventsStart && dot1agEvent < dot1agRmepErrEventsEnd)
  {
    if (hwEnabled == OFDPA_FALSE)
      rc = dot1agCfmRMepErrStateMachine(eventMsg);
    else
      rc = OFDPA_E_NONE;
  }
  else if (dot1agEvent > dot1agXConEventsStart && dot1agEvent < dot1agXConEventsEnd)
  {
    if (hwEnabled == OFDPA_FALSE)
      rc = dot1agCfmMepXConStateMachine(eventMsg);
    else
      rc = OFDPA_E_NONE;
  }
  else if (dot1agEvent > dot1agLBREventsStart && dot1agEvent < dot1agLBREventsEnd)
  {
    rc = dot1agCfmLBRStateMachine(eventMsg);
  }
  else if (dot1agEvent > dot1agLBIEventsStart && dot1agEvent < dot1agLBIEventsEnd)
  {
    rc = dot1agCfmLBIStateMachine(eventMsg);
  }
  else if (dot1agEvent > dot1agLBIREventsStart && dot1agEvent < dot1agLBIREventsEnd)
  {
    rc = dot1agCfmLBIRStateMachine(eventMsg);
  }
  else if (dot1agEvent > dot1agFNGEventsStart && dot1agEvent < dot1agFNGEventsEnd)
  {
    rc = dot1agCfmMepFNGStateMachine(eventMsg);
  }
  else if (dot1agEvent > dot1agLTIREventsStart && dot1agEvent < dot1agLTIREventsEnd)
  {
    rc = dot1agCfmLTIRStateMachine(eventMsg);
  }
  else if (dot1agEvent > dot1agLTRREventsStart && dot1agEvent < dot1agLTRREventsEnd)
  {
    rc = dot1agCfmLTRRStateMachine(eventMsg);
  }
  else if (dot1agEvent > dot1agLTRTEventsStart && dot1agEvent < dot1agLTRTEventsEnd)
  {
    rc = dot1agCfmLTRTStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731PROLMEventsStart && dot1agEvent < y1731PROLMEventsEnd)
  {
    rc = y1731CfmProLMStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731ONDEMLMMEventsStart && dot1agEvent < y1731ONDEMLMMEventsEnd)
  {
    rc = y1731CfmOndemLMMStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731LMREventsStart && dot1agEvent < y1731LMREventsEnd)
  {
    rc = y1731CfmLMRStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731DMMEventsStart && dot1agEvent < y1731DMMEventsEnd)
  {
    rc = y1731CfmDMMStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731DMREventsStart && dot1agEvent < y1731DMREventsEnd)
  {
    rc = y1731CfmDMRStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731AisSendEventsStart && dot1agEvent < y1731AisSendEventsEnd)
  {
    rc = y1731CfmMepAISSendStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731AisRcvEventsStart && dot1agEvent < y1731AisRcvEventsEnd)
  {
    rc = y1731CfmMepAISRcvStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731LckSendEventsStart && dot1agEvent < y1731LckSendEventsEnd)
  {
    rc = y1731CfmMepLCKSendStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731LckRcvEventsStart && dot1agEvent < y1731LckRcvEventsEnd)
  {
    rc = y1731CfmMepLCKRcvStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731CsfSendEventsStart && dot1agEvent < y1731CsfSendEventsEnd)
  {
    rc = y1731CfmMepCSFSendStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731CsfRcvEventsStart && dot1agEvent < y1731CsfRcvEventsEnd)
  {
    rc = y1731CfmMepCSFRcvStateMachine(eventMsg);
  }
  else if (dot1agEvent > y1731dUNPtrEventsStart && dot1agEvent < y1731dUNPtrEventsEnd)
  {
    rc = y1731CfmMepDefectStateMachine(eventMsg, Y1731_DEFECT_TYPE_UNPTR);
  }
  else if (dot1agEvent > y1731dUNLEventsStart && dot1agEvent < y1731dUNLEventsEnd)
  {
    rc = y1731CfmMepDefectStateMachine(eventMsg, Y1731_DEFECT_TYPE_UNL);
  }
  else if (dot1agEvent > y1731dMMGEventsStart && dot1agEvent < y1731dMMGEventsEnd)
  {
    rc = y1731CfmMepDefectStateMachine(eventMsg, Y1731_DEFECT_TYPE_MMG);
  }
  else if (dot1agEvent > y1731dUNPEventsStart && dot1agEvent < y1731dUNPEventsEnd)
  {
    rc = y1731CfmMepDefectStateMachine(eventMsg, Y1731_DEFECT_TYPE_UNP);
  }
  else if (dot1agEvent > y1731dUNMEventsStart && dot1agEvent < y1731dUNMEventsEnd)
  {
    rc = y1731CfmMepDefectStateMachine(eventMsg, Y1731_DEFECT_TYPE_UNM);
  }
  else
  {
    rc = OFDPA_E_FAIL;
  }

  return rc;
}

/***************************************************************
****************   CONTINUITY CHECK PROTOCOL FSMs   ************
***************************************************************/

/***************************************************************************
* @purpose  This routine calculates the next state for the CCI state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments MEP is only eligile for initiating the CCMs.
*           802.1ag d8-1 20.12
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmMepCCIStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agMEPCciStates_t  nextState, dot1agEvent;
  dot1agCfmStackObj_t  *pMEPObj;
  uint32_t              ccmInterval = 0;
  OFDPA_ERROR_t         rc          = OFDPA_E_NONE;

  /* Get the MEP Object.*/
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if ((pMEPObj == NULL) || (NULL == pMEPObj->mepData))
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - dot1agCCIEventsStart - 1;

  nextState = dot1agSMCCITable[dot1agEvent][pMEPObj->mepData->dot1agCfmMepCCIState];
  switch (nextState)
  {
    case  DOT1AG_MEP_CCI_IDLE:
      {
        /* Delete the cciTimer if running */
        if (pMEPObj->mepData->dot1agCfmMepCCIVar.cciTimer != NULL)
        {
          /* stop the timer if already running */
          dot1agTimerDelete(pMEPObj->mepData->dot1agCfmMepCCIVar.cciTimer);
          pMEPObj->mepData->dot1agCfmMepCCIVar.cciTimer = NULL;
        }
      }
      break;
    case DOT1AG_MEP_CCI_WAITING:
      {
        rc = addCCMToArray(pMEPObj);

        /* Start CCI Timer for CCI Interval */
        ccmInterval = pMEPObj->mepData->maData->dot1agCfmMaNetCcmInterval;
        if ((pMEPObj->mepData != NULL) &&
            (OFDPA_TRUE == dot1agIsMepHwEnabled(pMEPObj->mepData->dot1agCfmMepIdentifier)))
        {
          ccmInterval = CCMtime(ccmInterval, OFDPA_TRUE);
        }
        else
        {
          ccmInterval = CCMtime(ccmInterval, OFDPA_FALSE);
        }
        if (pMEPObj->mepData->dot1agCfmMepCCIVar.cciTimer != NULL)
        {
          dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepCCIVar.cciTimer, pMEPObj,
                            ccmInterval, dot1agCfmCciTimerCallback);
        }
        else
        {
          /* Start CCIWhile timer */
          pMEPObj->mepData->dot1agCfmMepCCIVar.cciTimer =
            dot1agTimerNodeAdd(ccmInterval, dot1agCfmCciTimerCallback, pMEPObj);
        }
        if (pMEPObj->mepData->dot1agCfmMepCCIVar.cciTimer == NULL)
        {
          rc = OFDPA_E_FAIL;
          break;
        }
        pMEPObj->mepData->dot1agCfmMepCCIVar.macStatusChanged = OFDPA_FALSE;
      }
      break;
    case DOT1AG_MEP_CCI_DEFAULT:
    default:
      return OFDPA_E_FAIL;
      break;
  }

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_CCI, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, pMEPObj->mepData->dot1agCfmMepCCIState, nextState);
    pMEPObj->mepData->dot1agCfmMepCCIState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d currState %d",
                       dot1agEvent, pMEPObj->mepData->dot1agCfmMepCCIState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the MEP CCR state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This receiver SM is applicable for MEP. Variables CCMreceivedLow,
*           CCMreceivedEqual not used as event is going to hold the value
*           802.1ag d8-1 20.18
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmMepCCRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agMEPCcrStates_t  nextState, dot1agEvent;
  dot1agCfmMepTable_t  *mepObj;
  OFDPA_BOOL            UCT = OFDPA_FALSE;
  OFDPA_ERROR_t         rc  = OFDPA_E_NONE;

  if (dot1agCfmEventMsg == NULL ||
      dot1agCfmEventMsg->pMPObj == NULL ||
      dot1agCfmEventMsg->pMPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  mepObj      = dot1agCfmEventMsg->pMPObj->mepData;
  dot1agEvent = dot1agCfmEventMsg->event - dot1agCCREventsStart - 1;
  nextState   = dot1agSMMepCCRTable[dot1agEvent][mepObj->dot1agCfmMepCCRState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_MEP_CCR_RESET:
        {
          UCT = OFDPA_TRUE;
          nextState = DOT1AG_MEP_CCR_IDLE;
          rc = OFDPA_E_NONE;
          break;
        }

      case DOT1AG_MEP_CCR_IDLE:
        {
          /* No action to do in this state.*/
          break;
        }

      case DOT1AG_MEP_CCR_RECORD_EQUAL:
        {
          MEPprocessEqualCCM(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);
          UCT = OFDPA_TRUE;
          nextState = DOT1AG_MEP_CCR_IDLE;
          rc = OFDPA_E_NONE;
          break;
        }

      case DOT1AG_MEP_CCR_RECORD_LOW:
        {
          MEPprocessLowCCM(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);
          UCT = OFDPA_TRUE;
          nextState = DOT1AG_MEP_CCR_IDLE;
          rc = OFDPA_E_NONE;
          break;
        }

      case DOT1AG_MEP_CCR_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_CCR, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, mepObj->dot1agCfmMepCCRState, nextState);
    mepObj->dot1agCfmMepCCRState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d currState %d",
                       dot1agEvent, mepObj->dot1agCfmMepCCRState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the Remote MEP state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This is run per-RMEP.
*           802.1ag d8-1 20.20
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmRMepStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agRmepStates_t   nextState, dot1agEvent;
  dot1agCfmMepTable_t *mepData;
  OFDPA_BOOL           UCT          = OFDPA_FALSE;
  OFDPA_ERROR_t        rc           = OFDPA_E_NONE;
  dot1agCfmRMepObj_t  *rmepObj;
  uint32_t             rmepInterval = 0;
  uint32_t             ccmInterval  = 0;
  OFDPA_BOOL           hwEnabled    = OFDPA_FALSE;
  ofdbOamLmepData_t *oamMapData;

  /* Get MEP Object */
  mepData = dot1agCfmEventMsg->pMepData;
  if (mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepData->dot1agCfmMepIdentifier))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  /* Check If RMEP ID Exists */
  rmepObj = dot1agCfmEventMsg->pRMEPObj;
  if (rmepObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - dot1agRmepEventsStart - 1;
  nextState = dot1agSMRmepTable[dot1agEvent][rmepObj->dot1agCfmRmepState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_RMEP_IDLE:
        rmepObj->dot1agCfmMepDbRMepFailedOkTime = 0;

        rmepObj->dot1agCfmRmepVar.rMepCcmDefect = OFDPA_FALSE;
        if (rmepObj->dot1agCfmRmepVar.rmepTimer != NULL)
        {
          dot1agTimerDelete(rmepObj->dot1agCfmRmepVar.rmepTimer);
          rmepObj->dot1agCfmRmepVar.rmepTimer = NULL;
        }
        UCT = OFDPA_TRUE;
        nextState = DOT1AG_RMEP_START;
        rc = OFDPA_E_NONE;
        break;

      case DOT1AG_RMEP_START:
        rmepObj->dot1agCfmMepDbRMepFailedOkTime = 0;

        /* CCM interval field will be unavailable for the below event */
        if (dot1agCfmEventMsg->event == dot1agRmepNotEnableRmepDefect)
        {
          ccmInterval = 0;
        }
        else
        {
          ccmInterval  = CCMtime(mepData->maData->dot1agCfmMaNetCcmInterval, hwEnabled);
          rmepInterval = DOT1AG_RMEP_INTERVAL_GET(ccmInterval);

          /* Start RMEP Timer for 3.5 * CCM Interval */
          rmepObj->dot1agCfmRmepVar.rmepTimer = dot1agTimerNodeAdd(rmepInterval, dot1agCfmRmepTimerCallback, rmepObj);

          if (rmepObj->dot1agCfmRmepVar.rmepTimer == NULL)
          {
            rc = OFDPA_E_FAIL;
            break;
          }
        }

        /*
         * Do not wipe out the MAC address for HW LMEPs.  This is because the HW never
         * reports the MAC address so it will not be restored as it would be if CCMs were
         * processed in SW.
         */
        if (OFDPA_TRUE != hwEnabled)
        {
          memset(rmepObj->dot1agCfmRmepVar.rMepMacAddress, 0xFF, OFDPA_MAC_ADDR_LEN);
        }
        rmepObj->dot1agCfmRmepVar.rMepLastRDI        = OFDPA_FALSE;
        rmepObj->dot1agCfmRmepVar.rMepLastPortState  = DOT1AG_RMEP_PORT_STATE_psNoPortStateTLV;
        rmepObj->dot1agCfmRmepVar.rMepLastIntfStatus = DOT1AG_RMEP_INTERFACE_STATUS_isNoInterfaceStatusTLV;

        break;

      case DOT1AG_RMEP_OK:
        rmepObj->dot1agCfmMepDbRMepFailedOkTime = dot1agCentisecondsGet();

        if (OFDPA_FALSE != rmepObj->dot1agCfmRmepVar.rMepCcmDefect)
        {
          /* report fault cleared to APS component */
          /* retrieve the livenessPortId associated with this mep */
          if (ofdbInjectedOamLmepIdMapDataGet(dot1agCfmEventMsg->pMPObj->ofdpaLocalMpId, &oamMapData) == OFDPA_E_NONE)
          {
            g8131PgFaultReport(oamMapData->livenessLogicalPortId, 1);  /* up */
          }
        }

        rmepObj->dot1agCfmRmepVar.rMepCcmDefect = OFDPA_FALSE;

        ccmInterval  = CCMtime(mepData->maData->dot1agCfmMaNetCcmInterval, hwEnabled);
        rmepInterval = DOT1AG_RMEP_INTERVAL_GET(ccmInterval);

        if (rmepObj->dot1agCfmRmepVar.rmepTimer != NULL)
        {
          rc = dot1agTimerUpdate(rmepObj->dot1agCfmRmepVar.rmepTimer, rmepObj, rmepInterval, dot1agCfmRmepTimerCallback);
          if (rc != OFDPA_E_NONE)
          {
            break;
          }
        }
        else
        {
          rmepObj->dot1agCfmRmepVar.rmepTimer = dot1agTimerNodeAdd(rmepInterval, dot1agCfmRmepTimerCallback, rmepObj);
          if (rmepObj->dot1agCfmRmepVar.rmepTimer == NULL)
          {
            rc = OFDPA_E_FAIL;
            break;
          }
        }

        memcpy(rmepObj->dot1agCfmRmepVar.rMepMacAddress, dot1agCfmEventMsg->dot1agPkt->srcMac, OFDPA_MAC_ADDR_LEN);

        rmepObj->dot1agCfmMepDbRdi                   = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.rdi;
        rmepObj->dot1agCfmRmepVar.rMepLastRDI        = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.rdi;

        /* Copy Port Status TLV */
        if (dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.portStatusTLVRead == OFDPA_TRUE)
        {
          rmepObj->dot1agCfmMepDbPortStatusTlv        = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.portStatusTLV.portStatus;
          rmepObj->dot1agCfmRmepVar.rMepLastPortState = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.portStatusTLV.portStatus;

          if (rmepObj->dot1agCfmRmepVar.rMepLastPortState != DOT1AG_RMEP_PORT_STATE_psNoPortStateTLV &&
              rmepObj->dot1agCfmRmepVar.rMepLastPortState != DOT1AG_RMEP_PORT_STATE_psUp)
          {
            rmepObj->dot1agCfmRmepVar.rMepPortStatusDefect = OFDPA_TRUE;
          }
          else
          {
            rmepObj->dot1agCfmRmepVar.rMepPortStatusDefect = OFDPA_FALSE;
          }
        }
        else
        {
          rmepObj->dot1agCfmMepDbPortStatusTlv = DOT1AG_RMEP_PORT_STATE_psNoPortStateTLV;
        }

        /* Copy Interface Status TLV */
        if (dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.interfaceStatusTLVRead == OFDPA_TRUE)
        {
          rmepObj->dot1agCfmMepDbInterfaceStatusTlv    = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.interfaceStatusTLV.interfaceStatus;
          rmepObj->dot1agCfmRmepVar.rMepLastIntfStatus = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.interfaceStatusTLV.interfaceStatus;

          if (rmepObj->dot1agCfmRmepVar.rMepLastIntfStatus != DOT1AG_RMEP_INTERFACE_STATUS_isNoInterfaceStatusTLV &&
              rmepObj->dot1agCfmRmepVar.rMepLastIntfStatus != DOT1AG_RMEP_INTERFACE_STATUS_isUp)
          {
            rmepObj->dot1agCfmRmepVar.rMepInterfaceStatusDefect = OFDPA_TRUE;
          }
          else
          {
            rmepObj->dot1agCfmRmepVar.rMepInterfaceStatusDefect = OFDPA_FALSE;
          }
        }
        else
        {
          rmepObj->dot1agCfmMepDbInterfaceStatusTlv = DOT1AG_RMEP_INTERFACE_STATUS_isNoInterfaceStatusTLV;
        }

        /* Copy Sender ID TLV */
        if (dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLVRead == OFDPA_TRUE)
        {
          rmepObj->dot1agCfmMepDbChassisIdLen     = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.chassisIdLen;
          rmepObj->dot1agCfmMepDbChassisIdSubtype = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.chassisIdSubtype;
          memcpy(rmepObj->dot1agCfmMepDbChassisId,
                 dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.chassisId,
                 dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.chassisIdLen);
          rmepObj->dot1agCfmMepDbManAddressDomainLen = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.addressDomainLen;
          memcpy(rmepObj->dot1agCfmMepDbManAddressDomain,
                 dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.addressDomain,
                 dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.addressDomainLen);
          rmepObj->dot1agCfmMepDbManAddressLen = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.manAddressLen;
          memcpy(rmepObj->dot1agCfmMepDbManAddress,
                 dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.manAddress,
                 dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.senderIdTLV.manAddressLen);
        }

        MAdefectIndicationNotify(mepData);

        break;

      case DOT1AG_RMEP_FAILED:
        rmepObj->dot1agCfmMepDbRMepFailedOkTime = dot1agCentisecondsGet();

        if (OFDPA_FALSE == rmepObj->dot1agCfmRmepVar.rMepCcmDefect)
        {
          /* report fault to APS component */
          /* retrieve the livenessPortId associated with this mep */
          if (ofdbInjectedOamLmepIdMapDataGet(dot1agCfmEventMsg->pMPObj->ofdpaLocalMpId, &oamMapData) == OFDPA_E_NONE)
          {
            g8131PgFaultReport(oamMapData->livenessLogicalPortId, 0);  /* down */
          }
        }

        rmepObj->dot1agCfmRmepVar.rMepCcmDefect = OFDPA_TRUE;
        mepData->mepPktChanged                  = OFDPA_TRUE;
        MAdefectIndicationNotify(mepData);

        if (NULL != rmepObj->dot1agCfmRmepVar.rmepTimer)
        {
          dot1agTimerDelete(rmepObj->dot1agCfmRmepVar.rmepTimer);
          rmepObj->dot1agCfmRmepVar.rmepTimer = NULL;
        }
        break;

      case DOT1AG_RMEP_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_RMEP, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, rmepObj->dot1agCfmRmepState, nextState);
    rmepObj->dot1agCfmRmepState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d",
                       dot1agEvent, rmepObj->dot1agCfmRmepState);
  }

  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the Remote MEP state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This is run per MEP though this is related to Errors generated by the RMEPS.
*           802.1ag d8-1 20.22
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmRMepErrStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agRmepErrStates_t  nextState, dot1agEvent;
  dot1agCfmStackObj_t   *pMEPObj;
  OFDPA_BOOL             UCT             = OFDPA_FALSE;
  OFDPA_ERROR_t          rc              = OFDPA_E_NONE;
  uint32_t               timeLeft        = 0;
  uint32_t               rmepErrInterval = 0, ccmInterval = 0;

  /* Get MEP Object */
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - dot1agRmepErrEventsStart - 1;
  nextState   = dot1agSMRmepErrTable[dot1agEvent][pMEPObj->mepData->dot1agCfmRmepErrState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_RMEP_ERRCCM_IDLE:
        {
          pMEPObj->mepData->dot1agCfmRmepErrVar.errorCcmDefect = OFDPA_FALSE;
          MAdefectIndicationNotify(pMEPObj->mepData);
          UCT = OFDPA_TRUE;
          nextState = DOT1AG_RMEP_ERRCCM_NO_DEFECT;
          rc = OFDPA_E_NONE;
          continue;
        }

      case DOT1AG_RMEP_ERRCCM_NO_DEFECT:
        {
          if (pMEPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer != NULL)
          {
            dot1agTimerDelete(pMEPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer);
            pMEPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer = NULL;
          }
          pMEPObj->mepData->dot1agCfmRmepErrVar.errorCcmDefect = OFDPA_FALSE;
          MAdefectIndicationNotify(pMEPObj->mepData);
          memset(pMEPObj->mepData->dot1agCfmRmepErrVar.errorCcmLastFailure,
                 0x0, OFDPA_DOT1AG_CCM_FRAME_LEN);

          break;
        }

      case DOT1AG_RMEP_ERRCCM_DEFECT:
        {
          if ((pMEPObj->mepData != NULL) &&
              (OFDPA_TRUE == dot1agIsMepHwEnabled
               (pMEPObj->mepData->dot1agCfmMepIdentifier)))
          {
            ccmInterval =
              CCMtime(dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.ccmInterval,
                      OFDPA_TRUE);
          }
          else
          {
            ccmInterval =
              CCMtime(dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.ccmInterval,
                      OFDPA_FALSE);
          }

          if (pMEPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer != NULL)
          {
            rmepErrInterval = DOT1AG_RMEP_INTERVAL_GET(ccmInterval);
            timeLeft = 0;
//            rmepErrInterval = max(timeLeft, rmepErrInterval);
            rmepErrInterval = (timeLeft > rmepErrInterval ? timeLeft : rmepErrInterval);
            rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer,
                                   pMEPObj, rmepErrInterval, dot1agCfmRmepErrTimerCallback);
            if (rc != OFDPA_E_NONE)
            {
              rc = OFDPA_E_FAIL;
              break;
            }
          }
          else
          {
            rmepErrInterval = DOT1AG_RMEP_INTERVAL_GET(ccmInterval);
            pMEPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer =
              dot1agTimerNodeAdd(rmepErrInterval, dot1agCfmRmepErrTimerCallback, pMEPObj);

            if (pMEPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer == NULL)
            {
              rc = OFDPA_E_FAIL;
              break;
            }
          }
          pMEPObj->mepData->dot1agCfmRmepErrVar.errorCcmDefect = OFDPA_TRUE;
          MAdefectIndicationNotify(pMEPObj->mepData);
          memcpy(pMEPObj->mepData->dot1agCfmRmepErrVar.errorCcmLastFailure,
                 dot1agCfmEventMsg->dot1agPkt->payLoad, OFDPA_DOT1AG_CCM_FRAME_LEN);

          break;
        }
      case DOT1AG_RMEP_ERRCCM_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  }while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_RMEP_ERR, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 pMEPObj->mepData->dot1agCfmRmepErrState, nextState);
    pMEPObj->mepData->dot1agCfmRmepErrState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       pMEPObj->mepData->dot1agCfmRmepErrState);
  }
  return rc;

}

/******************************************************************************
* @purpose  This routine calculates the next state for the Remote MEP state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This is run per MEP though this is related to Errors generated by the RMEPs.
*           802.1ag d8-1 20.24
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmMepXConStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agXConMepStates_t  nextState, dot1agEvent;
  uint32_t               timeLeft     = 0, xconCCMInterval = 0;
  dot1agCfmStackObj_t   *pMEPObj;
  OFDPA_BOOL             UCT          = OFDPA_FALSE;
  OFDPA_ERROR_t          rc           = OFDPA_E_NONE;
  uint32_t               rcvdInterval = 0;

  /* Get MEP Object */
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - dot1agXConEventsStart - 1;
  nextState = dot1agSMXConMepTable[dot1agEvent][pMEPObj->mepData->dot1agCfmMepXConState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_MEP_XCON_IDLE:
        {
          pMEPObj->mepData->dot1agCfmMepXConVar.xconCcmDefect = OFDPA_FALSE;
          MAdefectIndicationNotify(pMEPObj->mepData);
          UCT = OFDPA_TRUE;
          nextState = DOT1AG_MEP_XCON_NO_DEFECT;
          break;
        }

      case DOT1AG_MEP_XCON_NO_DEFECT:
        {
          if (pMEPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer != NULL)
          {
            dot1agTimerDelete(pMEPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer);
            pMEPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer = NULL;
          }
          pMEPObj->mepData->dot1agCfmMepXConVar.xconCcmDefect = OFDPA_FALSE;
          MAdefectIndicationNotify(pMEPObj->mepData);
          memset(pMEPObj->mepData->dot1agCfmMepXConVar.xconCcmLastFailure,
                 0x0, OFDPA_DOT1AG_CCM_FRAME_LEN);

          break;
        }

      case DOT1AG_MEP_XCON_DEFECT:
        {
          if ((pMEPObj->mepData != NULL) &&
              (OFDPA_TRUE == dot1agIsMepHwEnabled
               (pMEPObj->mepData->dot1agCfmMepIdentifier)))
          {
            rcvdInterval =
              CCMtime(dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.ccmInterval,
                      OFDPA_TRUE);
          }
          else
          {
            rcvdInterval =
              CCMtime(dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.ccmInterval,
                      OFDPA_FALSE);
          }

          if (pMEPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer != NULL)
          {
            xconCCMInterval = DOT1AG_RMEP_INTERVAL_GET(rcvdInterval);
            timeLeft = 0;
//              xconCCMInterval = max(timeLeft, xconCCMInterval);
            xconCCMInterval = (timeLeft > xconCCMInterval ? timeLeft : xconCCMInterval);
            rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer,
                                   pMEPObj, xconCCMInterval, dot1agCfmXConCCMTimerCallback);
            if (rc != OFDPA_E_NONE)
            {
              rc = OFDPA_E_FAIL;
              break;
            }
          }
          else
          {
            xconCCMInterval = DOT1AG_RMEP_INTERVAL_GET(rcvdInterval);
            pMEPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer =
              dot1agTimerNodeAdd(xconCCMInterval, dot1agCfmXConCCMTimerCallback, pMEPObj);

            if (pMEPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer == NULL)
            {
              rc = OFDPA_E_FAIL;
              break;
            }
          }
          pMEPObj->mepData->dot1agCfmMepXConVar.xconCcmDefect = OFDPA_TRUE;
          MAdefectIndicationNotify(pMEPObj->mepData);
          memcpy(pMEPObj->mepData->dot1agCfmMepXConVar.xconCcmLastFailure,
                 dot1agCfmEventMsg->dot1agPkt->payLoad, OFDPA_DOT1AG_CCM_FRAME_LEN);

          break;
        }
      case DOT1AG_MEP_XCON_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_XCON, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 pMEPObj->mepData->dot1agCfmMepXConState, nextState);
    pMEPObj->mepData->dot1agCfmMepXConState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       pMEPObj->mepData->dot1agCfmMepXConState);
  }
  return rc;
}

/***************************************************************
****************   LOOPBACK PROTOCOL FSMs   *******************
***************************************************************/

/******************************************************************************
* @purpose  This routine calculates the next state for the CFM LBI state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments MEP is only eligile for initiating the LBM.
*           802.1ag d8-1 20.30
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmLBIStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agLBIStates_t    nextState, dot1agEvent;
  dot1agCfmStackObj_t *pMEPObj;
  OFDPA_BOOL           UCT = OFDPA_FALSE;
  OFDPA_ERROR_t        rc  = OFDPA_E_NONE;
  uint32_t             lbmInterval;
  OFDPA_DOT1AG_MP_type_t mpType;

  /* Get MEP Object */
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  if (dot1agMpTypeGet(pMEPObj, &mpType) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - dot1agLBIEventsStart - 1;
  nextState = dot1agSMLBITable[dot1agEvent][pMEPObj->mepData->dot1agCfmMepLbiState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_LBI_IDLE:
        {
          pMEPObj->mepData->dot1agCfmMepLbiVar.lbmsToSend = 0;
          pMEPObj->mepData->dot1agCfmMepLbiVar.lbiActive = OFDPA_FALSE;
          pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmDiscoveryTTL = 0;
          pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmTransId = 0;
          pMEPObj->mepData->dot1agCfmMepLbiVar.lastRcvdLbrTransId = -1;
          pMEPObj->mepData->dot1agCfmMepTransmitLbmTestTlvPresent = OFDPA_FALSE;
          /* Delete the lbiTxTimer if it is running */
          if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer != NULL)
          {
            dot1agTimerDelete(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer);
            pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer = NULL;
          }
          rc = OFDPA_E_NONE;
        }
        break;

      case DOT1AG_LBI_STARTING:
        {
          if (pMEPObj->mepData->dot1agCfmMepTransmitLbmDiscovery != OFDPA_OAM_LB_DISCOVERY_NONE)
          {
            pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmDiscoveryTTL = 0;
            if (OFDPA_DOT1AG_MP_TYPE_BHH == mpType)
            {
              /* Start LBI Tx Timer for 250 milli seconds. */
              if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer != NULL)
              {
                rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer, pMEPObj,
                                       DOT1AG_CFM_LBI_TIMER_BHH_DISCOVERY_DEFAULT_MS, dot1agCfmLbiTxTimerCallback);
                if (rc != OFDPA_E_NONE)
                {
                  break;
                }
              }
              else
              {
                pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer = dot1agTimerNodeAdd(DOT1AG_CFM_LBI_TIMER_BHH_DISCOVERY_DEFAULT_MS,
                                                                                     dot1agCfmLbiTxTimerCallback, pMEPObj);
                if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer == NULL)
                {
                  rc = OFDPA_E_FAIL;
                  break;
                }
              }
            }
            else
            {
              /* Start LBI Timer for 5 seconds. */
              if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer != NULL)
              {
                rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer, pMEPObj,
                                       DOT1AG_CFM_LBI_TIMER_DEFAULT_MS, dot1agCfmLbiTimerCallback);
                if (rc != OFDPA_E_NONE)
                {
                  break;
                }
              }
              else
              {
                pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer = dot1agTimerNodeAdd(DOT1AG_CFM_LBI_TIMER_DEFAULT_MS,
                                                                                     dot1agCfmLbiTimerCallback, pMEPObj);
                if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer == NULL)
                {
                  rc = OFDPA_E_FAIL;
                  break;
                }
              }

            }
          }
          else
          {
            lbmInterval = pMEPObj->mepData->dot1agCfmMepTransmitLbmInterval;
            if (lbmInterval != 0)
            {
              /* Start LBI Timer with lbmInterval. */
              if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer != NULL)
              {
                rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer, pMEPObj,
                                       lbmInterval, dot1agCfmLbiTxTimerCallback);
                if (rc != OFDPA_E_NONE)
                {
                  break;
                }
              }
              else
              {
                pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer = dot1agTimerNodeAdd(lbmInterval,
                                                                                     dot1agCfmLbiTxTimerCallback, pMEPObj);
                if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer == NULL)
                {
                  rc = OFDPA_E_FAIL;
                  break;
                }
              }
            }
          }

          pMEPObj->mepData->dot1agCfmMepLbiVar.lbiActive = OFDPA_TRUE;
          pMEPObj->mepData->dot1agCfmMepLbrInOutOfOrder = 0;
          pMEPObj->mepData->dot1agCfmMepLbrIn = 0;
          nextState = DOT1AG_LBI_XMITTING;
          UCT = OFDPA_TRUE;
          break;
        }

      case DOT1AG_LBI_XMITTING:
        {
          if (pMEPObj->mepData->dot1agCfmMepTransmitLbmDiscovery != OFDPA_OAM_LB_DISCOVERY_NONE)
          {
            if (OFDPA_DOT1AG_MP_TYPE_BHH == mpType)
            {
              pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmDiscoveryTTL++;
              if (pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmDiscoveryTTL < 255)
              {
                nextState = DOT1AG_LBI_XMIT;
                UCT = OFDPA_TRUE;
              }
              else
              {
                if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer != NULL)
                {
                  nextState = DOT1AG_LBI_WAITING;
                }
                else
                {
                  nextState = DOT1AG_LBI_IDLE;
                  UCT = OFDPA_TRUE;
                }
              }
            }
            else
            {
              nextState = DOT1AG_LBI_XMIT;
              UCT = OFDPA_TRUE;
            }
          }
          else
          {
            if (pMEPObj->mepData->dot1agCfmMepTransmitLbmTestTlvPresent == OFDPA_TRUE)
            {
              /* Test */
              nextState = DOT1AG_LBI_XMIT;
              UCT = OFDPA_TRUE;
            }
            else
            {
              /* Series */
              if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbmsToSend != 0)
              {
                nextState = DOT1AG_LBI_XMIT;
                UCT = OFDPA_TRUE;
              }
              else
              {
                if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer != NULL)
                {
                  nextState = DOT1AG_LBI_WAITING;
                }
                else
                {
                  nextState = DOT1AG_LBI_IDLE;
                  UCT = OFDPA_TRUE;
                }
              }
            }
          }
        }
        break;

      case DOT1AG_LBI_XMIT:
        {
          rc = xmitLBM(pMEPObj);
          if (rc == OFDPA_E_FAIL)
          {
            return rc;
          }

          if (pMEPObj->mepData->dot1agCfmMepTransmitLbmDiscovery != OFDPA_OAM_LB_DISCOVERY_NONE)
          {
            if ((OFDPA_DOT1AG_MP_TYPE_BHH == mpType) && (pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmDiscoveryTTL < 255))
            {
              if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer != NULL)
              {
                rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer, pMEPObj,
                                       DOT1AG_CFM_LBI_TIMER_BHH_DISCOVERY_DEFAULT_MS, dot1agCfmLbiTxTimerCallback);
                if (rc != OFDPA_E_NONE)
                {
                  rc = OFDPA_E_FAIL;
                  break;
                }
              }
              else
              {
                rc = OFDPA_E_INTERNAL;
                break;
              }
            }
            else
            {
              if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer != NULL)
              {
                rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer, pMEPObj,
                                       DOT1AG_CFM_LBI_TIMER_DEFAULT_MS, dot1agCfmLbiTimerCallback);
                if (rc != OFDPA_E_NONE)
                {
                  rc = OFDPA_E_FAIL;
                  break;
                }
              }
              else
              {
                rc = OFDPA_E_INTERNAL;
                break;
              }
            }
          }
          else
          {
            /* G.8021, section 8.1.8.2)*/
            lbmInterval = pMEPObj->mepData->dot1agCfmMepTransmitLbmInterval;
            if (lbmInterval == 0)
            {
              pMEPObj->mepData->dot1agCfmMepLbiVar.lbmsToSend--;
              nextState = DOT1AG_LBI_XMITTING;
              UCT = OFDPA_TRUE;
            }
            else
            {
              if (pMEPObj->mepData->dot1agCfmMepTransmitLbmTestTlvPresent == OFDPA_TRUE)
              {
                /* Test */
                /* Start LBI Tx Timer */
                if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer != NULL)
                {
                  rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer, pMEPObj,
                                         lbmInterval,
                                         dot1agCfmLbiTxTimerCallback);
                  if (rc != OFDPA_E_NONE)
                  {
                    break;
                  }
                }
                else
                {
                  pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer = dot1agTimerNodeAdd(lbmInterval,
                                                                                       dot1agCfmLbiTxTimerCallback, pMEPObj);
                  if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer == NULL)
                  {
                    rc = OFDPA_E_FAIL;
                    break;
                  }
                }
              }
              else
              {
                /* Series */
                if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbmsToSend > 1)
                {
                  /* Start LBI Tx Timer */
                  if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer != NULL)
                  {
                    rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer, pMEPObj,
                                           lbmInterval,
                                           dot1agCfmLbiTxTimerCallback);
                    if (rc != OFDPA_E_NONE)
                    {
                      break;
                    }
                  }
                  else
                  {
                    pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer = dot1agTimerNodeAdd(lbmInterval,
                                                                                         dot1agCfmLbiTxTimerCallback, pMEPObj);
                    if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTxTimer == NULL)
                    {
                      rc = OFDPA_E_FAIL;
                      break;
                    }
                  }
                  pMEPObj->mepData->dot1agCfmMepLbiVar.lbmsToSend--;
                }
                else
                {
                  /* Start LBI Timer for 5 seconds. */
                  if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer != NULL)
                  {
                    rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer, pMEPObj,
                                           DOT1AG_CFM_LBI_TIMER_DEFAULT_MS, dot1agCfmLbiTimerCallback);
                    if (rc != OFDPA_E_NONE)
                    {
                      break;
                    }
                  }
                  else
                  {
                    pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer = dot1agTimerNodeAdd(DOT1AG_CFM_LBI_TIMER_DEFAULT_MS,
                                                                                       dot1agCfmLbiTimerCallback, pMEPObj);
                    if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer == NULL)
                    {
                      rc = OFDPA_E_FAIL;
                      break;
                    }
                  }
                }
              }
            }
          }
          break;
        }
      case DOT1AG_LBI_WAITING:
        {
          /* Nothing todo under this state.*/
        }
        break;
      case DOT1AG_LBI_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_LBI, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 pMEPObj->mepData->dot1agCfmMepLbiState, nextState);
    pMEPObj->mepData->dot1agCfmMepLbiState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       pMEPObj->mepData->dot1agCfmMepLbiState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the CFM LBIR state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns OFDPA_E_NONE
* @returns OFDPA_E_FAIL
*
* @comments Please not LBR can only processed by MEP as MEP is only eligile for
*           initiating the LBM.
*           802.1ag d8-1 20.32
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmLBIRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agLBIRStates_t     nextState, dot1agEvent;
  dot1agCfmMepTable_t   *mepObj;
  OFDPA_BOOL             UCT = OFDPA_FALSE;
  OFDPA_ERROR_t          rc  = OFDPA_E_NONE;
  OFDPA_DOT1AG_MP_type_t mpType = OFDPA_DOT1AG_MP_TYPE_ETH;

  if (dot1agCfmEventMsg->pMPObj == NULL ||
      dot1agCfmEventMsg->pMPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  mepObj      = dot1agCfmEventMsg->pMPObj->mepData;
  dot1agEvent = dot1agCfmEventMsg->event - dot1agLBIREventsStart - 1;
  nextState   = dot1agSMLBIRTable[dot1agEvent][mepObj->dot1agCfmMepLBIRState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_LBIR_IDLE:
        {
          rc = OFDPA_E_NONE;
        }
        break;

      case DOT1AG_LBIR_RESPOND:
        {
          ProcessLBR(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);

          /*
             Check ID Sub-Type to see if LBR is sent by MEP or MIP.
             If ID Sub-Type is 0x2 then LBR is sent by MEP.
             If ID Sub-Type is 0x3 then LBR is sent by MEP.
          */
          if ((mepObj->dot1agCfmMepTransmitLbmDiscovery != OFDPA_OAM_LB_DISCOVERY_NONE))
          {
            /* get MP type */
            rc = dot1agMpTypeGet(dot1agCfmEventMsg->pMPObj, &mpType);
            if (rc != OFDPA_E_NONE)
            {
              return (rc);
            }

            if ((OFDPA_DOT1AG_MP_TYPE_BHH == mpType) &&
                (*(dot1agCfmEventMsg->dot1agPkt->cfmPdu.lbr.pFirstTLV + DOT1AG_BHH_TLV_ID_SUBTYPE_OFFSET) == DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MEPID))
            {
              /* Start LBI Timer with 1 milli seconds period to stop LB test immediately. */
              if (mepObj->dot1agCfmMepLbiVar.lbiTimer != NULL)
              {
                rc = dot1agTimerUpdate(mepObj->dot1agCfmMepLbiVar.lbiTimer, dot1agCfmEventMsg->pMPObj,
                                       1, dot1agCfmLbiTimerCallback);
                if (rc != OFDPA_E_NONE)
                {
                  break;
                }
              }
              else
              {
                mepObj->dot1agCfmMepLbiVar.lbiTimer = dot1agTimerNodeAdd(1, dot1agCfmLbiTimerCallback,
                                                                         dot1agCfmEventMsg->pMPObj);
                if (mepObj->dot1agCfmMepLbiVar.lbiTimer == NULL)
                {
                  rc = OFDPA_E_FAIL;
                  break;
                }
              }
            }
          }

          nextState = DOT1AG_LBI_IDLE;
          UCT = OFDPA_TRUE;
          rc = OFDPA_E_NONE;
          continue;
        }
      case DOT1AG_LBIR_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  }while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_LBIR, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 mepObj->dot1agCfmMepLBIRState, nextState);
    mepObj->dot1agCfmMepLBIRState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       mepObj->dot1agCfmMepLBIRState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the CFM LBR state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns OFDPA_E_NONE
* @returns OFDPA_E_FAIL
*
* @comments This can be generate dby any MP.
*           802.1ag d8-1 20.33
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmLBRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agLBRStates_t nextState, dot1agEvent;
  OFDPA_BOOL        UCT = OFDPA_FALSE;
  OFDPA_ERROR_t     rc  = OFDPA_E_NONE;

  if (dot1agCfmEventMsg->pMPObj == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - dot1agLBREventsStart - 1;
  nextState = dot1agSMLBRTable[dot1agEvent][dot1agCfmEventMsg->pMPObj->dot1agCfmLbrMpState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_LBR_IDLE:
        {
          rc = OFDPA_E_NONE;
        }
        break;

      case DOT1AG_LBR_RESPOND:
        {
          ProcessLBM(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);
          nextState = DOT1AG_LBR_IDLE;
          UCT = OFDPA_TRUE;
          rc = OFDPA_E_NONE;
          continue;
        }
      case DOT1AG_LBR_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_LBR, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 dot1agCfmEventMsg->pMPObj->dot1agCfmLbrMpState, nextState);
    dot1agCfmEventMsg->pMPObj->dot1agCfmLbrMpState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       dot1agCfmEventMsg->pMPObj->dot1agCfmLbrMpState);
  }
  return rc;
}

/***************************************************************
****************   LINKTRACE PROTOCOL FSMs   *******************
***************************************************************/

/******************************************************************************
* @purpose  This routine calculates the next state for the CFM LTIR state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments MEP is only eligile for initiating the LTM.
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmLTIRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agLTIRStates_t   nextState, dot1agEvent;
  dot1agCfmMepTable_t *mepObj;
  OFDPA_BOOL           UCT = OFDPA_FALSE;
  OFDPA_ERROR_t        rc  = OFDPA_E_FAIL;

  if (dot1agCfmEventMsg == NULL ||
      dot1agCfmEventMsg->pMPObj == NULL ||
      dot1agCfmEventMsg->pMPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  mepObj      = dot1agCfmEventMsg->pMPObj->mepData;
  dot1agEvent = dot1agCfmEventMsg->event - dot1agLTIREventsStart - 1;
  nextState   = dot1agSMLTIRTable[dot1agEvent][mepObj->dot1agCfmMepLtirState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_LTIR_IDLE:
        break;

      case DOT1AG_LTIR_RESPOND:
        {
          rc = ProcessLTR(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);
          nextState = DOT1AG_LTIR_IDLE;
          UCT = OFDPA_TRUE;
          rc = OFDPA_E_NONE;
          continue;
        }
        break;

      case DOT1AG_LTIR_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_LTIR, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 mepObj->dot1agCfmMepLtirState, nextState);
    mepObj->dot1agCfmMepLtirState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       mepObj->dot1agCfmMepLtirState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the CFM LTRR state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This SM runs per bridge.
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmLTRRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agLTRRStates_t nextState, dot1agEvent;
  OFDPA_BOOL         UCT = OFDPA_FALSE;
  OFDPA_ERROR_t      rc  = OFDPA_E_FAIL;

  if (dot1agCfmEventMsg == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - dot1agLTRREventsStart - 1;
  nextState = dot1agSMLTRRTable[dot1agEvent][dot1agCFM.dot1agCfmLtrrState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_LTRR_IDLE:
        break;

      case DOT1AG_LTRR_RESPOND:
        {
          rc = ProcessLTM(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);
          nextState = DOT1AG_LTIR_IDLE;
          UCT = OFDPA_TRUE;
          rc = OFDPA_E_NONE;
          continue;
        }
        break;

      case DOT1AG_LTRR_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_LTIR, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 dot1agCFM.dot1agCfmLtrrState, nextState);
    dot1agCFM.dot1agCfmLtrrState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       dot1agCFM.dot1agCfmLtrrState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the CFM LTRR state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This SM runs per bridge.
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmLTRTStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agLTRTStates_t nextState, dot1agEvent;
  OFDPA_BOOL         UCT = OFDPA_FALSE;
  OFDPA_ERROR_t      rc  = OFDPA_E_NONE;

  if (dot1agCfmEventMsg == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - dot1agLTRTEventsStart - 1;
  nextState = dot1agSMLTRTTable[dot1agEvent][dot1agCFM.dot1agCfmLtrtState];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_LTRT_IDLE:
        {
          if (dot1agCFM.dot1agCfmLtmrVar.ltfTimer != NULL)
          {
            dot1agTimerDelete(dot1agCFM.dot1agCfmLtmrVar.ltfTimer);
            dot1agCFM.dot1agCfmLtmrVar.ltfTimer = NULL;
          }

          clearPendingLTRs();
          dot1agCFM.dot1agCfmLtmrVar.nPendingLTRs = 0;
          nextState = DOT1AG_LTRT_WAITING;
          UCT = OFDPA_TRUE;
          rc = OFDPA_E_NONE;
          continue;
        }
        break;

      case DOT1AG_LTRT_WAITING:
        {
#if OFDPA_NOT_SUPPORTED
          if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
            dot1agCFM.dot1agCfmLtmrVar.ltfTimer = appTimerAdd(dot1agCFM.timerCB, dot1agCfmLtfTimerCallback, &dot1agCFM,
                                                              DOT1AG_CFM_LTF_TIMER_DEFAULT, "dot1agCfmLTFTmr");
          else
#endif
          {
            if (dot1agCFM.dot1agCfmLtmrVar.ltfTimer == NULL)
            {
              dot1agCFM.dot1agCfmLtmrVar.ltfTimer = dot1agTimerNodeAdd(DOT1AG_CFM_LTF_TIMER_DEFAULT_MS,
                                                                       dot1agCfmLtfTimerCallback, &dot1agCFM);
              if (dot1agCFM.dot1agCfmLtmrVar.ltfTimer == NULL)
              {
                rc = OFDPA_E_FAIL;
                break;
              }
            }
            else
            {
              rc = dot1agTimerUpdate(dot1agCFM.dot1agCfmLtmrVar.ltfTimer, &dot1agCFM,
                                     DOT1AG_CFM_LTF_TIMER_DEFAULT_MS, dot1agCfmLtfTimerCallback);
              if (rc != OFDPA_E_NONE)
              {
                rc = OFDPA_E_FAIL;
                break;
              }
            }
          }

        }
        break;
      case DOT1AG_LTRT_XMITTING:
        {
          xmitOldestLTR();
          nextState = DOT1AG_LTRT_WAITING;
          UCT = OFDPA_TRUE;
        }
        break;

      case DOT1AG_LTRT_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_LTRT, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 dot1agCFM.dot1agCfmLtrtState, nextState);
    dot1agCFM.dot1agCfmLtrtState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       dot1agCFM.dot1agCfmLtrtState);
  }
  return rc;
}

/***************************************************************
****************  FAULT NOTIFICATION GENERATOR FSM   ***********
***************************************************************/

/******************************************************************************
* @purpose  This routine calculates the next state for the MEP FNG state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This receiver SM is applicable for MEP.
*           802.1ag d8-1 20.35
*
* @end
*******************************************************************************/
OFDPA_ERROR_t dot1agCfmMepFNGStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  dot1agMepFNGStates_t  nextState, dot1agEvent, prevState = DOT1AG_FNG_DEFAULT;
  dot1agCfmMepTable_t  *mepData;
  OFDPA_BOOL            UCT                               = OFDPA_FALSE;
  OFDPA_ERROR_t         rc                                = OFDPA_E_NONE;
  uint32_t              fngAlarmTime                      = 0;
  uint32_t              fngResetTime                      = 0;

  /* Get MEP Object */
  mepData = dot1agCfmEventMsg->pMepData;
  if (mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  fngAlarmTime = DOT1AG_FNG_TIME_GET(mepData->dot1agCfmMepFngAlarmTime);
  fngResetTime = DOT1AG_FNG_TIME_GET(mepData->dot1agCfmMepFngResetTime);

  dot1agEvent = dot1agCfmEventMsg->event - dot1agFNGEventsStart - 1;
  nextState = dot1agSMMepFNGTable[dot1agEvent][mepData->dot1agCfmMepSMFngState];
  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case DOT1AG_FNG_RESET:
        {
          /* No action to do in this state.*/
          if (mepData->dot1agCfmMepFngVar.fngTimer != NULL)
          {
            if (dot1agCfmFngTimerCallback == ((dot1ag_timer_list_t *)(mepData->dot1agCfmMepFngVar.fngTimer))->node.func_p)
            {
              dot1agTimerDelete(mepData->dot1agCfmMepFngVar.fngTimer);
              mepData->dot1agCfmMepFngVar.fngTimer = NULL;
            }
          }
          break;
        }

      case DOT1AG_FNG_DEFECT:
        {
          mepData->dot1agCfmMepFngVar.fngTimer = dot1agTimerNodeAdd(fngAlarmTime,
                                                                    dot1agCfmFngTimerCallback, mepData);

          if (mepData->dot1agCfmMepFngVar.fngTimer == NULL)
          {
            rc = OFDPA_E_FAIL;
            break;
          }
          /* No action to do in this state.*/
          break;
        }

      case DOT1AG_FNG_REPORT_DEFECT:
        {
          mepData->dot1agCfmMepFngVar.fngPriority = mepData->dot1agCfmMepFngVar.highestDefectPri;
          mepData->dot1agCfmMepFngVar.fngDefect   = mepData->dot1agCfmMepFngVar.highestDefect;

          xmitFaultAlarm(mepData->maData->mdData->dot1agCfmMdIndex,
                         mepData->maData->dot1agCfmMaIndex,
                         mepData->dot1agCfmMepIdentifier,
                         mepData->dot1agCfmMepFngVar.fngDefect);

          nextState = DOT1AG_FNG_DEFECT_REPORTED;
          prevState = DOT1AG_FNG_REPORT_DEFECT;
          UCT = OFDPA_TRUE;
          rc = OFDPA_E_NONE;
          continue;
        }

      case DOT1AG_FNG_DEFECT_REPORTED:
        {
          if (prevState != DOT1AG_FNG_REPORT_DEFECT)
          {
            mepData->dot1agCfmMepFngVar.fngPriority = mepData->dot1agCfmMepFngVar.highestDefectPri;
            mepData->dot1agCfmMepFngVar.fngDefect   = mepData->dot1agCfmMepFngVar.highestDefect;

            xmitFaultAlarm(mepData->maData->mdData->dot1agCfmMdIndex,
                           mepData->maData->dot1agCfmMaIndex,
                           mepData->dot1agCfmMepIdentifier,
                           mepData->dot1agCfmMepFngVar.fngDefect);
          }
          break;
        }

      case DOT1AG_FNG_DEFECT_CLEARING:
        {
          mepData->dot1agCfmMepFngVar.fngDefect   = mepData->dot1agCfmMepFngVar.highestDefect;
          xmitFaultAlarm(mepData->maData->mdData->dot1agCfmMdIndex,
                         mepData->maData->dot1agCfmMaIndex,
                         mepData->dot1agCfmMepIdentifier,
                         mepData->dot1agCfmMepFngVar.fngDefect);

          if (mepData->dot1agCfmMepFngVar.fngTimer == NULL)
          {
            mepData->dot1agCfmMepFngVar.fngTimer = dot1agTimerNodeAdd(fngResetTime,
                                                                      dot1agCfmFngTimerCallback, mepData);

            if (mepData->dot1agCfmMepFngVar.fngTimer == NULL)
            {
              rc = OFDPA_E_FAIL;
              break;
            }
          }
          else
          {
            rc = dot1agTimerUpdate(mepData->dot1agCfmMepFngVar.fngTimer,
                                   mepData, fngResetTime, dot1agCfmFngTimerCallback);

            if (rc != OFDPA_E_NONE)
            {
              rc = OFDPA_E_FAIL;
              break;
            }
          }
          prevState = DOT1AG_FNG_DEFECT_CLEARING;
        }
        break;
      case DOT1AG_FNG_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_SM_FNG, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 mepData->dot1agCfmMepSMFngState, nextState);
    mepData->dot1agCfmMepSMFngState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       mepData->dot1agCfmMepSMFngState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the PROLM state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns OFDPA_E_NONE
* @returns OFDPA_E_FAIL
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmProLMStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731MepProlmStates_t  nextState, dot1agEvent;
  dot1agCfmMepTable_t   *mepData;
//  void *        appTimer;
  OFDPA_BOOL             UCT = OFDPA_FALSE;
  OFDPA_ERROR_t          rc  = OFDPA_E_NONE;
  y1731CfmMepLmPrim      prim;
  uint8_t                priority;
  dot1agCfmSlmObj_t      *pSlmEntry = NULL;
  uint32_t               controlPeriod = 0;

  if (dot1agCfmEventMsg->pMPObj == NULL ||
      dot1agCfmEventMsg->pMPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  memset(&prim, 0x0, sizeof(prim));
  priority = dot1agCfmEventMsg->priority;
  mepData      = dot1agCfmEventMsg->pMPObj->mepData;
  dot1agEvent = dot1agCfmEventMsg->event - y1731PROLMEventsStart - 1;
  nextState   = y1731SMProlmTable[dot1agEvent][mepData->y1731CfmMepProlmState];
  if (mepData->y1731CfmMepProlmEntry == NULL ||
      mepData->y1731CfmMepProlmEntry->prolmEnabled == OFDPA_FALSE)
  {
    DOT1AG_TRACE(CFM_LM, "ProlmEntry is null, Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 mepData->y1731CfmMepProlmState, nextState);
    return OFDPA_E_NONE;
  }

  controlPeriod = CCMtime(mepData->y1731CfmMepProlmEntry->prolmControlPeriod, OFDPA_FALSE);

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case Y1731_PROLM_DISABLE:
        {
          /* Delete the Timer if running */
          if (mepData->y1731CfmMepProlmVar.prolmTimer != NULL)
          {
            dot1agTimerDelete(mepData->y1731CfmMepProlmVar.prolmTimer);
            mepData->y1731CfmMepProlmVar.prolmTimer = NULL;
          }
          mepData->y1731CfmMepProlmVar.prolmEnabled = OFDPA_FALSE;
          mepData->y1731CfmMepProlmEntry->prolmEnabled = OFDPA_FALSE;
          /*memset(mepData->y1731CfmMepProlmEntry, 0x0, sizeof(mepData->y1731CfmMepProlmEntry)); */
          rc = OFDPA_E_NONE;
        }
        break;

      case Y1731_PROLM_ENABLE:
        {
          if (dot1agCfmEventMsg->event == y1731PROLMCCMRecvd)
          {
            if (mepData->y1731CfmMepProlmEntry->saved)
            {
              prim.nNTF = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCf -
                mepData->y1731CfmMepProlmEntry->txFCfPrev;
              prim.nNLF = prim.nNTF -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.rxFCl - mepData->y1731CfmMepProlmEntry->rxFClPrev);
              prim.nFTF = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCb -
                mepData->y1731CfmMepProlmEntry->txFCbPrev;
              prim.nFLF = prim.nFTF -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.rxFCb - mepData->y1731CfmMepProlmEntry->rxFcbPrev);
              /*filter*/
              if (((int)prim.nNTF < 0) || ((int)prim.nNLF < 0))
              {
                prim.nNTF = prim.nNLF = 0;
              }
              if (((int)prim.nFTF < 0) || ((int)prim.nFLF < 0))
              {
                prim.nFTF = prim.nFLF = 0;
              }
              dpaRWlockWritelock(&y1731PmCFM.y1731PmCfgRWLock);
              /*memcpy(&mepData->y1731CfmMepProlmEntry->lmPrim, &prim, sizeof(mepData->y1731CfmMepProlmEntry->lmPrim)); */
              mepData->y1731CfmMepProlmEntry->lmPrim.nNTF += prim.nNTF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nNLF += prim.nNLF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nFTF += prim.nFTF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nFLF += prim.nFLF;
              dpaRWlockUnlock(&y1731PmCFM.y1731PmCfgRWLock);
            }

            /* info is valid if txFCb is non-zero */
            if (0 != dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCb &&
                0 != dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCf)
            {
              mepData->y1731CfmMepProlmEntry->saved = OFDPA_TRUE;
            }
            else
            {
              mepData->y1731CfmMepProlmEntry->saved = OFDPA_FALSE;
              DOT1AG_TRACE(CFM_LM, "wrong tx counter  %d, %d within ccm\n",
                     dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCf,
                     dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCb);
            }
            mepData->y1731CfmMepProlmEntry->txFCfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCf;
            mepData->y1731CfmMepProlmEntry->rxFClPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.rxFCl;
            mepData->y1731CfmMepProlmEntry->txFCbPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCb;
            mepData->y1731CfmMepProlmEntry->rxFcbPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.rxFCb;

            DOT1AG_TRACE(CFM_LM, "pdu is %d, %d, %d, %d\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.rxFCb,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.txFCb,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.rxFCl);
            DOT1AG_TRACE(CFM_LM, "primitive is %d, %d, %d, %d\r\n",
                         mepData->y1731CfmMepProlmEntry->lmPrim.nNTF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nNLF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nFTF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nFLF);
          }
          else if (dot1agCfmEventMsg->event == y1731PROLMLMRRecvd)
          {
            dot1agParseLMR(dot1agCfmEventMsg->dot1agPkt);
            if (mepData->y1731CfmMepProlmEntry->saved)
            {
              prim.nNTF =
                dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCb - mepData->y1731CfmMepProlmEntry->txFCbPrev;
              prim.nNLF =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCb - mepData->y1731CfmMepProlmEntry->txFCbPrev) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCl - mepData->y1731CfmMepProlmEntry->rxFClPrev);
              prim.nFTF =
                dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCf - mepData->y1731CfmMepProlmEntry->txFCfPrev;
              prim.nFLF =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCf - mepData->y1731CfmMepProlmEntry->txFCfPrev) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCf - mepData->y1731CfmMepProlmEntry->rxFCfPrev);

              /*filter*/
              if (((int)prim.nNTF < 0) || ((int)prim.nNLF < 0))
              {
                prim.nNTF = prim.nNLF = 0;
              }
              if (((int)prim.nFTF < 0) || ((int)prim.nFLF < 0))
              {
                prim.nFTF = prim.nFLF = 0;
              }

              dpaRWlockWritelock(&y1731PmCFM.y1731PmCfgRWLock);
              mepData->y1731CfmMepProlmEntry->lmPrim.nNTF += prim.nNTF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nNLF += prim.nNLF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nFTF += prim.nFTF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nFLF += prim.nFLF;
              dpaRWlockUnlock(&y1731PmCFM.y1731PmCfgRWLock);
            }
            mepData->y1731CfmMepProlmEntry->txFCfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCf;
            mepData->y1731CfmMepProlmEntry->rxFCfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCf;
            mepData->y1731CfmMepProlmEntry->txFCbPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCb;
            mepData->y1731CfmMepProlmEntry->rxFClPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCl;
            mepData->y1731CfmMepProlmEntry->saved = OFDPA_TRUE;

            DOT1AG_TRACE(CFM_LM, "pdu is %d, %d, %d, %d\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCb,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCl);
            DOT1AG_TRACE(CFM_LM, "primitive is %d, %d, %d, %d\r\n",
                         mepData->y1731CfmMepProlmEntry->lmPrim.nNTF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nNLF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nFTF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nFLF);
          }
          else if (dot1agCfmEventMsg->event == y1731PROLMSLRRecvd)
          {
            dot1agParseSLR(dot1agCfmEventMsg->dot1agPkt);
            if (mepData->y1731CfmMepProlmEntry->saved)
            {
              pSlmEntry = dot1agCfmSlmObjGet(dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.srcMepId,
                                             dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rspMepId,
                                             dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.testId, AVL_EXACT);

              if (NULL != pSlmEntry)
              {
                pSlmEntry->data.rxFCl++;
              }
              else
              {
                return OFDPA_E_FAIL;
              }

              dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCl = pSlmEntry->data.rxFCl;

              prim.nNTF = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCf - mepData->y1731CfmMepProlmEntry->txFCfPrev;
              prim.nFTF = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb - mepData->y1731CfmMepProlmEntry->txFCbPrev;
              prim.nNLF =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb - mepData->y1731CfmMepProlmEntry->txFCbPrev) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCl - mepData->y1731CfmMepProlmEntry->rxFClPrev);
              prim.nFLF =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCf - mepData->y1731CfmMepProlmEntry->txFCfPrev) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb - mepData->y1731CfmMepProlmEntry->txFCbPrev);

              /*filter*/
              if (((int)prim.nNTF < 0) || ((int)prim.nNLF < 0))
              {
                prim.nNTF = prim.nNLF = 0;
              }

              if (((int)prim.nFTF < 0) || ((int)prim.nFLF < 0))
              {
                prim.nFTF = prim.nFLF = 0;
              }

              dpaRWlockWritelock(&y1731PmCFM.y1731PmCfgRWLock);
              mepData->y1731CfmMepProlmEntry->lmPrim.nNTF += prim.nNTF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nNLF += prim.nNLF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nFTF += prim.nFTF;
              mepData->y1731CfmMepProlmEntry->lmPrim.nFLF += prim.nFLF;
              dpaRWlockUnlock(&y1731PmCFM.y1731PmCfgRWLock);
            }
            mepData->y1731CfmMepProlmEntry->txFCfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCf;
            mepData->y1731CfmMepProlmEntry->rxFCfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCf;
            mepData->y1731CfmMepProlmEntry->txFCbPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb;
            mepData->y1731CfmMepProlmEntry->rxFClPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCl;
            mepData->y1731CfmMepProlmEntry->saved = OFDPA_TRUE;

            DOT1AG_TRACE(CFM_LM, "pdu is %d, %d, %d, %d\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCl);

            DOT1AG_TRACE(CFM_LM, "primitive is %d, %d, %d, %d\r\n",
                         mepData->y1731CfmMepProlmEntry->lmPrim.nNTF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nNLF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nFTF,
                         mepData->y1731CfmMepProlmEntry->lmPrim.nFLF);
          }
          else
          {
            if (mepData->y1731CfmMepProlmVar.prolmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_LM) /* LMM based LM */
            {
              /*send LMM*/
              rc = xmitProLMM(dot1agCfmEventMsg->pMPObj, priority);
            }
            else if (mepData->y1731CfmMepProlmVar.prolmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM) /* SLM based LM */
            {
              /*send SLM*/
              rc = xmitProSLM(dot1agCfmEventMsg->pMPObj, priority);
            }

            if (rc == OFDPA_E_FAIL)
            {
              return rc;
            }

            /* clear primitive by PM*/
#if 0
            mepData->y1731CfmMepProlmEntry->lmPrim.nNTF = 0;
            mepData->y1731CfmMepProlmEntry->lmPrim.nNLF = 0;
            mepData->y1731CfmMepProlmEntry->lmPrim.nFTF = 0;
            mepData->y1731CfmMepProlmEntry->lmPrim.nFLF = 0;
#endif
            /* Start PROLM Timer for 1 seconds. */
            if (mepData->y1731CfmMepProlmVar.prolmTimer != NULL)
            {
              rc = dot1agTimerUpdate(mepData->y1731CfmMepProlmVar.prolmTimer, mepData->y1731CfmMepProlmEntry,
                                     controlPeriod, y1731CfmProlmTimerCallback);
              if (rc != OFDPA_E_NONE)
              {
                break;
              }
            }
            else
            {
              mepData->y1731CfmMepProlmVar.prolmTimer = dot1agTimerNodeAdd(controlPeriod,
                                                                           y1731CfmProlmTimerCallback,
                                                                           mepData->y1731CfmMepProlmEntry);
              if (mepData->y1731CfmMepProlmVar.prolmTimer == NULL)
              {
                rc = OFDPA_E_FAIL;
                break;
              }
            }
          }
        }
        break;
      case Y1731_PROLM_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_LM, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 mepData->y1731CfmMepProlmState, nextState);
    mepData->y1731CfmMepProlmState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d CurrState %d", dot1agEvent,
                       mepData->y1731CfmMepProlmState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the ONDEM LM INIT state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns OFDPA_E_NONE
* @returns OFDPA_E_FAIL
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmOndemLMMStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731OndemLMMStates_t  nextState, dot1agEvent;
  dot1agCfmMepTable_t   *mepData;
//  void *       appTimer;
  uint8_t                priority;
  OFDPA_BOOL             UCT           = OFDPA_FALSE;
  OFDPA_ERROR_t          rc            = OFDPA_E_NONE;
  uint32_t               controlPeriod = 0;
  y1731CfmMepLmPrim      prim;
//  L7_clocktime            ct;
  uint8_t                bufOutput[DOT1AG_LBM_DATA_LEN];
  OFDPA_BOOL             isLmPrimValid = OFDPA_TRUE, transmitLM = OFDPA_FALSE;
  OFDPA_BOOL             N_valid, F_valid;
  dot1agCfmSlmObj_t     *pSlmEntry = NULL, slmEntry;

  if (dot1agCfmEventMsg->pMPObj == NULL ||
      dot1agCfmEventMsg->pMPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  memset(&prim, 0x0, sizeof(prim));
  priority = dot1agCfmEventMsg->priority;
  mepData      = dot1agCfmEventMsg->pMPObj->mepData;
  dot1agEvent = dot1agCfmEventMsg->event - y1731ONDEMLMMEventsStart - 1;
  if (mepData->y1731CfmMepOndemlmEntry[priority] == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "MepId %d, pri %d: OndemlmEntry is null, Event %d CurrState %d, pri %d",
                       mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                       mepData->y1731CfmOndemLMMState[priority], priority);
    return OFDPA_E_NONE;
  }

  nextState   = y1731SMOndemLMMTable[dot1agEvent][mepData->y1731CfmOndemLMMState[priority]];
  controlPeriod = CCMtime(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmControlPeriod, OFDPA_FALSE);
  DOT1AG_TRACE(CFM_LM, "MepId %d, pri %d: Event %d CurrState %d, NextState %d\r\n",
               mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
               dot1agCfmEventMsg->pMPObj->mepData->y1731CfmOndemLMMState[priority], nextState);

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case Y1731_ONDEMLMM_IDLE:
        {
          if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)
          {
            /*start timeout timer for the final SLR*/
            /*
               ITU-T G.8013/Y.1731 8.4.1.3 SLR reception
               At the end of SLM based LM test we need to wait for 5 seconds and process SLR PDUs in that duration.
            */
            controlPeriod = 5000;
            DOT1AG_TRACE(CFM_LM, "final timer\r\n");

            if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer != NULL)
            {
              DOT1AG_TRACE(CFM_LM, "update timer\r\n");
              rc = dot1agTimerUpdate(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer,
                                     mepData->y1731CfmMepOndemlmEntry[priority],
                                     controlPeriod, y1731CfmOndemlmTimerCallback);
              if (rc != OFDPA_E_NONE)
              {
                break;
              }
            }
          }
          else
          {
            /* Delete the Timer if running */
            if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer != NULL)
            {
              DOT1AG_TRACE(CFM_LM, "remove timer\r\n");
              dot1agTimerDelete(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer);
              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer = NULL;
            }
            mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmEnabled = OFDPA_FALSE;

            if (mepData->y1731CfmMepOndemlmEntry[priority]->saved)
            { /*give report*/
              DOT1AG_TRACE(CFM_LM, "OndemLM result for mepid %d, pri %d as follows: \n\n",
                           mepData->dot1agCfmMepIdentifier, priority);
              y1731PmGetLmBinEvent(isLmPrimValid, mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNTF,
                                   mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNLF, OFDPA_FALSE,
                                   &(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.tNFLRCnt), &N_valid);
              y1731PmGetLmBinEvent(isLmPrimValid, mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFTF,
                                   mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFLF, OFDPA_FALSE,
                                   &(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.tFFLRCnt), &F_valid);

              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.sLMCnt = mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd;
              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.uLMCnt = mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSent -
                                                                              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd;

              if (debugTraceFlags & (CFM_LM))
              {
                memset(bufOutput, 0, DOT1AG_LBM_DATA_LEN);
                sprintf(bufOutput+strlen(bufOutput), "\n\nOndemLM result for mepid %d, pri %d as follows: \n",
                        mepData->dot1agCfmMepIdentifier, priority);
                sprintf(bufOutput+strlen(bufOutput), "      valid,          TF,              LF,               FLR\n");
                sprintf(bufOutput+strlen(bufOutput), "Near End[%d]:  %10d,      %10d,        %10d\n", N_valid,
                        mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNTF,
                        mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNLF,
                        mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.tNFLRCnt);
                sprintf(bufOutput+strlen(bufOutput), "Far End [%d]:  %10d,      %10d,        %10d\n", F_valid,
                        mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFTF,
                        mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFLF,
                        mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.tFFLRCnt);
                sprintf(bufOutput+strlen(bufOutput), "Duration      %10d, Sent %10d,  Recvd %10d\n\n",
////              ct.seconds - mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmStartTime,
                        dpaUpTimeSeconds() - mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmStartTime,
                        mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSent,
                        mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd);
                printf("%s", bufOutput);
              }
            }
            /* Delete the Timer if running */
            if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer != NULL)
            {
              DOT1AG_TRACE(CFM_LM, "remove timer\r\n");
              dot1agTimerDelete(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer);
              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer = NULL;
            }
            mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmEnabled = OFDPA_FALSE;

            if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)
            {
             /* Creates a new entry in the SLM tree. */
              memset(&slmEntry, 0x0, sizeof(slmEntry));

              slmEntry.key.dot1agCfmLmepIdentifier  = mepData->dot1agCfmMepIdentifier;
              slmEntry.key.dot1agCfmRmepIdentifier  = mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmDestMepId;
              slmEntry.key.dot1agCfmTestId          = mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSlmTestId;

              pSlmEntry = dot1agCfmSlmObjGet(slmEntry.key.dot1agCfmLmepIdentifier, slmEntry.key.dot1agCfmRmepIdentifier,
                                             slmEntry.key.dot1agCfmTestId, AVL_EXACT);
              if (NULL != pSlmEntry)
              {
                if (dot1agSlmEntryDelete(pSlmEntry) != OFDPA_E_NONE)
                {
                  return OFDPA_E_FAIL;
                }
              }
              else
              {
                return OFDPA_E_FAIL;
              }
            }
            }
          /*
            memset(mepData->y1731CfmMepOndemlmEntry[priority], 0x0, sizeof(mepData->y1731CfmMepOndemlmEntry[priority]));
          */
          rc = OFDPA_E_NONE;
        }
        break;

      case Y1731_ONDEMLMM_XMIT:
        {
          if (dot1agCfmEventMsg->event == y1731ONDEMLMMLMRRecvd)
          {
            dot1agParseLMR(dot1agCfmEventMsg->dot1agPkt);
            if (mepData->y1731CfmMepOndemlmEntry[priority]->saved)
            {
              prim.nNTF =
                dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCb - mepData->y1731CfmMepOndemlmEntry[priority]->txFCbPrev;
              prim.nNLF =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCb - mepData->y1731CfmMepOndemlmEntry[priority]->txFCbPrev) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCl - mepData->y1731CfmMepOndemlmEntry[priority]->rxFClPrev);
              prim.nFTF =
                dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCf - mepData->y1731CfmMepOndemlmEntry[priority]->txFCfPrev;
              prim.nFLF =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCf - mepData->y1731CfmMepOndemlmEntry[priority]->txFCfPrev) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCf - mepData->y1731CfmMepOndemlmEntry[priority]->rxFcfPrev);
              /*filter*/
              if (((int)prim.nNTF < 0) || ((int)prim.nNLF < 0))
              {
                prim.nNTF = prim.nNLF = 0;
              }
              if (((int)prim.nFTF < 0) || ((int)prim.nFLF < 0))
              {
                prim.nFTF = prim.nFLF = 0;
              }

              mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNTF += prim.nNTF;
              mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNLF += prim.nNLF;
              mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFTF += prim.nFTF;
              mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFLF += prim.nFLF;
            }
            mepData->y1731CfmMepOndemlmEntry[priority]->txFCfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCf;
            mepData->y1731CfmMepOndemlmEntry[priority]->rxFcfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCf;
            mepData->y1731CfmMepOndemlmEntry[priority]->txFCbPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCb;
            mepData->y1731CfmMepOndemlmEntry[priority]->rxFClPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCl;
            mepData->y1731CfmMepOndemlmEntry[priority]->saved = OFDPA_TRUE;

            DOT1AG_TRACE(CFM_LM, "pdu is %d, %d, %d, %d\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.txFCb,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.lmr.rxFCl);
            DOT1AG_TRACE(CFM_LM, "primitive is %d, %d, %d, %d\r\n",
                         mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNTF,
                         mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNLF,
                         mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFTF,
                         mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFLF);

            mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd++;
            if ((0 != mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToSend) &&
                (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd == mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToSend))
            {
              nextState = Y1731_ONDEMLMM_IDLE;
              DOT1AG_TRACE(CFM_LM, "MepId %d, pri %d Event %d CurrState %d, NextState %d: OndemLMM stop because Recvd done\r\n",
                           mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                           mepData->y1731CfmOndemLMMState[priority], nextState);
              UCT = OFDPA_TRUE;
            }
          }
          else if (dot1agCfmEventMsg->event == y1731ONDEMLMMSLRRecvd)
          {
            dot1agParseSLR(dot1agCfmEventMsg->dot1agPkt);

            if (mepData->y1731CfmMepOndemlmEntry[priority]->saved)
            {
              pSlmEntry = dot1agCfmSlmObjGet(dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.srcMepId,
                                             dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rspMepId,
                                             dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.testId, AVL_EXACT);
              if (NULL != pSlmEntry)
              {
                pSlmEntry->data.rxFCl++;
              }
              else
              {
                return OFDPA_E_FAIL;
              }

              dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCl = pSlmEntry->data.rxFCl;

              pSlmEntry->data.nNTF = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCf - pSlmEntry->data.txFCfStart;
              pSlmEntry->data.nFTF = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb - pSlmEntry->data.txFCbStart;
              pSlmEntry->data.nNLF =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb - pSlmEntry->data.txFCbStart) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCl - pSlmEntry->data.rxFClStart);
              pSlmEntry->data.nFLF =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCf - pSlmEntry->data.txFCfStart) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb - pSlmEntry->data.txFCbStart);

              /*filter*/
              if (((int)pSlmEntry->data.nNTF < 0) || ((int)pSlmEntry->data.nNLF < 0))
              {
                pSlmEntry->data.nNLF = 0;
              }

              if (((int)pSlmEntry->data.nFTF < 0) || ((int)pSlmEntry->data.nFLF < 0))
              {
                pSlmEntry->data.nFLF = 0;
              }
              mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNTF = pSlmEntry->data.nNTF;
              mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNLF = pSlmEntry->data.nNLF;
              mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFTF = pSlmEntry->data.nFTF;
              mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFLF = pSlmEntry->data.nFLF;
            }
            mepData->y1731CfmMepOndemlmEntry[priority]->txFCfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCf;
            mepData->y1731CfmMepOndemlmEntry[priority]->rxFcfPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCf;
            mepData->y1731CfmMepOndemlmEntry[priority]->txFCbPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb;
            mepData->y1731CfmMepOndemlmEntry[priority]->rxFClPrev = dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCl;
            mepData->y1731CfmMepOndemlmEntry[priority]->saved = OFDPA_TRUE;

            DOT1AG_TRACE(CFM_LM, "pdu is %d, %d, %d, %d\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCf,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.txFCb,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.slr.rxFCl);
            DOT1AG_TRACE(CFM_LM, "primitive is %d, %d, %d, %d\r\n",
                         mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNTF,
                         mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNLF,
                         mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFTF,
                         mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFLF);

            mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd++;
            if ((0 != mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToSend) &&
                (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd == mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToSend))
            {
              nextState = Y1731_ONDEMLMM_IDLE;
              DOT1AG_TRACE(CFM_LM, "MepId %d, pri %d Event %d CurrState %d, NextState %d: OndemLMM stop because Recvd done\r\n",
                           mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                           mepData->y1731CfmOndemLMMState[priority], nextState);
              UCT = OFDPA_TRUE;
            }
          }
          else /*if (dot1agCfmEventMsg->event == y1731ONDEMLMMIWhileZero)*/
          {
            if ((0 == mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToSend) ||
                (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSent < mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToSend))
            {
              transmitLM = OFDPA_TRUE;
            }

            if (transmitLM)
            {
              if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_LM) /* LMM based LM */
              {
                /*send LMM*/
                rc = xmitOndemLMM(dot1agCfmEventMsg->pMPObj, priority);
              }
              else if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM) /* SLM based LM */
              {
                /*send SLM*/
                rc = xmitOndemSLM(dot1agCfmEventMsg->pMPObj, priority);
              }

              if (rc == OFDPA_E_FAIL)
              {
                return rc;
              }
              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSent++;

              /* Start ONDEMLM Timer. */
              if ((0 != mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToSend) &&
                  (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSent == mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToSend))
              { /*start timeout timer for the final SLR*/
                /*
                   ITU-T G.8013/Y.1731 8.4.1.3 SLR reception
                   At the end of SLM based LM test we need to wait for 5 seconds and process SLR PDUs in that duration.
                */
                controlPeriod = 5000;
                DOT1AG_TRACE(CFM_LM, "final timer\r\n");
              }

              if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer != NULL)
              {
                DOT1AG_TRACE(CFM_LM, "update timer\r\n");
                rc = dot1agTimerUpdate(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer,
                                       mepData->y1731CfmMepOndemlmEntry[priority],
                                       controlPeriod, y1731CfmOndemlmTimerCallback);
                if (rc != OFDPA_E_NONE)
                {
                  break;
                }
              }
              else
              {
                DOT1AG_TRACE(CFM_LM, "add timer\r\n");
                mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer =
                  dot1agTimerNodeAdd(controlPeriod, y1731CfmOndemlmTimerCallback,
                                     mepData->y1731CfmMepOndemlmEntry[priority]);
                if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer == NULL)
                {
                  rc = OFDPA_E_FAIL;
                  break;
                }
              }
            }
            else
            {
              nextState = Y1731_ONDEMLMM_IDLE;
              DOT1AG_TRACE(CFM_LM, "MepId %d, pri %d Event %d CurrState %d, NextState %d: OndemLMM stop because Sent done\r\n",
                           mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                           mepData->y1731CfmOndemLMMState[priority], nextState);
              UCT = OFDPA_TRUE;
            }
          }
        }
        break;
      case Y1731_ONDEMLMM_DEFAULT:
        {
          if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)
          {
            /* Delete the Timer if running */
            if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer != NULL)
            {
              DOT1AG_TRACE(CFM_LM, "remove timer\r\n");
              dot1agTimerDelete(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer);
              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer = NULL;
            }
            mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmEnabled = OFDPA_FALSE;

            if (mepData->y1731CfmMepOndemlmEntry[priority]->saved)
            { /*give report*/
              DOT1AG_TRACE(CFM_LM, "OndemLM result for mepid %d, pri %d as follows: \n\n",
                           mepData->dot1agCfmMepIdentifier, priority);
              y1731PmGetLmBinEvent(isLmPrimValid, mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNTF,
                                   mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNLF, OFDPA_FALSE,
                                   &(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.tNFLRCnt), &N_valid);
              y1731PmGetLmBinEvent(isLmPrimValid, mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFTF,
                                   mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFLF, OFDPA_FALSE,
                                   &(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.tFFLRCnt), &F_valid);

              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.sLMCnt = mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd;
              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.uLMCnt = mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSent -
                                                                              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd;

              if (debugTraceFlags & (CFM_LM))
              {
                memset(bufOutput, 0, DOT1AG_LBM_DATA_LEN);
                sprintf(bufOutput+strlen(bufOutput), "\n\nOndemLM result for mepid %d, pri %d as follows: \n",
                        mepData->dot1agCfmMepIdentifier, priority);
                sprintf(bufOutput+strlen(bufOutput), "      valid,          TF,              LF,               FLR\n");
                sprintf(bufOutput+strlen(bufOutput), "Near End[%d]:  %10d,      %10d,        %10d\n", N_valid,
                        mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNTF,
                        mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nNLF,
                        mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.tNFLRCnt);
                sprintf(bufOutput+strlen(bufOutput), "Far End [%d]:  %10d,      %10d,        %10d\n", F_valid,
                        mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFTF,
                        mepData->y1731CfmMepOndemlmEntry[priority]->lmPrim.nFLF,
                        mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmBin.tFFLRCnt);
                sprintf(bufOutput+strlen(bufOutput), "Duration      %10d, Sent %10d,  Recvd %10d\n\n",
////              ct.seconds - mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmStartTime,
                        dpaUpTimeSeconds() - mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmStartTime,
                        mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSent,
                        mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmRecvd);
                printf("%s", bufOutput);
              }
            }
            /* Delete the Timer if running */
            if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer != NULL)
            {
              DOT1AG_TRACE(CFM_LM, "remove timer\r\n");
              dot1agTimerDelete(mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer);
              mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmTimer = NULL;
            }
            mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmEnabled = OFDPA_FALSE;

            if (mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)
            {
             /* Creates a new entry in the SLM tree. */
              memset(&slmEntry, 0x0, sizeof(slmEntry));

              slmEntry.key.dot1agCfmLmepIdentifier  = mepData->dot1agCfmMepIdentifier;
              slmEntry.key.dot1agCfmRmepIdentifier  = mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmDestMepId;
              slmEntry.key.dot1agCfmTestId          = mepData->y1731CfmMepOndemlmEntry[priority]->ondemlmSlmTestId;

              pSlmEntry = dot1agCfmSlmObjGet(slmEntry.key.dot1agCfmLmepIdentifier, slmEntry.key.dot1agCfmRmepIdentifier,
                                             slmEntry.key.dot1agCfmTestId, AVL_EXACT);
              if (NULL != pSlmEntry)
              {
                if (dot1agSlmEntryDelete(pSlmEntry) != OFDPA_E_NONE)
                {
                  return OFDPA_E_FAIL;
                }
              }
              else
              {
                return OFDPA_E_FAIL;
              }
            }
          }
          nextState = Y1731_ONDEMLMM_IDLE;
          rc = OFDPA_E_NONE;
        }
        break;
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_LM, "MepId %d, pri %d: Event %d CurrState %d nextState %d\r\n",
                 mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                 mepData->y1731CfmOndemLMMState[priority], nextState);
    mepData->y1731CfmOndemLMMState[priority] = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MepId %d, pri %d: Failed at Event %d CurrState %d",
                       mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                       mepData->y1731CfmOndemLMMState[priority]);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the LM RESP state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns OFDPA_E_NONE
* @returns OFDPA_E_FAIL
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmLMRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731LMRStates_t       nextState, dot1agEvent;
  dot1agCfmMepTable_t   *mepData;
  uint8_t                priority;
  OFDPA_BOOL             UCT = OFDPA_FALSE;
  OFDPA_ERROR_t          rc  = OFDPA_E_NONE;

  if (dot1agCfmEventMsg->pMPObj == NULL ||
      dot1agCfmEventMsg->pMPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  priority = dot1agCfmEventMsg->priority;
  mepData      = dot1agCfmEventMsg->pMPObj->mepData;
  dot1agEvent = dot1agCfmEventMsg->event - y1731LMREventsStart - 1;
  DOT1AG_TRACE(CFM_LM, "MepId %d, pri %d: Event %d CurrState %d\r\n",
               mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
               dot1agCfmEventMsg->pMPObj->mepData->y1731CfmLMRState[priority]);

  nextState = y1731SMLMRTable[dot1agEvent][mepData->y1731CfmLMRState[priority]];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case Y1731_LMR_IDLE:
        {
          rc = OFDPA_E_NONE;
        }
        break;

      case Y1731_LMR_RESPOND:
        {
          if (dot1agCfmEventMsg->dot1agPkt->cfmHdr.opcode == DOT1AG_LOSSMEASUREMENT_MESSAGE_OPCODE)
          {
            /*Process LMM*/
            ProcessLMM(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);
          }
          else if (dot1agCfmEventMsg->dot1agPkt->cfmHdr.opcode == DOT1AG_SYNTHETIC_LOSSMEASUREMENT_MESSAGE_OPCODE)
          {
            /*Process SLM*/
            ProcessSLM(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);
          }

          nextState = Y1731_LMR_IDLE;
          UCT = OFDPA_TRUE;
          rc = OFDPA_E_NONE;
          continue;
        }
      case Y1731_LMR_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_LM, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 mepData->y1731CfmLMRState[priority], nextState);
    mepData->y1731CfmLMRState[priority] = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MepId %d, pri %d: Failed at Event %d CurrState %d",
                       mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                       mepData->y1731CfmLMRState[priority]);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the DM INIT state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns OFDPA_E_NONE
* @returns OFDPA_E_FAIL
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmDMMStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731DMMStates_t     nextState, dot1agEvent;
  dot1agCfmMepTable_t *mepData;
//  void *    appTimer;
  uint8_t              priority;
  OFDPA_BOOL           UCT           = OFDPA_FALSE, transmitDM = OFDPA_FALSE;
  OFDPA_ERROR_t        rc            = OFDPA_E_NONE;
  uint32_t             controlPeriod = 0;
  uint32_t             iFD           = 0, rFD = 0;
  static uint32_t      ts[4];

  if (dot1agCfmEventMsg->pMPObj == NULL ||
      dot1agCfmEventMsg->pMPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  priority     = dot1agCfmEventMsg->priority;
  mepData      = dot1agCfmEventMsg->pMPObj->mepData;
  dot1agEvent  = dot1agCfmEventMsg->event - y1731DMMEventsStart - 1;
  if (mepData->y1731CfmMepDmEntry[priority] == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "MepId %d, pri %d: DmEntry is null, Event %d CurrState %d, pri %d",
                       mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                       mepData->y1731CfmDMMState[priority], priority);
    return OFDPA_E_NONE;
  }

//  appTimer = mepData->y1731CfmMepDmEntry[priority]->dmTimer;
  nextState   = y1731SMDMMTable[dot1agEvent][mepData->y1731CfmDMMState[priority]];
  controlPeriod = CCMtime(mepData->y1731CfmMepDmEntry[priority]->dmControlPeriod, OFDPA_FALSE);
  DOT1AG_TRACE(CFM_DM, "MepId %d, pri %d: Event %d CurrState %d, NextState %d\r\n",
               mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
               dot1agCfmEventMsg->pMPObj->mepData->y1731CfmDMMState[priority], nextState);

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case Y1731_DMM_IDLE:
        {
          if (mepData->y1731CfmMepDmEntry[priority]->saved)
          { /*give report*/
            if (mepData->y1731CfmMepDmEntry[priority]->dmType == y1731DmTypeOndemand)
            {
              DOT1AG_TRACE(CFM_DM, "DM result for mepid %d, pri %d, type %d as follows: \nhaha\nhaha\n",
                           mepData->dot1agCfmMepIdentifier, priority, mepData->y1731CfmMepDmEntry[priority]->dmType);
            }
          }
          /* Delete the Timer if running */
          if (mepData->y1731CfmMepDmEntry[priority]->dmTimer != NULL)
          {
            dot1agTimerDelete(mepData->y1731CfmMepDmEntry[priority]->dmTimer);
            mepData->y1731CfmMepDmEntry[priority]->dmTimer = NULL;
          }
          /*
            memset(mepData->y1731CfmMepDmEntry[priority], 0x0, sizeof(mepData->y1731CfmMepDmEntry[priority]));
          */
          mepData->y1731CfmMepDmEntry[priority]->dmEnabled = OFDPA_FALSE;

          rc = OFDPA_E_NONE;
        }
        break;

      case Y1731_DMM_XMIT:
        {
          if (dot1agCfmEventMsg->event == y1731DMMDMRRecvd)
          {
            dot1agParseDMR(dot1agCfmEventMsg->dot1agPkt);
            if ((dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.second == 0 &&
                 dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns == 0) &&
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.second == 0 &&
                 dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns == 0))
            {
              mepData->y1731CfmMepDmEntry[priority]->dmPrim.tBFD.ns =
                dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeb.ns - dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampf.ns;
            }
            else
            {
              mepData->y1731CfmMepDmEntry[priority]->dmPrim.tBFD.ns =
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeb.ns - dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampf.ns) -
                (dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns - dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeStampf.ns);
            }

            if (dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns == 0 &&
                dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeStampf.ns == 0)
            {
              mepData->y1731CfmMepDmEntry[priority]->dmPrim.tFFD.ns = mepData->y1731CfmMepDmEntry[priority]->dmPrim.tNFD.ns = 0;
            }
            else
            {
              mepData->y1731CfmMepDmEntry[priority]->dmPrim.tFFD.ns =
                dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeStampf.ns - dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampf.ns;
              mepData->y1731CfmMepDmEntry[priority]->dmPrim.tNFD.ns =
                dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeb.ns - dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns;
              iFD = (dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeb.ns - dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampf.ns);
              rFD = (dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns - dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeStampf.ns);
            }

            mepData->y1731CfmMepDmEntry[priority]->saved = OFDPA_TRUE;
            /*DM result for proactive or ondemand*/
            DOT1AG_TRACE(CFM_DM, "DMR pdu txTimeStampf is sec %x, nsec %x\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampf.second,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampf.ns);
            DOT1AG_TRACE(CFM_DM, "DMR pdu rxTimeStampf is sec %x, nsec %x\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeStampf.second,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeStampf.ns);
            DOT1AG_TRACE(CFM_DM, "DMR pdu txTimeStampb is sec %x, nsec %x\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.second,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns);

            DOT1AG_TRACE(CFM_DM, "DMR pdu txTimeb is sec %x, nsec %x\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeb.second,
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeb.ns);

            DOT1AG_TRACE(CFM_DM, "primitive is %x, %x, %x\r\n",
                         mepData->y1731CfmMepDmEntry[priority]->dmPrim.tBFD.ns,
                         mepData->y1731CfmMepDmEntry[priority]->dmPrim.tFFD.ns,
                         mepData->y1731CfmMepDmEntry[priority]->dmPrim.tNFD.ns);

            DOT1AG_TRACE(CFM_DM, "FD is %x, %x\r\n", iFD, rFD);

            DOT1AG_TRACE(CFM_DM, "var is %x, %x, %x, %x\r\n",
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampf.ns - ts[0],
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeStampf.ns - ts[1],
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns - ts[2],
                         dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeb.ns- ts[3]);
            ts[0] = dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampf.ns;
            ts[1] = dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeStampf.ns;
            ts[2] = dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.txTimeStampb.ns;
            ts[3] = dot1agCfmEventMsg->dot1agPkt->cfmPdu.dmr.rxTimeb.ns;
            DOT1AG_TRACE(CFM_DM, "now tick %d\r\n", dpaUpTimeMilliSeconds());

            mepData->y1731CfmMepDmEntry[priority]->dmRecvd ++;
            if ((0 != mepData->y1731CfmMepDmEntry[priority]->dmToSend) &&
                (mepData->y1731CfmMepDmEntry[priority]->dmRecvd  == mepData->y1731CfmMepDmEntry[priority]->dmToSend))
            {
              nextState = Y1731_DMM_IDLE;
              DOT1AG_TRACE(CFM_DM, "MepId %d, pri %d Event %d CurrState %d, NextState %d: DMM stop because Recvd done\r\n",
                           mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                           mepData->y1731CfmDMMState[priority], nextState);
              UCT = OFDPA_TRUE;
            }
          }
          else /*if (dot1agCfmEventMsg->event == y1731DMMWhileZero)*/
          {
            if ((0 == mepData->y1731CfmMepDmEntry[priority]->dmToSend) ||
                (mepData->y1731CfmMepDmEntry[priority]->dmSent < mepData->y1731CfmMepDmEntry[priority]->dmToSend))
            {
              transmitDM = OFDPA_TRUE;
            }

            if (transmitDM)
            {
              /*send DMM*/
              rc = xmitDMM(dot1agCfmEventMsg->pMPObj, priority);
              if (rc == OFDPA_E_FAIL)
              {
                return rc;
              }
              mepData->y1731CfmMepDmEntry[priority]->dmSent ++;

              /* Start DM Timer. */
              if (mepData->y1731CfmMepDmEntry[priority]->dmSent == mepData->y1731CfmMepDmEntry[priority]->dmToSend)
              { /*start timeout timer for the final DMR*/
                //controlPeriod = 300;
                controlPeriod = 1000;
              }
              if (mepData->y1731CfmMepDmEntry[priority]->dmTimer != NULL)
              {
                rc = dot1agTimerUpdate(mepData->y1731CfmMepDmEntry[priority]->dmTimer,
                                       mepData->y1731CfmMepDmEntry[priority],
                                       controlPeriod, y1731CfmDmmTimerCallback);
                if (rc != OFDPA_E_NONE)
                {
                  break;
                }
              }
              else
              {
                mepData->y1731CfmMepDmEntry[priority]->dmTimer = dot1agTimerNodeAdd(controlPeriod,
                                                                                    y1731CfmDmmTimerCallback,
                                                                                    mepData->y1731CfmMepDmEntry[priority]);
                if (mepData->y1731CfmMepDmEntry[priority]->dmTimer == NULL)
                {
                  rc = OFDPA_E_FAIL;
                  break;
                }
              }
            }
            else
            {
              nextState = Y1731_DMM_IDLE;
              DOT1AG_TRACE(CFM_DM, "MepId %d, pri %d Event %d CurrState %d, NextState %d: DMM stop because Sent done\r\n",
                           mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                           mepData->y1731CfmDMMState[priority], nextState);
              UCT = OFDPA_TRUE;
            }

          }
        }
        break;
      case Y1731_DMM_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DM, "MepId %d, pri %d: Event %d CurrState %d nextState %d\r\n",
                 mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                 mepData->y1731CfmDMMState[priority], nextState);
    mepData->y1731CfmDMMState[priority] = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MepId %d, pri %d: Failed at Event %d CurrState %d",
                       mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                       mepData->y1731CfmDMMState[priority]);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the DM RESP state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns OFDPA_E_NONE
* @returns OFDPA_E_FAIL
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmDMRStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731DMRStates_t     nextState, dot1agEvent;
  dot1agCfmMepTable_t *mepData;
  uint8_t              priority;
  OFDPA_BOOL           UCT = OFDPA_FALSE;
  OFDPA_ERROR_t        rc  = OFDPA_E_NONE;

  if (dot1agCfmEventMsg->pMPObj == NULL ||
      dot1agCfmEventMsg->pMPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  priority     = dot1agCfmEventMsg->priority;
  mepData      = dot1agCfmEventMsg->pMPObj->mepData;
  dot1agEvent  = dot1agCfmEventMsg->event - y1731DMREventsStart - 1;
  DOT1AG_TRACE(CFM_DM, "MepId %d, pri %d: Event %d CurrState %d\r\n",
               mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
               dot1agCfmEventMsg->pMPObj->mepData->y1731CfmDMMState[priority]);

  nextState = y1731SMDMRTable[dot1agEvent][mepData->y1731CfmDMRState[priority]];

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case Y1731_DMR_IDLE:
        {
          rc = OFDPA_E_NONE;
        }
        break;

      case Y1731_DMR_RESPOND:
        {
          ProcessDMM(dot1agCfmEventMsg->dot1agPkt, dot1agCfmEventMsg->pMPObj);
          nextState = Y1731_DMR_IDLE;
          UCT = OFDPA_TRUE;
          rc = OFDPA_E_NONE;
          continue;
        }
      case Y1731_DMR_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DM, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 mepData->y1731CfmDMRState[priority], nextState);
    mepData->y1731CfmDMRState[priority] = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MepId %d, pri %d: Failed at Event %d CurrState %d",
                       mepData->dot1agCfmMepIdentifier, priority, dot1agEvent,
                       mepData->y1731CfmDMRState[priority]);
  }
  return rc;
}

/***************************************************************************
* @purpose  This routine calculates the next state for the AIS Send state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Client signal fail (AIS)
*           Y.1731 9.7
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmMepAISSendStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731AISSendStates_t  nextState, dot1agEvent;
  dot1agCfmMepTable_t  *pMEPData;
  OFDPA_ERROR_t         rc = OFDPA_E_NONE;

  /* Get the MEP Object.*/
  pMEPData = dot1agCfmEventMsg->pMepData;
  if (pMEPData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - y1731AisSendEventsStart - 1;

  nextState = y1731SMAisSendTable[dot1agEvent][pMEPData->y1731CfmMepAisSendState];

  DOT1AG_TRACE(CFM_DEFECT, "At Event %d currState %d nextState %d\r\n",
               dot1agEvent, pMEPData->y1731CfmMepAisSendState, nextState);

  switch (nextState)
  {
    case Y1731_AIS_SEND_STOP:
      {
        /* Delete Timer if running */
        if (pMEPData->y1731CfmMepAisSendVar.aisSendTimer != NULL)
        {
          dot1agTimerDelete(pMEPData->y1731CfmMepAisSendVar.aisSendTimer);
          pMEPData->y1731CfmMepAisSendVar.aisSendTimer = NULL;
        }
      }
      break;

    case Y1731_AIS_SEND_START:
      {
        if (OFDPA_TRUE != IsAISTriggered(pMEPData))
        {
          /* Delete Timer if running */
          if (pMEPData->y1731CfmMepAisSendVar.aisSendTimer != NULL)
          {
            dot1agTimerDelete(pMEPData->y1731CfmMepAisSendVar.aisSendTimer);
            pMEPData->y1731CfmMepAisSendVar.aisSendTimer = NULL;
          }
          nextState = Y1731_AIS_SEND_STOP;
          break;
        }

        AISTriggerHandler(pMEPData);

        /* Start Timer */
        if (pMEPData->y1731CfmMepAisSendVar.aisSendTimer != NULL)
        {
          /* Update the timer if already running */
          dot1agTimerUpdate(pMEPData->y1731CfmMepAisSendVar.aisSendTimer,
                            pMEPData,
                            pMEPData->y1731CfmMepAisSendVar.aisInterval,
                            y1731CfmAisSendTimerCallback);
        }
        else
        {
          pMEPData->y1731CfmMepAisSendVar.aisSendTimer = dot1agTimerNodeAdd(pMEPData->y1731CfmMepAisSendVar.aisInterval,
                                                                            y1731CfmAisSendTimerCallback,
                                                                            pMEPData);
        }

        if (pMEPData->y1731CfmMepAisSendVar.aisSendTimer == NULL)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                             "Failed xmitAIS at Event %d currState %d nextState %d",
                             dot1agEvent, pMEPData->y1731CfmMepAisSendState, nextState);
          rc = OFDPA_E_FAIL;
          break;
        }
      }
      break;

    case Y1731_AIS_SEND_DEFAULT:
    default:
      return OFDPA_E_FAIL;
      break;
  }

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, pMEPData->y1731CfmMepAisSendState, nextState);
    pMEPData->y1731CfmMepAisSendState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d currState %d",
                       dot1agEvent, pMEPData->y1731CfmMepAisSendState);
  }
  return rc;
}

/***************************************************************************
* @purpose  This routine calculates the next state for the AIS Receive state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Client signal fail (AIS)
*           Y.1731 9.7
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmMepAISRcvStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731AISRcvStates_t   nextState, dot1agEvent;
  dot1agCfmStackObj_t  *pMEPObj;
  OFDPA_ERROR_t         rc = OFDPA_E_NONE;
  uint32_t              interval;
  dot1agCfmSMEventMsg_t eventMsg;

  /* Get the MEP Object.*/
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - y1731AisRcvEventsStart - 1;

  nextState = y1731SMAisRcvTable[dot1agEvent][pMEPObj->mepData->y1731CfmMepAisRcvState];

  DOT1AG_TRACE(CFM_DEFECT, "At Event %d currState %d nextState %d\r\n",
               dot1agEvent, pMEPObj->mepData->y1731CfmMepAisRcvState, nextState);

  switch (nextState)
  {
    case Y1731_AIS_RCV_IDLE:
      {
        /* Delete Timer if running */
        if (pMEPObj->mepData->y1731CfmMepAisRcvVar.aisRcvTimer != NULL)
        {
          dot1agTimerDelete(pMEPObj->mepData->y1731CfmMepAisRcvVar.aisRcvTimer);
          pMEPObj->mepData->y1731CfmMepAisRcvVar.aisRcvTimer = NULL;
        }
      }
      break;

    case Y1731_AIS_RCV_WAITING:
      {
        if (OFDPA_E_NONE != dot1agCheckAIS(dot1agCfmEventMsg, &interval))
        {
          rc = OFDPA_E_FAIL;
          break;
        }

        /* AIS RCV expiry timer period is 3.5 times of value mentioned in last 3 bits of Flags */
        interval = (interval * 7)/2;

        /* Start Timer */
        if (pMEPObj->mepData->y1731CfmMepAisRcvVar.aisRcvTimer != NULL)
        {
          /* Update the timer if already running */
          dot1agTimerUpdate(pMEPObj->mepData->y1731CfmMepAisRcvVar.aisRcvTimer,
                            pMEPObj,
                            interval,
                            y1731CfmAisRcvTimerCallback);
        }
        else
        {
          pMEPObj->mepData->y1731CfmMepAisRcvVar.aisRcvTimer = dot1agTimerNodeAdd(
            interval,
            y1731CfmAisRcvTimerCallback,
            pMEPObj);
        }

        if (pMEPObj->mepData->y1731CfmMepAisRcvVar.aisRcvTimer == NULL)
        {
          rc = OFDPA_E_FAIL;
        }

        if (OFDPA_TRUE == pMEPObj->mepData->y1731CfmMepAisSendVar.aisEnable)
        {
          memset(&eventMsg, 0, sizeof(eventMsg));
          eventMsg.pMepData = pMEPObj->mepData;
          eventMsg.event  = y1731AisSendStart;
      
          if (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMepData->dot1agCfmMepIdentifier))
          {
            dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
          }
          else
          {
            dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
          }
        }
      }
      break;

    case Y1731_AIS_RCV_DEFAULT:
    default:
      return OFDPA_E_FAIL;
      break;
  }

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, pMEPObj->mepData->y1731CfmMepAisRcvState, nextState);
    pMEPObj->mepData->y1731CfmMepAisRcvState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d currState %d",
                       dot1agEvent, pMEPObj->mepData->y1731CfmMepAisRcvState);
  }
  return rc;
}

/***************************************************************************
* @purpose  This routine calculates the next state for the AIS Send state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Client signal fail (LCK)
*           Y.1731 9.7
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmMepLCKSendStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731LCKSendStates_t  nextState, dot1agEvent;
  dot1agCfmStackObj_t  *pMEPObj;
  OFDPA_ERROR_t         rc = OFDPA_E_NONE;

  /* Get the MEP Object.*/
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - y1731LckSendEventsStart - 1;

  nextState = y1731SMLckSendTable[dot1agEvent][pMEPObj->mepData->y1731CfmMepLckSendState];

  DOT1AG_TRACE(CFM_DEFECT, "At Event %d currState %d nextState %d\r\n",
               dot1agEvent, pMEPObj->mepData->y1731CfmMepLckSendState, nextState);

  switch (nextState)
  {
    case Y1731_LCK_SEND_STOP:
      {
        /* Delete Timer if running */
        if (pMEPObj->mepData->y1731CfmMepLckSendVar.lckSendTimer != NULL)
        {
          dot1agTimerDelete(pMEPObj->mepData->y1731CfmMepLckSendVar.lckSendTimer);
          pMEPObj->mepData->y1731CfmMepLckSendVar.lckSendTimer = NULL;
        }
      }
      break;

    case Y1731_LCK_SEND_START:
      {
        rc = xmitClientLCK(pMEPObj);
        if (rc != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                             "Failed xmitAIS at Event %d currState %d nextState %d",
                             dot1agEvent, pMEPObj->mepData->y1731CfmMepLckSendState, nextState);
          rc = OFDPA_E_FAIL;
          break;
        }

        /* Start Timer */
        if (pMEPObj->mepData->y1731CfmMepLckSendVar.lckSendTimer != NULL)
        {
          /* Update the timer if already running */
          dot1agTimerUpdate(pMEPObj->mepData->y1731CfmMepLckSendVar.lckSendTimer,
                            pMEPObj,
                            pMEPObj->mepData->y1731CfmMepLckSendVar.lckInterval,
                            y1731CfmLckSendTimerCallback);
        }
        else
        {
          pMEPObj->mepData->y1731CfmMepLckSendVar.lckSendTimer = dot1agTimerNodeAdd(pMEPObj->mepData->y1731CfmMepLckSendVar.lckInterval,
                                                                                    y1731CfmLckSendTimerCallback,
                                                                                    pMEPObj);
        }

        if (pMEPObj->mepData->y1731CfmMepLckSendVar.lckSendTimer == NULL)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                             "Failed xmitAIS at Event %d currState %d nextState %d",
                             dot1agEvent, pMEPObj->mepData->y1731CfmMepLckSendState, nextState);
          rc = OFDPA_E_FAIL;
          break;
        }
      }
      break;

    case Y1731_LCK_SEND_DEFAULT:
    default:
      return OFDPA_E_FAIL;
      break;
  }

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, pMEPObj->mepData->y1731CfmMepLckSendState, nextState);
    pMEPObj->mepData->y1731CfmMepLckSendState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d currState %d",
                       dot1agEvent, pMEPObj->mepData->y1731CfmMepLckSendState);
  }
  return rc;
}

/***************************************************************************
* @purpose  This routine calculates the next state for the AIS Receive state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Client signal fail (LCK)
*           Y.1731 9.7
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmMepLCKRcvStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731LCKRcvStates_t    nextState, dot1agEvent;
  dot1agCfmStackObj_t   *pMEPObj;
  uint32_t               interval;
  OFDPA_ERROR_t          rc = OFDPA_E_NONE;
  dot1agCfmSMEventMsg_t  eventMsg;

  /* Get the MEP Object.*/
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - y1731LckRcvEventsStart - 1;

  nextState = y1731SMLckRcvTable[dot1agEvent][pMEPObj->mepData->y1731CfmMepLckRcvState];

  DOT1AG_TRACE(CFM_DEFECT, "At Event %d currState %d nextState %d\r\n",
               dot1agEvent, pMEPObj->mepData->y1731CfmMepLckRcvState, nextState);

  switch (nextState)
  {
    case Y1731_LCK_RCV_IDLE:
      {
        /* Delete Timer if running */
        if (pMEPObj->mepData->y1731CfmMepLckRcvVar.lckRcvTimer != NULL)
        {
          dot1agTimerDelete(pMEPObj->mepData->y1731CfmMepLckRcvVar.lckRcvTimer);
          pMEPObj->mepData->y1731CfmMepLckRcvVar.lckRcvTimer = NULL;
        }
      }
      break;

    case Y1731_LCK_RCV_WAITING:
      {
        if (OFDPA_E_NONE != dot1agCheckLCK(dot1agCfmEventMsg, &interval))
        {
          rc = OFDPA_E_FAIL;
          break;
        }

        /* LCK RCV expiry timer period is 3.5 times of value mentioned in last 3 bits of Flags */
        interval = (interval * 7)/2;

        /* Start Timer */
        if (pMEPObj->mepData->y1731CfmMepLckRcvVar.lckRcvTimer != NULL)
        {
          /* Update the timer if already running */
          dot1agTimerUpdate(pMEPObj->mepData->y1731CfmMepLckRcvVar.lckRcvTimer,
                            pMEPObj,
                            interval,
                            y1731CfmLckRcvTimerCallback);
        }
        else
        {
          pMEPObj->mepData->y1731CfmMepLckRcvVar.lckRcvTimer = dot1agTimerNodeAdd(interval,
                                                                                  y1731CfmLckRcvTimerCallback,
                                                                                  pMEPObj);
        }

        if (pMEPObj->mepData->y1731CfmMepLckRcvVar.lckRcvTimer == NULL)
        {
          rc = OFDPA_E_FAIL;
          break;
        }

        if (OFDPA_TRUE == pMEPObj->mepData->y1731CfmMepAisSendVar.aisEnable)
        {
          memset(&eventMsg, 0, sizeof(eventMsg));
          eventMsg.pMepData = pMEPObj->mepData;
          eventMsg.event  = y1731AisSendStart;
      
          if (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMepData->dot1agCfmMepIdentifier))
          {
            dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
          }
          else
          {
            dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
          }
        }
      }
      break;

    case Y1731_LCK_RCV_DEFAULT:
    default:
      return OFDPA_E_FAIL;
      break;
  }

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, pMEPObj->mepData->y1731CfmMepLckRcvState, nextState);
    pMEPObj->mepData->y1731CfmMepLckRcvState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d currState %d",
                       dot1agEvent, pMEPObj->mepData->y1731CfmMepLckRcvState);
  }
  return rc;
}

/***************************************************************************
* @purpose  This routine calculates the next state for the AIS Send state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Client signal fail (CSF)
*           Y.1731 9.7
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmMepCSFSendStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731CSFSendStates_t  nextState, dot1agEvent;
  dot1agCfmMepTable_t  *pMEPData;
  OFDPA_ERROR_t         rc = OFDPA_E_NONE;
  dot1agCfmStackObj_t  *pMPObj = NULL;

  if (dot1agCfmEventMsg == NULL ||
      dot1agCfmEventMsg->pMepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* Get the MEP Object.*/
  pMEPData = dot1agCfmEventMsg->pMepData;
  dot1agEvent = dot1agCfmEventMsg->event - y1731CsfSendEventsStart - 1;

  nextState = y1731SMCsfSendTable[dot1agEvent][pMEPData->y1731CfmMepCsfSendState];

  DOT1AG_TRACE(CFM_DEFECT, "At Event %d currState %d nextState %d\r\n",
               dot1agEvent, pMEPData->y1731CfmMepCsfSendState, nextState);

  switch (nextState)
  {
    case Y1731_CSF_SEND_STOP:
      {
        /* Delete Timer if running */
        if (pMEPData->y1731CfmMepCsfSendVar.csfSendTimer != NULL)
        {
          dot1agTimerDelete(pMEPData->y1731CfmMepCsfSendVar.csfSendTimer);
          pMEPData->y1731CfmMepCsfSendVar.csfSendTimer = NULL;
        }
      }
      break;

    case Y1731_CSF_SEND_START:
      {

        pMPObj = dot1agCfmStackObjGet(pMEPData->dot1agCfmMepIfIndex, pMEPData->dot1agCfmMepPrimaryVid,
                                      pMEPData->maData->mdData->dot1agCfmMdMdLevel,
                                      pMEPData->dot1agCfmMepDirection, AVL_EXACT);
        if (pMPObj == NULL)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                             "Failed to CFM stack object for lmep = %d",
                             pMEPData->dot1agCfmMepIdentifier);
          rc = OFDPA_E_FAIL;
          break;
        }

        if (pMEPData->y1731CfmMepAisRcvState == Y1731_AIS_RCV_WAITING)
          rc = xmitClientCSF(dot1agCfmEventMsg->pMPObj, DOT1AF_CSF_TYPE_AIS);
        else if (pMEPData->y1731DefectState[Y1731_DEFECT_TYPE_RDI] == Y1731_MEP_DEFECT_DEFECT)
          rc = xmitClientCSF(dot1agCfmEventMsg->pMPObj, DOT1AF_CSF_TYPE_RDI);
        else
          rc = xmitClientCSF(dot1agCfmEventMsg->pMPObj, DOT1AF_CSF_TYPE_LOS);

        if (rc != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                             "Failed xmitAIS at Event %d currState %d nextState %d",
                             dot1agEvent, pMEPData->y1731CfmMepCsfSendState, nextState);
          rc = OFDPA_E_FAIL;
          break;
        }

        /* Start Timer */
        if (pMEPData->y1731CfmMepCsfSendVar.csfSendTimer != NULL)
        {
          /* Update the timer if already running */
          dot1agTimerUpdate(pMEPData->y1731CfmMepCsfSendVar.csfSendTimer,
                            pMEPData,
                            pMEPData->y1731CfmMepCsfSendVar.csfInterval,
                            y1731CfmCsfSendTimerCallback);
        }
        else
        {
          pMEPData->y1731CfmMepCsfSendVar.csfSendTimer = dot1agTimerNodeAdd(pMEPData->y1731CfmMepCsfSendVar.csfInterval,
                                                                            y1731CfmCsfSendTimerCallback,
                                                                            pMEPData);
        }

        if (pMEPData->y1731CfmMepCsfSendVar.csfSendTimer == NULL)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                             "Failed xmitAIS at Event %d currState %d nextState %d",
                             dot1agEvent, pMEPData->y1731CfmMepCsfSendState, nextState);
          rc = OFDPA_E_FAIL;
          break;
        }
      }
      break;

    case Y1731_CSF_SEND_DEFAULT:
    default:
      return OFDPA_E_FAIL;
      break;
  }

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, pMEPData->y1731CfmMepCsfSendState, nextState);
    pMEPData->y1731CfmMepCsfSendState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d currState %d",
                       dot1agEvent, pMEPData->y1731CfmMepCsfSendState);
  }
  return rc;
}

/***************************************************************************
* @purpose  This routine calculates the next state for the AIS Receive state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Client signal fail (CSF)
*           Y.1731 9.7
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmMepCSFRcvStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg)
{
  y1731CSFRcvStates_t    nextState, dot1agEvent;
  dot1agCfmStackObj_t   *pMEPObj;
  OFDPA_ERROR_t          rc = OFDPA_E_NONE;
  uint32_t               interval;
  Dot1agCfmCsfTypeVal_t  type;

  /* Get the MEP Object.*/
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  dot1agEvent = dot1agCfmEventMsg->event - y1731CsfRcvEventsStart - 1;

  nextState = y1731SMCsfRcvTable[dot1agEvent][pMEPObj->mepData->y1731CfmMepCsfRcvState];

  DOT1AG_TRACE(CFM_DEFECT, "At Event %d currState %d nextState %d\r\n",
               dot1agEvent, pMEPObj->mepData->y1731CfmMepCsfRcvState, nextState);

  switch (nextState)
  {
    case Y1731_CSF_RCV_IDLE:
      {
        /* Delete Timer if running */
        if (pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfRcvTimer != NULL)
        {
          dot1agTimerDelete(pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfRcvTimer);
          pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfRcvTimer = NULL;
        }
      }
      break;

    case Y1731_CSF_RCV_WAITING:
      {
        if (OFDPA_E_NONE != dot1agCheckCSF(dot1agCfmEventMsg, &interval, &type))
        {
          rc = OFDPA_E_FAIL;
          break;
        }

        /* CSF RCV expiry timer period is 3.5 times of value mentioned in last 3 bits of Flags */
        interval = (interval * 7)/2;

        pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfInterval = interval;
        pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfType = type;
        /* Start Timer */
        if (pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfRcvTimer != NULL)
        {
          /* Update the timer if already running */
          dot1agTimerUpdate(pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfRcvTimer,
                            pMEPObj,
                            interval,
                            y1731CfmCsfRcvTimerCallback);
        }
        else
        {
          pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfRcvTimer = dot1agTimerNodeAdd(interval,
                                                                                  y1731CfmCsfRcvTimerCallback,
                                                                                  pMEPObj);
        }

        if (pMEPObj->mepData->y1731CfmMepCsfRcvVar.csfRcvTimer == NULL)
        {
          rc = OFDPA_E_FAIL;
          break;
        }
      }
      break;

    case Y1731_CSF_RCV_DEFAULT:
    default:
      return OFDPA_E_FAIL;
      break;
  }

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Event %d CurrState %d nextState %d\r\n",
                 dot1agEvent, pMEPObj->mepData->y1731CfmMepCsfRcvState, nextState);
    pMEPObj->mepData->y1731CfmMepCsfRcvState = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed at Event %d currState %d",
                       dot1agEvent, pMEPObj->mepData->y1731CfmMepCsfRcvState);
  }
  return rc;
}

/******************************************************************************
* @purpose  This routine calculates the next state for the Remote MEP state machine
*           and executes the action for that next state
*
* @param    dot1agCfmEventMsg  @b{(input)) event message having mepIndex, maindex,
*                                          mdlevel, internal interface number and
*                                          the required params for FSM.
* @param    defType            @b{(input)) defType.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This is run per MEP defect Y.1731.
*
* @end
*******************************************************************************/
OFDPA_ERROR_t y1731CfmMepDefectStateMachine(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg, y1731_defect_type_t defType)
{
  dot1agXConMepStates_t   nextState, dot1agEvent;
  uint32_t                timeLeft       = 0, xconCCMInterval = 0;
  dot1agCfmStackObj_t    *pMEPObj;
  OFDPA_BOOL              UCT            = OFDPA_FALSE;
  OFDPA_ERROR_t           rc             = OFDPA_E_NONE;
  uint32_t                rcvdInterval   = 0;
  y1731MepDefectStates_t  y1731DefectState;
  y1731DefectMepVars_t   *y1731DefectVar = NULL;
  dot1ag_tmr_fn           timerCallback;

  /* Get MEP Object */
  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    /* Invalid input parameters.*/
    return OFDPA_E_FAIL;
  }

  switch(defType)
  {
    case Y1731_DEFECT_TYPE_UNM:
      dot1agEvent   = dot1agCfmEventMsg->event - y1731dUNMEventsStart - 1;
      timerCallback = y1731DefectUNMTimerCallback;
      break;
    case Y1731_DEFECT_TYPE_UNP:
      dot1agEvent = dot1agCfmEventMsg->event - y1731dUNPEventsStart - 1;
      timerCallback = y1731DefectUNPTimerCallback;
      break;
    case Y1731_DEFECT_TYPE_MMG:
      dot1agEvent = dot1agCfmEventMsg->event - y1731dMMGEventsStart - 1;
      timerCallback = y1731DefectMMGTimerCallback;
      break;
    case Y1731_DEFECT_TYPE_UNL:
      dot1agEvent = dot1agCfmEventMsg->event - y1731dUNLEventsStart - 1;
      timerCallback = y1731DefectUNLTimerCallback;
      break;
    case Y1731_DEFECT_TYPE_UNPTR:
      dot1agEvent = dot1agCfmEventMsg->event - y1731dUNPtrEventsStart - 1;
      timerCallback = y1731DefectUNPTRTimerCallback;
      break;
    default:
      return OFDPA_E_FAIL;
  }

  y1731DefectState = pMEPObj->mepData->y1731DefectState[defType];
  y1731DefectVar   = &pMEPObj->mepData->y1731DefectVar[defType];
  nextState        = y1731defectTable[dot1agEvent][y1731DefectState];

  DOT1AG_TRACE(CFM_DEFECT, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
               y1731DefectState, nextState);

  do
  {
    UCT = OFDPA_FALSE;
    switch (nextState)
    {
      case Y1731_MEP_DEFECT_IDLE:
        {
          y1731DefectVar->y1731DefCcmDefect = OFDPA_FALSE;
          //MAdefectIndicationNotify(pMEPObj->mepData);
          UCT = OFDPA_TRUE;
          nextState = Y1731_MEP_DEFECT_NO_DEFECT;
          break;
        }

      case Y1731_MEP_DEFECT_NO_DEFECT:
        {
          if (y1731DefectVar->y1731DefCCMTimer != NULL)
          {
            dot1agTimerDelete(y1731DefectVar->y1731DefCCMTimer);
            y1731DefectVar->y1731DefCCMTimer = NULL;
          }
          y1731DefectVar->y1731DefCcmDefect = OFDPA_FALSE;
          //MAdefectIndicationNotify(pMEPObj->mepData);
          memset(y1731DefectVar->y1731DefCcmLastFailure,
                 0x0, OFDPA_DOT1AG_CCM_FRAME_LEN);
          break;
        }

      case Y1731_MEP_DEFECT_DEFECT:
        {
          if ((pMEPObj->mepData != NULL) &&
              (OFDPA_TRUE == dot1agIsMepHwEnabled
               (pMEPObj->mepData->dot1agCfmMepIdentifier)))
          {
            rcvdInterval =
              CCMtime(dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.ccmInterval,
                      OFDPA_TRUE);
          }
          else
          {
            rcvdInterval =
              CCMtime(dot1agCfmEventMsg->dot1agPkt->cfmPdu.ccm.ccmInterval,
                      OFDPA_FALSE);
          }

          if (y1731DefectVar->y1731DefCCMTimer != NULL)
          {
            xconCCMInterval = DOT1AG_RMEP_INTERVAL_GET(rcvdInterval);
            timeLeft = 0;
//            xconCCMInterval = max(timeLeft, xconCCMInterval);
            xconCCMInterval = (timeLeft > xconCCMInterval ? timeLeft : xconCCMInterval);
            rc = dot1agTimerUpdate(y1731DefectVar->y1731DefCCMTimer,
                                   pMEPObj, xconCCMInterval, timerCallback);
            if (rc != OFDPA_E_NONE)
            {
              rc = OFDPA_E_FAIL;
              break;
            }
          }
          else
          {
            xconCCMInterval = DOT1AG_RMEP_INTERVAL_GET(rcvdInterval);
            y1731DefectVar->y1731DefCCMTimer = dot1agTimerNodeAdd(xconCCMInterval,
                                                                  timerCallback, pMEPObj);
            if (y1731DefectVar->y1731DefCCMTimer == NULL)
            {
              rc = OFDPA_E_FAIL;
              break;
            }
          }
          y1731DefectVar->y1731DefCcmDefect = OFDPA_TRUE;
          //MAdefectIndicationNotify(pMEPObj->mepData);
          memcpy(y1731DefectVar->y1731DefCcmLastFailure,
                 dot1agCfmEventMsg->dot1agPkt->payLoad, OFDPA_DOT1AG_CCM_FRAME_LEN);
          break;
        }
      case Y1731_MEP_DEFECT_DEFAULT:
      default:
        return OFDPA_E_FAIL;
    }
  } while (UCT == OFDPA_TRUE);

  /* Update the current state.*/
  if (rc == OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Event %d CurrState %d nextState %d\r\n", dot1agEvent,
                 y1731DefectState, nextState);
    pMEPObj->mepData->y1731DefectState[defType] = nextState;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Failed at Event %d CurrState %d", dot1agEvent, y1731DefectState);
  }
  return rc;
}

/***************************************************************
******************  CFM TIMER CALLBACKS   **********************
***************************************************************/

/******************************************************************************
* @purpose  This timer callback sees that the CCMs are sent at configured CCM
*           interval.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.2
*
* @end
*******************************************************************************/
void dot1agCfmCciTimerCallback(void *handle)
{
  dot1agCfmStackObj_t   *pMEPObj = handle;
  dot1agCfmSMEventMsg_t  smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }
  DOT1AG_TRACE(CFM_TIMERS, "CCI Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

#if OFDPA_NOT_SUPPORTED
  if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    pMEPObj->mepData->dot1agCfmMepCCIVar.cciTimer = NULL;
#endif

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = dot1agCCIWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback is used to timeout a remote MEP.
*
* @param    handle  @b{(input)) pointer to Remote MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.7
*
* @end
*******************************************************************************/
void dot1agCfmRmepTimerCallback(void *handle)
{
  dot1agCfmStackObj_t   *pMPObj;
  dot1agCfmRMepObj_t    *rmepObj = handle;
  dot1agCfmSMEventMsg_t  smEventMsg;
  dot1agCfmMepObj_t     *pMEPObj = NULL;
  uint32_t               mdIndex, maIndex, mepId;

  if (rmepObj == NULL)
  {
    return;
  }

  memset(&smEventMsg, 0, sizeof(smEventMsg));

  /* Find the MEP object */
  mdIndex = rmepObj->key.dot1agCfmMdIndex;
  maIndex = rmepObj->key.dot1agCfmMaIndex;
  mepId   = rmepObj->key.dot1agCfmMepIdentifier;

  pMEPObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, 0, AVL_NEXT);
  if (pMEPObj != NULL &&
      pMEPObj->mepData != NULL &&
      mdIndex == pMEPObj->mepData->maData->mdData->dot1agCfmMdIndex &&
      maIndex == pMEPObj->mepData->maData->dot1agCfmMaIndex &&
      mepId   == pMEPObj->mepData->dot1agCfmMepIdentifier)
  {
    smEventMsg.pMepData = pMEPObj->mepData;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Remote MEP timer callback failed because MEP not found on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId,
                       (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIfIndex) : 0);
    return;
  }

  DOT1AG_TRACE(CFM_TIMERS, "Rmep Timer expired RmepEntry mdIndex %d, maIndex %d, mepId %d, rmepId %d\r\n",
               mdIndex, maIndex, mepId, rmepObj->key.dot1agCfmMepDbRMepIdentifier);

  pMPObj = dot1agCfmStackObjGet(pMEPObj->mepData->dot1agCfmMepIfIndex,
                                pMEPObj->mepData->dot1agCfmMepPrimaryVid,
                                pMEPObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                pMEPObj->mepData->dot1agCfmMepDirection, AVL_EXACT);
  if (NULL == pMPObj)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Remote MEP timer callback failed because of invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, pMEPObj->mepData->dot1agCfmMepIfIndex);
    return;
  }

  /* Check if it is a stale entry. If so destroy the RMEP entry */
  if (rmepObj->dot1agCfmRmepVar.rMepCcmDefect == OFDPA_TRUE)
  {
#if OFDPA_NOT_SUPPORTED
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    {
      if (dot1agDBTreeEntryDelete(DOT1AG_DB_ID_RMEP_TREE, &dot1agCFM.dot1agRMepTree,
                                  rmepObj) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to remove stale entry mdIndex %d maIndex %d mepId %d rmepId %d",
                           rmepObj->key.dot1agCfmMdIndex, rmepObj->key.dot1agCfmMaIndex,
                           rmepObj->key.dot1agCfmMepIdentifier, rmepObj->key.dot1agCfmMepDbRMepIdentifier);
      }
    }
#endif

    /* After the archive hold time, the errors are getting cleared, though the
     * MEP is still inactive.Commenting out the following piece of code will
     * maintain errors till the MEP becomes active. The reason for not removing the
     * below code, to see if any issues turn up due to this */
    /*if (pMEPObj != NULL && pMEPObj->mepData != NULL)
      {
      MAdefectIndicationNotify(pMEPObj->mepData);
      }*/
    return;
  }

  smEventMsg.pRMEPObj = rmepObj;
  smEventMsg.pMPObj   = pMPObj;
  smEventMsg.event    = dot1agRmepWhileZero;

  /* Revisit */
  dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
}

/******************************************************************************
* @purpose  This timer callback is used to timeout invalid CCMs.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.3
*
* @end
*******************************************************************************/
void dot1agCfmRmepErrTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }

#if OFDPA_NOT_SUPPORTED
  if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    pMEPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer = NULL;
#endif

  DOT1AG_TRACE(CFM_TIMERS, "Rmep Err Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = dot1agRmepErrCCMWhileZero;

  /* Revisit */
  dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
}

/******************************************************************************
* @purpose  This timer callback is used to timeout cross connect CCMs
*           (based on MD levels).
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.4
*
* @end
*******************************************************************************/
void dot1agCfmXConCCMTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }

#if OFDPA_NOT_SUPPORTED
  if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    pMEPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer = NULL;
#endif

  DOT1AG_TRACE(CFM_TIMERS, "XconCCM Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);
  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = dot1agXConCCMWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback is used to timeout expected reception of LBRs.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.5
*
* @end
*******************************************************************************/
void dot1agCfmLbiTxTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }

  DOT1AG_TRACE(CFM_TIMERS, "Lbi TxTimer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = dot1agLBILBMsToSend;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)) &&
      (DOT1AG_MP_DIRECTION_DOWN == smEventMsg.pMPObj->key.dot1agCfmStackDirection))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback is used xmit the next LBM
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.5
*
* @end
*******************************************************************************/
void dot1agCfmLbiTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return;
  }

  dot1agTimerDelete(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer);
  pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer = NULL;

  DOT1AG_TRACE(CFM_TIMERS, "Lbi Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = dot1agLBITestStop;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback is used to timeout expected transmission of LTRs
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.1
*
* @end
*******************************************************************************/
void dot1agCfmLtfTimerCallback(void  *handle)
{
  dot1agCFM_t           *dot1agCFM = handle;
  dot1agCfmSMEventMsg_t  smEventMsg;

  if (dot1agCFM == NULL)
  {
    return;
  }
  memset(&smEventMsg, 0, sizeof(smEventMsg));

  smEventMsg.event = dot1agLTRTLtfWhileZero;
  /* Revisit*/
  dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
}

/******************************************************************************
* @purpose  This timer callback is used to wait for defects to stabilize & disappear.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.6
*
* @end
*******************************************************************************/
void dot1agCfmFngTimerCallback(void *handle)
{
  dot1agCfmMepTable_t  *mepData = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (mepData == NULL)
  {
    return;
  }

#if OFDPA_NOT_SUPPORTED
  if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    mepData->dot1agCfmMepFngVar.fngTimer = NULL;
#endif

  DOT1AG_TRACE(CFM_TIMERS, "FNG Timer expired mepId %d\r\n", mepData->dot1agCfmMepIdentifier);
  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMepData = mepData;
  if (mepData->dot1agCfmGblMepVar.maDefectIndication == OFDPA_TRUE)
  {
    smEventMsg.event = dot1agFNGMaDefectIndicatnFNGWhileZero;
  }
  else
  {
    smEventMsg.event = dot1agFNGNotMaDefectIndicatnFNGWhileZero;
  }
  if ((smEventMsg.pMepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback for LTIT timer
*
* @param    handle  @b{(input)) pointer to CFM object.
*
*
* @returns  None
*
* @comments Refer Section 20.37.1
*
* @end
*******************************************************************************/
void dot1agCfmltitTimerCallback(void *handle)
{
  dot1agCFM_t *dot1agCFM = handle;

  if (dot1agCFM == NULL)
  {
    return;
  }
  DOT1AG_TRACE(CFM_TIMERS, "LTIT Timer expired\r\n");
  dot1agCFM->dot1agCfmLtmrVar.ltitTimer = NULL;
}

/******************************************************************************
* @purpose  This timer callback is used to timeout a PROLM.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmProlmTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj;
  dot1agCfmSMEventMsg_t smEventMsg;
  y1731CfmMepProlmTable_t *proLmEntry = handle;

  if (proLmEntry == NULL)
  {
    return;
  }

  pMEPObj = proLmEntry->pMPObj;
  if (pMEPObj == NULL)
  {
    return;
  }

  DOT1AG_TRACE(CFM_TIMERS, "PROLM Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.priority = proLmEntry->prolmPriorityClass;
  smEventMsg.event  = y1731PROLMWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback is used to timeout a ONDEM LM.
*
* @param    handle  @b{(input)) pointer to MEP object & ControlPriority.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmOndemlmTimerCallback(void *handle)
{
  dot1agCfmStackObj_t       *pMEPObj;
  dot1agCfmSMEventMsg_t      smEventMsg;
  y1731CfmMepOndemlmTable_t *mepOndemlmEntry = handle;

  DOT1AG_TRACE(CFM_TIMERS, "ONDEMLM Timer expired\r\n");
  if (mepOndemlmEntry == NULL)
  {
    return;
  }

  pMEPObj = mepOndemlmEntry->pMPObj;
  if (pMEPObj == NULL)
  {
    return;
  }

  DOT1AG_TRACE(CFM_TIMERS, "ONDEMLM Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d, priority %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0,
               mepOndemlmEntry->ondemlmControlPriority);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj   = pMEPObj;
  smEventMsg.priority = mepOndemlmEntry->ondemlmControlPriority;
  smEventMsg.event    = y1731ONDEMLMMWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback is used to timeout a DM.
*
* @param    handle  @b{(input)) pointer to MEP object & ControlPriority.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmDmmTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj;
  dot1agCfmSMEventMsg_t smEventMsg;
  y1731CfmMepDmTable_t *mepDmEntry = handle;

  DOT1AG_TRACE(CFM_TIMERS, "DMM Timer expired\r\n");
  if (mepDmEntry == NULL)
  {
    return;
  }

  pMEPObj = mepDmEntry->pMPObj;
  if (pMEPObj == NULL)
  {
    return;
  }

  DOT1AG_TRACE(CFM_TIMERS, "DMM Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d, priority %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0,
               mepDmEntry->dmControlPriority);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj   = pMEPObj;
  smEventMsg.priority = mepDmEntry->dmControlPriority;
  smEventMsg.event    = y1731DMMWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback sees that the AISs are sent at configured MEP
*           interval.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmAisSendTimerCallback(void *handle)
{
  dot1agCfmMepTable_t  *pmepData = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pmepData == NULL)
  {
    return;
  }
  DOT1AG_TRACE(CFM_DEFECT, "AIS Send Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pmepData->dot1agCfmMepIfIndex,
               pmepData->dot1agCfmMepPrimaryVid,
               pmepData->maData->mdData->dot1agCfmMdMdLevel,
               pmepData->dot1agCfmMepDirection,
               pmepData->dot1agCfmMepIdentifier);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMepData = pmepData;
  smEventMsg.event    = y1731AisSendWhileZero;

  if ((smEventMsg.pMepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback sees that the AISs are received at configured MEP
*           interval.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmAisRcvTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }
  DOT1AG_TRACE(CFM_DEFECT, "AIS RCV Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = y1731AisRcvWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback sees that the LCKs are sent at configured MEP
*           interval.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmLckSendTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }
  DOT1AG_TRACE(CFM_DEFECT, "LCK Send Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = y1731LckSendWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback sees that the LCKs are received at configured MEP
*           interval.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmLckRcvTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }
  DOT1AG_TRACE(CFM_DEFECT, "LCK RCV Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = y1731LckRcvWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback sees that the CSFs are sent at configured MEP
*           interval.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmCsfSendTimerCallback(void *handle)
{
  dot1agCfmMepTable_t  *pmepData = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pmepData == NULL)
  {
    return;
  }
  DOT1AG_TRACE(CFM_DEFECT, "CSF Send Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pmepData->dot1agCfmMepIfIndex,
               pmepData->dot1agCfmMepPrimaryVid,
               pmepData->maData->mdData->dot1agCfmMdMdLevel,
               pmepData->dot1agCfmMepDirection,
               pmepData->dot1agCfmMepIdentifier);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMepData = pmepData;
  smEventMsg.event    = y1731CsfSendWhileZero;

  if ((smEventMsg.pMepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback sees that the CSFs are received at configured MEP
*           interval.
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @end
*******************************************************************************/
void y1731CfmCsfRcvTimerCallback(void *handle)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }
  DOT1AG_TRACE(CFM_DEFECT, "AIS RCV Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  smEventMsg.event  = y1731CsfRcvWhileZero;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

/******************************************************************************
* @purpose  This timer callback is used to timeout cross connect CCMs
*           (based on MD levels).
*
* @param    handle  @b{(input)) pointer to MEP object.
*
* @returns  None
*
* @comments Refer Section 20.5.4
*
* @end
*******************************************************************************/
void y1731DefectTimerCallback(void *handle, y1731_defect_type_t defType)
{
  dot1agCfmStackObj_t  *pMEPObj = handle;
  dot1agCfmSMEventMsg_t smEventMsg;

  if (pMEPObj == NULL)
  {
    return;
  }

  DOT1AG_TRACE(CFM_DEFECT, "Y1731 type [%d] Timer expired MPObj intIfNum %d, vlanId %d, level %d, direction %d, mepId %d\r\n",
               defType,
               pMEPObj->key.dot1agCfmStackifIndex,
               pMEPObj->key.dot1agCfmStackVlanIdOrNone,
               pMEPObj->key.dot1agCfmStackMdLevel,
               pMEPObj->key.dot1agCfmStackDirection,
               (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);
  memset(&smEventMsg, 0, sizeof(smEventMsg));
  smEventMsg.pMPObj = pMEPObj;
  switch (defType)
  {
    case Y1731_DEFECT_TYPE_UNM:
      smEventMsg.event = y1731dUNMCCMWhileZero;
      break;
    case Y1731_DEFECT_TYPE_UNP:
      smEventMsg.event = y1731dUNPCCMWhileZero;
      break;
    case Y1731_DEFECT_TYPE_MMG:
      smEventMsg.event = y1731dMMGCCMWhileZero;
      break;
    case Y1731_DEFECT_TYPE_UNL:
      smEventMsg.event = y1731dUNLCCMWhileZero;
      break;
    case Y1731_DEFECT_TYPE_UNPTR:
      smEventMsg.event = y1731dUNPtrCCMWhileZero;
      break;
    default:
      return;
  }
  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_TRUE);
  }
  else
  {
    dot1agCfmStateMachineClassifier(&smEventMsg, OFDPA_FALSE);
  }
}

void y1731DefectUNMTimerCallback(void *handle)
{
  y1731DefectTimerCallback(handle, Y1731_DEFECT_TYPE_UNM);
}

void y1731DefectUNPTimerCallback(void *handle)
{
  y1731DefectTimerCallback(handle, Y1731_DEFECT_TYPE_UNP);
}

void y1731DefectMMGTimerCallback(void *handle)
{
  y1731DefectTimerCallback(handle, Y1731_DEFECT_TYPE_MMG);
}

void y1731DefectUNLTimerCallback(void *handle)
{
  y1731DefectTimerCallback(handle, Y1731_DEFECT_TYPE_UNL);
}

void y1731DefectUNPTRTimerCallback(void *handle)
{
  y1731DefectTimerCallback(handle, Y1731_DEFECT_TYPE_UNPTR);
}
