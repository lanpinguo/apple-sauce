/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     ofdpa_porting.h
*
* @purpose      Porting functions
*
* @component    OFDPA
*
* @comments     none
*
* @create       04/09/2013
*
* @end
*
**********************************************************************/
#ifndef INCLUDE_OFDPA_PORTING_H
#define INCLUDE_OFDPA_PORTING_H

#include "ofdpa_datatypes.h"

typedef struct port_mapping_s
{
  int         portNum;
	char 				*ifName;
} portMapping_t;



typedef struct dpa_PlatformCustomOperations_s {
  int (*PlatCustomPortNameGet)( int port, char *pName ); /* Get port name */
  int (*PlatCustomPortMacGet)( int port,char *pMac ); /* Get the mac address of specified port */
  int (*PlatCustomPortMacChgUpdate)(int port,char *pMac); /*update external device memory record,such as oam fpga etc.*/

}dpa_PlatformCustomOperations_t;


int dpaPlatCustomOpsRegister(dpa_PlatformCustomOperations_t * ops);


/**************************************************************************
*
* @purpose  Identify the default VLAN (created by default)
*
* @param    none
*
* @returns  0 if no default VLAN, else VLAN ID (1-4095)
*
* @end
*
*************************************************************************/
int dpaVlanDefault(void);

/**************************************************************************
*
* @purpose  Identify the reserved VLAN (created by default)
*
* @param    none
*
* @returns  0 if no default VLAN, else VLAN ID (1-4095)
*
* @end
*
*************************************************************************/
int dpaVlanReserved(void);

/**************************************************************************
*
* @purpose  Identify the first VLAN in the range OFPDA is to use
*
* @param    none
*
* @returns  VLAN ID
*
* @end
*
*************************************************************************/
int dpaVlanFirst(void);

/**************************************************************************
*
* @purpose  Identify the last VLAN in the range OFPDA is to use
*
* @param    none
*
* @returns  VLAN ID
*
* @end
*
*************************************************************************/
int dpaVlanLast(void);

/**************************************************************************
*
* @purpose  Return the number of VLANs for OFPDA to use
*
* @param    none
*
* @returns  max VLAN count
*
* @end
*
*************************************************************************/
int dpaVlanMaxCount(void);

/**************************************************************************
*
* @purpose  Identify the next VLAN in the range OFPDA is to use
*
* @param    last VLAN ID
*
* @returns  next VLAN ID (0 if none)
*
* @end
*
*************************************************************************/
int dpaVlanNext(int vlanId);

/**************************************************************************
*
* @purpose  Validates that a VLAN ID is in range for this system.
*
* @param    vlanId    @b{(input)} VLAN number
*
* @returns  0 if not valid, else 1
*
* @end
*
*************************************************************************/
int dpaVlanIsValid(unsigned int vlanId);

uint32_t dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_t tableId);
uint32_t dpaPortTableEntryCountGet(void);
uint32_t dpaMeterTableEntryCountGet(void);

uint32_t dpaTunnelNextHopTableEntryCountGet(void);
uint32_t dpaTunnelEcmpNextHopGroupTableEntryCountGet(void);
uint32_t dpaTunnelEcmpNextHopGroupMemberCountGet(void);
uint32_t dpaTunnelPortTenantCountGet(void);
uint32_t dpaTunnelTenantCountGet(void);

uint32_t dpaGroupMaxSizeGet(OFDPA_GROUP_ENTRY_TYPE_t groupType);
uint32_t dpaGroupSubtypeMaxSizeGet(OFDPA_GROUP_ENTRY_TYPE_t groupType, uint32_t subType);
uint32_t dpaGroupBucketMaxSizeGet(OFDPA_GROUP_ENTRY_TYPE_t groupId);
uint32_t dpaGroupSubtypeBucketMaxSizeGet(OFDPA_GROUP_ENTRY_TYPE_t groupType, uint32_t subType);

OFDPA_ERROR_t dpaPlatformPhysicalPortTableInit(void);
OFDPA_ERROR_t dpaSysMacAddrGet(char *macString);

uint32_t dpaDot1agMaintenanceDomainsMaxGet(void);

/**************************************************************************
*
* @purpose  Retrieve the maximum number of CoS queues that the
*           hardware supports
*
* @param    none
*
* @returns  Maximum number of CoS queues supported by the hardware.
*
* @end
*
*************************************************************************/
OFDPA_ERROR_t dpaCosQueuesMaxGet(uint32_t *numQueues);

uint32_t dpaOamMegTableEntryCountGet(void);
uint32_t dpaOamMepTableEntryCountGet(void);
uint32_t dpaOamMipTableEntryCountGet(void);
uint32_t dpaOamRemoteMpTableEntryCountGet(void);
uint32_t dpaOamMlpGroupTableEntryCountGet(void);
uint32_t dpaOamProtectionEntryCountGet(void);
uint32_t dpaMplsQosActionTableEntryCountGet(void);
uint32_t dpaDropStatusTableEntryCountGet(void);
uint32_t dpaDataPlaneCounterTableEntryCountGet(void);
uint32_t dpaRemarkActionTableEntryCountGet(void);
int dpaPlatformNumPortsGet(void);

#endif /* INCLUDE_OFDPA_PORTING_H */
