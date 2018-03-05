/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2005-2015
*
**********************************************************************
* @filename  g8131_include.h
*
* @purpose   g8131_include file
*
* @component g8131
*
* @comments
*
* @create    1-NOV-2013
*
* @end
*
**********************************************************************/

#ifndef INCLUDE_G8131_INCLUDE_H
#define INCLUDE_G8131_INCLUDE_H

/*
 **********************************************************************
 *                            HEADER FILES
 **********************************************************************
 */
#include "g8131_util.h"
#include "g8131_exports.h"
#include "g8131_sm.h"
#include "g8131_pdu.h"
#include "g8131_api.h"
#include "g8131_ctrl.h"
#include "g8131_debug.h"
#include "g8131_cfg.h"
#include "g8131.h"
#include "ofdpa_util.h"

#define G8131_NULL_PTR_CHECK(ptr) \
do \
{ \
  if (ptr == NULL) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "NULL ptr detected"); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

#define G8131_PG_CHECK(id) \
do \
{ \
  if ((id < OFDPA_G8131_PG_MIN_ID) || (id > OFDPA_G8131_PG_MAX_ID)) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "Grp ID %d out of range (%d-%d)", \
            id, OFDPA_G8131_PG_MIN_ID, OFDPA_G8131_PG_MAX_ID); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

#define G8131_PG_STATE_CHECK(id) \
do \
{ \
  if ((id < G8131_STATE_START) || (id > G8131_STATE_END)) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "Grp State %d out of range (%d-%d)", \
            id, G8131_STATE_START, G8131_STATE_END); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

#define G8131_PG_LOCAL_REQUEST_CHECK(id) \
do \
{ \
  if ((id < G8131_LR_START) || (id > G8131_LR_END)) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "Local request %d out of range (%d-%d)", \
            id, G8131_LR_START, G8131_LR_END); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

#define G8131_PG_REMOTE_REQUEST_CHECK(id) \
do \
{ \
  if ((id < G8131_RR_START) || (id > G8131_RR_END)) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "Remote request %d out of range (%d-%d)", \
            id, G8131_RR_START, G8131_RR_END); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

#define G8131_PG_REQUEST_CHECK(id) \
do \
{ \
  if (!(((id >= G8131_LR_START) && (id <= G8131_LR_END)) || \
        ((id >= G8131_RR_START) && (id <= G8131_RR_END)) || \
         (id == G8131_REQUEST_IGNORE))) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "Request %d out of range (%d-%d)(%d-%d)(%d)", \
            id, G8131_LR_START, G8131_LR_END, G8131_RR_START, G8131_RR_END, G8131_REQUEST_IGNORE); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

#define G8131_PG_BOOL_VALUE_CHECK(id) \
do \
{ \
  if ((id != OFDPA_TRUE) && (id != OFDPA_FALSE)) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "BOOL value %d out of range (%d-%d)", \
            id, OFDPA_TRUE, OFDPA_FALSE); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

/*WTR range is (5-12)minutes*/
#define G8131_PG_WTR_VALUE_MIN (5*60*1000)  /*5  minutes*/
#define G8131_PG_WTR_VALUE_MAX (12*60*1000) /*12 minutes*/
#define G8131_PG_WTR_CHECK(id) \
do \
{ \
  if ((id < G8131_PG_WTR_VALUE_MIN) || (id > G8131_PG_WTR_VALUE_MAX)) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "WTR value %d out of range (%d-%d)", \
            id, G8131_PG_WTR_VALUE_MIN, G8131_PG_WTR_VALUE_MAX); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

/*HOLD-OFF range is (0-10)seconds*/
#define G8131_PG_HOLDOFF_VALUE_MIN (0)
#define G8131_PG_HOLDOFF_VALUE_MAX (10*1000) /*0 - 10 seconds*/
#define G8131_PG_HOLDOFF_CHECK(id) \
do \
{ \
  if ((id < G8131_PG_HOLDOFF_VALUE_MIN) || (id > G8131_PG_HOLDOFF_VALUE_MAX)) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "HoldOff value %d out of range (%d-%d)", \
            id, G8131_PG_HOLDOFF_VALUE_MIN, G8131_PG_HOLDOFF_VALUE_MAX); \
    return OFDPA_E_PARAM; \
  } \
} while(0)

/*
 **********************************************************************
 *                            EXTERNAL DATA STRUCTURES
 **********************************************************************
*/

#endif
