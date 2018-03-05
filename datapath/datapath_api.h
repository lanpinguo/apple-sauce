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



uint32_t dpFlowTableSupported(OFDPA_FLOW_TABLE_ID_t tableId);

OFDPA_ERROR_t vlanPipeFlowAdd(ofdpaFlowEntry_t *flow_node);
OFDPA_ERROR_t vlanPipeFlowStatsGet(ofdbVlanFlowTable_node_t *flow_node,ofdpaFlowEntryStats_t *flowStats);
OFDPA_ERROR_t mplsL2PortPipeFlowAdd(ofdpaFlowEntry_t *flow_node);
OFDPA_ERROR_t mplsL2PortPipeFlowStatsGet(ofdbVlanFlowTable_node_t *flow_node,ofdpaFlowEntryStats_t *flowStats);





#endif /* INCLUDE_DATAPATH_API_H */


