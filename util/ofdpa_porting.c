/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     ofdpa_porting.c
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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>


#include "ofdpa_porting.h"
#include "ofdpa_util.h"
#include "ofdb_api.h"
#include "g8131_exports.h"



#define VLAN_DEFAULT   1
#define VLAN_MIN       0
#define VLAN_MAX       4095
#define VLAN_RESERVED  4095


static portMapping_t portMapping[] = 
{
	{.portNum = 1 , .ifName = "eth1"},
	{.portNum = 2 , .ifName = "eth2"},
	{.portNum = 3 , .ifName = "eth3"},
	{.portNum = 4 , .ifName = "eth4"},
	
	{.portNum = -1 , .ifName = ""}, /* the portNum of last one must be -1 */
};


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
int dpaVlanDefault(void)
{
  return VLAN_DEFAULT;
}

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
int dpaVlanReserved(void)
{
  return VLAN_RESERVED;
}

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
int dpaVlanFirst(void)
{
  return VLAN_MIN;
}

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
int dpaVlanLast(void)
{
  return VLAN_MAX;
}

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
int dpaVlanMaxCount(void)
{
  return (dpaVlanLast() - dpaVlanFirst() + 1);
}

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
int dpaVlanNext(int vlanId)
{
  if (vlanId >= dpaVlanLast())
  {
    return 0;
  }

  return vlanId + 1;
}

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
int dpaVlanIsValid(unsigned int vlanId)
{
  if ((vlanId >= dpaVlanFirst()) &&
      (vlanId <= dpaVlanLast()))
  {
    return 1;
  }

  return 0;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of entries the hardware supports
*           for a given flow table ID.
*
* @param    tableId @b{(input)} Identifer of the table.
*
* @returns  table size
*
* @end
*
*************************************************************************/
uint32_t dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_t tableId)
{
	return 10;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of entries the hardware supports
*           for the Port table.
*
* @param    none.
*
* @returns  table size
*
* @end
*
*************************************************************************/
uint32_t dpaPortTableEntryCountGet(void)
{
  return 4;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of entries the hardware supports
*           for the Meter table.
*
* @param    none.
*
* @returns  table size
*
* @end
*
*************************************************************************/
uint32_t dpaMeterTableEntryCountGet(void)
{
  return(dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_ACL_POLICY));
}

uint32_t dpaClassBasedCounterTableEntryCountGet(void)
{
	return 10;
}

uint32_t dpaColorBasedCounterTableEntryCountGet(void)
{
	return 10;
}


/**************************************************************************
*
* @purpose  Retrieve the maximum number of entries the hardware supports
*           for Next Hop table.
*
* @returns  table size
*
* @end
*
*************************************************************************/
uint32_t dpaTunnelNextHopTableEntryCountGet(void)
{
  return(10);
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of port-tenant bindings the hardware
*           supports.
*
* @returns  table size
*
* @end
*
*************************************************************************/
uint32_t dpaTunnelPortTenantCountGet(void)
{
  return(10);
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of tenants the hardware supports.
*
* @returns  table size
*
* @end
*
*************************************************************************/
uint32_t dpaTunnelTenantCountGet(void)
{
  return(10);
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of entries the hardware supports
*           for ECMP Next Hop table.
*
* @returns  table size
*
* @end
*
*************************************************************************/
uint32_t dpaTunnelEcmpNextHopGroupTableEntryCountGet(void)
{
  /* 
   * each logical port can have at most one nexthop group, so no need to
   * allocate more nexthop entries than there are ports
   */
  return(dpaPortTableEntryCountGet());
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of members the hardware supports
*           per ECMP Next Hop group entry.
*
* @returns  count
*
* @end
*
*************************************************************************/
uint32_t dpaTunnelEcmpNextHopGroupMemberCountGet(void)
{
  return(10);
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of entries the hardware supports
*           for a given group type.
*
* @param    groupType @b{(input)} group type.
*
* @returns  group size
*
* @end
*
*************************************************************************/
uint32_t dpaGroupMaxSizeGet(OFDPA_GROUP_ENTRY_TYPE_t groupType)
{
	return 10;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of entries the hardware supports
*           for a given group sub type.
*
* @param    groupType @b{(input)} group type.
* @param    subType @b{(input)} group sub type.
*
* @returns  group size
*
* @end
*
*************************************************************************/
uint32_t dpaGroupSubtypeMaxSizeGet(OFDPA_GROUP_ENTRY_TYPE_t groupType, uint32_t subType)
{
	return 5;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of bucket entries supported
*           for a given group ID.
*
* @param    groupType @b{(input)} group type.
*
* @returns  group size
*
* @end
*
*************************************************************************/
uint32_t dpaGroupBucketMaxSizeGet(OFDPA_GROUP_ENTRY_TYPE_t groupType)
{
	return 2;
}

OFDPA_ERROR_t dpaSysMacAddrGet(char *macString)
{
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  int fd;
  struct ifreq ifr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (fd >= 0)
  {
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) != -1)
    {
      /* return result */
      memcpy(macString, ifr.ifr_hwaddr.sa_data, 6);
      rc = OFDPA_E_NONE;
    }
    close(fd);
  }

  return(rc);

}



int dpaPhysicalPortNumNextGet(int portNum, int *nextPortNum)
{
  int i;
  int rc = -1;

  if (NULL != portMapping)
  {
    if (portNum == 0)
    {
      /* First Port */
      *nextPortNum = portMapping[0].portNum;
      rc = 0;
    }
    else
    {
      for (i = 0; portMapping[i].portNum >= 0 ; i++)
      {
        if (portMapping[i].portNum == portNum)
        {
          *nextPortNum = portMapping[i+1].portNum;
          rc = 0;
          break;
        }
      }
    }
  }

  return rc;
}



OFDPA_ERROR_t dpaPlatformPhysicalPortTableInit(void)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  int rc1;
  ofdbPortInfo_t portInfo;
  int port, nextPort;
  uint32_t numQueues;
  int i = 0;
  uint32_t minRate = 10, maxRate = 1000;
  port = 0;

  

  rc1 = dpaPhysicalPortNumNextGet(port, &nextPort);
  while (rc1 != -1)
  {
    memset(&portInfo, 0, sizeof(portInfo));
    dpaSysMacAddrGet(portInfo.hwAddr.addr);

    sprintf(portInfo.name, "port-%d", nextPort);
    
    portInfo.supported = 1;
    
		portInfo.advertised = 1;


    /* port is administratively up by default */
    portInfo.state = 0;

    portInfo.maxSpeed = 1000;

    
		numQueues = 8;

    portInfo.portQueueInfo = (ofdbPortQueueInfo_t *)malloc(sizeof(ofdbPortQueueInfo_t) * numQueues);
    if (portInfo.portQueueInfo == NULL)
    {
      return OFDPA_E_FAIL;
    }
    

    memset(portInfo.portQueueInfo, 0, sizeof(ofdbPortQueueInfo_t) * numQueues);
    for (i = 0; i < numQueues; i++)
    {
      portInfo.portQueueInfo[i].minRate = minRate;
      portInfo.portQueueInfo[i].maxRate = maxRate;
    }

		portInfo.last_chg = time(NULL);/*use first up time to initialise last chang time */

    rc = dpPortAdd(nextPort, &portInfo);

    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                         "\r\nError initializing Port Table, rc = %d.\r\n", rc);
      return rc;
    }

    port = nextPort;

		rc1 = dpaPhysicalPortNumNextGet(port, &nextPort);
  }
  

  return rc;
}

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
OFDPA_ERROR_t dpaCosQueuesMaxGet(uint32_t *numQueues)
{
	*numQueues = 8;
  return OFDPA_E_NONE;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of buckets the hardware supports
*           for a given group sub type.
*
* @param    groupType @b{(input)} group type.
* @param    subType @b{(input)} group sub type.
*
* @returns  group size
*
* @end
*
*************************************************************************/
uint32_t dpaGroupSubtypeBucketMaxSizeGet(OFDPA_GROUP_ENTRY_TYPE_t groupType, uint32_t subType)
{
	return 2;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM MEG table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of OAM MEG entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaOamMegTableEntryCountGet(void)
{
	return 32;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM MEP table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of OAM MEP entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaOamMepTableEntryCountGet(void)
{
	return 32;
}



/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM MIP table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of OAM MIP entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaOamMipTableEntryCountGet(void)
{
	return 32;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM Remote MP table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of OAM Remote MP entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaOamRemoteMpTableEntryCountGet(void)
{
	return 32;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM MLP Group table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaOamMlpGroupTableEntryCountGet(void)
{
  return OFDPA_G8131_PG_MAX_ID;
}



/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM Protection table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaOamProtectionEntryCountGet(void)
{
	return 32;

}


/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM MLP Group table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaPwRedunTableEntryCountGet(void)
{
  return 32;
}


/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM Protection table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaBfdProtectionEntryCountGet(void)
{
  int size;

  size = 32*2;

  return size;
  
}


#if 1
/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM MEP table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of OAM MEP entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaBfdTableEntryCountGet(void)
{
  int size;

  size = 256;
  return size;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM MEP table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of OAM MEP entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaY1564TableEntryCountGet(void)
{
  int size;

  size = 256;
  return size;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of OAM MEP table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of OAM MEP entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaLoopbackTableEntryCountGet(void)
{
  int size;

  size = 256;
  return size;
}



#endif


uint32_t dpaDot1agMaintenanceDomainsMaxGet(void)
{
  return 10;
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of MPLS Set QoS Action table
*           entries the system supports
*
* @param    none
*
* @returns  Maximum number of MPLS Set QoS Action entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaMplsQosActionTableEntryCountGet(void)
{
#if 0
  return driverMplsQosTableSizeGet();
#else
  return(2048);
#endif
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of Drop Status table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of Drop Status entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaDropStatusTableEntryCountGet(void)
{
#if 0
  return driverDropStatusMaxGet();
#else
  return(1);
#endif
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of Drop Status table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of Drop Status entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaDataPlaneCounterTableEntryCountGet(void)
{
#if 0
  return driverDataPlaneStatusMaxGet();
#else
  return(8);
#endif
}

/**************************************************************************
*
* @purpose  Retrieve the maximum number of Remark Action table entries the
*           system supports
*
* @param    none
*
* @returns  Maximum number of Remark Action entries supported.
*
* @end
*
*************************************************************************/
uint32_t dpaRemarkActionTableEntryCountGet(void)
{
#if 0
  return driverRemarkActionMaxGet();
#else
  return(1024);
#endif
}

/**************************************************************************
*
* @purpose  Get the number of ports on the platform
*
* @param    none
*
* @returns  Number of ports
*
* @end
*
*************************************************************************/
int dpaPlatformNumPortsGet(void)
{
    return 2;
}

