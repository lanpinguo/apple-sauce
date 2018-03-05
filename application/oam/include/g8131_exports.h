/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2000-2015
*
**********************************************************************
*
* @filename g8131_exports.h
*
* @purpose  Defines constants and feature definitions that are shared
*           by Management and the application
*
* @component
*
* @comments
*
*
* @Notes
*
* @created
*
* @end
*
 **********************************************************************/

#ifndef __G8131_EXPORTS_H_
#define __G8131_EXPORTS_H_

#define OFDPA_G8131_PG_MIN_ID          1
#define OFDPA_G8131_PG_MAX_ID          128	/*leishenghua modify 20170104, Ôö´óµ½128*/
#define OFDPA_G8131_PG_MAX             (OFDPA_G8131_PG_MAX_ID - OFDPA_G8131_PG_MIN_ID + 1)

#define OFDPA_G8131_HOLD_OFF_TIMER_GRANULARITY 100 /* 100ms */
#define OFDPA_G8131_WTR_TIMER_GRANULARITY     (60*1000) /* 1 minute */

#define OFDPA_G8131_HOLD_OFF_MIN       0
#define OFDPA_G8131_HOLD_OFF_MAX       100

#define OFDPA_G8131_WTR_MIN            5
#define OFDPA_G8131_WTR_MAX            12

#define OFDPA_G8131_REQUEST_MIN        0
#define OFDPA_G8131_REQUEST_MAX        28

typedef enum
{
  G8131_LINK_STATUS_INVALID       = 0,
  G8131_LINK_STATUS_DOWN          = 1,
  G8131_LINK_STATUS_UP            = 2,
  G8131_LINK_STATUS_MAX           = G8131_LINK_STATUS_UP
} g8131_link_status_t;

/* Debug trace flags */
typedef enum
{
  G8131_DEBUG_NONE        =        0x00,
  G8131_DEBUG_EVENT       =        0x01,
  G8131_DEBUG_CONFIG      =        0x02,
  G8131_DEBUG_SM          =        0x04, /*state machine*/
  G8131_DEBUG_TX_PKT      =        0x08,
  G8131_DEBUG_RX_PKT      =        0x10,
  G8131_DEBUG_ERROR       = 0x80000000
} g8131_trace_flag_t;

#endif /* __G8131_EXPORTS_H_*/
