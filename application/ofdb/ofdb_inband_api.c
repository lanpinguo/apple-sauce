/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     ofdb_inband_api.c
*
* @purpose      Openflow tables related to port management.
*
* @component    OFDB
*
* @comments     none
*
* @create       04/05/2017
*
* @end
*
**********************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ofdb_api.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "datapath.h"




int ofdbVlanPortConfigValidate(uint32_t vlan, uint32_t port)
{
  /*检查VLAN 和端口*/
  if (vlan > 4094 || vlan <= 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "vlan %d Validate failed.\r\n", vlan);
    return(0);
  }

  return 1;
}


OFDPA_ERROR_t ofdbVlanPortAdd(uint32_t vlan, uint32_t port)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbVlanPortTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.port = port;
  node.vlan = vlan;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanPortTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {
	if(dataPtr->vlan != vlan)
	{
		return OFDPA_E_FULL;
	}
	else
	{
		return OFDPA_E_EXISTS;
	}
  }

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbVlanPortTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanPortTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. port = %d\r\n",
                         port);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /*nothing to do*/
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbVlanPortDelete(uint32_t vlan, uint32_t port)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbVlanPortTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.port = port;
  node.vlan = vlan;


  dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanPortTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {   
	  rc = OFDPA_E_NONE;
      dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbVlanPortTable_tree, dataPtr);
      if (dataPtr == 0)
      {
        /* this should never happen; if it does, something is wrong with the AVL library */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete AVL tree entry just found. port = %d\r\n",
                           port);
        rc = OFDPA_E_INTERNAL;
      }
   
  }

  return rc;
}



OFDPA_ERROR_t ofdbVlanPortGet(uint32_t port, uint32_t *vlan)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbVlanPortTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.port = port;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanPortTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {
	if(vlan != NULL)
	{
    	*vlan = dataPtr->vlan;
	}
    rc = OFDPA_E_NONE;
  }

  return rc;
}


OFDPA_ERROR_t ofdbVlanPortNextGet(uint32_t port, uint32_t *nextport)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbVlanPortTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.port = port;

  if (nextport == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanPortTable_tree, &port, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextport = dataPtr->port;
    rc = OFDPA_E_NONE;
  }

  return rc;
}




int ofdbVlanIpConfigValidate(uint32_t vlan, unsigned char ip[4], unsigned char ipMask[4])
{
  /*检查VLAN 和端口*/
  if (vlan > 4094 || vlan <= 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "vlan %d Validate failed.\r\n", vlan);
    return(0);
  }

  return 1;
}


OFDPA_ERROR_t ofdbVlanIpAdd(uint32_t vlan, unsigned char ip[4], unsigned char ipMask[4])
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbVlanIpTable_node_t node, *dataPtr;
  int i = 0;
  
  memset(&node, 0, sizeof(node));
  node.vlan = vlan;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanIpTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {
	if(memcmp(dataPtr->ip, ip, 4) != 0 || memcmp(dataPtr->mask, ipMask, 4) !=0)
	{
		return OFDPA_E_FULL;
	}
	else
	{
		return OFDPA_E_EXISTS;
	}
  }

  for(i=0; i<4; i++)
  {
	node.ip[i] = ip[i];
	node.mask[i] = ipMask[i];
  }
 

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbVlanIpTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanIpTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. vlan = %d\r\n",
                         vlan);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /*nothing to do*/
	  
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbVlanIpDelete(uint32_t vlan, unsigned char ip[4], unsigned char ipMask[4])
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbVlanIpTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.vlan = vlan;


  dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanIpTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {   
	  rc = OFDPA_E_NONE;
      dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbVlanIpTable_tree, dataPtr);
      if (dataPtr == 0)
      {
        /* this should never happen; if it does, something is wrong with the AVL library */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete AVL tree entry just found. vlan = %d\r\n",
                           vlan);
        rc = OFDPA_E_INTERNAL;
      }
   
  }

  return rc;
}



OFDPA_ERROR_t ofdbVlanIpGet(uint32_t vlan, unsigned char *ip, unsigned char *ipMask)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbVlanIpTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.vlan = vlan;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanIpTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {
  	if(ip != NULL)
  	{
		memcpy(ip, dataPtr->ip, 4);	
  	}
	
  	if(ipMask != NULL)
  	{
		memcpy(ipMask, dataPtr->mask, 4);
  	}	
    rc = OFDPA_E_NONE;
  }

  return rc;
}


OFDPA_ERROR_t ofdbVlanIpNextGet(uint32_t vlan, uint32_t *nextvlan)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbVlanIpTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.vlan = vlan;

  if (nextvlan == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanIpTable_tree, &vlan, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextvlan = dataPtr->vlan;
    rc = OFDPA_E_NONE;
  }

  return rc;
}






