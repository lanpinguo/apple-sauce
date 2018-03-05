/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
* ********************************************************************
*
*  @filename       dot1ag_db.h
*  @purpose        Dot1ag MEP database, LTR database data strcuture
*                  definitions. Access routines prototypes.
*                  These data structures are accessed and modified by
*                  statemachines and managed objects.
*
*  @component      Dot1ag
*  @comments       none
*  @create         02/15/2009
*  @end
*
***********************************************************************/
#ifndef INCLUDE_DOT1AG_DB_H
#define INCLUDE_DOT1AG_DB_H

#include "avl_api.h"
//#include "driver_oam.h"
#include "dot1ag_cnfgr.h"
#include "dot1ag_proto.h"
#include "dot1ag_sm.h"

/* MA Objects */
typedef struct dot1agCfmMAObjKey_s
{
  uint32_t dot1agCfmMdIndex;
  uint32_t dot1agCfmMaIndex;

} dot1agCfmMAObjKey_t;

typedef struct dot1agCfmMAObj_s
{
  dot1agCfmMAObjKey_t           key;
  /* Ptr to provide access to MA net table containing
     data */
  struct dot1agCfmMaNetTable_s *maData;
  /* Must be last field */
  void                         *next;
} dot1agCfmMAObj_t;

typedef struct dot1agCfmMAObjPrimVIDKey_s
{
  uint32_t dot1agCfmMdIndex;
  uint32_t dot1agCfmMaCompPrimaryVlanId;

} dot1agCfmMAObjPrimVIDKey_t;

typedef struct dot1agCfmMAObjPrimVID_s
{
  dot1agCfmMAObjPrimVIDKey_t    key;
  /* Ptr to provide access to MA net table containing
     data */
  struct dot1agCfmMaNetTable_s *maData;
  /* Must be last field */
  void                         *next;
} dot1agCfmMAObjPrimVID_t;


/* MEP Objects */
typedef struct dot1agCfmMepObjKey_s
{
  uint32_t dot1agCfmMdIndex;
  uint32_t dot1agCfmMaIndex;
  uint32_t dot1agCfmMepIdentifier;
  uint32_t dot1agCfmMepIfIndex;

} dot1agCfmMepObjKey_t;

typedef struct dot1agCfmMepObj_s
{
  dot1agCfmMepObjKey_t        key;
  /* Ptr to provide access to mep table containing
     data */
  struct dot1agCfmMepTable_s *mepData;
  /* Must be last field */
  void                       *next;
} dot1agCfmMepObj_t;

/* MEP Database (RMEP Database) */
typedef struct dot1agCfmRMepObjKey_s
{
  uint32_t dot1agCfmMdIndex;
  uint32_t dot1agCfmMaIndex;
  uint32_t dot1agCfmMepIdentifier;
  uint32_t dot1agCfmMepDbRMepIdentifier;

} dot1agCfmRMepObjKey_t;


typedef struct dot1agCfmRMepObj_s
{
  dot1agCfmRMepObjKey_t         key;
  Dot1agCfmRemoteMepState_t     dot1agCfmMepDbRMepState;
  uint32_t                      dot1agCfmMepDbRMepFailedOkTime;
  uint8_t                       dot1agCfmMepDbMacAddress[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL                    dot1agCfmMepDbRdi;
  Dot1agCfmPortStatus_t         dot1agCfmMepDbPortStatusTlv;
  Dot1agCfmInterfaceStatus_t    dot1agCfmMepDbInterfaceStatusTlv;
  uint8_t                       dot1agCfmMepDbChassisIdLen;
  Dot1agLldpChassisIdSubtype_t  dot1agCfmMepDbChassisIdSubtype;
  uint8_t                       dot1agCfmMepDbChassisId[DOT1AG_LLDP_CHASSIS_ID_LEN];
  uint8_t                       dot1agCfmMepDbManAddressDomainLen;
  uint8_t                       dot1agCfmMepDbManAddressDomain[DOT1AG_TLV_FIELD_LEN_MAX_MGMT_ADDR_DOMAIN];
  uint8_t                       dot1agCfmMepDbManAddressLen;
  uint8_t                       dot1agCfmMepDbManAddress[DOT1AG_TLV_FIELD_LEN_MAX_MGMT_ADDR];
  uint32_t                      dot1agCfmMepDbseqNumber;
  /* The state and variable information required to run RMEP SM.*/
  dot1agRmepStates_t            dot1agCfmRmepState;
  dot1agCfmSMRmepsVars_t        dot1agCfmRmepVar;
  OFDPA_BOOL                    hwExist;
  void                         *next;
} dot1agCfmRMepObj_t;

/* CFM Stack table */
typedef struct dot1agCfmStackObjKey_s
{
  uint32_t               dot1agCfmStackifIndex;
  /*leishenghua add 20160920, 
  OFDPA_OAM_MEG_TYPE_G8113_1 ¿‡–Õ ±£¨  dot1agCfmStackVlanIdOrNone = megindex + 10000,
  OFDPA_OAM_MEG_TYPE_ETHERNET£¨  dot1agCfmStackVlanIdOrNone = vlanid*/
  uint16_t               dot1agCfmStackVlanIdOrNone;
  uint8_t                dot1agCfmStackMdLevel;
  Dot1agCfmMpDirection_t dot1agCfmStackDirection;

} dot1agCfmStackObjKey_t;


typedef struct dot1agCfmStackObj_s
{
  dot1agCfmStackObjKey_t        key;

  /* Ptr to provide access to mep table containing
     data. If MHF, then mepData == NULL */
  struct dot1agCfmMepTable_s   *mepData;
  uint8_t                       mpMacAddress[OFDPA_MAC_ADDR_LEN];          /* Only for Ethernet */
  uint8_t                       mipIdTlvData[OFDPA_OAM_ICC_MIP_ID_LENGTH]; /* Only for BHH MIPs */
  uint32_t                      ofdpaLocalMpId;
  OFDPA_BOOL                    operMode; /* Indicates if MP is operational */

  /* State and FM variable information. This is defined here as it is irrespective of
     MIP or MEP and depends on (bridge port, MD, MA) */
  dot1agLBRStates_t             dot1agCfmLbrMpState;
  dot1agCfmSMLbrMpVars_t        dot1agCfmLbrMpVar;

  void               *mepCcmPktDriverBuffer;

  void                         *next;
} dot1agCfmStackObj_t;

typedef enum
{
  DOT1AG_DB_ID_RMEP_TREE = 0,
  DOT1AG_DB_ID_LTR_TREE,
  DOT1AG_DB_ID_MEP_OBJ_TREE,
  DOT1AG_DB_ID_MA_OBJ_TREE,
  DOT1AG_DB_ID_MA_OBJ_PRIM_VID_TREE,
  DOT1AG_DB_ID_MP_OBJ_TREE,
  DOT1AG_DB_ID_SLM_TREE,
  DOT1AG_DB_ID_CLIENT_MP_DB_TREE,
} DOT1AG_DB_ID_t;

OFDPA_ERROR_t  dot1agDBTreeEntryAdd(DOT1AG_DB_ID_t    dot1agDBId, avlTree_t *pTree, void *data);
OFDPA_ERROR_t  dot1agDBTreeEntryDelete(DOT1AG_DB_ID_t dot1agDBId, avlTree_t *pTree, void *key);

dot1agCfmMAObj_t        *dot1agActiveMAGet(uint32_t mdIndex, uint32_t maIndex, uint32_t flag);
dot1agCfmMAObjPrimVID_t *dot1agActivePrimVIDMAGet(uint32_t mdIndex, uint32_t primVID, uint32_t flag);
dot1agCfmMepObj_t       *dot1agActiveMEPGet(uint32_t mdIndex, uint32_t maIndex, uint32_t mepId,
                                            uint32_t intIfNum, uint32_t flag);
dot1agCfmMdTable_t      *dot1agActiveMdGet(uint32_t mdIndex, uint8_t *mdName,
                                           int mdLevel, uint32_t active);
dot1agCfmMdTable_t      *dot1agMdEntryGet(uint32_t mdIndex, int mdLevel);
dot1agCfmRMepObj_t      *dot1agCfmRMepObjGet(uint32_t mdIndex, uint32_t maIndex, uint32_t mepId,
                                             uint32_t rmepId, uint32_t flag);
dot1agCfmLtrObj_t       *dot1agCfmLtrObjGet(uint32_t mdIndex, uint32_t maIndex, uint32_t mepId,
                                            uint32_t ltrSeqNumber, uint32_t ltrReceiveOrder,
                                            uint32_t flag);
dot1agCfmStackObj_t     *dot1agCfmStackObjGet(uint32_t intIfNum, uint16_t primVID,
                                              uint8_t mdLevel, Dot1agCfmMpDirection_t direction,
                                              uint32_t flag);
dot1agCfmStackObj_t     *dot1agCfmMEPExists(uint32_t intIfNum,
                                            uint16_t primVID,
                                            uint8_t mdLevel,
                                            Dot1agCfmMpDirection_t direction);
dot1agCfmSlmObj_t       *dot1agCfmSlmObjGet(uint32_t lmepId,
                                            uint32_t rmepId,
                                            uint32_t testId,
                                            uint32_t flag);
dot1agCfmClientMPDBObj_t *dot1agCfmClientMPDBObjGet(dot1agCfmClientMPDBObjKey_t key, uint32_t flag);
#endif
