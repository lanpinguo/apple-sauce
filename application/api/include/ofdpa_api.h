/*! \file ofdpa_api.h
 */

/*! @addtogroup GLOFDPAAPI OF-DPA APIs
 *    @{
 */

/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
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
* @filename     ofdpa_api.h
*
* @purpose      OF-DPA API header
*
* @component    OF-DPA
*
* @note         none
*
* @create       03/22/2013
*
* @end
*
**********************************************************************/
#ifndef INCLUDE_OFDPA_API_H
#define INCLUDE_OFDPA_API_H

#include <stdint.h>
#include <sys/time.h>
#include <netinet/in.h>
#include "ofdpa_datatypes.h"

/*
 * All clients must register first.
 */
/*****************************************************************//**
* @brief  Initialize OF-DPA client or user application. For OF-DPA Standalone
*           Application mode, sets up the RPC communication channel for the
*           client. For the statically linked User Application, initializes
*           the system.
*
* @param[in]    clientName     client name
*
* @retval   OFDPA_E_NONE  success
* @retval   OFDPA_E_PARAM error in parameter passed
* @retval   OFDPA_E_FAIL  failure
*
*********************************************************************/
OFDPA_ERROR_t ofdpaClientInitialize(char *clientName);


/*****************************************************************//**
* @brief  Add a controller connection to switch.
*
* @param[in]    cfg      The controller ip and port
* @param[in,out]    of_cxn_id      The connection ID
* @retval   OFDPA_E_UNAVAIL   If the feature is not supported
* @retval   OFDPA_E_NONE      If the feature is supported
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOfagentCxnAdd(ofdpaAgentCxnParam_t * cfg ,uint32_t *of_cxn_id);


/*****************************************************************//**
* @brief  Remove a controller connection from switch.
*
* @param[in]    of_cxn_id      The  connection ID
* @retval   OFDPA_E_UNAVAIL   If the feature is not supported
* @retval   OFDPA_E_NONE      If the feature is supported
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOfagentCxnRemove(uint32_t of_cxn_id);


/*****************************************************************//**
* @brief  Indicates whether the switch supports a particular OF-DPA
*           feature.
*
* @param[in]    feature      The feature identifier
*
* @retval   OFDPA_E_UNAVAIL   If the feature is not supported
* @retval   OFDPA_E_NONE      If the feature is supported
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFeatureSupported(OFDPA_FEATURE_t feature);

/*
 * Debugging and Logging definitions
 */

/*****************************************************************//**
* @brief  Generate a log message using printf formatting
*
* @param[in]    priority     message priority (syslog values)
* @param[in]    fmt          format string
* @param[in]    ...          optional parameters for format string
*
* @returns  number of characters in log message
* @returns  value less than zero if error
*
*********************************************************************/
int ofdpaCltLogPrintf(int priority, char *fmt, ...);

/*****************************************************************//**
* @brief  Generate a log message using a preformatted buffer
*
* @param[in]    priority     message priority (syslog values)
* @param[in]    message      buffer containing message
*
* @returns  number of characters in log message
* @returns  value less than zero if error
*
*********************************************************************/
int ofdpaCltLogBuf(int priority, ofdpa_buffdesc message);

/*****************************************************************//**
* @brief  Generate a debug message using printf formatting
*
* @param[in]    functionName  name of function generating message
* @param[in]    component     ID of component generating message
* @param[in]    verbosity     message severity
* @param[in]    format        format string
* @param[in]    ...           optional parameters for format string
*
* @returns  number of characters in debug message
* @returns  value less than zero if error
*
*********************************************************************/
int ofdpaCltDebugPrintf(const char *functionName, ofdpaComponentIds_t component, ofdpaDebugLevels_t verbosity, const char *format, ...);

/*****************************************************************//**
* @brief  Generate a debug message using a preformatted buffer
*
* @param[in]    functionName  name of function generating message
* @param[in]    component     ID of component generating message
* @param[in]    verbosity     message severity
* @param[in]    message       buffer containing message
*
* @returns  number of characters in debug message
* @returns  value less than zero if error
*
*********************************************************************/
int ofdpaCltDebugBuf(ofdpa_buffdesc functionName, ofdpaComponentIds_t component, ofdpaDebugLevels_t verbosity, ofdpa_buffdesc message);

/* Flow Tables.
*/

/*****************************************************************//**
 * @brief  Checks whether a table ID is a supported on the platform.
 *
 * @param[in]    tableId           Flow Table ID
 *
 * @retval   OFDPA_E_UNAVAIL   the value in tableId is not a valid table ID number
 * @retval   OFDPA_E_NONE      the value in tableId is a valid table ID number
 *
 * @note Not every flow table defined in the OF-DPA Abstract Switch Specification
 *       is supported on all platforms. This function reports whether flows
 *       can be added to the flow table specified.
 *
  *********************************************************************/
OFDPA_ERROR_t ofdpaFlowTableSupported(OFDPA_FLOW_TABLE_ID_t tableId);

/*****************************************************************//**
 * @brief  Initialize Flow entry structure.
 *
 * @param[in]    tableId           Flow Table ID
 * @param[in,out]    flow              Flow entry structure
 *
 * @retval   OFDPA_E_NONE     if flow entry structure is initialized successfully.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 *
  *********************************************************************/
OFDPA_ERROR_t ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_t tableId, ofdpaFlowEntry_t *flow);

/*****************************************************************//**
* @brief  Initialize Flow entry structure instructions. Preserves all the
*         fields in the flow structure except for the instructions. This
*         is useful to clear out the parts of the structure specifying
*         instructions in preparation for a flow modify call.
*
* @param[in,out]    flow              Flow entry structure
*
* @retval   OFDPA_E_NONE     if flow entry structure is initialized successfully.
* @retval   OFDPA_E_PARAM    if an input parameter is invalid.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowEntryInstructionsInit(ofdpaFlowEntry_t *flow);

/*****************************************************************//**
* @brief  Add a new entry to a flow table.
*
* @param[in]    flow     structure containing flow entry parameters
*
* @retval   OFDPA_E_NONE  flow entry added successfully
* @retval   OFDPA_E_PARAM error in flow entry parameters passed to function
* @retval   OFDPA_E_ERROR flow entry validity check failed
* @retval   OFDPA_E_FAIL  failure occurred during flow entry installation
* @retval   OFDPA_E_FULL  maximum number of flow entries already exist in table
* @retval   OFDPA_E_EXISTS an entry that overlaps the new entry already
*                          exists in the table
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowAdd(ofdpaFlowEntry_t *flow);

/*****************************************************************//**
* @brief  Modify an existing entry in a flow table.
*
* @param[in]    flow     structure containing flow entry parameters
*
* @retval   OFDPA_E_NONE  flow entry added successfully
* @retval   OFDPA_E_PARAM error in flow entry parameters passed to function
* @retval   OFDPA_E_ERROR flow entry validity check failed
* @retval   OFDPA_E_FAIL  failure occurred during flow entry installation
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowModify(ofdpaFlowEntry_t *flow);

/*****************************************************************//**
* @brief Delete an entry from a flow table.
*
* @param[in]    flow     structure containing flow entry parameters
*
* @retval   OFDPA_E_NONE  flow entry added successfully
* @retval   OFDPA_E_PARAM error in flow entry parameters passed to function
* @retval   OFDPA_E_FAIL  failure occurred during flow entry installation
* @retval   OFDPA_E_NOT_FOUND no matching flow entry found
* @retval   OFDPA_E_ERROR flow deletion validation failed
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowDelete(ofdpaFlowEntry_t *flow);

/*****************************************************************//**
* @brief Retrieve the next flow entry in the specified flow table.
*          This API allows the client to walk the entries in a flow table.
*          In order to get the first entry in a flow table, the client
*          calls with the flow table identifier set and all other
*          parameters set to zero.  Table entries are walked in the
*          order the table is indexed.  If there are no entries in the
*          table after the entry specified, a return code indicates the
*          end of the flow table has been reached
*
* @param[in]    flow     structure containing flow entry parameters
* @param[out]    nextFlow  structure containing the next flow table
*                                 entry, if any
*
* @retval   OFDPA_E_NONE  the next flow entry was found and returned
* @retval   OFDPA_E_PARAM error in flow entry parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no next flow entry found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowNextGet(ofdpaFlowEntry_t *flow, ofdpaFlowEntry_t *nextFlow);


OFDPA_ERROR_t ofdpaFlowIdGet(ofdpaFlowEntry_t *flow, uint32_t *flowId);


/*****************************************************************//**
* @brief Returns statistics for the flow entry specified.
*
* @param[in]    flow      structure containing flow entry parameters
* @param[out]    flowStats  structure to store flow entry statistics
*
* @retval   OFDPA_E_NONE  flow entry was found and statistics returned
* @retval   OFDPA_E_PARAM error in flow entry parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no matching flow entry found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowStatsGet(ofdpaFlowEntry_t *flow, ofdpaFlowEntryStats_t *flowStats);

/*****************************************************************//**
* @brief Returns statistics for the flow entry with the specified cookie value.
*
* @param[in]    cookie  cookie for the flow entry
* @param[out]    flow     structure to store flow entry parameters
* @param[out]    flowStats  structure to store flow entry statistics
*
* @retval   OFDPA_E_NONE  flow entry was found and statistics returned
* @retval   OFDPA_E_PARAM error in flow entry parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no flow entry with matching cookie found
* @retval   OFDPA_E_FAIL the operation failed to complete
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowByCookieGet(uint64_t cookie, ofdpaFlowEntry_t *flow, ofdpaFlowEntryStats_t *flowStats);

/*****************************************************************//**
* @brief  Delete the flow entry with the specified cookie.
*
* @param[in]    cookie  cookie for the flow entry
*
* @retval   OFDPA_E_NONE  flow entry deleted successfully
* @retval   OFDPA_E_FAIL  error while deleting flow entry, including
*                         failure to find matching flow entry
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowByCookieDelete(uint64_t cookie);

/*------------------------------------------------------------------------------------*/
/* group table APIs */

/*
   The Group Table contains one entry for each Group.  The table is indexed
   by the groupId which identifies the group entry.  Data is encoded into the groupId to specify the OF-DPA
   group entry type and information required by OF-DPA to configure the datapath.

   The groupId encoding method is:

   L2 Interface, L2 Rewrite types:
        (MSB to LSB) 4 bits encode the Group Table Entry type |12 bits of VLAN ID | 16 bits of port identifier

   L2 Multicast, L2 Flood, L3 Multicast, and L3 Interface types:
        (MSB to LSB) 4 bits encode the Group Table Entry type |12 bits of VLAN ID | 16 bits of index

   L3 Unicast and L3 ECMP types:
        (MSB to LSB) 4 bits encode the Group Table Entry type | 28 bits of index

*/

/* APIs for getting/setting group IDs -- set APIs must set the type first. */

/*****************************************************************//**
* @brief  Get the Group Type encoded in the Group ID.
*
* @param[in]    groupId     group id
* @param[out]    type        group type
*
* @retval   OFDPA_E_NONE  Group Id returned successfully
*
* @note     Applicable for all group types
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupTypeGet(uint32_t groupId, uint32_t *type);

/*****************************************************************//**
* @brief  Get the VLAN ID encoded in the Group Id.
*
* @param[in]    groupId     group id
* @param[out]    vlanId      VLAN Id
*
* @retval   OFDPA_E_NONE  VLAN Id returned successfully
* @retval   OFDPA_E_UNAVAIL  VLAN Id not available
*
* @note     Applicable for L2 Interface, L2 Multicast, L2 Flood,
*           and L3 Multicast group types only
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupVlanGet(uint32_t groupId, uint32_t *vlanId);

/*****************************************************************//**
* @brief  Get the Port ID encoded in the Group Id.
*
* @param[in]    groupId     group id
* @param[out]    portId      Port Id
*
* @retval   OFDPA_E_NONE  Port Id returned successfully
* @retval   OFDPA_E_UNAVAIL  Port Id not available
*
* @note     Applicable for L2 Interface group types only
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupPortIdGet(uint32_t groupId, uint32_t *portId);

/*****************************************************************//**
* @brief  Get the Index encoded in the Group Id.
*
* @param[in]    groupId     group id
* @param[out]    index       Index
*
* @retval   OFDPA_E_NONE  Index returned successfully
* @retval   OFDPA_E_UNAVAIL  Index not available
*
* @note     Applicable Group Types L2 Rewrite, L3 Unicast, L3 Interface, L3 ECMP
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupIndexGet(uint32_t groupId, uint32_t *index);

/*****************************************************************//**
* @brief  Encode the Group Type in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    type        group type
*
* @retval   OFDPA_E_NONE  Encoded successfully
*
* @note     Applicable for all group types
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupTypeSet(uint32_t *groupId, uint32_t type);

/*****************************************************************//**
* @brief  Encode the VLAN Id in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    vlanId      VLAN Id
*
* @retval   OFDPA_E_NONE  Encoded successfully
* @retval   OFDPA_E_UNAVAIL  VLAN Id field not available in Group Id
*
* @note     Applicable Group Types L2 Interface, L2 Multicast, L2 Flood
*           and L3 Multicast
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupVlanSet(uint32_t *groupId, uint32_t vlanId);

/*****************************************************************//**
* @brief  Get the tunnel Id from the Group Id.
*
* @param[in]    groupId     group id
* @param[out]    tunnelId    tunnel Id
*
* @retval   OFDPA_E_NONE  tunnel Id returned successfully
* @retval   OFDPA_E_UNAVAIL  tunnel Id field not available in Group Id
*
* @note     Applicable Group Types L2 Overlay
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupOverlayTunnelIdGet(uint32_t groupId, uint32_t *tunnelId);

/*****************************************************************//**
* @brief  Encode the tunnel Id in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    tunnelId    tunnel Id
*
* @retval   OFDPA_E_NONE  Encoded successfully
* @retval   OFDPA_E_UNAVAIL  tunnel Id field not available in Group Id
*
* @note     Applicable Group Types L2 Overlay
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupOverlayTunnelIdSet(uint32_t *groupId, uint32_t tunnelId);

/*****************************************************************//**
* @brief  Get the Overlay Group Sub-type from the Group Id.
*
* @param[in]    groupId     group id
* @param[out]    subType     sub-type
*
* @retval   OFDPA_E_NONE  Sub-type returned successfully
* @retval   OFDPA_E_UNAVAIL  Sub-type field not available in Group Id
*
* @note     Applicable Group Types L2 Overlay
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupOverlaySubTypeGet(uint32_t groupId, OFDPA_L2_OVERLAY_SUBTYPE_t *subType);

/*****************************************************************//**
* @brief  Encode the Overlay Group Sub-type in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    subType     sub-type
*
* @retval   OFDPA_E_NONE  Encoded successfully
* @retval   OFDPA_E_UNAVAIL  Sub-type field not available in Group Id
*
* @note     Applicable Group Types L2 Overlay
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupOverlaySubTypeSet(uint32_t *groupId, OFDPA_L2_OVERLAY_SUBTYPE_t subType);

/*****************************************************************//**
* @brief  Get the Overlay Group index in the Group Id.
*
* @param[in]    groupId     group id
* @param[out]    index       index
*
* @retval   OFDPA_E_NONE  index returned successfully
* @retval   OFDPA_E_UNAVAIL  index field not available in Group Id
*
* @note     Applicable Group Types L2 Overlay
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupOverlayIndexGet(uint32_t groupId, uint32_t *index);

/*****************************************************************//**
* @brief  Encode the Overlay Group index in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    index       index
*
* @retval   OFDPA_E_NONE  Encoded successfully
* @retval   OFDPA_E_UNAVAIL  Sub-type field not available in Group Id
*
* @note     Applicable Group Types L2 Overlay
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupOverlayIndexSet(uint32_t *groupId, uint32_t index);

/*****************************************************************//**
* @brief  Get the tunnel Id from the Group Id.
*
* @param[in]    groupId     group id
* @param[out]    tunnelId    tunnel Id
*
* @retval   OFDPA_E_NONE  tunnel Id returned successfully
* @retval   OFDPA_E_UNAVAIL  tunnel Id field not available in Group Id
*
* @note     Applicable MPLS Group Sub-types L2 Flood and L2 Multicast
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupMplsL2TunnelIdGet(uint32_t groupId, uint32_t *tunnelId);

/*****************************************************************//**
* @brief  Encode the tunnel Id in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    tunnelId    tunnel Id
*
* @retval   OFDPA_E_NONE  Encoded successfully
* @retval   OFDPA_E_UNAVAIL  tunnel Id field not available in Group Id
*
* @note     Applicable MPLS Group Sub-types L2 Flood and L2 Multicast
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupMplsL2TunnelIdSet(uint32_t *groupId, uint32_t tunnelId);

/*****************************************************************//**
* @brief  Get the MPLS Group Sub-type from the Group Id.
*
* @param[in]    groupId     group id
* @param[out]    subType     sub-type
*
* @retval   OFDPA_E_NONE  Sub-type returned successfully
* @retval   OFDPA_E_UNAVAIL  Sub-type field not available in Group Id
*
* @note     Applicable MPLS Group Sub-types MPLS Interface, MPLS Label,
*           MPLS FAST FAILOVER
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupMplsSubTypeGet(uint32_t groupId, uint32_t *subType);

/*****************************************************************//**
* @brief  Encode the MPLS Group Sub-type in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    subType     sub-type
*
* @retval   OFDPA_E_NONE  Encoded successfully
* @retval   OFDPA_E_UNAVAIL  Sub-type field not available in Group Id
*
* @note     Applicable Group Types MPLS Interface, MPLS Label,
*           MPLS FAST FAILOVER
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupMplsSubTypeSet(uint32_t *groupId, uint32_t subType);

/*****************************************************************//**
* @brief  Encode the Port Id in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    portId      Port Id
*
* @retval   OFDPA_E_NONE  Encoded successfully
* @retval   OFDPA_E_UNAVAIL  Port Id field not available in Group Id
*
* @note     Applicable Group Types L2 Interface only
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupPortIdSet(uint32_t *groupId, uint32_t portId);

/*****************************************************************//**
* @brief  Encode the Index in the Group Id.
*
* @param[in,out]    groupId     group id
* @param[in]    index       Index
*
* @retval   OFDPA_E_NONE  Encoded successfully
* @retval   OFDPA_E_UNAVAIL  Index field not available in Group Id
*
* @note     Applicable Group Types L2 Rewrite, L3 Unicast, L3 Interface, L3 ECMP
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupIndexSet(uint32_t *groupId, uint32_t index);

/*****************************************************************//**
* @brief  Decode a group ID value into a descriptive string suitable for
*         display to the user.
*
* @param[in]    groupId     group id
* @param[out]   outBuf      group id decoded
* @param[in]    bufSize     size of outBuf
*
* @retval   OFDPA_E_NONE  Group Id Decoded successfully
* @retval   OFDPA_E_FULL  bufSize not sufficient to hold the decoded Group Id,
*                         output truncated
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupDecode(uint32_t groupId, char *outBuf, int bufSize);

/*****************************************************************//**
* @brief  Decode a group bucket configuration structure into a descriptive string suitable for
*         display to the user.
*
* @param[in]    bucketEntry group bucket configuration structure
* @param[out]   outBuf      buffer to receive string
* @param[in]    bufSize     size of outBuf
*
* @retval   OFDPA_E_NONE  Group bucket entry decoded successfully
* @retval   OFDPA_E_FULL  bufSize not sufficient to hold the string generated,
*                         output truncated
* @retval   OFDPA_E_ERROR Error parsing group ID type or sub-type content
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketEntryDecode(ofdpaGroupBucketEntry_t *bucketEntry, char *outBuf, int bufSize);

/*****************************************************************//**
* @brief  Returns the a string suitable for
*         display to the user representing the color value.
*
* @param[in]    color value representing QoS color
*
* @retval   pointer to a string containing the color string
*
* @note If color value cannot be converted to a string, a string
*       containing the text "[Unknown color]" is returned.
*
*********************************************************************/
const char *ofdpaQosColorDecode(OFDPA_QOS_COLORS_t color);

/*****************************************************************//**
* @brief  Returns the flow table name as a string suitable for
*         display to the user.
*
* @param[in]    tableId flow table identifier
*
* @retval   pointer to a string containing the flow table's name
*
* @note If tableId value cannot be converted to name string, a string
*       containing the text "[Table name not found]" is returned.
*
*********************************************************************/
const char *ofdpaFlowTableNameGet(OFDPA_FLOW_TABLE_ID_t tableId);

/*****************************************************************//**
* @brief  Returns a string describing a Color Based Count Action
*         value suitable for display to the user.
*
* @param[in]    colorBasedCountAction one of the enum values
*
* @retval   pointer to a string containing a descriptive name
*
* @note If colorBasedCountAction value does not match an enum member, a string
*       containing the text "[Color Count Action string not found]" is returned.
*
*********************************************************************/
const char *colorBasedCountActionDecode(OFDPA_ACTION_COLOR_BASED_COUNT_t colorBasedCountAction);

/*****************************************************************//**
* @brief  Decode a flow configuration structure into a descriptive string suitable for
*         display to the user.
*
* @param[in]    flow        flow configuration structure
* @param[out]   outBuf      buffer to receive string
* @param[in]    bufSize     size of outBuf
*
* @retval   OFDPA_E_NONE  entry decoded successfully
* @retval   OFDPA_E_FULL  bufSize not sufficient to hold the string generated,
*                         output truncated
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowEntryDecode(ofdpaFlowEntry_t *flow, char *outBuf, int bufSize);

/*****************************************************************//**
* @brief  Add Group.
*
* @param[in]    group     group entry
*
* @retval   OFDPA_E_NONE  Group added successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FULL  Group table full
* @retval   OFDPA_E_EXISTS  Group entry exists
* @retval   OFDPA_E_NOT_FOUND  port encoded in Group type L2
*                              Interface not found
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupAdd(ofdpaGroupEntry_t *group);

/*****************************************************************//**
* @brief  Delete Group.
*
* @param[in]    groupId     group id
*
* @retval   OFDPA_E_NONE  Group deleted successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  group entry not found
* @retval   OFDPA_E_FAIL  failure; other errors
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupDelete(uint32_t groupId);

/*****************************************************************//**
* @brief  Get the next Group entry.
*
* @param[in]    groupId     group id
* @param[out]    nextGroup   next group entry
*
* @retval   OFDPA_E_NONE  next group entry returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  next group does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupNextGet(uint32_t groupId, ofdpaGroupEntry_t *nextGroup);

/*****************************************************************//**
* @brief  Get the next Group entry of a given type.
*
* @param[in]    groupId     group id
* @param[in]    groupType   group type
* @param[out]    nextGroup   next group entry
*
* @retval   OFDPA_E_NONE  next group entry returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  next group does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupTypeNextGet(uint32_t groupId,
                                    OFDPA_GROUP_ENTRY_TYPE_t groupType,
                                    ofdpaGroupEntry_t *nextGroup);

/*****************************************************************//**
* @brief  Get Group statistics.
*
* @param[in]    groupId     group id
* @param[out]    groupStats  group statistics
*
* @retval   OFDPA_E_NONE  group statistics returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  group not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupStatsGet(uint32_t groupId, ofdpaGroupEntryStats_t *groupStats);

/*
   Group Table entries contain one or more Action Buckets depending on their type.
   The Group Bucket Table stores these references.  It is indexed by groupId and referenceGroupId.  The presence
   of an entry in this table creates a referral by the Group Table entry specified in groupId to the Group Table
   entry specified in referenceGroupId.  Restrictions on the number of references and the allowable type of the
   referenced Group Table entries varies by entry type.
*/

/*****************************************************************//**
* @brief  Add a Group Bucket entry.
*
* @param[in]    bucket     group bucket entry
*
* @retval   OFDPA_E_NONE        group bucket entry added successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_FULL        group bucket table full
* @retval   OFDPA_E_NOT_FOUND   group not found
* @retval   OFDPA_E_EXISTS      group bucket entry exists
* @retval   OFDPA_E_INTERNAL    internal errors or failures
* @retval   OFDPA_E_FAIL        internal errors or failures
*
* @note For group table entries allowed to have multiple buckets,
*        this is called more than once
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *bucket);

/*****************************************************************//**
* @brief  Delete a Group Bucket entry.
*
* @param[in]    groupId       group id
* @param[in]    bucketIndex   group bucket index
*
* @retval   OFDPA_E_NONE        group bucket entry deleted successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND   group bucket entry not found
* @retval   OFDPA_E_EXISTS      group reference exists
* @retval   OFDPA_E_INTERNAL    internal errors or failures
* @retval   OFDPA_E_FAIL        internal errors or failures
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex);

/*****************************************************************//**
* @brief  Delete all Buckets from a Group.
*
* @param[in]    groupId       group id
*
* @retval   OFDPA_E_NONE        group bucket entry deleted successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_EXISTS      group reference exists
* @retval   OFDPA_E_INTERNAL    internal errors or failures
* @retval   OFDPA_E_FAIL        internal errors or failures
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketsDeleteAll(uint32_t groupId);

/*****************************************************************//**
* @brief  Get the Group Bucket entry.
*
* @param[in]    groupId       group id
* @param[in]    bucketIndex   bucket index
* @param[out]    groupBucket   group bucket entry
*
* @retval   OFDPA_E_NONE        group bucket entry returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_FAIL        group bucket entry does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketEntryGet(uint32_t groupId, uint32_t bucketIndex,
                                       ofdpaGroupBucketEntry_t *groupBucket);

/*****************************************************************//**
* @brief  Get the first Group Bucket entry.
*
* @param[in]    groupId       group id
* @param[out]    firstGroupBucket   first group bucket entry
*
* @retval   OFDPA_E_NONE        group bucket entry returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_FAIL        group bucket entry does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketEntryFirstGet(uint32_t groupId,
                                            ofdpaGroupBucketEntry_t *firstGroupBucket);

/*****************************************************************//**
* @brief  Get the next Group Bucket entry.
*
* @param[in]    groupId       group id
* @param[in]    bucketIndex   bucket index
* @param[out]    nextBucketEntry   next group bucket entry
*
* @retval   OFDPA_E_NONE        next group bucket entry returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_FAIL        next group bucket entry does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
                                           ofdpaGroupBucketEntry_t *nextBucketEntry);

/*****************************************************************//**
* @brief  Modify Group Bucket entry.
*
* @param[in]    bucket       group bucket entry to be modified
*
* @retval   OFDPA_E_NONE        group bucket entry modified successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND   group bucket entry not found
* @retval   OFDPA_E_INTERNAL    internal errors or failures
* @retval   OFDPA_E_FAIL        internal errors or failures
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketEntryModify(ofdpaGroupBucketEntry_t *bucket);

/*****************************************************************//**
* @brief  Reports the number of group entries of all types.
*
* @param[out]    entryCount         total number of group entries configured
*
* @retval   OFDPA_E_NONE        group entry count returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupTableTotalEntryCountGet(uint32_t *entryCount);

/*****************************************************************//**
* @brief  Get Group table info.
*
* @param[in]    groupId         group id
* @param[in]    info            info
*
* @retval   OFDPA_E_NONE        group info returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND   group type not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaGroupTableInfoGet(uint32_t groupId, ofdpaGroupTableInfo_t *info);

/*****************************************************************//**
* @brief  Get the port type encoded in a port number value.
*
* @param[in]    portNum     port number value
* @param[out]   type        port type
*
* @returns  nothing
*
*********************************************************************/
void ofdpaPortTypeGet(uint32_t portNum, uint32_t *type);

/*****************************************************************//**
* @brief  Encode the port type in a port number value
*
* @param[in,out]  portNum     port number storage
* @param[in]      type        port type
*
* @returns  nothing
*
*********************************************************************/
void ofdpaPortTypeSet(uint32_t *portNum, uint32_t type);

/*****************************************************************//**
* @brief  Get the port index encoded in a port number value.
*
* @param[in]    portNum      port number value
* @param[out]   index        port index
*
* @returns  nothing
*
*********************************************************************/
void ofdpaPortIndexGet(uint32_t portNum, uint32_t *index);

/*****************************************************************//**
* @brief  Encode the port index in a port number
*
* @param[in,out]  portNum      port number storage
* @param[in]      index        port index
*
* @returns  nothing
*
*********************************************************************/
void ofdpaPortIndexSet(uint32_t *portNum, uint32_t index);

/*****************************************************************//**
* @brief  Get the next port from the port table
*
* @param[in]    portNum      current port
* @param[out]   nextPortNum  next port
*
* @retval   OFDPA_E_NONE  next port returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  no more ports to be returned
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortNextGet(uint32_t portNum, uint32_t *nextPortNum);

/*****************************************************************//**
* @brief  Get the MAC address of the given port
*
* @param[in]    portNum  current port
* @param[out]   mac      MAC address of the port
*
* @retval   OFDPA_E_NONE  MAC address returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortMacGet(uint32_t portNum, ofdpaMacAddr_t *mac);

/*****************************************************************//**
* @brief  Get the port name
*
* @param[in]    portNum  port number
* @param[out]   name     port name
*
* @retval   OFDPA_E_NONE  port name returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
* @note     Initialize the ofdpa_buffdesc with the name buffer and size
*           appropriately. The size of the name buffer (including '\0')
*           should be at least equal to OFDPA_MAX_PORT_NAME_STRING_SIZE
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortNameGet(uint32_t portNum, ofdpa_buffdesc *name);

/*****************************************************************//**
* @brief  Get the port link state
*
* @param[in]    portNum  port number
* @param[out]   state    port link state
*
* @retval   OFDPA_E_NONE  port link state returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortStateGet(uint32_t portNum, uint32_t *state);

/*****************************************************************//**
* @brief  Set the port administrative state
*
* @param[in]    portNum  port number
* @param[in]    config   port administrative state
*
* @retval   OFDPA_E_NONE  port admin state set successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortConfigSet(uint32_t portNum, OFDPA_PORT_CONFIG_t config);

/*****************************************************************//**
* @brief  Get the port administrative state
*
* @param[in]    portNum  port number
* @param[out]   config   port administrative state
*
* @retval   OFDPA_E_NONE  port admin state returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortConfigGet(uint32_t portNum, uint32_t *config);

/*****************************************************************//**
* @brief  Get the port maximum speed
*
* @param[in]    portNum   port number
* @param[out]   maxSpeed  port maximum speed
*
* @retval   OFDPA_E_NONE  port max speed returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortMaxSpeedGet(uint32_t portNum, uint32_t  *maxSpeed);

/*****************************************************************//**
* @brief  Get the port current speed
*
* @param[in]    portNum    port number
* @param[out]   currSpeed  port current speed
*
* @retval   OFDPA_E_NONE  port current speed returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortCurrSpeedGet(uint32_t portNum, uint32_t  *currSpeed);

/*****************************************************************//**
* @brief  Get the port features
*
* @param[in]    portNum  port number
* @param[out]   feature  port features
*
* @retval   OFDPA_E_NONE  port features returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortFeatureGet(uint32_t portNum, ofdpaPortFeature_t *feature);

/*****************************************************************//**
* @brief  Set the port features that can be advertized durng Auto-Negotiation
*
* @param[in]    portNum    port number
* @param[in]    advertise  port features
*
* @retval   OFDPA_E_NONE  port features set successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortAdvertiseFeatureSet(uint32_t portNum, uint32_t advertise);

/*****************************************************************//**
* @brief  Clear port statistics
*
* @param[in]    portNum  port number
*
* @retval   OFDPA_E_NONE  port statistics cleared successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortStatsClear(uint32_t portNum);

/*****************************************************************//**
* @brief  Get port statistics
*
* @param[in]    portNum  port number
* @param[out]   stats    port statistics
*
* @retval   OFDPA_E_NONE  port statistics returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  port not found
* @retval   OFDPA_E_UNAVAIL   counter not available
* @retval   OFDPA_E_INTERNAL  internal error
*
* @note The data structure returned contains 64 bit, unsigned integer
*       members for various counters. Not all counters are supported
*       on every platform. Unsupported counters are indicated by the
*       counter value being set to all ones (0xFFFFFFFFFFFFFFFF). Counters
*       returning this value are considered unsupported.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortStatsGet(uint32_t portNum, ofdpaPortStats_t *stats);

/*****************************************************************************
 Prototype    : ofdpaPortQueueSchedSet
 Description  : This routine set the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                int32_t mode       
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
OFDPA_ERROR_t  ofdpaPortQueueSchedSet(uint32_t port, int32_t mode,int32_t *weights);

/*****************************************************************************
 Prototype    : ofdpaPortQueueWeightModify
 Description  : This routine modify the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                uint32_t queueId       
                int32_t  weight  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdpaPortQueueWeightModify(uint32_t port,uint32_t queueId, int32_t weight);


/*****************************************************************************
 Prototype    : ofdpaPortQueueSchedGet
 Description  : Get the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                int32_t *mode       
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
OFDPA_ERROR_t  ofdpaPortQueueSchedGet(uint32_t port, int32_t *mode,int32_t *weights);

/*****************************************************************************
 Prototype    : ofdpaPortDiscardModeSet
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
OFDPA_ERROR_t  ofdpaPortDiscardModeSet(uint32_t mode);


/*****************************************************************************
 Prototype    : ofdpaPortDiscardModeGet
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
OFDPA_ERROR_t  ofdpaPortDiscardModeGet(uint32_t *mode);

/*****************************************************************************
 Prototype    : ofdpaPortDiscardConfig
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
OFDPA_ERROR_t  ofdpaPortDiscardConfig(uint32_t port, uint32_t cosq,uint32_t color,
  int drop_start, int drop_slope,int drop_end);

/*****************************************************************************
 Prototype    : ofdpaPortDiscardConfigGet
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
OFDPA_ERROR_t  ofdpaPortDiscardConfigGet(uint32_t port, uint32_t cosq,uint32_t color,
  int *drop_start, int *drop_slope,int *drop_end);


/*****************************************************************//**
* @brief  Get the MPLS L2 port type encoded in a port number value.
*
* @param[in]    portNum     port number value
* @param[out]   type        port type
*
* @returns  nothing
*
*********************************************************************/
void ofdpaMplsL2PortTypeGet(uint32_t portNum, uint32_t *type);

/*****************************************************************//**
* @brief  Encode the MPLS L2 port type in a port number value
*
* @param[in,out]    portNum     port number storage
* @param[in]    type        port type
*
* @returns  nothing
*
*********************************************************************/
void ofdpaMplsL2PortTypeSet(uint32_t *portNum, uint32_t type);

/*****************************************************************//**
* @brief  Get the MPLS L2 port index encoded in a port number value.
*
* @param[in]    portNum      port number value
* @param[out]    index        port index
*
* @returns  nothing
*
*********************************************************************/
void ofdpaMplsL2PortIndexGet(uint32_t portNum, uint32_t *index);

/*****************************************************************//**
* @brief  Encode the MPLS L2 port index in a port number
*
* @param[in,out]    portNum      port number storage
* @param[in]        index        port index
*
* @returns  nothing
*
*********************************************************************/
void ofdpaMplsL2PortIndexSet(uint32_t *portNum, uint32_t index);

/*****************************************************************//**
* @brief  Get the tunnel type encoded in a tunnel ID value.
*
* @param[in]    tunnelId    tunnel ID value
* @param[out]   type        tunnel type
*
* @returns  nothing
*
*********************************************************************/
void ofdpaTunnelIdTypeGet(uint32_t tunnelId, uint32_t *type);

/*****************************************************************//**
* @brief  Get the tunnel index encoded in a tunnel ID value.
*
* @param[in]    tunnelId     tunnel ID value
* @param[out]   index        tunnel index
*
* @returns  nothing
*
*********************************************************************/
void ofdpaTunnelIdIndexGet(uint32_t tunnelId, uint32_t *index);

/*****************************************************************//**
* @brief  Encode the tunnel type in a tunnel ID value
*
* @param[in,out]    tunnelId     tunnel ID storage
* @param[in]        type         tunnel type
*
* @returns  nothing
*
*********************************************************************/
void ofdpaTunnelIdTypeSet(uint32_t *tunnelId, uint32_t type);

/*****************************************************************//**
* @brief  Encode the tunnel index in a tunnelId number
*
* @param[in,out]    tunnelId     tunnel ID storage
* @param[in]    index        tunnel index
*
* @returns  nothing
*
*********************************************************************/
void ofdpaTunnelIdIndexSet(uint32_t *tunnelId, uint32_t index);

/*****************************************************************//**
* @brief  Create a tunnel logical port.
*
* @param[in]    portNum   unique identifier for the port within the switch
* @param[in]    name      human readable name for the interface
* @param[in]    config    structure containing tunnel logical port parameters
*
* @retval   OFDPA_E_NONE  logical port successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR internal error
* @retval   OFDPA_E_FAIL  failure occurred during logical port creation
* @retval   OFDPA_E_FULL  maximum number of logical ports already created
* @retval   OFDPA_E_EXISTS an entry with the specified portnum already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelPortCreate(uint32_t portNum, ofdpa_buffdesc *name, ofdpaTunnelPortConfig_t *config);

/*****************************************************************//**
* @brief  Delete a tunnel logical port.
*
* @param[in]    portNum   unique identifier for the port within the switch
*
* @retval   OFDPA_E_NONE  logical port successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND logical port with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during logical port deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelPortDelete(uint32_t portNum);

/*****************************************************************//**
* @brief  Retrieve tunnel logical port configuration and/or status.
*
* @param[in]    portNum   unique identifier for the port within the switch
* @param[out]    config    structure containing tunnel logical port configuration (optional)
* @param[out]    status    structure containing tunnel logical port status (optional)
*
* @retval   OFDPA_E_NONE  logical port data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND logical port with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of a logical port, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelPortGet(uint32_t portNum,
                                 ofdpaTunnelPortConfig_t *config,
                                 ofdpaTunnelPortStatus_t *status);

/*****************************************************************//**
* @brief  Return the tunnel logical port identifier, if any, for the
*           logical port after the one specified in the logical port table.
*
* @param[in]    portNum   unique identifier for the port within the switch
* @param[out]    nextPortNum   unique identifier for the next logical port within the switch
*
* @retval   OFDPA_E_NONE  next logical port identifier found
* @retval   OFDPA_E_FAIL  no next logical port identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelPortNextGet(uint32_t portNum, uint32_t *nextPortNum);

/*****************************************************************//**
* @brief  Adds a tenant reference to a logical port tunnel entry. This
*           is done by adding any entry to the tunnel port tenant table.
*           The index into this table is the portNum, tunnelId tuple.
*
* @param[in]    portNum    unique identifier for the port within the switch
* @param[in]    tunnelId   tenant entry identifier
*
* @retval   OFDPA_E_NONE  logical port to tenant association successfully added
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error, includes logical port or tenant
*                         entry not found or other constraint violation
* @retval   OFDPA_E_FAIL  failure occurred during logical port association with tenant
* @retval   OFDPA_E_FULL  maximum number of associations already created
* @retval   OFDPA_E_EXISTS an entry with the specified portnum and tunnelId already added
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelPortTenantAdd(uint32_t portNum, uint32_t tunnelId);

/*****************************************************************//**
* @brief  Deletes a tenant reference to a logical port tunnel entry.
*
* @param[in]    portNum    unique identifier for the port within the switch
* @param[in]    tunnelId   tenant entry identifier
*
* @retval   OFDPA_E_NONE  logical port to tenant association successfully deleted
* @retval   OFDPA_E_FAIL  failure occurred deleting logical port association with tenant,
*                         including the entry not being found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelPortTenantDelete(uint32_t portNum, uint32_t tunnelId);

/*****************************************************************//**
* @brief  Test whether a tenant reference to a logical port exists for
*           the port and tenant combination.
*           Optionally retrieve status of entry.
*
* @param[in]    portNum    unique identifier for the port within the switch
* @param[in]    tunnelId   tenant entry identifier
* @param[out]    status     structure containing tunnel logical port/tenant status (optional)
*
* @retval   OFDPA_E_NONE  logical port to tenant association successfully deleted
* @retval   OFDPA_E_NOT_FOUND logical port to tenant association is not configured
*
* @note If the caller does not require the data in
*        status structure, this parameter may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of an entry for the logical port and tenant, but has no need for the
*        status data.
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelPortTenantGet(uint32_t portNum, uint32_t tunnelId, ofdpaTunnelPortTenantStatus_t *status);

/*****************************************************************//**
* @brief  Gets the next logical port tunnel entry after the port and tenant identified.
*           Iteration stops after returning the tunnelId for the last tenant
*           associated with the logical port.
*
* @param[in]    portNum        unique identifier for the port within the switch
* @param[in]    tunnelId       tenant entry identifier
* @param[out]    nextTunnelId   next tenant entry identifier, if any
*
* @retval   OFDPA_E_NONE       next logical port tunnel identifier found
* @retval   OFDPA_E_PARAM      error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no next logical port tunnel identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelPortTenantNextGet(uint32_t portNum, uint32_t tunnelId, uint32_t *nextTunnelId);

/*****************************************************************//**
* @brief  Create a tunnel tenant entry.
*
* @param[in]    tunnelId   unique identifier for the tenant
* @param[in]    config    structure containing tenant parameters
*
* @retval   OFDPA_E_NONE  tenant entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error including specifying a
*                         multicast next hop entry reference that does not exist
* @retval   OFDPA_E_FAIL  failure occurred during tenant creation
* @retval   OFDPA_E_FULL  maximum number of tenants already created
* @retval   OFDPA_E_EXISTS an entry with the specified tunnelId already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelTenantCreate(uint32_t tunnelId, ofdpaTunnelTenantConfig_t *config);

/*****************************************************************//**
* @brief  Delete a tunnel tenant entry.
*
* @param[in]    tunnelId   unique identifier for the tenant
*
* @retval   OFDPA_E_NONE  tenant entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND tenant entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during tenant deletion,
*                         reasons include attempting to delete a tenant entry
*                         that is being referenced
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelTenantDelete(uint32_t tunnelId);

/*****************************************************************//**
* @brief  Retrieve tunnel tenant configuration and/or status.
*
* @param[in]    tunnelId   unique identifier for the tenant
* @param[out]    config    structure containing tunnel tenant configuration (optional)
* @param[out]    status    structure containing tunnel tenant status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelTenantGet(uint32_t tunnelId,
                                   ofdpaTunnelTenantConfig_t *config,
                                   ofdpaTunnelTenantStatus_t *status);

/*****************************************************************//**
* @brief  Return the tunnel identifier, if any, for the
*           entry after the one specified.
*
* @param[in]    tunnelId   unique identifier for the tenant
* @param[out]    nextTunnelId   unique identifier for the next tenant entry
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelTenantNextGet(uint32_t tunnelId, uint32_t *nextTunnelId);

/*****************************************************************//**
* @brief  Create a tunnel next hop entry.
*
* @param[in]    nextHopId   unique identifier for the entry
* @param[in]    config    structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during entry creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified identifier already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelNextHopCreate(uint32_t nextHopId, ofdpaTunnelNextHopConfig_t *config);

/*****************************************************************//**
* @brief  Delete a tunnel next hop entry.
*
* @param[in]    nextHopId   unique identifier for the entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion,
*                         reasons include attempting to delete an entry
*                         that is being referenced
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelNextHopDelete(uint32_t nextHopId);

/*****************************************************************//**
* @brief  Modify a tunnel next hop entry. This allows updating the configuration
*           data of a next hop while it is referenced by other configuration.
*           A referenced entry cannot be deleted but it can be modified.
*
* @param[in]    nextHopId   unique identifier for an existing entry
* @param[in]    config    structure containing new configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully modified
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error, including attempting to modify next hop
*                         in a manner not allowed.  See notes.
* @retval   OFDPA_E_FAIL  failure occurred during entry modification
* @retval   OFDPA_E_NOT_FOUND  entry with identifier not configured
*
* @note Modification cannot change the destination MAC address from unicast to multicast or
*        multicast to unicast.  Modification cannot change the protocol of the next hop entry.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelNextHopModify(uint32_t nextHopId, ofdpaTunnelNextHopConfig_t *config);

/*****************************************************************//**
* @brief  Retrieve tunnel next hop configuration and/or status.
*
* @param[in]    nextHopId  unique identifier for the entry
* @param[out]   config     structure containing tunnel next hop configuration (optional)
* @param[out]   status     structure containing tunnel next hop status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelNextHopGet(uint32_t nextHopId,
                                    ofdpaTunnelNextHopConfig_t *config,
                                    ofdpaTunnelNextHopStatus_t *status);

/*****************************************************************//**
* @brief  Return the tunnel next hop identifier, if any, for the
*           entry after the one specified.
*
* @param[in]    nextHopId   unique identifier for the next hop
* @param[out]    nextNextHopId   unique identifier for the next next hop entry
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelNextHopNextGet(uint32_t nextHopId, uint32_t *nextNextHopId);

/*****************************************************************//**
* @brief  Create a tunnel ECMP next hop group entry.
*
* @param[in]    ecmpNextHopGroupId   unique identifier for the entry
* @param[in]    config    structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during entry creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified identifier already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupCreate(uint32_t ecmpNextHopGroupId, ofdpaTunnelEcmpNextHopGroupConfig_t *config);

/*****************************************************************//**
* @brief  Delete a tunnel ECMP next hop group entry.
*
* @param[in]    ecmpNextHopGroupId   unique identifier for the entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion,
*                         reasons include attempting to delete an entry
*                         that is being referenced
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupDelete(uint32_t ecmpNextHopGroupId);

/*****************************************************************//**
* @brief  Retrieve tunnel ECMP next hop group configuration and/or status.
*
* @param[in]    ecmpNextHopGroupId   unique identifier for the entry
* @param[out]    config    structure containing ECMP tunnel next hop configuration (optional)
* @param[out]    status    structure containing ECMP tunnel next hop status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupGet(uint32_t ecmpNextHopGroupId,
                                             ofdpaTunnelEcmpNextHopGroupConfig_t *config,
                                             ofdpaTunnelEcmpNextHopGroupStatus_t *status);

/*****************************************************************//**
* @brief  Return the ECMP tunnel next hop group identifier, if any, for the
*           entry after the one specified.
*
* @param[in]    ecmpNextHopGroupId   unique identifier for the entry
* @param[out]    nextEcmpNextHopGroupId   unique identifier for the next entry
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupNextGet(uint32_t ecmpNextHopGroupId, uint32_t *nextEcmpNextHopGroupId);

/*****************************************************************//**
* @brief  Return the number of ECMP next hop group members supported by the
*           switch.
*
* @param[out]    maxMemberCount   maximum members supported in an ECMP next hop group
*
* @retval   OFDPA_E_NONE  max count returned
* @retval   OFDPA_E_PARAM NULL pointer passed
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupMaxMembersGet(uint32_t *maxMemberCount);

/*****************************************************************//**
* @brief  Adds a next hop member to an ECMP next hop group entry. This
*           is done by adding any entry to the ECMP next hop group member table.
*           The index into this table is the ecmpNextHopGroupId, nextHopId tuple.
*
* @param[in]    ecmpNextHopGroupId    ECMP next hop group entry identifier
* @param[in]    nextHopId   next hop entry identifier
*
* @retval   OFDPA_E_NONE  member next hop successfully added
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error, includes ECMP tunnel next hop or
*                         next hop entry not found or other constraint violation
* @retval   OFDPA_E_FAIL  failure occurred while adding next hop to ECMP next hop group
* @retval   OFDPA_E_FULL  maximum number of member next hops already added
* @retval   OFDPA_E_EXISTS an entry with the specified identifier combination already added
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupMemberAdd(uint32_t ecmpNextHopGroupId, uint32_t nextHopId);

/*****************************************************************//**
* @brief  Adds a next hop member to an ECMP next hop group entry. This
*           is done by adding any entry to the ECMP next hop group member table.
*           The index into this table is the ecmpNextHopGroupId, nextHopId tuple.
*
* @param[in]    ecmpNextHopGroupId    ECMP next hop group entry identifier
* @param[in]    nextHopId   next hop entry identifier
*
* @retval   OFDPA_E_NONE  next hop member successfully deleted
* @retval   OFDPA_E_NOT_FOUND next hop member is not configured in ECMP next hop group
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupMemberDelete(uint32_t ecmpNextHopGroupId, uint32_t nextHopId);

/*****************************************************************//**
* @brief  Test whether a next hop is a member in the specified ECMP next hop group.
*
* @param[in]    ecmpNextHopListGroupId ECMP next hop group entry identifier
* @param[in]    nextHopId              next hop entry identifier
*
* @retval   OFDPA_E_NONE  next hop is a member of ECMP next hop group
* @retval   OFDPA_E_NOT_FOUND next hop is not a member of ECMP next hop group
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupMemberGet(uint32_t ecmpNextHopListGroupId, uint32_t nextHopId);

/*****************************************************************//**
* @brief  Gets the next member next hop in the ECMP next hop group.
*
* @param[in]    ecmpNextHopListGroupId ECMP next hop group entry identifier
* @param[in]    nextHopId              next hop entry identifier
* @param[out]   nextNextHopId          next next hop entry identifier, if any
*
* @retval   OFDPA_E_NONE       next entry found
* @retval   OFDPA_E_PARAM      error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no next entry found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaTunnelEcmpNextHopGroupMemberNextGet(uint32_t ecmpNextHopListGroupId, uint32_t nextHopId, uint32_t *nextNextHopId);

/*****************************************************************//**
* @brief  Add remark action entry.
*
* @param[in]    remarkEntry   structure containing remark entry parameters
*
* @retval   OFDPA_E_NONE  remark entry added successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FULL  Remark Action table full
* @retval   OFDPA_E_EXISTS  remark entry exists
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaRemarkActionAdd(ofdpaRemarkActionEntry_t *remarkEntry);

/*****************************************************************//**
* @brief  Delete remark action entry.
*
* @param[in]    remarkEntry   structure containing remark entry parameters
*
* @retval   OFDPA_E_NONE  remark entry deleted successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  remark entry not found
* @retval   OFDPA_E_FAIL  failure; other errors
*
*********************************************************************/
OFDPA_ERROR_t ofdpaRemarkActionDelete(ofdpaRemarkActionEntry_t *remarkEntry);

/*****************************************************************//**
* @brief  Get remark action entry.
*
* @param[in, out]   remarkEntry      remark entry
*
* @retval   OFDPA_E_NONE         remark entry returned successfully
* @retval   OFDPA_E_NOT_FOUND    remark entry not found
* @retval   OFDPA_E_PARAM        error in parameters passed to function
*
*********************************************************************/
OFDPA_ERROR_t ofdpaRemarkActionEntryGet(ofdpaRemarkActionEntry_t *remarkEntry);

/*****************************************************************//**
* @brief Retrieve the next remark entry. This API allows the client
*        to walk the entries in a remark table. In order to get the
*        first entry in a remark table, the client calls with all
*        parameters set to zero.  Table entries are walked in the
*        order the table is indexed.  If there are no entries in the
*        table after the entry specified, a return code indicates the
*        end of the remark table has been reached
*
* @param[in]    remarkEntry   structure containing flow entry parameters
* @param[out]   nextEntry     structure containing the next remark table
*                                 entry, if any
*
* @retval   OFDPA_E_NONE  the next flow entry was found and returned
* @retval   OFDPA_E_PARAM error in flow entry parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no next flow entry found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaRemarkEntryNextGet(ofdpaRemarkActionEntry_t *remarkEntry, ofdpaRemarkActionEntry_t *nextEntry);

/*****************************************************************//**
* @brief  Packet out API. Sends a packet out a switch port
*
* @param[in]    pkt  packet
* @param[in]    flags
* OFDPA_PACKET_LOOKUP_TX: Indicates that packet must go through a hardware lookup
*                          starting from the first table in the pipeline. The srcIfNum
*                          is used as the ingress port in table lookups.
*
*                          If this flag is not set, then the packet is sent unmodified
*                          out of the outIfNum
*
* @param[out]    outPortNum  Output port. Can be physical or logical or reserved
* @param[in]    inPortNum  Input port. Indicates the interface on which packet was
*                                 originally received.
*
* @retval   OFDPA_E_NONE if packet is transmitted successfully
* @retval   OFDPA_E_PARAM if an input parameter is bad
* @retval   OFDPA_E_NOT_FOUND if the input or output port parameter is bad
* @retval   OFDPA_E_INTERNAL if transmit DMA buffer has not been allocated
* @returns  Any other return code if there is a failure
*
* @note    DPA will not do any buffering of packets and it is expected to be done
*           at the agent if required
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPktSend(ofdpa_buffdesc *pkt, uint32_t flags, uint32_t outPortNum, uint32_t inPortNum);

/*****************************************************************//**
 * @brief  Report the size in bytes of the largest packet that can be received.
 *
 * @param[out]    pktSize      The maximum packet size, in bytes.
 *
 * @retval   OFDPA_E_NONE
 *
 * @note    A client can use the returned value to size a packet receive buffer.
 *
  * ********************************************************************/
OFDPA_ERROR_t ofdpaMaxPktSizeGet(uint32_t *pktSize);

/*------------------------------------------------------------------------------------*/
/* Event APIs */

/*
   Asynchronous events: Have classified them into two broad category:
   Packet In : Packet received from the hardware to be sent to the controller
   Control messages: Events like Port creation, deletion, link state, Flow age and Error

   The packet In events can have a high frequency, so we want to ensure that there is
   separate control for polling of these two event types to give flexibility to the agent
*/

/*****************************************************************//**
 * @brief  Opens and binds OF-DPA Client's event socket.
 *
  *
 * @returns  event socket fd
 *
 * @note This function must be called before calls to ofdpaEventReceive() or
 *        ofdpaClientEventSockFdGet().
 *
 *        Only one process in the system should bind the event socket at
 *        any one time. If multiple running processes have called this function,
 *        unpredictable results may occur.
 *
  *********************************************************************/
OFDPA_ERROR_t ofdpaClientEventSockBind(void);

/*****************************************************************//**
 * @brief  Get OF-DPA Client's event socket fd
 *
  *
 * @returns  event socket fd
 *
  *********************************************************************/
int ofdpaClientEventSockFdGet(void);

/*****************************************************************//**
 * @brief  Opens and binds OF-DPA Client's packet socket.
 *
  *
 * @returns  event socket fd
 *
 * @note This function must be called before calls to ofdpaPktReceive() or
 *        ofdpaClientPktSockFdGet().
 *
 *        Only one process in the system should bind the event socket at
 *        any one time. If multiple running processes have called this function,
 *        unpredictable results may occur.
 *
  *********************************************************************/
OFDPA_ERROR_t ofdpaClientPktSockBind(void);

/*****************************************************************//**
 * @brief  Get OF-DPA Client's packet socket fd
 *
  *
 * @returns  packet socket fd
 *
  *********************************************************************/
int ofdpaClientPktSockFdGet(void);

/*****************************************************************//**
 * @brief  The client calls this function to retrieve a single packet that the hardware
 *           has punted to the CPU.
 *
 * @param[in]    timeout    If NULL, the function blocks until a packet is received.
 *                                 If the timeout value is zero, the function returns immediately,
 *                                 whether or not a packet is available. The return code
 *                                 indicates whether a packet has been received. If the timeout
 *                                 value is non-zero, the function blocks up to this length of
 *                                 time if a packet is not immediately available. Again, the
 *                                 return code indicates whether a packet was received.
 * @param[out]    pkt       A packet structure allocated by the caller and used to
 *                                 return a received packet. The packet structure includes some
 *                                 metadata to indicate properties like why the packet came to
 *                                 the CPU and the ingress port. On input, the caller must set
 *                                 pkt-\>pktData.size to the size in bytes of the buffer allocated
 *                                 to hold the received packet. On output, pkt-\>pktData.size
 *                                 specifies the length of the packet in bytes. pkt-\>pktData.pstart
 *                                 on input must point to a buffer large enough to hold the largest
 *                                 possible received packet (OFDPA_MAX_PKT_LEN). This function copies
 *                                 the packet into this buffer, starting with the Ethernet header
 *                                 (the destination MAC address). The trailing Ethernet CRC is not
 *                                 included.
 *
 * @retval   OFDPA_E_NONE     if a packet is returned
 * @retval   OFDPA_E_TIMEOUT  if no packet is available within the specified timeout
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid (e.g., pkt is NULL)
 * @retval   OFDPA_E_FAIL     for any other failure
 *
 * @note    This function runs in the client's own context and is not an RPC API.
 *
  * ********************************************************************/
OFDPA_ERROR_t ofdpaPktReceive(struct timeval *timeout, ofdpaPacket_t *pkt);


/*****************************************************************//**
* @brief  Receive an event.
*
* @param[in]    timeout     time to wait for the event
*
* @retval   OFDPA_E_NONE        event received successfully
* @retval   OFDPA_E_FAIL        failure in socket creation, timeout configuration
*                               or event reception
* @retval   OFDPA_E_TIMEOUT     no event waiting to be received
*
*********************************************************************/
OFDPA_ERROR_t ofdpaEventReceive(struct timeval *timeout);

/*****************************************************************//**
* @brief  Get the next port event.
*
* @param[in,out]    eventData     event data
*
* @retval   OFDPA_E_NONE        port event data returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND   next port event not found
*
* @note     populate the port number in eventData to get the
*           event for the next port
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortEventNextGet(ofdpaPortEvent_t *eventData);

/*****************************************************************//**
* @brief  Get the port event.
*
* @param[in,out]    eventData     event data
*
* @retval   OFDPA_E_NONE        port event data returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND   next port event not found
*
* @note     get the event for the port
*
*********************************************************************/
OFDPA_ERROR_t ofdpaPortEventGet(ofdpaPortEvent_t *eventData);

/*****************************************************************//**
* @brief  Get the next flow event.
*
* @param[in,out]    eventData     event data
*
* @retval   OFDPA_E_NONE        flow event data returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND   next port event not found
*
* @note     populate the table id in eventData to get the
*           events for the flow table
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowEventNextGet(ofdpaFlowEvent_t *eventData);

/*****************************************************************//**
* @brief  Get the next OAM event.
*
* @param[in,out]    eventData     event data
*
* @retval   OFDPA_E_NONE        event data returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND   next port event not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamEventNextGet(ofdpaOamEvent_t *eventData);

/*------------------------------------------------------------------------------------*/
/* Table APIs */

/* Get information for a given table Id */

/*****************************************************************//**
* @brief  Get Flow Table Info.
*
* @param[in]    tableId     flow table Id
* @param[out]    info        flow table info
*
* @retval   OFDPA_E_NONE        table info returned successfully
* @retval   OFDPA_E_PARAM       error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND   table id not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaFlowTableInfoGet(OFDPA_FLOW_TABLE_ID_t tableId, ofdpaFlowTableInfo_t *info);

/*------------------------------------------------------------------------------------*/
/* Queue APIs */

/*****************************************************************//**
 * @brief  The client calls this function to get the number of COS
 *           queues on a port.
 *
 * @param[in]    portNum        Port number
 * @param[out]    numQueues     Number of Queues on a port
 *
 * @retval   OFDPA_E_NONE     if number of queues on a port is returned.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 * @retval   OFDPA_E_NOT_FOUND the port does not exist.
 *
  *
  *********************************************************************/
OFDPA_ERROR_t ofdpaNumQueuesGet(uint32_t portNum, uint32_t *numQueues);

/*****************************************************************//**
 * @brief  The client calls this function to get the statistics of a
 *           queue on a port.
 *
 * @param[in]    portNum        Port number
 * @param[in]    queueId        Queue ID
 * @param[out]    stats         Queue Statistics on a given port
 *
 * @retval   OFDPA_E_NONE     if the port queue statistics are returned.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 * @retval   OFDPA_E_NOT_FOUND the port does not exist.
 *
  *
  *********************************************************************/
OFDPA_ERROR_t ofdpaQueueStatsGet(uint32_t portNum, uint32_t queueId, ofdpaPortQueueStats_t *stats);

/*****************************************************************//**
 * @brief  The client calls this function to clear all the statistics of a
 *           queue on a port.
 *
 * @param[in]    portNum        Port number
 * @param[in]    queueId        Queue ID
 *
 * @retval   OFDPA_E_NONE     if the port queue statistics are cleared.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 * @retval   OFDPA_E_NOT_FOUND the port does not exist.
 *
  *
  *********************************************************************/
OFDPA_ERROR_t ofdpaQueueStatsClear(uint32_t portNum, uint32_t queueId);

/*****************************************************************//**
 * @brief  The client calls this function to set minimum and maximum
 *           bandwidth on a queue on a given port.
 *
 * @param[in]    portNum        Port number
 * @param[in]    queueId        Queue ID
 * @param[in]    minRate        Minimum bandwidth
 * @param[in]    maxRate        Maximum bandwidth
 *
 * @retval   OFDPA_E_NONE     if minimum and maximum bandwidths of a queue
 *                            are set successfully on a given port.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 * @retval   OFDPA_E_NOT_FOUND the port does not exist.
 *
  *
  *********************************************************************/
OFDPA_ERROR_t ofdpaQueueRateSet(uint32_t portNum, uint32_t queueId, uint32_t minRate, uint32_t maxRate);

/*****************************************************************//**
 * @brief  The client calls this function to get minimum and maximum
 *           bandwidth on a queue on a given port.
 *
 * @param[in]    portNum        Port number
 * @param[in]    queueId        Queue ID
 * @param[out]    minRate       Minimum bandwidth
 * @param[out]    maxRate       Maximum bandwidth
 *
 * @retval   OFDPA_E_NONE     if minimum and maximum bandwidths of a queue
 *                            are returned successfully on a given port.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid
 * @retval   OFDPA_E_NOT_FOUND the port does not exist.
 *
  *
  *********************************************************************/
OFDPA_ERROR_t ofdpaQueueRateGet(uint32_t portNum, uint32_t queueId, uint32_t *minRate, uint32_t *maxRate);

/*------------------------------------------------------------------------------------*/
/* Vendor Extension APIs */

/*****************************************************************//**
 * @brief  The client calls this function to set the Source Mac Learning
 *           Mode with other config parameters.
 *
 * @param[in]    mode                      Source MAC Learning Mode
 * @param[in]    srcMacLearnModeCfg        Config structure holding other parameters
 *                                                of Source MAC Learning
 *
 * @retval   OFDPA_E_NONE     if Source MAC Learning Mode is set successfully.
 * @retval   OFDPA_E_UNAVAIL  if any of the learning methods (Local/Controller managed)
 *                            is unsupported.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 *
 * @note    Enable mode: Copies the packets with unknown source address to CPU.
 * @note    Disable mode: Switches the packets with unknown source address.
 *
  *********************************************************************/
OFDPA_ERROR_t ofdpaSourceMacLearningSet(OFDPA_CONTROL_t mode, ofdpaSrcMacLearnModeCfg_t *srcMacLearnModeCfg);

/*****************************************************************//**
 * @brief  The client calls this function to get the Source Mac Learning
 *           Mode with other config parameters.
 *
 * @param[out]    mode                      Source MAC Learning Mode
 * @param[out]    srcMacLearnModeCfg        Config structure holding other parameters
 *                                                 of Source MAC Learning
 *
 * @retval   OFDPA_E_NONE     if Source MAC Learning Mode is returned successfully
 *                            on controller port.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 *
 *
  *********************************************************************/
OFDPA_ERROR_t ofdpaSourceMacLearningGet(OFDPA_CONTROL_t *mode, ofdpaSrcMacLearnModeCfg_t *srcMacLearnModeCfg);

/*****************************************************************//**
 * @brief  Initialize Flow entry structure.
 *
 * @param[in]    groupType         Group Type
 * @param[in,out]    group             Group entry structure
 *
 * @retval   OFDPA_E_NONE     if group entry structure is initialized successfully.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 *
  *********************************************************************/
OFDPA_ERROR_t ofdpaGroupEntryInit(OFDPA_GROUP_ENTRY_TYPE_t groupType, ofdpaGroupEntry_t *group);

/*****************************************************************//**
 * @brief  Initialize Flow entry structure.
 *
 * @param[in]    groupType         Group Type
 * @param[in,out]    bucket            Group bucket entry structure
 *
 * @retval   OFDPA_E_NONE     if group bucket entry structure is initialized successfully.
 * @retval   OFDPA_E_PARAM    if an input parameter is invalid.
 *
  *********************************************************************/
OFDPA_ERROR_t ofdpaGroupBucketEntryInit(OFDPA_GROUP_ENTRY_TYPE_t groupType, ofdpaGroupBucketEntry_t *bucket);

/*****************************************************************//**
* @brief  Add Meter.
*
* @param[in]    meterId   meter id
* @param[in]    meter     meter entry
*
* @retval   OFDPA_E_NONE  Meter added successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FULL  Meter table full
* @retval   OFDPA_E_EXISTS  Meter entry exists
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaMeterAdd(uint32_t meterId, ofdpaMeterEntry_t *meter);

/*****************************************************************//**
* @brief  Delete Meter.
*
* @param[in]    meterId     meter id
*
* @retval   OFDPA_E_NONE  Meter deleted successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  meter entry not found
* @retval   OFDPA_E_FAIL  failure; other errors
*
*********************************************************************/
OFDPA_ERROR_t ofdpaMeterDelete(uint32_t meterId);

/*****************************************************************//**
* @brief  Get the Meter entry.
*
* @param[in]    meterId     meter id
* @param[out]    meter       meter entry
*
* @retval   OFDPA_E_NONE  meter entry returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  next meter does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaMeterGet(uint32_t meterId, ofdpaMeterEntry_t *meter);

/*****************************************************************//**
* @brief  Get the next Meter entry.
*
* @param[in]    meterId     meter id
* @param[out]   nextMeterId   next meter entry
*
* @retval   OFDPA_E_NONE  next meter entry returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  next meter does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaMeterNextGet(uint32_t meterId, uint32_t *nextMeterId);

/*****************************************************************//**
* @brief  Get Meter statistics.
*
* @param[in]    meterId     meter id
* @param[out]    meterStats  meter statistics
*
* @retval   OFDPA_E_NONE  meter statistics returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  meter not found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaMeterStatsGet(uint32_t meterId, ofdpaMeterEntryStats_t *meterStats);

/*****************************************************************//**
* @brief  Initialize Meter entry.
*
* @param[in,out]    meter     meter entry
*
* @retval   OFDPA_E_PARAM error in parameters passed to function
*
*********************************************************************/
OFDPA_ERROR_t ofdpaMeterEntryInit(ofdpaMeterEntry_t *meter);

/*****************************************************************//**
* @brief  Add a Class Based counter to the counter table. Counter
*         identifier must be greater than 0.
*
* @param[in]    counterId   counter entry identifier
*
* @retval   OFDPA_E_NONE  counter added successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FULL  table full
* @retval   OFDPA_E_EXISTS  entry exists with ID specified
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaClassBasedCounterAdd(uint32_t counterId);

/*****************************************************************//**
* @brief  Delete a Class Based counter from the counter table.
*
* @param[in]    counterId   counter entry identifier
*
* @retval   OFDPA_E_NONE  counter deleted successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no entry exists with ID specified
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaClassBasedCounterDelete(uint32_t counterId);

/*****************************************************************//**
* @brief  Get the Class Based counter ID with the next higher value
*         from the counter table. To retrieve first counter, call with
*         counterId equal to 0.
*
* @param[in]    counterId      counter entry identifier
* @param[out]   nextCounterId  counter entry identifier with next higher value
*
* @retval   OFDPA_E_NONE  next counter ID found
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no next entry exists after ID specified
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaClassBasedCounterIdNextGet(uint32_t counterId, uint32_t *nextCounterId);

/*****************************************************************//**
* @brief  Get data for the Class Based counter set.
*
* @param[in]    counterId      counter entry identifier
* @param[out]   counterStats   counter entry data
*
* @retval   OFDPA_E_NONE  next counter ID found
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no entry exists with ID specified
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaClassBasedCounterStatsGet(uint32_t counterId, ofdpaClassBasedCounterStats_t *counterStats);

/*****************************************************************//**
* @brief  Add a Color Based counter to the counter table. Counter
*         identifier must be greater than 0.
*
* @param[in]    counterId   counter entry identifier
*
* @retval   OFDPA_E_NONE  counter added successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FULL  table full
* @retval   OFDPA_E_EXISTS  entry exists with ID specified
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaColorBasedCounterAdd(uint32_t counterId);

/*****************************************************************//**
* @brief  Delete a Color Based counter from the counter table.
*
* @param[in]    counterId   counter entry identifier
*
* @retval   OFDPA_E_NONE  counter deleted successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no entry exists with ID specified
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaColorBasedCounterDelete(uint32_t counterId);

/*****************************************************************//**
* @brief  Get the Color Based counter ID with the next higher value
*         from the counter table. To retrieve first counter, call with
*         counterId equal to 0.
*
* @param[in]    counterId      counter entry identifier
* @param[out]   nextCounterId  counter entry identifier with next higher value
*
* @retval   OFDPA_E_NONE  next counter ID found
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no next entry exists after ID specified
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaColorBasedCounterIdNextGet(uint32_t counterId, uint32_t *nextCounterId);

/*****************************************************************//**
* @brief  Get data for the Color Based counter set.
*
* @param[in]    counterId      counter entry identifier
* @param[out]   counterStats   counter entry data
*
* @retval   OFDPA_E_NONE  next counter ID found
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  no entry exists with ID specified
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaColorBasedCounterStatsGet(uint32_t counterId, ofdpaColorBasedCounterStats_t *counterStats);

/*****************************************************************//**
* @brief  Create an OAM MEG entry.
*
* @param[in]    megIndex   unique identifier for the MEG entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMegCreate(uint32_t megIndex, ofdpaOamMegConfig_t *config);

/*****************************************************************//**
* @brief  Delete an OAM MEG entry.
*
* @param[in]    megIndex   unique identifier for the MEG entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMegDelete(uint32_t megIndex);

/*****************************************************************//**
* @brief  Modify an OAM MEG entry.
*
* @param[in]    megIndex   unique identifier for the MEG entry
* @param[in]    newConfig  structure containing new configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMegModify(uint32_t megIndex, ofdpaOamMegConfig_t *newConfig);


/*****************************************************************//**
* @brief  Retrieve OAM MEG configuration and/or status.
*
* @param[in]    megIndex   unique identifier for the MEG entry
* @param[out]    config     structure containing configuration (optional)
* @param[out]    status     structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMegGet(uint32_t megIndex,
                             ofdpaOamMegConfig_t *config,
                             ofdpaOamMegStatus_t *status);

/*****************************************************************//**
* @brief  Return the MEG index, if any, for the
*           entry after the one specified.
*
* @param[in]    megIndex       specifies the starting MEG index
* @param[out]   nextMegIndex   unique identifier for the next
*                                      MEG entry of the given type
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMegNextGet(uint32_t megIndex, uint32_t *nextMegIndex);

/*****************************************************************//**
* @brief  Create an OAM MEP entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepCreate(uint32_t lmepId, ofdpaOamMepConfig_t *config);

/*****************************************************************//**
* @brief  Delete an OAM MEP entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepDelete(uint32_t lmepId);

/*****************************************************************//**
* @brief  Modify an OAM MEP entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    newConfig     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepModify(uint32_t lmepId, ofdpaOamMepConfig_t *newConfig);


/*****************************************************************//**
* @brief  Retrieve OAM MEP configuration and/or status.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]    config     structure containing configuration (optional)
* @param[out]    status     structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepGet(uint32_t lmepId,
                             ofdpaOamMepConfig_t *config,
                             ofdpaOamMepStatus_t *status);

/*****************************************************************//**
* @brief  Return the MEP identifier, if any, for the
*           entry after the one specified.
*
* @param[in]    lmepId      unique identifier for the MEP entry
* @param[out]    nextLmepId   unique identifier for the next
*                                   MEP entry on the MEG
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepNextGet(uint32_t lmepId, uint32_t *nextLmepId);

/*****************************************************************//**
* @brief  Create an OAM MIP entry.
*
* @param[in]    localMpId  unique identifier for the MIP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMipCreate(uint32_t localMpId, ofdpaOamMipConfig_t *config);

/*****************************************************************//**
* @brief  Delete an OAM MIP entry.
*
* @param[in]    localMpId  unique identifier for the MIP entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMipDelete(uint32_t localMpId);

/*****************************************************************//**
* @brief  Retrieve OAM MIP configuration and/or status.
*
* @param[in]    localMpId unique identifier for the MIP entry
* @param[out]   config    structure containing configuration (optional)
* @param[out]   status    structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMipGet(uint32_t localMpId,
                             ofdpaOamMipConfig_t *config,
                             ofdpaOamMipStatus_t *status);

/*****************************************************************//**
* @brief  Return the MIP identifier, if any, for the
*         entry after the one specified.
*
* @param[in]    localMpId      unique identifier for the current MIP entry
* @param[out]   nextLocalMpId  unique identifier for the next MIP entry
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMipNextGet(uint32_t localMpId, uint32_t *nextLocalMpId);

/*****************************************************************//**
* @brief  Create an OAM Remote MP entry.
*
* @param[in]    remoteMpId unique identifier for the remote MP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamRemoteMpCreate(uint32_t remoteMpId, ofdpaOamRemoteMpConfig_t *config);
OFDPA_ERROR_t ofdpaOamRemoteMpFind(uint32_t remoteMpId,
                                   ofdpaOamRemoteMpConfig_t *config);

/*****************************************************************//**
* @brief  Delete an OAM Remote MP entry.
*
* @param[in]    remoteMpId unique identifier for the remote MP entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamRemoteMpDelete(uint32_t remoteMpId);

/*****************************************************************//**
* @brief  Retrieve OAM Remote MP.
*
* @param[in]    remoteMpId   unique identifier for the remote MP entry
* @param[out]   config       structure containing configuration (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamRemoteMpGet(uint32_t remoteMpId,
                                  ofdpaOamRemoteMpConfig_t *config);

/*****************************************************************//**
* @brief  Return the Remote MP identifier, if any, for the
*         entry after the one specified for the given local MEP ID.
*
* @param[in]    remoteMpId      unique identifier for the remote MP entry
* @param[out]   nextRemoteMpId  unique identifier for the next remote MP entry
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamRemoteMpNextGet(uint32_t remoteMpId,
                                      uint32_t *nextRemoteMpId);

/*****************************************************************//**
* @brief  Retrieve OAM MEP CCM database entry status.
*
* @param[in]    lmepId      unique identifier for the MEP entry
* @param[in]    remoteMepId unique identifier for the remote MEP entry
* @param[out]    dbEntry     structure containing entry (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_INTERNAL internal error 
*
* @note If the caller does not require the data in the entry,
*        this parameter may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepCCMDatabaseEntryGet(uint32_t lmepId,
                                             uint32_t remoteMepId,
                                             ofdpaOamCcmDatabaseEntry_t *dbEntry);

/*****************************************************************//**
* @brief  Return the OAM MEP CCM database entry identifier, if any, for the
*           entry after the specified remote MEP.
*
* @param[in]    lmepId          unique identifier for the MEP entry
* @param[in]    remoteMepId     unique identifier for the remote MEP entry
* @param[out]   nextRemoteMepId unique identifier for the next remote MEP entry
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepCCMDatabaseEntryNextGet(uint32_t lmepId,
                                                 uint32_t remoteMepId,
                                                 uint32_t *nextRemoteMepId);

/*****************************************************************//**
* @brief  Retrieve OAM MEP LT Reply entry status.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    index      unique identifier for the entry
* @param[out]    ltrEntry   structure containing entry (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the entry,
*        this parameter may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepLtrEntryGet(uint32_t lmepId,
                                     uint32_t index,
                                     ofdpaOamLtrEntry_t *ltrEntry);

/*****************************************************************//**
* @brief  Return the OAM MEP LT Reply entry identifier, if any, for the
*           entry after the one specified.
*
* @param[in]    lmepId      unique identifier for the MEP entry
* @param[in]    index       unique identifier for the entry
* @param[out]    nextIndex   unique identifier for the next entry
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepLtrEntryNextGet(uint32_t lmepId,
                                         uint32_t index,
                                         uint32_t *nextIndex);

/*****************************************************************//**
* @brief  Start an on demand Loss Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     data to configure the session
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOnDemandLmStart(uint32_t lmepId,
                                      ofdpaOamOdLmConfig_t *config);

/*****************************************************************//**
* @brief  Start an on demand Delay Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     data to configure the session
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOnDemandDmStart(uint32_t lmepId,
                                      ofdpaOamOdDmConfig_t *config);

/*****************************************************************//**
* @brief  Create an MPLS-TP Linear Protection Group entry.
*
* @param[in]    index     unique identifier for the entry
* @param[in]    config    data to configure the entry
*
* @retval   OFDPA_E_NONE  entry added to table
* @retval   OFDPA_E_PARAM incorrect parameter (e.g., NULL pointer)
* @retval   OFDPA_E_FAIL  error adding entry to table
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMLPGroupCreate(uint32_t index, ofdpaOamMLPGroupConfig_t *config);

/*****************************************************************//**
* @brief  Delete an MPLS-TP Linear Protection Group entry.
*
* @param[in]    index     unique identifier for the entry
*
* @retval   OFDPA_E_NONE  entry deleted from table
* @retval   OFDPA_E_NOT_FOUND specified entry not found
* @retval   OFDPA_E_FAIL  error adding entry to table
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMLPGroupDelete(uint32_t index);

/*****************************************************************//**
* @brief  Retrieve an MPLS-TP Linear Protection Group entry.
*
* @param[in]    index      unique identifier for the entry
* @param[out]    config     structure containing configuration (optional)
* @param[out]    status     structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMLPGroupGet(uint32_t index,
                                  ofdpaOamMLPGroupConfig_t *config,
                                  ofdpaOamMLPGroupStatus_t *status);

/*****************************************************************//**
* @brief  Return the MPLS-TP Linear Protection Group entry identifier,
*           if any, for the entry after the one specified.
*
* @param[in]    index      unique identifier for the entry
* @param[out]    nextIndex  next entry identifier if found
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
* @retval   OFDPA_E_PARAM parameter error, such as passing a null pointer
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMLPGroupNextGet(uint32_t index, uint32_t *nextIndex);

/*****************************************************************//**
* @brief  Issue a request to the MPLS-TP Linear Protection Group.
*
* @param[in]     index      unique identifier for the entry
* @param[in]     request    type of request to issue
*
* @retval   OFDPA_E_NONE  request successfully initiated
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMLPGroupRequest(uint32_t index, OFDPA_PROTECTION_SWITCH_REQUEST_TYPE_t request);

/*****************************************************************//**
* @brief  Add a local MEP to the specfied MPLS-TP Linear Protection Group
*
* @param[in]    mlpGroupIndex  MLP Group entry index
* @param[in]    lmepId         unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  local MEP successfully added to protection group
* @retval   OFDPA_E_ERROR requested combination failed validation
* @retval   OFDPA_E_FAIL  failure adding local MEP to protection group
* @retval   OFDPA_E_NOT_FOUND  either the MLP group or local MEP entry specified is not configured
* @retval   OFDPA_E_EXISTS  an entry for this combination exists
* @retval   OFDPA_E_FULL  the table storing these entries is full
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepProtectionAdd(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg);

/*****************************************************************//**
* @brief  Remove a local MEP to the specfied MPLS-TP Linear Protection Group.
*
* @param[in]    mlpGroupIndex  MLP Group entry index
* @param[in]    lmepId         unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  local MEP successfully removed from protection group
* @retval   OFDPA_E_FAIL  failure adding local MEP to protection group
* @retval   OFDPA_E_NOT_FOUND  either the MLP group or local MEP entry specified is not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepProtectionRemove(uint32_t mlpGroupIndex, uint32_t lmepId);

/*****************************************************************//**
* @brief  Checks whether local MEP is associated with this MPLS-TP Linear Protection Group.
*
* @param[in]    mlpGroupIndex  MLP Group entry index
* @param[in]    lmepId         unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  local MEP is associated with this protection group
* @retval   OFDPA_E_NOT_FOUND  local MEP is not associated with this protection group
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepProtectionGet(uint32_t mlpGroupIndex, uint32_t lmepId);

/*****************************************************************//**
* @brief  Returns the index of the next local MEP associated with this MPLS-TP Linear Protection
*         Group, if any exists.
*
* @param[in]    mlpGroupIndex  MLP Group entry index
* @param[in]    lmepId         unique identifier for the MEP entry
* @param[out]   nextLmepId     storage for value of next MEP entry identifier, if any
*
* @retval   OFDPA_E_NONE  next identifier found
* @retval   OFDPA_E_FAIL  no next identifier found
* @retval   OFDPA_E_PARAM parameter error, such as passing a null pointer
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamMepProtectionNextGet(uint32_t mlpGroupIndex, uint32_t lmepId, uint32_t *nextLmepId);

/*****************************************************************//**
* @brief  Add drop status entry.
*
* @param[in]    dropEntry     drop entry
*
* @retval   OFDPA_E_NONE  drop entry added successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FULL  Drop Status table full
* @retval   OFDPA_E_EXISTS  drop entry exists
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaDropStatusAdd(ofdpaDropStatusEntry_t *dropEntry);

/*****************************************************************//**
* @brief  Delete drop status entry.
*
* @param[in]    lmepId      lmep id
*
* @retval   OFDPA_E_NONE  drop entry deleted successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  drop entry not found
* @retval   OFDPA_E_FAIL  failure; other errors
*
*********************************************************************/
OFDPA_ERROR_t ofdpaDropStatusDelete(uint32_t lmepId);

/*****************************************************************//**
* @brief  Get drop status entry.
*
* @param[in]    lmepId        lmep id
* @param[out]    dropEntry     drop entry
*
* @retval   OFDPA_E_NONE  drop entry returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  drop entry does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaDropStatusGet(uint32_t lmepId, ofdpaDropStatusEntry_t *dropEntry);

/*****************************************************************//**
* @brief  Get the next drop entry.
*
* @param[in]    lmepId          lmep id
* @param[out]    nextDropEntry   next drop entry
*
* @retval   OFDPA_E_NONE  next drop entry returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  next drop does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaDropStatusNextGet(uint32_t lmepId, ofdpaDropStatusEntry_t *nextDropEntry);

/*****************************************************************//**
* @brief  Get drop action value
*
* @param[in]    lmepId        lmep id
* @param[out]    dropAction     drop action (drop or don't)
*
* @retval   OFDPA_E_NONE  drop action returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  drop entry does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaDropStatusActionGet(uint32_t lmepId, uint8_t *dropAction);

/*****************************************************************//**
* @brief  Initialize drop entry.
*
* @param[in,out]    dropEntry     drop entry
*
* @retval   OFDPA_E_PARAM error in parameters passed to function
*
*********************************************************************/
OFDPA_ERROR_t ofdpaDropStatusEntryInit(ofdpaDropStatusEntry_t *dropEntry);

/*****************************************************************//**
* @brief  Add data counter entry.
*
* @param[in]    lmepId        lmep id
* @param[in]    trafficClass  traffic class
*
* @retval   OFDPA_E_NONE  data counter entry added successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FULL  Data Counter table full
* @retval   OFDPA_E_EXISTS  data counter entry exists
* @retval   OFDPA_E_INTERNAL  internal error
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamDataCounterAdd(uint32_t lmepId, uint8_t trafficClass);

/*****************************************************************//**
* @brief  Delete data counter entry.
*
* @param[in]    lmepId        lmep id
* @param[in]    trafficClass  traffic class
*
* @retval   OFDPA_E_NONE  data counter entry deleted successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND  data counter entry not found
* @retval   OFDPA_E_FAIL  failure; other errors
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamDataCounterDelete(uint32_t lmepId, uint8_t trafficClass);

/*****************************************************************//**
* @brief  Get data counter entry status.
*
* @param[in]    lmepId         lmep id
* @param[in]    trafficClass   traffic class
* @param[out]   status         entry status
*
* @retval   OFDPA_E_NONE  data counter entry status returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  data counter entry does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamDataCounterStatusGet(uint32_t lmepId, uint8_t trafficClass, ofdpaOamDataCounterStatus_t *status);

/*****************************************************************//**
* @brief  Get the data counter entry.
*
* @param[in]    index           lmep id and traffic class index
* @param[out]    status         entry status
*
* @retval   OFDPA_E_NONE  next data counter entry returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  data counter does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamDataCounterGet(ofdpaOamDataCounterIndex_t index, ofdpaOamDataCounterStatus_t *status);
/*****************************************************************//**
* @brief  Get the next data counter entry.
*
* @param[in]    index           lmep id and traffic class index
* @param[out]    nextIndex       index of the next data couter entry
* @param[out]    status          next entry status
*
* @retval   OFDPA_E_NONE  next data counter entry returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_FAIL  next data counter does not exist
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamDataCounterNextGet(ofdpaOamDataCounterIndex_t index, ofdpaOamDataCounterIndex_t *nextIndex,
                                        ofdpaOamDataCounterStatus_t *status);

/*****************************************************************//**
* @brief  Get OAM LM counters.
*
* @param[in]    index          lmep id and traffic class index
* @param[out]   TxFCl          tx counters
* @param[out]   RxFCl          rx counters
*
* @retval   OFDPA_E_NONE  LM counters returned successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamDataCountersLMGet(ofdpaOamDataCounterIndex_t index, uint32_t *TxFCl, uint32_t *RxFCl);

/*****************************************************************//**
* @brief  Clear OAM LM counters.
*
* @param[in]    index          lmep id and traffic class index
*
* @retval   OFDPA_E_NONE  LM counters cleared successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamDataCountersLMClear(ofdpaOamDataCounterIndex_t index);

/*****************************************************************//**
* @brief  Create an OAM Loopback Entry
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
***********************************************************************/
OFDPA_ERROR_t ofdpaOamLbCreate(uint32_t lmepId, ofdpaOamLbConfig_t *config);

/*****************************************************************//**
* @brief  Start on demand loopback
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  entry successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with id not configured
*
***********************************************************************/
OFDPA_ERROR_t ofdpaOamLbStart(uint32_t lmepId);

/*****************************************************************//**
* @brief  Stop on demand loopback
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  entry successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with id not configured
*
***********************************************************************/
OFDPA_ERROR_t ofdpaOamLbStop(uint32_t lmepId);

/*****************************************************************//**
* @brief  Get the status of loopback
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]    config     structure containing configuration parameters
* @param[out]    status     structure containing status parameters
*
* @retval   OFDPA_E_NONE  entry successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with id not configured
*
***********************************************************************/
OFDPA_ERROR_t ofdpaOamLbGet(uint32_t lmepId, ofdpaOamLbConfig_t *config, ofdpaOamLbStatus_t *status);

/*****************************************************************//**
* @brief  Create an OAM Performance Measurement Proactive Loss
* Measurement entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProLmCreate(uint32_t lmepId, ofdpaOamProLmConfig_t *config);

/*****************************************************************//**
* @brief  Delete an OAM Performance Measurement Proactive Loss
* Measurement entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProLmDelete(uint32_t lmepId);

/*****************************************************************//**
* @brief  Retrieve OAM Performance Measurement Proactive Loss
* Measurement configuration and/or status.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]    config     structure containing configuration (optional)
* @param[out]    status     structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProLmGet(uint32_t lmepId, ofdpaOamProLmConfig_t *config,
                               ofdpaOamProLmStatus_t *status);

/*****************************************************************//**
* @brief  Retrieve OAM Performance Measurement Proactive Loss
* Measurement counters.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]   counters   structure containing counters
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProLmCountersGet(uint32_t lmepId, ofdpaOamProLmCounters_t *counters);

/*****************************************************************//**
* @brief  Create an OAM Performance Measurement Proactive Delay
* Measurement entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProDmCreate(uint32_t lmepId, ofdpaOamProDmConfig_t *config);

/*****************************************************************//**
* @brief  Delete an OAM Performance Measurement Proactive Delay
* Measurement entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProDmDelete(uint32_t lmepId);

/*****************************************************************//**
* @brief  Retrieve OAM Performance Measurement Proactive Delay
* Measurement configuration and/or status.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]    config     structure containing configuration (optional)
* @param[out]    status     structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProDmGet(uint32_t lmepId, ofdpaOamProDmConfig_t *config,
                               ofdpaOamProDmStatus_t *status);

/*****************************************************************//**
* @brief  Retrieve OAM Performance Measurement Proactive Delay
* Measurement counters.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]   counters   structure containing counters
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProDmCountersGet(uint32_t lmepId, ofdpaOamProDmCounters_t *counters);

/*****************************************************************//**
* @brief  Create an OAM Performance Measurement OnDemand Loss
* Measurement entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdLmCreate(uint32_t lmepId, ofdpaOamOdLmConfig_t *config);

/*****************************************************************//**
* @brief  Delete an OAM Performance Measurement OnDemand Loss
* Measurement entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdLmDelete(uint32_t lmepId);

/*****************************************************************//**
* @brief  Retrieve OAM Performance Measurement Ondemand Loss
* Measurement configuration and/or status.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]    config     structure containing configuration (optional)
* @param[out]    status     structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdLmGet(uint32_t lmepId, ofdpaOamOdLmConfig_t *config,
                              ofdpaOamOdLmStatus_t *status);

/*****************************************************************//**
* @brief  config OAM Performance Measurement OnDemand Loss
* Measurement .
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]   config   structure containing config
*
* @retval   OFDPA_E_NONE  successfully
* @retval   other fail
*
*********************************************************************/

OFDPA_ERROR_t ofdpaOamOdLmSet(uint32_t lmepId, ofdpaOamOdLmConfig_t *config);


/*****************************************************************//**
* @brief  Retrieve OAM Performance Measurement OnDemand Loss
* Measurement counters.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]   counters   structure containing counters
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdLmCountersGet(uint32_t lmepId, ofdpaOamOdLmSnapshot_t *counters);

/*****************************************************************//**
* @brief  Create an OAM Performance Measurement OnDemand Delay
* Measurement entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdDmCreate(uint32_t lmepId, ofdpaOamOdDmConfig_t *config);

/*****************************************************************//**
* @brief  Delete an OAM Performance Measurement OnDemand Delay
* Measurement entry.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  entry successfully deleted
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
* @retval   OFDPA_E_FAIL  failure occurred during deletion
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdDmDelete(uint32_t lmepId);

/*****************************************************************//**
* @brief  Retrieve OAM Performance Measurement OnDemand Delay
* Measurement configuration and/or status.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]    config     structure containing configuration (optional)
* @param[out]    status     structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdDmGet(uint32_t lmepId, ofdpaOamOdDmConfig_t *config,
                              ofdpaOamOdDmStatus_t *status);


/*****************************************************************//**
* @brief  config OAM Performance Measurement OnDemand Delay
* Measurement .
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]   config   structure containing config
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   other fail
*
*********************************************************************/

OFDPA_ERROR_t ofdpaOamOdDmSet(uint32_t lmepId, ofdpaOamOdDmConfig_t *config);

/*****************************************************************//**
* @brief  Retrieve OAM Performance Measurement OnDemand Delay
* Measurement counters.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]   counters   structure containing counters
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdDmCountersGet(uint32_t lmepId, ofdpaOamOdDmSnapshot_t *counters);

/*****************************************************************//**
* @brief  Start OAM Performance Measurement OnDemand Loss
* Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdLmStart(uint32_t lmepId);

/*****************************************************************//**
* @brief  Stop OAM Performance Measurement OnDemand Loss
* Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdLmStop(uint32_t lmepId);

/*****************************************************************//**
* @brief  Start OAM Performance Measurement OnDemand Delay
* Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdDmStart(uint32_t lmepId);

/*****************************************************************//**
* @brief  Stop OAM Performance Measurement OnDemand Delay
* Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamOdDmStop(uint32_t lmepId);

/*****************************************************************//**
* @brief  Start OAM Performance Measurement Proactive Delay
* Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProDmStart(uint32_t lmepId);

/*****************************************************************//**
* @brief  Stop OAM Performance Measurement Proactive Delay
* Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProDmStop(uint32_t lmepId);

/*****************************************************************//**
* @brief  Enable OAM Performance Measurement Proactive Loss
* Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProLmEnable(uint32_t lmepId);

/*****************************************************************//**
* @brief  Disable OAM Performance Measurement Proactive Loss
* Measurement session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamProLmDisable(uint32_t lmepId);

/*****************************************************************//**
* @brief  Initiates a OAM Link Trace session.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  entry successfully created
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_ERROR configuration error
* @retval   OFDPA_E_FAIL  failure occurred during creation
* @retval   OFDPA_E_FULL  maximum number of entries already created
* @retval   OFDPA_E_EXISTS an entry with the specified indices already created
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamLtStart(uint32_t lmepId, ofdpaOamLtConfig_t *config);

/*****************************************************************//**
* @brief  Retrieve OAM Link Trace configuration and/or status.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[out]    config     structure containing configuration (optional)
* @param[out]    status     structure containing status (optional)
*
* @retval   OFDPA_E_NONE  data successfully retrieved
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
* @note If the caller does not require the data in the configuration or
*        status structure, either or both parameters may be set to NULL.
*        An example of this type of invocation is if the caller is checking
*        on presence of the entry, but has no need for the configuration or
*        status data.
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamLtGet(uint32_t lmepId, ofdpaOamLtConfig_t *config, ofdpaOamLtStatus_t *status);

/*****************************************************************//**
* @brief  Retrieve Next OAM Link Trace record.
*
* @param[in]    lmepId       unique identifier for the MEP entry
* @param[in]    sequenceNum  sequence number for the link trace record
* @param[in]    receiveOrder receive order for the link trace record
* @param[out]   ltResult     structure containing result
*
* @retval   OFDPA_E_NONE   successfully retrieved next entry
* @retval   OFDPA_E_NOT_FOUND next entry not found
*
* @note For retrieving first record sequenceNum and receiveOrder should be
*       set to zero.
*********************************************************************/
OFDPA_ERROR_t ofdpaOamLtResultNextGet(uint32_t lmepId, uint32_t sequenceNum, uint32_t receiveOrder, ofdpaOamLtrEntry_t *ltResult);

/*****************************************************************//**
* @brief  Enable sending OAM Client Signal Fail (CSF) messages for the
*         specified MEP using the configuration parameters provided.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  CSF configured successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamCsfEnable(uint32_t lmepId, ofdpaOamCsfConfig_t *config);

/*****************************************************************//**
* @brief  Disable sending OAM Client Signal Fail (CSF) messages for the
*         specified MEP.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  CSF unconfigured successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamCsfDisable(uint32_t lmepId);

/*****************************************************************//**
* @brief  Enable sending OAM Alarm Indication Signal (AIS) messages for the
*         specified MEP using the configuration parameters provided.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  AIS configured successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamAisEnable(uint32_t lmepId, ofdpaOamAisConfig_t *config);

/*****************************************************************//**
* @brief  Disable sending OAM Alarm Indication Signal (AIS) messages for the
*         specified MEP.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  AIS unconfigured successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamAisDisable(uint32_t lmepId);

/*****************************************************************//**
* @brief  Retrieve OAM Alarm Indication Signal configuration for the
*         specified MEP using the configuration parameters provided.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  AIS config retrieved successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamAisConfigGet(uint32_t lmepId, ofdpaOamAisConfig_t *config);

/*****************************************************************//**
* @brief  Enable sending OAM Lock Signal (LCK) messages for the
*         specified MEP using the configuration parameters provided.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  Lock Signal configured successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamLckEnable(uint32_t lmepId, ofdpaOamLckConfig_t *config);

/*****************************************************************//**
* @brief  Disable sending OAM Lock Signal (LCK) messages for the
* @brief  Disable sending OAM Alarm Indication Signal (AIS) messages for the
*         specified MEP.
*
* @param[in]    lmepId     unique identifier for the MEP entry
*
* @retval   OFDPA_E_NONE  Lock Signal unconfigured successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamLckDisable(uint32_t lmepId);

/*****************************************************************//**
* @brief  Retrieve OAM Lock Signal configuration for the
*         specified MEP using the configuration parameters provided.
*
* @param[in]    lmepId     unique identifier for the MEP entry
* @param[in]    config     structure containing configuration parameters
*
* @retval   OFDPA_E_NONE  Lock Signal config retrieved successfully
* @retval   OFDPA_E_PARAM error in parameters passed to function
* @retval   OFDPA_E_NOT_FOUND entry with identifier not configured
*
*********************************************************************/
OFDPA_ERROR_t ofdpaOamLckConfigGet(uint32_t lmepId, ofdpaOamLckConfig_t *config);



/*****************************************************************//**
* @brief  inband manage vlan and ip config
*
* @param[in]    vlan     manage vlan
* @param[in]    ip         ip address, for example: 0xc0,0xa8,0x1,0xfe --> 192.168.1.254
* @param[in]     ipMask   netmask
*
* @retval   OFDPA_E_NONE  successfully
* @retval   other			 fail
*
*********************************************************************/

OFDPA_ERROR_t ofdpaInbandMngAdd(int vlan, unsigned char ip[4], unsigned char ipMask[4]);


/*****************************************************************//**
* @brief  	delete inband manage vlan and ip 
*
* @param[in]    vlan     manage vlan
* @param[in]    ip     ip address, for example: 0xc0,0xa8,0x1,0xfe --> 192.168.1.254
*
* @retval   OFDPA_E_NONE  successfully
* @retval   other			 fail
*
*********************************************************************/

OFDPA_ERROR_t ofdpaInbandMngDelete(int vlan, unsigned char ip[4]);


/*****************************************************************//**
* @brief  dhcp packet to cpu config
*
* @param[in]    enable     1: enable, 0 disable
*
* @retval   OFDPA_E_NONE  successfully
* @retval   other			 fail
*
*********************************************************************/
	
OFDPA_ERROR_t ofdpaFlowDhcpAdd(uint32_t enable, uint32_t inports);
OFDPA_ERROR_t ofdpaFlowDhcpAddByPort(uint32_t enable, uint32_t inports);
OFDPA_ERROR_t ofdpaFlowDhcpAddByPorts(uint32_t inports);
OFDPA_ERROR_t ofdpaFlowDhcpClientEna(uint32_t enable);

/*****************************************************************//**
* @brief  reconnect packet to cpu config
*
* @param[in]    enable     1: enable, 0 disable
*
* @retval   OFDPA_E_NONE  successfully
* @retval   other			 fail
*
*********************************************************************/
	
OFDPA_ERROR_t ofdpaFlowDhcpReConnectAdd(uint32_t enable);

/*****************************************************************//**
* @brief  add port to a vlan
*
* @param[in]    enable     1: enable, 0 disable
*
* @retval   OFDPA_E_NONE  successfully
* @retval   other			 fail
*
*********************************************************************/
	
OFDPA_ERROR_t ofdpaFlowVlanPortAdd(uint32_t enable, uint32_t inport, uint32_t vlan);

OFDPA_ERROR_t ofdpaBfdCreate(uint32_t lmepId, ofdpaBfdConfig_t *config);
OFDPA_ERROR_t ofdpaBfdDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdpaBfdGet(uint32_t lmepId,
                             ofdpaBfdConfig_t *config,
                             ofdpaBfdStatus_t *status);
OFDPA_ERROR_t ofdpaBfdNextGet(uint32_t lmepId, uint32_t *nextLmepId);

OFDPA_ERROR_t ofdpaPwRedunCreate(uint32_t index, ofdpaPwRedunConfig_t *config);
OFDPA_ERROR_t ofdpaPwRedunDelete(uint32_t index);
OFDPA_ERROR_t ofdpaPwRedunGet(uint32_t index,
                                  ofdpaPwRedunConfig_t *config,
                                  ofdpaPwRedunStatus_t *status);
OFDPA_ERROR_t ofdpaBfdProtectionAdd(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg);
OFDPA_ERROR_t ofdpaBfdProtectionRemove(uint32_t mlpGroupIndex, uint32_t lmepId);
OFDPA_ERROR_t ofdpaBfdProtectionGet(uint32_t mlpGroupIndex, uint32_t lmepId);




OFDPA_ERROR_t ofdpaY1564Create(uint32_t lmepId);
OFDPA_ERROR_t ofdpaY1564Delete(uint32_t lmepId);
OFDPA_ERROR_t ofdpaY1564NextGet(uint32_t lmepId, uint32_t *nextLmepId);
OFDPA_ERROR_t ofdpaY1564Get(uint32_t lmepId,
                             ofdpaY1564Status_t *status);

OFDPA_ERROR_t ofdpaLoopbackCreate(uint32_t lmepId);
OFDPA_ERROR_t ofdpaLoopbackDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdpaLoopbackNextGet(uint32_t lmepId, uint32_t *nextLmepId);
OFDPA_ERROR_t ofdpaLoopbackGet(uint32_t lmepId,
                             ofdpaY1564Status_t *status);

OFDPA_ERROR_t ofdpaLmepidNextGet(uint32_t lmepId, uint32_t *nextLmepId);
OFDPA_ERROR_t ofdpaLmepidGet(uint32_t lmepId,
                             uint32_t *oamtype);

OFDPA_ERROR_t ofdpaVlanFlowGet(uint32_t port, uint32_t vlan);
OFDPA_ERROR_t ofdpaVlanPortGet(uint32_t port,
                             uint32_t *vlan);


/* Development utility APIs */
/*****************************************************************//**
* @brief  Set the Debug Verbosity
*
* @param[in]    lvl      verbosity level (ofdpaDebugLevels_t)
*
* @returns  0     success
*
*********************************************************************/
int ofdpaDebugLvl(int lvl);

/*****************************************************************//**
* @brief  Get the Debug Verbosity
*
*
* @returns  lvl     verbosity level (ofdpaDebugLevels_t)
*
*********************************************************************/
int ofdpaDebugLvlGet(void);

/*****************************************************************//**
* @brief  Get the Component Name
*
* @param[in]    component      component number
* @param[out]    name          component name
*
* @retval   OFDPA_E_NONE     success
* @retval   OFDPA_E_FAIL     failure to get component
*
*********************************************************************/
OFDPA_ERROR_t ofdpaComponentNameGet(int component, ofdpa_buffdesc *name);

/*****************************************************************//**
* @brief  Enable/Disable the Component for debugging
*
* @param[in]    component     component number (ofdpaComponentIds_t)
* @param[in]    enable       debug mode (ofdpaDebugLevels_t)
*
* @returns  0     success
* @returns  1     failure
*
*********************************************************************/
int ofdpaDebugComponentSet(int component, int enable);

/*****************************************************************//**
* @brief  Get Component debugging mode
*
* @param[in]    component     component number (ofdpaComponentIds_t)
*
* @returns  debug mode
*
*********************************************************************/
int ofdpaDebugComponentGet(int component);

/*****************************************************************//**
* @brief  Execute a BCM command
*
* @param[in]    buffer     BCM command
*
* @returns  0     success
* @returns  \<0    failure
*
*********************************************************************/
int ofdpaBcmCommand(ofdpa_buffdesc buffer);

OFDPA_ERROR_t ofdpaDevshellCommand(ofdpa_buffdesc buffer);




/** Action list */
uint64_t ofdpaActAllowVlanTrans(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActCpyMplsExpOutwards(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActCpyMplsTtlOutwards(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActIdentifyOutPort(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActIncClassBasedCounter(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActIncColorBasedCount(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActOamLmTxCount(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActPopVlan(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActPushL2Hdr(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActPushMplsCw(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActPushMplsHdr(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActPushVlan(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActPushVlan2(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetDstMac(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetEtherType(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetGrpId(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetLmepId(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMetaDataMplsL2Port(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMetaDataMplsType(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMetaDataOvid(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMetaDataTunId(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMetaDataVrf(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMplsBos(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMplsExp(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMplsLabel(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetMplsTtl(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetQosIndex(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetRemarkTableId(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetSrcMac(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetTpid(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetVlanId(void *this,void *pcb, uint64_t arg);
uint64_t ofdpaActSetVlanId2(void *this,void *pcb, uint64_t arg);


#endif /* INCLUDE_OFDPA_API_H */
/*!  @}
 */
