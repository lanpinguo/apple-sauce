/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2016
*
**********************************************************************
*
* @filename     ofdb_meter_api.c
*
* @purpose      Openflow tables related to meter management.
*
* @component    OFDB
*
* @comments     none
*
* @create       07/08/2014
*
* @end
*
**********************************************************************/
#include <string.h>
#include <stdio.h>

#include "ofdb_api.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "datapath.h"
#include "ofdpa_porting.h"

uint32_t ofdbMeterTableEntryCountGet(void)
{
  return ofdbAVLTrees.ofdbMeterTable_tree.count;
}

uint32_t ofdbMeterTableMaxCountGet(void)
{
  return ofdbMeterTableStatus->meterTableMaxCount;
}

OFDPA_ERROR_t ofdbMeterAdd(uint32_t meterId, ofdpaMeterEntry_t *meter)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbMeterTable_node_t meterNode;
  ofdbMeterTable_node_t *dataPtr;

  memset(&meterNode, 0, sizeof(meterNode));

  meterNode.meterId = meterId;
  memcpy(&meterNode.meter, meter, sizeof(meterNode.meter));
  meterNode.stats.meterAddUpTime = dpaUpTimeSeconds();

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMeterTable_tree, &meterNode);

  if (dataPtr == &meterNode)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Meter Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate Entry is found.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Meter Table entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbMeterDelete(uint32_t meterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbMeterTable_node_t *dataPtr;

  dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMeterTable_tree, &meterId);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Meter Table entry not found!\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbMeterGet(uint32_t meterId, ofdpaMeterEntry_t *meter)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbMeterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbMeterTable_tree, &meterId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy (meter, &dataPtr->meter, sizeof (ofdpaMeterEntry_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbMeterNextGet(uint32_t meterId, uint32_t *nextMeterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbMeterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbMeterTable_tree, &meterId, AVL_NEXT);
  if (dataPtr != NULL)
  {
    *nextMeterId = dataPtr->meterId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbMeterStatsGet(uint32_t meterId, ofdpaMeterEntryStats_t *meterStats)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbMeterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbMeterTable_tree, &meterId, AVL_EXACT);

  if (dataPtr != 0)
  {
    meterStats->refCount = dataPtr->stats.refCount;
    meterStats->duration = dpaUpTimeSeconds() - dataPtr->stats.meterAddUpTime;
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

static OFDPA_ERROR_t ofdbTcmMeterEntryValidate(ofdpaMeterParametersTcm_t *tcmParameters)
{
  /* validate color awareness */
  if ((tcmParameters->colorAwareMode != OFDPA_METER_COLOR_BLIND) &&
      (tcmParameters->colorAwareMode != OFDPA_METER_COLOR_AWARE))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Invalid color awareness. colorAwareMode = %d\r\n",
                       tcmParameters->colorAwareMode);
    return OFDPA_E_PARAM;
  }

  /* validate rate unit selector */
  if ((tcmParameters->tcmRateUnit != OFDPA_METER_RATE_KBPS) &&
      (tcmParameters->tcmRateUnit != OFDPA_METER_RATE_PKTPS))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Invalid rate selector. tcmRateUnit = %d\r\n",
                       tcmParameters->tcmRateUnit);
    return OFDPA_E_PARAM;
  }

  /* validation steps specific to TCM mode (single-rate vs. two-rate) */
  switch (tcmParameters->tcmMode)
  {
    case OFDPA_TCM_METER_MODE_TRTCM:
    case OFDPA_TCM_METER_MODE_MOD_TRTCM:
      if ((tcmParameters->redRate == 0) ||
          (tcmParameters->yellowRate == 0))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Error. PIR and CIR must both be greater than zero for two-rate meters. (redrate = %d, yellowRate = %d)\r\n",
                           tcmParameters->redRate,
                           tcmParameters->yellowRate);
        return OFDPA_E_PARAM;
      }

      if (tcmParameters->redRate < tcmParameters->yellowRate)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Error. PIR %d should not be smaller than CIR %d\r\n",
                           tcmParameters->redRate,
                           tcmParameters->yellowRate);
        return OFDPA_E_PARAM;
      }

      if ((tcmParameters->redBurst == 0) ||
          (tcmParameters->yellowBurst == 0))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Error. PBS and CBS must both be greater than zero for two-rate meters. (redBurst = %d, yellowBurst = %d)\r\n",
                           tcmParameters->redBurst,
                           tcmParameters->yellowBurst);
        return OFDPA_E_PARAM;
      }
      break;

    case OFDPA_TCM_METER_MODE_SRTCM:
      if (tcmParameters->yellowRate == 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Error. CIR must be greater than zero for single-rate meters. (yellowRate = %d)\r\n",
                           tcmParameters->yellowRate);
        return OFDPA_E_PARAM;
      }

      if (tcmParameters->redRate != 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Error. PIR must be zero for single-rate meters. (redRate = %d)\r\n",
                           tcmParameters->redRate);
        return OFDPA_E_PARAM;
      }
      break;

    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                        "Meter Mode not valid. Meter Mode = %d\r\n",
                         tcmParameters->tcmMode);
      return OFDPA_E_PARAM;
      break;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbMeterEntryValidate(uint32_t meterId, ofdpaMeterEntry_t *meter)
{
  OFDPA_ERROR_t rc;

  if (meterId == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Meter identifier out of range. (meterId = %d)\r\n",
                       meterId);
    return OFDPA_E_PARAM;
  }

  if (meter == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Null meter passed.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* validation based on meter type */
  switch (meter->meterType)
  {
    case OFDPA_METER_TYPE_TCM:
      rc = ofdbTcmMeterEntryValidate(&meter->u.tcmParameters);
      break;
    default:
      /* unrecognized meter type */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                        "Unrecognized meter type. (meterType = %d)\r\n",
                         meter->meterType);
      rc = OFDPA_E_PARAM;
      break;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbMeterReferenceUpdate(uint32_t meterId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbMeterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbMeterTable_tree, &meterId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                        "Incrementing reference count!\r\n", 0);
      dataPtr->stats.refCount++;
    }
    else
    {
      if (dataPtr->stats.refCount > 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                          "Decrementing reference count!\r\n", 0);
        dataPtr->stats.refCount--;
      }
    }
  }

  return rc;
}


