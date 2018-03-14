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
OFDPA_ERROR_t dpFlowEntryPrint(ofdpaFlowEntry_t *flow, ofdpaPrettyPrintBuf_t *buf);
OFDPA_ERROR_t dpGroupAdd(ofdpaGroupEntry_t *group);
OFDPA_ERROR_t dpGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *groupBucket);
OFDPA_ERROR_t dpGroupDataUpdate(ofdpaGroupEntry_t *group);
OFDPA_ERROR_t dpGroupStatsGet(uint32_t groupId, ofdpaGroupEntryStats_t *groupStats);
OFDPA_ERROR_t dpGroupBucketValidate(ofdpaGroupBucketEntry_t *groupBucket);
OFDPA_ERROR_t dpRefGroupValidate(uint32_t referredGroupId);
OFDPA_ERROR_t dpGroupNextGet(uint32_t groupId, ofdpaGroupEntry_t *nextGroup);
OFDPA_ERROR_t dpGroupTypeNextGet(uint32_t groupId,
                                   OFDPA_GROUP_ENTRY_TYPE_t groupType,
                                   ofdpaGroupEntry_t *nextGroup);
OFDPA_ERROR_t dpGroupBucketEntryGet(uint32_t groupId, uint32_t bucketIndex,
														 ofdpaGroupBucketEntry_t *groupBucket);
OFDPA_ERROR_t dpGroupBucketEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
																 ofdpaGroupBucketEntry_t *nextGroupBucket);
OFDPA_ERROR_t dpGroupBucketEntryFirstGet(uint32_t groupId,
																					 ofdpaGroupBucketEntry_t *firstGroupBucket);
uint32_t dpGroupTableMaxCountGet(OFDPA_GROUP_ENTRY_TYPE_t groupType);
uint32_t dpGroupTableEntryCountGet(OFDPA_GROUP_ENTRY_TYPE_t groupType);
uint32_t dpGroupBucketTableMaxCountGet(uint32_t groupId);
OFDPA_ERROR_t dpGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex);

uint32_t dpFlowTableSupported(OFDPA_FLOW_TABLE_ID_t tableId);
uint32_t dpFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_t tableId);
uint32_t dpFlowTableMaxCountGet(OFDPA_FLOW_TABLE_ID_t tableId);

uint32_t dpFlowVlanEntryValidate(ofdpaVlanFlowEntry_t *flowData);
OFDPA_ERROR_t dpGroupBucketEntryPrint(ofdpaGroupBucketEntry_t *bucketEntry, ofdpaPrettyPrintBuf_t *buf);





#endif /* INCLUDE_DATAPATH_API_H */


