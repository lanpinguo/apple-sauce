/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename  g8131_api.h
*
* @purpose   Externs for G8131
*
* @component g8131
*
* @comments  none
*
* @create    1-NOV-2013
*
* @end
**********************************************************************/

#ifndef INCLUDE_G8131_API_H
#define INCLUDE_G8131_API_H

typedef enum
{
  G8131_CMN_TYPE_HOLDOFF_VALUE,
  G8131_CMN_TYPE_WTR_VALUE,
  G8131_CMN_TYPE_REVERT_MODE,

  G8131_CMN_TYPE_STATE,
  G8131_CMN_TYPE_LAST_LR,          /*last local request*/
  G8131_CMN_TYPE_LAST_RR,          /*last remote request*/
  G8131_CMN_TYPE_LAST_SR,          /*last send request*/
  G8131_CMN_TYPE_LOCAL_SF_W_EXIST, /*local SF-W exists*/
  G8131_CMN_TYPE_LOCAL_SF_P_EXIST, /*local SF-P exists*/
  G8131_CMN_TYPE_LOCAL_SD_W_EXIST, /*local SD-W exists*/
  G8131_CMN_TYPE_LOCAL_SD_P_EXIST, /*local SD-P exists*/
  G8131_CMN_TYPE_ALARM_CM,         /*alarm dfop-cm*/
  G8131_CMN_TYPE_ALARM_PM,         /*alarm dfop-pm*/
  G8131_CMN_TYPE_ALARM_NR,         /*alarm dfop-nr*/
  G8131_CMN_TYPE_ALARM_TO,         /*alarm dfop-to*/
  G8131_CMN_TYPE_LAST_W_STATUS,    /*working lsp status*/
  G8131_CMN_TYPE_LAST_P_STATUS,    /*standby/protection lsp status*/

} G8131_CMN_TYPE_t;

typedef enum
{
  G8131_GLB_CMN_TYPE_HOLDOFF_VALUE = 1,
  G8131_GLB_CMN_TYPE_WTR_VALUE,
  G8131_GLB_CMN_TYPE_REVERT_MODE,
  G8131_GLB_CMN_TYPE_ENABLE
} G8131_GLB_CMN_t;

typedef enum
{
  G8131_REQUEST_TYPE_MANUAL_LOCK      = 0,
  G8131_REQUEST_TYPE_FORCE_SWITCH     = 1,
  G8131_REQUEST_TYPE_MANUAL_SWITCH_P  = 10,
  G8131_REQUEST_TYPE_MANUAL_SWITCH    = 11,
  G8131_REQUEST_TYPE_CLEAR            = 12
} G8131_REQUEST_t;

OFDPA_ERROR_t g8131PgCreate(uint32_t grpId,
                            uint32_t ffGroupId,
                            uint32_t livenessPortIdWorking,
                            uint32_t livenessPortIdProtection,
                            uint32_t primVidWorking,
                            uint32_t primVidProtection,
                            uint32_t lmepIdWorking,
                            uint32_t lmepIdProtection);

/*********************************************************************
*
* @purpose  Set G8131 Protection group information per-group.
*
* @param    grpId         @b{(input)}
* @param    itemKey       @b{(input)}
* @param    itemValue     @b{(output)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgCommonSet(uint32_t  grpId,
                               uint32_t  itemType,
                               uint32_t  itemValue);

/*********************************************************************
*
* @purpose  Delete G8131 Protection group
*
* @param    grpId        @b{(input)} protection group ID
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments  none
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgDelete(uint32_t grpId);

/*********************************************************************
*
* @purpose  Get G8131 Protection group information per-group.
*
* @param    grpId         @b{(input)}
* @param    itemKey       @b{(input)}
* @param    itemValue     @b{(output)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgCommonGet(uint32_t  grpId,
                               uint32_t  itemType,
                               uint32_t *itemValue);

/*********************************************************************
*
* @purpose  Send request to g8131 Protection group
*
* @param    grpId        @b{(input)}
* @param    request      @b{(input)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments MS/MS-P/CLEAR/FS/LOCK supported
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgRequest(uint32_t grpId,
                             uint32_t request);

/*********************************************************************
*
* @purpose  Set G8131 Protection group global information.
*
* @param    itemKey      @b{(input)}
* @param    itemValue    @b{(input)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgGlbCommonSet(uint32_t itemKey,
                                  uint32_t itemValue);

/*********************************************************************
*
* @purpose  Get G8131 Protection group global information.
*
* @param    itemKey      @b{(input)}
* @param    itemValue    @b{(output)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgGlbCommonGet(uint32_t  itemKey,
                                  uint32_t *itemValue);

/*********************************************************************
* @purpose  G8131 protection group auto switch
*
* @param    oamVlan      @b{(input)}    oamVlan
* @param    oamStatus    @b{(output)}   oamStatus - 0: Down 1: UP
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments OAM module will call this function to report fault information.
*           Presently this is only a boolean that indicates the path associated
*           with oamVlan is down.
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgFaultReport(uint32_t livenessPortId, int oamStatus);

#endif
