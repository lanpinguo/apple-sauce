/*********************************************************************
 *
 * (C) Copyright Broadcom Corporation 2000-2013
 *
 **********************************************************************
 * @filename  g8131_sm.h
 *
 * @purpose   Header file for CFM State Machines
 *
 * @component G8131
 *
 * @comments  none
 *
 * @create    28/10/2013
 *
 * @author    Jianpiha
 *
 * @end
 **********************************************************************/
#ifndef INCLUDE_G8131_SM_H
#define INCLUDE_G8131_SM_H

#define G8131_APS_SWITCH_PACKET_COUNT 3

typedef enum g8131SMRequest_s
{
  /*Local request*/
  G8131_LR_START = 0,
  G8131_LR_A_LOCKOUT = G8131_LR_START,  /*A*/
  G8131_LR_B_FS,                        /*B: force switch*/ 
  G8131_LR_C_SF,                        /*C: signal fault on working*/
  G8131_LR_D_SF_DIS,                    /*D: signal fault on working disappear*/
  G8131_LR_E_SF_P,                      /*E: signal fault on protection*/
  G8131_LR_F_SF_P_DIS,                  /*F: signal fault on protection disappear*/
  G8131_LR_G_SD,                        /*G: signal degrade on working*/
  G8131_LR_H_SD_DIS,                    /*H: signal degrade on working disappear*/
  G8131_LR_I_SD_P,                      /*I: signal degrade on protection*/
  G8131_LR_J_SD_P_DIS,                  /*J: signal degrade on protection disappear*/
  G8131_LR_K_MS_P,                      /*K: manual switch to protection*/
  G8131_LR_L_MS,                        /*L: manual switch to working*/
  G8131_LR_M_CLEAR,                     /*M*/
  G8131_LR_N_EXER,                      /*N*/
  G8131_LR_O_WTR_EXPIRED,               /*O*/
  G8131_LR_END = G8131_LR_O_WTR_EXPIRED,

  /*Remote request*/
  G8131_RR_START,
  G8131_RR_P_LO_NULL = G8131_RR_START,	/*15*/
  G8131_RR_Q_SF_P_NULL,
  G8131_RR_R_FS_NORMAL,
  G8131_RR_S_SF_NORMAL,
  G8131_RR_T_SD_NORMAL,
  G8131_RR_U_SD_NULL,
  G8131_RR_V_MS_NORMAL,
  G8131_RR_W_MS_NULL,
  G8131_RR_X_WTR_NORMAL,
  G8131_RR_Y_EXER_NULL,
  G8131_RR_YY_EXER_NORMAL,
  G8131_RR_Z_RR_NULL,
  G8131_RR_ZZ_RR_NORMAL,
  G8131_RR_AA_NR_NULL,
  G8131_RR_AB_NR_NORMAL,
  G8131_RR_AC_DNR_NORMAL,
  G8131_RR_END = G8131_RR_AC_DNR_NORMAL,

  G8131_REQUEST_IGNORE       /*No request should be sent to peer point.*/

}g8131SMRequest_t;

#define G8131_REQUEST_NUM ((G8131_RR_END - G8131_RR_START + 1) \
                          + (G8131_LR_END - G8131_LR_START + 1))

typedef enum g8131SMState_s
{
  G8131_STATE_START = 0,
  G8131_STATE_A_NR_NULL = G8131_STATE_START, /*A:(no request)    NR[r/b=null]    */
  G8131_STATE_B_NR_NORM,                     /*B:(no request)     NR[r/b=normal]  */
  G8131_STATE_C_LO_NULL,                     /*C:(lockout)        LO[r/b=null]    */
  G8131_STATE_D_FS_NORM,                     /*D:(force switch)   FS[r/b=normal]  */
  G8131_STATE_E_SF_NORM,                     /*E:(signal fail)    SF[r/b=normal]  */
  G8131_STATE_F_SFP_NUL,                     /*F:(signal fail)    SF-P[r/b=null]  */
  G8131_STATE_G_MS_NORM,                     /*G:(manual switch)  MS[r/b=normal]  */
  G8131_STATE_H_MS_NULL,                     /*H:(manual switch)  MS[r/b=null]    */
  G8131_STATE_I_WTR_NOR,                     /*I:(wait to restore)WTR[r/b=normal] */
  G8131_STATE_J_DNR_NOR,                     /*J:(do not revert)  DNR[r/b=normal] */
  G8131_STATE_K_EXE_NUL,                     /*K:(exercise)       EXER[r/b=null]  */
  G8131_STATE_L_EXE_NOR,                     /*L:(exercise)       EXER[r/b=normal]*/
  G8131_STATE_M_RR_NULL,                     /*M:(reverse request)RR[r/b=null]    */
  G8131_STATE_N_RR_NORM,                     /*N:(reverse request)RR[r/b=normal]  */
  G8131_STATE_P_SD_NORM,                     /*P:(signal degrade) SD[r/b=normal]  */
  G8131_STATE_Q_SD_NULL,                     /*Q:(signal degrade) SD[r/b=null]    */
  G8131_STATE_END = G8131_STATE_Q_SD_NULL
}g8131SMState_t;

typedef enum
{
  G8131_ACTION_SWITCH_N = 0, /*not switch*/
  G8131_ACTION_SWITCH_Y = 1, /*do switch*/
  G8131_ACT_MAX         = G8131_ACTION_SWITCH_Y
}g8131SMAction_t;

typedef enum
{
  G8131_TABLE_OK = 0, /*From standard '->', this case should be handled. */
  G8131_TABLE_NA = 1, /*From standard 'N/A'*/
  G8131_TABLE_NO = 2  /*From standard 'O'*/
}g8131SMTbFlag_t;

typedef struct
{
  g8131SMTbFlag_t   flag; /*If the flag is {G8131_TABLE_NA | G8131_TABLE_NO}, ignored.*/
  g8131SMAction_t   action; 
  g8131SMRequest_t  newRequest;/*Range{g8131RemotRequestStart, g8131RemotRequestEnd}*/
  g8131SMState_t    newState;
}g8131StateAction_t;

typedef enum
{
  G8131_REQUEST_PRIORITY_NR   = 0,
  G8131_REQUEST_PRIORITY_DNR  = 1,
  G8131_REQUEST_PRIORITY_RR   = 2,
  G8131_REQUEST_PRIORITY_EXER = 4,
  G8131_REQUEST_PRIORITY_WTR  = 5,
  G8131_REQUEST_PRIORITY_DEP  = 6,
  G8131_REQUEST_PRIORITY_MS   = 7,
  G8131_REQUEST_PRIORITY_SD   = 9, 
  G8131_REQUEST_PRIORITY_SF_W = 11,
  G8131_REQUEST_PRIORITY_FS   = 13,
  G8131_REQUEST_PRIORITY_SF_P = 14,
  G8131_REQUEST_PRIORITY_LO   = 15
}g8131_request_priority_t;

typedef struct g8131ReqMsg_s
{
  uint32_t         grpId;
  g8131SMRequest_t  request;  
  uint8_t          signal;
}g8131ReqMsg_t;

typedef struct g8131PduMsg_s
{
  uint32_t  vlanId;
  uint32_t  innerVlanId;
  uint32_t  dataLength;
  uint8_t  *g8131Pkt;
  uint32_t  cfmPduOffset;
}g8131PduMsg_t;

typedef struct g8131Msg_s
{
  uint32_t event;
  uint32_t intf;
  union
  {
    g8131ReqMsg_t reqMsg;
    g8131PduMsg_t pduMsg;
  }data;
} g8131MgmtMsg_t;

OFDPA_ERROR_t g8131ConvRequest2Field(g8131SMRequest_t newRequest, uint8_t *request, 
                               uint8_t *type, uint8_t *requestSig, uint8_t *bridgeSig);

OFDPA_ERROR_t g8131ConvField2Request(uint8_t request, uint8_t type, uint8_t requestSig,
                               uint8_t bridgeSig, g8131SMRequest_t *newRequest);

OFDPA_ERROR_t g8131PgCreateHandler(uint32_t grpId);

OFDPA_ERROR_t g8131PgDeleteHandler(uint32_t grpId);

OFDPA_BOOL g8131StateMachineIsEnabled(void);

void g8131AlarmCMHandler(uint32_t grpId);

void g8131AlarmPMHandler(uint32_t grpId);

OFDPA_ERROR_t g8131StateMachineClassfier(uint32_t grpId, g8131SMRequest_t request, uint8_t signal);

#endif
