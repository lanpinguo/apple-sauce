/*********************************************************************
 *
 * (C) Copyright Broadcom Corporation 2000-2015
 *
 **********************************************************************
 * @filename  g8131_sm.c
 *
 * @purpose   Header file for CFM State Machines
 *
 * @component G8131
 *
 * @comments  none
 *
 * @create    28/10/2013
 *
 * @end
 **********************************************************************/
#include <string.h>
#include "g8131_include.h"
#include "ofdb_api.h"
//#include "driver_group.h"
#include "datapath.h"

extern g8131Cfg_t   *g8131Cfg;

static char g8131ConvertFlag(g8131SMTbFlag_t flag)
{
  switch (flag)
  {
    case G8131_TABLE_NA:
    case G8131_TABLE_NO:
      return '-';
    default:
      return 'Y';
  }
}

static char g8131ConvertState(g8131SMState_t state)
{
  switch (state)
  {
    case G8131_STATE_A_NR_NULL:
      return 'A';
    case G8131_STATE_B_NR_NORM:
      return 'B';
    case G8131_STATE_C_LO_NULL:
      return 'C';
    case G8131_STATE_D_FS_NORM:
      return 'D';
    case G8131_STATE_E_SF_NORM:
      return 'E';
    case G8131_STATE_F_SFP_NUL:
      return 'F';
    case G8131_STATE_G_MS_NORM:
      return 'G';
    case G8131_STATE_H_MS_NULL:
      return 'H';
    case G8131_STATE_I_WTR_NOR:
      return 'I';
    case G8131_STATE_J_DNR_NOR:
      return 'J';
    case G8131_STATE_K_EXE_NUL:
      return 'K';
    case G8131_STATE_L_EXE_NOR:
      return 'L';
    case G8131_STATE_M_RR_NULL:
      return 'M';
    case G8131_STATE_N_RR_NORM:
      return 'N';
    case G8131_STATE_P_SD_NORM:
      return 'P';
    case G8131_STATE_Q_SD_NULL:
      return 'Q';
    default:
      return '-';
  }
}

static char *g8131ConvertRequest(g8131SMRequest_t request)
{
  switch (request)
  {
    /*Local event*/
    case G8131_LR_A_LOCKOUT:
      return "a";
    case G8131_LR_B_FS:
      return "b";
    case G8131_LR_C_SF:
      return "c";
    case G8131_LR_D_SF_DIS:
      return "d";
    case G8131_LR_E_SF_P:
      return "e";
    case G8131_LR_F_SF_P_DIS:
      return "f";
    case G8131_LR_G_SD:
      return "g";
    case G8131_LR_H_SD_DIS:
      return "h";
    case G8131_LR_I_SD_P:
      return "i";
    case G8131_LR_J_SD_P_DIS:
      return "j";
    case G8131_LR_K_MS_P:
      return "k";
    case G8131_LR_L_MS:
      return "l";
    case G8131_LR_M_CLEAR:
      return "m";
    case G8131_LR_N_EXER:
      return "n";
    case G8131_LR_O_WTR_EXPIRED:
      return "o";

      /*Remote event*/
    case G8131_RR_P_LO_NULL:
      return "p";
    case G8131_RR_Q_SF_P_NULL:
      return "q";
    case G8131_RR_R_FS_NORMAL:
      return "r";
    case G8131_RR_S_SF_NORMAL:
      return "s";
    case G8131_RR_T_SD_NORMAL:
      return "t";
    case G8131_RR_U_SD_NULL:
      return "u";
    case G8131_RR_V_MS_NORMAL:
      return "v";
    case G8131_RR_W_MS_NULL:
      return "w";
    case G8131_RR_X_WTR_NORMAL:
      return "x";
    case G8131_RR_Y_EXER_NULL:
      return "y";
    case G8131_RR_YY_EXER_NORMAL:
      return "yy";
    case G8131_RR_Z_RR_NULL:
      return "z";
    case G8131_RR_ZZ_RR_NORMAL:
      return "zz";
    case G8131_RR_AA_NR_NULL:
      return "aa";
    case G8131_RR_AB_NR_NORMAL:
      return "ab";
    case G8131_RR_AC_DNR_NORMAL:
      return "ac";

    default:
      return "-";
  }
}

static char g8131ConvertAction(g8131SMAction_t action)
{
  switch (action)
  {
    case G8131_ACTION_SWITCH_N:
      return 'N';
    default:
      return 'Y';
  }
}

g8131StateAction_t g8131LocalSMTable_state_A[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*k*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_V_MS_NORMAL,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_W_MS_NULL,    G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*n*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_Y_EXER_NULL,  G8131_STATE_K_EXE_NUL },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*x*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*y*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_Z_RR_NULL,    G8131_STATE_M_RR_NULL },
  /*yy*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*z*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*zz*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*AA*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL }, /*To be handled by software*/
  /*AB*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*AC*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM }  /*diffent for REVERTIVE & NON-REVERTIVE */
};

static void g8131StateMachineA(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  g8131SMRequest_t equalReq; /* equivalent request */
  OFDPA_BOOL sf_w_flag = g8131Cfg->pgData[grpId].sfwState;
  OFDPA_BOOL sf_p_flag = g8131Cfg->pgData[grpId].sfpState;
  OFDPA_BOOL sd_w_flag = g8131Cfg->pgData[grpId].sdwState;
  OFDPA_BOOL sd_p_flag = g8131Cfg->pgData[grpId].sdpState;

  switch (request)
  {
    case G8131_RR_AA_NR_NULL:
      if (sf_w_flag == OFDPA_TRUE) equalReq = G8131_LR_C_SF;
      else if (sf_p_flag == OFDPA_TRUE) equalReq = G8131_LR_E_SF_P;
      else if (sd_w_flag == OFDPA_TRUE) equalReq = G8131_LR_G_SD;
      else if (sd_p_flag == OFDPA_TRUE) equalReq = G8131_LR_I_SD_P;
      else equalReq = G8131_RR_AB_NR_NORMAL;

      *flag   = g8131LocalSMTable_state_A[equalReq].flag;
      *action = g8131LocalSMTable_state_A[equalReq].action;
      *newReq = g8131LocalSMTable_state_A[equalReq].newRequest;
      *newSta = g8131LocalSMTable_state_A[equalReq].newState;
      break;

    case G8131_RR_AC_DNR_NORMAL:
      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE) /*revertive mode*/
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_AB_NR_NORMAL;
        *newSta = G8131_STATE_B_NR_NORM;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_AC_DNR_NORMAL;
        *newSta = G8131_STATE_J_DNR_NOR;
      }
      break;

    default:
      *flag   = g8131LocalSMTable_state_A[request].flag;
      *action = g8131LocalSMTable_state_A[request].action;
      *newReq = g8131LocalSMTable_state_A[request].newRequest;
      *newSta = g8131LocalSMTable_state_A[request].newState;
      break;

  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_B[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL },
  /*k*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_V_MS_NORMAL,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_W_MS_NULL,    G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*n*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL }, /*Different from the standard*/
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL }, /*Different from the standard*/
  /*x*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*y*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*yy*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*z*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*zz*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM },
  /*AA*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM }, /*To be handled by software*/
  /*AB*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM }, /*To be handled by software*/
  /*AC*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_B_NR_NORM }  /*To be handled by software*/
};

static void g8131StateMachineB(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  g8131SMRequest_t lastLocalReq = g8131Cfg->pgData[grpId].lastLReq;
  g8131SMRequest_t equalReq; /* equivalent request */
  OFDPA_BOOL sf_w_flag = g8131Cfg->pgData[grpId].sfwState;
  OFDPA_BOOL sd_w_flag = g8131Cfg->pgData[grpId].sdwState;

  switch (request)
  {
    case G8131_RR_AA_NR_NULL:
      if (sf_w_flag == OFDPA_TRUE) equalReq = G8131_LR_C_SF;
      else if (sd_w_flag == OFDPA_TRUE) equalReq = G8131_LR_G_SD;
      else equalReq = G8131_RR_P_LO_NULL;

      *flag   = g8131LocalSMTable_state_B[equalReq].flag;
      *action = g8131LocalSMTable_state_B[equalReq].action;
      *newReq = g8131LocalSMTable_state_B[equalReq].newRequest;
      *newSta = g8131LocalSMTable_state_B[equalReq].newState;
      break;

    case G8131_RR_AB_NR_NORMAL:
      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
      	/*leishenghua modify 20170118, sf_dis说明之前是sf状态*/
        if ((G8131_LR_D_SF_DIS == lastLocalReq) ||
            (G8131_LR_H_SD_DIS == lastLocalReq))
        {
          *flag   = G8131_TABLE_OK;
          *action = G8131_ACTION_SWITCH_N;
          *newReq = G8131_RR_X_WTR_NORMAL;
          *newSta = G8131_STATE_I_WTR_NOR;
        }
        else
        {
          *flag   = g8131LocalSMTable_state_B[G8131_RR_P_LO_NULL].flag;
          *action = g8131LocalSMTable_state_B[G8131_RR_P_LO_NULL].action;
          *newReq = g8131LocalSMTable_state_B[G8131_RR_P_LO_NULL].newRequest;
          *newSta = g8131LocalSMTable_state_B[G8131_RR_P_LO_NULL].newState;
        }
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_AC_DNR_NORMAL;
        *newSta = G8131_STATE_J_DNR_NOR;
      }
      break;

    case G8131_RR_AC_DNR_NORMAL:
      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_REQUEST_IGNORE;
        *newSta = G8131_STATE_B_NR_NORM;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_AC_DNR_NORMAL;
        *newSta = G8131_STATE_J_DNR_NOR;
      }
      break;

    default:
      *flag   = g8131LocalSMTable_state_B[request].flag;
      *action = g8131LocalSMTable_state_B[request].action;
      *newReq = g8131LocalSMTable_state_B[request].newRequest;
      *newSta = g8131LocalSMTable_state_B[request].newState;
      break;
  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_C[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*c*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*d*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*e*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*f*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*g*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*h*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*i*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*j*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*k*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*l*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*m*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_A_NR_NULL }, /*To be handled by software*/
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*q*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*r*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*s*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*t*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*u*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*v*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*w*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*x*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*y*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*yy*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*z*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*zz*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_C_LO_NULL }
};

static void g8131StateMachineC(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  OFDPA_BOOL sf_w_flag = g8131Cfg->pgData[grpId].sfwState;
  OFDPA_BOOL sf_p_flag = g8131Cfg->pgData[grpId].sfpState;
  OFDPA_BOOL sd_w_flag = g8131Cfg->pgData[grpId].sdwState;
  OFDPA_BOOL sd_p_flag = g8131Cfg->pgData[grpId].sdpState;

  switch (request)
  {
    case G8131_LR_M_CLEAR:
      if (sf_w_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_S_SF_NORMAL;
        *newSta = G8131_STATE_E_SF_NORM;
        break;
      }

      if (sf_p_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_Q_SF_P_NULL;
        *newSta = G8131_STATE_F_SFP_NUL;
        break;
      }

      if (sd_w_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_T_SD_NORMAL;
        *newSta = G8131_STATE_P_SD_NORM;
        break;
      }

      if (sd_p_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_U_SD_NULL;
        *newSta = G8131_STATE_Q_SD_NULL;
        break;
      }

      *flag   = G8131_TABLE_OK;
      *action = G8131_ACTION_SWITCH_N;
      *newReq = G8131_RR_AA_NR_NULL;
      *newSta = G8131_STATE_A_NR_NULL;
      break;

    default:
      *flag   = g8131LocalSMTable_state_C[request].flag;
      *action = g8131LocalSMTable_state_C[request].action;
      *newReq = g8131LocalSMTable_state_C[request].newRequest;
      *newSta = g8131LocalSMTable_state_C[request].newState;
      break;
  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_D[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,   G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*d*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL, G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*g*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*h*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*i*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*j*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*k*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*l*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM }, /*To be handled by software*/
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,  G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,  G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*s*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*t*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*u*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*v*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*w*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*x*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*y*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*yy*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*z*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*zz*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_D_FS_NORM }
};

static void g8131StateMachineD(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  OFDPA_BOOL sf_w_flag = g8131Cfg->pgData[grpId].sfwState;
  OFDPA_BOOL sd_w_flag = g8131Cfg->pgData[grpId].sdwState;
  OFDPA_BOOL sd_p_flag = g8131Cfg->pgData[grpId].sdpState;

  switch (request)
  {
    case G8131_LR_M_CLEAR:
      if (sf_w_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_S_SF_NORMAL;
        *newSta = G8131_STATE_E_SF_NORM;
        break;
      }

      if (sd_w_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_T_SD_NORMAL;
        *newSta = G8131_STATE_P_SD_NORM;
        break;
      }

      if (sd_p_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_U_SD_NULL;
        *newSta = G8131_STATE_Q_SD_NULL;
        break;
      }

      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_AA_NR_NULL;
        *newSta = G8131_STATE_A_NR_NULL;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_AC_DNR_NORMAL;
        *newSta = G8131_STATE_J_DNR_NOR;
      }

      break;

    default:
      *flag   = g8131LocalSMTable_state_D[request].flag;
      *action = g8131LocalSMTable_state_D[request].action;
      *newReq = g8131LocalSMTable_state_D[request].newRequest;
      *newSta = g8131LocalSMTable_state_D[request].newState;
      break;

  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_E[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL }, /*To be handled by software*/
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*g*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*h*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*i*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*j*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*k*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*l*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*t*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*u*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*v*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*w*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*x*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*y*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*yy*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*z*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*zz*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_E_SF_NORM }
};

static void g8131StateMachineE(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  OFDPA_BOOL sd_w_flag = g8131Cfg->pgData[grpId].sdwState;
  OFDPA_BOOL sd_p_flag = g8131Cfg->pgData[grpId].sdpState;

  switch (request)
  {
    case G8131_LR_D_SF_DIS:
      if (sd_w_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_T_SD_NORMAL;
        *newSta = G8131_STATE_P_SD_NORM;
        break;
      }

      if (sd_p_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_U_SD_NULL;
        *newSta = G8131_STATE_Q_SD_NULL;
        break;
      }

      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_X_WTR_NORMAL;
        *newSta = G8131_STATE_I_WTR_NOR;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_AC_DNR_NORMAL;
        *newSta = G8131_STATE_J_DNR_NOR;
      }
      break;

    default:
      *flag   = g8131LocalSMTable_state_E[request].flag;
      *action = g8131LocalSMTable_state_E[request].action;
      *newReq = g8131LocalSMTable_state_E[request].newRequest;
      *newSta = g8131LocalSMTable_state_E[request].newState;
      break;

  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_F[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_P_LO_NULL,   G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*c*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*d*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*e*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL }, /*To be handled by software*/
  /*g*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*h*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*i*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*j*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*k*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*l*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,  G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*r*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*s*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*t*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*u*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*v*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*w*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*x*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*y*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*yy*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*z*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*zz*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE, G8131_STATE_F_SFP_NUL }
};

static void g8131StateMachineF(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  OFDPA_BOOL sf_w_flag = g8131Cfg->pgData[grpId].sfwState;
  OFDPA_BOOL sd_w_flag = g8131Cfg->pgData[grpId].sdwState;
  OFDPA_BOOL sd_p_flag = g8131Cfg->pgData[grpId].sdpState;

  switch (request)
  {
    case G8131_LR_F_SF_P_DIS:
      if (sf_w_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_S_SF_NORMAL;
        *newSta = G8131_STATE_E_SF_NORM;
        break;
      }

      if (sd_w_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_T_SD_NORMAL;
        *newSta = G8131_STATE_P_SD_NORM;
        break;
      }

      if (sd_p_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_U_SD_NULL;
        *newSta = G8131_STATE_Q_SD_NULL;
        break;
      }

      *flag   = G8131_TABLE_OK;
      *action = G8131_ACTION_SWITCH_N;
      *newReq = G8131_RR_AA_NR_NULL;
      *newSta = G8131_STATE_A_NR_NULL;
      break;

    default:
      *flag   = g8131LocalSMTable_state_F[request].flag;
      *action = g8131LocalSMTable_state_F[request].action;
      *newReq = g8131LocalSMTable_state_F[request].newRequest;
      *newSta = g8131LocalSMTable_state_F[request].newState;
      break;
  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_G[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*k*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*m*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL }, /*To be handled by software*/
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*w*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL }, /*To be handled by software*/
  /*x*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*y*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*yy*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*z*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*zz*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_G_MS_NORM }
};

static void g8131StateMachineG(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  g8131SMRequest_t lastRemoteReq = g8131Cfg->pgData[grpId].lastRReq;

  switch (request)
  {
    case G8131_LR_M_CLEAR:
      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_AA_NR_NULL;
        *newSta = G8131_STATE_A_NR_NULL;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_AC_DNR_NORMAL;
        *newSta = G8131_STATE_J_DNR_NOR;
      }
      break;

    case G8131_RR_W_MS_NULL:
      if (G8131_RR_AB_NR_NORMAL == lastRemoteReq) /*Simplify check whether the previous RR is NR_NORMAL.*/
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_REQUEST_IGNORE;
        *newSta = G8131_STATE_G_MS_NORM;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_AA_NR_NULL;
        *newSta = G8131_STATE_A_NR_NULL;
      }
      break;

    default:
      *flag   = g8131LocalSMTable_state_G[request].flag;
      *action = g8131LocalSMTable_state_G[request].action;
      *newReq = g8131LocalSMTable_state_G[request].newRequest;
      *newSta = g8131LocalSMTable_state_G[request].newState;
      break;

  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_H[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*k*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*l*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*x*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*y*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*yy*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*z*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*zz*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL }
};

static void g8131StateMachineH(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  *flag   = g8131LocalSMTable_state_H[request].flag;
  *action = g8131LocalSMTable_state_H[request].action;
  *newReq = g8131LocalSMTable_state_H[request].newRequest;
  *newSta = g8131LocalSMTable_state_H[request].newState;
  return;
}

g8131StateAction_t g8131LocalSMTable_state_I[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_H_MS_NULL },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*k*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_V_MS_NORMAL,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_W_MS_NULL,    G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*o*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*x*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*y*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*yy*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*z*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*zz*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },

  /*leishenghua modify 20161214, 主用链路down又快速恢复后，不会进入WTR状态，
   而是直接切回了主，状态机实现和标准不符：
   标准规定在WTR状态，收到NR(NULL)状态不处理*/
  /*AA*//*{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL }, */   /*Different from the standard*/
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,	G8131_STATE_I_WTR_NOR },

  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_I_WTR_NOR }
};

static void g8131StateMachineI(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  *flag   = g8131LocalSMTable_state_I[request].flag;
  *action = g8131LocalSMTable_state_I[request].action;
  *newReq = g8131LocalSMTable_state_I[request].newRequest;
  *newSta = g8131LocalSMTable_state_I[request].newState;
  return;
}

g8131StateAction_t g8131LocalSMTable_state_J[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*k*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_V_MS_NORMAL,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_W_MS_NULL,    G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*n*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_YY_EXER_NORMAL, G8131_STATE_L_EXE_NOR },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*x*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*y*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*yy*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_ZZ_RR_NORMAL, G8131_STATE_N_RR_NORM },
  /*z*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*zz*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR },
  /*AC*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_J_DNR_NOR }
};

static void g8131StateMachineJ(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  *flag   = g8131LocalSMTable_state_J[request].flag;
  *action = g8131LocalSMTable_state_J[request].action;
  *newReq = g8131LocalSMTable_state_J[request].newRequest;
  *newSta = g8131LocalSMTable_state_J[request].newState;
  return;
}

g8131StateAction_t g8131LocalSMTable_state_K[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*k*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_V_MS_NORMAL,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_W_MS_NULL,    G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*x*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL }, /*To be handled by software*/
  /*y*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*yy*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*z*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*zz*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*AB*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_K_EXE_NUL }  /*To be handled by software*/
};

static void g8131StateMachineK(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{

  switch (request)
  {
    case G8131_RR_X_WTR_NORMAL:
      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_NA;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_REQUEST_IGNORE;
        *newSta = G8131_STATE_K_EXE_NUL;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_AB_NR_NORMAL;
        *newSta = G8131_STATE_B_NR_NORM;
      }
      break;

    case G8131_RR_AC_DNR_NORMAL:
      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_NO;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_REQUEST_IGNORE;
        *newSta = G8131_STATE_K_EXE_NUL;
      }
      else
      {
        *flag   = G8131_TABLE_NA;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_REQUEST_IGNORE;
        *newSta = G8131_STATE_K_EXE_NUL;
      }
      break;

    default:
      *flag   = g8131LocalSMTable_state_K[request].flag;
      *action = g8131LocalSMTable_state_K[request].action;
      *newReq = g8131LocalSMTable_state_K[request].newRequest;
      *newSta = g8131LocalSMTable_state_K[request].newState;
      break;
  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_L[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*k*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_V_MS_NORMAL,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_W_MS_NULL,    G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AC_DNR_NORMAL, G8131_STATE_J_DNR_NOR },
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*x*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*y*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*yy*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*z*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*zz*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*AA*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_L_EXE_NOR }
};

static void g8131StateMachineL(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  *flag   = g8131LocalSMTable_state_L[request].flag;
  *action = g8131LocalSMTable_state_L[request].action;
  *newReq = g8131LocalSMTable_state_L[request].newRequest;
  *newSta = g8131LocalSMTable_state_L[request].newState;
  return;
}

g8131StateAction_t g8131LocalSMTable_state_M[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*k*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_V_MS_NORMAL,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_W_MS_NULL,    G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*n*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_Y_EXER_NULL,  G8131_STATE_K_EXE_NUL },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*x*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL }, /*To be handled by software*/
  /*y*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*yy*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*z*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*zz*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*AA*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*AB*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_M_RR_NULL } /*To be handled by software*/
};

static void g8131StateMachineM(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  switch (request)
  {
    case G8131_RR_X_WTR_NORMAL:
      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_NA;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_REQUEST_IGNORE;
        *newSta = G8131_STATE_K_EXE_NUL;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_Y;
        *newReq = G8131_RR_AB_NR_NORMAL;
        *newSta = G8131_STATE_B_NR_NORM;
      }
      break;

    case G8131_RR_AC_DNR_NORMAL:
      if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_NO;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_REQUEST_IGNORE;
        *newSta = G8131_STATE_L_EXE_NOR;
      }
      else
      {
        *flag   = G8131_TABLE_NA;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_REQUEST_IGNORE;
        *newSta = G8131_STATE_L_EXE_NOR;
      }
      break;

    default:
      *flag   = g8131LocalSMTable_state_M[request].flag;
      *action = g8131LocalSMTable_state_M[request].action;
      *newReq = g8131LocalSMTable_state_M[request].newRequest;
      *newSta = g8131LocalSMTable_state_M[request].newState;
      break;
  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_N[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*g*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_T_SD_NORMAL,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*i*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_U_SD_NULL,    G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*k*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_V_MS_NORMAL,  G8131_STATE_G_MS_NORM },
  /*l*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_W_MS_NULL,    G8131_STATE_H_MS_NULL },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*n*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_YY_EXER_NORMAL, G8131_STATE_L_EXE_NOR },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*v*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*w*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*x*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*y*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*yy*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*z*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*zz*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AC_DNR_NORMAL, G8131_STATE_J_DNR_NOR },
  /*AA*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*AB*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_N_RR_NORM },
  /*AC*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AC_DNR_NORMAL, G8131_STATE_J_DNR_NOR }
};

static void g8131StateMachineN(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  *flag   = g8131LocalSMTable_state_N[request].flag;
  *action = g8131LocalSMTable_state_N[request].action;
  *newReq = g8131LocalSMTable_state_N[request].newRequest;
  *newSta = g8131LocalSMTable_state_N[request].newState;
  return;
}

g8131StateAction_t g8131LocalSMTable_state_P[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*g*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*h*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM }, /*To be handled by software*/
  /*i*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*j*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*k*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*l*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*u*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*v*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*w*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*x*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*y*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*yy*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*z*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*zz*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*AA*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*AB*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM },
  /*AC*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_P_SD_NORM }
};

static void g8131StateMachineP(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  OFDPA_BOOL sd_p_flag = g8131Cfg->pgData[grpId].sdpState;

  switch (request)
  {
    case G8131_LR_H_SD_DIS:
      if (sd_p_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_U_SD_NULL;
        *newSta = G8131_STATE_Q_SD_NULL;
      }
      else
      {
        if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
        {
          *flag   = G8131_TABLE_OK;
          *action = G8131_ACTION_SWITCH_N;
          *newReq = G8131_RR_X_WTR_NORMAL;
          *newSta = G8131_STATE_I_WTR_NOR;
        }
        else
        {
          *flag   = G8131_TABLE_OK;
          *action = G8131_ACTION_SWITCH_N;
          *newReq = G8131_RR_AC_DNR_NORMAL;
          *newSta = G8131_STATE_J_DNR_NOR;
        }
      }
      break;

    default:
      *flag   = g8131LocalSMTable_state_P[request].flag;
      *action = g8131LocalSMTable_state_P[request].action;
      *newReq = g8131LocalSMTable_state_P[request].newRequest;
      *newSta = g8131LocalSMTable_state_P[request].newState;
      break;
  }

  return;
}

g8131StateAction_t g8131LocalSMTable_state_Q[G8131_REQUEST_NUM] =
{
/* ITU-T G.8031/Y.1342 Table A.1.1 State transition for 1:1 bidirectional switching with revertive mode*/
/* ITU-T G.8031/Y.1342 Table A.2.1 State transition for 1:1 bidirectional switching with non-revertive mode*/
  /*a*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_P_LO_NULL,    G8131_STATE_C_LO_NULL },
  /*b*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_R_FS_NORMAL,  G8131_STATE_D_FS_NORM },
  /*c*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_S_SF_NORMAL,  G8131_STATE_E_SF_NORM },
  /*d*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*e*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_Q_SF_P_NULL,  G8131_STATE_F_SFP_NUL },
  /*f*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*g*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*h*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*i*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*j*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_A_NR_NULL }, /*To be handled by software*/
  /*k*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*l*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*m*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*n*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*o*/{ G8131_TABLE_NA, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },

/* ITU-T G.8031/Y.1342 Table A.1.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
/* ITU-T G.8031/Y.1342 Table A.2.2 State transition by far-end requests (1:1 bidirectional, revertive mode)*/
  /*p*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*q*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_Y, G8131_RR_AA_NR_NULL,   G8131_STATE_A_NR_NULL },
  /*r*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*s*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_RR_AB_NR_NORMAL, G8131_STATE_B_NR_NORM },
  /*t*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*u*/{ G8131_TABLE_OK, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*v*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*w*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*x*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*y*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*yy*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*z*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*zz*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*AA*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*AB*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL },
  /*AC*/{ G8131_TABLE_NO, G8131_ACTION_SWITCH_N, G8131_REQUEST_IGNORE,  G8131_STATE_Q_SD_NULL }
};

static void g8131StateMachineQ(uint32_t         grpId,
                               g8131SMRequest_t  request,
                               g8131SMTbFlag_t  *flag,
                               g8131SMAction_t  *action,
                               g8131SMRequest_t *newReq,
                               g8131SMState_t   *newSta)
{
  OFDPA_BOOL sd_w_flag = g8131Cfg->pgData[grpId].sdwState;

  switch (request)
  {
    case G8131_LR_J_SD_P_DIS:
      if (sd_w_flag == OFDPA_TRUE)
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_T_SD_NORMAL;
        *newSta = G8131_STATE_P_SD_NORM;
      }
      else
      {
        *flag   = G8131_TABLE_OK;
        *action = G8131_ACTION_SWITCH_N;
        *newReq = G8131_RR_AA_NR_NULL;
        *newSta = G8131_STATE_A_NR_NULL;
      }
      break;

    default:
      *flag   = g8131LocalSMTable_state_Q[request].flag;
      *action = g8131LocalSMTable_state_Q[request].action;
      *newReq = g8131LocalSMTable_state_Q[request].newRequest;
      *newSta = g8131LocalSMTable_state_Q[request].newState;
      break;
  }

  return;
}

static OFDPA_ERROR_t g8131StateCheck(uint32_t grpId, uint32_t state)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
  {
    if ((G8131_STATE_J_DNR_NOR == state) ||
        (G8131_STATE_L_EXE_NOR == state) ||
        (G8131_STATE_N_RR_NORM == state))
    {
      status = OFDPA_E_UNAVAIL;
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "State %c NOT-SUPPORTED in revertive mode.",
                  g8131ConvertState(state));
    }
  }
  else
  {
    if ((G8131_STATE_I_WTR_NOR == state))
    {
      status = OFDPA_E_UNAVAIL;
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "State %c NOT-SUPPORTED in non-revertive mode.",
                  g8131ConvertState(state));
    }
  }

  return status;
}

static OFDPA_ERROR_t g8131RequestCheck(uint32_t grpId, g8131SMRequest_t req)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  if (g8131Cfg->pgData[grpId].revertiveMode == OFDPA_TRUE)
  {
    if ((G8131_RR_YY_EXER_NORMAL == req) ||
        (G8131_RR_ZZ_RR_NORMAL   == req))
    {
      status = OFDPA_E_UNAVAIL;
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "Request %s NOT-SUPPORTED in revertive mode.",
                  g8131ConvertRequest(req));
    }
  }
  else
  {
    if ((G8131_LR_O_WTR_EXPIRED == req))
    {
      status = OFDPA_E_UNAVAIL;
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "Request %s NOT-SUPPORTED in non-revertive mode.",
                  g8131ConvertRequest(req));
    }
  }

  return status;
}

static OFDPA_ERROR_t g8131StateMachineQuery(uint32_t         grpId,
                                            g8131SMRequest_t  request,
                                            g8131SMAction_t  *action,
                                            g8131SMRequest_t *newReq,
                                            g8131SMState_t   *newSta)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;
  g8131SMTbFlag_t flag;

  status = g8131StateCheck(grpId, g8131Cfg->pgData[grpId].smState);
  if (OFDPA_E_NONE != status)
  {
    return status;
  }

  status = g8131RequestCheck(grpId, request);
  if (OFDPA_E_NONE != status)
  {
    return status;
  }

  /* make sure request is in range for state machine table indexing */
  if (request >= G8131_REQUEST_NUM)
  {
    return OFDPA_E_UNAVAIL;
  }

  switch (g8131Cfg->pgData[grpId].smState)
  {
    case G8131_STATE_A_NR_NULL:
      g8131StateMachineA(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_B_NR_NORM:
      g8131StateMachineB(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_C_LO_NULL:
      g8131StateMachineC(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_D_FS_NORM:
      g8131StateMachineD(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_E_SF_NORM:
      g8131StateMachineE(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_F_SFP_NUL:
      g8131StateMachineF(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_G_MS_NORM:
      g8131StateMachineG(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_H_MS_NULL:
      g8131StateMachineH(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_I_WTR_NOR: /* Only for revertive mode */
      g8131StateMachineI(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_J_DNR_NOR: /* Only for non-revertive mode */
      g8131StateMachineJ(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_K_EXE_NUL:
      g8131StateMachineK(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_L_EXE_NOR: /* Only for non-revertive mode */
      g8131StateMachineL(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_M_RR_NULL:
      g8131StateMachineM(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_N_RR_NORM: /* Only for non-revertive mode */
      g8131StateMachineN(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_P_SD_NORM:
      g8131StateMachineP(grpId, request, &flag, action, newReq, newSta);
      break;

    case G8131_STATE_Q_SD_NULL:
      g8131StateMachineQ(grpId, request, &flag, action, newReq, newSta);
      break;

    default:
      status = OFDPA_E_UNAVAIL; /*Unknown state, just ignore.*/
      return status;
      break;
  }

  G8131_TRACE(G8131_DEBUG_SM,
              "grpId %d: Sta[%-1c] Req[%-2s] -> [%-1c] Act[%-1c] Req[%-2s] Sta[%-1c]",
              grpId, g8131ConvertState(g8131Cfg->pgData[grpId].smState),
              g8131ConvertRequest(request), g8131ConvertFlag(flag),
              g8131ConvertAction(*action), g8131ConvertRequest(*newReq),
              g8131ConvertState(*newSta));
  
  if(*newSta != g8131Cfg->pgData[grpId].smState || *action == G8131_ACTION_SWITCH_Y)
  {
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_BASIC,
			 "grpId %d: Sta[%-1c] Req[%-2s] -> [%-1c] Act[%-1c] Req[%-2s] Sta[%-1c]\r\n",
	              grpId, g8131ConvertState(g8131Cfg->pgData[grpId].smState),
	              g8131ConvertRequest(request), g8131ConvertFlag(flag),
	              g8131ConvertAction(*action), g8131ConvertRequest(*newReq),
	              g8131ConvertState(*newSta));
  }
  
  if (OFDPA_E_NONE != (status = g8131StateCheck(grpId, *newSta)))
  {
    return status;
  }

  if (OFDPA_E_NONE != (status = g8131RequestCheck(grpId, *newReq)))
  {
    return status;
  }

  if (flag == G8131_TABLE_NA)
  {
    status = OFDPA_E_UNAVAIL; /*The 'N/A' should be ignored.*/
  }

  return status;
}

OFDPA_BOOL g8131StateMachineIsEnabled(void)
{
  return g8131Cfg->enable;
}

static void g8131WtrTimerReset(uint32_t grpId)
{
  g8131Cfg->pgData[grpId].wtrCntValue = 0;
}


static OFDPA_BOOL g8131HoldOffTimerIsExpired(uint32_t grpId, g8131SMRequest_t request)
{
  OFDPA_BOOL expiredFlag;

  switch (request)
  {
    /*Hold-off timer only has effect on the local SD/SF requests.*/
    case G8131_LR_C_SF:
    case G8131_LR_E_SF_P:
    case G8131_LR_G_SD:
    case G8131_LR_I_SD_P:
      expiredFlag = !g8131Cfg->pgData[grpId].holdOffBlock;
      break;

      /*Local SD/SD_P/SF/SF_P recovery will delete the hook-off timer.*/
    case G8131_LR_D_SF_DIS:
    case G8131_LR_F_SF_P_DIS:
    case G8131_LR_H_SD_DIS:
    case G8131_LR_J_SD_P_DIS:
      if (NULL != g8131Cfg->pgData[grpId].holdOffTmNode)
      {
        g8131TimerDelete(g8131Cfg->pgData[grpId].holdOffTmNode);
        g8131Cfg->pgData[grpId].holdOffBlock = OFDPA_FALSE;
		g8131Cfg->pgData[grpId].holdOffTmNode = NULL;
      }
      expiredFlag = OFDPA_TRUE;
      break;

    default:
      expiredFlag = OFDPA_TRUE;
      break;
  }

  return expiredFlag;
}

static void g8131HoldOffTimerCallback(void *handle)
{
  pgOperData_t *pgNode = (pgOperData_t *)handle;

  if (!pgNode)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] holdoff timer callback for null protection group elment");
    return;
  }

  if ((pgNode->grpId < OFDPA_G8131_PG_MIN_ID) || (pgNode->grpId > OFDPA_G8131_PG_MAX_ID))
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] holdoff timer callback for invalid group %d", pgNode->grpId);
    return;
  }

  g8131TimerDelete(g8131Cfg->pgData[pgNode->grpId].holdOffTmNode);
  g8131Cfg->pgData[pgNode->grpId].holdOffBlock = OFDPA_FALSE;
  g8131Cfg->pgData[pgNode->grpId].holdOffTmNode = NULL;

  G8131_TRACE(G8131_DEBUG_EVENT, "grp %d hold off timer expired", pgNode->grpId);

  return;
}

static OFDPA_ERROR_t g8131HoldOffTimerStart(uint32_t grpId,
                                            g8131SMRequest_t request)
{
  g8131_timer_list_t *timerNode = NULL;
  uint32_t holdOffValue;

  G8131_PG_CHECK(grpId);

  /* If the hold-off-timer has been started, return directly;*/
  if (g8131Cfg->pgData[grpId].holdOffBlock == OFDPA_TRUE)
  {
    return OFDPA_E_NONE;
  }

  /* Even the hold-off timer is set as 0, it also be initialized as 1(100ms). */
  if (g8131Cfg->pgData[grpId].holdOffTime == 0)
  {
    holdOffValue = 1 * 100;
	/*leishenghua modify 20170112, 创建保护时，主备告警上报间隔太短，
	holdOffTime未被清除，导致备告警丢失*/
	return OFDPA_E_NONE;
  }
  else
  {
    holdOffValue = g8131Cfg->pgData[grpId].holdOffTime;
  }

  /*Hold-off-timer only works on the below request.*/
  switch (request)
  {
    case G8131_LR_C_SF:
    case G8131_LR_E_SF_P:
    case G8131_LR_G_SD:
    case G8131_LR_I_SD_P:
      timerNode = g8131TimerCreate(G8131_TIMER_TYPE_HOLD_OFF_COUNT_DOWN,
                                   holdOffValue / G8131_APS_TIMER_SLICE,
                                   g8131HoldOffTimerCallback,
                                   &g8131Cfg->pgData[grpId]);
      if (NULL == timerNode)
      {
        G8131_TRACE(G8131_DEBUG_ERROR, "[#ERROR#] hold-off timer create failed.");
        return OFDPA_E_FAIL;
      }

      g8131Cfg->pgData[grpId].holdOffBlock  = OFDPA_TRUE;
      g8131Cfg->pgData[grpId].holdOffTmNode = timerNode;
      G8131_TRACE(G8131_DEBUG_SM, "grp %d hold off timer created", grpId);
      break;

    default:
      break;
  }

  return OFDPA_E_NONE;
}

static void g8131ApsPktSendTimerCallBack(void *handle)
{
  OFDPA_ERROR_t status;
  pgOperData_t *pgNode = (pgOperData_t *)handle;
  g8131ReqMsg_t msg;

  if (!pgNode)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] PktSend timer callback for null protection group elment");
    return;
  }

  if ((pgNode->grpId < OFDPA_G8131_PG_MIN_ID) || (pgNode->grpId > OFDPA_G8131_PG_MAX_ID))
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] PktSend timer callback for invalid group %d", pgNode->grpId);
    return;
  }

  G8131_TRACE(G8131_DEBUG_SM, "grp %d [0x%x 0x%x 0x%x] wtr %d", pgNode->grpId,
              pgNode->livenessPortIdWorking, pgNode->livenessPortIdProtection, pgNode->livenessPortIdActive,
              g8131Cfg->pgData[pgNode->grpId].wtrCntValue);

  if (g8131Cfg->pgData[pgNode->grpId].lastSReq == G8131_RR_X_WTR_NORMAL)
  {
    /* BEGIN: Modified by hushouqiang, 2017/1/19 */
    if(g8131Cfg->pgData[pgNode->grpId].wtrCntValue > 0)
    {
      g8131Cfg->pgData[pgNode->grpId].wtrCntValue--;
    }
    /* END:   Modified by hushouqiang, 2017/1/19 */
    if (g8131Cfg->pgData[pgNode->grpId].wtrCntValue == 0)
    {
      if (g8131Cfg->pgData[pgNode->grpId].apsSendTmNode != NULL)
      {
        /*Delete the timer*/
        g8131TimerDelete(g8131Cfg->pgData[pgNode->grpId].apsSendTmNode);
        g8131Cfg->pgData[pgNode->grpId].apsSendTmNode = NULL;

        G8131_TRACE(G8131_DEBUG_SM, "grp %d wtr timer expired", pgNode->grpId);

        /*Trigger the wtr expired state machine.*/
        memset(&msg, 0, sizeof(msg));
        msg.grpId = pgNode->grpId;
        msg.request = G8131_LR_O_WTR_EXPIRED;
        if (OFDPA_E_NONE != g8131IssueCmd(G8131_EVENT_REQUEST, 0, &msg))
        {
          G8131_TRACE(G8131_DEBUG_ERROR, "[#ERROR#] send event-request failed.");
          return;
        }
      }
      return;
    }
  }

  /*Send the APS packet to peer side*/
  g8131PduXmitRequest(pgNode->grpId, g8131Cfg->pgData[pgNode->grpId].lastSReq);

  status = g8131TimerUpdate(g8131Cfg->pgData[pgNode->grpId].apsSendTmNode,
                            &g8131Cfg->pgData[pgNode->grpId],
                            G8131_APS_INTERVAL / G8131_APS_TIMER_SLICE,
                            g8131ApsPktSendTimerCallBack);
  if (OFDPA_E_NONE != status)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] update cycle aps timer grpId %d failed.", pgNode->grpId);
  }

  return;
}

static OFDPA_ERROR_t g8131ApsPktSend(uint32_t grpId,
                                     g8131SMRequest_t newReq)
{
  g8131_timer_list_t *tmNode;

  G8131_PG_CHECK(grpId);
  G8131_PG_REQUEST_CHECK(newReq);

  /*Delete the previous APS packet timer*/
  g8131TimerDelete(g8131Cfg->pgData[grpId].apsSendTmNode);
  g8131Cfg->pgData[grpId].apsSendTmNode = NULL;

  /*Send the APS packet to peer side*/
  g8131PduXmitRequest(grpId, newReq);

  /*Start timer for APS packets continue sending*/
  tmNode = g8131TimerCreate(G8131_TIMER_TYPE_APS_CYCLE_INTERVAL,
                            G8131_APS_INTERVAL / G8131_APS_TIMER_SLICE,
                            g8131ApsPktSendTimerCallBack,
                            &g8131Cfg->pgData[grpId]);
  if (NULL == tmNode)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] create cycle aps timer grpId %d failed.", grpId);
  }

  /*Record the newReq*/
  g8131Cfg->pgData[grpId].lastSReq = newReq;
  if (G8131_RR_X_WTR_NORMAL == newReq)
  {
    g8131Cfg->pgData[grpId].wtrCntValue = g8131Cfg->pgData[grpId].wtrTime / G8131_APS_INTERVAL + 1;
    G8131_TRACE(G8131_DEBUG_SM, "grp %d wtr timer created", grpId);
  }
  g8131Cfg->pgData[grpId].apsSendTmNode = tmNode;

  return OFDPA_E_NONE;
}

static OFDPA_ERROR_t g8131ApsPktSendForSwitch(uint32_t grpId,
                                              g8131SMRequest_t newReq)
{
  g8131_timer_list_t *tmNode;
  uint32_t cnt = G8131_APS_SWITCH_PACKET_COUNT;

  G8131_PG_CHECK(grpId);
  G8131_PG_REQUEST_CHECK(newReq);

  /*Delete the previous APS packet timer*/
  g8131TimerDelete(g8131Cfg->pgData[grpId].apsSendTmNode);
  g8131Cfg->pgData[grpId].apsSendTmNode = NULL;

  /*Send the APS packet to peer side*/
  for (; cnt > 0; cnt--)
  {
    g8131PduXmitRequest(grpId, newReq);
  }

  /*Start timer for APS packets continue sending*/
  tmNode = g8131TimerCreate(G8131_TIMER_TYPE_APS_CYCLE_INTERVAL,
                            G8131_APS_TIMER_SLICE / G8131_APS_TIMER_SLICE, /*1s*/
                            g8131ApsPktSendTimerCallBack,
                            &g8131Cfg->pgData[grpId]);
  if (NULL == tmNode)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] create cycle aps timer grpId %d failed.", grpId);
  }

  /*Record the newReq*/
  g8131Cfg->pgData[grpId].lastSReq      = newReq;
  g8131Cfg->pgData[grpId].apsSendTmNode = tmNode;

  return OFDPA_E_NONE;
}

static void g8131AlarmCMTimerCallBack(void *handle)
{
  pgOperData_t *pgNode = (pgOperData_t *)handle;

  if (!pgNode)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] AlarmCM timer callback for null protection group elment");
    return;
  }

  if ((pgNode->grpId < OFDPA_G8131_PG_MIN_ID) || (pgNode->grpId > OFDPA_G8131_PG_MAX_ID))
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] AlarmCM timer callback for invalid group %d", pgNode->grpId);
    return;
  }

  G8131_TRACE(G8131_DEBUG_SM, "grp %d [0x%x 0x%x 0x%x] CM disappeared.",
              pgNode->grpId, pgNode->livenessPortIdWorking, pgNode->livenessPortIdProtection, pgNode->livenessPortIdActive);

  g8131TimerDelete(g8131Cfg->pgData[pgNode->grpId].alarmCMTmNode);
  g8131Cfg->pgData[pgNode->grpId].alarmCMTmNode = NULL;
  g8131Cfg->pgData[pgNode->grpId].dfop_cm = OFDPA_FALSE;

  return;
}

static void g8131AlarmNRTimerCallBack(void *handle)
{
  pgOperData_t *pgNode = (pgOperData_t *)handle;

  if (!pgNode)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] AlarmNR timer callback for null protection group elment");
    return;
  }

  if ((pgNode->grpId < OFDPA_G8131_PG_MIN_ID) || (pgNode->grpId > OFDPA_G8131_PG_MAX_ID))
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] AlarmNR timer callback for invalid group %d", pgNode->grpId);
    return;
  }

  G8131_TRACE(G8131_DEBUG_SM, "grp %d [0x%x 0x%x 0x%x] NR occur", pgNode->grpId,
              pgNode->livenessPortIdWorking, pgNode->livenessPortIdProtection, pgNode->livenessPortIdActive);

  if (NULL != g8131Cfg->pgData[pgNode->grpId].alarmNRTmNode)
  {
    g8131TimerDelete(g8131Cfg->pgData[pgNode->grpId].alarmNRTmNode);
    g8131Cfg->pgData[pgNode->grpId].alarmNRTmNode = NULL;
  }
  else
  {
    G8131_TRACE(G8131_DEBUG_SM,
                "[#ERROR#] Alarm NR time out but timerNode is NULL");
  }

  g8131Cfg->pgData[pgNode->grpId].dfop_nr = OFDPA_TRUE;

  return;
}

static void g8131AlarmTOTimerCallBack(void *handle)
{
  OFDPA_ERROR_t status;
  pgOperData_t *pgNode = (pgOperData_t *)handle;

  if (!pgNode)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] AlarmTO timer callback for null protection group elment");
    return;
  }

  if ((pgNode->grpId < OFDPA_G8131_PG_MIN_ID) || (pgNode->grpId > OFDPA_G8131_PG_MAX_ID))
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] AlarmTO timer callback for invalid group %d", pgNode->grpId);
    return;
  }


  if (OFDPA_FALSE == g8131Cfg->pgData[pgNode->grpId].dfop_to)
  {
    G8131_TRACE(G8131_DEBUG_SM, "grp %d [0x%x 0x%x 0x%x] TO occur", pgNode->grpId,
                pgNode->livenessPortIdWorking, pgNode->livenessPortIdProtection, pgNode->livenessPortIdActive);

    g8131Cfg->pgData[pgNode->grpId].dfop_to = OFDPA_TRUE;
  }

  status = g8131TimerUpdate(g8131Cfg->pgData[pgNode->grpId].alarmTOTmNode,
                            &g8131Cfg->pgData[pgNode->grpId],
                            G8131_APS_MAX_INTEVAL / G8131_APS_TIMER_SLICE,
                            g8131AlarmTOTimerCallBack);
  if (OFDPA_E_NONE != status)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] update TO timer grpId %d failed.", pgNode->grpId);
    return;
  }

  return;
}

static OFDPA_ERROR_t g8131AlarmNRHandler(uint32_t grpId,
                                         uint8_t  rcvSignal,
                                         g8131SMRequest_t curReq)
{
  OFDPA_ERROR_t status;
  uint8_t xmtReq, xmtType, xmtReqSig, xmtBrgSig;
  g8131_timer_list_t *pNode;

  status = g8131ConvRequest2Field(curReq, &xmtReq, &xmtType, &xmtReqSig, &xmtBrgSig);
  _G8131_RETURN_CODE_CHECK(status);

  if (rcvSignal != xmtReqSig)
  {
    if (NULL == g8131Cfg->pgData[grpId].alarmNRTmNode)
    {
      pNode = g8131TimerCreate(G8131_TIMER_TYPE_ALARM_NR,
                               G8131_APS_ALARM_NR / G8131_APS_TIMER_SLICE,
                               g8131AlarmNRTimerCallBack,
                               &g8131Cfg->pgData[grpId]);
      if (NULL == pNode)
      {
        G8131_TRACE(G8131_DEBUG_ERROR,
                    "[#ERROR#] create NR timer grpId %d failed.", grpId);
        return OFDPA_E_FAIL;
      }
      g8131Cfg->pgData[grpId].alarmNRTmNode = pNode;
    }
    else
    {
      /*Do nothing, just left it timeout*/
    }
  }
  else
  {
    g8131Cfg->pgData[grpId].dfop_nr = OFDPA_FALSE;
    if (NULL != g8131Cfg->pgData[grpId].alarmNRTmNode)
    {
      g8131TimerDelete(g8131Cfg->pgData[grpId].alarmNRTmNode);
	  g8131Cfg->pgData[grpId].alarmNRTmNode = NULL;
    }
  }

  return OFDPA_E_NONE;
}

static void g8131AlarmClear(uint32_t grpId, g8131SMRequest_t request)
{
  OFDPA_ERROR_t status;

  /*The alarm only works based on the received request from far end.*/
  if ((request < G8131_RR_START) || (request > G8131_RR_END))
  {
    return;
  }

  /*Update ALARM_PM*/
  g8131Cfg->pgData[grpId].dfop_pm = OFDPA_FALSE;
  G8131_TRACE(G8131_DEBUG_SM, "grp %d PM clear", grpId);

  /*Update ALARM_TO*/
  status = g8131TimerUpdate(g8131Cfg->pgData[grpId].alarmTOTmNode,
                            &g8131Cfg->pgData[grpId],
                            G8131_APS_MAX_INTEVAL / G8131_APS_TIMER_SLICE,
                            g8131AlarmTOTimerCallBack);
  if (OFDPA_E_NONE != status)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] update TO timer grpId %d failed.", grpId);
    return;
  }
  g8131Cfg->pgData[grpId].dfop_to = OFDPA_FALSE;
  G8131_TRACE(G8131_DEBUG_SM, "grp %d TO clear", grpId);

  return;
}

static void g8131PgUpdateState(uint32_t        grpId,
                               g8131SMRequest_t request,
                               uint32_t        newState)
{
  g8131Cfg->pgData[grpId].smState = newState;
  if ((request >= G8131_LR_START) && (request <= G8131_LR_END))
  {
    g8131Cfg->pgData[grpId].lastLReq = request;

    /*Record the SF-W/SF-P/SD/SD-W state*/
    switch (request)
    {
      case G8131_LR_C_SF:
        g8131Cfg->pgData[grpId].sfwState = OFDPA_TRUE;
        break;
      case G8131_LR_D_SF_DIS:
        g8131Cfg->pgData[grpId].sfwState = OFDPA_FALSE;
        break;
      case G8131_LR_E_SF_P:
        g8131Cfg->pgData[grpId].sfpState = OFDPA_TRUE;
        break;
      case G8131_LR_F_SF_P_DIS:
        g8131Cfg->pgData[grpId].sfpState = OFDPA_FALSE;
        break;
      case G8131_LR_G_SD:
        g8131Cfg->pgData[grpId].sdwState = OFDPA_TRUE;
        break;
      case G8131_LR_H_SD_DIS:
        g8131Cfg->pgData[grpId].sdwState = OFDPA_FALSE;
        break;
      case G8131_LR_I_SD_P:
        g8131Cfg->pgData[grpId].sdpState = OFDPA_TRUE;
        break;
      case G8131_LR_J_SD_P_DIS:
        g8131Cfg->pgData[grpId].sdpState = OFDPA_FALSE;
        break;
      default:
        break;
    }

	
	g8131Cfg->pgData[grpId].sfwStateFast = g8131Cfg->pgData[grpId].sfwState;
	g8131Cfg->pgData[grpId].sfpStateFast = g8131Cfg->pgData[grpId].sfpState;
	
  }
  else if ((request >= G8131_RR_START) && (request <= G8131_RR_END))
  {
    g8131Cfg->pgData[grpId].lastRReq = request;
  }
}

void g8131UpdateLivenessPortState(pgOperData_t *pgOperData)
{
  OFDPA_PORT_STATE_t workingPortState;
  OFDPA_PORT_STATE_t protectionPortState;

  // OFDB_WRITE_LOCK_TAKE;
  if ((ofdbPortStateGet(pgOperData->livenessPortIdWorking, &workingPortState) == OFDPA_E_NONE) &&
      (ofdbPortStateGet(pgOperData->livenessPortIdProtection, &protectionPortState) == OFDPA_E_NONE))
  {
    if (pgOperData->livenessPortIdActive == pgOperData->livenessPortIdWorking)
    {
      /* new path is working */
      workingPortState    |= OFDPA_PORT_STATE_LIVE;
      protectionPortState &= ~OFDPA_PORT_STATE_LIVE;
    }
    else
    {
      /* new path is protection */
      protectionPortState |= OFDPA_PORT_STATE_LIVE;
      workingPortState    &= ~OFDPA_PORT_STATE_LIVE;
    }

    ofdbPortStateSet(pgOperData->livenessPortIdWorking, workingPortState);
    ofdbPortStateSet(pgOperData->livenessPortIdProtection, protectionPortState);
  }
  else
  {
    G8131_TRACE(G8131_DEBUG_SM, "Failed retrieving port state from OFDB.");
  }
  // OFDB_WRITE_LOCK_GIVE;
}

static void g8131PgReverseActive(uint32_t grpId)
{
  /* toggle the active port */
  if (g8131Cfg->pgData[grpId].livenessPortIdActive == g8131Cfg->pgData[grpId].livenessPortIdWorking)
  {
    g8131Cfg->pgData[grpId].livenessPortIdActive = g8131Cfg->pgData[grpId].livenessPortIdProtection;
  }
  else
  {
    g8131Cfg->pgData[grpId].livenessPortIdActive = g8131Cfg->pgData[grpId].livenessPortIdWorking;
  }

  /* update liveness port state */
  g8131UpdateLivenessPortState(&g8131Cfg->pgData[grpId]);
}

static OFDPA_ERROR_t g8131PgSwitch(uint32_t grpId)
{
	return 0;
}

OFDPA_ERROR_t g8131ConvRequest2Field(g8131SMRequest_t newRequest,
                                     uint8_t *request,
                                     uint8_t *type,
                                     uint8_t *requestSig,
                                     uint8_t *bridgeSig)
{
  switch (newRequest)
  {
    case G8131_RR_P_LO_NULL:
      *request    = G8131_APS_FIELD_REQUEST_LO;
      *requestSig = G8131_SIGNAL_NULL;
      *bridgeSig  = G8131_SIGNAL_NULL;
      break;

    case G8131_RR_Q_SF_P_NULL:
      *request    = G8131_APS_FIELD_REQUEST_SF_P;
      *requestSig = G8131_SIGNAL_NULL;
      *bridgeSig  = G8131_SIGNAL_NULL;
      break;

    case G8131_RR_R_FS_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_FS;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    case G8131_RR_S_SF_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_SF_W;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    case G8131_RR_T_SD_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_SD;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    case G8131_RR_U_SD_NULL:
      *request    = G8131_APS_FIELD_REQUEST_SD;
      *requestSig = G8131_SIGNAL_NULL;
      *bridgeSig  = G8131_SIGNAL_NULL;
      break;

    case G8131_RR_V_MS_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_MS;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    case G8131_RR_W_MS_NULL:
      *request    = G8131_APS_FIELD_REQUEST_MS;
      *requestSig = G8131_SIGNAL_NULL;
      *bridgeSig  = G8131_SIGNAL_NULL;
      break;

    case G8131_RR_X_WTR_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_WTR;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    case G8131_RR_Y_EXER_NULL:
      *request    = G8131_APS_FIELD_REQUEST_EXER;
      *requestSig = G8131_SIGNAL_NULL;
      *bridgeSig  = G8131_SIGNAL_NULL;
      break;

    case G8131_RR_YY_EXER_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_EXER;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    case G8131_RR_Z_RR_NULL:
      *request    = G8131_APS_FIELD_REQUEST_RR;
      *requestSig = G8131_SIGNAL_NULL;
      *bridgeSig  = G8131_SIGNAL_NULL;
      break;

    case G8131_RR_ZZ_RR_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_RR;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    case G8131_RR_AA_NR_NULL:
      *request    = G8131_APS_FIELD_REQUEST_NR;
      *requestSig = G8131_SIGNAL_NULL;
      *bridgeSig  = G8131_SIGNAL_NULL;
      break;

    case G8131_RR_AB_NR_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_NR;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    case G8131_RR_AC_DNR_NORMAL:
      *request    = G8131_APS_FIELD_REQUEST_DNR;
      *requestSig = G8131_SIGNAL_NORMAL;
      *bridgeSig  = G8131_SIGNAL_NORMAL;
      break;

    default:
      return OFDPA_E_FAIL;

  }

  *type = ((G8131_PROT_TYPE_APS_CHANNEL << 3) |
             (G8131_PROT_TYPE_NO_PERMANENT_BRG << 2) |
             (G8131_PROT_TYPE_BI_DIR_SWITCH << 1) |
             (G8131_PROT_TYPE_REVERT_OPER));

  return OFDPA_E_NONE;

}

OFDPA_ERROR_t g8131ConvField2Request(uint8_t request,
                                     uint8_t type,
                                     uint8_t requestSig,
                                     uint8_t bridgeSig,
                                     g8131SMRequest_t *newRequest)
{
  switch (request)
  {
    case G8131_APS_FIELD_REQUEST_LO:
      *newRequest = G8131_RR_P_LO_NULL;
      break;

    case G8131_APS_FIELD_REQUEST_SF_P:
      *newRequest = G8131_RR_Q_SF_P_NULL;
      break;

    case G8131_APS_FIELD_REQUEST_FS:
      *newRequest = G8131_RR_R_FS_NORMAL;
      break;

    case G8131_APS_FIELD_REQUEST_SF_W:
      *newRequest = G8131_RR_S_SF_NORMAL;
      break;

    case G8131_APS_FIELD_REQUEST_SD:
      if (G8131_SIGNAL_NORMAL == requestSig) *newRequest = G8131_RR_T_SD_NORMAL;
      else *newRequest = G8131_RR_U_SD_NULL;
      break;

    case G8131_APS_FIELD_REQUEST_MS:
      if (G8131_SIGNAL_NORMAL == requestSig) *newRequest = G8131_RR_V_MS_NORMAL;
      else *newRequest = G8131_RR_W_MS_NULL;
      break;

    case G8131_APS_FIELD_REQUEST_WTR:
      *newRequest = G8131_RR_X_WTR_NORMAL;
      break;

    case G8131_APS_FIELD_REQUEST_EXER:
      if (G8131_SIGNAL_NORMAL == requestSig) *newRequest = G8131_RR_YY_EXER_NORMAL;
      else *newRequest = G8131_RR_Y_EXER_NULL;
      break;

    case G8131_APS_FIELD_REQUEST_RR:
      if (G8131_SIGNAL_NORMAL == requestSig) *newRequest = G8131_RR_ZZ_RR_NORMAL;
      else *newRequest = G8131_RR_Z_RR_NULL;
      break;

    case G8131_APS_FIELD_REQUEST_NR:
      if (G8131_SIGNAL_NORMAL == requestSig) *newRequest = G8131_RR_AB_NR_NORMAL;
      else *newRequest = G8131_RR_AA_NR_NULL;
      break;

    case G8131_APS_FIELD_REQUEST_DNR:
      *newRequest = G8131_RR_AC_DNR_NORMAL;
      break;

    default:
      return OFDPA_E_FAIL;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t g8131PgCreateHandler(uint32_t grpId)
{
  g8131_timer_list_t *pNode;

  /*ALARM TO timer*/
  pNode = g8131TimerCreate(G8131_TIMER_TYPE_ALARM_TO,
                           G8131_APS_MAX_INTEVAL / G8131_APS_TIMER_SLICE,
                           g8131AlarmTOTimerCallBack,
                           &g8131Cfg->pgData[grpId]);
  if (NULL == pNode)
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] create TO timer grpId %d failed.", grpId);
    return OFDPA_E_FAIL;
  }
  g8131Cfg->pgData[grpId].alarmTOTmNode = pNode;

  /*Cycle sending APS packets*/
  pNode = g8131TimerCreate(G8131_TIMER_TYPE_APS_CYCLE_INTERVAL,
                           G8131_APS_INTERVAL / G8131_APS_TIMER_SLICE,
                           g8131ApsPktSendTimerCallBack,
                           &g8131Cfg->pgData[grpId]);
  if (NULL == pNode)
  {
    g8131TimerDelete(g8131Cfg->pgData[grpId].alarmTOTmNode);
    g8131Cfg->pgData[grpId].alarmTOTmNode = NULL;
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] create cycle aps timer grpId %d failed.", grpId);
    return OFDPA_E_FAIL;
  }
  g8131Cfg->pgData[grpId].apsSendTmNode = pNode;

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t g8131PgDeleteHandler(uint32_t grpId)
{
  /*Cycle APS timer*/
  if (NULL != g8131Cfg->pgData[grpId].apsSendTmNode)
  {
    g8131TimerDelete(g8131Cfg->pgData[grpId].apsSendTmNode);
    g8131Cfg->pgData[grpId].apsSendTmNode = NULL;
  }
  else
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] Delete group %d but cycle APS timer is NULL", grpId);
  }

  /*Hold-off timer*/
  if (NULL != g8131Cfg->pgData[grpId].holdOffTmNode)
  {
    g8131TimerDelete(g8131Cfg->pgData[grpId].holdOffTmNode);
    g8131Cfg->pgData[grpId].holdOffTmNode = NULL;
  }

  /*Alarm timer CM*/
  if (NULL != g8131Cfg->pgData[grpId].alarmCMTmNode)
  {
    g8131TimerDelete(g8131Cfg->pgData[grpId].alarmCMTmNode);
    g8131Cfg->pgData[grpId].alarmCMTmNode = NULL;
  }

  /*Alarm timer NR*/
  if (NULL != g8131Cfg->pgData[grpId].alarmNRTmNode)
  {
    g8131TimerDelete(g8131Cfg->pgData[grpId].alarmNRTmNode);
    g8131Cfg->pgData[grpId].alarmNRTmNode = NULL;
  }

  /*Alarm timer TO*/
  if (NULL != g8131Cfg->pgData[grpId].alarmTOTmNode)
  {
    g8131TimerDelete(g8131Cfg->pgData[grpId].alarmTOTmNode);
    g8131Cfg->pgData[grpId].alarmTOTmNode = NULL;
  }

  memset(&(g8131Cfg->pgData[grpId]), 0, sizeof(g8131Cfg->pgData[0]));

  return OFDPA_E_NONE;
}

void g8131AlarmCMHandler(uint32_t grpId)
{
  OFDPA_ERROR_t status;
  g8131_timer_list_t *pNode;

  g8131Cfg->pgData[grpId].dfop_cm = OFDPA_TRUE;
  G8131_TRACE(G8131_DEBUG_SM, "grp %d CM occur", grpId);

  if (NULL != g8131Cfg->pgData[grpId].alarmCMTmNode)
  {
    status = g8131TimerUpdate(g8131Cfg->pgData[grpId].alarmCMTmNode,
                              &g8131Cfg->pgData[grpId],
                              G8131_APS_MAX_INTEVAL / G8131_APS_TIMER_SLICE,
                              g8131AlarmCMTimerCallBack);
    if (OFDPA_E_NONE != status)
    {
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "[#ERROR#] update CM timer grpId %d failed.", grpId);
      return;
    }
  }
  else
  {
    pNode = g8131TimerCreate(G8131_TIMER_TYPE_ALARM_CM,
                             G8131_APS_MAX_INTEVAL / G8131_APS_TIMER_SLICE,
                             g8131AlarmCMTimerCallBack,
                             &g8131Cfg->pgData[grpId]);
    if (NULL == pNode)
    {
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "[#ERROR#] create CM timer grpId %d failed.", grpId);
      return;
    }
    g8131Cfg->pgData[grpId].alarmCMTmNode = pNode;
  }
}

void g8131AlarmPMHandler(uint32_t grpId)
{
  g8131Cfg->pgData[grpId].dfop_pm = OFDPA_TRUE;
  G8131_TRACE(G8131_DEBUG_SM, "grp %d PM occur", grpId);
}

OFDPA_ERROR_t g8131StateMachineClassfier(uint32_t grpId,
                                         g8131SMRequest_t request,
                                         uint8_t  signal)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;
  g8131SMAction_t  action;
  g8131SMState_t   newState;
  g8131SMRequest_t newRequest;

  G8131_PG_CHECK(grpId);
  G8131_PG_REQUEST_CHECK(request);

  g8131AlarmClear(grpId, request);

  if (OFDPA_TRUE != g8131StateMachineIsEnabled())
  {
    return OFDPA_E_NONE;
  }

  if (OFDPA_TRUE != g8131HoldOffTimerIsExpired(grpId, request))
  {
    /*If hold off not expired until now, ignore the request.*/
    return OFDPA_E_NONE;
  }



  if (OFDPA_E_NONE != g8131StateMachineQuery(grpId, request, &action, &newRequest, &newState))
  {
    /*Only the valid action/newRequest/newState should be handled continue. Ignore all others.*/
    return OFDPA_E_NONE;
  }


  if (action == G8131_ACTION_SWITCH_Y)
  {
    /*Clear WTR timer*/
    g8131WtrTimerReset(grpId);

    /*Execute the switch operation.*/
    status = g8131PgSwitch(grpId);
    _G8131_RETURN_CODE_CHECK(status);

    if (G8131_REQUEST_IGNORE != newRequest)
    {
      /*Check the NR Alarm for RR event*/
      if ((request >= G8131_RR_START) && (request <= G8131_RR_END)) g8131AlarmNRHandler(grpId, signal, newRequest);

      /*Start send 3*3.3ms APS packets then switch back to 5s New state APS packets*/
      status = g8131ApsPktSendForSwitch(grpId, newRequest);
      _G8131_RETURN_CODE_CHECK(status);

      /*Start hold-off timer*/
      status = g8131HoldOffTimerStart(grpId, request);
      _G8131_RETURN_CODE_CHECK(status);
    }
    else
    {
      /*Check the NR Alarm for RR event*/
      if ((request >= G8131_RR_START) && (request <= G8131_RR_END)) g8131AlarmNRHandler(grpId, signal, g8131Cfg->pgData[grpId].lastSReq);
    }
  }
  else
  {
    if (G8131_REQUEST_IGNORE != newRequest)
    {
      /*Clear WTR timer*/
      g8131WtrTimerReset(grpId);

      /*Check the NR Alarm for RR event*/
      if ((request >= G8131_RR_START) && (request <= G8131_RR_END)) g8131AlarmNRHandler(grpId, signal, newRequest);

      /*Send the new request to peer-side.*/
      status = g8131ApsPktSend(grpId, newRequest);
      _G8131_RETURN_CODE_CHECK(status);

      /*Start hold-off timer*/
      status = g8131HoldOffTimerStart(grpId, request);
      _G8131_RETURN_CODE_CHECK(status);
    }
    else
    {
      /*Check the NR Alarm for RR event*/
      if ((request >= G8131_RR_START) && (request <= G8131_RR_END)) g8131AlarmNRHandler(grpId, signal, g8131Cfg->pgData[grpId].lastSReq);
    }
  }

  /*Update the new state*/
  g8131PgUpdateState(grpId, request, newState);

  return status;
}

/*********************************************************************
* @purpose  Debug function to show the internal data structure information
*
* @returns  void
*
* @comments
*
* @end
 **********************************************************************/
void G8131DebugShowGlbCfg(void)
{
  uint32_t index;
  for (index = 0; index < OFDPA_G8131_PG_MAX + 1; index++)
  {
    if (g8131Cfg->pgData[index].inUse != OFDPA_FALSE)
    {
      printf("PG [W          P          A         ] [WV    PV   ] state LReq RReq SReq SF-W SF-P SD-W SD-P WTR HOLD-FLAG dFOP-CM dFOP-PM dFOP-NR dFOP-TO\r\n");
      printf("%-2d [0x%08x 0x%08x 0x%08x] [%5d %5d] %-5c %-4s %-4s %-4s ",
             g8131Cfg->pgData[index].grpId,
             g8131Cfg->pgData[index].livenessPortIdWorking,
             g8131Cfg->pgData[index].livenessPortIdProtection,
             g8131Cfg->pgData[index].livenessPortIdActive,
             g8131Cfg->pgData[index].primVidWorking,
             g8131Cfg->pgData[index].primVidProtection,
             g8131ConvertState(g8131Cfg->pgData[index].smState),
             g8131ConvertRequest(g8131Cfg->pgData[index].lastLReq),
             g8131ConvertRequest(g8131Cfg->pgData[index].lastRReq),
             g8131ConvertRequest(g8131Cfg->pgData[index].lastSReq));
      printf("%-4d %-4d %-4d %-4d ",
             g8131Cfg->pgData[index].sfwState,
             g8131Cfg->pgData[index].sfpState,
             g8131Cfg->pgData[index].sdwState,
             g8131Cfg->pgData[index].sdpState);
      printf("%-3d %-9d %-7d %-7d %-7d %-7d\r\n",
             g8131Cfg->pgData[index].wtrCntValue,
             g8131Cfg->pgData[index].holdOffBlock,
             g8131Cfg->pgData[index].dfop_cm,
             g8131Cfg->pgData[index].dfop_pm,
             g8131Cfg->pgData[index].dfop_nr,
             g8131Cfg->pgData[index].dfop_to);
      printf("HoldOff %d WtrTime %d Revert %d\r\n",
             g8131Cfg->pgData[index].holdOffTime,
             g8131Cfg->pgData[index].wtrTime,
             g8131Cfg->pgData[index].revertiveMode);

      printf("[apsSendTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].apsSendTmNode);
      printf("[holdOffTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].apsSendTmNode);
      printf("[alarmCMTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].holdOffTmNode);
      printf("[alarmNRTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].alarmNRTmNode);
      printf("[alarmTOTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].alarmTOTmNode);
    }
  }
}



/*********************************************************************
* @purpose  Debug function to show the internal data structure information
*
* @returns  uint32_t index
*
* @comments
*
* @end
 **********************************************************************/
void G8131DebugShowMlpGlbCfg(uint32_t index)
{
  
  if(index < OFDPA_G8131_PG_MAX + 1)
  {
    if (g8131Cfg->pgData[index].inUse != OFDPA_FALSE)
    {
      printf("PG [W          P          A         ] [WV    PV   ] state LReq RReq SReq SF-W SF-P SD-W SD-P WTR HOLD-FLAG dFOP-CM dFOP-PM dFOP-NR dFOP-TO\r\n");
      printf("%-2d [0x%08x 0x%08x 0x%08x] [%5d %5d] %-5c %-4s %-4s %-4s ",
             g8131Cfg->pgData[index].grpId,
             g8131Cfg->pgData[index].livenessPortIdWorking,
             g8131Cfg->pgData[index].livenessPortIdProtection,
             g8131Cfg->pgData[index].livenessPortIdActive,
             g8131Cfg->pgData[index].primVidWorking,
             g8131Cfg->pgData[index].primVidProtection,
             g8131ConvertState(g8131Cfg->pgData[index].smState),
             g8131ConvertRequest(g8131Cfg->pgData[index].lastLReq),
             g8131ConvertRequest(g8131Cfg->pgData[index].lastRReq),
             g8131ConvertRequest(g8131Cfg->pgData[index].lastSReq));
      printf("%-4d %-4d %-4d %-4d ",
             g8131Cfg->pgData[index].sfwState,
             g8131Cfg->pgData[index].sfpState,
             g8131Cfg->pgData[index].sdwState,
             g8131Cfg->pgData[index].sdpState);
      printf("%-3d %-9d %-7d %-7d %-7d %-7d\r\n",
             g8131Cfg->pgData[index].wtrCntValue,
             g8131Cfg->pgData[index].holdOffBlock,
             g8131Cfg->pgData[index].dfop_cm,
             g8131Cfg->pgData[index].dfop_pm,
             g8131Cfg->pgData[index].dfop_nr,
             g8131Cfg->pgData[index].dfop_to);
      printf("HoldOff %d WtrTime %d Revert %d\r\n",
             g8131Cfg->pgData[index].holdOffTime,
             g8131Cfg->pgData[index].wtrTime,
             g8131Cfg->pgData[index].revertiveMode);

      printf("[apsSendTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].apsSendTmNode);
      printf("[holdOffTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].apsSendTmNode);
      printf("[alarmCMTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].holdOffTmNode);
      printf("[alarmNRTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].alarmNRTmNode);
      printf("[alarmTOTmNode]0x%-8p\r\n", g8131Cfg->pgData[index].alarmTOTmNode);
    }
  }
  else
  {
    printf("Error Parameters!\r\n");
  }
}


