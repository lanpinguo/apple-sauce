/*********************************************************************
*
* (C) Copyright Pure Corporation 2017-2019
*
**********************************************************************
*
* @filename     datapath.h
*
* @purpose      Openflow Datapath Definitions
*
* @component    openflow
*
* @comments     none
*
* @create       04/04/2013
*
* @end
*
**********************************************************************/
#ifndef INCLUDE_DATAPATH_API_H
#define INCLUDE_DATAPATH_API_H


OFDPA_ERROR_t dpFlowAdd(ofdpaFlowEntry_t *flow);
OFDPA_ERROR_t dpFlowStatsGet(ofdpaFlowEntry_t *flow, ofdpaFlowEntryStats_t *flowStats);
OFDPA_ERROR_t dpFlowNextGet(ofdpaFlowEntry_t *flow, ofdpaFlowEntry_t *nextFlow);

uint32_t dpFlowTableSupported(OFDPA_FLOW_TABLE_ID_t tableId);
uint32_t dpFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_t tableId);
uint32_t dpFlowTableMaxCountGet(OFDPA_FLOW_TABLE_ID_t tableId);

ofdpaAct_f dpActGetFuncFromType(OFDPA_FLOW_ACTION_TYPE_t t);
uint32_t dpFlowVlanEntryValidate(ofdpaVlanFlowEntry_t *flowData);





#endif /* INCLUDE_DATAPATH_API_H */


