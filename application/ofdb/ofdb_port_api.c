/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     ofdb_port_api.c
*
* @purpose      Openflow tables related to port management.
*
* @component    OFDB
*
* @comments     none
*
* @create       04/25/2013
*
* @end
*
**********************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "ofdb_api.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
#include "datapath.h"
#include "g8131_api.h"

#define DEFAULT_PORT_Q_WEIGHT           1
#define OFDB_COS_COUNT_MAX              8
int ofdbPortIsValid(uint32_t portNum)
{
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != 0) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
    return 1;
  else
    return 0;
}

OFDPA_ERROR_t ofdbPortAdd(uint32_t port, ofdbPortInfo_t *portInfo)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbPortTable_node_t portNode;
  ofdbPortTable_node_t *dataPtr;

  memset(&portNode, 0, sizeof(portNode));

  portNode.portNum = port;
  memcpy(&portNode.portInfo, portInfo, sizeof(portNode.portInfo));

  portNode.port_add_up_time = dpaUpTimeSeconds();
  portNode.flags = OFDB_PORT_ADDED;

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbPortTable_tree, &portNode);

  if (dataPtr == &portNode)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Port Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* duplicate entry found, check if marked deleted */
    if (dataPtr->flags & OFDB_PORT_DELETED)
    {
      /* entry was marked for delete, change flag and update info */
      dataPtr->flags = OFDB_PORT_ADDED;
      dataPtr->port_add_up_time = dpaUpTimeSeconds();
      memcpy(&dataPtr->portInfo, portInfo, sizeof(portNode.portInfo));
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                        "Port entry already exists!\r\n", 0);
      rc = OFDPA_E_EXISTS;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    if (OFDB_PORT_TYPE(port) == OFDPA_PORT_TYPE_LOGICAL_TUNNEL)
    {
      if (portInfo->tunnelPortConfig.type == OFDPA_TUNNEL_PORT_TYPE_ENDPOINT)
      {
        if (portInfo->tunnelPortConfig.configData.endpoint.ecmp)
          ofdbEcmpNextHopGroupReferenceUpdate(portInfo->tunnelPortConfig.configData.endpoint.nextHopId, 1);
        else
          ofdbNextHopReferenceUpdate(portInfo->tunnelPortConfig.configData.endpoint.nextHopId, 1);

        if (portInfo->tunnelPortConfig.tunnelProtocol == OFDPA_TUNNEL_PROTO_VXLAN)
        {
          ofdbGlobalData->vxlanTunnelEndpointCount++;
          ofdbGlobalData->vxlanTerminatorUdpDestPort = portInfo->tunnelPortConfig.configData.endpoint.protocolInfo.vxlan.terminatorUdpDstPort;
          ofdbGlobalData->vxlanUseEntropy = portInfo->tunnelPortConfig.configData.endpoint.protocolInfo.vxlan.useEntropy;
        }
      }
    }

    /* Notify client  */
    datapathEventNotificationSend(NULL);
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortDelete(uint32_t portNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;
  ofdbPortInfo_t *portInfo;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if (dataPtr != 0)
  {
    /* check if already deleted */
    if (!(dataPtr->flags & OFDB_PORT_DELETED))
    {
      /* make sure no tenant entry references are still configured */
      if (dataPtr->portInfo.tunnelPortStatus.tenantCount != 0)
      {
        rc = OFDPA_E_FAIL;
      }
      else
      {
        dataPtr->flags = OFDB_PORT_DELETED;

        portInfo = &dataPtr->portInfo;
        if (OFDB_PORT_TYPE(portNum) == OFDPA_PORT_TYPE_LOGICAL_TUNNEL)
        {
          if (portInfo->tunnelPortConfig.type == OFDPA_TUNNEL_PORT_TYPE_ENDPOINT)
          {
            if (portInfo->tunnelPortConfig.configData.endpoint.ecmp)
              ofdbEcmpNextHopGroupReferenceUpdate(portInfo->tunnelPortConfig.configData.endpoint.nextHopId, 0);
            else
              ofdbNextHopReferenceUpdate(portInfo->tunnelPortConfig.configData.endpoint.nextHopId, 0);

            if (portInfo->tunnelPortConfig.tunnelProtocol == OFDPA_TUNNEL_PROTO_VXLAN)
            {
              if (ofdbGlobalData->vxlanTunnelEndpointCount > 0)
              {
                ofdbGlobalData->vxlanTunnelEndpointCount--;
              }
            }
          }
        }

        /* notify client */
        datapathEventNotificationSend(NULL);

        rc = OFDPA_E_NONE;
      }
    }
    else
    {
      /* entry already marked deleted, return success */
      rc = OFDPA_E_NONE;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortNextGet(uint32_t portNum, uint32_t *nextPortNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbPortTable_node_t *dataPtr;
  uint32_t searchPortNum;

  /* search for next entry that is NOT marked deleted */
  searchPortNum = portNum;

  while (NULL != (dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &searchPortNum, AVL_NEXT)))
  {
    if (dataPtr->flags & OFDB_PORT_DELETED)
    {
      searchPortNum = dataPtr->portNum;
    }
    else
    {
      break;
    }
  }

  /* see if we found an entry */
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (nextPortNum != NULL)
    {
      *nextPortNum = dataPtr->portNum;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortMacGet(uint32_t portNum, ofdpaMacAddr_t *mac)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (mac != NULL)
    {
      memcpy(mac, &dataPtr->portInfo.hwAddr, OFDPA_MAC_ADDR_LEN);
    }
  }

  return rc;
}



OFDPA_ERROR_t ofdbPortInfoSet(uint32_t portNum, ofdbPortInfo_t *portInfo)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (portInfo != NULL)
    {
      memcpy(&dataPtr->portInfo,portInfo ,sizeof(ofdbPortInfo_t));
    }
  }

  return rc;
}


OFDPA_ERROR_t ofdbPortNameGet(uint32_t portNum, ofdpa_buffdesc *name)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (name != NULL)
    {
      strncpy(name->pstart, dataPtr->portInfo.name, name->size);
      name->size = strlen(dataPtr->portInfo.name) + 1;
    }
  }

  return rc;
}


OFDPA_ERROR_t ofdbPortNameSet(uint32_t portNum, ofdpa_buffdesc *name)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (name != NULL)
    {
      strncpy(dataPtr->portInfo.name,name->pstart , 
	  	(name->size > OFDPA_PORT_NAME_STRING_SIZE?OFDPA_PORT_NAME_STRING_SIZE:name->size));
    }
  }

  return rc;
}


/* BEGIN: Added by HuShouqiang, 2016/3/17   问题单号:is00099 */


OFDPA_ERROR_t ofdbPortEnableGet(uint32_t  portNum, int *enable)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;



  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbPortEnableSet(uint32_t  portNum, int enable)
{
	OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
	
	
	
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t ofdbPortDescpGet(uint32_t portNum, ofdpa_buffdesc *descp)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (descp != NULL)
    {
      strncpy(descp->pstart, dataPtr->portInfo.descp, descp->size);
      descp->size = strlen(dataPtr->portInfo.descp) + 1;
    }
  }

  return rc;
}


OFDPA_ERROR_t ofdbPortDescpSet(uint32_t portNum, ofdpa_buffdesc *descp)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (descp != NULL)
    {
      strncpy(dataPtr->portInfo.name,descp->pstart , 
	  	(descp->size > OFDPA_PORT_NAME_STRING_SIZE?OFDPA_PORT_NAME_STRING_SIZE:descp->size));
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortLastChgTSGet(uint32_t portNum, uint32_t *ts)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (ts != NULL)
    {
      	*ts = dataPtr->portInfo.last_chg;
    }
  }

  return rc;
}


/* END:   Added by HuShouqiang, 2016/3/17 */



OFDPA_ERROR_t ofdbPortDurationGet(uint32_t  portNum, uint32_t *duration)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (duration != NULL)
    {
      *duration = dpaUpTimeSeconds() - dataPtr->port_add_up_time;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortStateGet(uint32_t  portNum, OFDPA_PORT_STATE_t *state)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (state != NULL)
    {
      *state = dataPtr->portInfo.state;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortStateSet(uint32_t  portNum, OFDPA_PORT_STATE_t state)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbPortTable_node_t *dataPtr;
  dpaEventMsg_t   eventMsg = {.msgType = DPA_EVENT_NOTIFICATION_MSG };

  
  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if (dataPtr != NULL)
  {
    if (state == dataPtr->portInfo.state)
    {
      /* no change in state */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                        "Port state set called with unchanged data. (portNum = %d [0x%x], state = 0x%x)\r\n",
                         portNum, portNum, state);
    }
    else
    {
      /* something changed, determine what and update OFDB event flags */
      if ((state & OFDPA_PORT_STATE_LINK_DOWN) != (dataPtr->portInfo.state & OFDPA_PORT_STATE_LINK_DOWN)) 
      {
        /* port link state transitioned */
/* BEGIN: Added by Hushouqiang, 2016/7/23   问题单号:P10008 */
        eventMsg.msgType  = DPA_EVENT_PORT_STATUS_MSG;
        eventMsg.msgSubType = (state & OFDPA_PORT_STATE_LINK_DOWN);
        eventMsg.cookie[0] = portNum;
/* END:   Added by Hushouqiang, 2016/7/23 */
        if ((state & OFDPA_PORT_STATE_LINK_DOWN) != 0)
        {
          OFDB_ENTRY_FLAG_SET(dataPtr->flags, OFDB_PORT_LINK_UP);
          OFDB_ENTRY_FLAG_UNSET(dataPtr->flags, OFDB_PORT_LINK_DOWN);
        }
        else
        {
          OFDB_ENTRY_FLAG_SET(dataPtr->flags, OFDB_PORT_LINK_DOWN);
          OFDB_ENTRY_FLAG_UNSET(dataPtr->flags, OFDB_PORT_LINK_UP);
        }
      }

      if ((state & OFDPA_PORT_STATE_BLOCKED) != (dataPtr->portInfo.state & OFDPA_PORT_STATE_BLOCKED)) 
      {
        /* port liveness transitioned */
        if ((state & OFDPA_PORT_STATE_BLOCKED) != 0)
        {
          OFDB_ENTRY_FLAG_SET(dataPtr->flags, OFDB_PORT_BLOCKED);
          OFDB_ENTRY_FLAG_UNSET(dataPtr->flags, OFDB_PORT_NOT_BLOCKED);
        }
        else
        {
          OFDB_ENTRY_FLAG_SET(dataPtr->flags, OFDB_PORT_NOT_BLOCKED);
          OFDB_ENTRY_FLAG_UNSET(dataPtr->flags, OFDB_PORT_BLOCKED);
        }
      }

      if ((state & OFDPA_PORT_STATE_LIVE) != (dataPtr->portInfo.state & OFDPA_PORT_STATE_LIVE)) 
      {
        /* port liveness transitioned */
        if ((state & OFDPA_PORT_STATE_LIVE) != 0)
        {
          OFDB_ENTRY_FLAG_SET(dataPtr->flags, OFDB_PORT_LIVE);
          OFDB_ENTRY_FLAG_UNSET(dataPtr->flags, OFDB_PORT_NOT_LIVE);
        }
        else
        {
          OFDB_ENTRY_FLAG_SET(dataPtr->flags, OFDB_PORT_NOT_LIVE);
          OFDB_ENTRY_FLAG_UNSET(dataPtr->flags, OFDB_PORT_LIVE);
        }
      }

      dataPtr->portInfo.state = state;
	  
	  /* BEGIN: Added by HuShouqiang, 2016/3/17   问题单号:P10001 */
		dataPtr->portInfo.last_chg = time(NULL);
	  /* END:   Added by HuShouqiang, 2016/3/17 */


	  /* Notify client. */
      datapathEventNotificationSend(&eventMsg);
    }

    rc = OFDPA_E_NONE;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                      "Port table entry not found. (portNum = %d [0x%x], state = 0x%x)\r\n",
                       portNum, portNum, state);
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortConfigSet(uint32_t portNum, OFDPA_PORT_CONFIG_t config)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbPortTable_node_t *dataPtr;
  OFDPA_PORT_TYPE_t portType;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    dataPtr->portInfo.config = config;

    /* handle any updates to port state due to config */
    portType = OFDB_PORT_TYPE(portNum);
    switch (portType)
    {
#if 0 /* need to determine if admin config can influence liveness */
      case OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT:
        if ((config & OFDPA_PORT_CONFIG_DOWN) == 0)
        {
          /* port is adminstratively enabled */
          if ((dataPtr->portInfo.state & OFDPA_PORT_STATE_LIVE) != 0)
          {
            /* unset liveness status */
            ofdbPortStateSet(portNum, (dataPtr->portInfo.state & ~OFDPA_PORT_STATE_LIVE));
          }
        }
        else
        {
          /* port is adminstratively disabled */
          if ((dataPtr->portInfo.state & OFDPA_PORT_STATE_LIVE) == 0)
          {
            /* set liveness status */
            ofdbPortStateSet(portNum, (dataPtr->portInfo.state | OFDPA_PORT_STATE_LIVE));
          }
        }
#endif
        break;

      default:
        /* no updates needed for port types not handled above, continue */
        break;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortConfigGet(uint32_t portNum, OFDPA_PORT_CONFIG_t  *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (config != NULL)
    {
      *config = dataPtr->portInfo.config;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortAdvertiseFeatureSet(uint32_t portNum, OFDPA_PORT_FEATURE_t advertise)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    dataPtr->portInfo.advertised = advertise;
  }

  return rc;
}


OFDPA_ERROR_t ofdbPortMaxSpeedGet(uint32_t portNum, uint32_t  *maxSpeed)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (maxSpeed != NULL)
    {
      *maxSpeed = dataPtr->portInfo.maxSpeed;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortEventNextGet(ofdpaPortEvent_t *eventData)
{
  OFDPA_ERROR_t rc;
  ofdbPortTable_node_t *dataPtr;
  int portFound;
  uint32_t scanPortNum;

  if (eventData == NULL)
  {
    return OFDPA_E_PARAM;
  }

  portFound = 0;
  scanPortNum = eventData->portNum;

  /* find port table entry that has been marked for an event */
  while ((!portFound) &&
         ((dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &scanPortNum, AVL_NEXT)) != NULL))
  {
    if (dataPtr->flags != 0)
    {
      eventData->portNum = dataPtr->portNum;
      eventData->eventMask = 0;

      if (dataPtr->flags & OFDB_PORT_ADDED)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_CREATE;
      }
      else if (dataPtr->flags & OFDB_PORT_DELETED)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_DELETE;
      }

      if (dataPtr->flags & OFDB_ENTRY_PORT_STATE_EVENT_FLAGS)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_STATE;
        eventData->state = dataPtr->portInfo.state;
      }

      if (dataPtr->flags & OFDB_PORT_DELETED)
      {
        /* remove deleted port node now that its deletion has been reported */
        if (dataPtr->portInfo.portQueueInfo != NULL)
        {
          free(dataPtr->portInfo.portQueueInfo);
        }
        avlDeleteEntry(&ofdbAVLTrees.ofdbPortTable_tree, dataPtr);
      }
      else
      {
        /* clear the event flags now that they have been reported */
        dataPtr->flags = 0;
      }

      portFound = 1;
    }
    else
    {
      scanPortNum = dataPtr->portNum;
    }
  }

  if (portFound)
  {
    rc = OFDPA_E_NONE;
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbPortEventGet(ofdpaPortEvent_t *eventData)
{
  OFDPA_ERROR_t rc;
  ofdbPortTable_node_t *dataPtr;
  int portFound;
  uint32_t scanPortNum;

  if (eventData == NULL)
  {
    return OFDPA_E_PARAM;
  }

  portFound = 0;
  scanPortNum = eventData->portNum;

  /* find port table entry that has been marked for an event */
  if ((dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &scanPortNum, AVL_EXACT)) != NULL)
  {
    if (dataPtr->flags != 0)
    {
      eventData->portNum = dataPtr->portNum;
      eventData->eventMask = 0;

      if (dataPtr->flags & OFDB_PORT_ADDED)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_CREATE;
      }
      else if (dataPtr->flags & OFDB_PORT_DELETED)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_DELETE;
      }

      if (dataPtr->flags & OFDB_ENTRY_PORT_STATE_EVENT_FLAGS)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_STATE;
        eventData->state = dataPtr->portInfo.state;
      }

      if (dataPtr->flags & OFDB_PORT_DELETED)
      {
        /* remove deleted port node now that its deletion has been reported */
        if (dataPtr->portInfo.portQueueInfo != NULL)
        {
          free(dataPtr->portInfo.portQueueInfo);
        }
        avlDeleteEntry(&ofdbAVLTrees.ofdbPortTable_tree, dataPtr);
      }
      else
      {
        /* clear the event flags now that they have been reported */
        dataPtr->flags = 0;
      }

      portFound = 1;
    }
  }

  if (portFound)
  {
    rc = OFDPA_E_NONE;
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return(rc);
}


/* BEGIN: Added by Hushouqiang, 2016/3/22   问题单号:P10002 */

OFDPA_ERROR_t ofdbCosQueueAdd(ofdbCosQueue_node_t *queue)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbCosQueue_node_t queueNode;
  ofdbCosQueue_node_t *dataPtr;


  memcpy(&queueNode, queue, sizeof(queueNode));


  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbCosQueues_tree, &queueNode);

  if (dataPtr == &queueNode)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "CoS Queue Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {

  }

  if (rc == OFDPA_E_NONE)
  {
		dataPtr->maxRate = 0;
		dataPtr->minRate = 0;

    /* Notify client  */
    datapathEventNotificationSend(NULL);
  }

  return rc;
}

/* END:   Added by Hushouqiang, 2016/3/22 */
OFDPA_ERROR_t ofdbPortQueueMinMaxRateSet(uint32_t portNum, uint32_t queueId, uint32_t minrate, uint32_t maxrate)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      (dataPtr->portInfo.portQueueInfo != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    dataPtr->portInfo.portQueueInfo[queueId].minRate = minrate;
    dataPtr->portInfo.portQueueInfo[queueId].maxRate = maxrate;
  }

  return rc;
}


OFDPA_ERROR_t ofdbPortQueueMinMaxRateGet(uint32_t portNum, uint32_t queueId, uint32_t *minrate, uint32_t *maxrate)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      (dataPtr->portInfo.portQueueInfo != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (minrate != NULL)
    {
      *minrate = dataPtr->portInfo.portQueueInfo[queueId].minRate;
    }
    if (maxrate != NULL)
    {
      *maxrate = dataPtr->portInfo.portQueueInfo[queueId].maxRate;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortQueueIdIsValid(uint32_t portNum, uint32_t queueId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint32_t numQueues;

  rc = dpaCosQueuesMaxGet(&numQueues);
  if (OFDPA_E_NONE == rc)
  {
    if (queueId >= numQueues)
    {
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortGet(uint32_t portNum, ofdbPortInfo_t *portInfo, OFDB_ENTRY_FLAG_t *flags)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  /* see if we found an entry */
  if (dataPtr != NULL)
  {
    if (portInfo != NULL)
    {
      memcpy(portInfo, &dataPtr->portInfo, sizeof(ofdbPortInfo_t));
    }
    if (flags != NULL)
    {
      *flags = dataPtr->flags;
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbPhysPortInUntaggedAccessFlagUpdate(uint32_t portNum, uint32_t set)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (set != 0)
    {
      dataPtr->portInfo.physicalPortOnUntaggedAccessPort = 1;
    }
    else
    {
      dataPtr->portInfo.physicalPortOnUntaggedAccessPort = 0;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortVxlanTunnelEndpointGlobalConfigGet(uint16_t *termUdpDestPort, uint16_t *useEntropy)
{
  OFDPA_ERROR_t rc;

  if (ofdbGlobalData->vxlanTunnelEndpointCount)
  {
    if (termUdpDestPort != NULL)
    {
      *termUdpDestPort = ofdbGlobalData->vxlanTerminatorUdpDestPort;
    }
    if (useEntropy != NULL)
    {
      *useEntropy = ofdbGlobalData->vxlanUseEntropy;
    }
    rc =  OFDPA_E_NONE;
  }
  else
  {
    rc = OFDPA_E_FAIL;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbTunnelPortReferenceUpdate(uint32_t port, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &port, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->portInfo.tunnelPortStatus.refCount++;
    }
    else
    {
      if (dataPtr->portInfo.tunnelPortStatus.refCount > 0)
      {
        dataPtr->portInfo.tunnelPortStatus.refCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbTunnelPortTenantCountUpdate(uint32_t port, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &port, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->portInfo.tunnelPortStatus.tenantCount++;
    }
    else
    {
      if (dataPtr->portInfo.tunnelPortStatus.tenantCount > 0)
      {
        dataPtr->portInfo.tunnelPortStatus.tenantCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortTenantReferenceUpdate(uint32_t port, uint32_t tunnelId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelPortTenantTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.portNum = port;
  node.tunnelId = tunnelId;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTenantTable_tree, &node, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->portTenantStatus.refCount++;
    }
    else
    {
      if (dataPtr->portTenantStatus.refCount > 0)
      {
        dataPtr->portTenantStatus.refCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortTenantAdd(uint32_t port, uint32_t tunnelId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelPortTenantTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.portNum = port;
  node.tunnelId = tunnelId;

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbPortTenantTable_tree, &node);

  if (dataPtr == &node)
  {
    /* table full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* existing entry */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    ofdbTunnelPortTenantCountUpdate(port, 1);
    ofdbTenantReferenceUpdate(tunnelId, 1);
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortTenantDelete(uint32_t port, uint32_t tunnelId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelPortTenantTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.portNum = port;
  node.tunnelId = tunnelId;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTenantTable_tree, &node, AVL_EXACT);

  if (dataPtr != NULL)
  {
    if (dataPtr->portTenantStatus.refCount == 0)
    {
      avlDeleteEntry(&ofdbAVLTrees.ofdbPortTenantTable_tree, &node);

      ofdbTunnelPortTenantCountUpdate(port, 0);
      ofdbTenantReferenceUpdate(tunnelId, 0);

      rc = OFDPA_E_NONE;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Attempted to delete tunnel port-tenant entry with refCount not 0. port = 0x%x, tunnelId = %d, refCount = %d\r\n",
                         port, tunnelId, dataPtr->portTenantStatus.refCount);
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortTenantGet(uint32_t port, uint32_t tunnelId, ofdpaTunnelPortTenantStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelPortTenantTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.portNum = port;
  node.tunnelId = tunnelId;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTenantTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (status)
    {
      memcpy(status, &dataPtr->portTenantStatus, sizeof(dataPtr->portTenantStatus));
    }
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbPortTenantNextGet(uint32_t port, uint32_t tunnelId, uint32_t *nextTunnelId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelPortTenantTable_node_t node, *dataPtr;

  if (nextTunnelId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&node, 0, sizeof(node));

  node.portNum = port;
  node.tunnelId = tunnelId;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTenantTable_tree, &node, AVL_NEXT);

  if ((dataPtr != 0) && (dataPtr->portNum == port))
  {
    *nextTunnelId = dataPtr->tunnelId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbTenantAdd(uint32_t tunnelId, ofdpaTunnelTenantConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbTunnelTenantTable_node_t node, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&node, 0, sizeof(node));

  node.tunnelId = tunnelId;
  memcpy(&node.tenantConfig, config, sizeof(node.tenantConfig));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbTunnelTenantTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Entry exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  if (config->mcastNextHopId)
  {
    ofdbNextHopReferenceUpdate(config->mcastNextHopId, 1);
  }

  return rc;
}

OFDPA_ERROR_t ofdbTenantDelete(uint32_t tunnelId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelTenantTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelTenantTable_tree, &tunnelId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (dataPtr->tenantStatus.refCount == 0)
    {
      if (dataPtr->tenantConfig.mcastNextHopId)
      {
        ofdbNextHopReferenceUpdate(dataPtr->tenantConfig.mcastNextHopId, 0);
      }

      avlDeleteEntry(&ofdbAVLTrees.ofdbTunnelTenantTable_tree, dataPtr);
      rc = OFDPA_E_NONE;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                        "Attempted to delete Tunnel Tenant entry with refCount not 0. tunnelId = %d, refCount = %d\r\n",
                         tunnelId, dataPtr->tenantStatus.refCount);
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbTenantGet(uint32_t tunnelId, ofdpaTunnelTenantConfig_t *config, ofdpaTunnelTenantStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelTenantTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelTenantTable_tree, &tunnelId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->tenantConfig, sizeof(dataPtr->tenantConfig));
    }
    if (status)
    {
      memcpy(status, &dataPtr->tenantStatus, sizeof(dataPtr->tenantStatus));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbTenantNextGet(uint32_t tunnelId, uint32_t *nextTunnelId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelTenantTable_node_t *dataPtr;

  if (nextTunnelId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelTenantTable_tree, &tunnelId, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextTunnelId = dataPtr->tunnelId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbTenantReferenceUpdate(uint32_t tunnelId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelTenantTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelTenantTable_tree, &tunnelId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->tenantStatus.refCount++;
    }
    else
    {
      if (dataPtr->tenantStatus.refCount > 0)
      {
        dataPtr->tenantStatus.refCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbNextHopAdd(uint32_t nextHopId, ofdpaTunnelNextHopConfig_t *nextHopInfo)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbTunnelNextHopTable_node_t nhNode, *dataPtr;

  if (nextHopInfo == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&nhNode, 0, sizeof(nhNode));

  nhNode.nextHopId = nextHopId;
  memcpy(&nhNode.nhConfig, nextHopInfo, sizeof(nhNode.nhConfig));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbTunnelNextHopTable_tree, &nhNode);

  if (dataPtr == &nhNode)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Tunnel Next Hop Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Tunnel Next Hop entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbNextHopDelete(uint32_t nextHopId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelNextHopTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelNextHopTable_tree, &nextHopId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (dataPtr->nhStatus.refCount == 0)
    {
      avlDeleteEntry(&ofdbAVLTrees.ofdbTunnelNextHopTable_tree, dataPtr);
      rc = OFDPA_E_NONE;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                        "Attempted to delete Tunnel Next Hop entry with refCount not 0. nextHopId = %d, refCount = %d\r\n",
                         nextHopId, dataPtr->nhStatus.refCount);
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbNextHopModify(uint32_t nextHopId, ofdpaTunnelNextHopConfig_t *nextHopInfo)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelNextHopTable_node_t *dataPtr;

  if (nextHopInfo == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelNextHopTable_tree, &nextHopId, AVL_EXACT);

  if (dataPtr != 0)
  {
    memcpy(&dataPtr->nhConfig, nextHopInfo, sizeof(dataPtr->nhConfig));
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbNextHopGet(uint32_t nextHopId, ofdpaTunnelNextHopConfig_t *config, ofdpaTunnelNextHopStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelNextHopTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelNextHopTable_tree, &nextHopId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->nhConfig, sizeof(dataPtr->nhConfig));
    }
    if (status)
    {
      memcpy(status, &dataPtr->nhStatus, sizeof(dataPtr->nhStatus));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbNextHopNextGet(uint32_t nextHopId, uint32_t *nextNextHopId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelNextHopTable_node_t *dataPtr;

  if (nextNextHopId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelNextHopTable_tree, &nextHopId, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextNextHopId = dataPtr->nextHopId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbNextHopReferenceUpdate(uint32_t nextHopId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelNextHopTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelNextHopTable_tree, &nextHopId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->nhStatus.refCount++;
    }
    else
    {
      if (dataPtr->nhStatus.refCount > 0)
      {
        dataPtr->nhStatus.refCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupAdd(uint32_t ecmpNextHopGroupId, ofdpaTunnelEcmpNextHopGroupConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbTunnelEcmpNextHopGroupTable_node_t nhNode, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&nhNode, 0, sizeof(nhNode));

  nhNode.ecmpNextHopGroupId = ecmpNextHopGroupId;
  memcpy(&nhNode.nhgConfig, config, sizeof(nhNode.nhgConfig));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupTable_tree, &nhNode);

  if (dataPtr == &nhNode)
  {
    /* table full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Tunnel ECMP Next Hop Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* existing entry */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Tunnel ECMP Next Hop entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupDelete(uint32_t ecmpNextHopGroupId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupTable_tree, &ecmpNextHopGroupId, AVL_EXACT);

  if (dataPtr != 0)
  {
    /* entry must not be referred to or have any members */
    if ((dataPtr->nhgStatus.refCount == 0) && (dataPtr->nhgStatus.memberCount == 0))
    {
      avlDeleteEntry(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupTable_tree, dataPtr);
      rc = OFDPA_E_NONE;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                        "Attempted to delete Tunnel ECMP Next Hop Group with members or is referred to. (id = %d, refCount = %d, memberCount = %d)\r\n",
                         ecmpNextHopGroupId, dataPtr->nhgStatus.refCount, dataPtr->nhgStatus.memberCount);
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupGet(uint32_t ecmpNextHopGroupId,
                                      ofdpaTunnelEcmpNextHopGroupConfig_t *config,
                                      ofdpaTunnelEcmpNextHopGroupStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupTable_tree, &ecmpNextHopGroupId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->nhgConfig, sizeof(dataPtr->nhgConfig));
    }
    if (status)
    {
      memcpy(status, &dataPtr->nhgStatus, sizeof(dataPtr->nhgStatus));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupNextGet(uint32_t ecmpNextHopGroupId, uint32_t *nextEcmpNextHopGroupId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupTable_node_t *dataPtr;

  if (nextEcmpNextHopGroupId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupTable_tree, &ecmpNextHopGroupId, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextEcmpNextHopGroupId = dataPtr->ecmpNextHopGroupId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupReferenceUpdate(uint32_t ecmpNextHopGroupId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupTable_tree, &ecmpNextHopGroupId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->nhgStatus.refCount++;
    }
    else
    {
      if (dataPtr->nhgStatus.refCount > 0)
      {
        dataPtr->nhgStatus.refCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberCountUpdate(uint32_t ecmpNextHopGroupId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupTable_tree, &ecmpNextHopGroupId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->nhgStatus.memberCount++;
    }
    else
    {
      if (dataPtr->nhgStatus.memberCount > 0)
      {
        dataPtr->nhgStatus.memberCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberAdd(uint32_t ecmpNextHopGroupId, uint32_t nextHopId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupMemberTable_node_t memberNode, *dataPtr;

  memset(&memberNode, 0, sizeof(memberNode));

  memberNode.ecmpNextHopGroupId = ecmpNextHopGroupId;
  memberNode.nextHopId = nextHopId;

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupMemberTable_tree, &memberNode);

  if (dataPtr == &memberNode)
  {
    /* table full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* existing entry */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* entry added, update counts */
    ofdbNextHopReferenceUpdate(nextHopId, 1);
    ofdbEcmpNextHopGroupMemberCountUpdate(ecmpNextHopGroupId, 1);

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberDelete(uint32_t ecmpNextHopGroupId, uint32_t nextHopId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupMemberTable_node_t memberNode, *dataPtr;

  memset(&memberNode, 0, sizeof(memberNode));

  memberNode.ecmpNextHopGroupId = ecmpNextHopGroupId;
  memberNode.nextHopId = nextHopId;

  dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupMemberTable_tree, &memberNode);

  if (dataPtr != NULL)
  {
    /* entry removed, update counts */
    ofdbNextHopReferenceUpdate(nextHopId, 0);
    ofdbEcmpNextHopGroupMemberCountUpdate(ecmpNextHopGroupId, 0);

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberGet(uint32_t ecmpNextHopGroupId, uint32_t nextHopId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupMemberTable_node_t memberNode, *dataPtr;

  memset(&memberNode, 0, sizeof(memberNode));

  memberNode.ecmpNextHopGroupId = ecmpNextHopGroupId;
  memberNode.nextHopId = nextHopId;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupMemberTable_tree, &memberNode, AVL_EXACT);

  if ((dataPtr != 0) && (dataPtr->ecmpNextHopGroupId == ecmpNextHopGroupId))
  {
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberNextGet(uint32_t ecmpNextHopGroupId, uint32_t nextHopId, uint32_t *nextNextHopId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbTunnelEcmpNextHopGroupMemberTable_node_t memberNode, *dataPtr;

  if (nextNextHopId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&memberNode, 0, sizeof(memberNode));

  memberNode.ecmpNextHopGroupId = ecmpNextHopGroupId;
  memberNode.nextHopId = nextHopId;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupMemberTable_tree, &memberNode, AVL_NEXT);

  if ((dataPtr != 0) && (dataPtr->ecmpNextHopGroupId == ecmpNextHopGroupId))
  {
    *nextNextHopId = dataPtr->nextHopId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}



/*****************************************************************************
 Prototype    : ofdbPortQueueSchedSet
 Description  : This routine set the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                uint32_t mode       
                int32_t *weights  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdbPortQueueSchedSet(uint32_t port, int32_t mode,int32_t *weights)
{ 
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;
  int i ;
  uint32_t numQueues;



  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &port, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if(mode != -1)
    {
      dataPtr->portInfo.schedMode = mode;
    }

  
    if((dataPtr->portInfo.schedMode & 
      (OFDB_QOS_QUEUE_SCHED_MODE_WRR | OFDB_QOS_QUEUE_SCHED_MODE_WDRR | OFDB_QOS_QUEUE_SCHED_MODE_WFQ)) != 0 )
    {
      
      rc = dpaCosQueuesMaxGet(&numQueues);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                           "Could not get max queues!!!\r\n", 0);
        return rc;
      }

      
      for(i = 0 ; i < numQueues ; i ++ )
      {
        if(weights != NULL)  
        {
          if(weights[i] == 0)
          {
            if((dataPtr->portInfo.schedMode & OFDB_QOS_QUEUE_SCHED_MODE_SP) == 0 )
            {
              /* NOT "sp + wxx" mode use the default value*/
              weights[i] = DEFAULT_PORT_Q_WEIGHT;
            }
          }
          /* record in database*/
          dataPtr->portInfo.portQueueInfo[i].weight = weights[i];
        }
        else
        {
          /* when switch scheduler mode, then set weight to default value */
          
          if((dataPtr->portInfo.schedMode & OFDB_QOS_QUEUE_SCHED_MODE_SP) == 0 )
          {
            /* NOT "sp + wxx" mode use the default value*/
            
            dataPtr->portInfo.portQueueInfo[i].weight = DEFAULT_PORT_Q_WEIGHT;
          }
          else
          {
            dataPtr->portInfo.portQueueInfo[i].weight = 0;
          }
        }        
      }

    }

    rc = OFDPA_E_NONE;
  }

  return rc;

}




/*****************************************************************************
 Prototype    : ofdbPortQueueWeightModify
 Description  : This routine set the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                unt32_t queueId      
                int32_t weight  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdbPortQueueWeightModify(uint32_t port,uint32_t queueId, int32_t weight)
{ 
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;
  int i ;
  uint32_t numQueues;
  int32_t mode;
  uint32_t weights[OFDB_COS_COUNT_MAX] = {0};

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &port, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    mode = dataPtr->portInfo.schedMode;
    if((mode & (OFDB_QOS_QUEUE_SCHED_MODE_WRR | OFDB_QOS_QUEUE_SCHED_MODE_WFQ)) != 0 )
    {
      
      rc = dpaCosQueuesMaxGet(&numQueues);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                           "Could not get max queues!!!\r\n", 0);
        return rc;
      }

      
      for(i = 0 ; i < numQueues ; i ++ )
      {
        /* Read out the record value*/
        weights[i] = dataPtr->portInfo.portQueueInfo[i].weight;
      }
      if(queueId < numQueues)
      {
        /*Modify record*/
        dataPtr->portInfo.portQueueInfo[queueId].weight = weight;
        
        weights[queueId] = weight;
      }
      else
      {
        return OFDPA_E_PARAM;
      }
    }
    else
    {
      return OFDPA_E_UNAVAIL;
    }
    rc = OFDPA_E_NONE;
  }

  return rc;

}

/*****************************************************************************
 Prototype    : ofdbPortQueueSchedGet
 Description  : Get the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                uint32_t *mode       
                int32_t *weights  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdbPortQueueSchedGet(uint32_t port, int32_t *mode,int32_t *weights)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbPortTable_node_t *dataPtr;
  int i ;
  uint32_t numQueues;

  #if 0
  rc =  driverPortQueueSchedGet(port, mode,  weights);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                       "Could not get driver queues sched info!!!\r\n", 0);
    return rc;
  }
  #endif
  
  dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortTable_tree, &port, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & OFDB_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if(mode != NULL)
    {
      *mode = dataPtr->portInfo.schedMode;
    }
    
    if((dataPtr->portInfo.schedMode & 
      (OFDB_QOS_QUEUE_SCHED_MODE_WRR | OFDB_QOS_QUEUE_SCHED_MODE_WFQ)) != 0 )
    {
      
      rc = dpaCosQueuesMaxGet(&numQueues);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                           "Could not get max queues!!!\r\n", 0);
        return rc;
      }

      if(weights != NULL)
      {
        for(i = 0 ; i < numQueues ; i ++ )
        {
          /* get from database*/
          weights[i] = dataPtr->portInfo.portQueueInfo[i].weight;
        }
      } 
     
    }
  }

  return rc;

}



/*****************************************************************************
 Prototype    : ofdbPortDiscardModeSet
 Description  : This routine set the congestion avoidance mode of COS queue on a port.
                
 Input        : uint32_t mode       
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdbPortDiscardModeSet(uint32_t mode)
{
	return OFDPA_E_NONE;
}




/*****************************************************************************
 Prototype    : ofdbPortDiscardModeGet
 Description  : Get the congestion avoidance mode of COS queue on a port.
                
 Input        : uint32_t *mode       
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdbPortDiscardModeGet(uint32_t *mode)
{
	return OFDPA_E_NONE;
}




/*****************************************************************************
 Prototype    : ofdbPortDiscardConfig
 Description  : This routine configure the congestion avoidance parameters
                of COS queue on a port.
 Input        : uint32_t port     
                uint32_t cosq     
                uint32_t color    
                int drop_start  
                int drop_slope  
                int drop_end    
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdbPortDiscardConfig(uint32_t port, uint32_t cosq,uint32_t color,
  int drop_start, int drop_slope,int drop_end)
{
	return OFDPA_E_NONE;
}




/*****************************************************************************
 Prototype    : ofdbPortDiscardConfigGet
 Description  : Get the congestion avoidance parameters
                of COS queue on a port.
 Input        : uint32_t port     
                uint32_t cosq     
                uint32_t color    
                int *drop_start  
                int *drop_slope  
                int *drop_end    
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdbPortDiscardConfigGet(uint32_t port, uint32_t cosq,uint32_t color,
  int *drop_start, int *drop_slope,int *drop_end)
{
  return 0;
}





