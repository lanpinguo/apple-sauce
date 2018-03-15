/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2013-2016
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
* @filename     ind_ofdpa_groups.c
*
* @purpose      OF-DPA Driver for Indigo
*
* @component    OF-DPA
*
* @comments     none
*
* @create       6 Nov 2013
*
* @end
*
**********************************************************************/

#include "indigo/forwarding.h"
#include "ind_ofdpa_util.h"
#include "ind_ofdpa_log.h"

#define IND_OFDPA_BUKT_ACT_APPEND(pBukt,actFn,actArg) \
do { \
	int i; \
	i = pBukt->act_cnt; \
	if((i + 1) >= IND_OFDPA_GRP_BUKT_ACT_MAX){ \
		return INDIGO_ERROR_PARAM; \
	} \
	pBukt->actions[i].act = actFn; \
	pBukt->actions[i].arg = actArg;\
	pBukt->act_cnt++;\
}while(0) 



static indigo_error_t
ind_ofdpa_translate_group_actions(of_list_action_t *actions,
                                  ind_ofdpa_group_bucket_t *group_bucket,
                                  uint64_t *group_action_bitmap,
                                  uint64_t *group_action_sf_bitmap)
{
    of_action_t act;
    int rv;
    uint16_t vlanId;
    of_oxm_t oxm;

    OF_LIST_ACTION_ITER(actions, &act, rv) {
        switch (act.header.object_id) {
        case OF_ACTION_OUTPUT: {
            of_port_no_t port_no;
            of_action_output_port_get(&act.output, &port_no);
            switch (port_no) {
                case OF_PORT_DEST_CONTROLLER:
                case OF_PORT_DEST_FLOOD:
                case OF_PORT_DEST_ALL:
                case OF_PORT_DEST_USE_TABLE:
                case OF_PORT_DEST_LOCAL:
                case OF_PORT_DEST_IN_PORT:
                case OF_PORT_DEST_NORMAL:
                    LOG_ERROR("unsupported output port 0x%x", port_no);
                    return INDIGO_ERROR_COMPAT;
                default:
                    //group_bucket->outputPort = port_no;
                    IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActIdentifyOutPort,port_no);
                    *group_action_bitmap |= IND_OFDPA_OUTPUT;
                  break;
            }
            break;
        }
        case OF_ACTION_SET_FIELD: {
            /* FIXUP: loci does not yet support the OXM field in the set-field action */
            uint64_t tmp;
            of_oxm_header_init(&oxm.header, act.header.version, 0, 1);
            oxm.header.wire_object = act.header.wire_object;
            oxm.header.wire_object.obj_offset += 4; /* skip action header */
            oxm.header.parent = &act.header;
            of_object_wire_init(&oxm.header, OF_OXM, 0);
            if (oxm.header.length == 0) {
                LOG_ERROR("failed to parse set-field action");
                return INDIGO_ERROR_COMPAT;
            }
            switch (oxm.header.object_id) {
                case OF_OXM_VLAN_VID:
                    of_oxm_vlan_vid_value_get(&oxm.vlan_vid, &vlanId);
                    vlanId &= (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
                    IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetVlanId,vlanId);
                    *group_action_sf_bitmap |= IND_OFDPA_VLANID;
                    break;

                case OF_OXM_ETH_SRC:
                    of_oxm_eth_src_value_get(&oxm.eth_src, &tmp);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetSrcMac,tmp);
                    *group_action_sf_bitmap |= IND_OFDPA_SRCMAC;
                    break;

                case OF_OXM_ETH_DST:
                    of_oxm_eth_dst_value_get(&oxm.eth_dst, &tmp);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetDstMac,tmp);
                    *group_action_sf_bitmap |= IND_OFDPA_DSTMAC;
                    break;

                case OF_OXM_MPLS_LABEL:
                    of_oxm_mpls_label_value_get(&oxm.mpls_label, &tmp);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetMplsLabel,tmp);
                    *group_action_sf_bitmap |= IND_OFDPA_MPLS_LABEL;
                    break;

                case OF_OXM_VLAN_PCP:
                {
                    uint8_t pcp;
                    of_oxm_vlan_pcp_value_get(&oxm.vlan_pcp, &pcp);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetVlanPcp,pcp);
                    *group_action_sf_bitmap |= IND_OFDPA_VLAN_PCP;
                    break;
                }
                case OF_OXM_OFDPA_DEI:
                {
                    uint8_t dei;
                    of_oxm_ofdpa_dei_value_get(&oxm.ofdpa_dei, &dei);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetVlanDei,dei);
                    *group_action_sf_bitmap |= IND_OFDPA_VLAN_DEI;
                    break;
                }
                case OF_OXM_IP_DSCP:
                {
                    uint8_t dscp;
                    of_oxm_ip_dscp_value_get(&oxm.ip_dscp, &dscp);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetDscp,dscp);
                    *group_action_sf_bitmap |= IND_OFDPA_IP_DSCP;
                    break;
                }
                case OF_OXM_MPLS_BOS:
                {
                    uint8_t bos;
                    of_oxm_mpls_bos_value_get(&oxm.mpls_bos, &bos);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetMplsBos,bos);
                    *group_action_sf_bitmap |= IND_OFDPA_MPLS_BOS;
                    break;
                }
                case OF_OXM_MPLS_TC:
                {
                    uint8_t tc;
                    of_oxm_mpls_tc_value_get(&oxm.mpls_tc, &tc);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetMplsExp,tc);
                    *group_action_sf_bitmap |= IND_OFDPA_MPLS_TC;
                    break;
                }
                case OF_OXM_OFDPA_MPLS_TTL:
                {
                    uint8_t ttl;
                    of_oxm_ofdpa_mpls_ttl_value_get(&oxm.ofdpa_mpls_ttl, &ttl);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetMplsTtl,ttl);
                    *group_action_sf_bitmap |= IND_OFDPA_MPLS_TTL;
                    break;
                }
                case OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION:
                {
                    uint8_t allowVlanTranslation;
                    of_oxm_ofdpa_allow_vlan_translation_value_get(&oxm.ofdpa_allow_vlan_translation, &allowVlanTranslation);
										IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActAllowVlanTrans,allowVlanTranslation);
                    *group_action_sf_bitmap |= IND_OFDPA_ALLOW_VLAN_TRANSLATION;
                    break;
                }
                default:
                    LOG_ERROR("unsupported set-field oxm %s", of_object_id_str[oxm.header.object_id]);
                    return INDIGO_ERROR_COMPAT;
            }
            break;
        }
        case OF_ACTION_SET_DL_SRC:
            break;

        case OF_ACTION_SET_DL_DST:
            break;

        case OF_ACTION_SET_VLAN_VID:
            break;

        case OF_ACTION_POP_VLAN:
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActPopVlan,1);
            *group_action_bitmap |= IND_OFDPA_POP_VLAN;
            break;
        case OF_ACTION_STRIP_VLAN:
            break;

        case OF_ACTION_PUSH_VLAN:
        {
            uint16_t eth_type;
            of_action_push_vlan_ethertype_get(&act.push_vlan, &eth_type);
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActPushVlan,eth_type);
            *group_action_bitmap |= IND_OFDPA_PUSH_VLAN;
            break;
        }
        case OF_ACTION_PUSH_MPLS:
        {
            uint16_t ether_type;
            of_action_push_mpls_ethertype_get(&act.push_mpls, &ether_type);
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActPushMplsHdr,ether_type);
            *group_action_bitmap |= IND_OFDPA_PUSH_MPLS;
            break;
        }
        case OF_ACTION_SET_MPLS_TTL:
        {
            uint8_t ttl;
            of_action_set_mpls_ttl_mpls_ttl_get(&act.push_mpls, &ttl);
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetMplsTtl,ttl);
            *group_action_bitmap |= IND_OFDPA_SET_MPLS_TTL;
            break;
        }
        case OF_ACTION_COPY_TTL_OUT:
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActCpyMplsTtlOutwards,1);
            *group_action_bitmap |= IND_OFDPA_COPY_TTL_OUT;
            break;

        case OF_ACTION_OFDPA_PUSH_L2HDR:
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActPushL2Hdr,1);
            *group_action_bitmap |= IND_OFDPA_PUSH_L2_HDR;
            break;

        case OF_ACTION_OFDPA_PUSH_CW:
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActPushCw,1);
            *group_action_bitmap |= IND_OFDPA_PUSH_CW;
            break;

        case OF_ACTION_OFDPA_COPY_TC_OUT:
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActCpyMplsExpOutwards,1);
            *group_action_bitmap |= IND_OFDPA_COPY_MPLS_TC_OUT;
            break;

        case OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE  :
        {
            uint16_t qosIndex;
            of_action_ofdpa_set_mpls_tc_from_vpn_table_qos_index_get(&act.ofdpa_set_mpls_tc_from_vpn_table, &qosIndex);

						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetMplsExpRemarkTable,qosIndex);
            *group_action_bitmap |= IND_OFDPA_MPLS_TC_REMARK_TABLE_INDEX;
            break;
        }
        case OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE:
        {
            uint16_t qosIndex;
            of_action_ofdpa_set_pcpdei_from_table_qos_index_get(&act.ofdpa_set_pcpdei_from_table, &qosIndex);
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetPcpPriRemarkTable,qosIndex);
            *group_action_bitmap |= IND_OFDPA_PCP_REMARK_TABLE_INDEX;
            break;
        }
        case OF_ACTION_GROUP:
            of_action_group_group_id_get(&act.group, &group_bucket->referenceGroupId);
            *group_action_bitmap |= IND_OFDPA_REFGROUP;
            break;
        case OF_ACTION_OFDPA_OAM_LM_TX_COUNT:
        {
            uint32_t lmepId;
            of_action_ofdpa_oam_lm_tx_count_lmep_id_get(&act.ofdpa_oam_lm_tx_count, &lmepId);
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActOamLmTxCount,lmepId);
            *group_action_bitmap |= IND_OFDPA_OAM_LM_TX_COUNT;
            break;
        }
        case OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE:
        {
            uint16_t qosIndex;
            of_action_ofdpa_set_mpls_tc_from_tunnel_table_qos_index_get(&act.ofdpa_set_mpls_tc_from_tunnel_table, &qosIndex);
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetMplsExpRemarkTable,qosIndex);
            *group_action_bitmap |= IND_OFDPA_MPLS_TC_REMARK_TABLE_INDEX;
            break;
        }
        case OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE:
        {
            uint16_t qosIndex;
            of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_qos_index_get(&act.ofdpa_set_mpls_pcpdei_from_table, &qosIndex);
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActSetMplsExpRemarkTable,qosIndex);
            *group_action_bitmap |= IND_OFDPA_ACT_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE;
            break;
        }
        case OF_ACTION_OFDPA_COLOR_BASED_COUNT:
        {
            uint32_t index;
            of_action_ofdpa_color_based_count_index_get(&act.ofdpa_color_based_count, &index);
						IND_OFDPA_BUKT_ACT_APPEND(group_bucket,ofdpaActIncColorBasedCount,index);
            *group_action_bitmap |= IND_OFDPA_COLOR_BASED_COUNT;
            break;
        }
        case OF_ACTION_OFDPA_CHECK_DROP_STATUS:
        {
            uint16_t drop_type;
            uint32_t drop_index;
            of_action_ofdpa_check_drop_status_drop_type_get(&act.ofdpa_check_drop_status, &drop_type);
            of_action_ofdpa_check_drop_status_drop_index_get(&act.ofdpa_check_drop_status, &drop_index);
            *group_action_bitmap |= IND_OFDPA_CHECK_DROP_STATUS;
            break;
        }
        default:
            LOG_ERROR("unsupported action %s", of_object_id_str[act.header.object_id]);
            return INDIGO_ERROR_COMPAT;
        }
    }

    LOG_TRACE("group_action_bitmap is 0x%llX, group_action_sf_bitmap is 0x%llX",
        *group_action_bitmap, *group_action_sf_bitmap);
    return INDIGO_ERROR_NONE;
}

static indigo_error_t
ind_ofdpa_translate_group_buckets(uint32_t group_id,
                                  of_list_bucket_t *of_buckets,
                                  uint16_t command)
{
  indigo_error_t err;
  uint16_t bucket_index = 0;
  of_list_action_t of_actions;
  of_bucket_t of_bucket;
  ind_ofdpa_group_bucket_t group_bucket;
  int rv;
  uint32_t group_type, sub_group_type;
  uint64_t group_action_bitmap = 0;
  uint64_t group_action_sf_bitmap = 0;
  ofdpaGroupEntry_t group_entry;
  ofdpaGroupBucketEntry_t group_bucket_entry;
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_FAIL;
  int group_added = 0;
  of_port_no_t watch_port;
	int i;
	ofdpaGroupBucketData_t	*pData;

  OF_LIST_BUCKET_ITER(of_buckets, &of_bucket, rv)
  {
    of_bucket_watch_port_get(&of_bucket,&watch_port);

    of_bucket_actions_bind(&of_bucket, &of_actions);

    memset(&group_bucket, 0, sizeof(group_bucket));

    err = ind_ofdpa_translate_group_actions(
        &of_actions, &group_bucket, &group_action_bitmap, &group_action_sf_bitmap);
    if (err < 0)
    {
      LOG_ERROR("Error in translating group actions");
      if (group_added == 1)
      {
        /* Delete the added group */
        (void)ofdpaGroupDelete(group_id);
      }
      return err;
    }

    ofdpaGroupTypeGet(group_id, &group_type);

    memset(&group_bucket_entry, 0, sizeof(group_bucket_entry));
    group_bucket_entry.groupId = group_id;
    group_bucket_entry.bucketIndex = bucket_index;

    err = INDIGO_ERROR_NONE;

    switch (group_type)
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:

        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:

        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
        if((group_action_bitmap | IND_OFDPA_L2REWRITE_BITMAP) != IND_OFDPA_L2REWRITE_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }
        if((group_action_sf_bitmap | IND_OFDPA_L2REWRITE_SF_BITMAP) != IND_OFDPA_L2REWRITE_SF_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }


        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
        if((group_action_bitmap | IND_OFDPA_L3UNICAST_BITMAP) != IND_OFDPA_L3UNICAST_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }
        if((group_action_sf_bitmap | IND_OFDPA_L3UNICAST_SF_BITMAP) != IND_OFDPA_L3UNICAST_SF_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }


        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
        if((group_action_bitmap | IND_OFDPA_L3INTERFACE_BITMAP) != IND_OFDPA_L3INTERFACE_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }
        if((group_action_sf_bitmap | IND_OFDPA_L3INTERFACE_SF_BITMAP) != IND_OFDPA_L3INTERFACE_SF_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }


        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
      case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
        if((group_action_bitmap | IND_OFDPA_REFGROUP) != IND_OFDPA_REFGROUP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        group_bucket_entry.referenceGroupId = group_bucket.referenceGroupId;
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        if((group_action_bitmap | IND_OFDPA_L2OVERLAY_BITMAP) != IND_OFDPA_L2OVERLAY_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        break;

      case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
        ofdpaGroupMplsSubTypeGet(group_id, &sub_group_type);
        switch (sub_group_type)
        {
        	int i;
          case OFDPA_MPLS_INTERFACE:
            if((group_action_bitmap | IND_OFDPA_MPLSINTERFACE_BITMAP) != IND_OFDPA_MPLSINTERFACE_BITMAP)
            {
              err = INDIGO_ERROR_COMPAT;
              break;
            }

            break;

          case OFDPA_MPLS_L2_VPN_LABEL:
          case OFDPA_MPLS_L3_VPN_LABEL:
          case OFDPA_MPLS_TUNNEL_LABEL1:
          case OFDPA_MPLS_TUNNEL_LABEL2:
          case OFDPA_MPLS_SWAP_LABEL:
            if((group_action_bitmap | IND_OFDPA_MPLSLABEL_BITMAP) != IND_OFDPA_MPLSLABEL_BITMAP)
            {
              err = INDIGO_ERROR_COMPAT;
              LOG_ERROR("unsupported action %d , expected %d", group_action_bitmap, IND_OFDPA_MPLSLABEL_BITMAP);
              break;
            }
            if((group_action_sf_bitmap | IND_OFDPA_MPLSLABEL_SF_BITMAP) != IND_OFDPA_MPLSLABEL_SF_BITMAP)
            {
              err = INDIGO_ERROR_COMPAT;
              LOG_ERROR("unsupported action %d , expected %d", group_action_bitmap, IND_OFDPA_MPLSLABEL_BITMAP);
              break;
            }

            break;

          default:
            LOG_ERROR("unsupported MPLS_SUBTYPE %d for GROUP_TYPE %d", sub_group_type, group_type);
            return INDIGO_ERROR_COMPAT;
        }
        break;

      case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
        ofdpaGroupMplsSubTypeGet(group_id, &sub_group_type);
        switch (sub_group_type)
        {
          case OFDPA_MPLS_FAST_FAILOVER:
            if((group_action_bitmap | IND_OFDPA_MPLSFF_BITMAP) != IND_OFDPA_MPLSFF_BITMAP)
            {
              err = INDIGO_ERROR_COMPAT;
              break;
            }

            break;

          case OFDPA_MPLS_L2_TAG:
            if((group_action_bitmap | IND_OFDPA_MPLSL2TAG_BITMAP) != IND_OFDPA_MPLSL2TAG_BITMAP)
            {
              err = INDIGO_ERROR_COMPAT;
              break;
            }
            if((group_action_sf_bitmap | IND_OFDPA_MPLSL2TAG_SF_BITMAP) != IND_OFDPA_MPLSL2TAG_SF_BITMAP)
            {
              err = INDIGO_ERROR_COMPAT;
              break;
            }

            break;

          case OFDPA_MPLS_L2_FLOOD:
          case OFDPA_MPLS_L2_MULTICAST:
          case OFDPA_MPLS_L2_LOCAL_FLOOD:
          case OFDPA_MPLS_L2_LOCAL_MULTICAST:
          case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
          case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
          case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
          case OFDPA_MPLS_ECMP:
            if((group_action_bitmap | IND_OFDPA_REFGROUP) != IND_OFDPA_REFGROUP)
            {
              err = INDIGO_ERROR_COMPAT;
              break;
            }

            break;

          default:
            LOG_ERROR("unsupported MPLS_SUBTYPE %d for GROUP_TYPE %d", sub_group_type, group_type);
            return INDIGO_ERROR_COMPAT;
        }
        break;

      default:
        err = INDIGO_ERROR_PARAM;
        LOG_ERROR("Invalid GROUP_TYPE %d", group_type);
        break;
    }



    if (err != INDIGO_ERROR_NONE)
    {
      if (err == INDIGO_ERROR_COMPAT)
      {
        LOG_ERROR("Incompatible fields for Group Type");
      }
      if (group_added == 1)
      {
        /* Delete the added group */
        (void)ofdpaGroupDelete(group_id);
      }
      return err;
    }


		pData = &group_bucket_entry.bucketData;
    for(i = 0 ; i < group_bucket.act_cnt ; i++){
			pData->actions[i] = group_bucket.actions[i];
			pData->act_cnt++;
		}
		group_bucket_entry.referenceGroupId = group_bucket.referenceGroupId;
		
    if (command == OF_GROUP_ADD)
    {
      if (bucket_index == 0)
      {
        /* First Bucket; Add Group first*/
        group_entry.groupId = group_id;
        ofdpa_rv = ofdpaGroupAdd(&group_entry);
        if (ofdpa_rv != OFDPA_E_NONE)
        {
          LOG_ERROR("Error in adding Group, rv = %d",ofdpa_rv);
          break;
        }
        group_added = 1;
      }
    }
    else /* OF_GROUP_MODIFY */
    {

      if(group_type == OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING)
      {
          ofdpaGroupMplsSubTypeGet(group_id, &sub_group_type);
          if(sub_group_type == OFDPA_MPLS_FAST_FAILOVER )
          {

            ofdpa_rv = ofdpaGroupBucketEntryModify(&group_bucket_entry);
            if (ofdpa_rv != OFDPA_E_NONE)
            {
              LOG_ERROR("Error in modifying Group buckets, rv = %d",ofdpa_rv);
              break;
            }
            bucket_index++;
            continue;
          }

      }
      
      if (bucket_index == 0)
      {
        /* First Bucket; Delete all buckets first */
        ofdpa_rv = ofdpaGroupBucketsDeleteAll(group_id);
        if (ofdpa_rv != OFDPA_E_NONE)
        {
          LOG_ERROR("Error in deleting Group buckets, rv = %d",ofdpa_rv);
          break;
        }
      }
    }

    ofdpa_rv = ofdpaGroupBucketEntryAdd(&group_bucket_entry);
    if (ofdpa_rv != OFDPA_E_NONE)
    {
      LOG_ERROR("Error in adding Group bucket, rv = %d",ofdpa_rv);
      if (group_added == 1)
      {
        /* Delete the added group */
        (void)ofdpaGroupDelete(group_id);
      }
      else /* OF_GROUP_MODIFY */
      {
        /* Need to clean up and delete Group here as well.
           Will be done by the caller as the Group also needs to
           be deleted from the Indigo database. */
      }
      break;
    }

    bucket_index++;
  }

  return indigoConvertOfdpaRv(ofdpa_rv);
}

indigo_error_t indigo_fwd_group_add(uint32_t id, uint8_t group_type, of_list_bucket_t *buckets)
{
  indigo_error_t err;

  if ((group_type != OF_GROUP_TYPE_INDIRECT) &&
      (group_type != OF_GROUP_TYPE_SELECT) &&
      (group_type != OF_GROUP_TYPE_FF) &&
      (group_type != OF_GROUP_TYPE_ALL))
  {
    return INDIGO_ERROR_NOT_SUPPORTED;
  }

  err = ind_ofdpa_translate_group_buckets(id, buckets, OF_GROUP_ADD);

  return err;
}

indigo_error_t indigo_fwd_group_modify(uint32_t id, of_list_bucket_t *buckets)
{
  indigo_error_t err;

  err = ind_ofdpa_translate_group_buckets(id, buckets, OF_GROUP_MODIFY);

  return err;
}

#ifdef OFDPA_FIXUP
indigo_error_t indigo_fwd_group_delete(uint32_t id)
#else
void indigo_fwd_group_delete(uint32_t id)
#endif
{
  OFDPA_ERROR_t ofdpa_rv;
  indigo_error_t rv;

  ofdpa_rv = ofdpaGroupDelete(id);

  rv = indigoConvertOfdpaRv(ofdpa_rv);

  /* OpenFlow 1.3.4, pp6.5: Not an error if the group is not found. */
  if (OFDPA_E_NOT_FOUND == ofdpa_rv)
  {
    rv = INDIGO_ERROR_NONE;
  }
  else if (OFDPA_E_REQUEST_DENIED == ofdpa_rv)
  {
    rv = INDIGO_ERROR_TABLE_FULL;
  }
  else if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Group Delete failed, rv = %d", ofdpa_rv);
  }

#ifdef OFDPA_FIXUP
  return rv;
#else
  return;
#endif
}

void indigo_fwd_group_stats_get(uint32_t id, of_group_stats_entry_t *entry)
{
  OFDPA_ERROR_t ofdpa_rv;
  ofdpaGroupEntryStats_t groupStats;

  memset(&groupStats, 0, sizeof(groupStats));
  ofdpa_rv = ofdpaGroupStatsGet(id, &groupStats);

  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Failed to get Group stats, rv = %d",ofdpa_rv);
  }
  else
  {
    of_group_stats_entry_packet_count_set(entry, groupStats.packet_count);
    of_group_stats_entry_byte_count_set(entry, groupStats.byte_count);

    of_group_stats_entry_ref_count_set(entry, groupStats.refCount);
    of_group_stats_entry_duration_sec_set(entry, groupStats.duration);
  }

  return;
}

