/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2014-2016
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************
*
* @filename   ind_ofdpa_meter.c
*
* @purpose    OF-DPA Driver for Indigo
*
* @component  OF-DPA
*
* @comments   none
*
* @create     12 Aug 2014
*
* @end
*
**********************************************************************/
#include "indigo/forwarding.h"
#include "ind_ofdpa_util.h"
#include "ind_ofdpa_log.h"

#ifdef OFDPA_FIXUP
indigo_error_t indigo_fwd_meter_add(uint32_t id, uint16_t flag, of_list_meter_band_t *meters)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  /*TODO*/
  return err;
}
indigo_error_t indigo_fwd_meter_modify(uint32_t id, uint16_t flag, of_list_meter_band_t *meters)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  /*TODO*/
  return err;
}
indigo_error_t indigo_fwd_meter_delete(uint32_t id)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  /*TODO*/
  return err;
}
#endif
