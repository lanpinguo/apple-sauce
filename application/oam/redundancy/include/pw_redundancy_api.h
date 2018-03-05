/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename  PW_REDUN_api.h
*
* @purpose   Externs for pwRedun
*
* @component pwRedun
*
* @comments  none
*
* @create    1-NOV-2013
*
* @end
**********************************************************************/

#ifndef INCLUDE_PW_REDUNDANCY_API_H
#define INCLUDE_PW_REDUNDANCY_API_H

typedef enum
{
  PW_REDUN_CMN_TYPE_HOLDOFF_VALUE,
  PW_REDUN_CMN_TYPE_WTR_VALUE,
  PW_REDUN_CMN_TYPE_REVERT_MODE,

} PW_REDUN_CMN_TYPE_t;




OFDPA_ERROR_t pwRedunCreate(uint32_t grpId,
                            uint32_t ffGroupId,
                            uint32_t livenessPortIdWorking,
                            uint32_t livenessPortIdProtection,
                            uint32_t lmepIdWorking,
                            uint32_t lmepIdProtection);

/*********************************************************************
*
* @purpose  Set pwRedun Protection group information per-group.
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
OFDPA_ERROR_t pwRedunCommonSet(uint32_t  grpId,
                               uint32_t  itemType,
                               uint32_t  itemValue);

/*********************************************************************
*
* @purpose  Delete pwRedun Protection group
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
OFDPA_ERROR_t pwRedunDelete(uint32_t grpId);

/*********************************************************************
*
* @purpose  Get pwRedun Protection group information per-group.
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
OFDPA_ERROR_t pwRedunCommonGet(uint32_t  grpId,
                               uint32_t  itemType,
                               uint32_t *itemValue);

/*********************************************************************
* @purpose  pwRedun protection group auto switch
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
OFDPA_ERROR_t pwRedunFaultReport(uint32_t livenessPortId, int oamStatus);

#endif
