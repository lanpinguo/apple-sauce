/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifndef __LOCI_CLASSES_H__
#define __LOCI_CLASSES_H__

void of_action_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_bsn_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_bsn_mirror_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_bsn_set_tunnel_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_enqueue_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_nicira_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_nicira_dec_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_output_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_dl_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_dl_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_nw_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_nw_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_nw_tos_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_tp_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_tp_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_vlan_pcp_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_vlan_vid_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_strip_vlan_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_header_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_aggregate_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_aggregate_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bad_action_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bad_request_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_barrier_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_barrier_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_header_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_bw_clear_data_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_bw_clear_data_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_bw_enable_get_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_bw_enable_get_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_bw_enable_set_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_bw_enable_set_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_interfaces_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_interfaces_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_ip_mask_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_ip_mask_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_l2_table_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_l2_table_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_mirroring_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_mirroring_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_hybrid_get_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_hybrid_get_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_interface_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_pdu_rx_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_pdu_rx_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_pdu_rx_timeout_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_pdu_tx_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_pdu_tx_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_ip_mask_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_l2_table_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_l2_table_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_mirroring_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_pktin_suppression_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_pktin_suppression_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_shell_command_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_shell_output_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_shell_status_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_experimenter_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_experimenter_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_virtual_port_create_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_vport_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_vport_q_in_q_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_virtual_port_create_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_virtual_port_remove_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_virtual_port_remove_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_desc_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_desc_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_echo_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_echo_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_features_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_features_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_mod_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_add_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_delete_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_delete_strict_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_mod_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_modify_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_modify_strict_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_removed_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_flow_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_get_config_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_get_config_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_hello_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_hello_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_match_v1_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_nicira_header_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_nicira_controller_role_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_nicira_controller_role_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_packet_in_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_packet_out_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_packet_queue_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_desc_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_mod_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_mod_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_status_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_get_config_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_get_config_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_op_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_prop_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_prop_min_rate_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_set_config_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_mod_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_copy_ttl_in_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_copy_ttl_out_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_dec_mpls_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_dec_nw_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_group_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_pop_mpls_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_pop_vlan_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_push_mpls_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_push_vlan_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_mpls_label_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_mpls_tc_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_mpls_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_nw_ecn_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_nw_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_queue_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bad_instruction_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bad_match_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bucket_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bucket_counter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_mod_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_add_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_delete_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_desc_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_desc_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_desc_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_mod_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_modify_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_apply_actions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_clear_actions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_goto_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_write_actions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_write_metadata_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_match_v2_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_switch_config_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_mod_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_set_field_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_experimenter_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_features_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_group_features_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_match_v3_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_op_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_op_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_sha_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_sha_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_spa_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_spa_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_tha_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_tha_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_tpa_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_arp_tpa_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_egr_port_group_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_egr_port_group_id_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_global_vrf_allowed_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_global_vrf_allowed_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_in_ports_128_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_in_ports_128_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_l3_dst_class_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_l3_dst_class_id_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_l3_interface_class_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_l3_interface_class_id_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_l3_src_class_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_l3_src_class_id_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_lag_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_lag_id_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf0_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf0_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf1_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf1_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf2_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf2_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf3_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf3_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf4_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf4_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf5_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf5_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf6_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf6_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf7_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_udf7_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_vrf_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_bsn_vrf_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_eth_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_eth_dst_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_eth_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_eth_src_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_eth_type_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_eth_type_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_icmpv4_code_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_icmpv4_code_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_icmpv4_type_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_icmpv4_type_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_icmpv6_code_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_icmpv6_code_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_icmpv6_type_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_icmpv6_type_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_in_phy_port_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_in_phy_port_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_in_port_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_in_port_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ip_dscp_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ip_dscp_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ip_ecn_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ip_ecn_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ip_proto_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ip_proto_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv4_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv4_dst_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv4_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv4_src_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_dst_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_flabel_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_flabel_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_nd_sll_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_nd_sll_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_nd_target_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_nd_target_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_nd_tll_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_nd_tll_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ipv6_src_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_metadata_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_metadata_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_mpls_bos_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_mpls_bos_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_mpls_label_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_mpls_label_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_mpls_tc_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_mpls_tc_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_sctp_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_sctp_dst_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_sctp_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_sctp_src_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_tcp_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_tcp_dst_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_tcp_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_tcp_src_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_tunnel_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_tunnel_id_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_udp_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_udp_dst_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_udp_src_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_udp_src_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_vlan_pcp_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_vlan_pcp_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_vlan_vid_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_vlan_vid_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_prop_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_queue_prop_max_rate_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_role_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_role_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_role_request_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_bsn_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_bsn_mirror_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_bsn_set_tunnel_dst_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_copy_ttl_in_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_copy_ttl_out_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_dec_mpls_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_dec_nw_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_group_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_nicira_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_nicira_dec_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_check_drop_status_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_check_drop_status_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_class_based_count_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_class_based_count_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_color_based_count_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_color_based_count_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_color_based_count_1_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_color_based_count_1_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_color_based_count_2_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_color_based_count_2_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_color_based_count_3_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_color_based_count_3_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_copy_tc_in_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_copy_tc_in_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_copy_tc_out_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_copy_tc_out_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_dec_ttl_mtu_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_dec_ttl_mtu_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_oam_lm_rx_count_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_oam_lm_rx_count_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_oam_lm_tx_count_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_oam_lm_tx_count_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_pop_cw_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_pop_cw_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_pop_l2hdr_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_pop_l2hdr_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_push_cw_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_push_cw_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_push_l2hdr_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_push_l2hdr_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_set_counter_fields_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_set_counter_fields_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_set_mpls_pcpdei_from_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_set_mpls_pcpdei_from_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_set_mpls_tc_from_tunnel_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_set_mpls_tc_from_vpn_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_set_mpls_tc_from_vpn_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_ofdpa_set_pcpdei_from_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_ofdpa_set_pcpdei_from_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_onf_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_onf_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_onf_copy_field_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_onf_copy_field_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_output_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_pop_mpls_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_pop_pbb_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_pop_pbb_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_pop_vlan_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_push_mpls_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_push_pbb_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_push_pbb_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_push_vlan_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_set_field_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_set_mpls_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_set_nw_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_action_id_set_queue_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_async_get_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_async_get_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_async_set_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_arp_idle_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_controller_connection_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_controller_connections_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_controller_connections_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_debug_counter_desc_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_debug_counter_desc_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_debug_counter_desc_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_debug_counter_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_debug_counter_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_debug_counter_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_flow_checksum_bucket_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_flow_checksum_bucket_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_flow_checksum_bucket_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_flow_idle_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_flow_idle_enable_get_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_flow_idle_enable_get_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_flow_idle_enable_set_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_flow_idle_enable_set_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_bucket_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_bucket_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_bucket_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_clear_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_clear_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_desc_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_desc_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_desc_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_entry_add_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_entry_delete_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_entry_desc_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_entry_desc_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_entry_desc_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_entry_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_entry_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_entry_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_set_buckets_size_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_gentable_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_switch_pipeline_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_get_switch_pipeline_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_lacp_convergence_notif_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_lacp_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_lacp_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_lacp_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_port_counter_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_port_counter_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_port_counter_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_role_status_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_aux_cxns_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_aux_cxns_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_lacp_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_lacp_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_switch_pipeline_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_set_switch_pipeline_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_switch_pipeline_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_switch_pipeline_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_switch_pipeline_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_table_checksum_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_table_checksum_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_table_checksum_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_table_set_buckets_size_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_time_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_time_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_broadcast_query_timeout_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_circuit_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_idle_notification_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_idle_time_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_idle_timeout_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_ipv4_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_mac_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_miss_packets_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_port_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_reply_packets_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_request_packets_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_rx_packets_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_tx_packets_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_udf_anchor_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_udf_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_udf_length_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_udf_offset_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_unicast_query_timeout_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_tlv_vlan_vid_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_vlan_counter_stats_entry_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_vlan_counter_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_bsn_vlan_counter_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_experimenter_multipart_reply_ofdpa_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_experimenter_multipart_request_ofdpa_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_experimenter_ofdpa_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_hello_elem_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_hello_elem_versionbitmap_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_apply_actions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_bsn_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_bsn_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_bsn_arp_offload_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_bsn_arp_offload_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_bsn_deny_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_bsn_deny_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_bsn_dhcp_offload_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_bsn_dhcp_offload_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_bsn_disable_split_horizon_check_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_bsn_disable_split_horizon_check_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_bsn_disable_src_mac_check_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_bsn_disable_src_mac_check_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_bsn_packet_of_death_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_bsn_packet_of_death_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_bsn_permit_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_bsn_permit_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_clear_actions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_goto_table_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_meter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_meter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_write_actions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_instruction_id_write_metadata_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_mod_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_add_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_band_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_band_drop_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_band_dscp_remark_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_band_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_band_ofdpa_color_set_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_band_stats_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_config_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_config_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_config_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_delete_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_features_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_features_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_features_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_mod_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_modify_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_stats_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_meter_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_class_based_ctr_mod_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_class_based_ctr_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_class_based_ctr_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_color_based_ctr_mod_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_color_based_ctr_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_color_based_ctr_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_l2_interface_remark_action_mod_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_l2_interface_remark_action_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_l2_interface_remark_action_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_mpls_tunnel_label_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_mpls_tunnel_label_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_oam_dataplane_ctr_mod_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_oam_dataplane_ctr_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_oam_dataplane_ctr_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_oam_drop_status_mod_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_oam_drop_status_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_oam_drop_status_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_port_experimenter_prop_scheduling_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_queue_desc_prop_congestion_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_queue_desc_prop_congestion_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_queue_desc_prop_parent_multipart_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_queue_desc_prop_parent_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_queue_mod_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_scheduler_node_mod_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_scheduler_node_multipart_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_ofdpa_scheduler_node_multipart_response_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_allow_vlan_translation_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_color_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_color_actions_index_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_dei_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_l3_in_port_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_lmep_id_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_mpls_ach_channel_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_mpls_data_first_nibble_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_mpls_l2_port_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_mpls_l2_port_masked_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_mpls_next_label_is_gal_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_mpls_ttl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_mpls_type_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_oam_y1731_mdl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_oam_y1731_opcode_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_ovid_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_protection_index_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_qos_index_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_rx_timestamp_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_rxfcl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_traffic_class_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_txfcl_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_ofdpa_vrf_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_oxm_onf_actset_output_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_desc_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_port_desc_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_apply_actions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_apply_actions_miss_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_apply_setfield_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_apply_setfield_miss_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_experimenter_miss_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_instructions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_instructions_miss_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_match_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_next_tables_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_next_tables_miss_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_wildcards_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_write_actions_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_write_actions_miss_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_write_setfield_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_feature_prop_write_setfield_miss_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_features_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_features_failed_error_msg_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_features_stats_reply_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_table_features_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_uint32_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_uint64_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void of_uint8_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void ofp_queue_desc_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void onf_oxm_packet_regs0_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void onf_oxm_packet_regs1_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void onf_oxm_packet_regs2_wire_object_id_get(of_object_t *obj, of_object_id_t *id);
void onf_oxm_packet_regs3_wire_object_id_get(of_object_t *obj, of_object_id_t *id);


/* LOCI inheritance parent typedefs */
typedef union of_queue_prop_u of_queue_prop_t;
typedef union of_hello_elem_u of_hello_elem_t;
typedef union of_bsn_vport_u of_bsn_vport_t;
typedef union of_table_feature_prop_u of_table_feature_prop_t;
typedef union of_action_u of_action_t;
typedef union of_instruction_u of_instruction_t;
typedef union of_bsn_tlv_u of_bsn_tlv_t;
typedef union of_instruction_id_u of_instruction_id_t;
typedef union of_meter_band_u of_meter_band_t;
typedef union of_action_id_u of_action_id_t;
typedef union of_oxm_u of_oxm_t;

/* LOCI object typedefs */
typedef of_object_t of_aggregate_stats_reply_t;
typedef of_object_t of_aggregate_stats_request_t;
typedef of_object_t of_async_get_reply_t;
typedef of_object_t of_async_get_request_t;
typedef of_object_t of_async_set_t;
typedef of_object_t of_bad_action_error_msg_t;
typedef of_object_t of_bad_instruction_error_msg_t;
typedef of_object_t of_bad_match_error_msg_t;
typedef of_object_t of_bad_request_error_msg_t;
typedef of_object_t of_barrier_reply_t;
typedef of_object_t of_barrier_request_t;
typedef of_object_t of_bsn_arp_idle_t;
typedef of_object_t of_bsn_bw_clear_data_reply_t;
typedef of_object_t of_bsn_bw_clear_data_request_t;
typedef of_object_t of_bsn_bw_enable_get_reply_t;
typedef of_object_t of_bsn_bw_enable_get_request_t;
typedef of_object_t of_bsn_bw_enable_set_reply_t;
typedef of_object_t of_bsn_bw_enable_set_request_t;
typedef of_object_t of_bsn_controller_connections_reply_t;
typedef of_object_t of_bsn_controller_connections_request_t;
typedef of_object_t of_bsn_debug_counter_desc_stats_reply_t;
typedef of_object_t of_bsn_debug_counter_desc_stats_request_t;
typedef of_object_t of_bsn_debug_counter_stats_reply_t;
typedef of_object_t of_bsn_debug_counter_stats_request_t;
typedef of_object_t of_bsn_flow_checksum_bucket_stats_reply_t;
typedef of_object_t of_bsn_flow_checksum_bucket_stats_request_t;
typedef of_object_t of_bsn_flow_idle_t;
typedef of_object_t of_bsn_flow_idle_enable_get_reply_t;
typedef of_object_t of_bsn_flow_idle_enable_get_request_t;
typedef of_object_t of_bsn_flow_idle_enable_set_reply_t;
typedef of_object_t of_bsn_flow_idle_enable_set_request_t;
typedef of_object_t of_bsn_gentable_bucket_stats_reply_t;
typedef of_object_t of_bsn_gentable_bucket_stats_request_t;
typedef of_object_t of_bsn_gentable_clear_reply_t;
typedef of_object_t of_bsn_gentable_clear_request_t;
typedef of_object_t of_bsn_gentable_desc_stats_reply_t;
typedef of_object_t of_bsn_gentable_desc_stats_request_t;
typedef of_object_t of_bsn_gentable_entry_add_t;
typedef of_object_t of_bsn_gentable_entry_delete_t;
typedef of_object_t of_bsn_gentable_entry_desc_stats_reply_t;
typedef of_object_t of_bsn_gentable_entry_desc_stats_request_t;
typedef of_object_t of_bsn_gentable_entry_stats_reply_t;
typedef of_object_t of_bsn_gentable_entry_stats_request_t;
typedef of_object_t of_bsn_gentable_set_buckets_size_t;
typedef of_object_t of_bsn_gentable_stats_reply_t;
typedef of_object_t of_bsn_gentable_stats_request_t;
typedef of_object_t of_bsn_get_interfaces_reply_t;
typedef of_object_t of_bsn_get_interfaces_request_t;
typedef of_object_t of_bsn_get_ip_mask_reply_t;
typedef of_object_t of_bsn_get_ip_mask_request_t;
typedef of_object_t of_bsn_get_l2_table_reply_t;
typedef of_object_t of_bsn_get_l2_table_request_t;
typedef of_object_t of_bsn_get_mirroring_reply_t;
typedef of_object_t of_bsn_get_mirroring_request_t;
typedef of_object_t of_bsn_get_switch_pipeline_reply_t;
typedef of_object_t of_bsn_get_switch_pipeline_request_t;
typedef of_object_t of_bsn_header_t;
typedef of_object_t of_bsn_hybrid_get_reply_t;
typedef of_object_t of_bsn_hybrid_get_request_t;
typedef of_object_t of_bsn_lacp_convergence_notif_t;
typedef of_object_t of_bsn_lacp_stats_reply_t;
typedef of_object_t of_bsn_lacp_stats_request_t;
typedef of_object_t of_bsn_pdu_rx_reply_t;
typedef of_object_t of_bsn_pdu_rx_request_t;
typedef of_object_t of_bsn_pdu_rx_timeout_t;
typedef of_object_t of_bsn_pdu_tx_reply_t;
typedef of_object_t of_bsn_pdu_tx_request_t;
typedef of_object_t of_bsn_port_counter_stats_reply_t;
typedef of_object_t of_bsn_port_counter_stats_request_t;
typedef of_object_t of_bsn_role_status_t;
typedef of_object_t of_bsn_set_aux_cxns_reply_t;
typedef of_object_t of_bsn_set_aux_cxns_request_t;
typedef of_object_t of_bsn_set_ip_mask_t;
typedef of_object_t of_bsn_set_l2_table_reply_t;
typedef of_object_t of_bsn_set_l2_table_request_t;
typedef of_object_t of_bsn_set_lacp_reply_t;
typedef of_object_t of_bsn_set_lacp_request_t;
typedef of_object_t of_bsn_set_mirroring_t;
typedef of_object_t of_bsn_set_pktin_suppression_reply_t;
typedef of_object_t of_bsn_set_pktin_suppression_request_t;
typedef of_object_t of_bsn_set_switch_pipeline_reply_t;
typedef of_object_t of_bsn_set_switch_pipeline_request_t;
typedef of_object_t of_bsn_shell_command_t;
typedef of_object_t of_bsn_shell_output_t;
typedef of_object_t of_bsn_shell_status_t;
typedef of_object_t of_bsn_stats_reply_t;
typedef of_object_t of_bsn_stats_request_t;
typedef of_object_t of_bsn_switch_pipeline_stats_reply_t;
typedef of_object_t of_bsn_switch_pipeline_stats_request_t;
typedef of_object_t of_bsn_table_checksum_stats_reply_t;
typedef of_object_t of_bsn_table_checksum_stats_request_t;
typedef of_object_t of_bsn_table_set_buckets_size_t;
typedef of_object_t of_bsn_time_reply_t;
typedef of_object_t of_bsn_time_request_t;
typedef of_object_t of_bsn_virtual_port_create_reply_t;
typedef of_object_t of_bsn_virtual_port_create_request_t;
typedef of_object_t of_bsn_virtual_port_remove_reply_t;
typedef of_object_t of_bsn_virtual_port_remove_request_t;
typedef of_object_t of_bsn_vlan_counter_stats_reply_t;
typedef of_object_t of_bsn_vlan_counter_stats_request_t;
typedef of_object_t of_desc_stats_reply_t;
typedef of_object_t of_desc_stats_request_t;
typedef of_object_t of_echo_reply_t;
typedef of_object_t of_echo_request_t;
typedef of_object_t of_error_msg_t;
typedef of_object_t of_experimenter_t;
typedef of_object_t of_experimenter_error_msg_t;
typedef of_object_t of_experimenter_multipart_reply_ofdpa_t;
typedef of_object_t of_experimenter_multipart_request_ofdpa_t;
typedef of_object_t of_experimenter_ofdpa_t;
typedef of_object_t of_experimenter_stats_reply_t;
typedef of_object_t of_experimenter_stats_request_t;
typedef of_object_t of_features_reply_t;
typedef of_object_t of_features_request_t;
typedef of_object_t of_flow_add_t;
typedef of_object_t of_flow_delete_t;
typedef of_object_t of_flow_delete_strict_t;
typedef of_object_t of_flow_mod_t;
typedef of_object_t of_flow_mod_failed_error_msg_t;
typedef of_object_t of_flow_modify_t;
typedef of_object_t of_flow_modify_strict_t;
typedef of_object_t of_flow_removed_t;
typedef of_object_t of_flow_stats_reply_t;
typedef of_object_t of_flow_stats_request_t;
typedef of_object_t of_get_config_reply_t;
typedef of_object_t of_get_config_request_t;
typedef of_object_t of_group_add_t;
typedef of_object_t of_group_delete_t;
typedef of_object_t of_group_desc_stats_reply_t;
typedef of_object_t of_group_desc_stats_request_t;
typedef of_object_t of_group_features_stats_reply_t;
typedef of_object_t of_group_features_stats_request_t;
typedef of_object_t of_group_mod_t;
typedef of_object_t of_group_mod_failed_error_msg_t;
typedef of_object_t of_group_modify_t;
typedef of_object_t of_group_stats_reply_t;
typedef of_object_t of_group_stats_request_t;
typedef of_object_t of_hello_t;
typedef of_object_t of_hello_failed_error_msg_t;
typedef of_object_t of_meter_add_t;
typedef of_object_t of_meter_config_stats_reply_t;
typedef of_object_t of_meter_config_stats_request_t;
typedef of_object_t of_meter_delete_t;
typedef of_object_t of_meter_features_stats_reply_t;
typedef of_object_t of_meter_features_stats_request_t;
typedef of_object_t of_meter_mod_t;
typedef of_object_t of_meter_mod_failed_error_msg_t;
typedef of_object_t of_meter_modify_t;
typedef of_object_t of_meter_stats_reply_t;
typedef of_object_t of_meter_stats_request_t;
typedef of_object_t of_nicira_controller_role_reply_t;
typedef of_object_t of_nicira_controller_role_request_t;
typedef of_object_t of_nicira_header_t;
typedef of_object_t of_ofdpa_class_based_ctr_mod_msg_t;
typedef of_object_t of_ofdpa_class_based_ctr_multipart_reply_t;
typedef of_object_t of_ofdpa_class_based_ctr_multipart_request_t;
typedef of_object_t of_ofdpa_color_based_ctr_mod_msg_t;
typedef of_object_t of_ofdpa_color_based_ctr_multipart_reply_t;
typedef of_object_t of_ofdpa_color_based_ctr_multipart_request_t;
typedef of_object_t of_ofdpa_l2_interface_remark_action_mod_msg_t;
typedef of_object_t of_ofdpa_l2_interface_remark_action_multipart_reply_t;
typedef of_object_t of_ofdpa_l2_interface_remark_action_multipart_request_t;
typedef of_object_t of_ofdpa_mpls_tunnel_label_multipart_reply_t;
typedef of_object_t of_ofdpa_mpls_tunnel_label_multipart_request_t;
typedef of_object_t of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t;
typedef of_object_t of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t;
typedef of_object_t of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t;
typedef of_object_t of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t;
typedef of_object_t of_ofdpa_oam_dataplane_ctr_mod_msg_t;
typedef of_object_t of_ofdpa_oam_dataplane_ctr_multipart_reply_t;
typedef of_object_t of_ofdpa_oam_dataplane_ctr_multipart_request_t;
typedef of_object_t of_ofdpa_oam_drop_status_mod_msg_t;
typedef of_object_t of_ofdpa_oam_drop_status_multipart_reply_t;
typedef of_object_t of_ofdpa_oam_drop_status_multipart_request_t;
typedef of_object_t of_ofdpa_port_experimenter_prop_scheduling_t;
typedef of_object_t of_ofdpa_queue_desc_prop_congestion_multipart_reply_t;
typedef of_object_t of_ofdpa_queue_desc_prop_congestion_multipart_request_t;
typedef of_object_t of_ofdpa_queue_desc_prop_parent_multipart_reply_t;
typedef of_object_t of_ofdpa_queue_desc_prop_parent_multipart_request_t;
typedef of_object_t of_ofdpa_queue_mod_t;
typedef of_object_t of_ofdpa_scheduler_node_mod_msg_t;
typedef of_object_t of_ofdpa_scheduler_node_multipart_request_t;
typedef of_object_t of_ofdpa_scheduler_node_multipart_response_t;
typedef of_object_t of_packet_in_t;
typedef of_object_t of_packet_out_t;
typedef of_object_t of_port_desc_stats_reply_t;
typedef of_object_t of_port_desc_stats_request_t;
typedef of_object_t of_port_mod_t;
typedef of_object_t of_port_mod_failed_error_msg_t;
typedef of_object_t of_port_stats_reply_t;
typedef of_object_t of_port_stats_request_t;
typedef of_object_t of_port_status_t;
typedef of_object_t of_queue_get_config_reply_t;
typedef of_object_t of_queue_get_config_request_t;
typedef of_object_t of_queue_op_failed_error_msg_t;
typedef of_object_t of_queue_stats_reply_t;
typedef of_object_t of_queue_stats_request_t;
typedef of_object_t of_role_reply_t;
typedef of_object_t of_role_request_t;
typedef of_object_t of_role_request_failed_error_msg_t;
typedef of_object_t of_set_config_t;
typedef of_object_t of_stats_reply_t;
typedef of_object_t of_stats_request_t;
typedef of_object_t of_switch_config_failed_error_msg_t;
typedef of_object_t of_table_features_failed_error_msg_t;
typedef of_object_t of_table_features_stats_reply_t;
typedef of_object_t of_table_features_stats_request_t;
typedef of_object_t of_table_mod_t;
typedef of_object_t of_table_mod_failed_error_msg_t;
typedef of_object_t of_table_stats_reply_t;
typedef of_object_t of_table_stats_request_t;
typedef of_object_t of_action_bsn_t;
typedef of_object_t of_action_bsn_mirror_t;
typedef of_object_t of_action_bsn_set_tunnel_dst_t;
typedef of_object_t of_action_copy_ttl_in_t;
typedef of_object_t of_action_copy_ttl_out_t;
typedef of_object_t of_action_dec_mpls_ttl_t;
typedef of_object_t of_action_dec_nw_ttl_t;
typedef of_object_t of_action_enqueue_t;
typedef of_object_t of_action_experimenter_t;
typedef of_object_t of_action_group_t;
typedef of_object_t of_action_header_t;
typedef of_object_t of_action_id_bsn_t;
typedef of_object_t of_action_id_bsn_mirror_t;
typedef of_object_t of_action_id_bsn_set_tunnel_dst_t;
typedef of_object_t of_action_id_copy_ttl_in_t;
typedef of_object_t of_action_id_copy_ttl_out_t;
typedef of_object_t of_action_id_dec_mpls_ttl_t;
typedef of_object_t of_action_id_dec_nw_ttl_t;
typedef of_object_t of_action_id_experimenter_t;
typedef of_object_t of_action_id_group_t;
typedef of_object_t of_action_id_header_t;
typedef of_object_t of_action_id_nicira_t;
typedef of_object_t of_action_id_nicira_dec_ttl_t;
typedef of_object_t of_action_id_ofdpa_t;
typedef of_object_t of_action_id_ofdpa_check_drop_status_t;
typedef of_object_t of_action_id_ofdpa_class_based_count_t;
typedef of_object_t of_action_id_ofdpa_color_based_count_t;
typedef of_object_t of_action_id_ofdpa_color_based_count_1_t;
typedef of_object_t of_action_id_ofdpa_color_based_count_2_t;
typedef of_object_t of_action_id_ofdpa_color_based_count_3_t;
typedef of_object_t of_action_id_ofdpa_copy_tc_in_t;
typedef of_object_t of_action_id_ofdpa_copy_tc_out_t;
typedef of_object_t of_action_id_ofdpa_dec_ttl_mtu_t;
typedef of_object_t of_action_id_ofdpa_oam_lm_rx_count_t;
typedef of_object_t of_action_id_ofdpa_oam_lm_tx_count_t;
typedef of_object_t of_action_id_ofdpa_pop_cw_t;
typedef of_object_t of_action_id_ofdpa_pop_l2hdr_t;
typedef of_object_t of_action_id_ofdpa_push_cw_t;
typedef of_object_t of_action_id_ofdpa_push_l2hdr_t;
typedef of_object_t of_action_id_ofdpa_set_counter_fields_t;
typedef of_object_t of_action_id_ofdpa_set_mpls_pcpdei_from_table_t;
typedef of_object_t of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t;
typedef of_object_t of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t;
typedef of_object_t of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t;
typedef of_object_t of_action_id_ofdpa_set_pcpdei_from_table_t;
typedef of_object_t of_action_id_onf_t;
typedef of_object_t of_action_id_onf_copy_field_t;
typedef of_object_t of_action_id_output_t;
typedef of_object_t of_action_id_pop_mpls_t;
typedef of_object_t of_action_id_pop_pbb_t;
typedef of_object_t of_action_id_pop_vlan_t;
typedef of_object_t of_action_id_push_mpls_t;
typedef of_object_t of_action_id_push_pbb_t;
typedef of_object_t of_action_id_push_vlan_t;
typedef of_object_t of_action_id_set_field_t;
typedef of_object_t of_action_id_set_mpls_ttl_t;
typedef of_object_t of_action_id_set_nw_ttl_t;
typedef of_object_t of_action_id_set_queue_t;
typedef of_object_t of_action_nicira_t;
typedef of_object_t of_action_nicira_dec_ttl_t;
typedef of_object_t of_action_ofdpa_t;
typedef of_object_t of_action_ofdpa_check_drop_status_t;
typedef of_object_t of_action_ofdpa_class_based_count_t;
typedef of_object_t of_action_ofdpa_color_based_count_t;
typedef of_object_t of_action_ofdpa_color_based_count_1_t;
typedef of_object_t of_action_ofdpa_color_based_count_2_t;
typedef of_object_t of_action_ofdpa_color_based_count_3_t;
typedef of_object_t of_action_ofdpa_copy_tc_in_t;
typedef of_object_t of_action_ofdpa_copy_tc_out_t;
typedef of_object_t of_action_ofdpa_dec_ttl_mtu_t;
typedef of_object_t of_action_ofdpa_oam_lm_rx_count_t;
typedef of_object_t of_action_ofdpa_oam_lm_tx_count_t;
typedef of_object_t of_action_ofdpa_pop_cw_t;
typedef of_object_t of_action_ofdpa_pop_l2hdr_t;
typedef of_object_t of_action_ofdpa_push_cw_t;
typedef of_object_t of_action_ofdpa_push_l2hdr_t;
typedef of_object_t of_action_ofdpa_set_counter_fields_t;
typedef of_object_t of_action_ofdpa_set_mpls_pcpdei_from_table_t;
typedef of_object_t of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t;
typedef of_object_t of_action_ofdpa_set_mpls_tc_from_tunnel_table_t;
typedef of_object_t of_action_ofdpa_set_mpls_tc_from_vpn_table_t;
typedef of_object_t of_action_ofdpa_set_pcpdei_from_table_t;
typedef of_object_t of_action_onf_t;
typedef of_object_t of_action_onf_copy_field_t;
typedef of_object_t of_action_output_t;
typedef of_object_t of_action_pop_mpls_t;
typedef of_object_t of_action_pop_pbb_t;
typedef of_object_t of_action_pop_vlan_t;
typedef of_object_t of_action_push_mpls_t;
typedef of_object_t of_action_push_pbb_t;
typedef of_object_t of_action_push_vlan_t;
typedef of_object_t of_action_set_dl_dst_t;
typedef of_object_t of_action_set_dl_src_t;
typedef of_object_t of_action_set_field_t;
typedef of_object_t of_action_set_mpls_label_t;
typedef of_object_t of_action_set_mpls_tc_t;
typedef of_object_t of_action_set_mpls_ttl_t;
typedef of_object_t of_action_set_nw_dst_t;
typedef of_object_t of_action_set_nw_ecn_t;
typedef of_object_t of_action_set_nw_src_t;
typedef of_object_t of_action_set_nw_tos_t;
typedef of_object_t of_action_set_nw_ttl_t;
typedef of_object_t of_action_set_queue_t;
typedef of_object_t of_action_set_tp_dst_t;
typedef of_object_t of_action_set_tp_src_t;
typedef of_object_t of_action_set_vlan_pcp_t;
typedef of_object_t of_action_set_vlan_vid_t;
typedef of_object_t of_action_strip_vlan_t;
typedef of_object_t of_bsn_controller_connection_t;
typedef of_object_t of_bsn_debug_counter_desc_stats_entry_t;
typedef of_object_t of_bsn_debug_counter_stats_entry_t;
typedef of_object_t of_bsn_flow_checksum_bucket_stats_entry_t;
typedef of_object_t of_bsn_gentable_bucket_stats_entry_t;
typedef of_object_t of_bsn_gentable_desc_stats_entry_t;
typedef of_object_t of_bsn_gentable_entry_desc_stats_entry_t;
typedef of_object_t of_bsn_gentable_entry_stats_entry_t;
typedef of_object_t of_bsn_gentable_stats_entry_t;
typedef of_object_t of_bsn_interface_t;
typedef of_object_t of_bsn_lacp_stats_entry_t;
typedef of_object_t of_bsn_port_counter_stats_entry_t;
typedef of_object_t of_bsn_switch_pipeline_stats_entry_t;
typedef of_object_t of_bsn_table_checksum_stats_entry_t;
typedef of_object_t of_bsn_tlv_broadcast_query_timeout_t;
typedef of_object_t of_bsn_tlv_circuit_id_t;
typedef of_object_t of_bsn_tlv_header_t;
typedef of_object_t of_bsn_tlv_idle_notification_t;
typedef of_object_t of_bsn_tlv_idle_time_t;
typedef of_object_t of_bsn_tlv_idle_timeout_t;
typedef of_object_t of_bsn_tlv_ipv4_t;
typedef of_object_t of_bsn_tlv_mac_t;
typedef of_object_t of_bsn_tlv_miss_packets_t;
typedef of_object_t of_bsn_tlv_port_t;
typedef of_object_t of_bsn_tlv_reply_packets_t;
typedef of_object_t of_bsn_tlv_request_packets_t;
typedef of_object_t of_bsn_tlv_rx_packets_t;
typedef of_object_t of_bsn_tlv_tx_packets_t;
typedef of_object_t of_bsn_tlv_udf_anchor_t;
typedef of_object_t of_bsn_tlv_udf_id_t;
typedef of_object_t of_bsn_tlv_udf_length_t;
typedef of_object_t of_bsn_tlv_udf_offset_t;
typedef of_object_t of_bsn_tlv_unicast_query_timeout_t;
typedef of_object_t of_bsn_tlv_vlan_vid_t;
typedef of_object_t of_bsn_vlan_counter_stats_entry_t;
typedef of_object_t of_bsn_vport_header_t;
typedef of_object_t of_bsn_vport_q_in_q_t;
typedef of_object_t of_bucket_t;
typedef of_object_t of_bucket_counter_t;
typedef of_object_t of_flow_stats_entry_t;
typedef of_object_t of_group_desc_stats_entry_t;
typedef of_object_t of_group_stats_entry_t;
typedef of_object_t of_header_t;
typedef of_object_t of_hello_elem_header_t;
typedef of_object_t of_hello_elem_versionbitmap_t;
typedef of_object_t of_instruction_apply_actions_t;
typedef of_object_t of_instruction_bsn_t;
typedef of_object_t of_instruction_bsn_arp_offload_t;
typedef of_object_t of_instruction_bsn_deny_t;
typedef of_object_t of_instruction_bsn_dhcp_offload_t;
typedef of_object_t of_instruction_bsn_disable_split_horizon_check_t;
typedef of_object_t of_instruction_bsn_disable_src_mac_check_t;
typedef of_object_t of_instruction_bsn_packet_of_death_t;
typedef of_object_t of_instruction_bsn_permit_t;
typedef of_object_t of_instruction_clear_actions_t;
typedef of_object_t of_instruction_experimenter_t;
typedef of_object_t of_instruction_goto_table_t;
typedef of_object_t of_instruction_header_t;
typedef of_object_t of_instruction_id_apply_actions_t;
typedef of_object_t of_instruction_id_bsn_t;
typedef of_object_t of_instruction_id_bsn_arp_offload_t;
typedef of_object_t of_instruction_id_bsn_deny_t;
typedef of_object_t of_instruction_id_bsn_dhcp_offload_t;
typedef of_object_t of_instruction_id_bsn_disable_split_horizon_check_t;
typedef of_object_t of_instruction_id_bsn_disable_src_mac_check_t;
typedef of_object_t of_instruction_id_bsn_packet_of_death_t;
typedef of_object_t of_instruction_id_bsn_permit_t;
typedef of_object_t of_instruction_id_clear_actions_t;
typedef of_object_t of_instruction_id_experimenter_t;
typedef of_object_t of_instruction_id_goto_table_t;
typedef of_object_t of_instruction_id_header_t;
typedef of_object_t of_instruction_id_meter_t;
typedef of_object_t of_instruction_id_write_actions_t;
typedef of_object_t of_instruction_id_write_metadata_t;
typedef of_object_t of_instruction_meter_t;
typedef of_object_t of_instruction_write_actions_t;
typedef of_object_t of_instruction_write_metadata_t;
typedef of_object_t of_match_v1_t;
typedef of_object_t of_match_v2_t;
typedef of_object_t of_match_v3_t;
typedef of_object_t of_meter_band_drop_t;
typedef of_object_t of_meter_band_dscp_remark_t;
typedef of_object_t of_meter_band_experimenter_t;
typedef of_object_t of_meter_band_header_t;
typedef of_object_t of_meter_band_ofdpa_color_set_t;
typedef of_object_t of_meter_band_stats_t;
typedef of_object_t of_meter_config_t;
typedef of_object_t of_meter_features_t;
typedef of_object_t of_meter_stats_t;
typedef of_object_t of_oxm_arp_op_t;
typedef of_object_t of_oxm_arp_op_masked_t;
typedef of_object_t of_oxm_arp_sha_t;
typedef of_object_t of_oxm_arp_sha_masked_t;
typedef of_object_t of_oxm_arp_spa_t;
typedef of_object_t of_oxm_arp_spa_masked_t;
typedef of_object_t of_oxm_arp_tha_t;
typedef of_object_t of_oxm_arp_tha_masked_t;
typedef of_object_t of_oxm_arp_tpa_t;
typedef of_object_t of_oxm_arp_tpa_masked_t;
typedef of_object_t of_oxm_bsn_egr_port_group_id_t;
typedef of_object_t of_oxm_bsn_egr_port_group_id_masked_t;
typedef of_object_t of_oxm_bsn_global_vrf_allowed_t;
typedef of_object_t of_oxm_bsn_global_vrf_allowed_masked_t;
typedef of_object_t of_oxm_bsn_in_ports_128_t;
typedef of_object_t of_oxm_bsn_in_ports_128_masked_t;
typedef of_object_t of_oxm_bsn_l3_dst_class_id_t;
typedef of_object_t of_oxm_bsn_l3_dst_class_id_masked_t;
typedef of_object_t of_oxm_bsn_l3_interface_class_id_t;
typedef of_object_t of_oxm_bsn_l3_interface_class_id_masked_t;
typedef of_object_t of_oxm_bsn_l3_src_class_id_t;
typedef of_object_t of_oxm_bsn_l3_src_class_id_masked_t;
typedef of_object_t of_oxm_bsn_lag_id_t;
typedef of_object_t of_oxm_bsn_lag_id_masked_t;
typedef of_object_t of_oxm_bsn_udf0_t;
typedef of_object_t of_oxm_bsn_udf0_masked_t;
typedef of_object_t of_oxm_bsn_udf1_t;
typedef of_object_t of_oxm_bsn_udf1_masked_t;
typedef of_object_t of_oxm_bsn_udf2_t;
typedef of_object_t of_oxm_bsn_udf2_masked_t;
typedef of_object_t of_oxm_bsn_udf3_t;
typedef of_object_t of_oxm_bsn_udf3_masked_t;
typedef of_object_t of_oxm_bsn_udf4_t;
typedef of_object_t of_oxm_bsn_udf4_masked_t;
typedef of_object_t of_oxm_bsn_udf5_t;
typedef of_object_t of_oxm_bsn_udf5_masked_t;
typedef of_object_t of_oxm_bsn_udf6_t;
typedef of_object_t of_oxm_bsn_udf6_masked_t;
typedef of_object_t of_oxm_bsn_udf7_t;
typedef of_object_t of_oxm_bsn_udf7_masked_t;
typedef of_object_t of_oxm_bsn_vrf_t;
typedef of_object_t of_oxm_bsn_vrf_masked_t;
typedef of_object_t of_oxm_eth_dst_t;
typedef of_object_t of_oxm_eth_dst_masked_t;
typedef of_object_t of_oxm_eth_src_t;
typedef of_object_t of_oxm_eth_src_masked_t;
typedef of_object_t of_oxm_eth_type_t;
typedef of_object_t of_oxm_eth_type_masked_t;
typedef of_object_t of_oxm_header_t;
typedef of_object_t of_oxm_icmpv4_code_t;
typedef of_object_t of_oxm_icmpv4_code_masked_t;
typedef of_object_t of_oxm_icmpv4_type_t;
typedef of_object_t of_oxm_icmpv4_type_masked_t;
typedef of_object_t of_oxm_icmpv6_code_t;
typedef of_object_t of_oxm_icmpv6_code_masked_t;
typedef of_object_t of_oxm_icmpv6_type_t;
typedef of_object_t of_oxm_icmpv6_type_masked_t;
typedef of_object_t of_oxm_in_phy_port_t;
typedef of_object_t of_oxm_in_phy_port_masked_t;
typedef of_object_t of_oxm_in_port_t;
typedef of_object_t of_oxm_in_port_masked_t;
typedef of_object_t of_oxm_ip_dscp_t;
typedef of_object_t of_oxm_ip_dscp_masked_t;
typedef of_object_t of_oxm_ip_ecn_t;
typedef of_object_t of_oxm_ip_ecn_masked_t;
typedef of_object_t of_oxm_ip_proto_t;
typedef of_object_t of_oxm_ip_proto_masked_t;
typedef of_object_t of_oxm_ipv4_dst_t;
typedef of_object_t of_oxm_ipv4_dst_masked_t;
typedef of_object_t of_oxm_ipv4_src_t;
typedef of_object_t of_oxm_ipv4_src_masked_t;
typedef of_object_t of_oxm_ipv6_dst_t;
typedef of_object_t of_oxm_ipv6_dst_masked_t;
typedef of_object_t of_oxm_ipv6_flabel_t;
typedef of_object_t of_oxm_ipv6_flabel_masked_t;
typedef of_object_t of_oxm_ipv6_nd_sll_t;
typedef of_object_t of_oxm_ipv6_nd_sll_masked_t;
typedef of_object_t of_oxm_ipv6_nd_target_t;
typedef of_object_t of_oxm_ipv6_nd_target_masked_t;
typedef of_object_t of_oxm_ipv6_nd_tll_t;
typedef of_object_t of_oxm_ipv6_nd_tll_masked_t;
typedef of_object_t of_oxm_ipv6_src_t;
typedef of_object_t of_oxm_ipv6_src_masked_t;
typedef of_object_t of_oxm_metadata_t;
typedef of_object_t of_oxm_metadata_masked_t;
typedef of_object_t of_oxm_mpls_bos_t;
typedef of_object_t of_oxm_mpls_bos_masked_t;
typedef of_object_t of_oxm_mpls_label_t;
typedef of_object_t of_oxm_mpls_label_masked_t;
typedef of_object_t of_oxm_mpls_tc_t;
typedef of_object_t of_oxm_mpls_tc_masked_t;
typedef of_object_t of_oxm_ofdpa_allow_vlan_translation_t;
typedef of_object_t of_oxm_ofdpa_color_t;
typedef of_object_t of_oxm_ofdpa_color_actions_index_t;
typedef of_object_t of_oxm_ofdpa_dei_t;
typedef of_object_t of_oxm_ofdpa_l3_in_port_t;
typedef of_object_t of_oxm_ofdpa_lmep_id_t;
typedef of_object_t of_oxm_ofdpa_mpls_ach_channel_t;
typedef of_object_t of_oxm_ofdpa_mpls_data_first_nibble_t;
typedef of_object_t of_oxm_ofdpa_mpls_l2_port_t;
typedef of_object_t of_oxm_ofdpa_mpls_l2_port_masked_t;
typedef of_object_t of_oxm_ofdpa_mpls_next_label_is_gal_t;
typedef of_object_t of_oxm_ofdpa_mpls_ttl_t;
typedef of_object_t of_oxm_ofdpa_mpls_type_t;
typedef of_object_t of_oxm_ofdpa_oam_y1731_mdl_t;
typedef of_object_t of_oxm_ofdpa_oam_y1731_opcode_t;
typedef of_object_t of_oxm_ofdpa_ovid_t;
typedef of_object_t of_oxm_ofdpa_protection_index_t;
typedef of_object_t of_oxm_ofdpa_qos_index_t;
typedef of_object_t of_oxm_ofdpa_rx_timestamp_t;
typedef of_object_t of_oxm_ofdpa_rxfcl_t;
typedef of_object_t of_oxm_ofdpa_traffic_class_t;
typedef of_object_t of_oxm_ofdpa_txfcl_t;
typedef of_object_t of_oxm_ofdpa_vrf_t;
typedef of_object_t of_oxm_onf_actset_output_t;
typedef of_object_t of_oxm_sctp_dst_t;
typedef of_object_t of_oxm_sctp_dst_masked_t;
typedef of_object_t of_oxm_sctp_src_t;
typedef of_object_t of_oxm_sctp_src_masked_t;
typedef of_object_t of_oxm_tcp_dst_t;
typedef of_object_t of_oxm_tcp_dst_masked_t;
typedef of_object_t of_oxm_tcp_src_t;
typedef of_object_t of_oxm_tcp_src_masked_t;
typedef of_object_t of_oxm_tunnel_id_t;
typedef of_object_t of_oxm_tunnel_id_masked_t;
typedef of_object_t of_oxm_udp_dst_t;
typedef of_object_t of_oxm_udp_dst_masked_t;
typedef of_object_t of_oxm_udp_src_t;
typedef of_object_t of_oxm_udp_src_masked_t;
typedef of_object_t of_oxm_vlan_pcp_t;
typedef of_object_t of_oxm_vlan_pcp_masked_t;
typedef of_object_t of_oxm_vlan_vid_t;
typedef of_object_t of_oxm_vlan_vid_masked_t;
typedef of_object_t of_packet_queue_t;
typedef of_object_t of_port_desc_t;
typedef of_object_t of_port_stats_entry_t;
typedef of_object_t of_queue_prop_experimenter_t;
typedef of_object_t of_queue_prop_header_t;
typedef of_object_t of_queue_prop_max_rate_t;
typedef of_object_t of_queue_prop_min_rate_t;
typedef of_object_t of_queue_stats_entry_t;
typedef of_object_t of_table_feature_prop_apply_actions_t;
typedef of_object_t of_table_feature_prop_apply_actions_miss_t;
typedef of_object_t of_table_feature_prop_apply_setfield_t;
typedef of_object_t of_table_feature_prop_apply_setfield_miss_t;
typedef of_object_t of_table_feature_prop_experimenter_t;
typedef of_object_t of_table_feature_prop_experimenter_miss_t;
typedef of_object_t of_table_feature_prop_header_t;
typedef of_object_t of_table_feature_prop_instructions_t;
typedef of_object_t of_table_feature_prop_instructions_miss_t;
typedef of_object_t of_table_feature_prop_match_t;
typedef of_object_t of_table_feature_prop_next_tables_t;
typedef of_object_t of_table_feature_prop_next_tables_miss_t;
typedef of_object_t of_table_feature_prop_wildcards_t;
typedef of_object_t of_table_feature_prop_write_actions_t;
typedef of_object_t of_table_feature_prop_write_actions_miss_t;
typedef of_object_t of_table_feature_prop_write_setfield_t;
typedef of_object_t of_table_feature_prop_write_setfield_miss_t;
typedef of_object_t of_table_features_t;
typedef of_object_t of_table_stats_entry_t;
typedef of_object_t of_uint32_t;
typedef of_object_t of_uint64_t;
typedef of_object_t of_uint8_t;
typedef of_object_t ofp_queue_desc_t;
typedef of_object_t onf_oxm_packet_regs0_t;
typedef of_object_t onf_oxm_packet_regs1_t;
typedef of_object_t onf_oxm_packet_regs2_t;
typedef of_object_t onf_oxm_packet_regs3_t;
typedef of_object_t of_list_action_t;
typedef of_object_t of_list_action_id_t;
typedef of_object_t of_list_bsn_controller_connection_t;
typedef of_object_t of_list_bsn_debug_counter_desc_stats_entry_t;
typedef of_object_t of_list_bsn_debug_counter_stats_entry_t;
typedef of_object_t of_list_bsn_flow_checksum_bucket_stats_entry_t;
typedef of_object_t of_list_bsn_gentable_bucket_stats_entry_t;
typedef of_object_t of_list_bsn_gentable_desc_stats_entry_t;
typedef of_object_t of_list_bsn_gentable_entry_desc_stats_entry_t;
typedef of_object_t of_list_bsn_gentable_entry_stats_entry_t;
typedef of_object_t of_list_bsn_gentable_stats_entry_t;
typedef of_object_t of_list_bsn_interface_t;
typedef of_object_t of_list_bsn_lacp_stats_entry_t;
typedef of_object_t of_list_bsn_port_counter_stats_entry_t;
typedef of_object_t of_list_bsn_switch_pipeline_stats_entry_t;
typedef of_object_t of_list_bsn_table_checksum_stats_entry_t;
typedef of_object_t of_list_bsn_tlv_t;
typedef of_object_t of_list_bsn_vlan_counter_stats_entry_t;
typedef of_object_t of_list_bucket_t;
typedef of_object_t of_list_bucket_counter_t;
typedef of_object_t of_list_flow_stats_entry_t;
typedef of_object_t of_list_group_desc_stats_entry_t;
typedef of_object_t of_list_group_stats_entry_t;
typedef of_object_t of_list_hello_elem_t;
typedef of_object_t of_list_instruction_t;
typedef of_object_t of_list_instruction_id_t;
typedef of_object_t of_list_meter_band_t;
typedef of_object_t of_list_meter_band_stats_t;
typedef of_object_t of_list_meter_stats_t;
typedef of_object_t of_list_oxm_t;
typedef of_object_t of_list_packet_queue_t;
typedef of_object_t of_list_port_desc_t;
typedef of_object_t of_list_port_stats_entry_t;
typedef of_object_t of_list_queue_prop_t;
typedef of_object_t of_list_queue_stats_entry_t;
typedef of_object_t of_list_table_feature_prop_t;
typedef of_object_t of_list_table_features_t;
typedef of_object_t of_list_table_stats_entry_t;
typedef of_object_t of_list_uint32_t;
typedef of_object_t of_list_uint64_t;
typedef of_object_t of_list_uint8_t;

/****************************************************************
 *
 * Additional of_object defines
 * These are needed for some static inline ops, so we put them here.
 *
 ****************************************************************/

/* Delete an OpenFlow object without reference to its type */
extern void of_object_delete(of_object_t *obj);


/****************************************************************
 *
 * New operator declarations
 *
 * _new: Create a new object for writing; includes init
 * _new_from_message: Create a new instance of the object and bind the
 *    message data to the object
 * _init: Initialize and optionally allocate buffer space for an
 *    automatic instance
 *
 * _new and _from_message require a delete operation to be called
 * on the object.
 *
 ****************************************************************/

extern of_aggregate_stats_reply_t *
    of_aggregate_stats_reply_new(of_version_t version);
extern of_aggregate_stats_reply_t *
    of_aggregate_stats_reply_new_from_message(of_message_t msg);
extern void of_aggregate_stats_reply_init(
    of_aggregate_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_aggregate_stats_request_t *
    of_aggregate_stats_request_new(of_version_t version);
extern of_aggregate_stats_request_t *
    of_aggregate_stats_request_new_from_message(of_message_t msg);
extern void of_aggregate_stats_request_init(
    of_aggregate_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_async_get_reply_t *
    of_async_get_reply_new(of_version_t version);
extern of_async_get_reply_t *
    of_async_get_reply_new_from_message(of_message_t msg);
extern void of_async_get_reply_init(
    of_async_get_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_async_get_request_t *
    of_async_get_request_new(of_version_t version);
extern of_async_get_request_t *
    of_async_get_request_new_from_message(of_message_t msg);
extern void of_async_get_request_init(
    of_async_get_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_async_set_t *
    of_async_set_new(of_version_t version);
extern of_async_set_t *
    of_async_set_new_from_message(of_message_t msg);
extern void of_async_set_init(
    of_async_set_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bad_action_error_msg_t *
    of_bad_action_error_msg_new(of_version_t version);
extern of_bad_action_error_msg_t *
    of_bad_action_error_msg_new_from_message(of_message_t msg);
extern void of_bad_action_error_msg_init(
    of_bad_action_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bad_instruction_error_msg_t *
    of_bad_instruction_error_msg_new(of_version_t version);
extern of_bad_instruction_error_msg_t *
    of_bad_instruction_error_msg_new_from_message(of_message_t msg);
extern void of_bad_instruction_error_msg_init(
    of_bad_instruction_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bad_match_error_msg_t *
    of_bad_match_error_msg_new(of_version_t version);
extern of_bad_match_error_msg_t *
    of_bad_match_error_msg_new_from_message(of_message_t msg);
extern void of_bad_match_error_msg_init(
    of_bad_match_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bad_request_error_msg_t *
    of_bad_request_error_msg_new(of_version_t version);
extern of_bad_request_error_msg_t *
    of_bad_request_error_msg_new_from_message(of_message_t msg);
extern void of_bad_request_error_msg_init(
    of_bad_request_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_barrier_reply_t *
    of_barrier_reply_new(of_version_t version);
extern of_barrier_reply_t *
    of_barrier_reply_new_from_message(of_message_t msg);
extern void of_barrier_reply_init(
    of_barrier_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_barrier_request_t *
    of_barrier_request_new(of_version_t version);
extern of_barrier_request_t *
    of_barrier_request_new_from_message(of_message_t msg);
extern void of_barrier_request_init(
    of_barrier_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_arp_idle_t *
    of_bsn_arp_idle_new(of_version_t version);
extern of_bsn_arp_idle_t *
    of_bsn_arp_idle_new_from_message(of_message_t msg);
extern void of_bsn_arp_idle_init(
    of_bsn_arp_idle_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_bw_clear_data_reply_t *
    of_bsn_bw_clear_data_reply_new(of_version_t version);
extern of_bsn_bw_clear_data_reply_t *
    of_bsn_bw_clear_data_reply_new_from_message(of_message_t msg);
extern void of_bsn_bw_clear_data_reply_init(
    of_bsn_bw_clear_data_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_bw_clear_data_request_t *
    of_bsn_bw_clear_data_request_new(of_version_t version);
extern of_bsn_bw_clear_data_request_t *
    of_bsn_bw_clear_data_request_new_from_message(of_message_t msg);
extern void of_bsn_bw_clear_data_request_init(
    of_bsn_bw_clear_data_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_bw_enable_get_reply_t *
    of_bsn_bw_enable_get_reply_new(of_version_t version);
extern of_bsn_bw_enable_get_reply_t *
    of_bsn_bw_enable_get_reply_new_from_message(of_message_t msg);
extern void of_bsn_bw_enable_get_reply_init(
    of_bsn_bw_enable_get_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_bw_enable_get_request_t *
    of_bsn_bw_enable_get_request_new(of_version_t version);
extern of_bsn_bw_enable_get_request_t *
    of_bsn_bw_enable_get_request_new_from_message(of_message_t msg);
extern void of_bsn_bw_enable_get_request_init(
    of_bsn_bw_enable_get_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_bw_enable_set_reply_t *
    of_bsn_bw_enable_set_reply_new(of_version_t version);
extern of_bsn_bw_enable_set_reply_t *
    of_bsn_bw_enable_set_reply_new_from_message(of_message_t msg);
extern void of_bsn_bw_enable_set_reply_init(
    of_bsn_bw_enable_set_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_bw_enable_set_request_t *
    of_bsn_bw_enable_set_request_new(of_version_t version);
extern of_bsn_bw_enable_set_request_t *
    of_bsn_bw_enable_set_request_new_from_message(of_message_t msg);
extern void of_bsn_bw_enable_set_request_init(
    of_bsn_bw_enable_set_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_controller_connections_reply_t *
    of_bsn_controller_connections_reply_new(of_version_t version);
extern of_bsn_controller_connections_reply_t *
    of_bsn_controller_connections_reply_new_from_message(of_message_t msg);
extern void of_bsn_controller_connections_reply_init(
    of_bsn_controller_connections_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_controller_connections_request_t *
    of_bsn_controller_connections_request_new(of_version_t version);
extern of_bsn_controller_connections_request_t *
    of_bsn_controller_connections_request_new_from_message(of_message_t msg);
extern void of_bsn_controller_connections_request_init(
    of_bsn_controller_connections_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_debug_counter_desc_stats_reply_t *
    of_bsn_debug_counter_desc_stats_reply_new(of_version_t version);
extern of_bsn_debug_counter_desc_stats_reply_t *
    of_bsn_debug_counter_desc_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_debug_counter_desc_stats_reply_init(
    of_bsn_debug_counter_desc_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_debug_counter_desc_stats_request_t *
    of_bsn_debug_counter_desc_stats_request_new(of_version_t version);
extern of_bsn_debug_counter_desc_stats_request_t *
    of_bsn_debug_counter_desc_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_debug_counter_desc_stats_request_init(
    of_bsn_debug_counter_desc_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_debug_counter_stats_reply_t *
    of_bsn_debug_counter_stats_reply_new(of_version_t version);
extern of_bsn_debug_counter_stats_reply_t *
    of_bsn_debug_counter_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_debug_counter_stats_reply_init(
    of_bsn_debug_counter_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_debug_counter_stats_request_t *
    of_bsn_debug_counter_stats_request_new(of_version_t version);
extern of_bsn_debug_counter_stats_request_t *
    of_bsn_debug_counter_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_debug_counter_stats_request_init(
    of_bsn_debug_counter_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_flow_checksum_bucket_stats_reply_t *
    of_bsn_flow_checksum_bucket_stats_reply_new(of_version_t version);
extern of_bsn_flow_checksum_bucket_stats_reply_t *
    of_bsn_flow_checksum_bucket_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_flow_checksum_bucket_stats_reply_init(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_flow_checksum_bucket_stats_request_t *
    of_bsn_flow_checksum_bucket_stats_request_new(of_version_t version);
extern of_bsn_flow_checksum_bucket_stats_request_t *
    of_bsn_flow_checksum_bucket_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_flow_checksum_bucket_stats_request_init(
    of_bsn_flow_checksum_bucket_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_flow_idle_t *
    of_bsn_flow_idle_new(of_version_t version);
extern of_bsn_flow_idle_t *
    of_bsn_flow_idle_new_from_message(of_message_t msg);
extern void of_bsn_flow_idle_init(
    of_bsn_flow_idle_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_flow_idle_enable_get_reply_t *
    of_bsn_flow_idle_enable_get_reply_new(of_version_t version);
extern of_bsn_flow_idle_enable_get_reply_t *
    of_bsn_flow_idle_enable_get_reply_new_from_message(of_message_t msg);
extern void of_bsn_flow_idle_enable_get_reply_init(
    of_bsn_flow_idle_enable_get_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_flow_idle_enable_get_request_t *
    of_bsn_flow_idle_enable_get_request_new(of_version_t version);
extern of_bsn_flow_idle_enable_get_request_t *
    of_bsn_flow_idle_enable_get_request_new_from_message(of_message_t msg);
extern void of_bsn_flow_idle_enable_get_request_init(
    of_bsn_flow_idle_enable_get_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_flow_idle_enable_set_reply_t *
    of_bsn_flow_idle_enable_set_reply_new(of_version_t version);
extern of_bsn_flow_idle_enable_set_reply_t *
    of_bsn_flow_idle_enable_set_reply_new_from_message(of_message_t msg);
extern void of_bsn_flow_idle_enable_set_reply_init(
    of_bsn_flow_idle_enable_set_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_flow_idle_enable_set_request_t *
    of_bsn_flow_idle_enable_set_request_new(of_version_t version);
extern of_bsn_flow_idle_enable_set_request_t *
    of_bsn_flow_idle_enable_set_request_new_from_message(of_message_t msg);
extern void of_bsn_flow_idle_enable_set_request_init(
    of_bsn_flow_idle_enable_set_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_bucket_stats_reply_t *
    of_bsn_gentable_bucket_stats_reply_new(of_version_t version);
extern of_bsn_gentable_bucket_stats_reply_t *
    of_bsn_gentable_bucket_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_gentable_bucket_stats_reply_init(
    of_bsn_gentable_bucket_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_bucket_stats_request_t *
    of_bsn_gentable_bucket_stats_request_new(of_version_t version);
extern of_bsn_gentable_bucket_stats_request_t *
    of_bsn_gentable_bucket_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_gentable_bucket_stats_request_init(
    of_bsn_gentable_bucket_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_clear_reply_t *
    of_bsn_gentable_clear_reply_new(of_version_t version);
extern of_bsn_gentable_clear_reply_t *
    of_bsn_gentable_clear_reply_new_from_message(of_message_t msg);
extern void of_bsn_gentable_clear_reply_init(
    of_bsn_gentable_clear_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_clear_request_t *
    of_bsn_gentable_clear_request_new(of_version_t version);
extern of_bsn_gentable_clear_request_t *
    of_bsn_gentable_clear_request_new_from_message(of_message_t msg);
extern void of_bsn_gentable_clear_request_init(
    of_bsn_gentable_clear_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_desc_stats_reply_t *
    of_bsn_gentable_desc_stats_reply_new(of_version_t version);
extern of_bsn_gentable_desc_stats_reply_t *
    of_bsn_gentable_desc_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_gentable_desc_stats_reply_init(
    of_bsn_gentable_desc_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_desc_stats_request_t *
    of_bsn_gentable_desc_stats_request_new(of_version_t version);
extern of_bsn_gentable_desc_stats_request_t *
    of_bsn_gentable_desc_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_gentable_desc_stats_request_init(
    of_bsn_gentable_desc_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_entry_add_t *
    of_bsn_gentable_entry_add_new(of_version_t version);
extern of_bsn_gentable_entry_add_t *
    of_bsn_gentable_entry_add_new_from_message(of_message_t msg);
extern void of_bsn_gentable_entry_add_init(
    of_bsn_gentable_entry_add_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_entry_delete_t *
    of_bsn_gentable_entry_delete_new(of_version_t version);
extern of_bsn_gentable_entry_delete_t *
    of_bsn_gentable_entry_delete_new_from_message(of_message_t msg);
extern void of_bsn_gentable_entry_delete_init(
    of_bsn_gentable_entry_delete_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_entry_desc_stats_reply_t *
    of_bsn_gentable_entry_desc_stats_reply_new(of_version_t version);
extern of_bsn_gentable_entry_desc_stats_reply_t *
    of_bsn_gentable_entry_desc_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_gentable_entry_desc_stats_reply_init(
    of_bsn_gentable_entry_desc_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_entry_desc_stats_request_t *
    of_bsn_gentable_entry_desc_stats_request_new(of_version_t version);
extern of_bsn_gentable_entry_desc_stats_request_t *
    of_bsn_gentable_entry_desc_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_gentable_entry_desc_stats_request_init(
    of_bsn_gentable_entry_desc_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_entry_stats_reply_t *
    of_bsn_gentable_entry_stats_reply_new(of_version_t version);
extern of_bsn_gentable_entry_stats_reply_t *
    of_bsn_gentable_entry_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_gentable_entry_stats_reply_init(
    of_bsn_gentable_entry_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_entry_stats_request_t *
    of_bsn_gentable_entry_stats_request_new(of_version_t version);
extern of_bsn_gentable_entry_stats_request_t *
    of_bsn_gentable_entry_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_gentable_entry_stats_request_init(
    of_bsn_gentable_entry_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_set_buckets_size_t *
    of_bsn_gentable_set_buckets_size_new(of_version_t version);
extern of_bsn_gentable_set_buckets_size_t *
    of_bsn_gentable_set_buckets_size_new_from_message(of_message_t msg);
extern void of_bsn_gentable_set_buckets_size_init(
    of_bsn_gentable_set_buckets_size_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_stats_reply_t *
    of_bsn_gentable_stats_reply_new(of_version_t version);
extern of_bsn_gentable_stats_reply_t *
    of_bsn_gentable_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_gentable_stats_reply_init(
    of_bsn_gentable_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_stats_request_t *
    of_bsn_gentable_stats_request_new(of_version_t version);
extern of_bsn_gentable_stats_request_t *
    of_bsn_gentable_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_gentable_stats_request_init(
    of_bsn_gentable_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_interfaces_reply_t *
    of_bsn_get_interfaces_reply_new(of_version_t version);
extern of_bsn_get_interfaces_reply_t *
    of_bsn_get_interfaces_reply_new_from_message(of_message_t msg);
extern void of_bsn_get_interfaces_reply_init(
    of_bsn_get_interfaces_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_interfaces_request_t *
    of_bsn_get_interfaces_request_new(of_version_t version);
extern of_bsn_get_interfaces_request_t *
    of_bsn_get_interfaces_request_new_from_message(of_message_t msg);
extern void of_bsn_get_interfaces_request_init(
    of_bsn_get_interfaces_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_ip_mask_reply_t *
    of_bsn_get_ip_mask_reply_new(of_version_t version);
extern of_bsn_get_ip_mask_reply_t *
    of_bsn_get_ip_mask_reply_new_from_message(of_message_t msg);
extern void of_bsn_get_ip_mask_reply_init(
    of_bsn_get_ip_mask_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_ip_mask_request_t *
    of_bsn_get_ip_mask_request_new(of_version_t version);
extern of_bsn_get_ip_mask_request_t *
    of_bsn_get_ip_mask_request_new_from_message(of_message_t msg);
extern void of_bsn_get_ip_mask_request_init(
    of_bsn_get_ip_mask_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_l2_table_reply_t *
    of_bsn_get_l2_table_reply_new(of_version_t version);
extern of_bsn_get_l2_table_reply_t *
    of_bsn_get_l2_table_reply_new_from_message(of_message_t msg);
extern void of_bsn_get_l2_table_reply_init(
    of_bsn_get_l2_table_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_l2_table_request_t *
    of_bsn_get_l2_table_request_new(of_version_t version);
extern of_bsn_get_l2_table_request_t *
    of_bsn_get_l2_table_request_new_from_message(of_message_t msg);
extern void of_bsn_get_l2_table_request_init(
    of_bsn_get_l2_table_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_mirroring_reply_t *
    of_bsn_get_mirroring_reply_new(of_version_t version);
extern of_bsn_get_mirroring_reply_t *
    of_bsn_get_mirroring_reply_new_from_message(of_message_t msg);
extern void of_bsn_get_mirroring_reply_init(
    of_bsn_get_mirroring_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_mirroring_request_t *
    of_bsn_get_mirroring_request_new(of_version_t version);
extern of_bsn_get_mirroring_request_t *
    of_bsn_get_mirroring_request_new_from_message(of_message_t msg);
extern void of_bsn_get_mirroring_request_init(
    of_bsn_get_mirroring_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_switch_pipeline_reply_t *
    of_bsn_get_switch_pipeline_reply_new(of_version_t version);
extern of_bsn_get_switch_pipeline_reply_t *
    of_bsn_get_switch_pipeline_reply_new_from_message(of_message_t msg);
extern void of_bsn_get_switch_pipeline_reply_init(
    of_bsn_get_switch_pipeline_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_get_switch_pipeline_request_t *
    of_bsn_get_switch_pipeline_request_new(of_version_t version);
extern of_bsn_get_switch_pipeline_request_t *
    of_bsn_get_switch_pipeline_request_new_from_message(of_message_t msg);
extern void of_bsn_get_switch_pipeline_request_init(
    of_bsn_get_switch_pipeline_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_header_t *
    of_bsn_header_new(of_version_t version);
extern of_bsn_header_t *
    of_bsn_header_new_from_message(of_message_t msg);
extern void of_bsn_header_init(
    of_bsn_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_hybrid_get_reply_t *
    of_bsn_hybrid_get_reply_new(of_version_t version);
extern of_bsn_hybrid_get_reply_t *
    of_bsn_hybrid_get_reply_new_from_message(of_message_t msg);
extern void of_bsn_hybrid_get_reply_init(
    of_bsn_hybrid_get_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_hybrid_get_request_t *
    of_bsn_hybrid_get_request_new(of_version_t version);
extern of_bsn_hybrid_get_request_t *
    of_bsn_hybrid_get_request_new_from_message(of_message_t msg);
extern void of_bsn_hybrid_get_request_init(
    of_bsn_hybrid_get_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_lacp_convergence_notif_t *
    of_bsn_lacp_convergence_notif_new(of_version_t version);
extern of_bsn_lacp_convergence_notif_t *
    of_bsn_lacp_convergence_notif_new_from_message(of_message_t msg);
extern void of_bsn_lacp_convergence_notif_init(
    of_bsn_lacp_convergence_notif_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_lacp_stats_reply_t *
    of_bsn_lacp_stats_reply_new(of_version_t version);
extern of_bsn_lacp_stats_reply_t *
    of_bsn_lacp_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_lacp_stats_reply_init(
    of_bsn_lacp_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_lacp_stats_request_t *
    of_bsn_lacp_stats_request_new(of_version_t version);
extern of_bsn_lacp_stats_request_t *
    of_bsn_lacp_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_lacp_stats_request_init(
    of_bsn_lacp_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_pdu_rx_reply_t *
    of_bsn_pdu_rx_reply_new(of_version_t version);
extern of_bsn_pdu_rx_reply_t *
    of_bsn_pdu_rx_reply_new_from_message(of_message_t msg);
extern void of_bsn_pdu_rx_reply_init(
    of_bsn_pdu_rx_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_pdu_rx_request_t *
    of_bsn_pdu_rx_request_new(of_version_t version);
extern of_bsn_pdu_rx_request_t *
    of_bsn_pdu_rx_request_new_from_message(of_message_t msg);
extern void of_bsn_pdu_rx_request_init(
    of_bsn_pdu_rx_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_pdu_rx_timeout_t *
    of_bsn_pdu_rx_timeout_new(of_version_t version);
extern of_bsn_pdu_rx_timeout_t *
    of_bsn_pdu_rx_timeout_new_from_message(of_message_t msg);
extern void of_bsn_pdu_rx_timeout_init(
    of_bsn_pdu_rx_timeout_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_pdu_tx_reply_t *
    of_bsn_pdu_tx_reply_new(of_version_t version);
extern of_bsn_pdu_tx_reply_t *
    of_bsn_pdu_tx_reply_new_from_message(of_message_t msg);
extern void of_bsn_pdu_tx_reply_init(
    of_bsn_pdu_tx_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_pdu_tx_request_t *
    of_bsn_pdu_tx_request_new(of_version_t version);
extern of_bsn_pdu_tx_request_t *
    of_bsn_pdu_tx_request_new_from_message(of_message_t msg);
extern void of_bsn_pdu_tx_request_init(
    of_bsn_pdu_tx_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_port_counter_stats_reply_t *
    of_bsn_port_counter_stats_reply_new(of_version_t version);
extern of_bsn_port_counter_stats_reply_t *
    of_bsn_port_counter_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_port_counter_stats_reply_init(
    of_bsn_port_counter_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_port_counter_stats_request_t *
    of_bsn_port_counter_stats_request_new(of_version_t version);
extern of_bsn_port_counter_stats_request_t *
    of_bsn_port_counter_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_port_counter_stats_request_init(
    of_bsn_port_counter_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_role_status_t *
    of_bsn_role_status_new(of_version_t version);
extern of_bsn_role_status_t *
    of_bsn_role_status_new_from_message(of_message_t msg);
extern void of_bsn_role_status_init(
    of_bsn_role_status_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_aux_cxns_reply_t *
    of_bsn_set_aux_cxns_reply_new(of_version_t version);
extern of_bsn_set_aux_cxns_reply_t *
    of_bsn_set_aux_cxns_reply_new_from_message(of_message_t msg);
extern void of_bsn_set_aux_cxns_reply_init(
    of_bsn_set_aux_cxns_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_aux_cxns_request_t *
    of_bsn_set_aux_cxns_request_new(of_version_t version);
extern of_bsn_set_aux_cxns_request_t *
    of_bsn_set_aux_cxns_request_new_from_message(of_message_t msg);
extern void of_bsn_set_aux_cxns_request_init(
    of_bsn_set_aux_cxns_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_ip_mask_t *
    of_bsn_set_ip_mask_new(of_version_t version);
extern of_bsn_set_ip_mask_t *
    of_bsn_set_ip_mask_new_from_message(of_message_t msg);
extern void of_bsn_set_ip_mask_init(
    of_bsn_set_ip_mask_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_l2_table_reply_t *
    of_bsn_set_l2_table_reply_new(of_version_t version);
extern of_bsn_set_l2_table_reply_t *
    of_bsn_set_l2_table_reply_new_from_message(of_message_t msg);
extern void of_bsn_set_l2_table_reply_init(
    of_bsn_set_l2_table_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_l2_table_request_t *
    of_bsn_set_l2_table_request_new(of_version_t version);
extern of_bsn_set_l2_table_request_t *
    of_bsn_set_l2_table_request_new_from_message(of_message_t msg);
extern void of_bsn_set_l2_table_request_init(
    of_bsn_set_l2_table_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_lacp_reply_t *
    of_bsn_set_lacp_reply_new(of_version_t version);
extern of_bsn_set_lacp_reply_t *
    of_bsn_set_lacp_reply_new_from_message(of_message_t msg);
extern void of_bsn_set_lacp_reply_init(
    of_bsn_set_lacp_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_lacp_request_t *
    of_bsn_set_lacp_request_new(of_version_t version);
extern of_bsn_set_lacp_request_t *
    of_bsn_set_lacp_request_new_from_message(of_message_t msg);
extern void of_bsn_set_lacp_request_init(
    of_bsn_set_lacp_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_mirroring_t *
    of_bsn_set_mirroring_new(of_version_t version);
extern of_bsn_set_mirroring_t *
    of_bsn_set_mirroring_new_from_message(of_message_t msg);
extern void of_bsn_set_mirroring_init(
    of_bsn_set_mirroring_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_pktin_suppression_reply_t *
    of_bsn_set_pktin_suppression_reply_new(of_version_t version);
extern of_bsn_set_pktin_suppression_reply_t *
    of_bsn_set_pktin_suppression_reply_new_from_message(of_message_t msg);
extern void of_bsn_set_pktin_suppression_reply_init(
    of_bsn_set_pktin_suppression_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_pktin_suppression_request_t *
    of_bsn_set_pktin_suppression_request_new(of_version_t version);
extern of_bsn_set_pktin_suppression_request_t *
    of_bsn_set_pktin_suppression_request_new_from_message(of_message_t msg);
extern void of_bsn_set_pktin_suppression_request_init(
    of_bsn_set_pktin_suppression_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_switch_pipeline_reply_t *
    of_bsn_set_switch_pipeline_reply_new(of_version_t version);
extern of_bsn_set_switch_pipeline_reply_t *
    of_bsn_set_switch_pipeline_reply_new_from_message(of_message_t msg);
extern void of_bsn_set_switch_pipeline_reply_init(
    of_bsn_set_switch_pipeline_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_set_switch_pipeline_request_t *
    of_bsn_set_switch_pipeline_request_new(of_version_t version);
extern of_bsn_set_switch_pipeline_request_t *
    of_bsn_set_switch_pipeline_request_new_from_message(of_message_t msg);
extern void of_bsn_set_switch_pipeline_request_init(
    of_bsn_set_switch_pipeline_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_shell_command_t *
    of_bsn_shell_command_new(of_version_t version);
extern of_bsn_shell_command_t *
    of_bsn_shell_command_new_from_message(of_message_t msg);
extern void of_bsn_shell_command_init(
    of_bsn_shell_command_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_shell_output_t *
    of_bsn_shell_output_new(of_version_t version);
extern of_bsn_shell_output_t *
    of_bsn_shell_output_new_from_message(of_message_t msg);
extern void of_bsn_shell_output_init(
    of_bsn_shell_output_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_shell_status_t *
    of_bsn_shell_status_new(of_version_t version);
extern of_bsn_shell_status_t *
    of_bsn_shell_status_new_from_message(of_message_t msg);
extern void of_bsn_shell_status_init(
    of_bsn_shell_status_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_stats_reply_t *
    of_bsn_stats_reply_new(of_version_t version);
extern of_bsn_stats_reply_t *
    of_bsn_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_stats_reply_init(
    of_bsn_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_stats_request_t *
    of_bsn_stats_request_new(of_version_t version);
extern of_bsn_stats_request_t *
    of_bsn_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_stats_request_init(
    of_bsn_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_switch_pipeline_stats_reply_t *
    of_bsn_switch_pipeline_stats_reply_new(of_version_t version);
extern of_bsn_switch_pipeline_stats_reply_t *
    of_bsn_switch_pipeline_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_switch_pipeline_stats_reply_init(
    of_bsn_switch_pipeline_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_switch_pipeline_stats_request_t *
    of_bsn_switch_pipeline_stats_request_new(of_version_t version);
extern of_bsn_switch_pipeline_stats_request_t *
    of_bsn_switch_pipeline_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_switch_pipeline_stats_request_init(
    of_bsn_switch_pipeline_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_table_checksum_stats_reply_t *
    of_bsn_table_checksum_stats_reply_new(of_version_t version);
extern of_bsn_table_checksum_stats_reply_t *
    of_bsn_table_checksum_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_table_checksum_stats_reply_init(
    of_bsn_table_checksum_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_table_checksum_stats_request_t *
    of_bsn_table_checksum_stats_request_new(of_version_t version);
extern of_bsn_table_checksum_stats_request_t *
    of_bsn_table_checksum_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_table_checksum_stats_request_init(
    of_bsn_table_checksum_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_table_set_buckets_size_t *
    of_bsn_table_set_buckets_size_new(of_version_t version);
extern of_bsn_table_set_buckets_size_t *
    of_bsn_table_set_buckets_size_new_from_message(of_message_t msg);
extern void of_bsn_table_set_buckets_size_init(
    of_bsn_table_set_buckets_size_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_time_reply_t *
    of_bsn_time_reply_new(of_version_t version);
extern of_bsn_time_reply_t *
    of_bsn_time_reply_new_from_message(of_message_t msg);
extern void of_bsn_time_reply_init(
    of_bsn_time_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_time_request_t *
    of_bsn_time_request_new(of_version_t version);
extern of_bsn_time_request_t *
    of_bsn_time_request_new_from_message(of_message_t msg);
extern void of_bsn_time_request_init(
    of_bsn_time_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_virtual_port_create_reply_t *
    of_bsn_virtual_port_create_reply_new(of_version_t version);
extern of_bsn_virtual_port_create_reply_t *
    of_bsn_virtual_port_create_reply_new_from_message(of_message_t msg);
extern void of_bsn_virtual_port_create_reply_init(
    of_bsn_virtual_port_create_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_virtual_port_create_request_t *
    of_bsn_virtual_port_create_request_new(of_version_t version);
extern of_bsn_virtual_port_create_request_t *
    of_bsn_virtual_port_create_request_new_from_message(of_message_t msg);
extern void of_bsn_virtual_port_create_request_init(
    of_bsn_virtual_port_create_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_virtual_port_remove_reply_t *
    of_bsn_virtual_port_remove_reply_new(of_version_t version);
extern of_bsn_virtual_port_remove_reply_t *
    of_bsn_virtual_port_remove_reply_new_from_message(of_message_t msg);
extern void of_bsn_virtual_port_remove_reply_init(
    of_bsn_virtual_port_remove_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_virtual_port_remove_request_t *
    of_bsn_virtual_port_remove_request_new(of_version_t version);
extern of_bsn_virtual_port_remove_request_t *
    of_bsn_virtual_port_remove_request_new_from_message(of_message_t msg);
extern void of_bsn_virtual_port_remove_request_init(
    of_bsn_virtual_port_remove_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_vlan_counter_stats_reply_t *
    of_bsn_vlan_counter_stats_reply_new(of_version_t version);
extern of_bsn_vlan_counter_stats_reply_t *
    of_bsn_vlan_counter_stats_reply_new_from_message(of_message_t msg);
extern void of_bsn_vlan_counter_stats_reply_init(
    of_bsn_vlan_counter_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_vlan_counter_stats_request_t *
    of_bsn_vlan_counter_stats_request_new(of_version_t version);
extern of_bsn_vlan_counter_stats_request_t *
    of_bsn_vlan_counter_stats_request_new_from_message(of_message_t msg);
extern void of_bsn_vlan_counter_stats_request_init(
    of_bsn_vlan_counter_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_desc_stats_reply_t *
    of_desc_stats_reply_new(of_version_t version);
extern of_desc_stats_reply_t *
    of_desc_stats_reply_new_from_message(of_message_t msg);
extern void of_desc_stats_reply_init(
    of_desc_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_desc_stats_request_t *
    of_desc_stats_request_new(of_version_t version);
extern of_desc_stats_request_t *
    of_desc_stats_request_new_from_message(of_message_t msg);
extern void of_desc_stats_request_init(
    of_desc_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_echo_reply_t *
    of_echo_reply_new(of_version_t version);
extern of_echo_reply_t *
    of_echo_reply_new_from_message(of_message_t msg);
extern void of_echo_reply_init(
    of_echo_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_echo_request_t *
    of_echo_request_new(of_version_t version);
extern of_echo_request_t *
    of_echo_request_new_from_message(of_message_t msg);
extern void of_echo_request_init(
    of_echo_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_error_msg_t *
    of_error_msg_new(of_version_t version);
extern of_error_msg_t *
    of_error_msg_new_from_message(of_message_t msg);
extern void of_error_msg_init(
    of_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_experimenter_t *
    of_experimenter_new(of_version_t version);
extern of_experimenter_t *
    of_experimenter_new_from_message(of_message_t msg);
extern void of_experimenter_init(
    of_experimenter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_experimenter_error_msg_t *
    of_experimenter_error_msg_new(of_version_t version);
extern of_experimenter_error_msg_t *
    of_experimenter_error_msg_new_from_message(of_message_t msg);
extern void of_experimenter_error_msg_init(
    of_experimenter_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_experimenter_multipart_reply_ofdpa_t *
    of_experimenter_multipart_reply_ofdpa_new(of_version_t version);
extern of_experimenter_multipart_reply_ofdpa_t *
    of_experimenter_multipart_reply_ofdpa_new_from_message(of_message_t msg);
extern void of_experimenter_multipart_reply_ofdpa_init(
    of_experimenter_multipart_reply_ofdpa_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_experimenter_multipart_request_ofdpa_t *
    of_experimenter_multipart_request_ofdpa_new(of_version_t version);
extern of_experimenter_multipart_request_ofdpa_t *
    of_experimenter_multipart_request_ofdpa_new_from_message(of_message_t msg);
extern void of_experimenter_multipart_request_ofdpa_init(
    of_experimenter_multipart_request_ofdpa_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_experimenter_ofdpa_t *
    of_experimenter_ofdpa_new(of_version_t version);
extern of_experimenter_ofdpa_t *
    of_experimenter_ofdpa_new_from_message(of_message_t msg);
extern void of_experimenter_ofdpa_init(
    of_experimenter_ofdpa_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_experimenter_stats_reply_t *
    of_experimenter_stats_reply_new(of_version_t version);
extern of_experimenter_stats_reply_t *
    of_experimenter_stats_reply_new_from_message(of_message_t msg);
extern void of_experimenter_stats_reply_init(
    of_experimenter_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_experimenter_stats_request_t *
    of_experimenter_stats_request_new(of_version_t version);
extern of_experimenter_stats_request_t *
    of_experimenter_stats_request_new_from_message(of_message_t msg);
extern void of_experimenter_stats_request_init(
    of_experimenter_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_features_reply_t *
    of_features_reply_new(of_version_t version);
extern of_features_reply_t *
    of_features_reply_new_from_message(of_message_t msg);
extern void of_features_reply_init(
    of_features_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_features_request_t *
    of_features_request_new(of_version_t version);
extern of_features_request_t *
    of_features_request_new_from_message(of_message_t msg);
extern void of_features_request_init(
    of_features_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_add_t *
    of_flow_add_new(of_version_t version);
extern of_flow_add_t *
    of_flow_add_new_from_message(of_message_t msg);
extern void of_flow_add_init(
    of_flow_add_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_delete_t *
    of_flow_delete_new(of_version_t version);
extern of_flow_delete_t *
    of_flow_delete_new_from_message(of_message_t msg);
extern void of_flow_delete_init(
    of_flow_delete_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_delete_strict_t *
    of_flow_delete_strict_new(of_version_t version);
extern of_flow_delete_strict_t *
    of_flow_delete_strict_new_from_message(of_message_t msg);
extern void of_flow_delete_strict_init(
    of_flow_delete_strict_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_mod_t *
    of_flow_mod_new(of_version_t version);
extern of_flow_mod_t *
    of_flow_mod_new_from_message(of_message_t msg);
extern void of_flow_mod_init(
    of_flow_mod_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_mod_failed_error_msg_t *
    of_flow_mod_failed_error_msg_new(of_version_t version);
extern of_flow_mod_failed_error_msg_t *
    of_flow_mod_failed_error_msg_new_from_message(of_message_t msg);
extern void of_flow_mod_failed_error_msg_init(
    of_flow_mod_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_modify_t *
    of_flow_modify_new(of_version_t version);
extern of_flow_modify_t *
    of_flow_modify_new_from_message(of_message_t msg);
extern void of_flow_modify_init(
    of_flow_modify_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_modify_strict_t *
    of_flow_modify_strict_new(of_version_t version);
extern of_flow_modify_strict_t *
    of_flow_modify_strict_new_from_message(of_message_t msg);
extern void of_flow_modify_strict_init(
    of_flow_modify_strict_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_removed_t *
    of_flow_removed_new(of_version_t version);
extern of_flow_removed_t *
    of_flow_removed_new_from_message(of_message_t msg);
extern void of_flow_removed_init(
    of_flow_removed_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_stats_reply_t *
    of_flow_stats_reply_new(of_version_t version);
extern of_flow_stats_reply_t *
    of_flow_stats_reply_new_from_message(of_message_t msg);
extern void of_flow_stats_reply_init(
    of_flow_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_stats_request_t *
    of_flow_stats_request_new(of_version_t version);
extern of_flow_stats_request_t *
    of_flow_stats_request_new_from_message(of_message_t msg);
extern void of_flow_stats_request_init(
    of_flow_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_get_config_reply_t *
    of_get_config_reply_new(of_version_t version);
extern of_get_config_reply_t *
    of_get_config_reply_new_from_message(of_message_t msg);
extern void of_get_config_reply_init(
    of_get_config_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_get_config_request_t *
    of_get_config_request_new(of_version_t version);
extern of_get_config_request_t *
    of_get_config_request_new_from_message(of_message_t msg);
extern void of_get_config_request_init(
    of_get_config_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_add_t *
    of_group_add_new(of_version_t version);
extern of_group_add_t *
    of_group_add_new_from_message(of_message_t msg);
extern void of_group_add_init(
    of_group_add_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_delete_t *
    of_group_delete_new(of_version_t version);
extern of_group_delete_t *
    of_group_delete_new_from_message(of_message_t msg);
extern void of_group_delete_init(
    of_group_delete_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_desc_stats_reply_t *
    of_group_desc_stats_reply_new(of_version_t version);
extern of_group_desc_stats_reply_t *
    of_group_desc_stats_reply_new_from_message(of_message_t msg);
extern void of_group_desc_stats_reply_init(
    of_group_desc_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_desc_stats_request_t *
    of_group_desc_stats_request_new(of_version_t version);
extern of_group_desc_stats_request_t *
    of_group_desc_stats_request_new_from_message(of_message_t msg);
extern void of_group_desc_stats_request_init(
    of_group_desc_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_features_stats_reply_t *
    of_group_features_stats_reply_new(of_version_t version);
extern of_group_features_stats_reply_t *
    of_group_features_stats_reply_new_from_message(of_message_t msg);
extern void of_group_features_stats_reply_init(
    of_group_features_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_features_stats_request_t *
    of_group_features_stats_request_new(of_version_t version);
extern of_group_features_stats_request_t *
    of_group_features_stats_request_new_from_message(of_message_t msg);
extern void of_group_features_stats_request_init(
    of_group_features_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_mod_t *
    of_group_mod_new(of_version_t version);
extern of_group_mod_t *
    of_group_mod_new_from_message(of_message_t msg);
extern void of_group_mod_init(
    of_group_mod_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_mod_failed_error_msg_t *
    of_group_mod_failed_error_msg_new(of_version_t version);
extern of_group_mod_failed_error_msg_t *
    of_group_mod_failed_error_msg_new_from_message(of_message_t msg);
extern void of_group_mod_failed_error_msg_init(
    of_group_mod_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_modify_t *
    of_group_modify_new(of_version_t version);
extern of_group_modify_t *
    of_group_modify_new_from_message(of_message_t msg);
extern void of_group_modify_init(
    of_group_modify_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_stats_reply_t *
    of_group_stats_reply_new(of_version_t version);
extern of_group_stats_reply_t *
    of_group_stats_reply_new_from_message(of_message_t msg);
extern void of_group_stats_reply_init(
    of_group_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_stats_request_t *
    of_group_stats_request_new(of_version_t version);
extern of_group_stats_request_t *
    of_group_stats_request_new_from_message(of_message_t msg);
extern void of_group_stats_request_init(
    of_group_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_hello_t *
    of_hello_new(of_version_t version);
extern of_hello_t *
    of_hello_new_from_message(of_message_t msg);
extern void of_hello_init(
    of_hello_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_hello_failed_error_msg_t *
    of_hello_failed_error_msg_new(of_version_t version);
extern of_hello_failed_error_msg_t *
    of_hello_failed_error_msg_new_from_message(of_message_t msg);
extern void of_hello_failed_error_msg_init(
    of_hello_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_add_t *
    of_meter_add_new(of_version_t version);
extern of_meter_add_t *
    of_meter_add_new_from_message(of_message_t msg);
extern void of_meter_add_init(
    of_meter_add_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_config_stats_reply_t *
    of_meter_config_stats_reply_new(of_version_t version);
extern of_meter_config_stats_reply_t *
    of_meter_config_stats_reply_new_from_message(of_message_t msg);
extern void of_meter_config_stats_reply_init(
    of_meter_config_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_config_stats_request_t *
    of_meter_config_stats_request_new(of_version_t version);
extern of_meter_config_stats_request_t *
    of_meter_config_stats_request_new_from_message(of_message_t msg);
extern void of_meter_config_stats_request_init(
    of_meter_config_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_delete_t *
    of_meter_delete_new(of_version_t version);
extern of_meter_delete_t *
    of_meter_delete_new_from_message(of_message_t msg);
extern void of_meter_delete_init(
    of_meter_delete_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_features_stats_reply_t *
    of_meter_features_stats_reply_new(of_version_t version);
extern of_meter_features_stats_reply_t *
    of_meter_features_stats_reply_new_from_message(of_message_t msg);
extern void of_meter_features_stats_reply_init(
    of_meter_features_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_features_stats_request_t *
    of_meter_features_stats_request_new(of_version_t version);
extern of_meter_features_stats_request_t *
    of_meter_features_stats_request_new_from_message(of_message_t msg);
extern void of_meter_features_stats_request_init(
    of_meter_features_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_mod_t *
    of_meter_mod_new(of_version_t version);
extern of_meter_mod_t *
    of_meter_mod_new_from_message(of_message_t msg);
extern void of_meter_mod_init(
    of_meter_mod_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_mod_failed_error_msg_t *
    of_meter_mod_failed_error_msg_new(of_version_t version);
extern of_meter_mod_failed_error_msg_t *
    of_meter_mod_failed_error_msg_new_from_message(of_message_t msg);
extern void of_meter_mod_failed_error_msg_init(
    of_meter_mod_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_modify_t *
    of_meter_modify_new(of_version_t version);
extern of_meter_modify_t *
    of_meter_modify_new_from_message(of_message_t msg);
extern void of_meter_modify_init(
    of_meter_modify_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_stats_reply_t *
    of_meter_stats_reply_new(of_version_t version);
extern of_meter_stats_reply_t *
    of_meter_stats_reply_new_from_message(of_message_t msg);
extern void of_meter_stats_reply_init(
    of_meter_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_stats_request_t *
    of_meter_stats_request_new(of_version_t version);
extern of_meter_stats_request_t *
    of_meter_stats_request_new_from_message(of_message_t msg);
extern void of_meter_stats_request_init(
    of_meter_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_nicira_controller_role_reply_t *
    of_nicira_controller_role_reply_new(of_version_t version);
extern of_nicira_controller_role_reply_t *
    of_nicira_controller_role_reply_new_from_message(of_message_t msg);
extern void of_nicira_controller_role_reply_init(
    of_nicira_controller_role_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_nicira_controller_role_request_t *
    of_nicira_controller_role_request_new(of_version_t version);
extern of_nicira_controller_role_request_t *
    of_nicira_controller_role_request_new_from_message(of_message_t msg);
extern void of_nicira_controller_role_request_init(
    of_nicira_controller_role_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_nicira_header_t *
    of_nicira_header_new(of_version_t version);
extern of_nicira_header_t *
    of_nicira_header_new_from_message(of_message_t msg);
extern void of_nicira_header_init(
    of_nicira_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_class_based_ctr_mod_msg_t *
    of_ofdpa_class_based_ctr_mod_msg_new(of_version_t version);
extern of_ofdpa_class_based_ctr_mod_msg_t *
    of_ofdpa_class_based_ctr_mod_msg_new_from_message(of_message_t msg);
extern void of_ofdpa_class_based_ctr_mod_msg_init(
    of_ofdpa_class_based_ctr_mod_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_class_based_ctr_multipart_reply_t *
    of_ofdpa_class_based_ctr_multipart_reply_new(of_version_t version);
extern of_ofdpa_class_based_ctr_multipart_reply_t *
    of_ofdpa_class_based_ctr_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_class_based_ctr_multipart_reply_init(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_class_based_ctr_multipart_request_t *
    of_ofdpa_class_based_ctr_multipart_request_new(of_version_t version);
extern of_ofdpa_class_based_ctr_multipart_request_t *
    of_ofdpa_class_based_ctr_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_class_based_ctr_multipart_request_init(
    of_ofdpa_class_based_ctr_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_color_based_ctr_mod_msg_t *
    of_ofdpa_color_based_ctr_mod_msg_new(of_version_t version);
extern of_ofdpa_color_based_ctr_mod_msg_t *
    of_ofdpa_color_based_ctr_mod_msg_new_from_message(of_message_t msg);
extern void of_ofdpa_color_based_ctr_mod_msg_init(
    of_ofdpa_color_based_ctr_mod_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_color_based_ctr_multipart_reply_t *
    of_ofdpa_color_based_ctr_multipart_reply_new(of_version_t version);
extern of_ofdpa_color_based_ctr_multipart_reply_t *
    of_ofdpa_color_based_ctr_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_color_based_ctr_multipart_reply_init(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_color_based_ctr_multipart_request_t *
    of_ofdpa_color_based_ctr_multipart_request_new(of_version_t version);
extern of_ofdpa_color_based_ctr_multipart_request_t *
    of_ofdpa_color_based_ctr_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_color_based_ctr_multipart_request_init(
    of_ofdpa_color_based_ctr_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_l2_interface_remark_action_mod_msg_t *
    of_ofdpa_l2_interface_remark_action_mod_msg_new(of_version_t version);
extern of_ofdpa_l2_interface_remark_action_mod_msg_t *
    of_ofdpa_l2_interface_remark_action_mod_msg_new_from_message(of_message_t msg);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_init(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_l2_interface_remark_action_multipart_reply_t *
    of_ofdpa_l2_interface_remark_action_multipart_reply_new(of_version_t version);
extern of_ofdpa_l2_interface_remark_action_multipart_reply_t *
    of_ofdpa_l2_interface_remark_action_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_init(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_l2_interface_remark_action_multipart_request_t *
    of_ofdpa_l2_interface_remark_action_multipart_request_new(of_version_t version);
extern of_ofdpa_l2_interface_remark_action_multipart_request_t *
    of_ofdpa_l2_interface_remark_action_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_init(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_mpls_tunnel_label_multipart_reply_t *
    of_ofdpa_mpls_tunnel_label_multipart_reply_new(of_version_t version);
extern of_ofdpa_mpls_tunnel_label_multipart_reply_t *
    of_ofdpa_mpls_tunnel_label_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_init(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_mpls_tunnel_label_multipart_request_t *
    of_ofdpa_mpls_tunnel_label_multipart_request_new(of_version_t version);
extern of_ofdpa_mpls_tunnel_label_multipart_request_t *
    of_ofdpa_mpls_tunnel_label_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_init(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_new(of_version_t version);
extern of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_new_from_message(of_message_t msg);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_init(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_new(of_version_t version);
extern of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_new_from_message(of_message_t msg);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_init(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_new(of_version_t version);
extern of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_init(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_new(of_version_t version);
extern of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_init(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_oam_dataplane_ctr_mod_msg_t *
    of_ofdpa_oam_dataplane_ctr_mod_msg_new(of_version_t version);
extern of_ofdpa_oam_dataplane_ctr_mod_msg_t *
    of_ofdpa_oam_dataplane_ctr_mod_msg_new_from_message(of_message_t msg);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_init(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_oam_dataplane_ctr_multipart_reply_t *
    of_ofdpa_oam_dataplane_ctr_multipart_reply_new(of_version_t version);
extern of_ofdpa_oam_dataplane_ctr_multipart_reply_t *
    of_ofdpa_oam_dataplane_ctr_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_init(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_oam_dataplane_ctr_multipart_request_t *
    of_ofdpa_oam_dataplane_ctr_multipart_request_new(of_version_t version);
extern of_ofdpa_oam_dataplane_ctr_multipart_request_t *
    of_ofdpa_oam_dataplane_ctr_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_init(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_oam_drop_status_mod_msg_t *
    of_ofdpa_oam_drop_status_mod_msg_new(of_version_t version);
extern of_ofdpa_oam_drop_status_mod_msg_t *
    of_ofdpa_oam_drop_status_mod_msg_new_from_message(of_message_t msg);
extern void of_ofdpa_oam_drop_status_mod_msg_init(
    of_ofdpa_oam_drop_status_mod_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_oam_drop_status_multipart_reply_t *
    of_ofdpa_oam_drop_status_multipart_reply_new(of_version_t version);
extern of_ofdpa_oam_drop_status_multipart_reply_t *
    of_ofdpa_oam_drop_status_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_oam_drop_status_multipart_reply_init(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_oam_drop_status_multipart_request_t *
    of_ofdpa_oam_drop_status_multipart_request_new(of_version_t version);
extern of_ofdpa_oam_drop_status_multipart_request_t *
    of_ofdpa_oam_drop_status_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_oam_drop_status_multipart_request_init(
    of_ofdpa_oam_drop_status_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_port_experimenter_prop_scheduling_t *
    of_ofdpa_port_experimenter_prop_scheduling_new(of_version_t version);
extern of_ofdpa_port_experimenter_prop_scheduling_t *
    of_ofdpa_port_experimenter_prop_scheduling_new_from_message(of_message_t msg);
extern void of_ofdpa_port_experimenter_prop_scheduling_init(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_new(of_version_t version);
extern of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_init(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_queue_desc_prop_congestion_multipart_request_t *
    of_ofdpa_queue_desc_prop_congestion_multipart_request_new(of_version_t version);
extern of_ofdpa_queue_desc_prop_congestion_multipart_request_t *
    of_ofdpa_queue_desc_prop_congestion_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_init(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_queue_desc_prop_parent_multipart_reply_t *
    of_ofdpa_queue_desc_prop_parent_multipart_reply_new(of_version_t version);
extern of_ofdpa_queue_desc_prop_parent_multipart_reply_t *
    of_ofdpa_queue_desc_prop_parent_multipart_reply_new_from_message(of_message_t msg);
extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_init(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_queue_desc_prop_parent_multipart_request_t *
    of_ofdpa_queue_desc_prop_parent_multipart_request_new(of_version_t version);
extern of_ofdpa_queue_desc_prop_parent_multipart_request_t *
    of_ofdpa_queue_desc_prop_parent_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_queue_desc_prop_parent_multipart_request_init(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_queue_mod_t *
    of_ofdpa_queue_mod_new(of_version_t version);
extern of_ofdpa_queue_mod_t *
    of_ofdpa_queue_mod_new_from_message(of_message_t msg);
extern void of_ofdpa_queue_mod_init(
    of_ofdpa_queue_mod_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_scheduler_node_mod_msg_t *
    of_ofdpa_scheduler_node_mod_msg_new(of_version_t version);
extern of_ofdpa_scheduler_node_mod_msg_t *
    of_ofdpa_scheduler_node_mod_msg_new_from_message(of_message_t msg);
extern void of_ofdpa_scheduler_node_mod_msg_init(
    of_ofdpa_scheduler_node_mod_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_scheduler_node_multipart_request_t *
    of_ofdpa_scheduler_node_multipart_request_new(of_version_t version);
extern of_ofdpa_scheduler_node_multipart_request_t *
    of_ofdpa_scheduler_node_multipart_request_new_from_message(of_message_t msg);
extern void of_ofdpa_scheduler_node_multipart_request_init(
    of_ofdpa_scheduler_node_multipart_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_ofdpa_scheduler_node_multipart_response_t *
    of_ofdpa_scheduler_node_multipart_response_new(of_version_t version);
extern of_ofdpa_scheduler_node_multipart_response_t *
    of_ofdpa_scheduler_node_multipart_response_new_from_message(of_message_t msg);
extern void of_ofdpa_scheduler_node_multipart_response_init(
    of_ofdpa_scheduler_node_multipart_response_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_packet_in_t *
    of_packet_in_new(of_version_t version);
extern of_packet_in_t *
    of_packet_in_new_from_message(of_message_t msg);
extern void of_packet_in_init(
    of_packet_in_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_packet_out_t *
    of_packet_out_new(of_version_t version);
extern of_packet_out_t *
    of_packet_out_new_from_message(of_message_t msg);
extern void of_packet_out_init(
    of_packet_out_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_desc_stats_reply_t *
    of_port_desc_stats_reply_new(of_version_t version);
extern of_port_desc_stats_reply_t *
    of_port_desc_stats_reply_new_from_message(of_message_t msg);
extern void of_port_desc_stats_reply_init(
    of_port_desc_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_desc_stats_request_t *
    of_port_desc_stats_request_new(of_version_t version);
extern of_port_desc_stats_request_t *
    of_port_desc_stats_request_new_from_message(of_message_t msg);
extern void of_port_desc_stats_request_init(
    of_port_desc_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_mod_t *
    of_port_mod_new(of_version_t version);
extern of_port_mod_t *
    of_port_mod_new_from_message(of_message_t msg);
extern void of_port_mod_init(
    of_port_mod_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_mod_failed_error_msg_t *
    of_port_mod_failed_error_msg_new(of_version_t version);
extern of_port_mod_failed_error_msg_t *
    of_port_mod_failed_error_msg_new_from_message(of_message_t msg);
extern void of_port_mod_failed_error_msg_init(
    of_port_mod_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_stats_reply_t *
    of_port_stats_reply_new(of_version_t version);
extern of_port_stats_reply_t *
    of_port_stats_reply_new_from_message(of_message_t msg);
extern void of_port_stats_reply_init(
    of_port_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_stats_request_t *
    of_port_stats_request_new(of_version_t version);
extern of_port_stats_request_t *
    of_port_stats_request_new_from_message(of_message_t msg);
extern void of_port_stats_request_init(
    of_port_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_status_t *
    of_port_status_new(of_version_t version);
extern of_port_status_t *
    of_port_status_new_from_message(of_message_t msg);
extern void of_port_status_init(
    of_port_status_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_get_config_reply_t *
    of_queue_get_config_reply_new(of_version_t version);
extern of_queue_get_config_reply_t *
    of_queue_get_config_reply_new_from_message(of_message_t msg);
extern void of_queue_get_config_reply_init(
    of_queue_get_config_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_get_config_request_t *
    of_queue_get_config_request_new(of_version_t version);
extern of_queue_get_config_request_t *
    of_queue_get_config_request_new_from_message(of_message_t msg);
extern void of_queue_get_config_request_init(
    of_queue_get_config_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_op_failed_error_msg_t *
    of_queue_op_failed_error_msg_new(of_version_t version);
extern of_queue_op_failed_error_msg_t *
    of_queue_op_failed_error_msg_new_from_message(of_message_t msg);
extern void of_queue_op_failed_error_msg_init(
    of_queue_op_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_stats_reply_t *
    of_queue_stats_reply_new(of_version_t version);
extern of_queue_stats_reply_t *
    of_queue_stats_reply_new_from_message(of_message_t msg);
extern void of_queue_stats_reply_init(
    of_queue_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_stats_request_t *
    of_queue_stats_request_new(of_version_t version);
extern of_queue_stats_request_t *
    of_queue_stats_request_new_from_message(of_message_t msg);
extern void of_queue_stats_request_init(
    of_queue_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_role_reply_t *
    of_role_reply_new(of_version_t version);
extern of_role_reply_t *
    of_role_reply_new_from_message(of_message_t msg);
extern void of_role_reply_init(
    of_role_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_role_request_t *
    of_role_request_new(of_version_t version);
extern of_role_request_t *
    of_role_request_new_from_message(of_message_t msg);
extern void of_role_request_init(
    of_role_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_role_request_failed_error_msg_t *
    of_role_request_failed_error_msg_new(of_version_t version);
extern of_role_request_failed_error_msg_t *
    of_role_request_failed_error_msg_new_from_message(of_message_t msg);
extern void of_role_request_failed_error_msg_init(
    of_role_request_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_set_config_t *
    of_set_config_new(of_version_t version);
extern of_set_config_t *
    of_set_config_new_from_message(of_message_t msg);
extern void of_set_config_init(
    of_set_config_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_stats_reply_t *
    of_stats_reply_new(of_version_t version);
extern of_stats_reply_t *
    of_stats_reply_new_from_message(of_message_t msg);
extern void of_stats_reply_init(
    of_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_stats_request_t *
    of_stats_request_new(of_version_t version);
extern of_stats_request_t *
    of_stats_request_new_from_message(of_message_t msg);
extern void of_stats_request_init(
    of_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_switch_config_failed_error_msg_t *
    of_switch_config_failed_error_msg_new(of_version_t version);
extern of_switch_config_failed_error_msg_t *
    of_switch_config_failed_error_msg_new_from_message(of_message_t msg);
extern void of_switch_config_failed_error_msg_init(
    of_switch_config_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_features_failed_error_msg_t *
    of_table_features_failed_error_msg_new(of_version_t version);
extern of_table_features_failed_error_msg_t *
    of_table_features_failed_error_msg_new_from_message(of_message_t msg);
extern void of_table_features_failed_error_msg_init(
    of_table_features_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_features_stats_reply_t *
    of_table_features_stats_reply_new(of_version_t version);
extern of_table_features_stats_reply_t *
    of_table_features_stats_reply_new_from_message(of_message_t msg);
extern void of_table_features_stats_reply_init(
    of_table_features_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_features_stats_request_t *
    of_table_features_stats_request_new(of_version_t version);
extern of_table_features_stats_request_t *
    of_table_features_stats_request_new_from_message(of_message_t msg);
extern void of_table_features_stats_request_init(
    of_table_features_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_mod_t *
    of_table_mod_new(of_version_t version);
extern of_table_mod_t *
    of_table_mod_new_from_message(of_message_t msg);
extern void of_table_mod_init(
    of_table_mod_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_mod_failed_error_msg_t *
    of_table_mod_failed_error_msg_new(of_version_t version);
extern of_table_mod_failed_error_msg_t *
    of_table_mod_failed_error_msg_new_from_message(of_message_t msg);
extern void of_table_mod_failed_error_msg_init(
    of_table_mod_failed_error_msg_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_stats_reply_t *
    of_table_stats_reply_new(of_version_t version);
extern of_table_stats_reply_t *
    of_table_stats_reply_new_from_message(of_message_t msg);
extern void of_table_stats_reply_init(
    of_table_stats_reply_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_stats_request_t *
    of_table_stats_request_new(of_version_t version);
extern of_table_stats_request_t *
    of_table_stats_request_new_from_message(of_message_t msg);
extern void of_table_stats_request_init(
    of_table_stats_request_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_t *
    of_action_new(of_version_t version);
extern void of_action_init(
    of_action_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_bsn_t *
    of_action_bsn_new(of_version_t version);
extern void of_action_bsn_init(
    of_action_bsn_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_bsn_mirror_t *
    of_action_bsn_mirror_new(of_version_t version);
extern void of_action_bsn_mirror_init(
    of_action_bsn_mirror_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_bsn_set_tunnel_dst_t *
    of_action_bsn_set_tunnel_dst_new(of_version_t version);
extern void of_action_bsn_set_tunnel_dst_init(
    of_action_bsn_set_tunnel_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_copy_ttl_in_t *
    of_action_copy_ttl_in_new(of_version_t version);
extern void of_action_copy_ttl_in_init(
    of_action_copy_ttl_in_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_copy_ttl_out_t *
    of_action_copy_ttl_out_new(of_version_t version);
extern void of_action_copy_ttl_out_init(
    of_action_copy_ttl_out_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_dec_mpls_ttl_t *
    of_action_dec_mpls_ttl_new(of_version_t version);
extern void of_action_dec_mpls_ttl_init(
    of_action_dec_mpls_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_dec_nw_ttl_t *
    of_action_dec_nw_ttl_new(of_version_t version);
extern void of_action_dec_nw_ttl_init(
    of_action_dec_nw_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_enqueue_t *
    of_action_enqueue_new(of_version_t version);
extern void of_action_enqueue_init(
    of_action_enqueue_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_experimenter_t *
    of_action_experimenter_new(of_version_t version);
extern void of_action_experimenter_init(
    of_action_experimenter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_group_t *
    of_action_group_new(of_version_t version);
extern void of_action_group_init(
    of_action_group_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_header_t *
    of_action_header_new(of_version_t version);
extern void of_action_header_init(
    of_action_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_t *
    of_action_id_new(of_version_t version);
extern void of_action_id_init(
    of_action_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_bsn_t *
    of_action_id_bsn_new(of_version_t version);
extern void of_action_id_bsn_init(
    of_action_id_bsn_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_bsn_mirror_t *
    of_action_id_bsn_mirror_new(of_version_t version);
extern void of_action_id_bsn_mirror_init(
    of_action_id_bsn_mirror_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_bsn_set_tunnel_dst_t *
    of_action_id_bsn_set_tunnel_dst_new(of_version_t version);
extern void of_action_id_bsn_set_tunnel_dst_init(
    of_action_id_bsn_set_tunnel_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_copy_ttl_in_t *
    of_action_id_copy_ttl_in_new(of_version_t version);
extern void of_action_id_copy_ttl_in_init(
    of_action_id_copy_ttl_in_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_copy_ttl_out_t *
    of_action_id_copy_ttl_out_new(of_version_t version);
extern void of_action_id_copy_ttl_out_init(
    of_action_id_copy_ttl_out_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_dec_mpls_ttl_t *
    of_action_id_dec_mpls_ttl_new(of_version_t version);
extern void of_action_id_dec_mpls_ttl_init(
    of_action_id_dec_mpls_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_dec_nw_ttl_t *
    of_action_id_dec_nw_ttl_new(of_version_t version);
extern void of_action_id_dec_nw_ttl_init(
    of_action_id_dec_nw_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_experimenter_t *
    of_action_id_experimenter_new(of_version_t version);
extern void of_action_id_experimenter_init(
    of_action_id_experimenter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_group_t *
    of_action_id_group_new(of_version_t version);
extern void of_action_id_group_init(
    of_action_id_group_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_header_t *
    of_action_id_header_new(of_version_t version);
extern void of_action_id_header_init(
    of_action_id_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_nicira_t *
    of_action_id_nicira_new(of_version_t version);
extern void of_action_id_nicira_init(
    of_action_id_nicira_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_nicira_dec_ttl_t *
    of_action_id_nicira_dec_ttl_new(of_version_t version);
extern void of_action_id_nicira_dec_ttl_init(
    of_action_id_nicira_dec_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_t *
    of_action_id_ofdpa_new(of_version_t version);
extern void of_action_id_ofdpa_init(
    of_action_id_ofdpa_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_check_drop_status_t *
    of_action_id_ofdpa_check_drop_status_new(of_version_t version);
extern void of_action_id_ofdpa_check_drop_status_init(
    of_action_id_ofdpa_check_drop_status_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_class_based_count_t *
    of_action_id_ofdpa_class_based_count_new(of_version_t version);
extern void of_action_id_ofdpa_class_based_count_init(
    of_action_id_ofdpa_class_based_count_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_color_based_count_t *
    of_action_id_ofdpa_color_based_count_new(of_version_t version);
extern void of_action_id_ofdpa_color_based_count_init(
    of_action_id_ofdpa_color_based_count_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_color_based_count_1_t *
    of_action_id_ofdpa_color_based_count_1_new(of_version_t version);
extern void of_action_id_ofdpa_color_based_count_1_init(
    of_action_id_ofdpa_color_based_count_1_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_color_based_count_2_t *
    of_action_id_ofdpa_color_based_count_2_new(of_version_t version);
extern void of_action_id_ofdpa_color_based_count_2_init(
    of_action_id_ofdpa_color_based_count_2_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_color_based_count_3_t *
    of_action_id_ofdpa_color_based_count_3_new(of_version_t version);
extern void of_action_id_ofdpa_color_based_count_3_init(
    of_action_id_ofdpa_color_based_count_3_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_copy_tc_in_t *
    of_action_id_ofdpa_copy_tc_in_new(of_version_t version);
extern void of_action_id_ofdpa_copy_tc_in_init(
    of_action_id_ofdpa_copy_tc_in_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_copy_tc_out_t *
    of_action_id_ofdpa_copy_tc_out_new(of_version_t version);
extern void of_action_id_ofdpa_copy_tc_out_init(
    of_action_id_ofdpa_copy_tc_out_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_dec_ttl_mtu_t *
    of_action_id_ofdpa_dec_ttl_mtu_new(of_version_t version);
extern void of_action_id_ofdpa_dec_ttl_mtu_init(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_oam_lm_rx_count_t *
    of_action_id_ofdpa_oam_lm_rx_count_new(of_version_t version);
extern void of_action_id_ofdpa_oam_lm_rx_count_init(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_oam_lm_tx_count_t *
    of_action_id_ofdpa_oam_lm_tx_count_new(of_version_t version);
extern void of_action_id_ofdpa_oam_lm_tx_count_init(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_pop_cw_t *
    of_action_id_ofdpa_pop_cw_new(of_version_t version);
extern void of_action_id_ofdpa_pop_cw_init(
    of_action_id_ofdpa_pop_cw_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_pop_l2hdr_t *
    of_action_id_ofdpa_pop_l2hdr_new(of_version_t version);
extern void of_action_id_ofdpa_pop_l2hdr_init(
    of_action_id_ofdpa_pop_l2hdr_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_push_cw_t *
    of_action_id_ofdpa_push_cw_new(of_version_t version);
extern void of_action_id_ofdpa_push_cw_init(
    of_action_id_ofdpa_push_cw_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_push_l2hdr_t *
    of_action_id_ofdpa_push_l2hdr_new(of_version_t version);
extern void of_action_id_ofdpa_push_l2hdr_init(
    of_action_id_ofdpa_push_l2hdr_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_set_counter_fields_t *
    of_action_id_ofdpa_set_counter_fields_new(of_version_t version);
extern void of_action_id_ofdpa_set_counter_fields_init(
    of_action_id_ofdpa_set_counter_fields_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_new(of_version_t version);
extern void of_action_id_ofdpa_set_mpls_pcpdei_from_table_init(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_new(of_version_t version);
extern void of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_init(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_new(of_version_t version);
extern void of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_init(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_new(of_version_t version);
extern void of_action_id_ofdpa_set_mpls_tc_from_vpn_table_init(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_ofdpa_set_pcpdei_from_table_t *
    of_action_id_ofdpa_set_pcpdei_from_table_new(of_version_t version);
extern void of_action_id_ofdpa_set_pcpdei_from_table_init(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_onf_t *
    of_action_id_onf_new(of_version_t version);
extern void of_action_id_onf_init(
    of_action_id_onf_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_onf_copy_field_t *
    of_action_id_onf_copy_field_new(of_version_t version);
extern void of_action_id_onf_copy_field_init(
    of_action_id_onf_copy_field_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_output_t *
    of_action_id_output_new(of_version_t version);
extern void of_action_id_output_init(
    of_action_id_output_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_pop_mpls_t *
    of_action_id_pop_mpls_new(of_version_t version);
extern void of_action_id_pop_mpls_init(
    of_action_id_pop_mpls_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_pop_pbb_t *
    of_action_id_pop_pbb_new(of_version_t version);
extern void of_action_id_pop_pbb_init(
    of_action_id_pop_pbb_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_pop_vlan_t *
    of_action_id_pop_vlan_new(of_version_t version);
extern void of_action_id_pop_vlan_init(
    of_action_id_pop_vlan_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_push_mpls_t *
    of_action_id_push_mpls_new(of_version_t version);
extern void of_action_id_push_mpls_init(
    of_action_id_push_mpls_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_push_pbb_t *
    of_action_id_push_pbb_new(of_version_t version);
extern void of_action_id_push_pbb_init(
    of_action_id_push_pbb_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_push_vlan_t *
    of_action_id_push_vlan_new(of_version_t version);
extern void of_action_id_push_vlan_init(
    of_action_id_push_vlan_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_set_field_t *
    of_action_id_set_field_new(of_version_t version);
extern void of_action_id_set_field_init(
    of_action_id_set_field_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_set_mpls_ttl_t *
    of_action_id_set_mpls_ttl_new(of_version_t version);
extern void of_action_id_set_mpls_ttl_init(
    of_action_id_set_mpls_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_set_nw_ttl_t *
    of_action_id_set_nw_ttl_new(of_version_t version);
extern void of_action_id_set_nw_ttl_init(
    of_action_id_set_nw_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_id_set_queue_t *
    of_action_id_set_queue_new(of_version_t version);
extern void of_action_id_set_queue_init(
    of_action_id_set_queue_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_nicira_t *
    of_action_nicira_new(of_version_t version);
extern void of_action_nicira_init(
    of_action_nicira_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_nicira_dec_ttl_t *
    of_action_nicira_dec_ttl_new(of_version_t version);
extern void of_action_nicira_dec_ttl_init(
    of_action_nicira_dec_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_t *
    of_action_ofdpa_new(of_version_t version);
extern void of_action_ofdpa_init(
    of_action_ofdpa_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_check_drop_status_t *
    of_action_ofdpa_check_drop_status_new(of_version_t version);
extern void of_action_ofdpa_check_drop_status_init(
    of_action_ofdpa_check_drop_status_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_class_based_count_t *
    of_action_ofdpa_class_based_count_new(of_version_t version);
extern void of_action_ofdpa_class_based_count_init(
    of_action_ofdpa_class_based_count_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_color_based_count_t *
    of_action_ofdpa_color_based_count_new(of_version_t version);
extern void of_action_ofdpa_color_based_count_init(
    of_action_ofdpa_color_based_count_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_color_based_count_1_t *
    of_action_ofdpa_color_based_count_1_new(of_version_t version);
extern void of_action_ofdpa_color_based_count_1_init(
    of_action_ofdpa_color_based_count_1_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_color_based_count_2_t *
    of_action_ofdpa_color_based_count_2_new(of_version_t version);
extern void of_action_ofdpa_color_based_count_2_init(
    of_action_ofdpa_color_based_count_2_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_color_based_count_3_t *
    of_action_ofdpa_color_based_count_3_new(of_version_t version);
extern void of_action_ofdpa_color_based_count_3_init(
    of_action_ofdpa_color_based_count_3_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_copy_tc_in_t *
    of_action_ofdpa_copy_tc_in_new(of_version_t version);
extern void of_action_ofdpa_copy_tc_in_init(
    of_action_ofdpa_copy_tc_in_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_copy_tc_out_t *
    of_action_ofdpa_copy_tc_out_new(of_version_t version);
extern void of_action_ofdpa_copy_tc_out_init(
    of_action_ofdpa_copy_tc_out_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_dec_ttl_mtu_t *
    of_action_ofdpa_dec_ttl_mtu_new(of_version_t version);
extern void of_action_ofdpa_dec_ttl_mtu_init(
    of_action_ofdpa_dec_ttl_mtu_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_oam_lm_rx_count_t *
    of_action_ofdpa_oam_lm_rx_count_new(of_version_t version);
extern void of_action_ofdpa_oam_lm_rx_count_init(
    of_action_ofdpa_oam_lm_rx_count_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_oam_lm_tx_count_t *
    of_action_ofdpa_oam_lm_tx_count_new(of_version_t version);
extern void of_action_ofdpa_oam_lm_tx_count_init(
    of_action_ofdpa_oam_lm_tx_count_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_pop_cw_t *
    of_action_ofdpa_pop_cw_new(of_version_t version);
extern void of_action_ofdpa_pop_cw_init(
    of_action_ofdpa_pop_cw_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_pop_l2hdr_t *
    of_action_ofdpa_pop_l2hdr_new(of_version_t version);
extern void of_action_ofdpa_pop_l2hdr_init(
    of_action_ofdpa_pop_l2hdr_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_push_cw_t *
    of_action_ofdpa_push_cw_new(of_version_t version);
extern void of_action_ofdpa_push_cw_init(
    of_action_ofdpa_push_cw_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_push_l2hdr_t *
    of_action_ofdpa_push_l2hdr_new(of_version_t version);
extern void of_action_ofdpa_push_l2hdr_init(
    of_action_ofdpa_push_l2hdr_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_set_counter_fields_t *
    of_action_ofdpa_set_counter_fields_new(of_version_t version);
extern void of_action_ofdpa_set_counter_fields_init(
    of_action_ofdpa_set_counter_fields_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_set_mpls_pcpdei_from_table_t *
    of_action_ofdpa_set_mpls_pcpdei_from_table_new(of_version_t version);
extern void of_action_ofdpa_set_mpls_pcpdei_from_table_init(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_new(of_version_t version);
extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_init(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_new(of_version_t version);
extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_init(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_set_mpls_tc_from_vpn_table_t *
    of_action_ofdpa_set_mpls_tc_from_vpn_table_new(of_version_t version);
extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_init(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_ofdpa_set_pcpdei_from_table_t *
    of_action_ofdpa_set_pcpdei_from_table_new(of_version_t version);
extern void of_action_ofdpa_set_pcpdei_from_table_init(
    of_action_ofdpa_set_pcpdei_from_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_onf_t *
    of_action_onf_new(of_version_t version);
extern void of_action_onf_init(
    of_action_onf_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_onf_copy_field_t *
    of_action_onf_copy_field_new(of_version_t version);
extern void of_action_onf_copy_field_init(
    of_action_onf_copy_field_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_output_t *
    of_action_output_new(of_version_t version);
extern void of_action_output_init(
    of_action_output_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_pop_mpls_t *
    of_action_pop_mpls_new(of_version_t version);
extern void of_action_pop_mpls_init(
    of_action_pop_mpls_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_pop_pbb_t *
    of_action_pop_pbb_new(of_version_t version);
extern void of_action_pop_pbb_init(
    of_action_pop_pbb_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_pop_vlan_t *
    of_action_pop_vlan_new(of_version_t version);
extern void of_action_pop_vlan_init(
    of_action_pop_vlan_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_push_mpls_t *
    of_action_push_mpls_new(of_version_t version);
extern void of_action_push_mpls_init(
    of_action_push_mpls_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_push_pbb_t *
    of_action_push_pbb_new(of_version_t version);
extern void of_action_push_pbb_init(
    of_action_push_pbb_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_push_vlan_t *
    of_action_push_vlan_new(of_version_t version);
extern void of_action_push_vlan_init(
    of_action_push_vlan_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_dl_dst_t *
    of_action_set_dl_dst_new(of_version_t version);
extern void of_action_set_dl_dst_init(
    of_action_set_dl_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_dl_src_t *
    of_action_set_dl_src_new(of_version_t version);
extern void of_action_set_dl_src_init(
    of_action_set_dl_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_field_t *
    of_action_set_field_new(of_version_t version);
extern void of_action_set_field_init(
    of_action_set_field_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_mpls_label_t *
    of_action_set_mpls_label_new(of_version_t version);
extern void of_action_set_mpls_label_init(
    of_action_set_mpls_label_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_mpls_tc_t *
    of_action_set_mpls_tc_new(of_version_t version);
extern void of_action_set_mpls_tc_init(
    of_action_set_mpls_tc_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_mpls_ttl_t *
    of_action_set_mpls_ttl_new(of_version_t version);
extern void of_action_set_mpls_ttl_init(
    of_action_set_mpls_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_nw_dst_t *
    of_action_set_nw_dst_new(of_version_t version);
extern void of_action_set_nw_dst_init(
    of_action_set_nw_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_nw_ecn_t *
    of_action_set_nw_ecn_new(of_version_t version);
extern void of_action_set_nw_ecn_init(
    of_action_set_nw_ecn_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_nw_src_t *
    of_action_set_nw_src_new(of_version_t version);
extern void of_action_set_nw_src_init(
    of_action_set_nw_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_nw_tos_t *
    of_action_set_nw_tos_new(of_version_t version);
extern void of_action_set_nw_tos_init(
    of_action_set_nw_tos_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_nw_ttl_t *
    of_action_set_nw_ttl_new(of_version_t version);
extern void of_action_set_nw_ttl_init(
    of_action_set_nw_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_queue_t *
    of_action_set_queue_new(of_version_t version);
extern void of_action_set_queue_init(
    of_action_set_queue_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_tp_dst_t *
    of_action_set_tp_dst_new(of_version_t version);
extern void of_action_set_tp_dst_init(
    of_action_set_tp_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_tp_src_t *
    of_action_set_tp_src_new(of_version_t version);
extern void of_action_set_tp_src_init(
    of_action_set_tp_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_vlan_pcp_t *
    of_action_set_vlan_pcp_new(of_version_t version);
extern void of_action_set_vlan_pcp_init(
    of_action_set_vlan_pcp_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_set_vlan_vid_t *
    of_action_set_vlan_vid_new(of_version_t version);
extern void of_action_set_vlan_vid_init(
    of_action_set_vlan_vid_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_action_strip_vlan_t *
    of_action_strip_vlan_new(of_version_t version);
extern void of_action_strip_vlan_init(
    of_action_strip_vlan_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_controller_connection_t *
    of_bsn_controller_connection_new(of_version_t version);
extern void of_bsn_controller_connection_init(
    of_bsn_controller_connection_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_debug_counter_desc_stats_entry_t *
    of_bsn_debug_counter_desc_stats_entry_new(of_version_t version);
extern void of_bsn_debug_counter_desc_stats_entry_init(
    of_bsn_debug_counter_desc_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_debug_counter_stats_entry_t *
    of_bsn_debug_counter_stats_entry_new(of_version_t version);
extern void of_bsn_debug_counter_stats_entry_init(
    of_bsn_debug_counter_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_flow_checksum_bucket_stats_entry_t *
    of_bsn_flow_checksum_bucket_stats_entry_new(of_version_t version);
extern void of_bsn_flow_checksum_bucket_stats_entry_init(
    of_bsn_flow_checksum_bucket_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_bucket_stats_entry_t *
    of_bsn_gentable_bucket_stats_entry_new(of_version_t version);
extern void of_bsn_gentable_bucket_stats_entry_init(
    of_bsn_gentable_bucket_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_desc_stats_entry_t *
    of_bsn_gentable_desc_stats_entry_new(of_version_t version);
extern void of_bsn_gentable_desc_stats_entry_init(
    of_bsn_gentable_desc_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_entry_desc_stats_entry_t *
    of_bsn_gentable_entry_desc_stats_entry_new(of_version_t version);
extern void of_bsn_gentable_entry_desc_stats_entry_init(
    of_bsn_gentable_entry_desc_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_entry_stats_entry_t *
    of_bsn_gentable_entry_stats_entry_new(of_version_t version);
extern void of_bsn_gentable_entry_stats_entry_init(
    of_bsn_gentable_entry_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_gentable_stats_entry_t *
    of_bsn_gentable_stats_entry_new(of_version_t version);
extern void of_bsn_gentable_stats_entry_init(
    of_bsn_gentable_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_interface_t *
    of_bsn_interface_new(of_version_t version);
extern void of_bsn_interface_init(
    of_bsn_interface_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_lacp_stats_entry_t *
    of_bsn_lacp_stats_entry_new(of_version_t version);
extern void of_bsn_lacp_stats_entry_init(
    of_bsn_lacp_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_port_counter_stats_entry_t *
    of_bsn_port_counter_stats_entry_new(of_version_t version);
extern void of_bsn_port_counter_stats_entry_init(
    of_bsn_port_counter_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_switch_pipeline_stats_entry_t *
    of_bsn_switch_pipeline_stats_entry_new(of_version_t version);
extern void of_bsn_switch_pipeline_stats_entry_init(
    of_bsn_switch_pipeline_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_table_checksum_stats_entry_t *
    of_bsn_table_checksum_stats_entry_new(of_version_t version);
extern void of_bsn_table_checksum_stats_entry_init(
    of_bsn_table_checksum_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_t *
    of_bsn_tlv_new(of_version_t version);
extern void of_bsn_tlv_init(
    of_bsn_tlv_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_broadcast_query_timeout_t *
    of_bsn_tlv_broadcast_query_timeout_new(of_version_t version);
extern void of_bsn_tlv_broadcast_query_timeout_init(
    of_bsn_tlv_broadcast_query_timeout_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_circuit_id_t *
    of_bsn_tlv_circuit_id_new(of_version_t version);
extern void of_bsn_tlv_circuit_id_init(
    of_bsn_tlv_circuit_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_header_t *
    of_bsn_tlv_header_new(of_version_t version);
extern void of_bsn_tlv_header_init(
    of_bsn_tlv_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_idle_notification_t *
    of_bsn_tlv_idle_notification_new(of_version_t version);
extern void of_bsn_tlv_idle_notification_init(
    of_bsn_tlv_idle_notification_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_idle_time_t *
    of_bsn_tlv_idle_time_new(of_version_t version);
extern void of_bsn_tlv_idle_time_init(
    of_bsn_tlv_idle_time_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_idle_timeout_t *
    of_bsn_tlv_idle_timeout_new(of_version_t version);
extern void of_bsn_tlv_idle_timeout_init(
    of_bsn_tlv_idle_timeout_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_ipv4_t *
    of_bsn_tlv_ipv4_new(of_version_t version);
extern void of_bsn_tlv_ipv4_init(
    of_bsn_tlv_ipv4_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_mac_t *
    of_bsn_tlv_mac_new(of_version_t version);
extern void of_bsn_tlv_mac_init(
    of_bsn_tlv_mac_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_miss_packets_t *
    of_bsn_tlv_miss_packets_new(of_version_t version);
extern void of_bsn_tlv_miss_packets_init(
    of_bsn_tlv_miss_packets_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_port_t *
    of_bsn_tlv_port_new(of_version_t version);
extern void of_bsn_tlv_port_init(
    of_bsn_tlv_port_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_reply_packets_t *
    of_bsn_tlv_reply_packets_new(of_version_t version);
extern void of_bsn_tlv_reply_packets_init(
    of_bsn_tlv_reply_packets_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_request_packets_t *
    of_bsn_tlv_request_packets_new(of_version_t version);
extern void of_bsn_tlv_request_packets_init(
    of_bsn_tlv_request_packets_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_rx_packets_t *
    of_bsn_tlv_rx_packets_new(of_version_t version);
extern void of_bsn_tlv_rx_packets_init(
    of_bsn_tlv_rx_packets_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_tx_packets_t *
    of_bsn_tlv_tx_packets_new(of_version_t version);
extern void of_bsn_tlv_tx_packets_init(
    of_bsn_tlv_tx_packets_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_udf_anchor_t *
    of_bsn_tlv_udf_anchor_new(of_version_t version);
extern void of_bsn_tlv_udf_anchor_init(
    of_bsn_tlv_udf_anchor_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_udf_id_t *
    of_bsn_tlv_udf_id_new(of_version_t version);
extern void of_bsn_tlv_udf_id_init(
    of_bsn_tlv_udf_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_udf_length_t *
    of_bsn_tlv_udf_length_new(of_version_t version);
extern void of_bsn_tlv_udf_length_init(
    of_bsn_tlv_udf_length_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_udf_offset_t *
    of_bsn_tlv_udf_offset_new(of_version_t version);
extern void of_bsn_tlv_udf_offset_init(
    of_bsn_tlv_udf_offset_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_unicast_query_timeout_t *
    of_bsn_tlv_unicast_query_timeout_new(of_version_t version);
extern void of_bsn_tlv_unicast_query_timeout_init(
    of_bsn_tlv_unicast_query_timeout_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_tlv_vlan_vid_t *
    of_bsn_tlv_vlan_vid_new(of_version_t version);
extern void of_bsn_tlv_vlan_vid_init(
    of_bsn_tlv_vlan_vid_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_vlan_counter_stats_entry_t *
    of_bsn_vlan_counter_stats_entry_new(of_version_t version);
extern void of_bsn_vlan_counter_stats_entry_init(
    of_bsn_vlan_counter_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_vport_t *
    of_bsn_vport_new(of_version_t version);
extern void of_bsn_vport_init(
    of_bsn_vport_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_vport_header_t *
    of_bsn_vport_header_new(of_version_t version);
extern void of_bsn_vport_header_init(
    of_bsn_vport_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bsn_vport_q_in_q_t *
    of_bsn_vport_q_in_q_new(of_version_t version);
extern void of_bsn_vport_q_in_q_init(
    of_bsn_vport_q_in_q_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bucket_t *
    of_bucket_new(of_version_t version);
extern void of_bucket_init(
    of_bucket_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_bucket_counter_t *
    of_bucket_counter_new(of_version_t version);
extern void of_bucket_counter_init(
    of_bucket_counter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_flow_stats_entry_t *
    of_flow_stats_entry_new(of_version_t version);
extern void of_flow_stats_entry_init(
    of_flow_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_desc_stats_entry_t *
    of_group_desc_stats_entry_new(of_version_t version);
extern void of_group_desc_stats_entry_init(
    of_group_desc_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_group_stats_entry_t *
    of_group_stats_entry_new(of_version_t version);
extern void of_group_stats_entry_init(
    of_group_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_header_t *
    of_header_new(of_version_t version);
extern void of_header_init(
    of_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_hello_elem_t *
    of_hello_elem_new(of_version_t version);
extern void of_hello_elem_init(
    of_hello_elem_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_hello_elem_header_t *
    of_hello_elem_header_new(of_version_t version);
extern void of_hello_elem_header_init(
    of_hello_elem_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_hello_elem_versionbitmap_t *
    of_hello_elem_versionbitmap_new(of_version_t version);
extern void of_hello_elem_versionbitmap_init(
    of_hello_elem_versionbitmap_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_t *
    of_instruction_new(of_version_t version);
extern void of_instruction_init(
    of_instruction_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_apply_actions_t *
    of_instruction_apply_actions_new(of_version_t version);
extern void of_instruction_apply_actions_init(
    of_instruction_apply_actions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_bsn_t *
    of_instruction_bsn_new(of_version_t version);
extern void of_instruction_bsn_init(
    of_instruction_bsn_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_bsn_arp_offload_t *
    of_instruction_bsn_arp_offload_new(of_version_t version);
extern void of_instruction_bsn_arp_offload_init(
    of_instruction_bsn_arp_offload_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_bsn_deny_t *
    of_instruction_bsn_deny_new(of_version_t version);
extern void of_instruction_bsn_deny_init(
    of_instruction_bsn_deny_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_bsn_dhcp_offload_t *
    of_instruction_bsn_dhcp_offload_new(of_version_t version);
extern void of_instruction_bsn_dhcp_offload_init(
    of_instruction_bsn_dhcp_offload_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_bsn_disable_split_horizon_check_t *
    of_instruction_bsn_disable_split_horizon_check_new(of_version_t version);
extern void of_instruction_bsn_disable_split_horizon_check_init(
    of_instruction_bsn_disable_split_horizon_check_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_bsn_disable_src_mac_check_t *
    of_instruction_bsn_disable_src_mac_check_new(of_version_t version);
extern void of_instruction_bsn_disable_src_mac_check_init(
    of_instruction_bsn_disable_src_mac_check_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_bsn_packet_of_death_t *
    of_instruction_bsn_packet_of_death_new(of_version_t version);
extern void of_instruction_bsn_packet_of_death_init(
    of_instruction_bsn_packet_of_death_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_bsn_permit_t *
    of_instruction_bsn_permit_new(of_version_t version);
extern void of_instruction_bsn_permit_init(
    of_instruction_bsn_permit_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_clear_actions_t *
    of_instruction_clear_actions_new(of_version_t version);
extern void of_instruction_clear_actions_init(
    of_instruction_clear_actions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_experimenter_t *
    of_instruction_experimenter_new(of_version_t version);
extern void of_instruction_experimenter_init(
    of_instruction_experimenter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_goto_table_t *
    of_instruction_goto_table_new(of_version_t version);
extern void of_instruction_goto_table_init(
    of_instruction_goto_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_header_t *
    of_instruction_header_new(of_version_t version);
extern void of_instruction_header_init(
    of_instruction_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_t *
    of_instruction_id_new(of_version_t version);
extern void of_instruction_id_init(
    of_instruction_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_apply_actions_t *
    of_instruction_id_apply_actions_new(of_version_t version);
extern void of_instruction_id_apply_actions_init(
    of_instruction_id_apply_actions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_bsn_t *
    of_instruction_id_bsn_new(of_version_t version);
extern void of_instruction_id_bsn_init(
    of_instruction_id_bsn_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_bsn_arp_offload_t *
    of_instruction_id_bsn_arp_offload_new(of_version_t version);
extern void of_instruction_id_bsn_arp_offload_init(
    of_instruction_id_bsn_arp_offload_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_bsn_deny_t *
    of_instruction_id_bsn_deny_new(of_version_t version);
extern void of_instruction_id_bsn_deny_init(
    of_instruction_id_bsn_deny_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_bsn_dhcp_offload_t *
    of_instruction_id_bsn_dhcp_offload_new(of_version_t version);
extern void of_instruction_id_bsn_dhcp_offload_init(
    of_instruction_id_bsn_dhcp_offload_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_bsn_disable_split_horizon_check_t *
    of_instruction_id_bsn_disable_split_horizon_check_new(of_version_t version);
extern void of_instruction_id_bsn_disable_split_horizon_check_init(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_bsn_disable_src_mac_check_t *
    of_instruction_id_bsn_disable_src_mac_check_new(of_version_t version);
extern void of_instruction_id_bsn_disable_src_mac_check_init(
    of_instruction_id_bsn_disable_src_mac_check_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_bsn_packet_of_death_t *
    of_instruction_id_bsn_packet_of_death_new(of_version_t version);
extern void of_instruction_id_bsn_packet_of_death_init(
    of_instruction_id_bsn_packet_of_death_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_bsn_permit_t *
    of_instruction_id_bsn_permit_new(of_version_t version);
extern void of_instruction_id_bsn_permit_init(
    of_instruction_id_bsn_permit_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_clear_actions_t *
    of_instruction_id_clear_actions_new(of_version_t version);
extern void of_instruction_id_clear_actions_init(
    of_instruction_id_clear_actions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_experimenter_t *
    of_instruction_id_experimenter_new(of_version_t version);
extern void of_instruction_id_experimenter_init(
    of_instruction_id_experimenter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_goto_table_t *
    of_instruction_id_goto_table_new(of_version_t version);
extern void of_instruction_id_goto_table_init(
    of_instruction_id_goto_table_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_header_t *
    of_instruction_id_header_new(of_version_t version);
extern void of_instruction_id_header_init(
    of_instruction_id_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_meter_t *
    of_instruction_id_meter_new(of_version_t version);
extern void of_instruction_id_meter_init(
    of_instruction_id_meter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_write_actions_t *
    of_instruction_id_write_actions_new(of_version_t version);
extern void of_instruction_id_write_actions_init(
    of_instruction_id_write_actions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_id_write_metadata_t *
    of_instruction_id_write_metadata_new(of_version_t version);
extern void of_instruction_id_write_metadata_init(
    of_instruction_id_write_metadata_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_meter_t *
    of_instruction_meter_new(of_version_t version);
extern void of_instruction_meter_init(
    of_instruction_meter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_write_actions_t *
    of_instruction_write_actions_new(of_version_t version);
extern void of_instruction_write_actions_init(
    of_instruction_write_actions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_instruction_write_metadata_t *
    of_instruction_write_metadata_new(of_version_t version);
extern void of_instruction_write_metadata_init(
    of_instruction_write_metadata_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_match_v1_t *
    of_match_v1_new(of_version_t version);
extern void of_match_v1_init(
    of_match_v1_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_match_v2_t *
    of_match_v2_new(of_version_t version);
extern void of_match_v2_init(
    of_match_v2_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_match_v3_t *
    of_match_v3_new(of_version_t version);
extern void of_match_v3_init(
    of_match_v3_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_band_t *
    of_meter_band_new(of_version_t version);
extern void of_meter_band_init(
    of_meter_band_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_band_drop_t *
    of_meter_band_drop_new(of_version_t version);
extern void of_meter_band_drop_init(
    of_meter_band_drop_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_band_dscp_remark_t *
    of_meter_band_dscp_remark_new(of_version_t version);
extern void of_meter_band_dscp_remark_init(
    of_meter_band_dscp_remark_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_band_experimenter_t *
    of_meter_band_experimenter_new(of_version_t version);
extern void of_meter_band_experimenter_init(
    of_meter_band_experimenter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_band_header_t *
    of_meter_band_header_new(of_version_t version);
extern void of_meter_band_header_init(
    of_meter_band_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_band_ofdpa_color_set_t *
    of_meter_band_ofdpa_color_set_new(of_version_t version);
extern void of_meter_band_ofdpa_color_set_init(
    of_meter_band_ofdpa_color_set_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_band_stats_t *
    of_meter_band_stats_new(of_version_t version);
extern void of_meter_band_stats_init(
    of_meter_band_stats_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_config_t *
    of_meter_config_new(of_version_t version);
extern void of_meter_config_init(
    of_meter_config_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_features_t *
    of_meter_features_new(of_version_t version);
extern void of_meter_features_init(
    of_meter_features_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_meter_stats_t *
    of_meter_stats_new(of_version_t version);
extern void of_meter_stats_init(
    of_meter_stats_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_t *
    of_oxm_new(of_version_t version);
extern void of_oxm_init(
    of_oxm_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_op_t *
    of_oxm_arp_op_new(of_version_t version);
extern void of_oxm_arp_op_init(
    of_oxm_arp_op_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_op_masked_t *
    of_oxm_arp_op_masked_new(of_version_t version);
extern void of_oxm_arp_op_masked_init(
    of_oxm_arp_op_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_sha_t *
    of_oxm_arp_sha_new(of_version_t version);
extern void of_oxm_arp_sha_init(
    of_oxm_arp_sha_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_sha_masked_t *
    of_oxm_arp_sha_masked_new(of_version_t version);
extern void of_oxm_arp_sha_masked_init(
    of_oxm_arp_sha_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_spa_t *
    of_oxm_arp_spa_new(of_version_t version);
extern void of_oxm_arp_spa_init(
    of_oxm_arp_spa_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_spa_masked_t *
    of_oxm_arp_spa_masked_new(of_version_t version);
extern void of_oxm_arp_spa_masked_init(
    of_oxm_arp_spa_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_tha_t *
    of_oxm_arp_tha_new(of_version_t version);
extern void of_oxm_arp_tha_init(
    of_oxm_arp_tha_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_tha_masked_t *
    of_oxm_arp_tha_masked_new(of_version_t version);
extern void of_oxm_arp_tha_masked_init(
    of_oxm_arp_tha_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_tpa_t *
    of_oxm_arp_tpa_new(of_version_t version);
extern void of_oxm_arp_tpa_init(
    of_oxm_arp_tpa_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_arp_tpa_masked_t *
    of_oxm_arp_tpa_masked_new(of_version_t version);
extern void of_oxm_arp_tpa_masked_init(
    of_oxm_arp_tpa_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_egr_port_group_id_t *
    of_oxm_bsn_egr_port_group_id_new(of_version_t version);
extern void of_oxm_bsn_egr_port_group_id_init(
    of_oxm_bsn_egr_port_group_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_egr_port_group_id_masked_t *
    of_oxm_bsn_egr_port_group_id_masked_new(of_version_t version);
extern void of_oxm_bsn_egr_port_group_id_masked_init(
    of_oxm_bsn_egr_port_group_id_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_global_vrf_allowed_t *
    of_oxm_bsn_global_vrf_allowed_new(of_version_t version);
extern void of_oxm_bsn_global_vrf_allowed_init(
    of_oxm_bsn_global_vrf_allowed_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_global_vrf_allowed_masked_t *
    of_oxm_bsn_global_vrf_allowed_masked_new(of_version_t version);
extern void of_oxm_bsn_global_vrf_allowed_masked_init(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_in_ports_128_t *
    of_oxm_bsn_in_ports_128_new(of_version_t version);
extern void of_oxm_bsn_in_ports_128_init(
    of_oxm_bsn_in_ports_128_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_in_ports_128_masked_t *
    of_oxm_bsn_in_ports_128_masked_new(of_version_t version);
extern void of_oxm_bsn_in_ports_128_masked_init(
    of_oxm_bsn_in_ports_128_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_l3_dst_class_id_t *
    of_oxm_bsn_l3_dst_class_id_new(of_version_t version);
extern void of_oxm_bsn_l3_dst_class_id_init(
    of_oxm_bsn_l3_dst_class_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_l3_dst_class_id_masked_t *
    of_oxm_bsn_l3_dst_class_id_masked_new(of_version_t version);
extern void of_oxm_bsn_l3_dst_class_id_masked_init(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_l3_interface_class_id_t *
    of_oxm_bsn_l3_interface_class_id_new(of_version_t version);
extern void of_oxm_bsn_l3_interface_class_id_init(
    of_oxm_bsn_l3_interface_class_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_l3_interface_class_id_masked_t *
    of_oxm_bsn_l3_interface_class_id_masked_new(of_version_t version);
extern void of_oxm_bsn_l3_interface_class_id_masked_init(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_l3_src_class_id_t *
    of_oxm_bsn_l3_src_class_id_new(of_version_t version);
extern void of_oxm_bsn_l3_src_class_id_init(
    of_oxm_bsn_l3_src_class_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_l3_src_class_id_masked_t *
    of_oxm_bsn_l3_src_class_id_masked_new(of_version_t version);
extern void of_oxm_bsn_l3_src_class_id_masked_init(
    of_oxm_bsn_l3_src_class_id_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_lag_id_t *
    of_oxm_bsn_lag_id_new(of_version_t version);
extern void of_oxm_bsn_lag_id_init(
    of_oxm_bsn_lag_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_lag_id_masked_t *
    of_oxm_bsn_lag_id_masked_new(of_version_t version);
extern void of_oxm_bsn_lag_id_masked_init(
    of_oxm_bsn_lag_id_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf0_t *
    of_oxm_bsn_udf0_new(of_version_t version);
extern void of_oxm_bsn_udf0_init(
    of_oxm_bsn_udf0_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf0_masked_t *
    of_oxm_bsn_udf0_masked_new(of_version_t version);
extern void of_oxm_bsn_udf0_masked_init(
    of_oxm_bsn_udf0_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf1_t *
    of_oxm_bsn_udf1_new(of_version_t version);
extern void of_oxm_bsn_udf1_init(
    of_oxm_bsn_udf1_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf1_masked_t *
    of_oxm_bsn_udf1_masked_new(of_version_t version);
extern void of_oxm_bsn_udf1_masked_init(
    of_oxm_bsn_udf1_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf2_t *
    of_oxm_bsn_udf2_new(of_version_t version);
extern void of_oxm_bsn_udf2_init(
    of_oxm_bsn_udf2_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf2_masked_t *
    of_oxm_bsn_udf2_masked_new(of_version_t version);
extern void of_oxm_bsn_udf2_masked_init(
    of_oxm_bsn_udf2_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf3_t *
    of_oxm_bsn_udf3_new(of_version_t version);
extern void of_oxm_bsn_udf3_init(
    of_oxm_bsn_udf3_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf3_masked_t *
    of_oxm_bsn_udf3_masked_new(of_version_t version);
extern void of_oxm_bsn_udf3_masked_init(
    of_oxm_bsn_udf3_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf4_t *
    of_oxm_bsn_udf4_new(of_version_t version);
extern void of_oxm_bsn_udf4_init(
    of_oxm_bsn_udf4_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf4_masked_t *
    of_oxm_bsn_udf4_masked_new(of_version_t version);
extern void of_oxm_bsn_udf4_masked_init(
    of_oxm_bsn_udf4_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf5_t *
    of_oxm_bsn_udf5_new(of_version_t version);
extern void of_oxm_bsn_udf5_init(
    of_oxm_bsn_udf5_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf5_masked_t *
    of_oxm_bsn_udf5_masked_new(of_version_t version);
extern void of_oxm_bsn_udf5_masked_init(
    of_oxm_bsn_udf5_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf6_t *
    of_oxm_bsn_udf6_new(of_version_t version);
extern void of_oxm_bsn_udf6_init(
    of_oxm_bsn_udf6_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf6_masked_t *
    of_oxm_bsn_udf6_masked_new(of_version_t version);
extern void of_oxm_bsn_udf6_masked_init(
    of_oxm_bsn_udf6_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf7_t *
    of_oxm_bsn_udf7_new(of_version_t version);
extern void of_oxm_bsn_udf7_init(
    of_oxm_bsn_udf7_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_udf7_masked_t *
    of_oxm_bsn_udf7_masked_new(of_version_t version);
extern void of_oxm_bsn_udf7_masked_init(
    of_oxm_bsn_udf7_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_vrf_t *
    of_oxm_bsn_vrf_new(of_version_t version);
extern void of_oxm_bsn_vrf_init(
    of_oxm_bsn_vrf_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_bsn_vrf_masked_t *
    of_oxm_bsn_vrf_masked_new(of_version_t version);
extern void of_oxm_bsn_vrf_masked_init(
    of_oxm_bsn_vrf_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_eth_dst_t *
    of_oxm_eth_dst_new(of_version_t version);
extern void of_oxm_eth_dst_init(
    of_oxm_eth_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_eth_dst_masked_t *
    of_oxm_eth_dst_masked_new(of_version_t version);
extern void of_oxm_eth_dst_masked_init(
    of_oxm_eth_dst_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_eth_src_t *
    of_oxm_eth_src_new(of_version_t version);
extern void of_oxm_eth_src_init(
    of_oxm_eth_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_eth_src_masked_t *
    of_oxm_eth_src_masked_new(of_version_t version);
extern void of_oxm_eth_src_masked_init(
    of_oxm_eth_src_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_eth_type_t *
    of_oxm_eth_type_new(of_version_t version);
extern void of_oxm_eth_type_init(
    of_oxm_eth_type_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_eth_type_masked_t *
    of_oxm_eth_type_masked_new(of_version_t version);
extern void of_oxm_eth_type_masked_init(
    of_oxm_eth_type_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_header_t *
    of_oxm_header_new(of_version_t version);
extern void of_oxm_header_init(
    of_oxm_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_icmpv4_code_t *
    of_oxm_icmpv4_code_new(of_version_t version);
extern void of_oxm_icmpv4_code_init(
    of_oxm_icmpv4_code_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_icmpv4_code_masked_t *
    of_oxm_icmpv4_code_masked_new(of_version_t version);
extern void of_oxm_icmpv4_code_masked_init(
    of_oxm_icmpv4_code_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_icmpv4_type_t *
    of_oxm_icmpv4_type_new(of_version_t version);
extern void of_oxm_icmpv4_type_init(
    of_oxm_icmpv4_type_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_icmpv4_type_masked_t *
    of_oxm_icmpv4_type_masked_new(of_version_t version);
extern void of_oxm_icmpv4_type_masked_init(
    of_oxm_icmpv4_type_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_icmpv6_code_t *
    of_oxm_icmpv6_code_new(of_version_t version);
extern void of_oxm_icmpv6_code_init(
    of_oxm_icmpv6_code_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_icmpv6_code_masked_t *
    of_oxm_icmpv6_code_masked_new(of_version_t version);
extern void of_oxm_icmpv6_code_masked_init(
    of_oxm_icmpv6_code_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_icmpv6_type_t *
    of_oxm_icmpv6_type_new(of_version_t version);
extern void of_oxm_icmpv6_type_init(
    of_oxm_icmpv6_type_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_icmpv6_type_masked_t *
    of_oxm_icmpv6_type_masked_new(of_version_t version);
extern void of_oxm_icmpv6_type_masked_init(
    of_oxm_icmpv6_type_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_in_phy_port_t *
    of_oxm_in_phy_port_new(of_version_t version);
extern void of_oxm_in_phy_port_init(
    of_oxm_in_phy_port_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_in_phy_port_masked_t *
    of_oxm_in_phy_port_masked_new(of_version_t version);
extern void of_oxm_in_phy_port_masked_init(
    of_oxm_in_phy_port_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_in_port_t *
    of_oxm_in_port_new(of_version_t version);
extern void of_oxm_in_port_init(
    of_oxm_in_port_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_in_port_masked_t *
    of_oxm_in_port_masked_new(of_version_t version);
extern void of_oxm_in_port_masked_init(
    of_oxm_in_port_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ip_dscp_t *
    of_oxm_ip_dscp_new(of_version_t version);
extern void of_oxm_ip_dscp_init(
    of_oxm_ip_dscp_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ip_dscp_masked_t *
    of_oxm_ip_dscp_masked_new(of_version_t version);
extern void of_oxm_ip_dscp_masked_init(
    of_oxm_ip_dscp_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ip_ecn_t *
    of_oxm_ip_ecn_new(of_version_t version);
extern void of_oxm_ip_ecn_init(
    of_oxm_ip_ecn_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ip_ecn_masked_t *
    of_oxm_ip_ecn_masked_new(of_version_t version);
extern void of_oxm_ip_ecn_masked_init(
    of_oxm_ip_ecn_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ip_proto_t *
    of_oxm_ip_proto_new(of_version_t version);
extern void of_oxm_ip_proto_init(
    of_oxm_ip_proto_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ip_proto_masked_t *
    of_oxm_ip_proto_masked_new(of_version_t version);
extern void of_oxm_ip_proto_masked_init(
    of_oxm_ip_proto_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv4_dst_t *
    of_oxm_ipv4_dst_new(of_version_t version);
extern void of_oxm_ipv4_dst_init(
    of_oxm_ipv4_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv4_dst_masked_t *
    of_oxm_ipv4_dst_masked_new(of_version_t version);
extern void of_oxm_ipv4_dst_masked_init(
    of_oxm_ipv4_dst_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv4_src_t *
    of_oxm_ipv4_src_new(of_version_t version);
extern void of_oxm_ipv4_src_init(
    of_oxm_ipv4_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv4_src_masked_t *
    of_oxm_ipv4_src_masked_new(of_version_t version);
extern void of_oxm_ipv4_src_masked_init(
    of_oxm_ipv4_src_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_dst_t *
    of_oxm_ipv6_dst_new(of_version_t version);
extern void of_oxm_ipv6_dst_init(
    of_oxm_ipv6_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_dst_masked_t *
    of_oxm_ipv6_dst_masked_new(of_version_t version);
extern void of_oxm_ipv6_dst_masked_init(
    of_oxm_ipv6_dst_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_flabel_t *
    of_oxm_ipv6_flabel_new(of_version_t version);
extern void of_oxm_ipv6_flabel_init(
    of_oxm_ipv6_flabel_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_flabel_masked_t *
    of_oxm_ipv6_flabel_masked_new(of_version_t version);
extern void of_oxm_ipv6_flabel_masked_init(
    of_oxm_ipv6_flabel_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_nd_sll_t *
    of_oxm_ipv6_nd_sll_new(of_version_t version);
extern void of_oxm_ipv6_nd_sll_init(
    of_oxm_ipv6_nd_sll_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_nd_sll_masked_t *
    of_oxm_ipv6_nd_sll_masked_new(of_version_t version);
extern void of_oxm_ipv6_nd_sll_masked_init(
    of_oxm_ipv6_nd_sll_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_nd_target_t *
    of_oxm_ipv6_nd_target_new(of_version_t version);
extern void of_oxm_ipv6_nd_target_init(
    of_oxm_ipv6_nd_target_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_nd_target_masked_t *
    of_oxm_ipv6_nd_target_masked_new(of_version_t version);
extern void of_oxm_ipv6_nd_target_masked_init(
    of_oxm_ipv6_nd_target_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_nd_tll_t *
    of_oxm_ipv6_nd_tll_new(of_version_t version);
extern void of_oxm_ipv6_nd_tll_init(
    of_oxm_ipv6_nd_tll_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_nd_tll_masked_t *
    of_oxm_ipv6_nd_tll_masked_new(of_version_t version);
extern void of_oxm_ipv6_nd_tll_masked_init(
    of_oxm_ipv6_nd_tll_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_src_t *
    of_oxm_ipv6_src_new(of_version_t version);
extern void of_oxm_ipv6_src_init(
    of_oxm_ipv6_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ipv6_src_masked_t *
    of_oxm_ipv6_src_masked_new(of_version_t version);
extern void of_oxm_ipv6_src_masked_init(
    of_oxm_ipv6_src_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_metadata_t *
    of_oxm_metadata_new(of_version_t version);
extern void of_oxm_metadata_init(
    of_oxm_metadata_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_metadata_masked_t *
    of_oxm_metadata_masked_new(of_version_t version);
extern void of_oxm_metadata_masked_init(
    of_oxm_metadata_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_mpls_bos_t *
    of_oxm_mpls_bos_new(of_version_t version);
extern void of_oxm_mpls_bos_init(
    of_oxm_mpls_bos_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_mpls_bos_masked_t *
    of_oxm_mpls_bos_masked_new(of_version_t version);
extern void of_oxm_mpls_bos_masked_init(
    of_oxm_mpls_bos_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_mpls_label_t *
    of_oxm_mpls_label_new(of_version_t version);
extern void of_oxm_mpls_label_init(
    of_oxm_mpls_label_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_mpls_label_masked_t *
    of_oxm_mpls_label_masked_new(of_version_t version);
extern void of_oxm_mpls_label_masked_init(
    of_oxm_mpls_label_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_mpls_tc_t *
    of_oxm_mpls_tc_new(of_version_t version);
extern void of_oxm_mpls_tc_init(
    of_oxm_mpls_tc_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_mpls_tc_masked_t *
    of_oxm_mpls_tc_masked_new(of_version_t version);
extern void of_oxm_mpls_tc_masked_init(
    of_oxm_mpls_tc_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_allow_vlan_translation_t *
    of_oxm_ofdpa_allow_vlan_translation_new(of_version_t version);
extern void of_oxm_ofdpa_allow_vlan_translation_init(
    of_oxm_ofdpa_allow_vlan_translation_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_color_t *
    of_oxm_ofdpa_color_new(of_version_t version);
extern void of_oxm_ofdpa_color_init(
    of_oxm_ofdpa_color_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_color_actions_index_t *
    of_oxm_ofdpa_color_actions_index_new(of_version_t version);
extern void of_oxm_ofdpa_color_actions_index_init(
    of_oxm_ofdpa_color_actions_index_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_dei_t *
    of_oxm_ofdpa_dei_new(of_version_t version);
extern void of_oxm_ofdpa_dei_init(
    of_oxm_ofdpa_dei_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_l3_in_port_t *
    of_oxm_ofdpa_l3_in_port_new(of_version_t version);
extern void of_oxm_ofdpa_l3_in_port_init(
    of_oxm_ofdpa_l3_in_port_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_lmep_id_t *
    of_oxm_ofdpa_lmep_id_new(of_version_t version);
extern void of_oxm_ofdpa_lmep_id_init(
    of_oxm_ofdpa_lmep_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_mpls_ach_channel_t *
    of_oxm_ofdpa_mpls_ach_channel_new(of_version_t version);
extern void of_oxm_ofdpa_mpls_ach_channel_init(
    of_oxm_ofdpa_mpls_ach_channel_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_mpls_data_first_nibble_t *
    of_oxm_ofdpa_mpls_data_first_nibble_new(of_version_t version);
extern void of_oxm_ofdpa_mpls_data_first_nibble_init(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_mpls_l2_port_t *
    of_oxm_ofdpa_mpls_l2_port_new(of_version_t version);
extern void of_oxm_ofdpa_mpls_l2_port_init(
    of_oxm_ofdpa_mpls_l2_port_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_mpls_l2_port_masked_t *
    of_oxm_ofdpa_mpls_l2_port_masked_new(of_version_t version);
extern void of_oxm_ofdpa_mpls_l2_port_masked_init(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_mpls_next_label_is_gal_t *
    of_oxm_ofdpa_mpls_next_label_is_gal_new(of_version_t version);
extern void of_oxm_ofdpa_mpls_next_label_is_gal_init(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_mpls_ttl_t *
    of_oxm_ofdpa_mpls_ttl_new(of_version_t version);
extern void of_oxm_ofdpa_mpls_ttl_init(
    of_oxm_ofdpa_mpls_ttl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_mpls_type_t *
    of_oxm_ofdpa_mpls_type_new(of_version_t version);
extern void of_oxm_ofdpa_mpls_type_init(
    of_oxm_ofdpa_mpls_type_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_oam_y1731_mdl_t *
    of_oxm_ofdpa_oam_y1731_mdl_new(of_version_t version);
extern void of_oxm_ofdpa_oam_y1731_mdl_init(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_oam_y1731_opcode_t *
    of_oxm_ofdpa_oam_y1731_opcode_new(of_version_t version);
extern void of_oxm_ofdpa_oam_y1731_opcode_init(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_ovid_t *
    of_oxm_ofdpa_ovid_new(of_version_t version);
extern void of_oxm_ofdpa_ovid_init(
    of_oxm_ofdpa_ovid_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_protection_index_t *
    of_oxm_ofdpa_protection_index_new(of_version_t version);
extern void of_oxm_ofdpa_protection_index_init(
    of_oxm_ofdpa_protection_index_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_qos_index_t *
    of_oxm_ofdpa_qos_index_new(of_version_t version);
extern void of_oxm_ofdpa_qos_index_init(
    of_oxm_ofdpa_qos_index_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_rx_timestamp_t *
    of_oxm_ofdpa_rx_timestamp_new(of_version_t version);
extern void of_oxm_ofdpa_rx_timestamp_init(
    of_oxm_ofdpa_rx_timestamp_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_rxfcl_t *
    of_oxm_ofdpa_rxfcl_new(of_version_t version);
extern void of_oxm_ofdpa_rxfcl_init(
    of_oxm_ofdpa_rxfcl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_traffic_class_t *
    of_oxm_ofdpa_traffic_class_new(of_version_t version);
extern void of_oxm_ofdpa_traffic_class_init(
    of_oxm_ofdpa_traffic_class_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_txfcl_t *
    of_oxm_ofdpa_txfcl_new(of_version_t version);
extern void of_oxm_ofdpa_txfcl_init(
    of_oxm_ofdpa_txfcl_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_ofdpa_vrf_t *
    of_oxm_ofdpa_vrf_new(of_version_t version);
extern void of_oxm_ofdpa_vrf_init(
    of_oxm_ofdpa_vrf_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_onf_actset_output_t *
    of_oxm_onf_actset_output_new(of_version_t version);
extern void of_oxm_onf_actset_output_init(
    of_oxm_onf_actset_output_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_sctp_dst_t *
    of_oxm_sctp_dst_new(of_version_t version);
extern void of_oxm_sctp_dst_init(
    of_oxm_sctp_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_sctp_dst_masked_t *
    of_oxm_sctp_dst_masked_new(of_version_t version);
extern void of_oxm_sctp_dst_masked_init(
    of_oxm_sctp_dst_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_sctp_src_t *
    of_oxm_sctp_src_new(of_version_t version);
extern void of_oxm_sctp_src_init(
    of_oxm_sctp_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_sctp_src_masked_t *
    of_oxm_sctp_src_masked_new(of_version_t version);
extern void of_oxm_sctp_src_masked_init(
    of_oxm_sctp_src_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_tcp_dst_t *
    of_oxm_tcp_dst_new(of_version_t version);
extern void of_oxm_tcp_dst_init(
    of_oxm_tcp_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_tcp_dst_masked_t *
    of_oxm_tcp_dst_masked_new(of_version_t version);
extern void of_oxm_tcp_dst_masked_init(
    of_oxm_tcp_dst_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_tcp_src_t *
    of_oxm_tcp_src_new(of_version_t version);
extern void of_oxm_tcp_src_init(
    of_oxm_tcp_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_tcp_src_masked_t *
    of_oxm_tcp_src_masked_new(of_version_t version);
extern void of_oxm_tcp_src_masked_init(
    of_oxm_tcp_src_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_tunnel_id_t *
    of_oxm_tunnel_id_new(of_version_t version);
extern void of_oxm_tunnel_id_init(
    of_oxm_tunnel_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_tunnel_id_masked_t *
    of_oxm_tunnel_id_masked_new(of_version_t version);
extern void of_oxm_tunnel_id_masked_init(
    of_oxm_tunnel_id_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_udp_dst_t *
    of_oxm_udp_dst_new(of_version_t version);
extern void of_oxm_udp_dst_init(
    of_oxm_udp_dst_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_udp_dst_masked_t *
    of_oxm_udp_dst_masked_new(of_version_t version);
extern void of_oxm_udp_dst_masked_init(
    of_oxm_udp_dst_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_udp_src_t *
    of_oxm_udp_src_new(of_version_t version);
extern void of_oxm_udp_src_init(
    of_oxm_udp_src_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_udp_src_masked_t *
    of_oxm_udp_src_masked_new(of_version_t version);
extern void of_oxm_udp_src_masked_init(
    of_oxm_udp_src_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_vlan_pcp_t *
    of_oxm_vlan_pcp_new(of_version_t version);
extern void of_oxm_vlan_pcp_init(
    of_oxm_vlan_pcp_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_vlan_pcp_masked_t *
    of_oxm_vlan_pcp_masked_new(of_version_t version);
extern void of_oxm_vlan_pcp_masked_init(
    of_oxm_vlan_pcp_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_vlan_vid_t *
    of_oxm_vlan_vid_new(of_version_t version);
extern void of_oxm_vlan_vid_init(
    of_oxm_vlan_vid_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_oxm_vlan_vid_masked_t *
    of_oxm_vlan_vid_masked_new(of_version_t version);
extern void of_oxm_vlan_vid_masked_init(
    of_oxm_vlan_vid_masked_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_packet_queue_t *
    of_packet_queue_new(of_version_t version);
extern void of_packet_queue_init(
    of_packet_queue_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_desc_t *
    of_port_desc_new(of_version_t version);
extern void of_port_desc_init(
    of_port_desc_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_port_stats_entry_t *
    of_port_stats_entry_new(of_version_t version);
extern void of_port_stats_entry_init(
    of_port_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_prop_t *
    of_queue_prop_new(of_version_t version);
extern void of_queue_prop_init(
    of_queue_prop_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_prop_experimenter_t *
    of_queue_prop_experimenter_new(of_version_t version);
extern void of_queue_prop_experimenter_init(
    of_queue_prop_experimenter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_prop_header_t *
    of_queue_prop_header_new(of_version_t version);
extern void of_queue_prop_header_init(
    of_queue_prop_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_prop_max_rate_t *
    of_queue_prop_max_rate_new(of_version_t version);
extern void of_queue_prop_max_rate_init(
    of_queue_prop_max_rate_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_prop_min_rate_t *
    of_queue_prop_min_rate_new(of_version_t version);
extern void of_queue_prop_min_rate_init(
    of_queue_prop_min_rate_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_queue_stats_entry_t *
    of_queue_stats_entry_new(of_version_t version);
extern void of_queue_stats_entry_init(
    of_queue_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_t *
    of_table_feature_prop_new(of_version_t version);
extern void of_table_feature_prop_init(
    of_table_feature_prop_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_apply_actions_t *
    of_table_feature_prop_apply_actions_new(of_version_t version);
extern void of_table_feature_prop_apply_actions_init(
    of_table_feature_prop_apply_actions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_apply_actions_miss_t *
    of_table_feature_prop_apply_actions_miss_new(of_version_t version);
extern void of_table_feature_prop_apply_actions_miss_init(
    of_table_feature_prop_apply_actions_miss_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_apply_setfield_t *
    of_table_feature_prop_apply_setfield_new(of_version_t version);
extern void of_table_feature_prop_apply_setfield_init(
    of_table_feature_prop_apply_setfield_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_apply_setfield_miss_t *
    of_table_feature_prop_apply_setfield_miss_new(of_version_t version);
extern void of_table_feature_prop_apply_setfield_miss_init(
    of_table_feature_prop_apply_setfield_miss_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_experimenter_t *
    of_table_feature_prop_experimenter_new(of_version_t version);
extern void of_table_feature_prop_experimenter_init(
    of_table_feature_prop_experimenter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_experimenter_miss_t *
    of_table_feature_prop_experimenter_miss_new(of_version_t version);
extern void of_table_feature_prop_experimenter_miss_init(
    of_table_feature_prop_experimenter_miss_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_header_t *
    of_table_feature_prop_header_new(of_version_t version);
extern void of_table_feature_prop_header_init(
    of_table_feature_prop_header_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_instructions_t *
    of_table_feature_prop_instructions_new(of_version_t version);
extern void of_table_feature_prop_instructions_init(
    of_table_feature_prop_instructions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_instructions_miss_t *
    of_table_feature_prop_instructions_miss_new(of_version_t version);
extern void of_table_feature_prop_instructions_miss_init(
    of_table_feature_prop_instructions_miss_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_match_t *
    of_table_feature_prop_match_new(of_version_t version);
extern void of_table_feature_prop_match_init(
    of_table_feature_prop_match_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_next_tables_t *
    of_table_feature_prop_next_tables_new(of_version_t version);
extern void of_table_feature_prop_next_tables_init(
    of_table_feature_prop_next_tables_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_next_tables_miss_t *
    of_table_feature_prop_next_tables_miss_new(of_version_t version);
extern void of_table_feature_prop_next_tables_miss_init(
    of_table_feature_prop_next_tables_miss_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_wildcards_t *
    of_table_feature_prop_wildcards_new(of_version_t version);
extern void of_table_feature_prop_wildcards_init(
    of_table_feature_prop_wildcards_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_write_actions_t *
    of_table_feature_prop_write_actions_new(of_version_t version);
extern void of_table_feature_prop_write_actions_init(
    of_table_feature_prop_write_actions_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_write_actions_miss_t *
    of_table_feature_prop_write_actions_miss_new(of_version_t version);
extern void of_table_feature_prop_write_actions_miss_init(
    of_table_feature_prop_write_actions_miss_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_write_setfield_t *
    of_table_feature_prop_write_setfield_new(of_version_t version);
extern void of_table_feature_prop_write_setfield_init(
    of_table_feature_prop_write_setfield_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_feature_prop_write_setfield_miss_t *
    of_table_feature_prop_write_setfield_miss_new(of_version_t version);
extern void of_table_feature_prop_write_setfield_miss_init(
    of_table_feature_prop_write_setfield_miss_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_features_t *
    of_table_features_new(of_version_t version);
extern void of_table_features_init(
    of_table_features_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_table_stats_entry_t *
    of_table_stats_entry_new(of_version_t version);
extern void of_table_stats_entry_init(
    of_table_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_uint32_t *
    of_uint32_new(of_version_t version);
extern void of_uint32_init(
    of_uint32_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_uint64_t *
    of_uint64_new(of_version_t version);
extern void of_uint64_init(
    of_uint64_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_uint8_t *
    of_uint8_new(of_version_t version);
extern void of_uint8_init(
    of_uint8_t *obj, of_version_t version, int bytes, int clean_wire);

extern ofp_queue_desc_t *
    ofp_queue_desc_new(of_version_t version);
extern void ofp_queue_desc_init(
    ofp_queue_desc_t *obj, of_version_t version, int bytes, int clean_wire);

extern onf_oxm_packet_regs0_t *
    onf_oxm_packet_regs0_new(of_version_t version);
extern void onf_oxm_packet_regs0_init(
    onf_oxm_packet_regs0_t *obj, of_version_t version, int bytes, int clean_wire);

extern onf_oxm_packet_regs1_t *
    onf_oxm_packet_regs1_new(of_version_t version);
extern void onf_oxm_packet_regs1_init(
    onf_oxm_packet_regs1_t *obj, of_version_t version, int bytes, int clean_wire);

extern onf_oxm_packet_regs2_t *
    onf_oxm_packet_regs2_new(of_version_t version);
extern void onf_oxm_packet_regs2_init(
    onf_oxm_packet_regs2_t *obj, of_version_t version, int bytes, int clean_wire);

extern onf_oxm_packet_regs3_t *
    onf_oxm_packet_regs3_new(of_version_t version);
extern void onf_oxm_packet_regs3_init(
    onf_oxm_packet_regs3_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_action_t *
    of_list_action_new(of_version_t version);
extern void of_list_action_init(
    of_list_action_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_action_id_t *
    of_list_action_id_new(of_version_t version);
extern void of_list_action_id_init(
    of_list_action_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_controller_connection_t *
    of_list_bsn_controller_connection_new(of_version_t version);
extern void of_list_bsn_controller_connection_init(
    of_list_bsn_controller_connection_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_debug_counter_desc_stats_entry_t *
    of_list_bsn_debug_counter_desc_stats_entry_new(of_version_t version);
extern void of_list_bsn_debug_counter_desc_stats_entry_init(
    of_list_bsn_debug_counter_desc_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_debug_counter_stats_entry_t *
    of_list_bsn_debug_counter_stats_entry_new(of_version_t version);
extern void of_list_bsn_debug_counter_stats_entry_init(
    of_list_bsn_debug_counter_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_flow_checksum_bucket_stats_entry_t *
    of_list_bsn_flow_checksum_bucket_stats_entry_new(of_version_t version);
extern void of_list_bsn_flow_checksum_bucket_stats_entry_init(
    of_list_bsn_flow_checksum_bucket_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_gentable_bucket_stats_entry_t *
    of_list_bsn_gentable_bucket_stats_entry_new(of_version_t version);
extern void of_list_bsn_gentable_bucket_stats_entry_init(
    of_list_bsn_gentable_bucket_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_gentable_desc_stats_entry_t *
    of_list_bsn_gentable_desc_stats_entry_new(of_version_t version);
extern void of_list_bsn_gentable_desc_stats_entry_init(
    of_list_bsn_gentable_desc_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_gentable_entry_desc_stats_entry_t *
    of_list_bsn_gentable_entry_desc_stats_entry_new(of_version_t version);
extern void of_list_bsn_gentable_entry_desc_stats_entry_init(
    of_list_bsn_gentable_entry_desc_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_gentable_entry_stats_entry_t *
    of_list_bsn_gentable_entry_stats_entry_new(of_version_t version);
extern void of_list_bsn_gentable_entry_stats_entry_init(
    of_list_bsn_gentable_entry_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_gentable_stats_entry_t *
    of_list_bsn_gentable_stats_entry_new(of_version_t version);
extern void of_list_bsn_gentable_stats_entry_init(
    of_list_bsn_gentable_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_interface_t *
    of_list_bsn_interface_new(of_version_t version);
extern void of_list_bsn_interface_init(
    of_list_bsn_interface_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_lacp_stats_entry_t *
    of_list_bsn_lacp_stats_entry_new(of_version_t version);
extern void of_list_bsn_lacp_stats_entry_init(
    of_list_bsn_lacp_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_port_counter_stats_entry_t *
    of_list_bsn_port_counter_stats_entry_new(of_version_t version);
extern void of_list_bsn_port_counter_stats_entry_init(
    of_list_bsn_port_counter_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_switch_pipeline_stats_entry_t *
    of_list_bsn_switch_pipeline_stats_entry_new(of_version_t version);
extern void of_list_bsn_switch_pipeline_stats_entry_init(
    of_list_bsn_switch_pipeline_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_table_checksum_stats_entry_t *
    of_list_bsn_table_checksum_stats_entry_new(of_version_t version);
extern void of_list_bsn_table_checksum_stats_entry_init(
    of_list_bsn_table_checksum_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_tlv_t *
    of_list_bsn_tlv_new(of_version_t version);
extern void of_list_bsn_tlv_init(
    of_list_bsn_tlv_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bsn_vlan_counter_stats_entry_t *
    of_list_bsn_vlan_counter_stats_entry_new(of_version_t version);
extern void of_list_bsn_vlan_counter_stats_entry_init(
    of_list_bsn_vlan_counter_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bucket_t *
    of_list_bucket_new(of_version_t version);
extern void of_list_bucket_init(
    of_list_bucket_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_bucket_counter_t *
    of_list_bucket_counter_new(of_version_t version);
extern void of_list_bucket_counter_init(
    of_list_bucket_counter_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_flow_stats_entry_t *
    of_list_flow_stats_entry_new(of_version_t version);
extern void of_list_flow_stats_entry_init(
    of_list_flow_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_group_desc_stats_entry_t *
    of_list_group_desc_stats_entry_new(of_version_t version);
extern void of_list_group_desc_stats_entry_init(
    of_list_group_desc_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_group_stats_entry_t *
    of_list_group_stats_entry_new(of_version_t version);
extern void of_list_group_stats_entry_init(
    of_list_group_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_hello_elem_t *
    of_list_hello_elem_new(of_version_t version);
extern void of_list_hello_elem_init(
    of_list_hello_elem_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_instruction_t *
    of_list_instruction_new(of_version_t version);
extern void of_list_instruction_init(
    of_list_instruction_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_instruction_id_t *
    of_list_instruction_id_new(of_version_t version);
extern void of_list_instruction_id_init(
    of_list_instruction_id_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_meter_band_t *
    of_list_meter_band_new(of_version_t version);
extern void of_list_meter_band_init(
    of_list_meter_band_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_meter_band_stats_t *
    of_list_meter_band_stats_new(of_version_t version);
extern void of_list_meter_band_stats_init(
    of_list_meter_band_stats_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_meter_stats_t *
    of_list_meter_stats_new(of_version_t version);
extern void of_list_meter_stats_init(
    of_list_meter_stats_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_oxm_t *
    of_list_oxm_new(of_version_t version);
extern void of_list_oxm_init(
    of_list_oxm_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_packet_queue_t *
    of_list_packet_queue_new(of_version_t version);
extern void of_list_packet_queue_init(
    of_list_packet_queue_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_port_desc_t *
    of_list_port_desc_new(of_version_t version);
extern void of_list_port_desc_init(
    of_list_port_desc_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_port_stats_entry_t *
    of_list_port_stats_entry_new(of_version_t version);
extern void of_list_port_stats_entry_init(
    of_list_port_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_queue_prop_t *
    of_list_queue_prop_new(of_version_t version);
extern void of_list_queue_prop_init(
    of_list_queue_prop_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_queue_stats_entry_t *
    of_list_queue_stats_entry_new(of_version_t version);
extern void of_list_queue_stats_entry_init(
    of_list_queue_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_table_feature_prop_t *
    of_list_table_feature_prop_new(of_version_t version);
extern void of_list_table_feature_prop_init(
    of_list_table_feature_prop_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_table_features_t *
    of_list_table_features_new(of_version_t version);
extern void of_list_table_features_init(
    of_list_table_features_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_table_stats_entry_t *
    of_list_table_stats_entry_new(of_version_t version);
extern void of_list_table_stats_entry_init(
    of_list_table_stats_entry_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_uint32_t *
    of_list_uint32_new(of_version_t version);
extern void of_list_uint32_init(
    of_list_uint32_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_uint64_t *
    of_list_uint64_new(of_version_t version);
extern void of_list_uint64_init(
    of_list_uint64_t *obj, of_version_t version, int bytes, int clean_wire);

extern of_list_uint8_t *
    of_list_uint8_new(of_version_t version);
extern void of_list_uint8_init(
    of_list_uint8_t *obj, of_version_t version, int bytes, int clean_wire);

/****************************************************************
 *
 * Delete operator static inline definitions.
 * These are here for type checking purposes only
 *
 ****************************************************************/

/**
 * Delete an object of type of_aggregate_stats_reply_t
 * @param obj An instance of type of_aggregate_stats_reply_t
 *
 * \ingroup of_aggregate_stats_reply
 */
static inline void
of_aggregate_stats_reply_delete(of_aggregate_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_aggregate_stats_request_t
 * @param obj An instance of type of_aggregate_stats_request_t
 *
 * \ingroup of_aggregate_stats_request
 */
static inline void
of_aggregate_stats_request_delete(of_aggregate_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_async_get_reply_t
 * @param obj An instance of type of_async_get_reply_t
 *
 * \ingroup of_async_get_reply
 */
static inline void
of_async_get_reply_delete(of_async_get_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_async_get_request_t
 * @param obj An instance of type of_async_get_request_t
 *
 * \ingroup of_async_get_request
 */
static inline void
of_async_get_request_delete(of_async_get_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_async_set_t
 * @param obj An instance of type of_async_set_t
 *
 * \ingroup of_async_set
 */
static inline void
of_async_set_delete(of_async_set_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bad_action_error_msg_t
 * @param obj An instance of type of_bad_action_error_msg_t
 *
 * \ingroup of_bad_action_error_msg
 */
static inline void
of_bad_action_error_msg_delete(of_bad_action_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bad_instruction_error_msg_t
 * @param obj An instance of type of_bad_instruction_error_msg_t
 *
 * \ingroup of_bad_instruction_error_msg
 */
static inline void
of_bad_instruction_error_msg_delete(of_bad_instruction_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bad_match_error_msg_t
 * @param obj An instance of type of_bad_match_error_msg_t
 *
 * \ingroup of_bad_match_error_msg
 */
static inline void
of_bad_match_error_msg_delete(of_bad_match_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bad_request_error_msg_t
 * @param obj An instance of type of_bad_request_error_msg_t
 *
 * \ingroup of_bad_request_error_msg
 */
static inline void
of_bad_request_error_msg_delete(of_bad_request_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_barrier_reply_t
 * @param obj An instance of type of_barrier_reply_t
 *
 * \ingroup of_barrier_reply
 */
static inline void
of_barrier_reply_delete(of_barrier_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_barrier_request_t
 * @param obj An instance of type of_barrier_request_t
 *
 * \ingroup of_barrier_request
 */
static inline void
of_barrier_request_delete(of_barrier_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_arp_idle_t
 * @param obj An instance of type of_bsn_arp_idle_t
 *
 * \ingroup of_bsn_arp_idle
 */
static inline void
of_bsn_arp_idle_delete(of_bsn_arp_idle_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_bw_clear_data_reply_t
 * @param obj An instance of type of_bsn_bw_clear_data_reply_t
 *
 * \ingroup of_bsn_bw_clear_data_reply
 */
static inline void
of_bsn_bw_clear_data_reply_delete(of_bsn_bw_clear_data_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_bw_clear_data_request_t
 * @param obj An instance of type of_bsn_bw_clear_data_request_t
 *
 * \ingroup of_bsn_bw_clear_data_request
 */
static inline void
of_bsn_bw_clear_data_request_delete(of_bsn_bw_clear_data_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_bw_enable_get_reply_t
 * @param obj An instance of type of_bsn_bw_enable_get_reply_t
 *
 * \ingroup of_bsn_bw_enable_get_reply
 */
static inline void
of_bsn_bw_enable_get_reply_delete(of_bsn_bw_enable_get_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_bw_enable_get_request_t
 * @param obj An instance of type of_bsn_bw_enable_get_request_t
 *
 * \ingroup of_bsn_bw_enable_get_request
 */
static inline void
of_bsn_bw_enable_get_request_delete(of_bsn_bw_enable_get_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_bw_enable_set_reply_t
 * @param obj An instance of type of_bsn_bw_enable_set_reply_t
 *
 * \ingroup of_bsn_bw_enable_set_reply
 */
static inline void
of_bsn_bw_enable_set_reply_delete(of_bsn_bw_enable_set_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_bw_enable_set_request_t
 * @param obj An instance of type of_bsn_bw_enable_set_request_t
 *
 * \ingroup of_bsn_bw_enable_set_request
 */
static inline void
of_bsn_bw_enable_set_request_delete(of_bsn_bw_enable_set_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_controller_connections_reply_t
 * @param obj An instance of type of_bsn_controller_connections_reply_t
 *
 * \ingroup of_bsn_controller_connections_reply
 */
static inline void
of_bsn_controller_connections_reply_delete(of_bsn_controller_connections_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_controller_connections_request_t
 * @param obj An instance of type of_bsn_controller_connections_request_t
 *
 * \ingroup of_bsn_controller_connections_request
 */
static inline void
of_bsn_controller_connections_request_delete(of_bsn_controller_connections_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_debug_counter_desc_stats_reply_t
 * @param obj An instance of type of_bsn_debug_counter_desc_stats_reply_t
 *
 * \ingroup of_bsn_debug_counter_desc_stats_reply
 */
static inline void
of_bsn_debug_counter_desc_stats_reply_delete(of_bsn_debug_counter_desc_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_debug_counter_desc_stats_request_t
 * @param obj An instance of type of_bsn_debug_counter_desc_stats_request_t
 *
 * \ingroup of_bsn_debug_counter_desc_stats_request
 */
static inline void
of_bsn_debug_counter_desc_stats_request_delete(of_bsn_debug_counter_desc_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_debug_counter_stats_reply_t
 * @param obj An instance of type of_bsn_debug_counter_stats_reply_t
 *
 * \ingroup of_bsn_debug_counter_stats_reply
 */
static inline void
of_bsn_debug_counter_stats_reply_delete(of_bsn_debug_counter_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_debug_counter_stats_request_t
 * @param obj An instance of type of_bsn_debug_counter_stats_request_t
 *
 * \ingroup of_bsn_debug_counter_stats_request
 */
static inline void
of_bsn_debug_counter_stats_request_delete(of_bsn_debug_counter_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_flow_checksum_bucket_stats_reply_t
 * @param obj An instance of type of_bsn_flow_checksum_bucket_stats_reply_t
 *
 * \ingroup of_bsn_flow_checksum_bucket_stats_reply
 */
static inline void
of_bsn_flow_checksum_bucket_stats_reply_delete(of_bsn_flow_checksum_bucket_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_flow_checksum_bucket_stats_request_t
 * @param obj An instance of type of_bsn_flow_checksum_bucket_stats_request_t
 *
 * \ingroup of_bsn_flow_checksum_bucket_stats_request
 */
static inline void
of_bsn_flow_checksum_bucket_stats_request_delete(of_bsn_flow_checksum_bucket_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_flow_idle_t
 * @param obj An instance of type of_bsn_flow_idle_t
 *
 * \ingroup of_bsn_flow_idle
 */
static inline void
of_bsn_flow_idle_delete(of_bsn_flow_idle_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_flow_idle_enable_get_reply_t
 * @param obj An instance of type of_bsn_flow_idle_enable_get_reply_t
 *
 * \ingroup of_bsn_flow_idle_enable_get_reply
 */
static inline void
of_bsn_flow_idle_enable_get_reply_delete(of_bsn_flow_idle_enable_get_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_flow_idle_enable_get_request_t
 * @param obj An instance of type of_bsn_flow_idle_enable_get_request_t
 *
 * \ingroup of_bsn_flow_idle_enable_get_request
 */
static inline void
of_bsn_flow_idle_enable_get_request_delete(of_bsn_flow_idle_enable_get_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_flow_idle_enable_set_reply_t
 * @param obj An instance of type of_bsn_flow_idle_enable_set_reply_t
 *
 * \ingroup of_bsn_flow_idle_enable_set_reply
 */
static inline void
of_bsn_flow_idle_enable_set_reply_delete(of_bsn_flow_idle_enable_set_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_flow_idle_enable_set_request_t
 * @param obj An instance of type of_bsn_flow_idle_enable_set_request_t
 *
 * \ingroup of_bsn_flow_idle_enable_set_request
 */
static inline void
of_bsn_flow_idle_enable_set_request_delete(of_bsn_flow_idle_enable_set_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_bucket_stats_reply_t
 * @param obj An instance of type of_bsn_gentable_bucket_stats_reply_t
 *
 * \ingroup of_bsn_gentable_bucket_stats_reply
 */
static inline void
of_bsn_gentable_bucket_stats_reply_delete(of_bsn_gentable_bucket_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_bucket_stats_request_t
 * @param obj An instance of type of_bsn_gentable_bucket_stats_request_t
 *
 * \ingroup of_bsn_gentable_bucket_stats_request
 */
static inline void
of_bsn_gentable_bucket_stats_request_delete(of_bsn_gentable_bucket_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_clear_reply_t
 * @param obj An instance of type of_bsn_gentable_clear_reply_t
 *
 * \ingroup of_bsn_gentable_clear_reply
 */
static inline void
of_bsn_gentable_clear_reply_delete(of_bsn_gentable_clear_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_clear_request_t
 * @param obj An instance of type of_bsn_gentable_clear_request_t
 *
 * \ingroup of_bsn_gentable_clear_request
 */
static inline void
of_bsn_gentable_clear_request_delete(of_bsn_gentable_clear_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_desc_stats_reply_t
 * @param obj An instance of type of_bsn_gentable_desc_stats_reply_t
 *
 * \ingroup of_bsn_gentable_desc_stats_reply
 */
static inline void
of_bsn_gentable_desc_stats_reply_delete(of_bsn_gentable_desc_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_desc_stats_request_t
 * @param obj An instance of type of_bsn_gentable_desc_stats_request_t
 *
 * \ingroup of_bsn_gentable_desc_stats_request
 */
static inline void
of_bsn_gentable_desc_stats_request_delete(of_bsn_gentable_desc_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_entry_add_t
 * @param obj An instance of type of_bsn_gentable_entry_add_t
 *
 * \ingroup of_bsn_gentable_entry_add
 */
static inline void
of_bsn_gentable_entry_add_delete(of_bsn_gentable_entry_add_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_entry_delete_t
 * @param obj An instance of type of_bsn_gentable_entry_delete_t
 *
 * \ingroup of_bsn_gentable_entry_delete
 */
static inline void
of_bsn_gentable_entry_delete_delete(of_bsn_gentable_entry_delete_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_entry_desc_stats_reply_t
 * @param obj An instance of type of_bsn_gentable_entry_desc_stats_reply_t
 *
 * \ingroup of_bsn_gentable_entry_desc_stats_reply
 */
static inline void
of_bsn_gentable_entry_desc_stats_reply_delete(of_bsn_gentable_entry_desc_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_entry_desc_stats_request_t
 * @param obj An instance of type of_bsn_gentable_entry_desc_stats_request_t
 *
 * \ingroup of_bsn_gentable_entry_desc_stats_request
 */
static inline void
of_bsn_gentable_entry_desc_stats_request_delete(of_bsn_gentable_entry_desc_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_entry_stats_reply_t
 * @param obj An instance of type of_bsn_gentable_entry_stats_reply_t
 *
 * \ingroup of_bsn_gentable_entry_stats_reply
 */
static inline void
of_bsn_gentable_entry_stats_reply_delete(of_bsn_gentable_entry_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_entry_stats_request_t
 * @param obj An instance of type of_bsn_gentable_entry_stats_request_t
 *
 * \ingroup of_bsn_gentable_entry_stats_request
 */
static inline void
of_bsn_gentable_entry_stats_request_delete(of_bsn_gentable_entry_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_set_buckets_size_t
 * @param obj An instance of type of_bsn_gentable_set_buckets_size_t
 *
 * \ingroup of_bsn_gentable_set_buckets_size
 */
static inline void
of_bsn_gentable_set_buckets_size_delete(of_bsn_gentable_set_buckets_size_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_stats_reply_t
 * @param obj An instance of type of_bsn_gentable_stats_reply_t
 *
 * \ingroup of_bsn_gentable_stats_reply
 */
static inline void
of_bsn_gentable_stats_reply_delete(of_bsn_gentable_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_stats_request_t
 * @param obj An instance of type of_bsn_gentable_stats_request_t
 *
 * \ingroup of_bsn_gentable_stats_request
 */
static inline void
of_bsn_gentable_stats_request_delete(of_bsn_gentable_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_interfaces_reply_t
 * @param obj An instance of type of_bsn_get_interfaces_reply_t
 *
 * \ingroup of_bsn_get_interfaces_reply
 */
static inline void
of_bsn_get_interfaces_reply_delete(of_bsn_get_interfaces_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_interfaces_request_t
 * @param obj An instance of type of_bsn_get_interfaces_request_t
 *
 * \ingroup of_bsn_get_interfaces_request
 */
static inline void
of_bsn_get_interfaces_request_delete(of_bsn_get_interfaces_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_ip_mask_reply_t
 * @param obj An instance of type of_bsn_get_ip_mask_reply_t
 *
 * \ingroup of_bsn_get_ip_mask_reply
 */
static inline void
of_bsn_get_ip_mask_reply_delete(of_bsn_get_ip_mask_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_ip_mask_request_t
 * @param obj An instance of type of_bsn_get_ip_mask_request_t
 *
 * \ingroup of_bsn_get_ip_mask_request
 */
static inline void
of_bsn_get_ip_mask_request_delete(of_bsn_get_ip_mask_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_l2_table_reply_t
 * @param obj An instance of type of_bsn_get_l2_table_reply_t
 *
 * \ingroup of_bsn_get_l2_table_reply
 */
static inline void
of_bsn_get_l2_table_reply_delete(of_bsn_get_l2_table_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_l2_table_request_t
 * @param obj An instance of type of_bsn_get_l2_table_request_t
 *
 * \ingroup of_bsn_get_l2_table_request
 */
static inline void
of_bsn_get_l2_table_request_delete(of_bsn_get_l2_table_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_mirroring_reply_t
 * @param obj An instance of type of_bsn_get_mirroring_reply_t
 *
 * \ingroup of_bsn_get_mirroring_reply
 */
static inline void
of_bsn_get_mirroring_reply_delete(of_bsn_get_mirroring_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_mirroring_request_t
 * @param obj An instance of type of_bsn_get_mirroring_request_t
 *
 * \ingroup of_bsn_get_mirroring_request
 */
static inline void
of_bsn_get_mirroring_request_delete(of_bsn_get_mirroring_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_switch_pipeline_reply_t
 * @param obj An instance of type of_bsn_get_switch_pipeline_reply_t
 *
 * \ingroup of_bsn_get_switch_pipeline_reply
 */
static inline void
of_bsn_get_switch_pipeline_reply_delete(of_bsn_get_switch_pipeline_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_get_switch_pipeline_request_t
 * @param obj An instance of type of_bsn_get_switch_pipeline_request_t
 *
 * \ingroup of_bsn_get_switch_pipeline_request
 */
static inline void
of_bsn_get_switch_pipeline_request_delete(of_bsn_get_switch_pipeline_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_header_t
 * @param obj An instance of type of_bsn_header_t
 *
 * \ingroup of_bsn_header
 */
static inline void
of_bsn_header_delete(of_bsn_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_hybrid_get_reply_t
 * @param obj An instance of type of_bsn_hybrid_get_reply_t
 *
 * \ingroup of_bsn_hybrid_get_reply
 */
static inline void
of_bsn_hybrid_get_reply_delete(of_bsn_hybrid_get_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_hybrid_get_request_t
 * @param obj An instance of type of_bsn_hybrid_get_request_t
 *
 * \ingroup of_bsn_hybrid_get_request
 */
static inline void
of_bsn_hybrid_get_request_delete(of_bsn_hybrid_get_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_lacp_convergence_notif_t
 * @param obj An instance of type of_bsn_lacp_convergence_notif_t
 *
 * \ingroup of_bsn_lacp_convergence_notif
 */
static inline void
of_bsn_lacp_convergence_notif_delete(of_bsn_lacp_convergence_notif_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_lacp_stats_reply_t
 * @param obj An instance of type of_bsn_lacp_stats_reply_t
 *
 * \ingroup of_bsn_lacp_stats_reply
 */
static inline void
of_bsn_lacp_stats_reply_delete(of_bsn_lacp_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_lacp_stats_request_t
 * @param obj An instance of type of_bsn_lacp_stats_request_t
 *
 * \ingroup of_bsn_lacp_stats_request
 */
static inline void
of_bsn_lacp_stats_request_delete(of_bsn_lacp_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_pdu_rx_reply_t
 * @param obj An instance of type of_bsn_pdu_rx_reply_t
 *
 * \ingroup of_bsn_pdu_rx_reply
 */
static inline void
of_bsn_pdu_rx_reply_delete(of_bsn_pdu_rx_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_pdu_rx_request_t
 * @param obj An instance of type of_bsn_pdu_rx_request_t
 *
 * \ingroup of_bsn_pdu_rx_request
 */
static inline void
of_bsn_pdu_rx_request_delete(of_bsn_pdu_rx_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_pdu_rx_timeout_t
 * @param obj An instance of type of_bsn_pdu_rx_timeout_t
 *
 * \ingroup of_bsn_pdu_rx_timeout
 */
static inline void
of_bsn_pdu_rx_timeout_delete(of_bsn_pdu_rx_timeout_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_pdu_tx_reply_t
 * @param obj An instance of type of_bsn_pdu_tx_reply_t
 *
 * \ingroup of_bsn_pdu_tx_reply
 */
static inline void
of_bsn_pdu_tx_reply_delete(of_bsn_pdu_tx_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_pdu_tx_request_t
 * @param obj An instance of type of_bsn_pdu_tx_request_t
 *
 * \ingroup of_bsn_pdu_tx_request
 */
static inline void
of_bsn_pdu_tx_request_delete(of_bsn_pdu_tx_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_port_counter_stats_reply_t
 * @param obj An instance of type of_bsn_port_counter_stats_reply_t
 *
 * \ingroup of_bsn_port_counter_stats_reply
 */
static inline void
of_bsn_port_counter_stats_reply_delete(of_bsn_port_counter_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_port_counter_stats_request_t
 * @param obj An instance of type of_bsn_port_counter_stats_request_t
 *
 * \ingroup of_bsn_port_counter_stats_request
 */
static inline void
of_bsn_port_counter_stats_request_delete(of_bsn_port_counter_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_role_status_t
 * @param obj An instance of type of_bsn_role_status_t
 *
 * \ingroup of_bsn_role_status
 */
static inline void
of_bsn_role_status_delete(of_bsn_role_status_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_aux_cxns_reply_t
 * @param obj An instance of type of_bsn_set_aux_cxns_reply_t
 *
 * \ingroup of_bsn_set_aux_cxns_reply
 */
static inline void
of_bsn_set_aux_cxns_reply_delete(of_bsn_set_aux_cxns_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_aux_cxns_request_t
 * @param obj An instance of type of_bsn_set_aux_cxns_request_t
 *
 * \ingroup of_bsn_set_aux_cxns_request
 */
static inline void
of_bsn_set_aux_cxns_request_delete(of_bsn_set_aux_cxns_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_ip_mask_t
 * @param obj An instance of type of_bsn_set_ip_mask_t
 *
 * \ingroup of_bsn_set_ip_mask
 */
static inline void
of_bsn_set_ip_mask_delete(of_bsn_set_ip_mask_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_l2_table_reply_t
 * @param obj An instance of type of_bsn_set_l2_table_reply_t
 *
 * \ingroup of_bsn_set_l2_table_reply
 */
static inline void
of_bsn_set_l2_table_reply_delete(of_bsn_set_l2_table_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_l2_table_request_t
 * @param obj An instance of type of_bsn_set_l2_table_request_t
 *
 * \ingroup of_bsn_set_l2_table_request
 */
static inline void
of_bsn_set_l2_table_request_delete(of_bsn_set_l2_table_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_lacp_reply_t
 * @param obj An instance of type of_bsn_set_lacp_reply_t
 *
 * \ingroup of_bsn_set_lacp_reply
 */
static inline void
of_bsn_set_lacp_reply_delete(of_bsn_set_lacp_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_lacp_request_t
 * @param obj An instance of type of_bsn_set_lacp_request_t
 *
 * \ingroup of_bsn_set_lacp_request
 */
static inline void
of_bsn_set_lacp_request_delete(of_bsn_set_lacp_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_mirroring_t
 * @param obj An instance of type of_bsn_set_mirroring_t
 *
 * \ingroup of_bsn_set_mirroring
 */
static inline void
of_bsn_set_mirroring_delete(of_bsn_set_mirroring_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_pktin_suppression_reply_t
 * @param obj An instance of type of_bsn_set_pktin_suppression_reply_t
 *
 * \ingroup of_bsn_set_pktin_suppression_reply
 */
static inline void
of_bsn_set_pktin_suppression_reply_delete(of_bsn_set_pktin_suppression_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_pktin_suppression_request_t
 * @param obj An instance of type of_bsn_set_pktin_suppression_request_t
 *
 * \ingroup of_bsn_set_pktin_suppression_request
 */
static inline void
of_bsn_set_pktin_suppression_request_delete(of_bsn_set_pktin_suppression_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_switch_pipeline_reply_t
 * @param obj An instance of type of_bsn_set_switch_pipeline_reply_t
 *
 * \ingroup of_bsn_set_switch_pipeline_reply
 */
static inline void
of_bsn_set_switch_pipeline_reply_delete(of_bsn_set_switch_pipeline_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_set_switch_pipeline_request_t
 * @param obj An instance of type of_bsn_set_switch_pipeline_request_t
 *
 * \ingroup of_bsn_set_switch_pipeline_request
 */
static inline void
of_bsn_set_switch_pipeline_request_delete(of_bsn_set_switch_pipeline_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_shell_command_t
 * @param obj An instance of type of_bsn_shell_command_t
 *
 * \ingroup of_bsn_shell_command
 */
static inline void
of_bsn_shell_command_delete(of_bsn_shell_command_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_shell_output_t
 * @param obj An instance of type of_bsn_shell_output_t
 *
 * \ingroup of_bsn_shell_output
 */
static inline void
of_bsn_shell_output_delete(of_bsn_shell_output_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_shell_status_t
 * @param obj An instance of type of_bsn_shell_status_t
 *
 * \ingroup of_bsn_shell_status
 */
static inline void
of_bsn_shell_status_delete(of_bsn_shell_status_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_stats_reply_t
 * @param obj An instance of type of_bsn_stats_reply_t
 *
 * \ingroup of_bsn_stats_reply
 */
static inline void
of_bsn_stats_reply_delete(of_bsn_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_stats_request_t
 * @param obj An instance of type of_bsn_stats_request_t
 *
 * \ingroup of_bsn_stats_request
 */
static inline void
of_bsn_stats_request_delete(of_bsn_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_switch_pipeline_stats_reply_t
 * @param obj An instance of type of_bsn_switch_pipeline_stats_reply_t
 *
 * \ingroup of_bsn_switch_pipeline_stats_reply
 */
static inline void
of_bsn_switch_pipeline_stats_reply_delete(of_bsn_switch_pipeline_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_switch_pipeline_stats_request_t
 * @param obj An instance of type of_bsn_switch_pipeline_stats_request_t
 *
 * \ingroup of_bsn_switch_pipeline_stats_request
 */
static inline void
of_bsn_switch_pipeline_stats_request_delete(of_bsn_switch_pipeline_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_table_checksum_stats_reply_t
 * @param obj An instance of type of_bsn_table_checksum_stats_reply_t
 *
 * \ingroup of_bsn_table_checksum_stats_reply
 */
static inline void
of_bsn_table_checksum_stats_reply_delete(of_bsn_table_checksum_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_table_checksum_stats_request_t
 * @param obj An instance of type of_bsn_table_checksum_stats_request_t
 *
 * \ingroup of_bsn_table_checksum_stats_request
 */
static inline void
of_bsn_table_checksum_stats_request_delete(of_bsn_table_checksum_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_table_set_buckets_size_t
 * @param obj An instance of type of_bsn_table_set_buckets_size_t
 *
 * \ingroup of_bsn_table_set_buckets_size
 */
static inline void
of_bsn_table_set_buckets_size_delete(of_bsn_table_set_buckets_size_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_time_reply_t
 * @param obj An instance of type of_bsn_time_reply_t
 *
 * \ingroup of_bsn_time_reply
 */
static inline void
of_bsn_time_reply_delete(of_bsn_time_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_time_request_t
 * @param obj An instance of type of_bsn_time_request_t
 *
 * \ingroup of_bsn_time_request
 */
static inline void
of_bsn_time_request_delete(of_bsn_time_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_virtual_port_create_reply_t
 * @param obj An instance of type of_bsn_virtual_port_create_reply_t
 *
 * \ingroup of_bsn_virtual_port_create_reply
 */
static inline void
of_bsn_virtual_port_create_reply_delete(of_bsn_virtual_port_create_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_virtual_port_create_request_t
 * @param obj An instance of type of_bsn_virtual_port_create_request_t
 *
 * \ingroup of_bsn_virtual_port_create_request
 */
static inline void
of_bsn_virtual_port_create_request_delete(of_bsn_virtual_port_create_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_virtual_port_remove_reply_t
 * @param obj An instance of type of_bsn_virtual_port_remove_reply_t
 *
 * \ingroup of_bsn_virtual_port_remove_reply
 */
static inline void
of_bsn_virtual_port_remove_reply_delete(of_bsn_virtual_port_remove_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_virtual_port_remove_request_t
 * @param obj An instance of type of_bsn_virtual_port_remove_request_t
 *
 * \ingroup of_bsn_virtual_port_remove_request
 */
static inline void
of_bsn_virtual_port_remove_request_delete(of_bsn_virtual_port_remove_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_vlan_counter_stats_reply_t
 * @param obj An instance of type of_bsn_vlan_counter_stats_reply_t
 *
 * \ingroup of_bsn_vlan_counter_stats_reply
 */
static inline void
of_bsn_vlan_counter_stats_reply_delete(of_bsn_vlan_counter_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_vlan_counter_stats_request_t
 * @param obj An instance of type of_bsn_vlan_counter_stats_request_t
 *
 * \ingroup of_bsn_vlan_counter_stats_request
 */
static inline void
of_bsn_vlan_counter_stats_request_delete(of_bsn_vlan_counter_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_desc_stats_reply_t
 * @param obj An instance of type of_desc_stats_reply_t
 *
 * \ingroup of_desc_stats_reply
 */
static inline void
of_desc_stats_reply_delete(of_desc_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_desc_stats_request_t
 * @param obj An instance of type of_desc_stats_request_t
 *
 * \ingroup of_desc_stats_request
 */
static inline void
of_desc_stats_request_delete(of_desc_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_echo_reply_t
 * @param obj An instance of type of_echo_reply_t
 *
 * \ingroup of_echo_reply
 */
static inline void
of_echo_reply_delete(of_echo_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_echo_request_t
 * @param obj An instance of type of_echo_request_t
 *
 * \ingroup of_echo_request
 */
static inline void
of_echo_request_delete(of_echo_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_error_msg_t
 * @param obj An instance of type of_error_msg_t
 *
 * \ingroup of_error_msg
 */
static inline void
of_error_msg_delete(of_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_experimenter_t
 * @param obj An instance of type of_experimenter_t
 *
 * \ingroup of_experimenter
 */
static inline void
of_experimenter_delete(of_experimenter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_experimenter_error_msg_t
 * @param obj An instance of type of_experimenter_error_msg_t
 *
 * \ingroup of_experimenter_error_msg
 */
static inline void
of_experimenter_error_msg_delete(of_experimenter_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_experimenter_multipart_reply_ofdpa_t
 * @param obj An instance of type of_experimenter_multipart_reply_ofdpa_t
 *
 * \ingroup of_experimenter_multipart_reply_ofdpa
 */
static inline void
of_experimenter_multipart_reply_ofdpa_delete(of_experimenter_multipart_reply_ofdpa_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_experimenter_multipart_request_ofdpa_t
 * @param obj An instance of type of_experimenter_multipart_request_ofdpa_t
 *
 * \ingroup of_experimenter_multipart_request_ofdpa
 */
static inline void
of_experimenter_multipart_request_ofdpa_delete(of_experimenter_multipart_request_ofdpa_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_experimenter_ofdpa_t
 * @param obj An instance of type of_experimenter_ofdpa_t
 *
 * \ingroup of_experimenter_ofdpa
 */
static inline void
of_experimenter_ofdpa_delete(of_experimenter_ofdpa_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_experimenter_stats_reply_t
 * @param obj An instance of type of_experimenter_stats_reply_t
 *
 * \ingroup of_experimenter_stats_reply
 */
static inline void
of_experimenter_stats_reply_delete(of_experimenter_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_experimenter_stats_request_t
 * @param obj An instance of type of_experimenter_stats_request_t
 *
 * \ingroup of_experimenter_stats_request
 */
static inline void
of_experimenter_stats_request_delete(of_experimenter_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_features_reply_t
 * @param obj An instance of type of_features_reply_t
 *
 * \ingroup of_features_reply
 */
static inline void
of_features_reply_delete(of_features_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_features_request_t
 * @param obj An instance of type of_features_request_t
 *
 * \ingroup of_features_request
 */
static inline void
of_features_request_delete(of_features_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_add_t
 * @param obj An instance of type of_flow_add_t
 *
 * \ingroup of_flow_add
 */
static inline void
of_flow_add_delete(of_flow_add_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_delete_t
 * @param obj An instance of type of_flow_delete_t
 *
 * \ingroup of_flow_delete
 */
static inline void
of_flow_delete_delete(of_flow_delete_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_delete_strict_t
 * @param obj An instance of type of_flow_delete_strict_t
 *
 * \ingroup of_flow_delete_strict
 */
static inline void
of_flow_delete_strict_delete(of_flow_delete_strict_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_mod_t
 * @param obj An instance of type of_flow_mod_t
 *
 * \ingroup of_flow_mod
 */
static inline void
of_flow_mod_delete(of_flow_mod_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_mod_failed_error_msg_t
 * @param obj An instance of type of_flow_mod_failed_error_msg_t
 *
 * \ingroup of_flow_mod_failed_error_msg
 */
static inline void
of_flow_mod_failed_error_msg_delete(of_flow_mod_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_modify_t
 * @param obj An instance of type of_flow_modify_t
 *
 * \ingroup of_flow_modify
 */
static inline void
of_flow_modify_delete(of_flow_modify_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_modify_strict_t
 * @param obj An instance of type of_flow_modify_strict_t
 *
 * \ingroup of_flow_modify_strict
 */
static inline void
of_flow_modify_strict_delete(of_flow_modify_strict_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_removed_t
 * @param obj An instance of type of_flow_removed_t
 *
 * \ingroup of_flow_removed
 */
static inline void
of_flow_removed_delete(of_flow_removed_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_stats_reply_t
 * @param obj An instance of type of_flow_stats_reply_t
 *
 * \ingroup of_flow_stats_reply
 */
static inline void
of_flow_stats_reply_delete(of_flow_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_stats_request_t
 * @param obj An instance of type of_flow_stats_request_t
 *
 * \ingroup of_flow_stats_request
 */
static inline void
of_flow_stats_request_delete(of_flow_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_get_config_reply_t
 * @param obj An instance of type of_get_config_reply_t
 *
 * \ingroup of_get_config_reply
 */
static inline void
of_get_config_reply_delete(of_get_config_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_get_config_request_t
 * @param obj An instance of type of_get_config_request_t
 *
 * \ingroup of_get_config_request
 */
static inline void
of_get_config_request_delete(of_get_config_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_add_t
 * @param obj An instance of type of_group_add_t
 *
 * \ingroup of_group_add
 */
static inline void
of_group_add_delete(of_group_add_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_delete_t
 * @param obj An instance of type of_group_delete_t
 *
 * \ingroup of_group_delete
 */
static inline void
of_group_delete_delete(of_group_delete_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_desc_stats_reply_t
 * @param obj An instance of type of_group_desc_stats_reply_t
 *
 * \ingroup of_group_desc_stats_reply
 */
static inline void
of_group_desc_stats_reply_delete(of_group_desc_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_desc_stats_request_t
 * @param obj An instance of type of_group_desc_stats_request_t
 *
 * \ingroup of_group_desc_stats_request
 */
static inline void
of_group_desc_stats_request_delete(of_group_desc_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_features_stats_reply_t
 * @param obj An instance of type of_group_features_stats_reply_t
 *
 * \ingroup of_group_features_stats_reply
 */
static inline void
of_group_features_stats_reply_delete(of_group_features_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_features_stats_request_t
 * @param obj An instance of type of_group_features_stats_request_t
 *
 * \ingroup of_group_features_stats_request
 */
static inline void
of_group_features_stats_request_delete(of_group_features_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_mod_t
 * @param obj An instance of type of_group_mod_t
 *
 * \ingroup of_group_mod
 */
static inline void
of_group_mod_delete(of_group_mod_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_mod_failed_error_msg_t
 * @param obj An instance of type of_group_mod_failed_error_msg_t
 *
 * \ingroup of_group_mod_failed_error_msg
 */
static inline void
of_group_mod_failed_error_msg_delete(of_group_mod_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_modify_t
 * @param obj An instance of type of_group_modify_t
 *
 * \ingroup of_group_modify
 */
static inline void
of_group_modify_delete(of_group_modify_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_stats_reply_t
 * @param obj An instance of type of_group_stats_reply_t
 *
 * \ingroup of_group_stats_reply
 */
static inline void
of_group_stats_reply_delete(of_group_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_stats_request_t
 * @param obj An instance of type of_group_stats_request_t
 *
 * \ingroup of_group_stats_request
 */
static inline void
of_group_stats_request_delete(of_group_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_hello_t
 * @param obj An instance of type of_hello_t
 *
 * \ingroup of_hello
 */
static inline void
of_hello_delete(of_hello_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_hello_failed_error_msg_t
 * @param obj An instance of type of_hello_failed_error_msg_t
 *
 * \ingroup of_hello_failed_error_msg
 */
static inline void
of_hello_failed_error_msg_delete(of_hello_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_add_t
 * @param obj An instance of type of_meter_add_t
 *
 * \ingroup of_meter_add
 */
static inline void
of_meter_add_delete(of_meter_add_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_config_stats_reply_t
 * @param obj An instance of type of_meter_config_stats_reply_t
 *
 * \ingroup of_meter_config_stats_reply
 */
static inline void
of_meter_config_stats_reply_delete(of_meter_config_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_config_stats_request_t
 * @param obj An instance of type of_meter_config_stats_request_t
 *
 * \ingroup of_meter_config_stats_request
 */
static inline void
of_meter_config_stats_request_delete(of_meter_config_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_delete_t
 * @param obj An instance of type of_meter_delete_t
 *
 * \ingroup of_meter_delete
 */
static inline void
of_meter_delete_delete(of_meter_delete_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_features_stats_reply_t
 * @param obj An instance of type of_meter_features_stats_reply_t
 *
 * \ingroup of_meter_features_stats_reply
 */
static inline void
of_meter_features_stats_reply_delete(of_meter_features_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_features_stats_request_t
 * @param obj An instance of type of_meter_features_stats_request_t
 *
 * \ingroup of_meter_features_stats_request
 */
static inline void
of_meter_features_stats_request_delete(of_meter_features_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_mod_t
 * @param obj An instance of type of_meter_mod_t
 *
 * \ingroup of_meter_mod
 */
static inline void
of_meter_mod_delete(of_meter_mod_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_mod_failed_error_msg_t
 * @param obj An instance of type of_meter_mod_failed_error_msg_t
 *
 * \ingroup of_meter_mod_failed_error_msg
 */
static inline void
of_meter_mod_failed_error_msg_delete(of_meter_mod_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_modify_t
 * @param obj An instance of type of_meter_modify_t
 *
 * \ingroup of_meter_modify
 */
static inline void
of_meter_modify_delete(of_meter_modify_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_stats_reply_t
 * @param obj An instance of type of_meter_stats_reply_t
 *
 * \ingroup of_meter_stats_reply
 */
static inline void
of_meter_stats_reply_delete(of_meter_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_stats_request_t
 * @param obj An instance of type of_meter_stats_request_t
 *
 * \ingroup of_meter_stats_request
 */
static inline void
of_meter_stats_request_delete(of_meter_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_nicira_controller_role_reply_t
 * @param obj An instance of type of_nicira_controller_role_reply_t
 *
 * \ingroup of_nicira_controller_role_reply
 */
static inline void
of_nicira_controller_role_reply_delete(of_nicira_controller_role_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_nicira_controller_role_request_t
 * @param obj An instance of type of_nicira_controller_role_request_t
 *
 * \ingroup of_nicira_controller_role_request
 */
static inline void
of_nicira_controller_role_request_delete(of_nicira_controller_role_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_nicira_header_t
 * @param obj An instance of type of_nicira_header_t
 *
 * \ingroup of_nicira_header
 */
static inline void
of_nicira_header_delete(of_nicira_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_class_based_ctr_mod_msg_t
 * @param obj An instance of type of_ofdpa_class_based_ctr_mod_msg_t
 *
 * \ingroup of_ofdpa_class_based_ctr_mod_msg
 */
static inline void
of_ofdpa_class_based_ctr_mod_msg_delete(of_ofdpa_class_based_ctr_mod_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_class_based_ctr_multipart_reply_t
 * @param obj An instance of type of_ofdpa_class_based_ctr_multipart_reply_t
 *
 * \ingroup of_ofdpa_class_based_ctr_multipart_reply
 */
static inline void
of_ofdpa_class_based_ctr_multipart_reply_delete(of_ofdpa_class_based_ctr_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_class_based_ctr_multipart_request_t
 * @param obj An instance of type of_ofdpa_class_based_ctr_multipart_request_t
 *
 * \ingroup of_ofdpa_class_based_ctr_multipart_request
 */
static inline void
of_ofdpa_class_based_ctr_multipart_request_delete(of_ofdpa_class_based_ctr_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_color_based_ctr_mod_msg_t
 * @param obj An instance of type of_ofdpa_color_based_ctr_mod_msg_t
 *
 * \ingroup of_ofdpa_color_based_ctr_mod_msg
 */
static inline void
of_ofdpa_color_based_ctr_mod_msg_delete(of_ofdpa_color_based_ctr_mod_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_color_based_ctr_multipart_reply_t
 * @param obj An instance of type of_ofdpa_color_based_ctr_multipart_reply_t
 *
 * \ingroup of_ofdpa_color_based_ctr_multipart_reply
 */
static inline void
of_ofdpa_color_based_ctr_multipart_reply_delete(of_ofdpa_color_based_ctr_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_color_based_ctr_multipart_request_t
 * @param obj An instance of type of_ofdpa_color_based_ctr_multipart_request_t
 *
 * \ingroup of_ofdpa_color_based_ctr_multipart_request
 */
static inline void
of_ofdpa_color_based_ctr_multipart_request_delete(of_ofdpa_color_based_ctr_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_l2_interface_remark_action_mod_msg_t
 * @param obj An instance of type of_ofdpa_l2_interface_remark_action_mod_msg_t
 *
 * \ingroup of_ofdpa_l2_interface_remark_action_mod_msg
 */
static inline void
of_ofdpa_l2_interface_remark_action_mod_msg_delete(of_ofdpa_l2_interface_remark_action_mod_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_l2_interface_remark_action_multipart_reply_t
 * @param obj An instance of type of_ofdpa_l2_interface_remark_action_multipart_reply_t
 *
 * \ingroup of_ofdpa_l2_interface_remark_action_multipart_reply
 */
static inline void
of_ofdpa_l2_interface_remark_action_multipart_reply_delete(of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_l2_interface_remark_action_multipart_request_t
 * @param obj An instance of type of_ofdpa_l2_interface_remark_action_multipart_request_t
 *
 * \ingroup of_ofdpa_l2_interface_remark_action_multipart_request
 */
static inline void
of_ofdpa_l2_interface_remark_action_multipart_request_delete(of_ofdpa_l2_interface_remark_action_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_mpls_tunnel_label_multipart_reply_t
 * @param obj An instance of type of_ofdpa_mpls_tunnel_label_multipart_reply_t
 *
 * \ingroup of_ofdpa_mpls_tunnel_label_multipart_reply
 */
static inline void
of_ofdpa_mpls_tunnel_label_multipart_reply_delete(of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_mpls_tunnel_label_multipart_request_t
 * @param obj An instance of type of_ofdpa_mpls_tunnel_label_multipart_request_t
 *
 * \ingroup of_ofdpa_mpls_tunnel_label_multipart_request
 */
static inline void
of_ofdpa_mpls_tunnel_label_multipart_request_delete(of_ofdpa_mpls_tunnel_label_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t
 * @param obj An instance of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t
 *
 * \ingroup of_ofdpa_mpls_tunnel_label_remark_action_mod_msg
 */
static inline void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_delete(of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t
 * @param obj An instance of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t
 *
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_mod_msg
 */
static inline void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_delete(of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t
 * @param obj An instance of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t
 *
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_multipart_reply
 */
static inline void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_delete(of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t
 * @param obj An instance of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t
 *
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_multipart_request
 */
static inline void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_delete(of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_oam_dataplane_ctr_mod_msg_t
 * @param obj An instance of type of_ofdpa_oam_dataplane_ctr_mod_msg_t
 *
 * \ingroup of_ofdpa_oam_dataplane_ctr_mod_msg
 */
static inline void
of_ofdpa_oam_dataplane_ctr_mod_msg_delete(of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply_t
 * @param obj An instance of type of_ofdpa_oam_dataplane_ctr_multipart_reply_t
 *
 * \ingroup of_ofdpa_oam_dataplane_ctr_multipart_reply
 */
static inline void
of_ofdpa_oam_dataplane_ctr_multipart_reply_delete(of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_oam_dataplane_ctr_multipart_request_t
 * @param obj An instance of type of_ofdpa_oam_dataplane_ctr_multipart_request_t
 *
 * \ingroup of_ofdpa_oam_dataplane_ctr_multipart_request
 */
static inline void
of_ofdpa_oam_dataplane_ctr_multipart_request_delete(of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_oam_drop_status_mod_msg_t
 * @param obj An instance of type of_ofdpa_oam_drop_status_mod_msg_t
 *
 * \ingroup of_ofdpa_oam_drop_status_mod_msg
 */
static inline void
of_ofdpa_oam_drop_status_mod_msg_delete(of_ofdpa_oam_drop_status_mod_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_oam_drop_status_multipart_reply_t
 * @param obj An instance of type of_ofdpa_oam_drop_status_multipart_reply_t
 *
 * \ingroup of_ofdpa_oam_drop_status_multipart_reply
 */
static inline void
of_ofdpa_oam_drop_status_multipart_reply_delete(of_ofdpa_oam_drop_status_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_oam_drop_status_multipart_request_t
 * @param obj An instance of type of_ofdpa_oam_drop_status_multipart_request_t
 *
 * \ingroup of_ofdpa_oam_drop_status_multipart_request
 */
static inline void
of_ofdpa_oam_drop_status_multipart_request_delete(of_ofdpa_oam_drop_status_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_port_experimenter_prop_scheduling_t
 * @param obj An instance of type of_ofdpa_port_experimenter_prop_scheduling_t
 *
 * \ingroup of_ofdpa_port_experimenter_prop_scheduling
 */
static inline void
of_ofdpa_port_experimenter_prop_scheduling_delete(of_ofdpa_port_experimenter_prop_scheduling_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply_t
 * @param obj An instance of type of_ofdpa_queue_desc_prop_congestion_multipart_reply_t
 *
 * \ingroup of_ofdpa_queue_desc_prop_congestion_multipart_reply
 */
static inline void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_delete(of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request_t
 * @param obj An instance of type of_ofdpa_queue_desc_prop_congestion_multipart_request_t
 *
 * \ingroup of_ofdpa_queue_desc_prop_congestion_multipart_request
 */
static inline void
of_ofdpa_queue_desc_prop_congestion_multipart_request_delete(of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply_t
 * @param obj An instance of type of_ofdpa_queue_desc_prop_parent_multipart_reply_t
 *
 * \ingroup of_ofdpa_queue_desc_prop_parent_multipart_reply
 */
static inline void
of_ofdpa_queue_desc_prop_parent_multipart_reply_delete(of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_queue_desc_prop_parent_multipart_request_t
 * @param obj An instance of type of_ofdpa_queue_desc_prop_parent_multipart_request_t
 *
 * \ingroup of_ofdpa_queue_desc_prop_parent_multipart_request
 */
static inline void
of_ofdpa_queue_desc_prop_parent_multipart_request_delete(of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_queue_mod_t
 * @param obj An instance of type of_ofdpa_queue_mod_t
 *
 * \ingroup of_ofdpa_queue_mod
 */
static inline void
of_ofdpa_queue_mod_delete(of_ofdpa_queue_mod_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_scheduler_node_mod_msg_t
 * @param obj An instance of type of_ofdpa_scheduler_node_mod_msg_t
 *
 * \ingroup of_ofdpa_scheduler_node_mod_msg
 */
static inline void
of_ofdpa_scheduler_node_mod_msg_delete(of_ofdpa_scheduler_node_mod_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_scheduler_node_multipart_request_t
 * @param obj An instance of type of_ofdpa_scheduler_node_multipart_request_t
 *
 * \ingroup of_ofdpa_scheduler_node_multipart_request
 */
static inline void
of_ofdpa_scheduler_node_multipart_request_delete(of_ofdpa_scheduler_node_multipart_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_ofdpa_scheduler_node_multipart_response_t
 * @param obj An instance of type of_ofdpa_scheduler_node_multipart_response_t
 *
 * \ingroup of_ofdpa_scheduler_node_multipart_response
 */
static inline void
of_ofdpa_scheduler_node_multipart_response_delete(of_ofdpa_scheduler_node_multipart_response_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_packet_in_t
 * @param obj An instance of type of_packet_in_t
 *
 * \ingroup of_packet_in
 */
static inline void
of_packet_in_delete(of_packet_in_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_packet_out_t
 * @param obj An instance of type of_packet_out_t
 *
 * \ingroup of_packet_out
 */
static inline void
of_packet_out_delete(of_packet_out_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_desc_stats_reply_t
 * @param obj An instance of type of_port_desc_stats_reply_t
 *
 * \ingroup of_port_desc_stats_reply
 */
static inline void
of_port_desc_stats_reply_delete(of_port_desc_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_desc_stats_request_t
 * @param obj An instance of type of_port_desc_stats_request_t
 *
 * \ingroup of_port_desc_stats_request
 */
static inline void
of_port_desc_stats_request_delete(of_port_desc_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_mod_t
 * @param obj An instance of type of_port_mod_t
 *
 * \ingroup of_port_mod
 */
static inline void
of_port_mod_delete(of_port_mod_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_mod_failed_error_msg_t
 * @param obj An instance of type of_port_mod_failed_error_msg_t
 *
 * \ingroup of_port_mod_failed_error_msg
 */
static inline void
of_port_mod_failed_error_msg_delete(of_port_mod_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_stats_reply_t
 * @param obj An instance of type of_port_stats_reply_t
 *
 * \ingroup of_port_stats_reply
 */
static inline void
of_port_stats_reply_delete(of_port_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_stats_request_t
 * @param obj An instance of type of_port_stats_request_t
 *
 * \ingroup of_port_stats_request
 */
static inline void
of_port_stats_request_delete(of_port_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_status_t
 * @param obj An instance of type of_port_status_t
 *
 * \ingroup of_port_status
 */
static inline void
of_port_status_delete(of_port_status_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_get_config_reply_t
 * @param obj An instance of type of_queue_get_config_reply_t
 *
 * \ingroup of_queue_get_config_reply
 */
static inline void
of_queue_get_config_reply_delete(of_queue_get_config_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_get_config_request_t
 * @param obj An instance of type of_queue_get_config_request_t
 *
 * \ingroup of_queue_get_config_request
 */
static inline void
of_queue_get_config_request_delete(of_queue_get_config_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_op_failed_error_msg_t
 * @param obj An instance of type of_queue_op_failed_error_msg_t
 *
 * \ingroup of_queue_op_failed_error_msg
 */
static inline void
of_queue_op_failed_error_msg_delete(of_queue_op_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_stats_reply_t
 * @param obj An instance of type of_queue_stats_reply_t
 *
 * \ingroup of_queue_stats_reply
 */
static inline void
of_queue_stats_reply_delete(of_queue_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_stats_request_t
 * @param obj An instance of type of_queue_stats_request_t
 *
 * \ingroup of_queue_stats_request
 */
static inline void
of_queue_stats_request_delete(of_queue_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_role_reply_t
 * @param obj An instance of type of_role_reply_t
 *
 * \ingroup of_role_reply
 */
static inline void
of_role_reply_delete(of_role_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_role_request_t
 * @param obj An instance of type of_role_request_t
 *
 * \ingroup of_role_request
 */
static inline void
of_role_request_delete(of_role_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_role_request_failed_error_msg_t
 * @param obj An instance of type of_role_request_failed_error_msg_t
 *
 * \ingroup of_role_request_failed_error_msg
 */
static inline void
of_role_request_failed_error_msg_delete(of_role_request_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_set_config_t
 * @param obj An instance of type of_set_config_t
 *
 * \ingroup of_set_config
 */
static inline void
of_set_config_delete(of_set_config_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_stats_reply_t
 * @param obj An instance of type of_stats_reply_t
 *
 * \ingroup of_stats_reply
 */
static inline void
of_stats_reply_delete(of_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_stats_request_t
 * @param obj An instance of type of_stats_request_t
 *
 * \ingroup of_stats_request
 */
static inline void
of_stats_request_delete(of_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_switch_config_failed_error_msg_t
 * @param obj An instance of type of_switch_config_failed_error_msg_t
 *
 * \ingroup of_switch_config_failed_error_msg
 */
static inline void
of_switch_config_failed_error_msg_delete(of_switch_config_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_features_failed_error_msg_t
 * @param obj An instance of type of_table_features_failed_error_msg_t
 *
 * \ingroup of_table_features_failed_error_msg
 */
static inline void
of_table_features_failed_error_msg_delete(of_table_features_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_features_stats_reply_t
 * @param obj An instance of type of_table_features_stats_reply_t
 *
 * \ingroup of_table_features_stats_reply
 */
static inline void
of_table_features_stats_reply_delete(of_table_features_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_features_stats_request_t
 * @param obj An instance of type of_table_features_stats_request_t
 *
 * \ingroup of_table_features_stats_request
 */
static inline void
of_table_features_stats_request_delete(of_table_features_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_mod_t
 * @param obj An instance of type of_table_mod_t
 *
 * \ingroup of_table_mod
 */
static inline void
of_table_mod_delete(of_table_mod_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_mod_failed_error_msg_t
 * @param obj An instance of type of_table_mod_failed_error_msg_t
 *
 * \ingroup of_table_mod_failed_error_msg
 */
static inline void
of_table_mod_failed_error_msg_delete(of_table_mod_failed_error_msg_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_stats_reply_t
 * @param obj An instance of type of_table_stats_reply_t
 *
 * \ingroup of_table_stats_reply
 */
static inline void
of_table_stats_reply_delete(of_table_stats_reply_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_stats_request_t
 * @param obj An instance of type of_table_stats_request_t
 *
 * \ingroup of_table_stats_request
 */
static inline void
of_table_stats_request_delete(of_table_stats_request_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_t
 * @param obj An instance of type of_action_t
 *
 * \ingroup of_action
 */
static inline void
of_action_delete(of_action_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_bsn_t
 * @param obj An instance of type of_action_bsn_t
 *
 * \ingroup of_action_bsn
 */
static inline void
of_action_bsn_delete(of_action_bsn_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_bsn_mirror_t
 * @param obj An instance of type of_action_bsn_mirror_t
 *
 * \ingroup of_action_bsn_mirror
 */
static inline void
of_action_bsn_mirror_delete(of_action_bsn_mirror_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_bsn_set_tunnel_dst_t
 * @param obj An instance of type of_action_bsn_set_tunnel_dst_t
 *
 * \ingroup of_action_bsn_set_tunnel_dst
 */
static inline void
of_action_bsn_set_tunnel_dst_delete(of_action_bsn_set_tunnel_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_copy_ttl_in_t
 * @param obj An instance of type of_action_copy_ttl_in_t
 *
 * \ingroup of_action_copy_ttl_in
 */
static inline void
of_action_copy_ttl_in_delete(of_action_copy_ttl_in_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_copy_ttl_out_t
 * @param obj An instance of type of_action_copy_ttl_out_t
 *
 * \ingroup of_action_copy_ttl_out
 */
static inline void
of_action_copy_ttl_out_delete(of_action_copy_ttl_out_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_dec_mpls_ttl_t
 * @param obj An instance of type of_action_dec_mpls_ttl_t
 *
 * \ingroup of_action_dec_mpls_ttl
 */
static inline void
of_action_dec_mpls_ttl_delete(of_action_dec_mpls_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_dec_nw_ttl_t
 * @param obj An instance of type of_action_dec_nw_ttl_t
 *
 * \ingroup of_action_dec_nw_ttl
 */
static inline void
of_action_dec_nw_ttl_delete(of_action_dec_nw_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_enqueue_t
 * @param obj An instance of type of_action_enqueue_t
 *
 * \ingroup of_action_enqueue
 */
static inline void
of_action_enqueue_delete(of_action_enqueue_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_experimenter_t
 * @param obj An instance of type of_action_experimenter_t
 *
 * \ingroup of_action_experimenter
 */
static inline void
of_action_experimenter_delete(of_action_experimenter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_group_t
 * @param obj An instance of type of_action_group_t
 *
 * \ingroup of_action_group
 */
static inline void
of_action_group_delete(of_action_group_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_header_t
 * @param obj An instance of type of_action_header_t
 *
 * \ingroup of_action_header
 */
static inline void
of_action_header_delete(of_action_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_t
 * @param obj An instance of type of_action_id_t
 *
 * \ingroup of_action_id
 */
static inline void
of_action_id_delete(of_action_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_bsn_t
 * @param obj An instance of type of_action_id_bsn_t
 *
 * \ingroup of_action_id_bsn
 */
static inline void
of_action_id_bsn_delete(of_action_id_bsn_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_bsn_mirror_t
 * @param obj An instance of type of_action_id_bsn_mirror_t
 *
 * \ingroup of_action_id_bsn_mirror
 */
static inline void
of_action_id_bsn_mirror_delete(of_action_id_bsn_mirror_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_bsn_set_tunnel_dst_t
 * @param obj An instance of type of_action_id_bsn_set_tunnel_dst_t
 *
 * \ingroup of_action_id_bsn_set_tunnel_dst
 */
static inline void
of_action_id_bsn_set_tunnel_dst_delete(of_action_id_bsn_set_tunnel_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_copy_ttl_in_t
 * @param obj An instance of type of_action_id_copy_ttl_in_t
 *
 * \ingroup of_action_id_copy_ttl_in
 */
static inline void
of_action_id_copy_ttl_in_delete(of_action_id_copy_ttl_in_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_copy_ttl_out_t
 * @param obj An instance of type of_action_id_copy_ttl_out_t
 *
 * \ingroup of_action_id_copy_ttl_out
 */
static inline void
of_action_id_copy_ttl_out_delete(of_action_id_copy_ttl_out_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_dec_mpls_ttl_t
 * @param obj An instance of type of_action_id_dec_mpls_ttl_t
 *
 * \ingroup of_action_id_dec_mpls_ttl
 */
static inline void
of_action_id_dec_mpls_ttl_delete(of_action_id_dec_mpls_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_dec_nw_ttl_t
 * @param obj An instance of type of_action_id_dec_nw_ttl_t
 *
 * \ingroup of_action_id_dec_nw_ttl
 */
static inline void
of_action_id_dec_nw_ttl_delete(of_action_id_dec_nw_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_experimenter_t
 * @param obj An instance of type of_action_id_experimenter_t
 *
 * \ingroup of_action_id_experimenter
 */
static inline void
of_action_id_experimenter_delete(of_action_id_experimenter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_group_t
 * @param obj An instance of type of_action_id_group_t
 *
 * \ingroup of_action_id_group
 */
static inline void
of_action_id_group_delete(of_action_id_group_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_header_t
 * @param obj An instance of type of_action_id_header_t
 *
 * \ingroup of_action_id_header
 */
static inline void
of_action_id_header_delete(of_action_id_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_nicira_t
 * @param obj An instance of type of_action_id_nicira_t
 *
 * \ingroup of_action_id_nicira
 */
static inline void
of_action_id_nicira_delete(of_action_id_nicira_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_nicira_dec_ttl_t
 * @param obj An instance of type of_action_id_nicira_dec_ttl_t
 *
 * \ingroup of_action_id_nicira_dec_ttl
 */
static inline void
of_action_id_nicira_dec_ttl_delete(of_action_id_nicira_dec_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_t
 * @param obj An instance of type of_action_id_ofdpa_t
 *
 * \ingroup of_action_id_ofdpa
 */
static inline void
of_action_id_ofdpa_delete(of_action_id_ofdpa_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_check_drop_status_t
 * @param obj An instance of type of_action_id_ofdpa_check_drop_status_t
 *
 * \ingroup of_action_id_ofdpa_check_drop_status
 */
static inline void
of_action_id_ofdpa_check_drop_status_delete(of_action_id_ofdpa_check_drop_status_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_class_based_count_t
 * @param obj An instance of type of_action_id_ofdpa_class_based_count_t
 *
 * \ingroup of_action_id_ofdpa_class_based_count
 */
static inline void
of_action_id_ofdpa_class_based_count_delete(of_action_id_ofdpa_class_based_count_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_color_based_count_t
 * @param obj An instance of type of_action_id_ofdpa_color_based_count_t
 *
 * \ingroup of_action_id_ofdpa_color_based_count
 */
static inline void
of_action_id_ofdpa_color_based_count_delete(of_action_id_ofdpa_color_based_count_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_color_based_count_1_t
 * @param obj An instance of type of_action_id_ofdpa_color_based_count_1_t
 *
 * \ingroup of_action_id_ofdpa_color_based_count_1
 */
static inline void
of_action_id_ofdpa_color_based_count_1_delete(of_action_id_ofdpa_color_based_count_1_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_color_based_count_2_t
 * @param obj An instance of type of_action_id_ofdpa_color_based_count_2_t
 *
 * \ingroup of_action_id_ofdpa_color_based_count_2
 */
static inline void
of_action_id_ofdpa_color_based_count_2_delete(of_action_id_ofdpa_color_based_count_2_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_color_based_count_3_t
 * @param obj An instance of type of_action_id_ofdpa_color_based_count_3_t
 *
 * \ingroup of_action_id_ofdpa_color_based_count_3
 */
static inline void
of_action_id_ofdpa_color_based_count_3_delete(of_action_id_ofdpa_color_based_count_3_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_copy_tc_in_t
 * @param obj An instance of type of_action_id_ofdpa_copy_tc_in_t
 *
 * \ingroup of_action_id_ofdpa_copy_tc_in
 */
static inline void
of_action_id_ofdpa_copy_tc_in_delete(of_action_id_ofdpa_copy_tc_in_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_copy_tc_out_t
 * @param obj An instance of type of_action_id_ofdpa_copy_tc_out_t
 *
 * \ingroup of_action_id_ofdpa_copy_tc_out
 */
static inline void
of_action_id_ofdpa_copy_tc_out_delete(of_action_id_ofdpa_copy_tc_out_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_dec_ttl_mtu_t
 * @param obj An instance of type of_action_id_ofdpa_dec_ttl_mtu_t
 *
 * \ingroup of_action_id_ofdpa_dec_ttl_mtu
 */
static inline void
of_action_id_ofdpa_dec_ttl_mtu_delete(of_action_id_ofdpa_dec_ttl_mtu_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_oam_lm_rx_count_t
 * @param obj An instance of type of_action_id_ofdpa_oam_lm_rx_count_t
 *
 * \ingroup of_action_id_ofdpa_oam_lm_rx_count
 */
static inline void
of_action_id_ofdpa_oam_lm_rx_count_delete(of_action_id_ofdpa_oam_lm_rx_count_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_oam_lm_tx_count_t
 * @param obj An instance of type of_action_id_ofdpa_oam_lm_tx_count_t
 *
 * \ingroup of_action_id_ofdpa_oam_lm_tx_count
 */
static inline void
of_action_id_ofdpa_oam_lm_tx_count_delete(of_action_id_ofdpa_oam_lm_tx_count_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_pop_cw_t
 * @param obj An instance of type of_action_id_ofdpa_pop_cw_t
 *
 * \ingroup of_action_id_ofdpa_pop_cw
 */
static inline void
of_action_id_ofdpa_pop_cw_delete(of_action_id_ofdpa_pop_cw_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_pop_l2hdr_t
 * @param obj An instance of type of_action_id_ofdpa_pop_l2hdr_t
 *
 * \ingroup of_action_id_ofdpa_pop_l2hdr
 */
static inline void
of_action_id_ofdpa_pop_l2hdr_delete(of_action_id_ofdpa_pop_l2hdr_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_push_cw_t
 * @param obj An instance of type of_action_id_ofdpa_push_cw_t
 *
 * \ingroup of_action_id_ofdpa_push_cw
 */
static inline void
of_action_id_ofdpa_push_cw_delete(of_action_id_ofdpa_push_cw_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_push_l2hdr_t
 * @param obj An instance of type of_action_id_ofdpa_push_l2hdr_t
 *
 * \ingroup of_action_id_ofdpa_push_l2hdr
 */
static inline void
of_action_id_ofdpa_push_l2hdr_delete(of_action_id_ofdpa_push_l2hdr_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_set_counter_fields_t
 * @param obj An instance of type of_action_id_ofdpa_set_counter_fields_t
 *
 * \ingroup of_action_id_ofdpa_set_counter_fields
 */
static inline void
of_action_id_ofdpa_set_counter_fields_delete(of_action_id_ofdpa_set_counter_fields_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table_t
 * @param obj An instance of type of_action_id_ofdpa_set_mpls_pcpdei_from_table_t
 *
 * \ingroup of_action_id_ofdpa_set_mpls_pcpdei_from_table
 */
static inline void
of_action_id_ofdpa_set_mpls_pcpdei_from_table_delete(of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t
 * @param obj An instance of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t
 *
 * \ingroup of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table
 */
static inline void
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_delete(of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t
 * @param obj An instance of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t
 *
 * \ingroup of_action_id_ofdpa_set_mpls_tc_from_tunnel_table
 */
static inline void
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_delete(of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t
 * @param obj An instance of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t
 *
 * \ingroup of_action_id_ofdpa_set_mpls_tc_from_vpn_table
 */
static inline void
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_delete(of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_ofdpa_set_pcpdei_from_table_t
 * @param obj An instance of type of_action_id_ofdpa_set_pcpdei_from_table_t
 *
 * \ingroup of_action_id_ofdpa_set_pcpdei_from_table
 */
static inline void
of_action_id_ofdpa_set_pcpdei_from_table_delete(of_action_id_ofdpa_set_pcpdei_from_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_onf_t
 * @param obj An instance of type of_action_id_onf_t
 *
 * \ingroup of_action_id_onf
 */
static inline void
of_action_id_onf_delete(of_action_id_onf_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_onf_copy_field_t
 * @param obj An instance of type of_action_id_onf_copy_field_t
 *
 * \ingroup of_action_id_onf_copy_field
 */
static inline void
of_action_id_onf_copy_field_delete(of_action_id_onf_copy_field_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_output_t
 * @param obj An instance of type of_action_id_output_t
 *
 * \ingroup of_action_id_output
 */
static inline void
of_action_id_output_delete(of_action_id_output_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_pop_mpls_t
 * @param obj An instance of type of_action_id_pop_mpls_t
 *
 * \ingroup of_action_id_pop_mpls
 */
static inline void
of_action_id_pop_mpls_delete(of_action_id_pop_mpls_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_pop_pbb_t
 * @param obj An instance of type of_action_id_pop_pbb_t
 *
 * \ingroup of_action_id_pop_pbb
 */
static inline void
of_action_id_pop_pbb_delete(of_action_id_pop_pbb_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_pop_vlan_t
 * @param obj An instance of type of_action_id_pop_vlan_t
 *
 * \ingroup of_action_id_pop_vlan
 */
static inline void
of_action_id_pop_vlan_delete(of_action_id_pop_vlan_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_push_mpls_t
 * @param obj An instance of type of_action_id_push_mpls_t
 *
 * \ingroup of_action_id_push_mpls
 */
static inline void
of_action_id_push_mpls_delete(of_action_id_push_mpls_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_push_pbb_t
 * @param obj An instance of type of_action_id_push_pbb_t
 *
 * \ingroup of_action_id_push_pbb
 */
static inline void
of_action_id_push_pbb_delete(of_action_id_push_pbb_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_push_vlan_t
 * @param obj An instance of type of_action_id_push_vlan_t
 *
 * \ingroup of_action_id_push_vlan
 */
static inline void
of_action_id_push_vlan_delete(of_action_id_push_vlan_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_set_field_t
 * @param obj An instance of type of_action_id_set_field_t
 *
 * \ingroup of_action_id_set_field
 */
static inline void
of_action_id_set_field_delete(of_action_id_set_field_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_set_mpls_ttl_t
 * @param obj An instance of type of_action_id_set_mpls_ttl_t
 *
 * \ingroup of_action_id_set_mpls_ttl
 */
static inline void
of_action_id_set_mpls_ttl_delete(of_action_id_set_mpls_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_set_nw_ttl_t
 * @param obj An instance of type of_action_id_set_nw_ttl_t
 *
 * \ingroup of_action_id_set_nw_ttl
 */
static inline void
of_action_id_set_nw_ttl_delete(of_action_id_set_nw_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_id_set_queue_t
 * @param obj An instance of type of_action_id_set_queue_t
 *
 * \ingroup of_action_id_set_queue
 */
static inline void
of_action_id_set_queue_delete(of_action_id_set_queue_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_nicira_t
 * @param obj An instance of type of_action_nicira_t
 *
 * \ingroup of_action_nicira
 */
static inline void
of_action_nicira_delete(of_action_nicira_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_nicira_dec_ttl_t
 * @param obj An instance of type of_action_nicira_dec_ttl_t
 *
 * \ingroup of_action_nicira_dec_ttl
 */
static inline void
of_action_nicira_dec_ttl_delete(of_action_nicira_dec_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_t
 * @param obj An instance of type of_action_ofdpa_t
 *
 * \ingroup of_action_ofdpa
 */
static inline void
of_action_ofdpa_delete(of_action_ofdpa_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_check_drop_status_t
 * @param obj An instance of type of_action_ofdpa_check_drop_status_t
 *
 * \ingroup of_action_ofdpa_check_drop_status
 */
static inline void
of_action_ofdpa_check_drop_status_delete(of_action_ofdpa_check_drop_status_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_class_based_count_t
 * @param obj An instance of type of_action_ofdpa_class_based_count_t
 *
 * \ingroup of_action_ofdpa_class_based_count
 */
static inline void
of_action_ofdpa_class_based_count_delete(of_action_ofdpa_class_based_count_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_color_based_count_t
 * @param obj An instance of type of_action_ofdpa_color_based_count_t
 *
 * \ingroup of_action_ofdpa_color_based_count
 */
static inline void
of_action_ofdpa_color_based_count_delete(of_action_ofdpa_color_based_count_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_color_based_count_1_t
 * @param obj An instance of type of_action_ofdpa_color_based_count_1_t
 *
 * \ingroup of_action_ofdpa_color_based_count_1
 */
static inline void
of_action_ofdpa_color_based_count_1_delete(of_action_ofdpa_color_based_count_1_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_color_based_count_2_t
 * @param obj An instance of type of_action_ofdpa_color_based_count_2_t
 *
 * \ingroup of_action_ofdpa_color_based_count_2
 */
static inline void
of_action_ofdpa_color_based_count_2_delete(of_action_ofdpa_color_based_count_2_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_color_based_count_3_t
 * @param obj An instance of type of_action_ofdpa_color_based_count_3_t
 *
 * \ingroup of_action_ofdpa_color_based_count_3
 */
static inline void
of_action_ofdpa_color_based_count_3_delete(of_action_ofdpa_color_based_count_3_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_copy_tc_in_t
 * @param obj An instance of type of_action_ofdpa_copy_tc_in_t
 *
 * \ingroup of_action_ofdpa_copy_tc_in
 */
static inline void
of_action_ofdpa_copy_tc_in_delete(of_action_ofdpa_copy_tc_in_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_copy_tc_out_t
 * @param obj An instance of type of_action_ofdpa_copy_tc_out_t
 *
 * \ingroup of_action_ofdpa_copy_tc_out
 */
static inline void
of_action_ofdpa_copy_tc_out_delete(of_action_ofdpa_copy_tc_out_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_dec_ttl_mtu_t
 * @param obj An instance of type of_action_ofdpa_dec_ttl_mtu_t
 *
 * \ingroup of_action_ofdpa_dec_ttl_mtu
 */
static inline void
of_action_ofdpa_dec_ttl_mtu_delete(of_action_ofdpa_dec_ttl_mtu_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_oam_lm_rx_count_t
 * @param obj An instance of type of_action_ofdpa_oam_lm_rx_count_t
 *
 * \ingroup of_action_ofdpa_oam_lm_rx_count
 */
static inline void
of_action_ofdpa_oam_lm_rx_count_delete(of_action_ofdpa_oam_lm_rx_count_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_oam_lm_tx_count_t
 * @param obj An instance of type of_action_ofdpa_oam_lm_tx_count_t
 *
 * \ingroup of_action_ofdpa_oam_lm_tx_count
 */
static inline void
of_action_ofdpa_oam_lm_tx_count_delete(of_action_ofdpa_oam_lm_tx_count_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_pop_cw_t
 * @param obj An instance of type of_action_ofdpa_pop_cw_t
 *
 * \ingroup of_action_ofdpa_pop_cw
 */
static inline void
of_action_ofdpa_pop_cw_delete(of_action_ofdpa_pop_cw_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_pop_l2hdr_t
 * @param obj An instance of type of_action_ofdpa_pop_l2hdr_t
 *
 * \ingroup of_action_ofdpa_pop_l2hdr
 */
static inline void
of_action_ofdpa_pop_l2hdr_delete(of_action_ofdpa_pop_l2hdr_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_push_cw_t
 * @param obj An instance of type of_action_ofdpa_push_cw_t
 *
 * \ingroup of_action_ofdpa_push_cw
 */
static inline void
of_action_ofdpa_push_cw_delete(of_action_ofdpa_push_cw_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_push_l2hdr_t
 * @param obj An instance of type of_action_ofdpa_push_l2hdr_t
 *
 * \ingroup of_action_ofdpa_push_l2hdr
 */
static inline void
of_action_ofdpa_push_l2hdr_delete(of_action_ofdpa_push_l2hdr_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_set_counter_fields_t
 * @param obj An instance of type of_action_ofdpa_set_counter_fields_t
 *
 * \ingroup of_action_ofdpa_set_counter_fields
 */
static inline void
of_action_ofdpa_set_counter_fields_delete(of_action_ofdpa_set_counter_fields_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_set_mpls_pcpdei_from_table_t
 * @param obj An instance of type of_action_ofdpa_set_mpls_pcpdei_from_table_t
 *
 * \ingroup of_action_ofdpa_set_mpls_pcpdei_from_table
 */
static inline void
of_action_ofdpa_set_mpls_pcpdei_from_table_delete(of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t
 * @param obj An instance of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t
 *
 * \ingroup of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table
 */
static inline void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_delete(of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table_t
 * @param obj An instance of type of_action_ofdpa_set_mpls_tc_from_tunnel_table_t
 *
 * \ingroup of_action_ofdpa_set_mpls_tc_from_tunnel_table
 */
static inline void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_delete(of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table_t
 * @param obj An instance of type of_action_ofdpa_set_mpls_tc_from_vpn_table_t
 *
 * \ingroup of_action_ofdpa_set_mpls_tc_from_vpn_table
 */
static inline void
of_action_ofdpa_set_mpls_tc_from_vpn_table_delete(of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_ofdpa_set_pcpdei_from_table_t
 * @param obj An instance of type of_action_ofdpa_set_pcpdei_from_table_t
 *
 * \ingroup of_action_ofdpa_set_pcpdei_from_table
 */
static inline void
of_action_ofdpa_set_pcpdei_from_table_delete(of_action_ofdpa_set_pcpdei_from_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_onf_t
 * @param obj An instance of type of_action_onf_t
 *
 * \ingroup of_action_onf
 */
static inline void
of_action_onf_delete(of_action_onf_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_onf_copy_field_t
 * @param obj An instance of type of_action_onf_copy_field_t
 *
 * \ingroup of_action_onf_copy_field
 */
static inline void
of_action_onf_copy_field_delete(of_action_onf_copy_field_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_output_t
 * @param obj An instance of type of_action_output_t
 *
 * \ingroup of_action_output
 */
static inline void
of_action_output_delete(of_action_output_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_pop_mpls_t
 * @param obj An instance of type of_action_pop_mpls_t
 *
 * \ingroup of_action_pop_mpls
 */
static inline void
of_action_pop_mpls_delete(of_action_pop_mpls_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_pop_pbb_t
 * @param obj An instance of type of_action_pop_pbb_t
 *
 * \ingroup of_action_pop_pbb
 */
static inline void
of_action_pop_pbb_delete(of_action_pop_pbb_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_pop_vlan_t
 * @param obj An instance of type of_action_pop_vlan_t
 *
 * \ingroup of_action_pop_vlan
 */
static inline void
of_action_pop_vlan_delete(of_action_pop_vlan_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_push_mpls_t
 * @param obj An instance of type of_action_push_mpls_t
 *
 * \ingroup of_action_push_mpls
 */
static inline void
of_action_push_mpls_delete(of_action_push_mpls_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_push_pbb_t
 * @param obj An instance of type of_action_push_pbb_t
 *
 * \ingroup of_action_push_pbb
 */
static inline void
of_action_push_pbb_delete(of_action_push_pbb_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_push_vlan_t
 * @param obj An instance of type of_action_push_vlan_t
 *
 * \ingroup of_action_push_vlan
 */
static inline void
of_action_push_vlan_delete(of_action_push_vlan_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_dl_dst_t
 * @param obj An instance of type of_action_set_dl_dst_t
 *
 * \ingroup of_action_set_dl_dst
 */
static inline void
of_action_set_dl_dst_delete(of_action_set_dl_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_dl_src_t
 * @param obj An instance of type of_action_set_dl_src_t
 *
 * \ingroup of_action_set_dl_src
 */
static inline void
of_action_set_dl_src_delete(of_action_set_dl_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_field_t
 * @param obj An instance of type of_action_set_field_t
 *
 * \ingroup of_action_set_field
 */
static inline void
of_action_set_field_delete(of_action_set_field_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_mpls_label_t
 * @param obj An instance of type of_action_set_mpls_label_t
 *
 * \ingroup of_action_set_mpls_label
 */
static inline void
of_action_set_mpls_label_delete(of_action_set_mpls_label_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_mpls_tc_t
 * @param obj An instance of type of_action_set_mpls_tc_t
 *
 * \ingroup of_action_set_mpls_tc
 */
static inline void
of_action_set_mpls_tc_delete(of_action_set_mpls_tc_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_mpls_ttl_t
 * @param obj An instance of type of_action_set_mpls_ttl_t
 *
 * \ingroup of_action_set_mpls_ttl
 */
static inline void
of_action_set_mpls_ttl_delete(of_action_set_mpls_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_nw_dst_t
 * @param obj An instance of type of_action_set_nw_dst_t
 *
 * \ingroup of_action_set_nw_dst
 */
static inline void
of_action_set_nw_dst_delete(of_action_set_nw_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_nw_ecn_t
 * @param obj An instance of type of_action_set_nw_ecn_t
 *
 * \ingroup of_action_set_nw_ecn
 */
static inline void
of_action_set_nw_ecn_delete(of_action_set_nw_ecn_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_nw_src_t
 * @param obj An instance of type of_action_set_nw_src_t
 *
 * \ingroup of_action_set_nw_src
 */
static inline void
of_action_set_nw_src_delete(of_action_set_nw_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_nw_tos_t
 * @param obj An instance of type of_action_set_nw_tos_t
 *
 * \ingroup of_action_set_nw_tos
 */
static inline void
of_action_set_nw_tos_delete(of_action_set_nw_tos_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_nw_ttl_t
 * @param obj An instance of type of_action_set_nw_ttl_t
 *
 * \ingroup of_action_set_nw_ttl
 */
static inline void
of_action_set_nw_ttl_delete(of_action_set_nw_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_queue_t
 * @param obj An instance of type of_action_set_queue_t
 *
 * \ingroup of_action_set_queue
 */
static inline void
of_action_set_queue_delete(of_action_set_queue_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_tp_dst_t
 * @param obj An instance of type of_action_set_tp_dst_t
 *
 * \ingroup of_action_set_tp_dst
 */
static inline void
of_action_set_tp_dst_delete(of_action_set_tp_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_tp_src_t
 * @param obj An instance of type of_action_set_tp_src_t
 *
 * \ingroup of_action_set_tp_src
 */
static inline void
of_action_set_tp_src_delete(of_action_set_tp_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_vlan_pcp_t
 * @param obj An instance of type of_action_set_vlan_pcp_t
 *
 * \ingroup of_action_set_vlan_pcp
 */
static inline void
of_action_set_vlan_pcp_delete(of_action_set_vlan_pcp_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_set_vlan_vid_t
 * @param obj An instance of type of_action_set_vlan_vid_t
 *
 * \ingroup of_action_set_vlan_vid
 */
static inline void
of_action_set_vlan_vid_delete(of_action_set_vlan_vid_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_action_strip_vlan_t
 * @param obj An instance of type of_action_strip_vlan_t
 *
 * \ingroup of_action_strip_vlan
 */
static inline void
of_action_strip_vlan_delete(of_action_strip_vlan_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_controller_connection_t
 * @param obj An instance of type of_bsn_controller_connection_t
 *
 * \ingroup of_bsn_controller_connection
 */
static inline void
of_bsn_controller_connection_delete(of_bsn_controller_connection_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_debug_counter_desc_stats_entry_t
 * @param obj An instance of type of_bsn_debug_counter_desc_stats_entry_t
 *
 * \ingroup of_bsn_debug_counter_desc_stats_entry
 */
static inline void
of_bsn_debug_counter_desc_stats_entry_delete(of_bsn_debug_counter_desc_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_debug_counter_stats_entry_t
 * @param obj An instance of type of_bsn_debug_counter_stats_entry_t
 *
 * \ingroup of_bsn_debug_counter_stats_entry
 */
static inline void
of_bsn_debug_counter_stats_entry_delete(of_bsn_debug_counter_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_flow_checksum_bucket_stats_entry_t
 * @param obj An instance of type of_bsn_flow_checksum_bucket_stats_entry_t
 *
 * \ingroup of_bsn_flow_checksum_bucket_stats_entry
 */
static inline void
of_bsn_flow_checksum_bucket_stats_entry_delete(of_bsn_flow_checksum_bucket_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_bucket_stats_entry_t
 * @param obj An instance of type of_bsn_gentable_bucket_stats_entry_t
 *
 * \ingroup of_bsn_gentable_bucket_stats_entry
 */
static inline void
of_bsn_gentable_bucket_stats_entry_delete(of_bsn_gentable_bucket_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_desc_stats_entry_t
 * @param obj An instance of type of_bsn_gentable_desc_stats_entry_t
 *
 * \ingroup of_bsn_gentable_desc_stats_entry
 */
static inline void
of_bsn_gentable_desc_stats_entry_delete(of_bsn_gentable_desc_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_entry_desc_stats_entry_t
 * @param obj An instance of type of_bsn_gentable_entry_desc_stats_entry_t
 *
 * \ingroup of_bsn_gentable_entry_desc_stats_entry
 */
static inline void
of_bsn_gentable_entry_desc_stats_entry_delete(of_bsn_gentable_entry_desc_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_entry_stats_entry_t
 * @param obj An instance of type of_bsn_gentable_entry_stats_entry_t
 *
 * \ingroup of_bsn_gentable_entry_stats_entry
 */
static inline void
of_bsn_gentable_entry_stats_entry_delete(of_bsn_gentable_entry_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_gentable_stats_entry_t
 * @param obj An instance of type of_bsn_gentable_stats_entry_t
 *
 * \ingroup of_bsn_gentable_stats_entry
 */
static inline void
of_bsn_gentable_stats_entry_delete(of_bsn_gentable_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_interface_t
 * @param obj An instance of type of_bsn_interface_t
 *
 * \ingroup of_bsn_interface
 */
static inline void
of_bsn_interface_delete(of_bsn_interface_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_lacp_stats_entry_t
 * @param obj An instance of type of_bsn_lacp_stats_entry_t
 *
 * \ingroup of_bsn_lacp_stats_entry
 */
static inline void
of_bsn_lacp_stats_entry_delete(of_bsn_lacp_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_port_counter_stats_entry_t
 * @param obj An instance of type of_bsn_port_counter_stats_entry_t
 *
 * \ingroup of_bsn_port_counter_stats_entry
 */
static inline void
of_bsn_port_counter_stats_entry_delete(of_bsn_port_counter_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_switch_pipeline_stats_entry_t
 * @param obj An instance of type of_bsn_switch_pipeline_stats_entry_t
 *
 * \ingroup of_bsn_switch_pipeline_stats_entry
 */
static inline void
of_bsn_switch_pipeline_stats_entry_delete(of_bsn_switch_pipeline_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_table_checksum_stats_entry_t
 * @param obj An instance of type of_bsn_table_checksum_stats_entry_t
 *
 * \ingroup of_bsn_table_checksum_stats_entry
 */
static inline void
of_bsn_table_checksum_stats_entry_delete(of_bsn_table_checksum_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_t
 * @param obj An instance of type of_bsn_tlv_t
 *
 * \ingroup of_bsn_tlv
 */
static inline void
of_bsn_tlv_delete(of_bsn_tlv_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_broadcast_query_timeout_t
 * @param obj An instance of type of_bsn_tlv_broadcast_query_timeout_t
 *
 * \ingroup of_bsn_tlv_broadcast_query_timeout
 */
static inline void
of_bsn_tlv_broadcast_query_timeout_delete(of_bsn_tlv_broadcast_query_timeout_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_circuit_id_t
 * @param obj An instance of type of_bsn_tlv_circuit_id_t
 *
 * \ingroup of_bsn_tlv_circuit_id
 */
static inline void
of_bsn_tlv_circuit_id_delete(of_bsn_tlv_circuit_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_header_t
 * @param obj An instance of type of_bsn_tlv_header_t
 *
 * \ingroup of_bsn_tlv_header
 */
static inline void
of_bsn_tlv_header_delete(of_bsn_tlv_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_idle_notification_t
 * @param obj An instance of type of_bsn_tlv_idle_notification_t
 *
 * \ingroup of_bsn_tlv_idle_notification
 */
static inline void
of_bsn_tlv_idle_notification_delete(of_bsn_tlv_idle_notification_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_idle_time_t
 * @param obj An instance of type of_bsn_tlv_idle_time_t
 *
 * \ingroup of_bsn_tlv_idle_time
 */
static inline void
of_bsn_tlv_idle_time_delete(of_bsn_tlv_idle_time_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_idle_timeout_t
 * @param obj An instance of type of_bsn_tlv_idle_timeout_t
 *
 * \ingroup of_bsn_tlv_idle_timeout
 */
static inline void
of_bsn_tlv_idle_timeout_delete(of_bsn_tlv_idle_timeout_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_ipv4_t
 * @param obj An instance of type of_bsn_tlv_ipv4_t
 *
 * \ingroup of_bsn_tlv_ipv4
 */
static inline void
of_bsn_tlv_ipv4_delete(of_bsn_tlv_ipv4_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_mac_t
 * @param obj An instance of type of_bsn_tlv_mac_t
 *
 * \ingroup of_bsn_tlv_mac
 */
static inline void
of_bsn_tlv_mac_delete(of_bsn_tlv_mac_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_miss_packets_t
 * @param obj An instance of type of_bsn_tlv_miss_packets_t
 *
 * \ingroup of_bsn_tlv_miss_packets
 */
static inline void
of_bsn_tlv_miss_packets_delete(of_bsn_tlv_miss_packets_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_port_t
 * @param obj An instance of type of_bsn_tlv_port_t
 *
 * \ingroup of_bsn_tlv_port
 */
static inline void
of_bsn_tlv_port_delete(of_bsn_tlv_port_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_reply_packets_t
 * @param obj An instance of type of_bsn_tlv_reply_packets_t
 *
 * \ingroup of_bsn_tlv_reply_packets
 */
static inline void
of_bsn_tlv_reply_packets_delete(of_bsn_tlv_reply_packets_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_request_packets_t
 * @param obj An instance of type of_bsn_tlv_request_packets_t
 *
 * \ingroup of_bsn_tlv_request_packets
 */
static inline void
of_bsn_tlv_request_packets_delete(of_bsn_tlv_request_packets_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_rx_packets_t
 * @param obj An instance of type of_bsn_tlv_rx_packets_t
 *
 * \ingroup of_bsn_tlv_rx_packets
 */
static inline void
of_bsn_tlv_rx_packets_delete(of_bsn_tlv_rx_packets_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_tx_packets_t
 * @param obj An instance of type of_bsn_tlv_tx_packets_t
 *
 * \ingroup of_bsn_tlv_tx_packets
 */
static inline void
of_bsn_tlv_tx_packets_delete(of_bsn_tlv_tx_packets_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_udf_anchor_t
 * @param obj An instance of type of_bsn_tlv_udf_anchor_t
 *
 * \ingroup of_bsn_tlv_udf_anchor
 */
static inline void
of_bsn_tlv_udf_anchor_delete(of_bsn_tlv_udf_anchor_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_udf_id_t
 * @param obj An instance of type of_bsn_tlv_udf_id_t
 *
 * \ingroup of_bsn_tlv_udf_id
 */
static inline void
of_bsn_tlv_udf_id_delete(of_bsn_tlv_udf_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_udf_length_t
 * @param obj An instance of type of_bsn_tlv_udf_length_t
 *
 * \ingroup of_bsn_tlv_udf_length
 */
static inline void
of_bsn_tlv_udf_length_delete(of_bsn_tlv_udf_length_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_udf_offset_t
 * @param obj An instance of type of_bsn_tlv_udf_offset_t
 *
 * \ingroup of_bsn_tlv_udf_offset
 */
static inline void
of_bsn_tlv_udf_offset_delete(of_bsn_tlv_udf_offset_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_unicast_query_timeout_t
 * @param obj An instance of type of_bsn_tlv_unicast_query_timeout_t
 *
 * \ingroup of_bsn_tlv_unicast_query_timeout
 */
static inline void
of_bsn_tlv_unicast_query_timeout_delete(of_bsn_tlv_unicast_query_timeout_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_tlv_vlan_vid_t
 * @param obj An instance of type of_bsn_tlv_vlan_vid_t
 *
 * \ingroup of_bsn_tlv_vlan_vid
 */
static inline void
of_bsn_tlv_vlan_vid_delete(of_bsn_tlv_vlan_vid_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_vlan_counter_stats_entry_t
 * @param obj An instance of type of_bsn_vlan_counter_stats_entry_t
 *
 * \ingroup of_bsn_vlan_counter_stats_entry
 */
static inline void
of_bsn_vlan_counter_stats_entry_delete(of_bsn_vlan_counter_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_vport_t
 * @param obj An instance of type of_bsn_vport_t
 *
 * \ingroup of_bsn_vport
 */
static inline void
of_bsn_vport_delete(of_bsn_vport_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_vport_header_t
 * @param obj An instance of type of_bsn_vport_header_t
 *
 * \ingroup of_bsn_vport_header
 */
static inline void
of_bsn_vport_header_delete(of_bsn_vport_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bsn_vport_q_in_q_t
 * @param obj An instance of type of_bsn_vport_q_in_q_t
 *
 * \ingroup of_bsn_vport_q_in_q
 */
static inline void
of_bsn_vport_q_in_q_delete(of_bsn_vport_q_in_q_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bucket_t
 * @param obj An instance of type of_bucket_t
 *
 * \ingroup of_bucket
 */
static inline void
of_bucket_delete(of_bucket_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_bucket_counter_t
 * @param obj An instance of type of_bucket_counter_t
 *
 * \ingroup of_bucket_counter
 */
static inline void
of_bucket_counter_delete(of_bucket_counter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_flow_stats_entry_t
 * @param obj An instance of type of_flow_stats_entry_t
 *
 * \ingroup of_flow_stats_entry
 */
static inline void
of_flow_stats_entry_delete(of_flow_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_desc_stats_entry_t
 * @param obj An instance of type of_group_desc_stats_entry_t
 *
 * \ingroup of_group_desc_stats_entry
 */
static inline void
of_group_desc_stats_entry_delete(of_group_desc_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_group_stats_entry_t
 * @param obj An instance of type of_group_stats_entry_t
 *
 * \ingroup of_group_stats_entry
 */
static inline void
of_group_stats_entry_delete(of_group_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_header_t
 * @param obj An instance of type of_header_t
 *
 * \ingroup of_header
 */
static inline void
of_header_delete(of_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_hello_elem_t
 * @param obj An instance of type of_hello_elem_t
 *
 * \ingroup of_hello_elem
 */
static inline void
of_hello_elem_delete(of_hello_elem_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_hello_elem_header_t
 * @param obj An instance of type of_hello_elem_header_t
 *
 * \ingroup of_hello_elem_header
 */
static inline void
of_hello_elem_header_delete(of_hello_elem_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_hello_elem_versionbitmap_t
 * @param obj An instance of type of_hello_elem_versionbitmap_t
 *
 * \ingroup of_hello_elem_versionbitmap
 */
static inline void
of_hello_elem_versionbitmap_delete(of_hello_elem_versionbitmap_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_t
 * @param obj An instance of type of_instruction_t
 *
 * \ingroup of_instruction
 */
static inline void
of_instruction_delete(of_instruction_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_apply_actions_t
 * @param obj An instance of type of_instruction_apply_actions_t
 *
 * \ingroup of_instruction_apply_actions
 */
static inline void
of_instruction_apply_actions_delete(of_instruction_apply_actions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_bsn_t
 * @param obj An instance of type of_instruction_bsn_t
 *
 * \ingroup of_instruction_bsn
 */
static inline void
of_instruction_bsn_delete(of_instruction_bsn_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_bsn_arp_offload_t
 * @param obj An instance of type of_instruction_bsn_arp_offload_t
 *
 * \ingroup of_instruction_bsn_arp_offload
 */
static inline void
of_instruction_bsn_arp_offload_delete(of_instruction_bsn_arp_offload_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_bsn_deny_t
 * @param obj An instance of type of_instruction_bsn_deny_t
 *
 * \ingroup of_instruction_bsn_deny
 */
static inline void
of_instruction_bsn_deny_delete(of_instruction_bsn_deny_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_bsn_dhcp_offload_t
 * @param obj An instance of type of_instruction_bsn_dhcp_offload_t
 *
 * \ingroup of_instruction_bsn_dhcp_offload
 */
static inline void
of_instruction_bsn_dhcp_offload_delete(of_instruction_bsn_dhcp_offload_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_bsn_disable_split_horizon_check_t
 * @param obj An instance of type of_instruction_bsn_disable_split_horizon_check_t
 *
 * \ingroup of_instruction_bsn_disable_split_horizon_check
 */
static inline void
of_instruction_bsn_disable_split_horizon_check_delete(of_instruction_bsn_disable_split_horizon_check_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_bsn_disable_src_mac_check_t
 * @param obj An instance of type of_instruction_bsn_disable_src_mac_check_t
 *
 * \ingroup of_instruction_bsn_disable_src_mac_check
 */
static inline void
of_instruction_bsn_disable_src_mac_check_delete(of_instruction_bsn_disable_src_mac_check_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_bsn_packet_of_death_t
 * @param obj An instance of type of_instruction_bsn_packet_of_death_t
 *
 * \ingroup of_instruction_bsn_packet_of_death
 */
static inline void
of_instruction_bsn_packet_of_death_delete(of_instruction_bsn_packet_of_death_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_bsn_permit_t
 * @param obj An instance of type of_instruction_bsn_permit_t
 *
 * \ingroup of_instruction_bsn_permit
 */
static inline void
of_instruction_bsn_permit_delete(of_instruction_bsn_permit_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_clear_actions_t
 * @param obj An instance of type of_instruction_clear_actions_t
 *
 * \ingroup of_instruction_clear_actions
 */
static inline void
of_instruction_clear_actions_delete(of_instruction_clear_actions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_experimenter_t
 * @param obj An instance of type of_instruction_experimenter_t
 *
 * \ingroup of_instruction_experimenter
 */
static inline void
of_instruction_experimenter_delete(of_instruction_experimenter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_goto_table_t
 * @param obj An instance of type of_instruction_goto_table_t
 *
 * \ingroup of_instruction_goto_table
 */
static inline void
of_instruction_goto_table_delete(of_instruction_goto_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_header_t
 * @param obj An instance of type of_instruction_header_t
 *
 * \ingroup of_instruction_header
 */
static inline void
of_instruction_header_delete(of_instruction_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_t
 * @param obj An instance of type of_instruction_id_t
 *
 * \ingroup of_instruction_id
 */
static inline void
of_instruction_id_delete(of_instruction_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_apply_actions_t
 * @param obj An instance of type of_instruction_id_apply_actions_t
 *
 * \ingroup of_instruction_id_apply_actions
 */
static inline void
of_instruction_id_apply_actions_delete(of_instruction_id_apply_actions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_bsn_t
 * @param obj An instance of type of_instruction_id_bsn_t
 *
 * \ingroup of_instruction_id_bsn
 */
static inline void
of_instruction_id_bsn_delete(of_instruction_id_bsn_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_bsn_arp_offload_t
 * @param obj An instance of type of_instruction_id_bsn_arp_offload_t
 *
 * \ingroup of_instruction_id_bsn_arp_offload
 */
static inline void
of_instruction_id_bsn_arp_offload_delete(of_instruction_id_bsn_arp_offload_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_bsn_deny_t
 * @param obj An instance of type of_instruction_id_bsn_deny_t
 *
 * \ingroup of_instruction_id_bsn_deny
 */
static inline void
of_instruction_id_bsn_deny_delete(of_instruction_id_bsn_deny_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_bsn_dhcp_offload_t
 * @param obj An instance of type of_instruction_id_bsn_dhcp_offload_t
 *
 * \ingroup of_instruction_id_bsn_dhcp_offload
 */
static inline void
of_instruction_id_bsn_dhcp_offload_delete(of_instruction_id_bsn_dhcp_offload_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_bsn_disable_split_horizon_check_t
 * @param obj An instance of type of_instruction_id_bsn_disable_split_horizon_check_t
 *
 * \ingroup of_instruction_id_bsn_disable_split_horizon_check
 */
static inline void
of_instruction_id_bsn_disable_split_horizon_check_delete(of_instruction_id_bsn_disable_split_horizon_check_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_bsn_disable_src_mac_check_t
 * @param obj An instance of type of_instruction_id_bsn_disable_src_mac_check_t
 *
 * \ingroup of_instruction_id_bsn_disable_src_mac_check
 */
static inline void
of_instruction_id_bsn_disable_src_mac_check_delete(of_instruction_id_bsn_disable_src_mac_check_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_bsn_packet_of_death_t
 * @param obj An instance of type of_instruction_id_bsn_packet_of_death_t
 *
 * \ingroup of_instruction_id_bsn_packet_of_death
 */
static inline void
of_instruction_id_bsn_packet_of_death_delete(of_instruction_id_bsn_packet_of_death_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_bsn_permit_t
 * @param obj An instance of type of_instruction_id_bsn_permit_t
 *
 * \ingroup of_instruction_id_bsn_permit
 */
static inline void
of_instruction_id_bsn_permit_delete(of_instruction_id_bsn_permit_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_clear_actions_t
 * @param obj An instance of type of_instruction_id_clear_actions_t
 *
 * \ingroup of_instruction_id_clear_actions
 */
static inline void
of_instruction_id_clear_actions_delete(of_instruction_id_clear_actions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_experimenter_t
 * @param obj An instance of type of_instruction_id_experimenter_t
 *
 * \ingroup of_instruction_id_experimenter
 */
static inline void
of_instruction_id_experimenter_delete(of_instruction_id_experimenter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_goto_table_t
 * @param obj An instance of type of_instruction_id_goto_table_t
 *
 * \ingroup of_instruction_id_goto_table
 */
static inline void
of_instruction_id_goto_table_delete(of_instruction_id_goto_table_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_header_t
 * @param obj An instance of type of_instruction_id_header_t
 *
 * \ingroup of_instruction_id_header
 */
static inline void
of_instruction_id_header_delete(of_instruction_id_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_meter_t
 * @param obj An instance of type of_instruction_id_meter_t
 *
 * \ingroup of_instruction_id_meter
 */
static inline void
of_instruction_id_meter_delete(of_instruction_id_meter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_write_actions_t
 * @param obj An instance of type of_instruction_id_write_actions_t
 *
 * \ingroup of_instruction_id_write_actions
 */
static inline void
of_instruction_id_write_actions_delete(of_instruction_id_write_actions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_id_write_metadata_t
 * @param obj An instance of type of_instruction_id_write_metadata_t
 *
 * \ingroup of_instruction_id_write_metadata
 */
static inline void
of_instruction_id_write_metadata_delete(of_instruction_id_write_metadata_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_meter_t
 * @param obj An instance of type of_instruction_meter_t
 *
 * \ingroup of_instruction_meter
 */
static inline void
of_instruction_meter_delete(of_instruction_meter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_write_actions_t
 * @param obj An instance of type of_instruction_write_actions_t
 *
 * \ingroup of_instruction_write_actions
 */
static inline void
of_instruction_write_actions_delete(of_instruction_write_actions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_instruction_write_metadata_t
 * @param obj An instance of type of_instruction_write_metadata_t
 *
 * \ingroup of_instruction_write_metadata
 */
static inline void
of_instruction_write_metadata_delete(of_instruction_write_metadata_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_match_v1_t
 * @param obj An instance of type of_match_v1_t
 *
 * \ingroup of_match_v1
 */
static inline void
of_match_v1_delete(of_match_v1_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_match_v2_t
 * @param obj An instance of type of_match_v2_t
 *
 * \ingroup of_match_v2
 */
static inline void
of_match_v2_delete(of_match_v2_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_match_v3_t
 * @param obj An instance of type of_match_v3_t
 *
 * \ingroup of_match_v3
 */
static inline void
of_match_v3_delete(of_match_v3_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_band_t
 * @param obj An instance of type of_meter_band_t
 *
 * \ingroup of_meter_band
 */
static inline void
of_meter_band_delete(of_meter_band_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_band_drop_t
 * @param obj An instance of type of_meter_band_drop_t
 *
 * \ingroup of_meter_band_drop
 */
static inline void
of_meter_band_drop_delete(of_meter_band_drop_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_band_dscp_remark_t
 * @param obj An instance of type of_meter_band_dscp_remark_t
 *
 * \ingroup of_meter_band_dscp_remark
 */
static inline void
of_meter_band_dscp_remark_delete(of_meter_band_dscp_remark_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_band_experimenter_t
 * @param obj An instance of type of_meter_band_experimenter_t
 *
 * \ingroup of_meter_band_experimenter
 */
static inline void
of_meter_band_experimenter_delete(of_meter_band_experimenter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_band_header_t
 * @param obj An instance of type of_meter_band_header_t
 *
 * \ingroup of_meter_band_header
 */
static inline void
of_meter_band_header_delete(of_meter_band_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_band_ofdpa_color_set_t
 * @param obj An instance of type of_meter_band_ofdpa_color_set_t
 *
 * \ingroup of_meter_band_ofdpa_color_set
 */
static inline void
of_meter_band_ofdpa_color_set_delete(of_meter_band_ofdpa_color_set_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_band_stats_t
 * @param obj An instance of type of_meter_band_stats_t
 *
 * \ingroup of_meter_band_stats
 */
static inline void
of_meter_band_stats_delete(of_meter_band_stats_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_config_t
 * @param obj An instance of type of_meter_config_t
 *
 * \ingroup of_meter_config
 */
static inline void
of_meter_config_delete(of_meter_config_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_features_t
 * @param obj An instance of type of_meter_features_t
 *
 * \ingroup of_meter_features
 */
static inline void
of_meter_features_delete(of_meter_features_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_meter_stats_t
 * @param obj An instance of type of_meter_stats_t
 *
 * \ingroup of_meter_stats
 */
static inline void
of_meter_stats_delete(of_meter_stats_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_t
 * @param obj An instance of type of_oxm_t
 *
 * \ingroup of_oxm
 */
static inline void
of_oxm_delete(of_oxm_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_op_t
 * @param obj An instance of type of_oxm_arp_op_t
 *
 * \ingroup of_oxm_arp_op
 */
static inline void
of_oxm_arp_op_delete(of_oxm_arp_op_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_op_masked_t
 * @param obj An instance of type of_oxm_arp_op_masked_t
 *
 * \ingroup of_oxm_arp_op_masked
 */
static inline void
of_oxm_arp_op_masked_delete(of_oxm_arp_op_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_sha_t
 * @param obj An instance of type of_oxm_arp_sha_t
 *
 * \ingroup of_oxm_arp_sha
 */
static inline void
of_oxm_arp_sha_delete(of_oxm_arp_sha_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_sha_masked_t
 * @param obj An instance of type of_oxm_arp_sha_masked_t
 *
 * \ingroup of_oxm_arp_sha_masked
 */
static inline void
of_oxm_arp_sha_masked_delete(of_oxm_arp_sha_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_spa_t
 * @param obj An instance of type of_oxm_arp_spa_t
 *
 * \ingroup of_oxm_arp_spa
 */
static inline void
of_oxm_arp_spa_delete(of_oxm_arp_spa_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_spa_masked_t
 * @param obj An instance of type of_oxm_arp_spa_masked_t
 *
 * \ingroup of_oxm_arp_spa_masked
 */
static inline void
of_oxm_arp_spa_masked_delete(of_oxm_arp_spa_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_tha_t
 * @param obj An instance of type of_oxm_arp_tha_t
 *
 * \ingroup of_oxm_arp_tha
 */
static inline void
of_oxm_arp_tha_delete(of_oxm_arp_tha_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_tha_masked_t
 * @param obj An instance of type of_oxm_arp_tha_masked_t
 *
 * \ingroup of_oxm_arp_tha_masked
 */
static inline void
of_oxm_arp_tha_masked_delete(of_oxm_arp_tha_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_tpa_t
 * @param obj An instance of type of_oxm_arp_tpa_t
 *
 * \ingroup of_oxm_arp_tpa
 */
static inline void
of_oxm_arp_tpa_delete(of_oxm_arp_tpa_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_arp_tpa_masked_t
 * @param obj An instance of type of_oxm_arp_tpa_masked_t
 *
 * \ingroup of_oxm_arp_tpa_masked
 */
static inline void
of_oxm_arp_tpa_masked_delete(of_oxm_arp_tpa_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_egr_port_group_id_t
 * @param obj An instance of type of_oxm_bsn_egr_port_group_id_t
 *
 * \ingroup of_oxm_bsn_egr_port_group_id
 */
static inline void
of_oxm_bsn_egr_port_group_id_delete(of_oxm_bsn_egr_port_group_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_egr_port_group_id_masked_t
 * @param obj An instance of type of_oxm_bsn_egr_port_group_id_masked_t
 *
 * \ingroup of_oxm_bsn_egr_port_group_id_masked
 */
static inline void
of_oxm_bsn_egr_port_group_id_masked_delete(of_oxm_bsn_egr_port_group_id_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_global_vrf_allowed_t
 * @param obj An instance of type of_oxm_bsn_global_vrf_allowed_t
 *
 * \ingroup of_oxm_bsn_global_vrf_allowed
 */
static inline void
of_oxm_bsn_global_vrf_allowed_delete(of_oxm_bsn_global_vrf_allowed_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_global_vrf_allowed_masked_t
 * @param obj An instance of type of_oxm_bsn_global_vrf_allowed_masked_t
 *
 * \ingroup of_oxm_bsn_global_vrf_allowed_masked
 */
static inline void
of_oxm_bsn_global_vrf_allowed_masked_delete(of_oxm_bsn_global_vrf_allowed_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_in_ports_128_t
 * @param obj An instance of type of_oxm_bsn_in_ports_128_t
 *
 * \ingroup of_oxm_bsn_in_ports_128
 */
static inline void
of_oxm_bsn_in_ports_128_delete(of_oxm_bsn_in_ports_128_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_in_ports_128_masked_t
 * @param obj An instance of type of_oxm_bsn_in_ports_128_masked_t
 *
 * \ingroup of_oxm_bsn_in_ports_128_masked
 */
static inline void
of_oxm_bsn_in_ports_128_masked_delete(of_oxm_bsn_in_ports_128_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_l3_dst_class_id_t
 * @param obj An instance of type of_oxm_bsn_l3_dst_class_id_t
 *
 * \ingroup of_oxm_bsn_l3_dst_class_id
 */
static inline void
of_oxm_bsn_l3_dst_class_id_delete(of_oxm_bsn_l3_dst_class_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_l3_dst_class_id_masked_t
 * @param obj An instance of type of_oxm_bsn_l3_dst_class_id_masked_t
 *
 * \ingroup of_oxm_bsn_l3_dst_class_id_masked
 */
static inline void
of_oxm_bsn_l3_dst_class_id_masked_delete(of_oxm_bsn_l3_dst_class_id_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_l3_interface_class_id_t
 * @param obj An instance of type of_oxm_bsn_l3_interface_class_id_t
 *
 * \ingroup of_oxm_bsn_l3_interface_class_id
 */
static inline void
of_oxm_bsn_l3_interface_class_id_delete(of_oxm_bsn_l3_interface_class_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_l3_interface_class_id_masked_t
 * @param obj An instance of type of_oxm_bsn_l3_interface_class_id_masked_t
 *
 * \ingroup of_oxm_bsn_l3_interface_class_id_masked
 */
static inline void
of_oxm_bsn_l3_interface_class_id_masked_delete(of_oxm_bsn_l3_interface_class_id_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_l3_src_class_id_t
 * @param obj An instance of type of_oxm_bsn_l3_src_class_id_t
 *
 * \ingroup of_oxm_bsn_l3_src_class_id
 */
static inline void
of_oxm_bsn_l3_src_class_id_delete(of_oxm_bsn_l3_src_class_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_l3_src_class_id_masked_t
 * @param obj An instance of type of_oxm_bsn_l3_src_class_id_masked_t
 *
 * \ingroup of_oxm_bsn_l3_src_class_id_masked
 */
static inline void
of_oxm_bsn_l3_src_class_id_masked_delete(of_oxm_bsn_l3_src_class_id_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_lag_id_t
 * @param obj An instance of type of_oxm_bsn_lag_id_t
 *
 * \ingroup of_oxm_bsn_lag_id
 */
static inline void
of_oxm_bsn_lag_id_delete(of_oxm_bsn_lag_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_lag_id_masked_t
 * @param obj An instance of type of_oxm_bsn_lag_id_masked_t
 *
 * \ingroup of_oxm_bsn_lag_id_masked
 */
static inline void
of_oxm_bsn_lag_id_masked_delete(of_oxm_bsn_lag_id_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf0_t
 * @param obj An instance of type of_oxm_bsn_udf0_t
 *
 * \ingroup of_oxm_bsn_udf0
 */
static inline void
of_oxm_bsn_udf0_delete(of_oxm_bsn_udf0_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf0_masked_t
 * @param obj An instance of type of_oxm_bsn_udf0_masked_t
 *
 * \ingroup of_oxm_bsn_udf0_masked
 */
static inline void
of_oxm_bsn_udf0_masked_delete(of_oxm_bsn_udf0_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf1_t
 * @param obj An instance of type of_oxm_bsn_udf1_t
 *
 * \ingroup of_oxm_bsn_udf1
 */
static inline void
of_oxm_bsn_udf1_delete(of_oxm_bsn_udf1_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf1_masked_t
 * @param obj An instance of type of_oxm_bsn_udf1_masked_t
 *
 * \ingroup of_oxm_bsn_udf1_masked
 */
static inline void
of_oxm_bsn_udf1_masked_delete(of_oxm_bsn_udf1_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf2_t
 * @param obj An instance of type of_oxm_bsn_udf2_t
 *
 * \ingroup of_oxm_bsn_udf2
 */
static inline void
of_oxm_bsn_udf2_delete(of_oxm_bsn_udf2_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf2_masked_t
 * @param obj An instance of type of_oxm_bsn_udf2_masked_t
 *
 * \ingroup of_oxm_bsn_udf2_masked
 */
static inline void
of_oxm_bsn_udf2_masked_delete(of_oxm_bsn_udf2_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf3_t
 * @param obj An instance of type of_oxm_bsn_udf3_t
 *
 * \ingroup of_oxm_bsn_udf3
 */
static inline void
of_oxm_bsn_udf3_delete(of_oxm_bsn_udf3_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf3_masked_t
 * @param obj An instance of type of_oxm_bsn_udf3_masked_t
 *
 * \ingroup of_oxm_bsn_udf3_masked
 */
static inline void
of_oxm_bsn_udf3_masked_delete(of_oxm_bsn_udf3_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf4_t
 * @param obj An instance of type of_oxm_bsn_udf4_t
 *
 * \ingroup of_oxm_bsn_udf4
 */
static inline void
of_oxm_bsn_udf4_delete(of_oxm_bsn_udf4_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf4_masked_t
 * @param obj An instance of type of_oxm_bsn_udf4_masked_t
 *
 * \ingroup of_oxm_bsn_udf4_masked
 */
static inline void
of_oxm_bsn_udf4_masked_delete(of_oxm_bsn_udf4_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf5_t
 * @param obj An instance of type of_oxm_bsn_udf5_t
 *
 * \ingroup of_oxm_bsn_udf5
 */
static inline void
of_oxm_bsn_udf5_delete(of_oxm_bsn_udf5_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf5_masked_t
 * @param obj An instance of type of_oxm_bsn_udf5_masked_t
 *
 * \ingroup of_oxm_bsn_udf5_masked
 */
static inline void
of_oxm_bsn_udf5_masked_delete(of_oxm_bsn_udf5_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf6_t
 * @param obj An instance of type of_oxm_bsn_udf6_t
 *
 * \ingroup of_oxm_bsn_udf6
 */
static inline void
of_oxm_bsn_udf6_delete(of_oxm_bsn_udf6_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf6_masked_t
 * @param obj An instance of type of_oxm_bsn_udf6_masked_t
 *
 * \ingroup of_oxm_bsn_udf6_masked
 */
static inline void
of_oxm_bsn_udf6_masked_delete(of_oxm_bsn_udf6_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf7_t
 * @param obj An instance of type of_oxm_bsn_udf7_t
 *
 * \ingroup of_oxm_bsn_udf7
 */
static inline void
of_oxm_bsn_udf7_delete(of_oxm_bsn_udf7_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_udf7_masked_t
 * @param obj An instance of type of_oxm_bsn_udf7_masked_t
 *
 * \ingroup of_oxm_bsn_udf7_masked
 */
static inline void
of_oxm_bsn_udf7_masked_delete(of_oxm_bsn_udf7_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_vrf_t
 * @param obj An instance of type of_oxm_bsn_vrf_t
 *
 * \ingroup of_oxm_bsn_vrf
 */
static inline void
of_oxm_bsn_vrf_delete(of_oxm_bsn_vrf_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_bsn_vrf_masked_t
 * @param obj An instance of type of_oxm_bsn_vrf_masked_t
 *
 * \ingroup of_oxm_bsn_vrf_masked
 */
static inline void
of_oxm_bsn_vrf_masked_delete(of_oxm_bsn_vrf_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_eth_dst_t
 * @param obj An instance of type of_oxm_eth_dst_t
 *
 * \ingroup of_oxm_eth_dst
 */
static inline void
of_oxm_eth_dst_delete(of_oxm_eth_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_eth_dst_masked_t
 * @param obj An instance of type of_oxm_eth_dst_masked_t
 *
 * \ingroup of_oxm_eth_dst_masked
 */
static inline void
of_oxm_eth_dst_masked_delete(of_oxm_eth_dst_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_eth_src_t
 * @param obj An instance of type of_oxm_eth_src_t
 *
 * \ingroup of_oxm_eth_src
 */
static inline void
of_oxm_eth_src_delete(of_oxm_eth_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_eth_src_masked_t
 * @param obj An instance of type of_oxm_eth_src_masked_t
 *
 * \ingroup of_oxm_eth_src_masked
 */
static inline void
of_oxm_eth_src_masked_delete(of_oxm_eth_src_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_eth_type_t
 * @param obj An instance of type of_oxm_eth_type_t
 *
 * \ingroup of_oxm_eth_type
 */
static inline void
of_oxm_eth_type_delete(of_oxm_eth_type_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_eth_type_masked_t
 * @param obj An instance of type of_oxm_eth_type_masked_t
 *
 * \ingroup of_oxm_eth_type_masked
 */
static inline void
of_oxm_eth_type_masked_delete(of_oxm_eth_type_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_header_t
 * @param obj An instance of type of_oxm_header_t
 *
 * \ingroup of_oxm_header
 */
static inline void
of_oxm_header_delete(of_oxm_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_icmpv4_code_t
 * @param obj An instance of type of_oxm_icmpv4_code_t
 *
 * \ingroup of_oxm_icmpv4_code
 */
static inline void
of_oxm_icmpv4_code_delete(of_oxm_icmpv4_code_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_icmpv4_code_masked_t
 * @param obj An instance of type of_oxm_icmpv4_code_masked_t
 *
 * \ingroup of_oxm_icmpv4_code_masked
 */
static inline void
of_oxm_icmpv4_code_masked_delete(of_oxm_icmpv4_code_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_icmpv4_type_t
 * @param obj An instance of type of_oxm_icmpv4_type_t
 *
 * \ingroup of_oxm_icmpv4_type
 */
static inline void
of_oxm_icmpv4_type_delete(of_oxm_icmpv4_type_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_icmpv4_type_masked_t
 * @param obj An instance of type of_oxm_icmpv4_type_masked_t
 *
 * \ingroup of_oxm_icmpv4_type_masked
 */
static inline void
of_oxm_icmpv4_type_masked_delete(of_oxm_icmpv4_type_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_icmpv6_code_t
 * @param obj An instance of type of_oxm_icmpv6_code_t
 *
 * \ingroup of_oxm_icmpv6_code
 */
static inline void
of_oxm_icmpv6_code_delete(of_oxm_icmpv6_code_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_icmpv6_code_masked_t
 * @param obj An instance of type of_oxm_icmpv6_code_masked_t
 *
 * \ingroup of_oxm_icmpv6_code_masked
 */
static inline void
of_oxm_icmpv6_code_masked_delete(of_oxm_icmpv6_code_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_icmpv6_type_t
 * @param obj An instance of type of_oxm_icmpv6_type_t
 *
 * \ingroup of_oxm_icmpv6_type
 */
static inline void
of_oxm_icmpv6_type_delete(of_oxm_icmpv6_type_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_icmpv6_type_masked_t
 * @param obj An instance of type of_oxm_icmpv6_type_masked_t
 *
 * \ingroup of_oxm_icmpv6_type_masked
 */
static inline void
of_oxm_icmpv6_type_masked_delete(of_oxm_icmpv6_type_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_in_phy_port_t
 * @param obj An instance of type of_oxm_in_phy_port_t
 *
 * \ingroup of_oxm_in_phy_port
 */
static inline void
of_oxm_in_phy_port_delete(of_oxm_in_phy_port_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_in_phy_port_masked_t
 * @param obj An instance of type of_oxm_in_phy_port_masked_t
 *
 * \ingroup of_oxm_in_phy_port_masked
 */
static inline void
of_oxm_in_phy_port_masked_delete(of_oxm_in_phy_port_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_in_port_t
 * @param obj An instance of type of_oxm_in_port_t
 *
 * \ingroup of_oxm_in_port
 */
static inline void
of_oxm_in_port_delete(of_oxm_in_port_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_in_port_masked_t
 * @param obj An instance of type of_oxm_in_port_masked_t
 *
 * \ingroup of_oxm_in_port_masked
 */
static inline void
of_oxm_in_port_masked_delete(of_oxm_in_port_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ip_dscp_t
 * @param obj An instance of type of_oxm_ip_dscp_t
 *
 * \ingroup of_oxm_ip_dscp
 */
static inline void
of_oxm_ip_dscp_delete(of_oxm_ip_dscp_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ip_dscp_masked_t
 * @param obj An instance of type of_oxm_ip_dscp_masked_t
 *
 * \ingroup of_oxm_ip_dscp_masked
 */
static inline void
of_oxm_ip_dscp_masked_delete(of_oxm_ip_dscp_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ip_ecn_t
 * @param obj An instance of type of_oxm_ip_ecn_t
 *
 * \ingroup of_oxm_ip_ecn
 */
static inline void
of_oxm_ip_ecn_delete(of_oxm_ip_ecn_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ip_ecn_masked_t
 * @param obj An instance of type of_oxm_ip_ecn_masked_t
 *
 * \ingroup of_oxm_ip_ecn_masked
 */
static inline void
of_oxm_ip_ecn_masked_delete(of_oxm_ip_ecn_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ip_proto_t
 * @param obj An instance of type of_oxm_ip_proto_t
 *
 * \ingroup of_oxm_ip_proto
 */
static inline void
of_oxm_ip_proto_delete(of_oxm_ip_proto_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ip_proto_masked_t
 * @param obj An instance of type of_oxm_ip_proto_masked_t
 *
 * \ingroup of_oxm_ip_proto_masked
 */
static inline void
of_oxm_ip_proto_masked_delete(of_oxm_ip_proto_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv4_dst_t
 * @param obj An instance of type of_oxm_ipv4_dst_t
 *
 * \ingroup of_oxm_ipv4_dst
 */
static inline void
of_oxm_ipv4_dst_delete(of_oxm_ipv4_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv4_dst_masked_t
 * @param obj An instance of type of_oxm_ipv4_dst_masked_t
 *
 * \ingroup of_oxm_ipv4_dst_masked
 */
static inline void
of_oxm_ipv4_dst_masked_delete(of_oxm_ipv4_dst_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv4_src_t
 * @param obj An instance of type of_oxm_ipv4_src_t
 *
 * \ingroup of_oxm_ipv4_src
 */
static inline void
of_oxm_ipv4_src_delete(of_oxm_ipv4_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv4_src_masked_t
 * @param obj An instance of type of_oxm_ipv4_src_masked_t
 *
 * \ingroup of_oxm_ipv4_src_masked
 */
static inline void
of_oxm_ipv4_src_masked_delete(of_oxm_ipv4_src_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_dst_t
 * @param obj An instance of type of_oxm_ipv6_dst_t
 *
 * \ingroup of_oxm_ipv6_dst
 */
static inline void
of_oxm_ipv6_dst_delete(of_oxm_ipv6_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_dst_masked_t
 * @param obj An instance of type of_oxm_ipv6_dst_masked_t
 *
 * \ingroup of_oxm_ipv6_dst_masked
 */
static inline void
of_oxm_ipv6_dst_masked_delete(of_oxm_ipv6_dst_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_flabel_t
 * @param obj An instance of type of_oxm_ipv6_flabel_t
 *
 * \ingroup of_oxm_ipv6_flabel
 */
static inline void
of_oxm_ipv6_flabel_delete(of_oxm_ipv6_flabel_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_flabel_masked_t
 * @param obj An instance of type of_oxm_ipv6_flabel_masked_t
 *
 * \ingroup of_oxm_ipv6_flabel_masked
 */
static inline void
of_oxm_ipv6_flabel_masked_delete(of_oxm_ipv6_flabel_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_nd_sll_t
 * @param obj An instance of type of_oxm_ipv6_nd_sll_t
 *
 * \ingroup of_oxm_ipv6_nd_sll
 */
static inline void
of_oxm_ipv6_nd_sll_delete(of_oxm_ipv6_nd_sll_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_nd_sll_masked_t
 * @param obj An instance of type of_oxm_ipv6_nd_sll_masked_t
 *
 * \ingroup of_oxm_ipv6_nd_sll_masked
 */
static inline void
of_oxm_ipv6_nd_sll_masked_delete(of_oxm_ipv6_nd_sll_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_nd_target_t
 * @param obj An instance of type of_oxm_ipv6_nd_target_t
 *
 * \ingroup of_oxm_ipv6_nd_target
 */
static inline void
of_oxm_ipv6_nd_target_delete(of_oxm_ipv6_nd_target_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_nd_target_masked_t
 * @param obj An instance of type of_oxm_ipv6_nd_target_masked_t
 *
 * \ingroup of_oxm_ipv6_nd_target_masked
 */
static inline void
of_oxm_ipv6_nd_target_masked_delete(of_oxm_ipv6_nd_target_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_nd_tll_t
 * @param obj An instance of type of_oxm_ipv6_nd_tll_t
 *
 * \ingroup of_oxm_ipv6_nd_tll
 */
static inline void
of_oxm_ipv6_nd_tll_delete(of_oxm_ipv6_nd_tll_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_nd_tll_masked_t
 * @param obj An instance of type of_oxm_ipv6_nd_tll_masked_t
 *
 * \ingroup of_oxm_ipv6_nd_tll_masked
 */
static inline void
of_oxm_ipv6_nd_tll_masked_delete(of_oxm_ipv6_nd_tll_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_src_t
 * @param obj An instance of type of_oxm_ipv6_src_t
 *
 * \ingroup of_oxm_ipv6_src
 */
static inline void
of_oxm_ipv6_src_delete(of_oxm_ipv6_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ipv6_src_masked_t
 * @param obj An instance of type of_oxm_ipv6_src_masked_t
 *
 * \ingroup of_oxm_ipv6_src_masked
 */
static inline void
of_oxm_ipv6_src_masked_delete(of_oxm_ipv6_src_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_metadata_t
 * @param obj An instance of type of_oxm_metadata_t
 *
 * \ingroup of_oxm_metadata
 */
static inline void
of_oxm_metadata_delete(of_oxm_metadata_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_metadata_masked_t
 * @param obj An instance of type of_oxm_metadata_masked_t
 *
 * \ingroup of_oxm_metadata_masked
 */
static inline void
of_oxm_metadata_masked_delete(of_oxm_metadata_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_mpls_bos_t
 * @param obj An instance of type of_oxm_mpls_bos_t
 *
 * \ingroup of_oxm_mpls_bos
 */
static inline void
of_oxm_mpls_bos_delete(of_oxm_mpls_bos_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_mpls_bos_masked_t
 * @param obj An instance of type of_oxm_mpls_bos_masked_t
 *
 * \ingroup of_oxm_mpls_bos_masked
 */
static inline void
of_oxm_mpls_bos_masked_delete(of_oxm_mpls_bos_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_mpls_label_t
 * @param obj An instance of type of_oxm_mpls_label_t
 *
 * \ingroup of_oxm_mpls_label
 */
static inline void
of_oxm_mpls_label_delete(of_oxm_mpls_label_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_mpls_label_masked_t
 * @param obj An instance of type of_oxm_mpls_label_masked_t
 *
 * \ingroup of_oxm_mpls_label_masked
 */
static inline void
of_oxm_mpls_label_masked_delete(of_oxm_mpls_label_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_mpls_tc_t
 * @param obj An instance of type of_oxm_mpls_tc_t
 *
 * \ingroup of_oxm_mpls_tc
 */
static inline void
of_oxm_mpls_tc_delete(of_oxm_mpls_tc_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_mpls_tc_masked_t
 * @param obj An instance of type of_oxm_mpls_tc_masked_t
 *
 * \ingroup of_oxm_mpls_tc_masked
 */
static inline void
of_oxm_mpls_tc_masked_delete(of_oxm_mpls_tc_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_allow_vlan_translation_t
 * @param obj An instance of type of_oxm_ofdpa_allow_vlan_translation_t
 *
 * \ingroup of_oxm_ofdpa_allow_vlan_translation
 */
static inline void
of_oxm_ofdpa_allow_vlan_translation_delete(of_oxm_ofdpa_allow_vlan_translation_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_color_t
 * @param obj An instance of type of_oxm_ofdpa_color_t
 *
 * \ingroup of_oxm_ofdpa_color
 */
static inline void
of_oxm_ofdpa_color_delete(of_oxm_ofdpa_color_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_color_actions_index_t
 * @param obj An instance of type of_oxm_ofdpa_color_actions_index_t
 *
 * \ingroup of_oxm_ofdpa_color_actions_index
 */
static inline void
of_oxm_ofdpa_color_actions_index_delete(of_oxm_ofdpa_color_actions_index_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_dei_t
 * @param obj An instance of type of_oxm_ofdpa_dei_t
 *
 * \ingroup of_oxm_ofdpa_dei
 */
static inline void
of_oxm_ofdpa_dei_delete(of_oxm_ofdpa_dei_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_l3_in_port_t
 * @param obj An instance of type of_oxm_ofdpa_l3_in_port_t
 *
 * \ingroup of_oxm_ofdpa_l3_in_port
 */
static inline void
of_oxm_ofdpa_l3_in_port_delete(of_oxm_ofdpa_l3_in_port_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_lmep_id_t
 * @param obj An instance of type of_oxm_ofdpa_lmep_id_t
 *
 * \ingroup of_oxm_ofdpa_lmep_id
 */
static inline void
of_oxm_ofdpa_lmep_id_delete(of_oxm_ofdpa_lmep_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_mpls_ach_channel_t
 * @param obj An instance of type of_oxm_ofdpa_mpls_ach_channel_t
 *
 * \ingroup of_oxm_ofdpa_mpls_ach_channel
 */
static inline void
of_oxm_ofdpa_mpls_ach_channel_delete(of_oxm_ofdpa_mpls_ach_channel_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_mpls_data_first_nibble_t
 * @param obj An instance of type of_oxm_ofdpa_mpls_data_first_nibble_t
 *
 * \ingroup of_oxm_ofdpa_mpls_data_first_nibble
 */
static inline void
of_oxm_ofdpa_mpls_data_first_nibble_delete(of_oxm_ofdpa_mpls_data_first_nibble_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_mpls_l2_port_t
 * @param obj An instance of type of_oxm_ofdpa_mpls_l2_port_t
 *
 * \ingroup of_oxm_ofdpa_mpls_l2_port
 */
static inline void
of_oxm_ofdpa_mpls_l2_port_delete(of_oxm_ofdpa_mpls_l2_port_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_mpls_l2_port_masked_t
 * @param obj An instance of type of_oxm_ofdpa_mpls_l2_port_masked_t
 *
 * \ingroup of_oxm_ofdpa_mpls_l2_port_masked
 */
static inline void
of_oxm_ofdpa_mpls_l2_port_masked_delete(of_oxm_ofdpa_mpls_l2_port_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_mpls_next_label_is_gal_t
 * @param obj An instance of type of_oxm_ofdpa_mpls_next_label_is_gal_t
 *
 * \ingroup of_oxm_ofdpa_mpls_next_label_is_gal
 */
static inline void
of_oxm_ofdpa_mpls_next_label_is_gal_delete(of_oxm_ofdpa_mpls_next_label_is_gal_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_mpls_ttl_t
 * @param obj An instance of type of_oxm_ofdpa_mpls_ttl_t
 *
 * \ingroup of_oxm_ofdpa_mpls_ttl
 */
static inline void
of_oxm_ofdpa_mpls_ttl_delete(of_oxm_ofdpa_mpls_ttl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_mpls_type_t
 * @param obj An instance of type of_oxm_ofdpa_mpls_type_t
 *
 * \ingroup of_oxm_ofdpa_mpls_type
 */
static inline void
of_oxm_ofdpa_mpls_type_delete(of_oxm_ofdpa_mpls_type_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_oam_y1731_mdl_t
 * @param obj An instance of type of_oxm_ofdpa_oam_y1731_mdl_t
 *
 * \ingroup of_oxm_ofdpa_oam_y1731_mdl
 */
static inline void
of_oxm_ofdpa_oam_y1731_mdl_delete(of_oxm_ofdpa_oam_y1731_mdl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_oam_y1731_opcode_t
 * @param obj An instance of type of_oxm_ofdpa_oam_y1731_opcode_t
 *
 * \ingroup of_oxm_ofdpa_oam_y1731_opcode
 */
static inline void
of_oxm_ofdpa_oam_y1731_opcode_delete(of_oxm_ofdpa_oam_y1731_opcode_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_ovid_t
 * @param obj An instance of type of_oxm_ofdpa_ovid_t
 *
 * \ingroup of_oxm_ofdpa_ovid
 */
static inline void
of_oxm_ofdpa_ovid_delete(of_oxm_ofdpa_ovid_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_protection_index_t
 * @param obj An instance of type of_oxm_ofdpa_protection_index_t
 *
 * \ingroup of_oxm_ofdpa_protection_index
 */
static inline void
of_oxm_ofdpa_protection_index_delete(of_oxm_ofdpa_protection_index_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_qos_index_t
 * @param obj An instance of type of_oxm_ofdpa_qos_index_t
 *
 * \ingroup of_oxm_ofdpa_qos_index
 */
static inline void
of_oxm_ofdpa_qos_index_delete(of_oxm_ofdpa_qos_index_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_rx_timestamp_t
 * @param obj An instance of type of_oxm_ofdpa_rx_timestamp_t
 *
 * \ingroup of_oxm_ofdpa_rx_timestamp
 */
static inline void
of_oxm_ofdpa_rx_timestamp_delete(of_oxm_ofdpa_rx_timestamp_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_rxfcl_t
 * @param obj An instance of type of_oxm_ofdpa_rxfcl_t
 *
 * \ingroup of_oxm_ofdpa_rxfcl
 */
static inline void
of_oxm_ofdpa_rxfcl_delete(of_oxm_ofdpa_rxfcl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_traffic_class_t
 * @param obj An instance of type of_oxm_ofdpa_traffic_class_t
 *
 * \ingroup of_oxm_ofdpa_traffic_class
 */
static inline void
of_oxm_ofdpa_traffic_class_delete(of_oxm_ofdpa_traffic_class_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_txfcl_t
 * @param obj An instance of type of_oxm_ofdpa_txfcl_t
 *
 * \ingroup of_oxm_ofdpa_txfcl
 */
static inline void
of_oxm_ofdpa_txfcl_delete(of_oxm_ofdpa_txfcl_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_ofdpa_vrf_t
 * @param obj An instance of type of_oxm_ofdpa_vrf_t
 *
 * \ingroup of_oxm_ofdpa_vrf
 */
static inline void
of_oxm_ofdpa_vrf_delete(of_oxm_ofdpa_vrf_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_onf_actset_output_t
 * @param obj An instance of type of_oxm_onf_actset_output_t
 *
 * \ingroup of_oxm_onf_actset_output
 */
static inline void
of_oxm_onf_actset_output_delete(of_oxm_onf_actset_output_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_sctp_dst_t
 * @param obj An instance of type of_oxm_sctp_dst_t
 *
 * \ingroup of_oxm_sctp_dst
 */
static inline void
of_oxm_sctp_dst_delete(of_oxm_sctp_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_sctp_dst_masked_t
 * @param obj An instance of type of_oxm_sctp_dst_masked_t
 *
 * \ingroup of_oxm_sctp_dst_masked
 */
static inline void
of_oxm_sctp_dst_masked_delete(of_oxm_sctp_dst_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_sctp_src_t
 * @param obj An instance of type of_oxm_sctp_src_t
 *
 * \ingroup of_oxm_sctp_src
 */
static inline void
of_oxm_sctp_src_delete(of_oxm_sctp_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_sctp_src_masked_t
 * @param obj An instance of type of_oxm_sctp_src_masked_t
 *
 * \ingroup of_oxm_sctp_src_masked
 */
static inline void
of_oxm_sctp_src_masked_delete(of_oxm_sctp_src_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_tcp_dst_t
 * @param obj An instance of type of_oxm_tcp_dst_t
 *
 * \ingroup of_oxm_tcp_dst
 */
static inline void
of_oxm_tcp_dst_delete(of_oxm_tcp_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_tcp_dst_masked_t
 * @param obj An instance of type of_oxm_tcp_dst_masked_t
 *
 * \ingroup of_oxm_tcp_dst_masked
 */
static inline void
of_oxm_tcp_dst_masked_delete(of_oxm_tcp_dst_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_tcp_src_t
 * @param obj An instance of type of_oxm_tcp_src_t
 *
 * \ingroup of_oxm_tcp_src
 */
static inline void
of_oxm_tcp_src_delete(of_oxm_tcp_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_tcp_src_masked_t
 * @param obj An instance of type of_oxm_tcp_src_masked_t
 *
 * \ingroup of_oxm_tcp_src_masked
 */
static inline void
of_oxm_tcp_src_masked_delete(of_oxm_tcp_src_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_tunnel_id_t
 * @param obj An instance of type of_oxm_tunnel_id_t
 *
 * \ingroup of_oxm_tunnel_id
 */
static inline void
of_oxm_tunnel_id_delete(of_oxm_tunnel_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_tunnel_id_masked_t
 * @param obj An instance of type of_oxm_tunnel_id_masked_t
 *
 * \ingroup of_oxm_tunnel_id_masked
 */
static inline void
of_oxm_tunnel_id_masked_delete(of_oxm_tunnel_id_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_udp_dst_t
 * @param obj An instance of type of_oxm_udp_dst_t
 *
 * \ingroup of_oxm_udp_dst
 */
static inline void
of_oxm_udp_dst_delete(of_oxm_udp_dst_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_udp_dst_masked_t
 * @param obj An instance of type of_oxm_udp_dst_masked_t
 *
 * \ingroup of_oxm_udp_dst_masked
 */
static inline void
of_oxm_udp_dst_masked_delete(of_oxm_udp_dst_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_udp_src_t
 * @param obj An instance of type of_oxm_udp_src_t
 *
 * \ingroup of_oxm_udp_src
 */
static inline void
of_oxm_udp_src_delete(of_oxm_udp_src_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_udp_src_masked_t
 * @param obj An instance of type of_oxm_udp_src_masked_t
 *
 * \ingroup of_oxm_udp_src_masked
 */
static inline void
of_oxm_udp_src_masked_delete(of_oxm_udp_src_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_vlan_pcp_t
 * @param obj An instance of type of_oxm_vlan_pcp_t
 *
 * \ingroup of_oxm_vlan_pcp
 */
static inline void
of_oxm_vlan_pcp_delete(of_oxm_vlan_pcp_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_vlan_pcp_masked_t
 * @param obj An instance of type of_oxm_vlan_pcp_masked_t
 *
 * \ingroup of_oxm_vlan_pcp_masked
 */
static inline void
of_oxm_vlan_pcp_masked_delete(of_oxm_vlan_pcp_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_vlan_vid_t
 * @param obj An instance of type of_oxm_vlan_vid_t
 *
 * \ingroup of_oxm_vlan_vid
 */
static inline void
of_oxm_vlan_vid_delete(of_oxm_vlan_vid_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_oxm_vlan_vid_masked_t
 * @param obj An instance of type of_oxm_vlan_vid_masked_t
 *
 * \ingroup of_oxm_vlan_vid_masked
 */
static inline void
of_oxm_vlan_vid_masked_delete(of_oxm_vlan_vid_masked_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_packet_queue_t
 * @param obj An instance of type of_packet_queue_t
 *
 * \ingroup of_packet_queue
 */
static inline void
of_packet_queue_delete(of_packet_queue_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_desc_t
 * @param obj An instance of type of_port_desc_t
 *
 * \ingroup of_port_desc
 */
static inline void
of_port_desc_delete(of_port_desc_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_port_stats_entry_t
 * @param obj An instance of type of_port_stats_entry_t
 *
 * \ingroup of_port_stats_entry
 */
static inline void
of_port_stats_entry_delete(of_port_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_prop_t
 * @param obj An instance of type of_queue_prop_t
 *
 * \ingroup of_queue_prop
 */
static inline void
of_queue_prop_delete(of_queue_prop_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_prop_experimenter_t
 * @param obj An instance of type of_queue_prop_experimenter_t
 *
 * \ingroup of_queue_prop_experimenter
 */
static inline void
of_queue_prop_experimenter_delete(of_queue_prop_experimenter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_prop_header_t
 * @param obj An instance of type of_queue_prop_header_t
 *
 * \ingroup of_queue_prop_header
 */
static inline void
of_queue_prop_header_delete(of_queue_prop_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_prop_max_rate_t
 * @param obj An instance of type of_queue_prop_max_rate_t
 *
 * \ingroup of_queue_prop_max_rate
 */
static inline void
of_queue_prop_max_rate_delete(of_queue_prop_max_rate_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_prop_min_rate_t
 * @param obj An instance of type of_queue_prop_min_rate_t
 *
 * \ingroup of_queue_prop_min_rate
 */
static inline void
of_queue_prop_min_rate_delete(of_queue_prop_min_rate_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_queue_stats_entry_t
 * @param obj An instance of type of_queue_stats_entry_t
 *
 * \ingroup of_queue_stats_entry
 */
static inline void
of_queue_stats_entry_delete(of_queue_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_t
 * @param obj An instance of type of_table_feature_prop_t
 *
 * \ingroup of_table_feature_prop
 */
static inline void
of_table_feature_prop_delete(of_table_feature_prop_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_apply_actions_t
 * @param obj An instance of type of_table_feature_prop_apply_actions_t
 *
 * \ingroup of_table_feature_prop_apply_actions
 */
static inline void
of_table_feature_prop_apply_actions_delete(of_table_feature_prop_apply_actions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_apply_actions_miss_t
 * @param obj An instance of type of_table_feature_prop_apply_actions_miss_t
 *
 * \ingroup of_table_feature_prop_apply_actions_miss
 */
static inline void
of_table_feature_prop_apply_actions_miss_delete(of_table_feature_prop_apply_actions_miss_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_apply_setfield_t
 * @param obj An instance of type of_table_feature_prop_apply_setfield_t
 *
 * \ingroup of_table_feature_prop_apply_setfield
 */
static inline void
of_table_feature_prop_apply_setfield_delete(of_table_feature_prop_apply_setfield_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_apply_setfield_miss_t
 * @param obj An instance of type of_table_feature_prop_apply_setfield_miss_t
 *
 * \ingroup of_table_feature_prop_apply_setfield_miss
 */
static inline void
of_table_feature_prop_apply_setfield_miss_delete(of_table_feature_prop_apply_setfield_miss_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_experimenter_t
 * @param obj An instance of type of_table_feature_prop_experimenter_t
 *
 * \ingroup of_table_feature_prop_experimenter
 */
static inline void
of_table_feature_prop_experimenter_delete(of_table_feature_prop_experimenter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_experimenter_miss_t
 * @param obj An instance of type of_table_feature_prop_experimenter_miss_t
 *
 * \ingroup of_table_feature_prop_experimenter_miss
 */
static inline void
of_table_feature_prop_experimenter_miss_delete(of_table_feature_prop_experimenter_miss_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_header_t
 * @param obj An instance of type of_table_feature_prop_header_t
 *
 * \ingroup of_table_feature_prop_header
 */
static inline void
of_table_feature_prop_header_delete(of_table_feature_prop_header_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_instructions_t
 * @param obj An instance of type of_table_feature_prop_instructions_t
 *
 * \ingroup of_table_feature_prop_instructions
 */
static inline void
of_table_feature_prop_instructions_delete(of_table_feature_prop_instructions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_instructions_miss_t
 * @param obj An instance of type of_table_feature_prop_instructions_miss_t
 *
 * \ingroup of_table_feature_prop_instructions_miss
 */
static inline void
of_table_feature_prop_instructions_miss_delete(of_table_feature_prop_instructions_miss_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_match_t
 * @param obj An instance of type of_table_feature_prop_match_t
 *
 * \ingroup of_table_feature_prop_match
 */
static inline void
of_table_feature_prop_match_delete(of_table_feature_prop_match_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_next_tables_t
 * @param obj An instance of type of_table_feature_prop_next_tables_t
 *
 * \ingroup of_table_feature_prop_next_tables
 */
static inline void
of_table_feature_prop_next_tables_delete(of_table_feature_prop_next_tables_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_next_tables_miss_t
 * @param obj An instance of type of_table_feature_prop_next_tables_miss_t
 *
 * \ingroup of_table_feature_prop_next_tables_miss
 */
static inline void
of_table_feature_prop_next_tables_miss_delete(of_table_feature_prop_next_tables_miss_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_wildcards_t
 * @param obj An instance of type of_table_feature_prop_wildcards_t
 *
 * \ingroup of_table_feature_prop_wildcards
 */
static inline void
of_table_feature_prop_wildcards_delete(of_table_feature_prop_wildcards_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_write_actions_t
 * @param obj An instance of type of_table_feature_prop_write_actions_t
 *
 * \ingroup of_table_feature_prop_write_actions
 */
static inline void
of_table_feature_prop_write_actions_delete(of_table_feature_prop_write_actions_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_write_actions_miss_t
 * @param obj An instance of type of_table_feature_prop_write_actions_miss_t
 *
 * \ingroup of_table_feature_prop_write_actions_miss
 */
static inline void
of_table_feature_prop_write_actions_miss_delete(of_table_feature_prop_write_actions_miss_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_write_setfield_t
 * @param obj An instance of type of_table_feature_prop_write_setfield_t
 *
 * \ingroup of_table_feature_prop_write_setfield
 */
static inline void
of_table_feature_prop_write_setfield_delete(of_table_feature_prop_write_setfield_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_feature_prop_write_setfield_miss_t
 * @param obj An instance of type of_table_feature_prop_write_setfield_miss_t
 *
 * \ingroup of_table_feature_prop_write_setfield_miss
 */
static inline void
of_table_feature_prop_write_setfield_miss_delete(of_table_feature_prop_write_setfield_miss_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_features_t
 * @param obj An instance of type of_table_features_t
 *
 * \ingroup of_table_features
 */
static inline void
of_table_features_delete(of_table_features_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_table_stats_entry_t
 * @param obj An instance of type of_table_stats_entry_t
 *
 * \ingroup of_table_stats_entry
 */
static inline void
of_table_stats_entry_delete(of_table_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_uint32_t
 * @param obj An instance of type of_uint32_t
 *
 * \ingroup of_uint32
 */
static inline void
of_uint32_delete(of_uint32_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_uint64_t
 * @param obj An instance of type of_uint64_t
 *
 * \ingroup of_uint64
 */
static inline void
of_uint64_delete(of_uint64_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_uint8_t
 * @param obj An instance of type of_uint8_t
 *
 * \ingroup of_uint8
 */
static inline void
of_uint8_delete(of_uint8_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type ofp_queue_desc_t
 * @param obj An instance of type ofp_queue_desc_t
 *
 * \ingroup ofp_queue_desc
 */
static inline void
ofp_queue_desc_delete(ofp_queue_desc_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type onf_oxm_packet_regs0_t
 * @param obj An instance of type onf_oxm_packet_regs0_t
 *
 * \ingroup onf_oxm_packet_regs0
 */
static inline void
onf_oxm_packet_regs0_delete(onf_oxm_packet_regs0_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type onf_oxm_packet_regs1_t
 * @param obj An instance of type onf_oxm_packet_regs1_t
 *
 * \ingroup onf_oxm_packet_regs1
 */
static inline void
onf_oxm_packet_regs1_delete(onf_oxm_packet_regs1_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type onf_oxm_packet_regs2_t
 * @param obj An instance of type onf_oxm_packet_regs2_t
 *
 * \ingroup onf_oxm_packet_regs2
 */
static inline void
onf_oxm_packet_regs2_delete(onf_oxm_packet_regs2_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type onf_oxm_packet_regs3_t
 * @param obj An instance of type onf_oxm_packet_regs3_t
 *
 * \ingroup onf_oxm_packet_regs3
 */
static inline void
onf_oxm_packet_regs3_delete(onf_oxm_packet_regs3_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_action_t
 * @param obj An instance of type of_list_action_t
 *
 * \ingroup of_list_action
 */
static inline void
of_list_action_delete(of_list_action_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_action_id_t
 * @param obj An instance of type of_list_action_id_t
 *
 * \ingroup of_list_action_id
 */
static inline void
of_list_action_id_delete(of_list_action_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_controller_connection_t
 * @param obj An instance of type of_list_bsn_controller_connection_t
 *
 * \ingroup of_list_bsn_controller_connection
 */
static inline void
of_list_bsn_controller_connection_delete(of_list_bsn_controller_connection_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_debug_counter_desc_stats_entry_t
 * @param obj An instance of type of_list_bsn_debug_counter_desc_stats_entry_t
 *
 * \ingroup of_list_bsn_debug_counter_desc_stats_entry
 */
static inline void
of_list_bsn_debug_counter_desc_stats_entry_delete(of_list_bsn_debug_counter_desc_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_debug_counter_stats_entry_t
 * @param obj An instance of type of_list_bsn_debug_counter_stats_entry_t
 *
 * \ingroup of_list_bsn_debug_counter_stats_entry
 */
static inline void
of_list_bsn_debug_counter_stats_entry_delete(of_list_bsn_debug_counter_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_flow_checksum_bucket_stats_entry_t
 * @param obj An instance of type of_list_bsn_flow_checksum_bucket_stats_entry_t
 *
 * \ingroup of_list_bsn_flow_checksum_bucket_stats_entry
 */
static inline void
of_list_bsn_flow_checksum_bucket_stats_entry_delete(of_list_bsn_flow_checksum_bucket_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_gentable_bucket_stats_entry_t
 * @param obj An instance of type of_list_bsn_gentable_bucket_stats_entry_t
 *
 * \ingroup of_list_bsn_gentable_bucket_stats_entry
 */
static inline void
of_list_bsn_gentable_bucket_stats_entry_delete(of_list_bsn_gentable_bucket_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_gentable_desc_stats_entry_t
 * @param obj An instance of type of_list_bsn_gentable_desc_stats_entry_t
 *
 * \ingroup of_list_bsn_gentable_desc_stats_entry
 */
static inline void
of_list_bsn_gentable_desc_stats_entry_delete(of_list_bsn_gentable_desc_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_gentable_entry_desc_stats_entry_t
 * @param obj An instance of type of_list_bsn_gentable_entry_desc_stats_entry_t
 *
 * \ingroup of_list_bsn_gentable_entry_desc_stats_entry
 */
static inline void
of_list_bsn_gentable_entry_desc_stats_entry_delete(of_list_bsn_gentable_entry_desc_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_gentable_entry_stats_entry_t
 * @param obj An instance of type of_list_bsn_gentable_entry_stats_entry_t
 *
 * \ingroup of_list_bsn_gentable_entry_stats_entry
 */
static inline void
of_list_bsn_gentable_entry_stats_entry_delete(of_list_bsn_gentable_entry_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_gentable_stats_entry_t
 * @param obj An instance of type of_list_bsn_gentable_stats_entry_t
 *
 * \ingroup of_list_bsn_gentable_stats_entry
 */
static inline void
of_list_bsn_gentable_stats_entry_delete(of_list_bsn_gentable_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_interface_t
 * @param obj An instance of type of_list_bsn_interface_t
 *
 * \ingroup of_list_bsn_interface
 */
static inline void
of_list_bsn_interface_delete(of_list_bsn_interface_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_lacp_stats_entry_t
 * @param obj An instance of type of_list_bsn_lacp_stats_entry_t
 *
 * \ingroup of_list_bsn_lacp_stats_entry
 */
static inline void
of_list_bsn_lacp_stats_entry_delete(of_list_bsn_lacp_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_port_counter_stats_entry_t
 * @param obj An instance of type of_list_bsn_port_counter_stats_entry_t
 *
 * \ingroup of_list_bsn_port_counter_stats_entry
 */
static inline void
of_list_bsn_port_counter_stats_entry_delete(of_list_bsn_port_counter_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_switch_pipeline_stats_entry_t
 * @param obj An instance of type of_list_bsn_switch_pipeline_stats_entry_t
 *
 * \ingroup of_list_bsn_switch_pipeline_stats_entry
 */
static inline void
of_list_bsn_switch_pipeline_stats_entry_delete(of_list_bsn_switch_pipeline_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_table_checksum_stats_entry_t
 * @param obj An instance of type of_list_bsn_table_checksum_stats_entry_t
 *
 * \ingroup of_list_bsn_table_checksum_stats_entry
 */
static inline void
of_list_bsn_table_checksum_stats_entry_delete(of_list_bsn_table_checksum_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_tlv_t
 * @param obj An instance of type of_list_bsn_tlv_t
 *
 * \ingroup of_list_bsn_tlv
 */
static inline void
of_list_bsn_tlv_delete(of_list_bsn_tlv_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bsn_vlan_counter_stats_entry_t
 * @param obj An instance of type of_list_bsn_vlan_counter_stats_entry_t
 *
 * \ingroup of_list_bsn_vlan_counter_stats_entry
 */
static inline void
of_list_bsn_vlan_counter_stats_entry_delete(of_list_bsn_vlan_counter_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bucket_t
 * @param obj An instance of type of_list_bucket_t
 *
 * \ingroup of_list_bucket
 */
static inline void
of_list_bucket_delete(of_list_bucket_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_bucket_counter_t
 * @param obj An instance of type of_list_bucket_counter_t
 *
 * \ingroup of_list_bucket_counter
 */
static inline void
of_list_bucket_counter_delete(of_list_bucket_counter_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_flow_stats_entry_t
 * @param obj An instance of type of_list_flow_stats_entry_t
 *
 * \ingroup of_list_flow_stats_entry
 */
static inline void
of_list_flow_stats_entry_delete(of_list_flow_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_group_desc_stats_entry_t
 * @param obj An instance of type of_list_group_desc_stats_entry_t
 *
 * \ingroup of_list_group_desc_stats_entry
 */
static inline void
of_list_group_desc_stats_entry_delete(of_list_group_desc_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_group_stats_entry_t
 * @param obj An instance of type of_list_group_stats_entry_t
 *
 * \ingroup of_list_group_stats_entry
 */
static inline void
of_list_group_stats_entry_delete(of_list_group_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_hello_elem_t
 * @param obj An instance of type of_list_hello_elem_t
 *
 * \ingroup of_list_hello_elem
 */
static inline void
of_list_hello_elem_delete(of_list_hello_elem_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_instruction_t
 * @param obj An instance of type of_list_instruction_t
 *
 * \ingroup of_list_instruction
 */
static inline void
of_list_instruction_delete(of_list_instruction_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_instruction_id_t
 * @param obj An instance of type of_list_instruction_id_t
 *
 * \ingroup of_list_instruction_id
 */
static inline void
of_list_instruction_id_delete(of_list_instruction_id_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_meter_band_t
 * @param obj An instance of type of_list_meter_band_t
 *
 * \ingroup of_list_meter_band
 */
static inline void
of_list_meter_band_delete(of_list_meter_band_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_meter_band_stats_t
 * @param obj An instance of type of_list_meter_band_stats_t
 *
 * \ingroup of_list_meter_band_stats
 */
static inline void
of_list_meter_band_stats_delete(of_list_meter_band_stats_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_meter_stats_t
 * @param obj An instance of type of_list_meter_stats_t
 *
 * \ingroup of_list_meter_stats
 */
static inline void
of_list_meter_stats_delete(of_list_meter_stats_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_oxm_t
 * @param obj An instance of type of_list_oxm_t
 *
 * \ingroup of_list_oxm
 */
static inline void
of_list_oxm_delete(of_list_oxm_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_packet_queue_t
 * @param obj An instance of type of_list_packet_queue_t
 *
 * \ingroup of_list_packet_queue
 */
static inline void
of_list_packet_queue_delete(of_list_packet_queue_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_port_desc_t
 * @param obj An instance of type of_list_port_desc_t
 *
 * \ingroup of_list_port_desc
 */
static inline void
of_list_port_desc_delete(of_list_port_desc_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_port_stats_entry_t
 * @param obj An instance of type of_list_port_stats_entry_t
 *
 * \ingroup of_list_port_stats_entry
 */
static inline void
of_list_port_stats_entry_delete(of_list_port_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_queue_prop_t
 * @param obj An instance of type of_list_queue_prop_t
 *
 * \ingroup of_list_queue_prop
 */
static inline void
of_list_queue_prop_delete(of_list_queue_prop_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_queue_stats_entry_t
 * @param obj An instance of type of_list_queue_stats_entry_t
 *
 * \ingroup of_list_queue_stats_entry
 */
static inline void
of_list_queue_stats_entry_delete(of_list_queue_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_table_feature_prop_t
 * @param obj An instance of type of_list_table_feature_prop_t
 *
 * \ingroup of_list_table_feature_prop
 */
static inline void
of_list_table_feature_prop_delete(of_list_table_feature_prop_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_table_features_t
 * @param obj An instance of type of_list_table_features_t
 *
 * \ingroup of_list_table_features
 */
static inline void
of_list_table_features_delete(of_list_table_features_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_table_stats_entry_t
 * @param obj An instance of type of_list_table_stats_entry_t
 *
 * \ingroup of_list_table_stats_entry
 */
static inline void
of_list_table_stats_entry_delete(of_list_table_stats_entry_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_uint32_t
 * @param obj An instance of type of_list_uint32_t
 *
 * \ingroup of_list_uint32
 */
static inline void
of_list_uint32_delete(of_list_uint32_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_uint64_t
 * @param obj An instance of type of_list_uint64_t
 *
 * \ingroup of_list_uint64
 */
static inline void
of_list_uint64_delete(of_list_uint64_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

/**
 * Delete an object of type of_list_uint8_t
 * @param obj An instance of type of_list_uint8_t
 *
 * \ingroup of_list_uint8
 */
static inline void
of_list_uint8_delete(of_list_uint8_t *obj) {
    of_object_delete((of_object_t *)(obj));
}

typedef void (*of_object_init_f)(of_object_t *obj, of_version_t version,
    int bytes, int clean_wire);
extern const of_object_init_f of_object_init_map[];

/****************************************************************
 *
 * Function pointer initialization functions
 * These are part of the "coerce" type casting for objects
 *
 ****************************************************************/

/****************************************************************
 *
 * Unified, per-member accessor function declarations
 *
 ****************************************************************/

/* Unified accessor functions for of_aggregate_stats_reply */

extern void of_aggregate_stats_reply_xid_set(
    of_aggregate_stats_reply_t *obj,
    uint32_t xid);
extern void of_aggregate_stats_reply_xid_get(
    of_aggregate_stats_reply_t *obj,
    uint32_t *xid);

extern void of_aggregate_stats_reply_flags_set(
    of_aggregate_stats_reply_t *obj,
    uint16_t flags);
extern void of_aggregate_stats_reply_flags_get(
    of_aggregate_stats_reply_t *obj,
    uint16_t *flags);

extern void of_aggregate_stats_reply_packet_count_set(
    of_aggregate_stats_reply_t *obj,
    uint64_t packet_count);
extern void of_aggregate_stats_reply_packet_count_get(
    of_aggregate_stats_reply_t *obj,
    uint64_t *packet_count);

extern void of_aggregate_stats_reply_byte_count_set(
    of_aggregate_stats_reply_t *obj,
    uint64_t byte_count);
extern void of_aggregate_stats_reply_byte_count_get(
    of_aggregate_stats_reply_t *obj,
    uint64_t *byte_count);

extern void of_aggregate_stats_reply_flow_count_set(
    of_aggregate_stats_reply_t *obj,
    uint32_t flow_count);
extern void of_aggregate_stats_reply_flow_count_get(
    of_aggregate_stats_reply_t *obj,
    uint32_t *flow_count);

/* Unified accessor functions for of_aggregate_stats_request */

extern void of_aggregate_stats_request_xid_set(
    of_aggregate_stats_request_t *obj,
    uint32_t xid);
extern void of_aggregate_stats_request_xid_get(
    of_aggregate_stats_request_t *obj,
    uint32_t *xid);

extern void of_aggregate_stats_request_flags_set(
    of_aggregate_stats_request_t *obj,
    uint16_t flags);
extern void of_aggregate_stats_request_flags_get(
    of_aggregate_stats_request_t *obj,
    uint16_t *flags);

extern void of_aggregate_stats_request_table_id_set(
    of_aggregate_stats_request_t *obj,
    uint8_t table_id);
extern void of_aggregate_stats_request_table_id_get(
    of_aggregate_stats_request_t *obj,
    uint8_t *table_id);

extern void of_aggregate_stats_request_out_port_set(
    of_aggregate_stats_request_t *obj,
    of_port_no_t out_port);
extern void of_aggregate_stats_request_out_port_get(
    of_aggregate_stats_request_t *obj,
    of_port_no_t *out_port);

extern void of_aggregate_stats_request_out_group_set(
    of_aggregate_stats_request_t *obj,
    uint32_t out_group);
extern void of_aggregate_stats_request_out_group_get(
    of_aggregate_stats_request_t *obj,
    uint32_t *out_group);

extern void of_aggregate_stats_request_cookie_set(
    of_aggregate_stats_request_t *obj,
    uint64_t cookie);
extern void of_aggregate_stats_request_cookie_get(
    of_aggregate_stats_request_t *obj,
    uint64_t *cookie);

extern void of_aggregate_stats_request_cookie_mask_set(
    of_aggregate_stats_request_t *obj,
    uint64_t cookie_mask);
extern void of_aggregate_stats_request_cookie_mask_get(
    of_aggregate_stats_request_t *obj,
    uint64_t *cookie_mask);

extern int WARN_UNUSED_RESULT of_aggregate_stats_request_match_set(
    of_aggregate_stats_request_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_aggregate_stats_request_match_get(
    of_aggregate_stats_request_t *obj,
    of_match_t *match);

/* Unified accessor functions for of_async_get_reply */

extern void of_async_get_reply_xid_set(
    of_async_get_reply_t *obj,
    uint32_t xid);
extern void of_async_get_reply_xid_get(
    of_async_get_reply_t *obj,
    uint32_t *xid);

extern void of_async_get_reply_packet_in_mask_equal_master_set(
    of_async_get_reply_t *obj,
    uint32_t packet_in_mask_equal_master);
extern void of_async_get_reply_packet_in_mask_equal_master_get(
    of_async_get_reply_t *obj,
    uint32_t *packet_in_mask_equal_master);

extern void of_async_get_reply_packet_in_mask_slave_set(
    of_async_get_reply_t *obj,
    uint32_t packet_in_mask_slave);
extern void of_async_get_reply_packet_in_mask_slave_get(
    of_async_get_reply_t *obj,
    uint32_t *packet_in_mask_slave);

extern void of_async_get_reply_port_status_mask_equal_master_set(
    of_async_get_reply_t *obj,
    uint32_t port_status_mask_equal_master);
extern void of_async_get_reply_port_status_mask_equal_master_get(
    of_async_get_reply_t *obj,
    uint32_t *port_status_mask_equal_master);

extern void of_async_get_reply_port_status_mask_slave_set(
    of_async_get_reply_t *obj,
    uint32_t port_status_mask_slave);
extern void of_async_get_reply_port_status_mask_slave_get(
    of_async_get_reply_t *obj,
    uint32_t *port_status_mask_slave);

extern void of_async_get_reply_flow_removed_mask_equal_master_set(
    of_async_get_reply_t *obj,
    uint32_t flow_removed_mask_equal_master);
extern void of_async_get_reply_flow_removed_mask_equal_master_get(
    of_async_get_reply_t *obj,
    uint32_t *flow_removed_mask_equal_master);

extern void of_async_get_reply_flow_removed_mask_slave_set(
    of_async_get_reply_t *obj,
    uint32_t flow_removed_mask_slave);
extern void of_async_get_reply_flow_removed_mask_slave_get(
    of_async_get_reply_t *obj,
    uint32_t *flow_removed_mask_slave);

/* Unified accessor functions for of_async_get_request */

extern void of_async_get_request_xid_set(
    of_async_get_request_t *obj,
    uint32_t xid);
extern void of_async_get_request_xid_get(
    of_async_get_request_t *obj,
    uint32_t *xid);

extern void of_async_get_request_packet_in_mask_equal_master_set(
    of_async_get_request_t *obj,
    uint32_t packet_in_mask_equal_master);
extern void of_async_get_request_packet_in_mask_equal_master_get(
    of_async_get_request_t *obj,
    uint32_t *packet_in_mask_equal_master);

extern void of_async_get_request_packet_in_mask_slave_set(
    of_async_get_request_t *obj,
    uint32_t packet_in_mask_slave);
extern void of_async_get_request_packet_in_mask_slave_get(
    of_async_get_request_t *obj,
    uint32_t *packet_in_mask_slave);

extern void of_async_get_request_port_status_mask_equal_master_set(
    of_async_get_request_t *obj,
    uint32_t port_status_mask_equal_master);
extern void of_async_get_request_port_status_mask_equal_master_get(
    of_async_get_request_t *obj,
    uint32_t *port_status_mask_equal_master);

extern void of_async_get_request_port_status_mask_slave_set(
    of_async_get_request_t *obj,
    uint32_t port_status_mask_slave);
extern void of_async_get_request_port_status_mask_slave_get(
    of_async_get_request_t *obj,
    uint32_t *port_status_mask_slave);

extern void of_async_get_request_flow_removed_mask_equal_master_set(
    of_async_get_request_t *obj,
    uint32_t flow_removed_mask_equal_master);
extern void of_async_get_request_flow_removed_mask_equal_master_get(
    of_async_get_request_t *obj,
    uint32_t *flow_removed_mask_equal_master);

extern void of_async_get_request_flow_removed_mask_slave_set(
    of_async_get_request_t *obj,
    uint32_t flow_removed_mask_slave);
extern void of_async_get_request_flow_removed_mask_slave_get(
    of_async_get_request_t *obj,
    uint32_t *flow_removed_mask_slave);

/* Unified accessor functions for of_async_set */

extern void of_async_set_xid_set(
    of_async_set_t *obj,
    uint32_t xid);
extern void of_async_set_xid_get(
    of_async_set_t *obj,
    uint32_t *xid);

extern void of_async_set_packet_in_mask_equal_master_set(
    of_async_set_t *obj,
    uint32_t packet_in_mask_equal_master);
extern void of_async_set_packet_in_mask_equal_master_get(
    of_async_set_t *obj,
    uint32_t *packet_in_mask_equal_master);

extern void of_async_set_packet_in_mask_slave_set(
    of_async_set_t *obj,
    uint32_t packet_in_mask_slave);
extern void of_async_set_packet_in_mask_slave_get(
    of_async_set_t *obj,
    uint32_t *packet_in_mask_slave);

extern void of_async_set_port_status_mask_equal_master_set(
    of_async_set_t *obj,
    uint32_t port_status_mask_equal_master);
extern void of_async_set_port_status_mask_equal_master_get(
    of_async_set_t *obj,
    uint32_t *port_status_mask_equal_master);

extern void of_async_set_port_status_mask_slave_set(
    of_async_set_t *obj,
    uint32_t port_status_mask_slave);
extern void of_async_set_port_status_mask_slave_get(
    of_async_set_t *obj,
    uint32_t *port_status_mask_slave);

extern void of_async_set_flow_removed_mask_equal_master_set(
    of_async_set_t *obj,
    uint32_t flow_removed_mask_equal_master);
extern void of_async_set_flow_removed_mask_equal_master_get(
    of_async_set_t *obj,
    uint32_t *flow_removed_mask_equal_master);

extern void of_async_set_flow_removed_mask_slave_set(
    of_async_set_t *obj,
    uint32_t flow_removed_mask_slave);
extern void of_async_set_flow_removed_mask_slave_get(
    of_async_set_t *obj,
    uint32_t *flow_removed_mask_slave);

/* Unified accessor functions for of_bad_action_error_msg */

extern void of_bad_action_error_msg_xid_set(
    of_bad_action_error_msg_t *obj,
    uint32_t xid);
extern void of_bad_action_error_msg_xid_get(
    of_bad_action_error_msg_t *obj,
    uint32_t *xid);

extern void of_bad_action_error_msg_code_set(
    of_bad_action_error_msg_t *obj,
    uint16_t code);
extern void of_bad_action_error_msg_code_get(
    of_bad_action_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_bad_action_error_msg_data_set(
    of_bad_action_error_msg_t *obj,
    of_octets_t *data);
extern void of_bad_action_error_msg_data_get(
    of_bad_action_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_bad_instruction_error_msg */

extern void of_bad_instruction_error_msg_xid_set(
    of_bad_instruction_error_msg_t *obj,
    uint32_t xid);
extern void of_bad_instruction_error_msg_xid_get(
    of_bad_instruction_error_msg_t *obj,
    uint32_t *xid);

extern void of_bad_instruction_error_msg_code_set(
    of_bad_instruction_error_msg_t *obj,
    uint16_t code);
extern void of_bad_instruction_error_msg_code_get(
    of_bad_instruction_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_bad_instruction_error_msg_data_set(
    of_bad_instruction_error_msg_t *obj,
    of_octets_t *data);
extern void of_bad_instruction_error_msg_data_get(
    of_bad_instruction_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_bad_match_error_msg */

extern void of_bad_match_error_msg_xid_set(
    of_bad_match_error_msg_t *obj,
    uint32_t xid);
extern void of_bad_match_error_msg_xid_get(
    of_bad_match_error_msg_t *obj,
    uint32_t *xid);

extern void of_bad_match_error_msg_code_set(
    of_bad_match_error_msg_t *obj,
    uint16_t code);
extern void of_bad_match_error_msg_code_get(
    of_bad_match_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_bad_match_error_msg_data_set(
    of_bad_match_error_msg_t *obj,
    of_octets_t *data);
extern void of_bad_match_error_msg_data_get(
    of_bad_match_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_bad_request_error_msg */

extern void of_bad_request_error_msg_xid_set(
    of_bad_request_error_msg_t *obj,
    uint32_t xid);
extern void of_bad_request_error_msg_xid_get(
    of_bad_request_error_msg_t *obj,
    uint32_t *xid);

extern void of_bad_request_error_msg_code_set(
    of_bad_request_error_msg_t *obj,
    uint16_t code);
extern void of_bad_request_error_msg_code_get(
    of_bad_request_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_bad_request_error_msg_data_set(
    of_bad_request_error_msg_t *obj,
    of_octets_t *data);
extern void of_bad_request_error_msg_data_get(
    of_bad_request_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_barrier_reply */

extern void of_barrier_reply_xid_set(
    of_barrier_reply_t *obj,
    uint32_t xid);
extern void of_barrier_reply_xid_get(
    of_barrier_reply_t *obj,
    uint32_t *xid);

/* Unified accessor functions for of_barrier_request */

extern void of_barrier_request_xid_set(
    of_barrier_request_t *obj,
    uint32_t xid);
extern void of_barrier_request_xid_get(
    of_barrier_request_t *obj,
    uint32_t *xid);

/* Unified accessor functions for of_bsn_arp_idle */

extern void of_bsn_arp_idle_xid_set(
    of_bsn_arp_idle_t *obj,
    uint32_t xid);
extern void of_bsn_arp_idle_xid_get(
    of_bsn_arp_idle_t *obj,
    uint32_t *xid);

extern void of_bsn_arp_idle_experimenter_set(
    of_bsn_arp_idle_t *obj,
    uint32_t experimenter);
extern void of_bsn_arp_idle_experimenter_get(
    of_bsn_arp_idle_t *obj,
    uint32_t *experimenter);

extern void of_bsn_arp_idle_subtype_set(
    of_bsn_arp_idle_t *obj,
    uint32_t subtype);
extern void of_bsn_arp_idle_subtype_get(
    of_bsn_arp_idle_t *obj,
    uint32_t *subtype);

extern void of_bsn_arp_idle_vlan_vid_set(
    of_bsn_arp_idle_t *obj,
    uint16_t vlan_vid);
extern void of_bsn_arp_idle_vlan_vid_get(
    of_bsn_arp_idle_t *obj,
    uint16_t *vlan_vid);

extern void of_bsn_arp_idle_ipv4_addr_set(
    of_bsn_arp_idle_t *obj,
    of_ipv4_t ipv4_addr);
extern void of_bsn_arp_idle_ipv4_addr_get(
    of_bsn_arp_idle_t *obj,
    of_ipv4_t *ipv4_addr);

/* Unified accessor functions for of_bsn_bw_clear_data_reply */

extern void of_bsn_bw_clear_data_reply_xid_set(
    of_bsn_bw_clear_data_reply_t *obj,
    uint32_t xid);
extern void of_bsn_bw_clear_data_reply_xid_get(
    of_bsn_bw_clear_data_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_bw_clear_data_reply_experimenter_set(
    of_bsn_bw_clear_data_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_bw_clear_data_reply_experimenter_get(
    of_bsn_bw_clear_data_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_bw_clear_data_reply_subtype_set(
    of_bsn_bw_clear_data_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_bw_clear_data_reply_subtype_get(
    of_bsn_bw_clear_data_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_bw_clear_data_reply_status_set(
    of_bsn_bw_clear_data_reply_t *obj,
    uint32_t status);
extern void of_bsn_bw_clear_data_reply_status_get(
    of_bsn_bw_clear_data_reply_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_bw_clear_data_request */

extern void of_bsn_bw_clear_data_request_xid_set(
    of_bsn_bw_clear_data_request_t *obj,
    uint32_t xid);
extern void of_bsn_bw_clear_data_request_xid_get(
    of_bsn_bw_clear_data_request_t *obj,
    uint32_t *xid);

extern void of_bsn_bw_clear_data_request_experimenter_set(
    of_bsn_bw_clear_data_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_bw_clear_data_request_experimenter_get(
    of_bsn_bw_clear_data_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_bw_clear_data_request_subtype_set(
    of_bsn_bw_clear_data_request_t *obj,
    uint32_t subtype);
extern void of_bsn_bw_clear_data_request_subtype_get(
    of_bsn_bw_clear_data_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_bw_enable_get_reply */

extern void of_bsn_bw_enable_get_reply_xid_set(
    of_bsn_bw_enable_get_reply_t *obj,
    uint32_t xid);
extern void of_bsn_bw_enable_get_reply_xid_get(
    of_bsn_bw_enable_get_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_bw_enable_get_reply_experimenter_set(
    of_bsn_bw_enable_get_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_bw_enable_get_reply_experimenter_get(
    of_bsn_bw_enable_get_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_bw_enable_get_reply_subtype_set(
    of_bsn_bw_enable_get_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_bw_enable_get_reply_subtype_get(
    of_bsn_bw_enable_get_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_bw_enable_get_reply_enabled_set(
    of_bsn_bw_enable_get_reply_t *obj,
    uint32_t enabled);
extern void of_bsn_bw_enable_get_reply_enabled_get(
    of_bsn_bw_enable_get_reply_t *obj,
    uint32_t *enabled);

/* Unified accessor functions for of_bsn_bw_enable_get_request */

extern void of_bsn_bw_enable_get_request_xid_set(
    of_bsn_bw_enable_get_request_t *obj,
    uint32_t xid);
extern void of_bsn_bw_enable_get_request_xid_get(
    of_bsn_bw_enable_get_request_t *obj,
    uint32_t *xid);

extern void of_bsn_bw_enable_get_request_experimenter_set(
    of_bsn_bw_enable_get_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_bw_enable_get_request_experimenter_get(
    of_bsn_bw_enable_get_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_bw_enable_get_request_subtype_set(
    of_bsn_bw_enable_get_request_t *obj,
    uint32_t subtype);
extern void of_bsn_bw_enable_get_request_subtype_get(
    of_bsn_bw_enable_get_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_bw_enable_set_reply */

extern void of_bsn_bw_enable_set_reply_xid_set(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t xid);
extern void of_bsn_bw_enable_set_reply_xid_get(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_bw_enable_set_reply_experimenter_set(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_bw_enable_set_reply_experimenter_get(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_bw_enable_set_reply_subtype_set(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_bw_enable_set_reply_subtype_get(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_bw_enable_set_reply_enable_set(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t enable);
extern void of_bsn_bw_enable_set_reply_enable_get(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t *enable);

extern void of_bsn_bw_enable_set_reply_status_set(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t status);
extern void of_bsn_bw_enable_set_reply_status_get(
    of_bsn_bw_enable_set_reply_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_bw_enable_set_request */

extern void of_bsn_bw_enable_set_request_xid_set(
    of_bsn_bw_enable_set_request_t *obj,
    uint32_t xid);
extern void of_bsn_bw_enable_set_request_xid_get(
    of_bsn_bw_enable_set_request_t *obj,
    uint32_t *xid);

extern void of_bsn_bw_enable_set_request_experimenter_set(
    of_bsn_bw_enable_set_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_bw_enable_set_request_experimenter_get(
    of_bsn_bw_enable_set_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_bw_enable_set_request_subtype_set(
    of_bsn_bw_enable_set_request_t *obj,
    uint32_t subtype);
extern void of_bsn_bw_enable_set_request_subtype_get(
    of_bsn_bw_enable_set_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_bw_enable_set_request_enable_set(
    of_bsn_bw_enable_set_request_t *obj,
    uint32_t enable);
extern void of_bsn_bw_enable_set_request_enable_get(
    of_bsn_bw_enable_set_request_t *obj,
    uint32_t *enable);

/* Unified accessor functions for of_bsn_controller_connections_reply */

extern void of_bsn_controller_connections_reply_xid_set(
    of_bsn_controller_connections_reply_t *obj,
    uint32_t xid);
extern void of_bsn_controller_connections_reply_xid_get(
    of_bsn_controller_connections_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_controller_connections_reply_experimenter_set(
    of_bsn_controller_connections_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_controller_connections_reply_experimenter_get(
    of_bsn_controller_connections_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_controller_connections_reply_subtype_set(
    of_bsn_controller_connections_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_controller_connections_reply_subtype_get(
    of_bsn_controller_connections_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_controller_connections_reply_connections_set(
    of_bsn_controller_connections_reply_t *obj,
    of_list_bsn_controller_connection_t *connections);
extern void of_bsn_controller_connections_reply_connections_bind(
    of_bsn_controller_connections_reply_t *obj,
    of_list_bsn_controller_connection_t *connections);
extern of_list_bsn_controller_connection_t *of_bsn_controller_connections_reply_connections_get(
    of_bsn_controller_connections_reply_t *obj);

/* Unified accessor functions for of_bsn_controller_connections_request */

extern void of_bsn_controller_connections_request_xid_set(
    of_bsn_controller_connections_request_t *obj,
    uint32_t xid);
extern void of_bsn_controller_connections_request_xid_get(
    of_bsn_controller_connections_request_t *obj,
    uint32_t *xid);

extern void of_bsn_controller_connections_request_experimenter_set(
    of_bsn_controller_connections_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_controller_connections_request_experimenter_get(
    of_bsn_controller_connections_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_controller_connections_request_subtype_set(
    of_bsn_controller_connections_request_t *obj,
    uint32_t subtype);
extern void of_bsn_controller_connections_request_subtype_get(
    of_bsn_controller_connections_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_debug_counter_desc_stats_reply */

extern void of_bsn_debug_counter_desc_stats_reply_xid_set(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_debug_counter_desc_stats_reply_xid_get(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_debug_counter_desc_stats_reply_flags_set(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_debug_counter_desc_stats_reply_flags_get(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_debug_counter_desc_stats_reply_experimenter_set(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_debug_counter_desc_stats_reply_experimenter_get(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_debug_counter_desc_stats_reply_subtype_set(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_debug_counter_desc_stats_reply_subtype_get(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_debug_counter_desc_stats_reply_entries_set(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    of_list_bsn_debug_counter_desc_stats_entry_t *entries);
extern void of_bsn_debug_counter_desc_stats_reply_entries_bind(
    of_bsn_debug_counter_desc_stats_reply_t *obj,
    of_list_bsn_debug_counter_desc_stats_entry_t *entries);
extern of_list_bsn_debug_counter_desc_stats_entry_t *of_bsn_debug_counter_desc_stats_reply_entries_get(
    of_bsn_debug_counter_desc_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_debug_counter_desc_stats_request */

extern void of_bsn_debug_counter_desc_stats_request_xid_set(
    of_bsn_debug_counter_desc_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_debug_counter_desc_stats_request_xid_get(
    of_bsn_debug_counter_desc_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_debug_counter_desc_stats_request_flags_set(
    of_bsn_debug_counter_desc_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_debug_counter_desc_stats_request_flags_get(
    of_bsn_debug_counter_desc_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_debug_counter_desc_stats_request_experimenter_set(
    of_bsn_debug_counter_desc_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_debug_counter_desc_stats_request_experimenter_get(
    of_bsn_debug_counter_desc_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_debug_counter_desc_stats_request_subtype_set(
    of_bsn_debug_counter_desc_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_debug_counter_desc_stats_request_subtype_get(
    of_bsn_debug_counter_desc_stats_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_debug_counter_stats_reply */

extern void of_bsn_debug_counter_stats_reply_xid_set(
    of_bsn_debug_counter_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_debug_counter_stats_reply_xid_get(
    of_bsn_debug_counter_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_debug_counter_stats_reply_flags_set(
    of_bsn_debug_counter_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_debug_counter_stats_reply_flags_get(
    of_bsn_debug_counter_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_debug_counter_stats_reply_experimenter_set(
    of_bsn_debug_counter_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_debug_counter_stats_reply_experimenter_get(
    of_bsn_debug_counter_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_debug_counter_stats_reply_subtype_set(
    of_bsn_debug_counter_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_debug_counter_stats_reply_subtype_get(
    of_bsn_debug_counter_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_debug_counter_stats_reply_entries_set(
    of_bsn_debug_counter_stats_reply_t *obj,
    of_list_bsn_debug_counter_stats_entry_t *entries);
extern void of_bsn_debug_counter_stats_reply_entries_bind(
    of_bsn_debug_counter_stats_reply_t *obj,
    of_list_bsn_debug_counter_stats_entry_t *entries);
extern of_list_bsn_debug_counter_stats_entry_t *of_bsn_debug_counter_stats_reply_entries_get(
    of_bsn_debug_counter_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_debug_counter_stats_request */

extern void of_bsn_debug_counter_stats_request_xid_set(
    of_bsn_debug_counter_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_debug_counter_stats_request_xid_get(
    of_bsn_debug_counter_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_debug_counter_stats_request_flags_set(
    of_bsn_debug_counter_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_debug_counter_stats_request_flags_get(
    of_bsn_debug_counter_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_debug_counter_stats_request_experimenter_set(
    of_bsn_debug_counter_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_debug_counter_stats_request_experimenter_get(
    of_bsn_debug_counter_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_debug_counter_stats_request_subtype_set(
    of_bsn_debug_counter_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_debug_counter_stats_request_subtype_get(
    of_bsn_debug_counter_stats_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_flow_checksum_bucket_stats_reply */

extern void of_bsn_flow_checksum_bucket_stats_reply_xid_set(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_flow_checksum_bucket_stats_reply_xid_get(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_flow_checksum_bucket_stats_reply_flags_set(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_flow_checksum_bucket_stats_reply_flags_get(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_flow_checksum_bucket_stats_reply_experimenter_set(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_flow_checksum_bucket_stats_reply_experimenter_get(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_flow_checksum_bucket_stats_reply_subtype_set(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_flow_checksum_bucket_stats_reply_subtype_get(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_flow_checksum_bucket_stats_reply_entries_set(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    of_list_bsn_flow_checksum_bucket_stats_entry_t *entries);
extern void of_bsn_flow_checksum_bucket_stats_reply_entries_bind(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj,
    of_list_bsn_flow_checksum_bucket_stats_entry_t *entries);
extern of_list_bsn_flow_checksum_bucket_stats_entry_t *of_bsn_flow_checksum_bucket_stats_reply_entries_get(
    of_bsn_flow_checksum_bucket_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_flow_checksum_bucket_stats_request */

extern void of_bsn_flow_checksum_bucket_stats_request_xid_set(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_flow_checksum_bucket_stats_request_xid_get(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_flow_checksum_bucket_stats_request_flags_set(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_flow_checksum_bucket_stats_request_flags_get(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_flow_checksum_bucket_stats_request_experimenter_set(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_flow_checksum_bucket_stats_request_experimenter_get(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_flow_checksum_bucket_stats_request_subtype_set(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_flow_checksum_bucket_stats_request_subtype_get(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_flow_checksum_bucket_stats_request_table_id_set(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint8_t table_id);
extern void of_bsn_flow_checksum_bucket_stats_request_table_id_get(
    of_bsn_flow_checksum_bucket_stats_request_t *obj,
    uint8_t *table_id);

/* Unified accessor functions for of_bsn_flow_idle */

extern void of_bsn_flow_idle_xid_set(
    of_bsn_flow_idle_t *obj,
    uint32_t xid);
extern void of_bsn_flow_idle_xid_get(
    of_bsn_flow_idle_t *obj,
    uint32_t *xid);

extern void of_bsn_flow_idle_experimenter_set(
    of_bsn_flow_idle_t *obj,
    uint32_t experimenter);
extern void of_bsn_flow_idle_experimenter_get(
    of_bsn_flow_idle_t *obj,
    uint32_t *experimenter);

extern void of_bsn_flow_idle_subtype_set(
    of_bsn_flow_idle_t *obj,
    uint32_t subtype);
extern void of_bsn_flow_idle_subtype_get(
    of_bsn_flow_idle_t *obj,
    uint32_t *subtype);

extern void of_bsn_flow_idle_cookie_set(
    of_bsn_flow_idle_t *obj,
    uint64_t cookie);
extern void of_bsn_flow_idle_cookie_get(
    of_bsn_flow_idle_t *obj,
    uint64_t *cookie);

extern void of_bsn_flow_idle_priority_set(
    of_bsn_flow_idle_t *obj,
    uint16_t priority);
extern void of_bsn_flow_idle_priority_get(
    of_bsn_flow_idle_t *obj,
    uint16_t *priority);

extern void of_bsn_flow_idle_table_id_set(
    of_bsn_flow_idle_t *obj,
    uint8_t table_id);
extern void of_bsn_flow_idle_table_id_get(
    of_bsn_flow_idle_t *obj,
    uint8_t *table_id);

extern int WARN_UNUSED_RESULT of_bsn_flow_idle_match_set(
    of_bsn_flow_idle_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_bsn_flow_idle_match_get(
    of_bsn_flow_idle_t *obj,
    of_match_t *match);

/* Unified accessor functions for of_bsn_flow_idle_enable_get_reply */

extern void of_bsn_flow_idle_enable_get_reply_xid_set(
    of_bsn_flow_idle_enable_get_reply_t *obj,
    uint32_t xid);
extern void of_bsn_flow_idle_enable_get_reply_xid_get(
    of_bsn_flow_idle_enable_get_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_flow_idle_enable_get_reply_experimenter_set(
    of_bsn_flow_idle_enable_get_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_flow_idle_enable_get_reply_experimenter_get(
    of_bsn_flow_idle_enable_get_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_flow_idle_enable_get_reply_subtype_set(
    of_bsn_flow_idle_enable_get_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_flow_idle_enable_get_reply_subtype_get(
    of_bsn_flow_idle_enable_get_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_flow_idle_enable_get_reply_enabled_set(
    of_bsn_flow_idle_enable_get_reply_t *obj,
    uint32_t enabled);
extern void of_bsn_flow_idle_enable_get_reply_enabled_get(
    of_bsn_flow_idle_enable_get_reply_t *obj,
    uint32_t *enabled);

/* Unified accessor functions for of_bsn_flow_idle_enable_get_request */

extern void of_bsn_flow_idle_enable_get_request_xid_set(
    of_bsn_flow_idle_enable_get_request_t *obj,
    uint32_t xid);
extern void of_bsn_flow_idle_enable_get_request_xid_get(
    of_bsn_flow_idle_enable_get_request_t *obj,
    uint32_t *xid);

extern void of_bsn_flow_idle_enable_get_request_experimenter_set(
    of_bsn_flow_idle_enable_get_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_flow_idle_enable_get_request_experimenter_get(
    of_bsn_flow_idle_enable_get_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_flow_idle_enable_get_request_subtype_set(
    of_bsn_flow_idle_enable_get_request_t *obj,
    uint32_t subtype);
extern void of_bsn_flow_idle_enable_get_request_subtype_get(
    of_bsn_flow_idle_enable_get_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_flow_idle_enable_set_reply */

extern void of_bsn_flow_idle_enable_set_reply_xid_set(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t xid);
extern void of_bsn_flow_idle_enable_set_reply_xid_get(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_flow_idle_enable_set_reply_experimenter_set(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_flow_idle_enable_set_reply_experimenter_get(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_flow_idle_enable_set_reply_subtype_set(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_flow_idle_enable_set_reply_subtype_get(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_flow_idle_enable_set_reply_enable_set(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t enable);
extern void of_bsn_flow_idle_enable_set_reply_enable_get(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t *enable);

extern void of_bsn_flow_idle_enable_set_reply_status_set(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t status);
extern void of_bsn_flow_idle_enable_set_reply_status_get(
    of_bsn_flow_idle_enable_set_reply_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_flow_idle_enable_set_request */

extern void of_bsn_flow_idle_enable_set_request_xid_set(
    of_bsn_flow_idle_enable_set_request_t *obj,
    uint32_t xid);
extern void of_bsn_flow_idle_enable_set_request_xid_get(
    of_bsn_flow_idle_enable_set_request_t *obj,
    uint32_t *xid);

extern void of_bsn_flow_idle_enable_set_request_experimenter_set(
    of_bsn_flow_idle_enable_set_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_flow_idle_enable_set_request_experimenter_get(
    of_bsn_flow_idle_enable_set_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_flow_idle_enable_set_request_subtype_set(
    of_bsn_flow_idle_enable_set_request_t *obj,
    uint32_t subtype);
extern void of_bsn_flow_idle_enable_set_request_subtype_get(
    of_bsn_flow_idle_enable_set_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_flow_idle_enable_set_request_enable_set(
    of_bsn_flow_idle_enable_set_request_t *obj,
    uint32_t enable);
extern void of_bsn_flow_idle_enable_set_request_enable_get(
    of_bsn_flow_idle_enable_set_request_t *obj,
    uint32_t *enable);

/* Unified accessor functions for of_bsn_gentable_bucket_stats_reply */

extern void of_bsn_gentable_bucket_stats_reply_xid_set(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_bucket_stats_reply_xid_get(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_bucket_stats_reply_flags_set(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_bucket_stats_reply_flags_get(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_bucket_stats_reply_experimenter_set(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_bucket_stats_reply_experimenter_get(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_bucket_stats_reply_subtype_set(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_bucket_stats_reply_subtype_get(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_gentable_bucket_stats_reply_entries_set(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    of_list_bsn_gentable_bucket_stats_entry_t *entries);
extern void of_bsn_gentable_bucket_stats_reply_entries_bind(
    of_bsn_gentable_bucket_stats_reply_t *obj,
    of_list_bsn_gentable_bucket_stats_entry_t *entries);
extern of_list_bsn_gentable_bucket_stats_entry_t *of_bsn_gentable_bucket_stats_reply_entries_get(
    of_bsn_gentable_bucket_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_gentable_bucket_stats_request */

extern void of_bsn_gentable_bucket_stats_request_xid_set(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_bucket_stats_request_xid_get(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_bucket_stats_request_flags_set(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_bucket_stats_request_flags_get(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_bucket_stats_request_experimenter_set(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_bucket_stats_request_experimenter_get(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_bucket_stats_request_subtype_set(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_bucket_stats_request_subtype_get(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_gentable_bucket_stats_request_table_id_set(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_bucket_stats_request_table_id_get(
    of_bsn_gentable_bucket_stats_request_t *obj,
    uint16_t *table_id);

/* Unified accessor functions for of_bsn_gentable_clear_reply */

extern void of_bsn_gentable_clear_reply_xid_set(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_clear_reply_xid_get(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_clear_reply_experimenter_set(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_clear_reply_experimenter_get(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_clear_reply_subtype_set(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_clear_reply_subtype_get(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_gentable_clear_reply_table_id_set(
    of_bsn_gentable_clear_reply_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_clear_reply_table_id_get(
    of_bsn_gentable_clear_reply_t *obj,
    uint16_t *table_id);

extern void of_bsn_gentable_clear_reply_deleted_count_set(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t deleted_count);
extern void of_bsn_gentable_clear_reply_deleted_count_get(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t *deleted_count);

extern void of_bsn_gentable_clear_reply_error_count_set(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t error_count);
extern void of_bsn_gentable_clear_reply_error_count_get(
    of_bsn_gentable_clear_reply_t *obj,
    uint32_t *error_count);

/* Unified accessor functions for of_bsn_gentable_clear_request */

extern void of_bsn_gentable_clear_request_xid_set(
    of_bsn_gentable_clear_request_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_clear_request_xid_get(
    of_bsn_gentable_clear_request_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_clear_request_experimenter_set(
    of_bsn_gentable_clear_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_clear_request_experimenter_get(
    of_bsn_gentable_clear_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_clear_request_subtype_set(
    of_bsn_gentable_clear_request_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_clear_request_subtype_get(
    of_bsn_gentable_clear_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_gentable_clear_request_table_id_set(
    of_bsn_gentable_clear_request_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_clear_request_table_id_get(
    of_bsn_gentable_clear_request_t *obj,
    uint16_t *table_id);

extern void of_bsn_gentable_clear_request_checksum_set(
    of_bsn_gentable_clear_request_t *obj,
    of_checksum_128_t checksum);
extern void of_bsn_gentable_clear_request_checksum_get(
    of_bsn_gentable_clear_request_t *obj,
    of_checksum_128_t *checksum);

extern void of_bsn_gentable_clear_request_checksum_mask_set(
    of_bsn_gentable_clear_request_t *obj,
    of_checksum_128_t checksum_mask);
extern void of_bsn_gentable_clear_request_checksum_mask_get(
    of_bsn_gentable_clear_request_t *obj,
    of_checksum_128_t *checksum_mask);

/* Unified accessor functions for of_bsn_gentable_desc_stats_reply */

extern void of_bsn_gentable_desc_stats_reply_xid_set(
    of_bsn_gentable_desc_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_desc_stats_reply_xid_get(
    of_bsn_gentable_desc_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_desc_stats_reply_flags_set(
    of_bsn_gentable_desc_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_desc_stats_reply_flags_get(
    of_bsn_gentable_desc_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_desc_stats_reply_experimenter_set(
    of_bsn_gentable_desc_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_desc_stats_reply_experimenter_get(
    of_bsn_gentable_desc_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_desc_stats_reply_subtype_set(
    of_bsn_gentable_desc_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_desc_stats_reply_subtype_get(
    of_bsn_gentable_desc_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_gentable_desc_stats_reply_entries_set(
    of_bsn_gentable_desc_stats_reply_t *obj,
    of_list_bsn_gentable_desc_stats_entry_t *entries);
extern void of_bsn_gentable_desc_stats_reply_entries_bind(
    of_bsn_gentable_desc_stats_reply_t *obj,
    of_list_bsn_gentable_desc_stats_entry_t *entries);
extern of_list_bsn_gentable_desc_stats_entry_t *of_bsn_gentable_desc_stats_reply_entries_get(
    of_bsn_gentable_desc_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_gentable_desc_stats_request */

extern void of_bsn_gentable_desc_stats_request_xid_set(
    of_bsn_gentable_desc_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_desc_stats_request_xid_get(
    of_bsn_gentable_desc_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_desc_stats_request_flags_set(
    of_bsn_gentable_desc_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_desc_stats_request_flags_get(
    of_bsn_gentable_desc_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_desc_stats_request_experimenter_set(
    of_bsn_gentable_desc_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_desc_stats_request_experimenter_get(
    of_bsn_gentable_desc_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_desc_stats_request_subtype_set(
    of_bsn_gentable_desc_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_desc_stats_request_subtype_get(
    of_bsn_gentable_desc_stats_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_gentable_entry_add */

extern void of_bsn_gentable_entry_add_xid_set(
    of_bsn_gentable_entry_add_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_entry_add_xid_get(
    of_bsn_gentable_entry_add_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_entry_add_experimenter_set(
    of_bsn_gentable_entry_add_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_entry_add_experimenter_get(
    of_bsn_gentable_entry_add_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_entry_add_subtype_set(
    of_bsn_gentable_entry_add_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_entry_add_subtype_get(
    of_bsn_gentable_entry_add_t *obj,
    uint32_t *subtype);

extern void of_bsn_gentable_entry_add_table_id_set(
    of_bsn_gentable_entry_add_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_entry_add_table_id_get(
    of_bsn_gentable_entry_add_t *obj,
    uint16_t *table_id);

extern void of_bsn_gentable_entry_add_checksum_set(
    of_bsn_gentable_entry_add_t *obj,
    of_checksum_128_t checksum);
extern void of_bsn_gentable_entry_add_checksum_get(
    of_bsn_gentable_entry_add_t *obj,
    of_checksum_128_t *checksum);

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_add_key_set(
    of_bsn_gentable_entry_add_t *obj,
    of_list_bsn_tlv_t *key);
extern void of_bsn_gentable_entry_add_key_bind(
    of_bsn_gentable_entry_add_t *obj,
    of_list_bsn_tlv_t *key);
extern of_list_bsn_tlv_t *of_bsn_gentable_entry_add_key_get(
    of_bsn_gentable_entry_add_t *obj);

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_add_value_set(
    of_bsn_gentable_entry_add_t *obj,
    of_list_bsn_tlv_t *value);
extern void of_bsn_gentable_entry_add_value_bind(
    of_bsn_gentable_entry_add_t *obj,
    of_list_bsn_tlv_t *value);
extern of_list_bsn_tlv_t *of_bsn_gentable_entry_add_value_get(
    of_bsn_gentable_entry_add_t *obj);

/* Unified accessor functions for of_bsn_gentable_entry_delete */

extern void of_bsn_gentable_entry_delete_xid_set(
    of_bsn_gentable_entry_delete_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_entry_delete_xid_get(
    of_bsn_gentable_entry_delete_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_entry_delete_experimenter_set(
    of_bsn_gentable_entry_delete_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_entry_delete_experimenter_get(
    of_bsn_gentable_entry_delete_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_entry_delete_subtype_set(
    of_bsn_gentable_entry_delete_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_entry_delete_subtype_get(
    of_bsn_gentable_entry_delete_t *obj,
    uint32_t *subtype);

extern void of_bsn_gentable_entry_delete_table_id_set(
    of_bsn_gentable_entry_delete_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_entry_delete_table_id_get(
    of_bsn_gentable_entry_delete_t *obj,
    uint16_t *table_id);

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_delete_key_set(
    of_bsn_gentable_entry_delete_t *obj,
    of_list_bsn_tlv_t *key);
extern void of_bsn_gentable_entry_delete_key_bind(
    of_bsn_gentable_entry_delete_t *obj,
    of_list_bsn_tlv_t *key);
extern of_list_bsn_tlv_t *of_bsn_gentable_entry_delete_key_get(
    of_bsn_gentable_entry_delete_t *obj);

/* Unified accessor functions for of_bsn_gentable_entry_desc_stats_reply */

extern void of_bsn_gentable_entry_desc_stats_reply_xid_set(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_entry_desc_stats_reply_xid_get(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_entry_desc_stats_reply_flags_set(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_entry_desc_stats_reply_flags_get(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_entry_desc_stats_reply_experimenter_set(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_entry_desc_stats_reply_experimenter_get(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_entry_desc_stats_reply_subtype_set(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_entry_desc_stats_reply_subtype_get(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_desc_stats_reply_entries_set(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    of_list_bsn_gentable_entry_desc_stats_entry_t *entries);
extern void of_bsn_gentable_entry_desc_stats_reply_entries_bind(
    of_bsn_gentable_entry_desc_stats_reply_t *obj,
    of_list_bsn_gentable_entry_desc_stats_entry_t *entries);
extern of_list_bsn_gentable_entry_desc_stats_entry_t *of_bsn_gentable_entry_desc_stats_reply_entries_get(
    of_bsn_gentable_entry_desc_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_gentable_entry_desc_stats_request */

extern void of_bsn_gentable_entry_desc_stats_request_xid_set(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_entry_desc_stats_request_xid_get(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_entry_desc_stats_request_flags_set(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_entry_desc_stats_request_flags_get(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_entry_desc_stats_request_experimenter_set(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_entry_desc_stats_request_experimenter_get(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_entry_desc_stats_request_subtype_set(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_entry_desc_stats_request_subtype_get(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_gentable_entry_desc_stats_request_table_id_set(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_entry_desc_stats_request_table_id_get(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    uint16_t *table_id);

extern void of_bsn_gentable_entry_desc_stats_request_checksum_set(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    of_checksum_128_t checksum);
extern void of_bsn_gentable_entry_desc_stats_request_checksum_get(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    of_checksum_128_t *checksum);

extern void of_bsn_gentable_entry_desc_stats_request_checksum_mask_set(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    of_checksum_128_t checksum_mask);
extern void of_bsn_gentable_entry_desc_stats_request_checksum_mask_get(
    of_bsn_gentable_entry_desc_stats_request_t *obj,
    of_checksum_128_t *checksum_mask);

/* Unified accessor functions for of_bsn_gentable_entry_stats_reply */

extern void of_bsn_gentable_entry_stats_reply_xid_set(
    of_bsn_gentable_entry_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_entry_stats_reply_xid_get(
    of_bsn_gentable_entry_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_entry_stats_reply_flags_set(
    of_bsn_gentable_entry_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_entry_stats_reply_flags_get(
    of_bsn_gentable_entry_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_entry_stats_reply_experimenter_set(
    of_bsn_gentable_entry_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_entry_stats_reply_experimenter_get(
    of_bsn_gentable_entry_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_entry_stats_reply_subtype_set(
    of_bsn_gentable_entry_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_entry_stats_reply_subtype_get(
    of_bsn_gentable_entry_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_stats_reply_entries_set(
    of_bsn_gentable_entry_stats_reply_t *obj,
    of_list_bsn_gentable_entry_stats_entry_t *entries);
extern void of_bsn_gentable_entry_stats_reply_entries_bind(
    of_bsn_gentable_entry_stats_reply_t *obj,
    of_list_bsn_gentable_entry_stats_entry_t *entries);
extern of_list_bsn_gentable_entry_stats_entry_t *of_bsn_gentable_entry_stats_reply_entries_get(
    of_bsn_gentable_entry_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_gentable_entry_stats_request */

extern void of_bsn_gentable_entry_stats_request_xid_set(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_entry_stats_request_xid_get(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_entry_stats_request_flags_set(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_entry_stats_request_flags_get(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_entry_stats_request_experimenter_set(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_entry_stats_request_experimenter_get(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_entry_stats_request_subtype_set(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_entry_stats_request_subtype_get(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_gentable_entry_stats_request_table_id_set(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_entry_stats_request_table_id_get(
    of_bsn_gentable_entry_stats_request_t *obj,
    uint16_t *table_id);

extern void of_bsn_gentable_entry_stats_request_checksum_set(
    of_bsn_gentable_entry_stats_request_t *obj,
    of_checksum_128_t checksum);
extern void of_bsn_gentable_entry_stats_request_checksum_get(
    of_bsn_gentable_entry_stats_request_t *obj,
    of_checksum_128_t *checksum);

extern void of_bsn_gentable_entry_stats_request_checksum_mask_set(
    of_bsn_gentable_entry_stats_request_t *obj,
    of_checksum_128_t checksum_mask);
extern void of_bsn_gentable_entry_stats_request_checksum_mask_get(
    of_bsn_gentable_entry_stats_request_t *obj,
    of_checksum_128_t *checksum_mask);

/* Unified accessor functions for of_bsn_gentable_set_buckets_size */

extern void of_bsn_gentable_set_buckets_size_xid_set(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_set_buckets_size_xid_get(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_set_buckets_size_experimenter_set(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_set_buckets_size_experimenter_get(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_set_buckets_size_subtype_set(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_set_buckets_size_subtype_get(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint32_t *subtype);

extern void of_bsn_gentable_set_buckets_size_table_id_set(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_set_buckets_size_table_id_get(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint16_t *table_id);

extern void of_bsn_gentable_set_buckets_size_buckets_size_set(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint32_t buckets_size);
extern void of_bsn_gentable_set_buckets_size_buckets_size_get(
    of_bsn_gentable_set_buckets_size_t *obj,
    uint32_t *buckets_size);

/* Unified accessor functions for of_bsn_gentable_stats_reply */

extern void of_bsn_gentable_stats_reply_xid_set(
    of_bsn_gentable_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_stats_reply_xid_get(
    of_bsn_gentable_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_stats_reply_flags_set(
    of_bsn_gentable_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_stats_reply_flags_get(
    of_bsn_gentable_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_stats_reply_experimenter_set(
    of_bsn_gentable_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_stats_reply_experimenter_get(
    of_bsn_gentable_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_stats_reply_subtype_set(
    of_bsn_gentable_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_stats_reply_subtype_get(
    of_bsn_gentable_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_gentable_stats_reply_entries_set(
    of_bsn_gentable_stats_reply_t *obj,
    of_list_bsn_gentable_stats_entry_t *entries);
extern void of_bsn_gentable_stats_reply_entries_bind(
    of_bsn_gentable_stats_reply_t *obj,
    of_list_bsn_gentable_stats_entry_t *entries);
extern of_list_bsn_gentable_stats_entry_t *of_bsn_gentable_stats_reply_entries_get(
    of_bsn_gentable_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_gentable_stats_request */

extern void of_bsn_gentable_stats_request_xid_set(
    of_bsn_gentable_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_gentable_stats_request_xid_get(
    of_bsn_gentable_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_gentable_stats_request_flags_set(
    of_bsn_gentable_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_gentable_stats_request_flags_get(
    of_bsn_gentable_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_gentable_stats_request_experimenter_set(
    of_bsn_gentable_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_gentable_stats_request_experimenter_get(
    of_bsn_gentable_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_gentable_stats_request_subtype_set(
    of_bsn_gentable_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_gentable_stats_request_subtype_get(
    of_bsn_gentable_stats_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_get_interfaces_reply */

extern void of_bsn_get_interfaces_reply_xid_set(
    of_bsn_get_interfaces_reply_t *obj,
    uint32_t xid);
extern void of_bsn_get_interfaces_reply_xid_get(
    of_bsn_get_interfaces_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_get_interfaces_reply_experimenter_set(
    of_bsn_get_interfaces_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_interfaces_reply_experimenter_get(
    of_bsn_get_interfaces_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_interfaces_reply_subtype_set(
    of_bsn_get_interfaces_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_get_interfaces_reply_subtype_get(
    of_bsn_get_interfaces_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_get_interfaces_reply_interfaces_set(
    of_bsn_get_interfaces_reply_t *obj,
    of_list_bsn_interface_t *interfaces);
extern void of_bsn_get_interfaces_reply_interfaces_bind(
    of_bsn_get_interfaces_reply_t *obj,
    of_list_bsn_interface_t *interfaces);
extern of_list_bsn_interface_t *of_bsn_get_interfaces_reply_interfaces_get(
    of_bsn_get_interfaces_reply_t *obj);

/* Unified accessor functions for of_bsn_get_interfaces_request */

extern void of_bsn_get_interfaces_request_xid_set(
    of_bsn_get_interfaces_request_t *obj,
    uint32_t xid);
extern void of_bsn_get_interfaces_request_xid_get(
    of_bsn_get_interfaces_request_t *obj,
    uint32_t *xid);

extern void of_bsn_get_interfaces_request_experimenter_set(
    of_bsn_get_interfaces_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_interfaces_request_experimenter_get(
    of_bsn_get_interfaces_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_interfaces_request_subtype_set(
    of_bsn_get_interfaces_request_t *obj,
    uint32_t subtype);
extern void of_bsn_get_interfaces_request_subtype_get(
    of_bsn_get_interfaces_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_get_ip_mask_reply */

extern void of_bsn_get_ip_mask_reply_xid_set(
    of_bsn_get_ip_mask_reply_t *obj,
    uint32_t xid);
extern void of_bsn_get_ip_mask_reply_xid_get(
    of_bsn_get_ip_mask_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_get_ip_mask_reply_experimenter_set(
    of_bsn_get_ip_mask_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_ip_mask_reply_experimenter_get(
    of_bsn_get_ip_mask_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_ip_mask_reply_subtype_set(
    of_bsn_get_ip_mask_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_get_ip_mask_reply_subtype_get(
    of_bsn_get_ip_mask_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_get_ip_mask_reply_index_set(
    of_bsn_get_ip_mask_reply_t *obj,
    uint8_t index);
extern void of_bsn_get_ip_mask_reply_index_get(
    of_bsn_get_ip_mask_reply_t *obj,
    uint8_t *index);

extern void of_bsn_get_ip_mask_reply_mask_set(
    of_bsn_get_ip_mask_reply_t *obj,
    uint32_t mask);
extern void of_bsn_get_ip_mask_reply_mask_get(
    of_bsn_get_ip_mask_reply_t *obj,
    uint32_t *mask);

/* Unified accessor functions for of_bsn_get_ip_mask_request */

extern void of_bsn_get_ip_mask_request_xid_set(
    of_bsn_get_ip_mask_request_t *obj,
    uint32_t xid);
extern void of_bsn_get_ip_mask_request_xid_get(
    of_bsn_get_ip_mask_request_t *obj,
    uint32_t *xid);

extern void of_bsn_get_ip_mask_request_experimenter_set(
    of_bsn_get_ip_mask_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_ip_mask_request_experimenter_get(
    of_bsn_get_ip_mask_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_ip_mask_request_subtype_set(
    of_bsn_get_ip_mask_request_t *obj,
    uint32_t subtype);
extern void of_bsn_get_ip_mask_request_subtype_get(
    of_bsn_get_ip_mask_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_get_ip_mask_request_index_set(
    of_bsn_get_ip_mask_request_t *obj,
    uint8_t index);
extern void of_bsn_get_ip_mask_request_index_get(
    of_bsn_get_ip_mask_request_t *obj,
    uint8_t *index);

/* Unified accessor functions for of_bsn_get_l2_table_reply */

extern void of_bsn_get_l2_table_reply_xid_set(
    of_bsn_get_l2_table_reply_t *obj,
    uint32_t xid);
extern void of_bsn_get_l2_table_reply_xid_get(
    of_bsn_get_l2_table_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_get_l2_table_reply_experimenter_set(
    of_bsn_get_l2_table_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_l2_table_reply_experimenter_get(
    of_bsn_get_l2_table_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_l2_table_reply_subtype_set(
    of_bsn_get_l2_table_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_get_l2_table_reply_subtype_get(
    of_bsn_get_l2_table_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_get_l2_table_reply_l2_table_enable_set(
    of_bsn_get_l2_table_reply_t *obj,
    uint8_t l2_table_enable);
extern void of_bsn_get_l2_table_reply_l2_table_enable_get(
    of_bsn_get_l2_table_reply_t *obj,
    uint8_t *l2_table_enable);

extern void of_bsn_get_l2_table_reply_l2_table_priority_set(
    of_bsn_get_l2_table_reply_t *obj,
    uint16_t l2_table_priority);
extern void of_bsn_get_l2_table_reply_l2_table_priority_get(
    of_bsn_get_l2_table_reply_t *obj,
    uint16_t *l2_table_priority);

/* Unified accessor functions for of_bsn_get_l2_table_request */

extern void of_bsn_get_l2_table_request_xid_set(
    of_bsn_get_l2_table_request_t *obj,
    uint32_t xid);
extern void of_bsn_get_l2_table_request_xid_get(
    of_bsn_get_l2_table_request_t *obj,
    uint32_t *xid);

extern void of_bsn_get_l2_table_request_experimenter_set(
    of_bsn_get_l2_table_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_l2_table_request_experimenter_get(
    of_bsn_get_l2_table_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_l2_table_request_subtype_set(
    of_bsn_get_l2_table_request_t *obj,
    uint32_t subtype);
extern void of_bsn_get_l2_table_request_subtype_get(
    of_bsn_get_l2_table_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_get_mirroring_reply */

extern void of_bsn_get_mirroring_reply_xid_set(
    of_bsn_get_mirroring_reply_t *obj,
    uint32_t xid);
extern void of_bsn_get_mirroring_reply_xid_get(
    of_bsn_get_mirroring_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_get_mirroring_reply_experimenter_set(
    of_bsn_get_mirroring_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_mirroring_reply_experimenter_get(
    of_bsn_get_mirroring_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_mirroring_reply_subtype_set(
    of_bsn_get_mirroring_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_get_mirroring_reply_subtype_get(
    of_bsn_get_mirroring_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_get_mirroring_reply_report_mirror_ports_set(
    of_bsn_get_mirroring_reply_t *obj,
    uint8_t report_mirror_ports);
extern void of_bsn_get_mirroring_reply_report_mirror_ports_get(
    of_bsn_get_mirroring_reply_t *obj,
    uint8_t *report_mirror_ports);

/* Unified accessor functions for of_bsn_get_mirroring_request */

extern void of_bsn_get_mirroring_request_xid_set(
    of_bsn_get_mirroring_request_t *obj,
    uint32_t xid);
extern void of_bsn_get_mirroring_request_xid_get(
    of_bsn_get_mirroring_request_t *obj,
    uint32_t *xid);

extern void of_bsn_get_mirroring_request_experimenter_set(
    of_bsn_get_mirroring_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_mirroring_request_experimenter_get(
    of_bsn_get_mirroring_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_mirroring_request_subtype_set(
    of_bsn_get_mirroring_request_t *obj,
    uint32_t subtype);
extern void of_bsn_get_mirroring_request_subtype_get(
    of_bsn_get_mirroring_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_get_mirroring_request_report_mirror_ports_set(
    of_bsn_get_mirroring_request_t *obj,
    uint8_t report_mirror_ports);
extern void of_bsn_get_mirroring_request_report_mirror_ports_get(
    of_bsn_get_mirroring_request_t *obj,
    uint8_t *report_mirror_ports);

/* Unified accessor functions for of_bsn_get_switch_pipeline_reply */

extern void of_bsn_get_switch_pipeline_reply_xid_set(
    of_bsn_get_switch_pipeline_reply_t *obj,
    uint32_t xid);
extern void of_bsn_get_switch_pipeline_reply_xid_get(
    of_bsn_get_switch_pipeline_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_get_switch_pipeline_reply_experimenter_set(
    of_bsn_get_switch_pipeline_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_switch_pipeline_reply_experimenter_get(
    of_bsn_get_switch_pipeline_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_switch_pipeline_reply_subtype_set(
    of_bsn_get_switch_pipeline_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_get_switch_pipeline_reply_subtype_get(
    of_bsn_get_switch_pipeline_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_get_switch_pipeline_reply_pipeline_set(
    of_bsn_get_switch_pipeline_reply_t *obj,
    of_desc_str_t pipeline);
extern void of_bsn_get_switch_pipeline_reply_pipeline_get(
    of_bsn_get_switch_pipeline_reply_t *obj,
    of_desc_str_t *pipeline);

/* Unified accessor functions for of_bsn_get_switch_pipeline_request */

extern void of_bsn_get_switch_pipeline_request_xid_set(
    of_bsn_get_switch_pipeline_request_t *obj,
    uint32_t xid);
extern void of_bsn_get_switch_pipeline_request_xid_get(
    of_bsn_get_switch_pipeline_request_t *obj,
    uint32_t *xid);

extern void of_bsn_get_switch_pipeline_request_experimenter_set(
    of_bsn_get_switch_pipeline_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_get_switch_pipeline_request_experimenter_get(
    of_bsn_get_switch_pipeline_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_get_switch_pipeline_request_subtype_set(
    of_bsn_get_switch_pipeline_request_t *obj,
    uint32_t subtype);
extern void of_bsn_get_switch_pipeline_request_subtype_get(
    of_bsn_get_switch_pipeline_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_header */

extern void of_bsn_header_xid_set(
    of_bsn_header_t *obj,
    uint32_t xid);
extern void of_bsn_header_xid_get(
    of_bsn_header_t *obj,
    uint32_t *xid);

extern void of_bsn_header_experimenter_set(
    of_bsn_header_t *obj,
    uint32_t experimenter);
extern void of_bsn_header_experimenter_get(
    of_bsn_header_t *obj,
    uint32_t *experimenter);

extern void of_bsn_header_subtype_set(
    of_bsn_header_t *obj,
    uint32_t subtype);
extern void of_bsn_header_subtype_get(
    of_bsn_header_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_hybrid_get_reply */

extern void of_bsn_hybrid_get_reply_xid_set(
    of_bsn_hybrid_get_reply_t *obj,
    uint32_t xid);
extern void of_bsn_hybrid_get_reply_xid_get(
    of_bsn_hybrid_get_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_hybrid_get_reply_experimenter_set(
    of_bsn_hybrid_get_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_hybrid_get_reply_experimenter_get(
    of_bsn_hybrid_get_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_hybrid_get_reply_subtype_set(
    of_bsn_hybrid_get_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_hybrid_get_reply_subtype_get(
    of_bsn_hybrid_get_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_hybrid_get_reply_hybrid_enable_set(
    of_bsn_hybrid_get_reply_t *obj,
    uint8_t hybrid_enable);
extern void of_bsn_hybrid_get_reply_hybrid_enable_get(
    of_bsn_hybrid_get_reply_t *obj,
    uint8_t *hybrid_enable);

extern void of_bsn_hybrid_get_reply_hybrid_version_set(
    of_bsn_hybrid_get_reply_t *obj,
    uint16_t hybrid_version);
extern void of_bsn_hybrid_get_reply_hybrid_version_get(
    of_bsn_hybrid_get_reply_t *obj,
    uint16_t *hybrid_version);

/* Unified accessor functions for of_bsn_hybrid_get_request */

extern void of_bsn_hybrid_get_request_xid_set(
    of_bsn_hybrid_get_request_t *obj,
    uint32_t xid);
extern void of_bsn_hybrid_get_request_xid_get(
    of_bsn_hybrid_get_request_t *obj,
    uint32_t *xid);

extern void of_bsn_hybrid_get_request_experimenter_set(
    of_bsn_hybrid_get_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_hybrid_get_request_experimenter_get(
    of_bsn_hybrid_get_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_hybrid_get_request_subtype_set(
    of_bsn_hybrid_get_request_t *obj,
    uint32_t subtype);
extern void of_bsn_hybrid_get_request_subtype_get(
    of_bsn_hybrid_get_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_lacp_convergence_notif */

extern void of_bsn_lacp_convergence_notif_xid_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint32_t xid);
extern void of_bsn_lacp_convergence_notif_xid_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint32_t *xid);

extern void of_bsn_lacp_convergence_notif_experimenter_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint32_t experimenter);
extern void of_bsn_lacp_convergence_notif_experimenter_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint32_t *experimenter);

extern void of_bsn_lacp_convergence_notif_subtype_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint32_t subtype);
extern void of_bsn_lacp_convergence_notif_subtype_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint32_t *subtype);

extern void of_bsn_lacp_convergence_notif_convergence_status_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint8_t convergence_status);
extern void of_bsn_lacp_convergence_notif_convergence_status_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint8_t *convergence_status);

extern void of_bsn_lacp_convergence_notif_port_no_set(
    of_bsn_lacp_convergence_notif_t *obj,
    of_port_no_t port_no);
extern void of_bsn_lacp_convergence_notif_port_no_get(
    of_bsn_lacp_convergence_notif_t *obj,
    of_port_no_t *port_no);

extern void of_bsn_lacp_convergence_notif_actor_sys_priority_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t actor_sys_priority);
extern void of_bsn_lacp_convergence_notif_actor_sys_priority_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t *actor_sys_priority);

extern void of_bsn_lacp_convergence_notif_actor_sys_mac_set(
    of_bsn_lacp_convergence_notif_t *obj,
    of_mac_addr_t actor_sys_mac);
extern void of_bsn_lacp_convergence_notif_actor_sys_mac_get(
    of_bsn_lacp_convergence_notif_t *obj,
    of_mac_addr_t *actor_sys_mac);

extern void of_bsn_lacp_convergence_notif_actor_port_priority_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t actor_port_priority);
extern void of_bsn_lacp_convergence_notif_actor_port_priority_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t *actor_port_priority);

extern void of_bsn_lacp_convergence_notif_actor_port_num_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t actor_port_num);
extern void of_bsn_lacp_convergence_notif_actor_port_num_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t *actor_port_num);

extern void of_bsn_lacp_convergence_notif_actor_key_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t actor_key);
extern void of_bsn_lacp_convergence_notif_actor_key_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t *actor_key);

extern void of_bsn_lacp_convergence_notif_partner_sys_priority_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t partner_sys_priority);
extern void of_bsn_lacp_convergence_notif_partner_sys_priority_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t *partner_sys_priority);

extern void of_bsn_lacp_convergence_notif_partner_sys_mac_set(
    of_bsn_lacp_convergence_notif_t *obj,
    of_mac_addr_t partner_sys_mac);
extern void of_bsn_lacp_convergence_notif_partner_sys_mac_get(
    of_bsn_lacp_convergence_notif_t *obj,
    of_mac_addr_t *partner_sys_mac);

extern void of_bsn_lacp_convergence_notif_partner_port_priority_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t partner_port_priority);
extern void of_bsn_lacp_convergence_notif_partner_port_priority_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t *partner_port_priority);

extern void of_bsn_lacp_convergence_notif_partner_port_num_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t partner_port_num);
extern void of_bsn_lacp_convergence_notif_partner_port_num_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t *partner_port_num);

extern void of_bsn_lacp_convergence_notif_partner_key_set(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t partner_key);
extern void of_bsn_lacp_convergence_notif_partner_key_get(
    of_bsn_lacp_convergence_notif_t *obj,
    uint16_t *partner_key);

/* Unified accessor functions for of_bsn_lacp_stats_reply */

extern void of_bsn_lacp_stats_reply_xid_set(
    of_bsn_lacp_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_lacp_stats_reply_xid_get(
    of_bsn_lacp_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_lacp_stats_reply_flags_set(
    of_bsn_lacp_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_lacp_stats_reply_flags_get(
    of_bsn_lacp_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_lacp_stats_reply_experimenter_set(
    of_bsn_lacp_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_lacp_stats_reply_experimenter_get(
    of_bsn_lacp_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_lacp_stats_reply_subtype_set(
    of_bsn_lacp_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_lacp_stats_reply_subtype_get(
    of_bsn_lacp_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_lacp_stats_reply_entries_set(
    of_bsn_lacp_stats_reply_t *obj,
    of_list_bsn_lacp_stats_entry_t *entries);
extern void of_bsn_lacp_stats_reply_entries_bind(
    of_bsn_lacp_stats_reply_t *obj,
    of_list_bsn_lacp_stats_entry_t *entries);
extern of_list_bsn_lacp_stats_entry_t *of_bsn_lacp_stats_reply_entries_get(
    of_bsn_lacp_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_lacp_stats_request */

extern void of_bsn_lacp_stats_request_xid_set(
    of_bsn_lacp_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_lacp_stats_request_xid_get(
    of_bsn_lacp_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_lacp_stats_request_flags_set(
    of_bsn_lacp_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_lacp_stats_request_flags_get(
    of_bsn_lacp_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_lacp_stats_request_experimenter_set(
    of_bsn_lacp_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_lacp_stats_request_experimenter_get(
    of_bsn_lacp_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_lacp_stats_request_subtype_set(
    of_bsn_lacp_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_lacp_stats_request_subtype_get(
    of_bsn_lacp_stats_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_pdu_rx_reply */

extern void of_bsn_pdu_rx_reply_xid_set(
    of_bsn_pdu_rx_reply_t *obj,
    uint32_t xid);
extern void of_bsn_pdu_rx_reply_xid_get(
    of_bsn_pdu_rx_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_pdu_rx_reply_experimenter_set(
    of_bsn_pdu_rx_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_pdu_rx_reply_experimenter_get(
    of_bsn_pdu_rx_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_pdu_rx_reply_subtype_set(
    of_bsn_pdu_rx_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_pdu_rx_reply_subtype_get(
    of_bsn_pdu_rx_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_pdu_rx_reply_status_set(
    of_bsn_pdu_rx_reply_t *obj,
    uint32_t status);
extern void of_bsn_pdu_rx_reply_status_get(
    of_bsn_pdu_rx_reply_t *obj,
    uint32_t *status);

extern void of_bsn_pdu_rx_reply_port_no_set(
    of_bsn_pdu_rx_reply_t *obj,
    of_port_no_t port_no);
extern void of_bsn_pdu_rx_reply_port_no_get(
    of_bsn_pdu_rx_reply_t *obj,
    of_port_no_t *port_no);

extern void of_bsn_pdu_rx_reply_slot_num_set(
    of_bsn_pdu_rx_reply_t *obj,
    uint8_t slot_num);
extern void of_bsn_pdu_rx_reply_slot_num_get(
    of_bsn_pdu_rx_reply_t *obj,
    uint8_t *slot_num);

/* Unified accessor functions for of_bsn_pdu_rx_request */

extern void of_bsn_pdu_rx_request_xid_set(
    of_bsn_pdu_rx_request_t *obj,
    uint32_t xid);
extern void of_bsn_pdu_rx_request_xid_get(
    of_bsn_pdu_rx_request_t *obj,
    uint32_t *xid);

extern void of_bsn_pdu_rx_request_experimenter_set(
    of_bsn_pdu_rx_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_pdu_rx_request_experimenter_get(
    of_bsn_pdu_rx_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_pdu_rx_request_subtype_set(
    of_bsn_pdu_rx_request_t *obj,
    uint32_t subtype);
extern void of_bsn_pdu_rx_request_subtype_get(
    of_bsn_pdu_rx_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_pdu_rx_request_timeout_ms_set(
    of_bsn_pdu_rx_request_t *obj,
    uint32_t timeout_ms);
extern void of_bsn_pdu_rx_request_timeout_ms_get(
    of_bsn_pdu_rx_request_t *obj,
    uint32_t *timeout_ms);

extern void of_bsn_pdu_rx_request_port_no_set(
    of_bsn_pdu_rx_request_t *obj,
    of_port_no_t port_no);
extern void of_bsn_pdu_rx_request_port_no_get(
    of_bsn_pdu_rx_request_t *obj,
    of_port_no_t *port_no);

extern void of_bsn_pdu_rx_request_slot_num_set(
    of_bsn_pdu_rx_request_t *obj,
    uint8_t slot_num);
extern void of_bsn_pdu_rx_request_slot_num_get(
    of_bsn_pdu_rx_request_t *obj,
    uint8_t *slot_num);

extern int WARN_UNUSED_RESULT of_bsn_pdu_rx_request_data_set(
    of_bsn_pdu_rx_request_t *obj,
    of_octets_t *data);
extern void of_bsn_pdu_rx_request_data_get(
    of_bsn_pdu_rx_request_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_bsn_pdu_rx_timeout */

extern void of_bsn_pdu_rx_timeout_xid_set(
    of_bsn_pdu_rx_timeout_t *obj,
    uint32_t xid);
extern void of_bsn_pdu_rx_timeout_xid_get(
    of_bsn_pdu_rx_timeout_t *obj,
    uint32_t *xid);

extern void of_bsn_pdu_rx_timeout_experimenter_set(
    of_bsn_pdu_rx_timeout_t *obj,
    uint32_t experimenter);
extern void of_bsn_pdu_rx_timeout_experimenter_get(
    of_bsn_pdu_rx_timeout_t *obj,
    uint32_t *experimenter);

extern void of_bsn_pdu_rx_timeout_subtype_set(
    of_bsn_pdu_rx_timeout_t *obj,
    uint32_t subtype);
extern void of_bsn_pdu_rx_timeout_subtype_get(
    of_bsn_pdu_rx_timeout_t *obj,
    uint32_t *subtype);

extern void of_bsn_pdu_rx_timeout_port_no_set(
    of_bsn_pdu_rx_timeout_t *obj,
    of_port_no_t port_no);
extern void of_bsn_pdu_rx_timeout_port_no_get(
    of_bsn_pdu_rx_timeout_t *obj,
    of_port_no_t *port_no);

extern void of_bsn_pdu_rx_timeout_slot_num_set(
    of_bsn_pdu_rx_timeout_t *obj,
    uint8_t slot_num);
extern void of_bsn_pdu_rx_timeout_slot_num_get(
    of_bsn_pdu_rx_timeout_t *obj,
    uint8_t *slot_num);

/* Unified accessor functions for of_bsn_pdu_tx_reply */

extern void of_bsn_pdu_tx_reply_xid_set(
    of_bsn_pdu_tx_reply_t *obj,
    uint32_t xid);
extern void of_bsn_pdu_tx_reply_xid_get(
    of_bsn_pdu_tx_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_pdu_tx_reply_experimenter_set(
    of_bsn_pdu_tx_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_pdu_tx_reply_experimenter_get(
    of_bsn_pdu_tx_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_pdu_tx_reply_subtype_set(
    of_bsn_pdu_tx_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_pdu_tx_reply_subtype_get(
    of_bsn_pdu_tx_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_pdu_tx_reply_status_set(
    of_bsn_pdu_tx_reply_t *obj,
    uint32_t status);
extern void of_bsn_pdu_tx_reply_status_get(
    of_bsn_pdu_tx_reply_t *obj,
    uint32_t *status);

extern void of_bsn_pdu_tx_reply_port_no_set(
    of_bsn_pdu_tx_reply_t *obj,
    of_port_no_t port_no);
extern void of_bsn_pdu_tx_reply_port_no_get(
    of_bsn_pdu_tx_reply_t *obj,
    of_port_no_t *port_no);

extern void of_bsn_pdu_tx_reply_slot_num_set(
    of_bsn_pdu_tx_reply_t *obj,
    uint8_t slot_num);
extern void of_bsn_pdu_tx_reply_slot_num_get(
    of_bsn_pdu_tx_reply_t *obj,
    uint8_t *slot_num);

/* Unified accessor functions for of_bsn_pdu_tx_request */

extern void of_bsn_pdu_tx_request_xid_set(
    of_bsn_pdu_tx_request_t *obj,
    uint32_t xid);
extern void of_bsn_pdu_tx_request_xid_get(
    of_bsn_pdu_tx_request_t *obj,
    uint32_t *xid);

extern void of_bsn_pdu_tx_request_experimenter_set(
    of_bsn_pdu_tx_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_pdu_tx_request_experimenter_get(
    of_bsn_pdu_tx_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_pdu_tx_request_subtype_set(
    of_bsn_pdu_tx_request_t *obj,
    uint32_t subtype);
extern void of_bsn_pdu_tx_request_subtype_get(
    of_bsn_pdu_tx_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_pdu_tx_request_tx_interval_ms_set(
    of_bsn_pdu_tx_request_t *obj,
    uint32_t tx_interval_ms);
extern void of_bsn_pdu_tx_request_tx_interval_ms_get(
    of_bsn_pdu_tx_request_t *obj,
    uint32_t *tx_interval_ms);

extern void of_bsn_pdu_tx_request_port_no_set(
    of_bsn_pdu_tx_request_t *obj,
    of_port_no_t port_no);
extern void of_bsn_pdu_tx_request_port_no_get(
    of_bsn_pdu_tx_request_t *obj,
    of_port_no_t *port_no);

extern void of_bsn_pdu_tx_request_slot_num_set(
    of_bsn_pdu_tx_request_t *obj,
    uint8_t slot_num);
extern void of_bsn_pdu_tx_request_slot_num_get(
    of_bsn_pdu_tx_request_t *obj,
    uint8_t *slot_num);

extern int WARN_UNUSED_RESULT of_bsn_pdu_tx_request_data_set(
    of_bsn_pdu_tx_request_t *obj,
    of_octets_t *data);
extern void of_bsn_pdu_tx_request_data_get(
    of_bsn_pdu_tx_request_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_bsn_port_counter_stats_reply */

extern void of_bsn_port_counter_stats_reply_xid_set(
    of_bsn_port_counter_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_port_counter_stats_reply_xid_get(
    of_bsn_port_counter_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_port_counter_stats_reply_flags_set(
    of_bsn_port_counter_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_port_counter_stats_reply_flags_get(
    of_bsn_port_counter_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_port_counter_stats_reply_experimenter_set(
    of_bsn_port_counter_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_port_counter_stats_reply_experimenter_get(
    of_bsn_port_counter_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_port_counter_stats_reply_subtype_set(
    of_bsn_port_counter_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_port_counter_stats_reply_subtype_get(
    of_bsn_port_counter_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_port_counter_stats_reply_entries_set(
    of_bsn_port_counter_stats_reply_t *obj,
    of_list_bsn_port_counter_stats_entry_t *entries);
extern void of_bsn_port_counter_stats_reply_entries_bind(
    of_bsn_port_counter_stats_reply_t *obj,
    of_list_bsn_port_counter_stats_entry_t *entries);
extern of_list_bsn_port_counter_stats_entry_t *of_bsn_port_counter_stats_reply_entries_get(
    of_bsn_port_counter_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_port_counter_stats_request */

extern void of_bsn_port_counter_stats_request_xid_set(
    of_bsn_port_counter_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_port_counter_stats_request_xid_get(
    of_bsn_port_counter_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_port_counter_stats_request_flags_set(
    of_bsn_port_counter_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_port_counter_stats_request_flags_get(
    of_bsn_port_counter_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_port_counter_stats_request_experimenter_set(
    of_bsn_port_counter_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_port_counter_stats_request_experimenter_get(
    of_bsn_port_counter_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_port_counter_stats_request_subtype_set(
    of_bsn_port_counter_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_port_counter_stats_request_subtype_get(
    of_bsn_port_counter_stats_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_port_counter_stats_request_port_no_set(
    of_bsn_port_counter_stats_request_t *obj,
    of_port_no_t port_no);
extern void of_bsn_port_counter_stats_request_port_no_get(
    of_bsn_port_counter_stats_request_t *obj,
    of_port_no_t *port_no);

/* Unified accessor functions for of_bsn_role_status */

extern void of_bsn_role_status_xid_set(
    of_bsn_role_status_t *obj,
    uint32_t xid);
extern void of_bsn_role_status_xid_get(
    of_bsn_role_status_t *obj,
    uint32_t *xid);

extern void of_bsn_role_status_experimenter_set(
    of_bsn_role_status_t *obj,
    uint32_t experimenter);
extern void of_bsn_role_status_experimenter_get(
    of_bsn_role_status_t *obj,
    uint32_t *experimenter);

extern void of_bsn_role_status_subtype_set(
    of_bsn_role_status_t *obj,
    uint32_t subtype);
extern void of_bsn_role_status_subtype_get(
    of_bsn_role_status_t *obj,
    uint32_t *subtype);

extern void of_bsn_role_status_role_set(
    of_bsn_role_status_t *obj,
    uint32_t role);
extern void of_bsn_role_status_role_get(
    of_bsn_role_status_t *obj,
    uint32_t *role);

extern void of_bsn_role_status_reason_set(
    of_bsn_role_status_t *obj,
    uint8_t reason);
extern void of_bsn_role_status_reason_get(
    of_bsn_role_status_t *obj,
    uint8_t *reason);

extern void of_bsn_role_status_generation_id_set(
    of_bsn_role_status_t *obj,
    uint64_t generation_id);
extern void of_bsn_role_status_generation_id_get(
    of_bsn_role_status_t *obj,
    uint64_t *generation_id);

/* Unified accessor functions for of_bsn_set_aux_cxns_reply */

extern void of_bsn_set_aux_cxns_reply_xid_set(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t xid);
extern void of_bsn_set_aux_cxns_reply_xid_get(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_set_aux_cxns_reply_experimenter_set(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_aux_cxns_reply_experimenter_get(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_aux_cxns_reply_subtype_set(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_set_aux_cxns_reply_subtype_get(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_aux_cxns_reply_num_aux_set(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t num_aux);
extern void of_bsn_set_aux_cxns_reply_num_aux_get(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t *num_aux);

extern void of_bsn_set_aux_cxns_reply_status_set(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t status);
extern void of_bsn_set_aux_cxns_reply_status_get(
    of_bsn_set_aux_cxns_reply_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_set_aux_cxns_request */

extern void of_bsn_set_aux_cxns_request_xid_set(
    of_bsn_set_aux_cxns_request_t *obj,
    uint32_t xid);
extern void of_bsn_set_aux_cxns_request_xid_get(
    of_bsn_set_aux_cxns_request_t *obj,
    uint32_t *xid);

extern void of_bsn_set_aux_cxns_request_experimenter_set(
    of_bsn_set_aux_cxns_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_aux_cxns_request_experimenter_get(
    of_bsn_set_aux_cxns_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_aux_cxns_request_subtype_set(
    of_bsn_set_aux_cxns_request_t *obj,
    uint32_t subtype);
extern void of_bsn_set_aux_cxns_request_subtype_get(
    of_bsn_set_aux_cxns_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_aux_cxns_request_num_aux_set(
    of_bsn_set_aux_cxns_request_t *obj,
    uint32_t num_aux);
extern void of_bsn_set_aux_cxns_request_num_aux_get(
    of_bsn_set_aux_cxns_request_t *obj,
    uint32_t *num_aux);

/* Unified accessor functions for of_bsn_set_ip_mask */

extern void of_bsn_set_ip_mask_xid_set(
    of_bsn_set_ip_mask_t *obj,
    uint32_t xid);
extern void of_bsn_set_ip_mask_xid_get(
    of_bsn_set_ip_mask_t *obj,
    uint32_t *xid);

extern void of_bsn_set_ip_mask_experimenter_set(
    of_bsn_set_ip_mask_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_ip_mask_experimenter_get(
    of_bsn_set_ip_mask_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_ip_mask_subtype_set(
    of_bsn_set_ip_mask_t *obj,
    uint32_t subtype);
extern void of_bsn_set_ip_mask_subtype_get(
    of_bsn_set_ip_mask_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_ip_mask_index_set(
    of_bsn_set_ip_mask_t *obj,
    uint8_t index);
extern void of_bsn_set_ip_mask_index_get(
    of_bsn_set_ip_mask_t *obj,
    uint8_t *index);

extern void of_bsn_set_ip_mask_mask_set(
    of_bsn_set_ip_mask_t *obj,
    uint32_t mask);
extern void of_bsn_set_ip_mask_mask_get(
    of_bsn_set_ip_mask_t *obj,
    uint32_t *mask);

/* Unified accessor functions for of_bsn_set_l2_table_reply */

extern void of_bsn_set_l2_table_reply_xid_set(
    of_bsn_set_l2_table_reply_t *obj,
    uint32_t xid);
extern void of_bsn_set_l2_table_reply_xid_get(
    of_bsn_set_l2_table_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_set_l2_table_reply_experimenter_set(
    of_bsn_set_l2_table_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_l2_table_reply_experimenter_get(
    of_bsn_set_l2_table_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_l2_table_reply_subtype_set(
    of_bsn_set_l2_table_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_set_l2_table_reply_subtype_get(
    of_bsn_set_l2_table_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_l2_table_reply_l2_table_enable_set(
    of_bsn_set_l2_table_reply_t *obj,
    uint8_t l2_table_enable);
extern void of_bsn_set_l2_table_reply_l2_table_enable_get(
    of_bsn_set_l2_table_reply_t *obj,
    uint8_t *l2_table_enable);

extern void of_bsn_set_l2_table_reply_l2_table_priority_set(
    of_bsn_set_l2_table_reply_t *obj,
    uint16_t l2_table_priority);
extern void of_bsn_set_l2_table_reply_l2_table_priority_get(
    of_bsn_set_l2_table_reply_t *obj,
    uint16_t *l2_table_priority);

extern void of_bsn_set_l2_table_reply_status_set(
    of_bsn_set_l2_table_reply_t *obj,
    uint32_t status);
extern void of_bsn_set_l2_table_reply_status_get(
    of_bsn_set_l2_table_reply_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_set_l2_table_request */

extern void of_bsn_set_l2_table_request_xid_set(
    of_bsn_set_l2_table_request_t *obj,
    uint32_t xid);
extern void of_bsn_set_l2_table_request_xid_get(
    of_bsn_set_l2_table_request_t *obj,
    uint32_t *xid);

extern void of_bsn_set_l2_table_request_experimenter_set(
    of_bsn_set_l2_table_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_l2_table_request_experimenter_get(
    of_bsn_set_l2_table_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_l2_table_request_subtype_set(
    of_bsn_set_l2_table_request_t *obj,
    uint32_t subtype);
extern void of_bsn_set_l2_table_request_subtype_get(
    of_bsn_set_l2_table_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_l2_table_request_l2_table_enable_set(
    of_bsn_set_l2_table_request_t *obj,
    uint8_t l2_table_enable);
extern void of_bsn_set_l2_table_request_l2_table_enable_get(
    of_bsn_set_l2_table_request_t *obj,
    uint8_t *l2_table_enable);

extern void of_bsn_set_l2_table_request_l2_table_priority_set(
    of_bsn_set_l2_table_request_t *obj,
    uint16_t l2_table_priority);
extern void of_bsn_set_l2_table_request_l2_table_priority_get(
    of_bsn_set_l2_table_request_t *obj,
    uint16_t *l2_table_priority);

/* Unified accessor functions for of_bsn_set_lacp_reply */

extern void of_bsn_set_lacp_reply_xid_set(
    of_bsn_set_lacp_reply_t *obj,
    uint32_t xid);
extern void of_bsn_set_lacp_reply_xid_get(
    of_bsn_set_lacp_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_set_lacp_reply_experimenter_set(
    of_bsn_set_lacp_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_lacp_reply_experimenter_get(
    of_bsn_set_lacp_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_lacp_reply_subtype_set(
    of_bsn_set_lacp_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_set_lacp_reply_subtype_get(
    of_bsn_set_lacp_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_lacp_reply_status_set(
    of_bsn_set_lacp_reply_t *obj,
    uint32_t status);
extern void of_bsn_set_lacp_reply_status_get(
    of_bsn_set_lacp_reply_t *obj,
    uint32_t *status);

extern void of_bsn_set_lacp_reply_port_no_set(
    of_bsn_set_lacp_reply_t *obj,
    of_port_no_t port_no);
extern void of_bsn_set_lacp_reply_port_no_get(
    of_bsn_set_lacp_reply_t *obj,
    of_port_no_t *port_no);

/* Unified accessor functions for of_bsn_set_lacp_request */

extern void of_bsn_set_lacp_request_xid_set(
    of_bsn_set_lacp_request_t *obj,
    uint32_t xid);
extern void of_bsn_set_lacp_request_xid_get(
    of_bsn_set_lacp_request_t *obj,
    uint32_t *xid);

extern void of_bsn_set_lacp_request_experimenter_set(
    of_bsn_set_lacp_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_lacp_request_experimenter_get(
    of_bsn_set_lacp_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_lacp_request_subtype_set(
    of_bsn_set_lacp_request_t *obj,
    uint32_t subtype);
extern void of_bsn_set_lacp_request_subtype_get(
    of_bsn_set_lacp_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_lacp_request_enabled_set(
    of_bsn_set_lacp_request_t *obj,
    uint8_t enabled);
extern void of_bsn_set_lacp_request_enabled_get(
    of_bsn_set_lacp_request_t *obj,
    uint8_t *enabled);

extern void of_bsn_set_lacp_request_port_no_set(
    of_bsn_set_lacp_request_t *obj,
    of_port_no_t port_no);
extern void of_bsn_set_lacp_request_port_no_get(
    of_bsn_set_lacp_request_t *obj,
    of_port_no_t *port_no);

extern void of_bsn_set_lacp_request_actor_sys_priority_set(
    of_bsn_set_lacp_request_t *obj,
    uint16_t actor_sys_priority);
extern void of_bsn_set_lacp_request_actor_sys_priority_get(
    of_bsn_set_lacp_request_t *obj,
    uint16_t *actor_sys_priority);

extern void of_bsn_set_lacp_request_actor_sys_mac_set(
    of_bsn_set_lacp_request_t *obj,
    of_mac_addr_t actor_sys_mac);
extern void of_bsn_set_lacp_request_actor_sys_mac_get(
    of_bsn_set_lacp_request_t *obj,
    of_mac_addr_t *actor_sys_mac);

extern void of_bsn_set_lacp_request_actor_port_priority_set(
    of_bsn_set_lacp_request_t *obj,
    uint16_t actor_port_priority);
extern void of_bsn_set_lacp_request_actor_port_priority_get(
    of_bsn_set_lacp_request_t *obj,
    uint16_t *actor_port_priority);

extern void of_bsn_set_lacp_request_actor_port_num_set(
    of_bsn_set_lacp_request_t *obj,
    uint16_t actor_port_num);
extern void of_bsn_set_lacp_request_actor_port_num_get(
    of_bsn_set_lacp_request_t *obj,
    uint16_t *actor_port_num);

extern void of_bsn_set_lacp_request_actor_key_set(
    of_bsn_set_lacp_request_t *obj,
    uint16_t actor_key);
extern void of_bsn_set_lacp_request_actor_key_get(
    of_bsn_set_lacp_request_t *obj,
    uint16_t *actor_key);

/* Unified accessor functions for of_bsn_set_mirroring */

extern void of_bsn_set_mirroring_xid_set(
    of_bsn_set_mirroring_t *obj,
    uint32_t xid);
extern void of_bsn_set_mirroring_xid_get(
    of_bsn_set_mirroring_t *obj,
    uint32_t *xid);

extern void of_bsn_set_mirroring_experimenter_set(
    of_bsn_set_mirroring_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_mirroring_experimenter_get(
    of_bsn_set_mirroring_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_mirroring_subtype_set(
    of_bsn_set_mirroring_t *obj,
    uint32_t subtype);
extern void of_bsn_set_mirroring_subtype_get(
    of_bsn_set_mirroring_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_mirroring_report_mirror_ports_set(
    of_bsn_set_mirroring_t *obj,
    uint8_t report_mirror_ports);
extern void of_bsn_set_mirroring_report_mirror_ports_get(
    of_bsn_set_mirroring_t *obj,
    uint8_t *report_mirror_ports);

/* Unified accessor functions for of_bsn_set_pktin_suppression_reply */

extern void of_bsn_set_pktin_suppression_reply_xid_set(
    of_bsn_set_pktin_suppression_reply_t *obj,
    uint32_t xid);
extern void of_bsn_set_pktin_suppression_reply_xid_get(
    of_bsn_set_pktin_suppression_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_set_pktin_suppression_reply_experimenter_set(
    of_bsn_set_pktin_suppression_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_pktin_suppression_reply_experimenter_get(
    of_bsn_set_pktin_suppression_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_pktin_suppression_reply_subtype_set(
    of_bsn_set_pktin_suppression_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_set_pktin_suppression_reply_subtype_get(
    of_bsn_set_pktin_suppression_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_pktin_suppression_reply_status_set(
    of_bsn_set_pktin_suppression_reply_t *obj,
    uint32_t status);
extern void of_bsn_set_pktin_suppression_reply_status_get(
    of_bsn_set_pktin_suppression_reply_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_set_pktin_suppression_request */

extern void of_bsn_set_pktin_suppression_request_xid_set(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint32_t xid);
extern void of_bsn_set_pktin_suppression_request_xid_get(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint32_t *xid);

extern void of_bsn_set_pktin_suppression_request_experimenter_set(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_pktin_suppression_request_experimenter_get(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_pktin_suppression_request_subtype_set(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint32_t subtype);
extern void of_bsn_set_pktin_suppression_request_subtype_get(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_pktin_suppression_request_enabled_set(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint8_t enabled);
extern void of_bsn_set_pktin_suppression_request_enabled_get(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint8_t *enabled);

extern void of_bsn_set_pktin_suppression_request_idle_timeout_set(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint16_t idle_timeout);
extern void of_bsn_set_pktin_suppression_request_idle_timeout_get(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint16_t *idle_timeout);

extern void of_bsn_set_pktin_suppression_request_hard_timeout_set(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint16_t hard_timeout);
extern void of_bsn_set_pktin_suppression_request_hard_timeout_get(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint16_t *hard_timeout);

extern void of_bsn_set_pktin_suppression_request_priority_set(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint16_t priority);
extern void of_bsn_set_pktin_suppression_request_priority_get(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint16_t *priority);

extern void of_bsn_set_pktin_suppression_request_cookie_set(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint64_t cookie);
extern void of_bsn_set_pktin_suppression_request_cookie_get(
    of_bsn_set_pktin_suppression_request_t *obj,
    uint64_t *cookie);

/* Unified accessor functions for of_bsn_set_switch_pipeline_reply */

extern void of_bsn_set_switch_pipeline_reply_xid_set(
    of_bsn_set_switch_pipeline_reply_t *obj,
    uint32_t xid);
extern void of_bsn_set_switch_pipeline_reply_xid_get(
    of_bsn_set_switch_pipeline_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_set_switch_pipeline_reply_experimenter_set(
    of_bsn_set_switch_pipeline_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_switch_pipeline_reply_experimenter_get(
    of_bsn_set_switch_pipeline_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_switch_pipeline_reply_subtype_set(
    of_bsn_set_switch_pipeline_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_set_switch_pipeline_reply_subtype_get(
    of_bsn_set_switch_pipeline_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_switch_pipeline_reply_status_set(
    of_bsn_set_switch_pipeline_reply_t *obj,
    uint32_t status);
extern void of_bsn_set_switch_pipeline_reply_status_get(
    of_bsn_set_switch_pipeline_reply_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_set_switch_pipeline_request */

extern void of_bsn_set_switch_pipeline_request_xid_set(
    of_bsn_set_switch_pipeline_request_t *obj,
    uint32_t xid);
extern void of_bsn_set_switch_pipeline_request_xid_get(
    of_bsn_set_switch_pipeline_request_t *obj,
    uint32_t *xid);

extern void of_bsn_set_switch_pipeline_request_experimenter_set(
    of_bsn_set_switch_pipeline_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_set_switch_pipeline_request_experimenter_get(
    of_bsn_set_switch_pipeline_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_set_switch_pipeline_request_subtype_set(
    of_bsn_set_switch_pipeline_request_t *obj,
    uint32_t subtype);
extern void of_bsn_set_switch_pipeline_request_subtype_get(
    of_bsn_set_switch_pipeline_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_set_switch_pipeline_request_pipeline_set(
    of_bsn_set_switch_pipeline_request_t *obj,
    of_desc_str_t pipeline);
extern void of_bsn_set_switch_pipeline_request_pipeline_get(
    of_bsn_set_switch_pipeline_request_t *obj,
    of_desc_str_t *pipeline);

/* Unified accessor functions for of_bsn_shell_command */

extern void of_bsn_shell_command_xid_set(
    of_bsn_shell_command_t *obj,
    uint32_t xid);
extern void of_bsn_shell_command_xid_get(
    of_bsn_shell_command_t *obj,
    uint32_t *xid);

extern void of_bsn_shell_command_experimenter_set(
    of_bsn_shell_command_t *obj,
    uint32_t experimenter);
extern void of_bsn_shell_command_experimenter_get(
    of_bsn_shell_command_t *obj,
    uint32_t *experimenter);

extern void of_bsn_shell_command_subtype_set(
    of_bsn_shell_command_t *obj,
    uint32_t subtype);
extern void of_bsn_shell_command_subtype_get(
    of_bsn_shell_command_t *obj,
    uint32_t *subtype);

extern void of_bsn_shell_command_service_set(
    of_bsn_shell_command_t *obj,
    uint32_t service);
extern void of_bsn_shell_command_service_get(
    of_bsn_shell_command_t *obj,
    uint32_t *service);

extern int WARN_UNUSED_RESULT of_bsn_shell_command_data_set(
    of_bsn_shell_command_t *obj,
    of_octets_t *data);
extern void of_bsn_shell_command_data_get(
    of_bsn_shell_command_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_bsn_shell_output */

extern void of_bsn_shell_output_xid_set(
    of_bsn_shell_output_t *obj,
    uint32_t xid);
extern void of_bsn_shell_output_xid_get(
    of_bsn_shell_output_t *obj,
    uint32_t *xid);

extern void of_bsn_shell_output_experimenter_set(
    of_bsn_shell_output_t *obj,
    uint32_t experimenter);
extern void of_bsn_shell_output_experimenter_get(
    of_bsn_shell_output_t *obj,
    uint32_t *experimenter);

extern void of_bsn_shell_output_subtype_set(
    of_bsn_shell_output_t *obj,
    uint32_t subtype);
extern void of_bsn_shell_output_subtype_get(
    of_bsn_shell_output_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_shell_output_data_set(
    of_bsn_shell_output_t *obj,
    of_octets_t *data);
extern void of_bsn_shell_output_data_get(
    of_bsn_shell_output_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_bsn_shell_status */

extern void of_bsn_shell_status_xid_set(
    of_bsn_shell_status_t *obj,
    uint32_t xid);
extern void of_bsn_shell_status_xid_get(
    of_bsn_shell_status_t *obj,
    uint32_t *xid);

extern void of_bsn_shell_status_experimenter_set(
    of_bsn_shell_status_t *obj,
    uint32_t experimenter);
extern void of_bsn_shell_status_experimenter_get(
    of_bsn_shell_status_t *obj,
    uint32_t *experimenter);

extern void of_bsn_shell_status_subtype_set(
    of_bsn_shell_status_t *obj,
    uint32_t subtype);
extern void of_bsn_shell_status_subtype_get(
    of_bsn_shell_status_t *obj,
    uint32_t *subtype);

extern void of_bsn_shell_status_status_set(
    of_bsn_shell_status_t *obj,
    uint32_t status);
extern void of_bsn_shell_status_status_get(
    of_bsn_shell_status_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_stats_reply */

extern void of_bsn_stats_reply_xid_set(
    of_bsn_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_stats_reply_xid_get(
    of_bsn_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_stats_reply_flags_set(
    of_bsn_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_stats_reply_flags_get(
    of_bsn_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_stats_reply_experimenter_set(
    of_bsn_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_stats_reply_experimenter_get(
    of_bsn_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_stats_reply_subtype_set(
    of_bsn_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_stats_reply_subtype_get(
    of_bsn_stats_reply_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_stats_request */

extern void of_bsn_stats_request_xid_set(
    of_bsn_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_stats_request_xid_get(
    of_bsn_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_stats_request_flags_set(
    of_bsn_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_stats_request_flags_get(
    of_bsn_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_stats_request_experimenter_set(
    of_bsn_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_stats_request_experimenter_get(
    of_bsn_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_stats_request_subtype_set(
    of_bsn_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_stats_request_subtype_get(
    of_bsn_stats_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_switch_pipeline_stats_reply */

extern void of_bsn_switch_pipeline_stats_reply_xid_set(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_switch_pipeline_stats_reply_xid_get(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_switch_pipeline_stats_reply_flags_set(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_switch_pipeline_stats_reply_flags_get(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_switch_pipeline_stats_reply_experimenter_set(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_switch_pipeline_stats_reply_experimenter_get(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_switch_pipeline_stats_reply_subtype_set(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_switch_pipeline_stats_reply_subtype_get(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_switch_pipeline_stats_reply_entries_set(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    of_list_bsn_switch_pipeline_stats_entry_t *entries);
extern void of_bsn_switch_pipeline_stats_reply_entries_bind(
    of_bsn_switch_pipeline_stats_reply_t *obj,
    of_list_bsn_switch_pipeline_stats_entry_t *entries);
extern of_list_bsn_switch_pipeline_stats_entry_t *of_bsn_switch_pipeline_stats_reply_entries_get(
    of_bsn_switch_pipeline_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_switch_pipeline_stats_request */

extern void of_bsn_switch_pipeline_stats_request_xid_set(
    of_bsn_switch_pipeline_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_switch_pipeline_stats_request_xid_get(
    of_bsn_switch_pipeline_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_switch_pipeline_stats_request_flags_set(
    of_bsn_switch_pipeline_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_switch_pipeline_stats_request_flags_get(
    of_bsn_switch_pipeline_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_switch_pipeline_stats_request_experimenter_set(
    of_bsn_switch_pipeline_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_switch_pipeline_stats_request_experimenter_get(
    of_bsn_switch_pipeline_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_switch_pipeline_stats_request_subtype_set(
    of_bsn_switch_pipeline_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_switch_pipeline_stats_request_subtype_get(
    of_bsn_switch_pipeline_stats_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_table_checksum_stats_reply */

extern void of_bsn_table_checksum_stats_reply_xid_set(
    of_bsn_table_checksum_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_table_checksum_stats_reply_xid_get(
    of_bsn_table_checksum_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_table_checksum_stats_reply_flags_set(
    of_bsn_table_checksum_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_table_checksum_stats_reply_flags_get(
    of_bsn_table_checksum_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_table_checksum_stats_reply_experimenter_set(
    of_bsn_table_checksum_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_table_checksum_stats_reply_experimenter_get(
    of_bsn_table_checksum_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_table_checksum_stats_reply_subtype_set(
    of_bsn_table_checksum_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_table_checksum_stats_reply_subtype_get(
    of_bsn_table_checksum_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_table_checksum_stats_reply_entries_set(
    of_bsn_table_checksum_stats_reply_t *obj,
    of_list_bsn_table_checksum_stats_entry_t *entries);
extern void of_bsn_table_checksum_stats_reply_entries_bind(
    of_bsn_table_checksum_stats_reply_t *obj,
    of_list_bsn_table_checksum_stats_entry_t *entries);
extern of_list_bsn_table_checksum_stats_entry_t *of_bsn_table_checksum_stats_reply_entries_get(
    of_bsn_table_checksum_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_table_checksum_stats_request */

extern void of_bsn_table_checksum_stats_request_xid_set(
    of_bsn_table_checksum_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_table_checksum_stats_request_xid_get(
    of_bsn_table_checksum_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_table_checksum_stats_request_flags_set(
    of_bsn_table_checksum_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_table_checksum_stats_request_flags_get(
    of_bsn_table_checksum_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_table_checksum_stats_request_experimenter_set(
    of_bsn_table_checksum_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_table_checksum_stats_request_experimenter_get(
    of_bsn_table_checksum_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_table_checksum_stats_request_subtype_set(
    of_bsn_table_checksum_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_table_checksum_stats_request_subtype_get(
    of_bsn_table_checksum_stats_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_table_set_buckets_size */

extern void of_bsn_table_set_buckets_size_xid_set(
    of_bsn_table_set_buckets_size_t *obj,
    uint32_t xid);
extern void of_bsn_table_set_buckets_size_xid_get(
    of_bsn_table_set_buckets_size_t *obj,
    uint32_t *xid);

extern void of_bsn_table_set_buckets_size_experimenter_set(
    of_bsn_table_set_buckets_size_t *obj,
    uint32_t experimenter);
extern void of_bsn_table_set_buckets_size_experimenter_get(
    of_bsn_table_set_buckets_size_t *obj,
    uint32_t *experimenter);

extern void of_bsn_table_set_buckets_size_subtype_set(
    of_bsn_table_set_buckets_size_t *obj,
    uint32_t subtype);
extern void of_bsn_table_set_buckets_size_subtype_get(
    of_bsn_table_set_buckets_size_t *obj,
    uint32_t *subtype);

extern void of_bsn_table_set_buckets_size_table_id_set(
    of_bsn_table_set_buckets_size_t *obj,
    uint16_t table_id);
extern void of_bsn_table_set_buckets_size_table_id_get(
    of_bsn_table_set_buckets_size_t *obj,
    uint16_t *table_id);

extern void of_bsn_table_set_buckets_size_buckets_size_set(
    of_bsn_table_set_buckets_size_t *obj,
    uint32_t buckets_size);
extern void of_bsn_table_set_buckets_size_buckets_size_get(
    of_bsn_table_set_buckets_size_t *obj,
    uint32_t *buckets_size);

/* Unified accessor functions for of_bsn_time_reply */

extern void of_bsn_time_reply_xid_set(
    of_bsn_time_reply_t *obj,
    uint32_t xid);
extern void of_bsn_time_reply_xid_get(
    of_bsn_time_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_time_reply_experimenter_set(
    of_bsn_time_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_time_reply_experimenter_get(
    of_bsn_time_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_time_reply_subtype_set(
    of_bsn_time_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_time_reply_subtype_get(
    of_bsn_time_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_time_reply_time_ms_set(
    of_bsn_time_reply_t *obj,
    uint64_t time_ms);
extern void of_bsn_time_reply_time_ms_get(
    of_bsn_time_reply_t *obj,
    uint64_t *time_ms);

/* Unified accessor functions for of_bsn_time_request */

extern void of_bsn_time_request_xid_set(
    of_bsn_time_request_t *obj,
    uint32_t xid);
extern void of_bsn_time_request_xid_get(
    of_bsn_time_request_t *obj,
    uint32_t *xid);

extern void of_bsn_time_request_experimenter_set(
    of_bsn_time_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_time_request_experimenter_get(
    of_bsn_time_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_time_request_subtype_set(
    of_bsn_time_request_t *obj,
    uint32_t subtype);
extern void of_bsn_time_request_subtype_get(
    of_bsn_time_request_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_bsn_virtual_port_create_reply */

extern void of_bsn_virtual_port_create_reply_xid_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t xid);
extern void of_bsn_virtual_port_create_reply_xid_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_virtual_port_create_reply_experimenter_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_virtual_port_create_reply_experimenter_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_virtual_port_create_reply_subtype_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_virtual_port_create_reply_subtype_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_virtual_port_create_reply_status_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t status);
extern void of_bsn_virtual_port_create_reply_status_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *status);

extern void of_bsn_virtual_port_create_reply_vport_no_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t vport_no);
extern void of_bsn_virtual_port_create_reply_vport_no_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *vport_no);

/* Unified accessor functions for of_bsn_virtual_port_create_request */

extern void of_bsn_virtual_port_create_request_xid_set(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t xid);
extern void of_bsn_virtual_port_create_request_xid_get(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t *xid);

extern void of_bsn_virtual_port_create_request_experimenter_set(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_virtual_port_create_request_experimenter_get(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_virtual_port_create_request_subtype_set(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t subtype);
extern void of_bsn_virtual_port_create_request_subtype_get(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_virtual_port_create_request_vport_set(
    of_bsn_virtual_port_create_request_t *obj,
    of_bsn_vport_q_in_q_t *vport);
extern void of_bsn_virtual_port_create_request_vport_bind(
    of_bsn_virtual_port_create_request_t *obj,
    of_bsn_vport_q_in_q_t *vport);
extern of_bsn_vport_q_in_q_t *of_bsn_virtual_port_create_request_vport_get(
    of_bsn_virtual_port_create_request_t *obj);

/* Unified accessor functions for of_bsn_virtual_port_remove_reply */

extern void of_bsn_virtual_port_remove_reply_xid_set(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t xid);
extern void of_bsn_virtual_port_remove_reply_xid_get(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_virtual_port_remove_reply_experimenter_set(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_virtual_port_remove_reply_experimenter_get(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_virtual_port_remove_reply_subtype_set(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_virtual_port_remove_reply_subtype_get(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t *subtype);

extern void of_bsn_virtual_port_remove_reply_status_set(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t status);
extern void of_bsn_virtual_port_remove_reply_status_get(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t *status);

/* Unified accessor functions for of_bsn_virtual_port_remove_request */

extern void of_bsn_virtual_port_remove_request_xid_set(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t xid);
extern void of_bsn_virtual_port_remove_request_xid_get(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t *xid);

extern void of_bsn_virtual_port_remove_request_experimenter_set(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_virtual_port_remove_request_experimenter_get(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_virtual_port_remove_request_subtype_set(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t subtype);
extern void of_bsn_virtual_port_remove_request_subtype_get(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_virtual_port_remove_request_vport_no_set(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t vport_no);
extern void of_bsn_virtual_port_remove_request_vport_no_get(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t *vport_no);

/* Unified accessor functions for of_bsn_vlan_counter_stats_reply */

extern void of_bsn_vlan_counter_stats_reply_xid_set(
    of_bsn_vlan_counter_stats_reply_t *obj,
    uint32_t xid);
extern void of_bsn_vlan_counter_stats_reply_xid_get(
    of_bsn_vlan_counter_stats_reply_t *obj,
    uint32_t *xid);

extern void of_bsn_vlan_counter_stats_reply_flags_set(
    of_bsn_vlan_counter_stats_reply_t *obj,
    uint16_t flags);
extern void of_bsn_vlan_counter_stats_reply_flags_get(
    of_bsn_vlan_counter_stats_reply_t *obj,
    uint16_t *flags);

extern void of_bsn_vlan_counter_stats_reply_experimenter_set(
    of_bsn_vlan_counter_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_bsn_vlan_counter_stats_reply_experimenter_get(
    of_bsn_vlan_counter_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_bsn_vlan_counter_stats_reply_subtype_set(
    of_bsn_vlan_counter_stats_reply_t *obj,
    uint32_t subtype);
extern void of_bsn_vlan_counter_stats_reply_subtype_get(
    of_bsn_vlan_counter_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_bsn_vlan_counter_stats_reply_entries_set(
    of_bsn_vlan_counter_stats_reply_t *obj,
    of_list_bsn_vlan_counter_stats_entry_t *entries);
extern void of_bsn_vlan_counter_stats_reply_entries_bind(
    of_bsn_vlan_counter_stats_reply_t *obj,
    of_list_bsn_vlan_counter_stats_entry_t *entries);
extern of_list_bsn_vlan_counter_stats_entry_t *of_bsn_vlan_counter_stats_reply_entries_get(
    of_bsn_vlan_counter_stats_reply_t *obj);

/* Unified accessor functions for of_bsn_vlan_counter_stats_request */

extern void of_bsn_vlan_counter_stats_request_xid_set(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint32_t xid);
extern void of_bsn_vlan_counter_stats_request_xid_get(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint32_t *xid);

extern void of_bsn_vlan_counter_stats_request_flags_set(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint16_t flags);
extern void of_bsn_vlan_counter_stats_request_flags_get(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint16_t *flags);

extern void of_bsn_vlan_counter_stats_request_experimenter_set(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint32_t experimenter);
extern void of_bsn_vlan_counter_stats_request_experimenter_get(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_bsn_vlan_counter_stats_request_subtype_set(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint32_t subtype);
extern void of_bsn_vlan_counter_stats_request_subtype_get(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint32_t *subtype);

extern void of_bsn_vlan_counter_stats_request_vlan_vid_set(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint16_t vlan_vid);
extern void of_bsn_vlan_counter_stats_request_vlan_vid_get(
    of_bsn_vlan_counter_stats_request_t *obj,
    uint16_t *vlan_vid);

/* Unified accessor functions for of_desc_stats_reply */

extern void of_desc_stats_reply_xid_set(
    of_desc_stats_reply_t *obj,
    uint32_t xid);
extern void of_desc_stats_reply_xid_get(
    of_desc_stats_reply_t *obj,
    uint32_t *xid);

extern void of_desc_stats_reply_flags_set(
    of_desc_stats_reply_t *obj,
    uint16_t flags);
extern void of_desc_stats_reply_flags_get(
    of_desc_stats_reply_t *obj,
    uint16_t *flags);

extern void of_desc_stats_reply_mfr_desc_set(
    of_desc_stats_reply_t *obj,
    of_desc_str_t mfr_desc);
extern void of_desc_stats_reply_mfr_desc_get(
    of_desc_stats_reply_t *obj,
    of_desc_str_t *mfr_desc);

extern void of_desc_stats_reply_hw_desc_set(
    of_desc_stats_reply_t *obj,
    of_desc_str_t hw_desc);
extern void of_desc_stats_reply_hw_desc_get(
    of_desc_stats_reply_t *obj,
    of_desc_str_t *hw_desc);

extern void of_desc_stats_reply_sw_desc_set(
    of_desc_stats_reply_t *obj,
    of_desc_str_t sw_desc);
extern void of_desc_stats_reply_sw_desc_get(
    of_desc_stats_reply_t *obj,
    of_desc_str_t *sw_desc);

extern void of_desc_stats_reply_serial_num_set(
    of_desc_stats_reply_t *obj,
    of_serial_num_t serial_num);
extern void of_desc_stats_reply_serial_num_get(
    of_desc_stats_reply_t *obj,
    of_serial_num_t *serial_num);

extern void of_desc_stats_reply_dp_desc_set(
    of_desc_stats_reply_t *obj,
    of_desc_str_t dp_desc);
extern void of_desc_stats_reply_dp_desc_get(
    of_desc_stats_reply_t *obj,
    of_desc_str_t *dp_desc);

/* Unified accessor functions for of_desc_stats_request */

extern void of_desc_stats_request_xid_set(
    of_desc_stats_request_t *obj,
    uint32_t xid);
extern void of_desc_stats_request_xid_get(
    of_desc_stats_request_t *obj,
    uint32_t *xid);

extern void of_desc_stats_request_flags_set(
    of_desc_stats_request_t *obj,
    uint16_t flags);
extern void of_desc_stats_request_flags_get(
    of_desc_stats_request_t *obj,
    uint16_t *flags);

/* Unified accessor functions for of_echo_reply */

extern void of_echo_reply_xid_set(
    of_echo_reply_t *obj,
    uint32_t xid);
extern void of_echo_reply_xid_get(
    of_echo_reply_t *obj,
    uint32_t *xid);

extern int WARN_UNUSED_RESULT of_echo_reply_data_set(
    of_echo_reply_t *obj,
    of_octets_t *data);
extern void of_echo_reply_data_get(
    of_echo_reply_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_echo_request */

extern void of_echo_request_xid_set(
    of_echo_request_t *obj,
    uint32_t xid);
extern void of_echo_request_xid_get(
    of_echo_request_t *obj,
    uint32_t *xid);

extern int WARN_UNUSED_RESULT of_echo_request_data_set(
    of_echo_request_t *obj,
    of_octets_t *data);
extern void of_echo_request_data_get(
    of_echo_request_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_error_msg */

extern void of_error_msg_xid_set(
    of_error_msg_t *obj,
    uint32_t xid);
extern void of_error_msg_xid_get(
    of_error_msg_t *obj,
    uint32_t *xid);

/* Unified accessor functions for of_experimenter */

extern void of_experimenter_xid_set(
    of_experimenter_t *obj,
    uint32_t xid);
extern void of_experimenter_xid_get(
    of_experimenter_t *obj,
    uint32_t *xid);

extern void of_experimenter_experimenter_set(
    of_experimenter_t *obj,
    uint32_t experimenter);
extern void of_experimenter_experimenter_get(
    of_experimenter_t *obj,
    uint32_t *experimenter);

extern void of_experimenter_subtype_set(
    of_experimenter_t *obj,
    uint32_t subtype);
extern void of_experimenter_subtype_get(
    of_experimenter_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_experimenter_data_set(
    of_experimenter_t *obj,
    of_octets_t *data);
extern void of_experimenter_data_get(
    of_experimenter_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_experimenter_error_msg */

extern void of_experimenter_error_msg_xid_set(
    of_experimenter_error_msg_t *obj,
    uint32_t xid);
extern void of_experimenter_error_msg_xid_get(
    of_experimenter_error_msg_t *obj,
    uint32_t *xid);

extern void of_experimenter_error_msg_subtype_set(
    of_experimenter_error_msg_t *obj,
    uint16_t subtype);
extern void of_experimenter_error_msg_subtype_get(
    of_experimenter_error_msg_t *obj,
    uint16_t *subtype);

extern void of_experimenter_error_msg_experimenter_set(
    of_experimenter_error_msg_t *obj,
    uint32_t experimenter);
extern void of_experimenter_error_msg_experimenter_get(
    of_experimenter_error_msg_t *obj,
    uint32_t *experimenter);

extern int WARN_UNUSED_RESULT of_experimenter_error_msg_data_set(
    of_experimenter_error_msg_t *obj,
    of_octets_t *data);
extern void of_experimenter_error_msg_data_get(
    of_experimenter_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_experimenter_multipart_reply_ofdpa */

extern void of_experimenter_multipart_reply_ofdpa_xid_set(
    of_experimenter_multipart_reply_ofdpa_t *obj,
    uint32_t xid);
extern void of_experimenter_multipart_reply_ofdpa_xid_get(
    of_experimenter_multipart_reply_ofdpa_t *obj,
    uint32_t *xid);

extern void of_experimenter_multipart_reply_ofdpa_flags_set(
    of_experimenter_multipart_reply_ofdpa_t *obj,
    uint16_t flags);
extern void of_experimenter_multipart_reply_ofdpa_flags_get(
    of_experimenter_multipart_reply_ofdpa_t *obj,
    uint16_t *flags);

extern void of_experimenter_multipart_reply_ofdpa_experimenter_set(
    of_experimenter_multipart_reply_ofdpa_t *obj,
    uint32_t experimenter);
extern void of_experimenter_multipart_reply_ofdpa_experimenter_get(
    of_experimenter_multipart_reply_ofdpa_t *obj,
    uint32_t *experimenter);

extern void of_experimenter_multipart_reply_ofdpa_subtype_set(
    of_experimenter_multipart_reply_ofdpa_t *obj,
    uint32_t subtype);
extern void of_experimenter_multipart_reply_ofdpa_subtype_get(
    of_experimenter_multipart_reply_ofdpa_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_experimenter_multipart_request_ofdpa */

extern void of_experimenter_multipart_request_ofdpa_xid_set(
    of_experimenter_multipart_request_ofdpa_t *obj,
    uint32_t xid);
extern void of_experimenter_multipart_request_ofdpa_xid_get(
    of_experimenter_multipart_request_ofdpa_t *obj,
    uint32_t *xid);

extern void of_experimenter_multipart_request_ofdpa_flags_set(
    of_experimenter_multipart_request_ofdpa_t *obj,
    uint16_t flags);
extern void of_experimenter_multipart_request_ofdpa_flags_get(
    of_experimenter_multipart_request_ofdpa_t *obj,
    uint16_t *flags);

extern void of_experimenter_multipart_request_ofdpa_experimenter_set(
    of_experimenter_multipart_request_ofdpa_t *obj,
    uint32_t experimenter);
extern void of_experimenter_multipart_request_ofdpa_experimenter_get(
    of_experimenter_multipart_request_ofdpa_t *obj,
    uint32_t *experimenter);

extern void of_experimenter_multipart_request_ofdpa_subtype_set(
    of_experimenter_multipart_request_ofdpa_t *obj,
    uint32_t subtype);
extern void of_experimenter_multipart_request_ofdpa_subtype_get(
    of_experimenter_multipart_request_ofdpa_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_experimenter_ofdpa */

extern void of_experimenter_ofdpa_xid_set(
    of_experimenter_ofdpa_t *obj,
    uint32_t xid);
extern void of_experimenter_ofdpa_xid_get(
    of_experimenter_ofdpa_t *obj,
    uint32_t *xid);

extern void of_experimenter_ofdpa_experimenter_set(
    of_experimenter_ofdpa_t *obj,
    uint32_t experimenter);
extern void of_experimenter_ofdpa_experimenter_get(
    of_experimenter_ofdpa_t *obj,
    uint32_t *experimenter);

extern void of_experimenter_ofdpa_subtype_set(
    of_experimenter_ofdpa_t *obj,
    uint32_t subtype);
extern void of_experimenter_ofdpa_subtype_get(
    of_experimenter_ofdpa_t *obj,
    uint32_t *subtype);

extern void of_experimenter_ofdpa_mod_command_set(
    of_experimenter_ofdpa_t *obj,
    uint32_t mod_command);
extern void of_experimenter_ofdpa_mod_command_get(
    of_experimenter_ofdpa_t *obj,
    uint32_t *mod_command);

extern int WARN_UNUSED_RESULT of_experimenter_ofdpa_data_set(
    of_experimenter_ofdpa_t *obj,
    of_octets_t *data);
extern void of_experimenter_ofdpa_data_get(
    of_experimenter_ofdpa_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_experimenter_stats_reply */

extern void of_experimenter_stats_reply_xid_set(
    of_experimenter_stats_reply_t *obj,
    uint32_t xid);
extern void of_experimenter_stats_reply_xid_get(
    of_experimenter_stats_reply_t *obj,
    uint32_t *xid);

extern void of_experimenter_stats_reply_flags_set(
    of_experimenter_stats_reply_t *obj,
    uint16_t flags);
extern void of_experimenter_stats_reply_flags_get(
    of_experimenter_stats_reply_t *obj,
    uint16_t *flags);

extern void of_experimenter_stats_reply_experimenter_set(
    of_experimenter_stats_reply_t *obj,
    uint32_t experimenter);
extern void of_experimenter_stats_reply_experimenter_get(
    of_experimenter_stats_reply_t *obj,
    uint32_t *experimenter);

extern void of_experimenter_stats_reply_subtype_set(
    of_experimenter_stats_reply_t *obj,
    uint32_t subtype);
extern void of_experimenter_stats_reply_subtype_get(
    of_experimenter_stats_reply_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_experimenter_stats_reply_data_set(
    of_experimenter_stats_reply_t *obj,
    of_octets_t *data);
extern void of_experimenter_stats_reply_data_get(
    of_experimenter_stats_reply_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_experimenter_stats_request */

extern void of_experimenter_stats_request_xid_set(
    of_experimenter_stats_request_t *obj,
    uint32_t xid);
extern void of_experimenter_stats_request_xid_get(
    of_experimenter_stats_request_t *obj,
    uint32_t *xid);

extern void of_experimenter_stats_request_flags_set(
    of_experimenter_stats_request_t *obj,
    uint16_t flags);
extern void of_experimenter_stats_request_flags_get(
    of_experimenter_stats_request_t *obj,
    uint16_t *flags);

extern void of_experimenter_stats_request_experimenter_set(
    of_experimenter_stats_request_t *obj,
    uint32_t experimenter);
extern void of_experimenter_stats_request_experimenter_get(
    of_experimenter_stats_request_t *obj,
    uint32_t *experimenter);

extern void of_experimenter_stats_request_subtype_set(
    of_experimenter_stats_request_t *obj,
    uint32_t subtype);
extern void of_experimenter_stats_request_subtype_get(
    of_experimenter_stats_request_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_experimenter_stats_request_data_set(
    of_experimenter_stats_request_t *obj,
    of_octets_t *data);
extern void of_experimenter_stats_request_data_get(
    of_experimenter_stats_request_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_features_reply */

extern void of_features_reply_xid_set(
    of_features_reply_t *obj,
    uint32_t xid);
extern void of_features_reply_xid_get(
    of_features_reply_t *obj,
    uint32_t *xid);

extern void of_features_reply_datapath_id_set(
    of_features_reply_t *obj,
    uint64_t datapath_id);
extern void of_features_reply_datapath_id_get(
    of_features_reply_t *obj,
    uint64_t *datapath_id);

extern void of_features_reply_n_buffers_set(
    of_features_reply_t *obj,
    uint32_t n_buffers);
extern void of_features_reply_n_buffers_get(
    of_features_reply_t *obj,
    uint32_t *n_buffers);

extern void of_features_reply_n_tables_set(
    of_features_reply_t *obj,
    uint8_t n_tables);
extern void of_features_reply_n_tables_get(
    of_features_reply_t *obj,
    uint8_t *n_tables);

extern void of_features_reply_auxiliary_id_set(
    of_features_reply_t *obj,
    uint8_t auxiliary_id);
extern void of_features_reply_auxiliary_id_get(
    of_features_reply_t *obj,
    uint8_t *auxiliary_id);

extern void of_features_reply_capabilities_set(
    of_features_reply_t *obj,
    uint32_t capabilities);
extern void of_features_reply_capabilities_get(
    of_features_reply_t *obj,
    uint32_t *capabilities);

extern void of_features_reply_reserved_set(
    of_features_reply_t *obj,
    uint32_t reserved);
extern void of_features_reply_reserved_get(
    of_features_reply_t *obj,
    uint32_t *reserved);

extern int WARN_UNUSED_RESULT of_features_reply_ports_set(
    of_features_reply_t *obj,
    of_list_port_desc_t *ports);
extern void of_features_reply_ports_bind(
    of_features_reply_t *obj,
    of_list_port_desc_t *ports);
extern of_list_port_desc_t *of_features_reply_ports_get(
    of_features_reply_t *obj);

extern void of_features_reply_actions_set(
    of_features_reply_t *obj,
    uint32_t actions);
extern void of_features_reply_actions_get(
    of_features_reply_t *obj,
    uint32_t *actions);

/* Unified accessor functions for of_features_request */

extern void of_features_request_xid_set(
    of_features_request_t *obj,
    uint32_t xid);
extern void of_features_request_xid_get(
    of_features_request_t *obj,
    uint32_t *xid);

/* Unified accessor functions for of_flow_add */

extern void of_flow_add_xid_set(
    of_flow_add_t *obj,
    uint32_t xid);
extern void of_flow_add_xid_get(
    of_flow_add_t *obj,
    uint32_t *xid);

extern void of_flow_add_cookie_set(
    of_flow_add_t *obj,
    uint64_t cookie);
extern void of_flow_add_cookie_get(
    of_flow_add_t *obj,
    uint64_t *cookie);

extern void of_flow_add_cookie_mask_set(
    of_flow_add_t *obj,
    uint64_t cookie_mask);
extern void of_flow_add_cookie_mask_get(
    of_flow_add_t *obj,
    uint64_t *cookie_mask);

extern void of_flow_add_table_id_set(
    of_flow_add_t *obj,
    uint8_t table_id);
extern void of_flow_add_table_id_get(
    of_flow_add_t *obj,
    uint8_t *table_id);

extern void of_flow_add_idle_timeout_set(
    of_flow_add_t *obj,
    uint16_t idle_timeout);
extern void of_flow_add_idle_timeout_get(
    of_flow_add_t *obj,
    uint16_t *idle_timeout);

extern void of_flow_add_hard_timeout_set(
    of_flow_add_t *obj,
    uint16_t hard_timeout);
extern void of_flow_add_hard_timeout_get(
    of_flow_add_t *obj,
    uint16_t *hard_timeout);

extern void of_flow_add_priority_set(
    of_flow_add_t *obj,
    uint16_t priority);
extern void of_flow_add_priority_get(
    of_flow_add_t *obj,
    uint16_t *priority);

extern void of_flow_add_buffer_id_set(
    of_flow_add_t *obj,
    uint32_t buffer_id);
extern void of_flow_add_buffer_id_get(
    of_flow_add_t *obj,
    uint32_t *buffer_id);

extern void of_flow_add_out_port_set(
    of_flow_add_t *obj,
    of_port_no_t out_port);
extern void of_flow_add_out_port_get(
    of_flow_add_t *obj,
    of_port_no_t *out_port);

extern void of_flow_add_out_group_set(
    of_flow_add_t *obj,
    uint32_t out_group);
extern void of_flow_add_out_group_get(
    of_flow_add_t *obj,
    uint32_t *out_group);

extern void of_flow_add_flags_set(
    of_flow_add_t *obj,
    uint16_t flags);
extern void of_flow_add_flags_get(
    of_flow_add_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_flow_add_match_set(
    of_flow_add_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_add_match_get(
    of_flow_add_t *obj,
    of_match_t *match);

extern int WARN_UNUSED_RESULT of_flow_add_instructions_set(
    of_flow_add_t *obj,
    of_list_instruction_t *instructions);
extern void of_flow_add_instructions_bind(
    of_flow_add_t *obj,
    of_list_instruction_t *instructions);
extern of_list_instruction_t *of_flow_add_instructions_get(
    of_flow_add_t *obj);

extern int WARN_UNUSED_RESULT of_flow_add_actions_set(
    of_flow_add_t *obj,
    of_list_action_t *actions);
extern void of_flow_add_actions_bind(
    of_flow_add_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_flow_add_actions_get(
    of_flow_add_t *obj);

/* Unified accessor functions for of_flow_delete */

extern void of_flow_delete_xid_set(
    of_flow_delete_t *obj,
    uint32_t xid);
extern void of_flow_delete_xid_get(
    of_flow_delete_t *obj,
    uint32_t *xid);

extern void of_flow_delete_cookie_set(
    of_flow_delete_t *obj,
    uint64_t cookie);
extern void of_flow_delete_cookie_get(
    of_flow_delete_t *obj,
    uint64_t *cookie);

extern void of_flow_delete_cookie_mask_set(
    of_flow_delete_t *obj,
    uint64_t cookie_mask);
extern void of_flow_delete_cookie_mask_get(
    of_flow_delete_t *obj,
    uint64_t *cookie_mask);

extern void of_flow_delete_table_id_set(
    of_flow_delete_t *obj,
    uint8_t table_id);
extern void of_flow_delete_table_id_get(
    of_flow_delete_t *obj,
    uint8_t *table_id);

extern void of_flow_delete_idle_timeout_set(
    of_flow_delete_t *obj,
    uint16_t idle_timeout);
extern void of_flow_delete_idle_timeout_get(
    of_flow_delete_t *obj,
    uint16_t *idle_timeout);

extern void of_flow_delete_hard_timeout_set(
    of_flow_delete_t *obj,
    uint16_t hard_timeout);
extern void of_flow_delete_hard_timeout_get(
    of_flow_delete_t *obj,
    uint16_t *hard_timeout);

extern void of_flow_delete_priority_set(
    of_flow_delete_t *obj,
    uint16_t priority);
extern void of_flow_delete_priority_get(
    of_flow_delete_t *obj,
    uint16_t *priority);

extern void of_flow_delete_buffer_id_set(
    of_flow_delete_t *obj,
    uint32_t buffer_id);
extern void of_flow_delete_buffer_id_get(
    of_flow_delete_t *obj,
    uint32_t *buffer_id);

extern void of_flow_delete_out_port_set(
    of_flow_delete_t *obj,
    of_port_no_t out_port);
extern void of_flow_delete_out_port_get(
    of_flow_delete_t *obj,
    of_port_no_t *out_port);

extern void of_flow_delete_out_group_set(
    of_flow_delete_t *obj,
    uint32_t out_group);
extern void of_flow_delete_out_group_get(
    of_flow_delete_t *obj,
    uint32_t *out_group);

extern void of_flow_delete_flags_set(
    of_flow_delete_t *obj,
    uint16_t flags);
extern void of_flow_delete_flags_get(
    of_flow_delete_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_flow_delete_match_set(
    of_flow_delete_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_delete_match_get(
    of_flow_delete_t *obj,
    of_match_t *match);

extern int WARN_UNUSED_RESULT of_flow_delete_instructions_set(
    of_flow_delete_t *obj,
    of_list_instruction_t *instructions);
extern void of_flow_delete_instructions_bind(
    of_flow_delete_t *obj,
    of_list_instruction_t *instructions);
extern of_list_instruction_t *of_flow_delete_instructions_get(
    of_flow_delete_t *obj);

extern int WARN_UNUSED_RESULT of_flow_delete_actions_set(
    of_flow_delete_t *obj,
    of_list_action_t *actions);
extern void of_flow_delete_actions_bind(
    of_flow_delete_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_flow_delete_actions_get(
    of_flow_delete_t *obj);

/* Unified accessor functions for of_flow_delete_strict */

extern void of_flow_delete_strict_xid_set(
    of_flow_delete_strict_t *obj,
    uint32_t xid);
extern void of_flow_delete_strict_xid_get(
    of_flow_delete_strict_t *obj,
    uint32_t *xid);

extern void of_flow_delete_strict_cookie_set(
    of_flow_delete_strict_t *obj,
    uint64_t cookie);
extern void of_flow_delete_strict_cookie_get(
    of_flow_delete_strict_t *obj,
    uint64_t *cookie);

extern void of_flow_delete_strict_cookie_mask_set(
    of_flow_delete_strict_t *obj,
    uint64_t cookie_mask);
extern void of_flow_delete_strict_cookie_mask_get(
    of_flow_delete_strict_t *obj,
    uint64_t *cookie_mask);

extern void of_flow_delete_strict_table_id_set(
    of_flow_delete_strict_t *obj,
    uint8_t table_id);
extern void of_flow_delete_strict_table_id_get(
    of_flow_delete_strict_t *obj,
    uint8_t *table_id);

extern void of_flow_delete_strict_idle_timeout_set(
    of_flow_delete_strict_t *obj,
    uint16_t idle_timeout);
extern void of_flow_delete_strict_idle_timeout_get(
    of_flow_delete_strict_t *obj,
    uint16_t *idle_timeout);

extern void of_flow_delete_strict_hard_timeout_set(
    of_flow_delete_strict_t *obj,
    uint16_t hard_timeout);
extern void of_flow_delete_strict_hard_timeout_get(
    of_flow_delete_strict_t *obj,
    uint16_t *hard_timeout);

extern void of_flow_delete_strict_priority_set(
    of_flow_delete_strict_t *obj,
    uint16_t priority);
extern void of_flow_delete_strict_priority_get(
    of_flow_delete_strict_t *obj,
    uint16_t *priority);

extern void of_flow_delete_strict_buffer_id_set(
    of_flow_delete_strict_t *obj,
    uint32_t buffer_id);
extern void of_flow_delete_strict_buffer_id_get(
    of_flow_delete_strict_t *obj,
    uint32_t *buffer_id);

extern void of_flow_delete_strict_out_port_set(
    of_flow_delete_strict_t *obj,
    of_port_no_t out_port);
extern void of_flow_delete_strict_out_port_get(
    of_flow_delete_strict_t *obj,
    of_port_no_t *out_port);

extern void of_flow_delete_strict_out_group_set(
    of_flow_delete_strict_t *obj,
    uint32_t out_group);
extern void of_flow_delete_strict_out_group_get(
    of_flow_delete_strict_t *obj,
    uint32_t *out_group);

extern void of_flow_delete_strict_flags_set(
    of_flow_delete_strict_t *obj,
    uint16_t flags);
extern void of_flow_delete_strict_flags_get(
    of_flow_delete_strict_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_flow_delete_strict_match_set(
    of_flow_delete_strict_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_delete_strict_match_get(
    of_flow_delete_strict_t *obj,
    of_match_t *match);

extern int WARN_UNUSED_RESULT of_flow_delete_strict_instructions_set(
    of_flow_delete_strict_t *obj,
    of_list_instruction_t *instructions);
extern void of_flow_delete_strict_instructions_bind(
    of_flow_delete_strict_t *obj,
    of_list_instruction_t *instructions);
extern of_list_instruction_t *of_flow_delete_strict_instructions_get(
    of_flow_delete_strict_t *obj);

extern int WARN_UNUSED_RESULT of_flow_delete_strict_actions_set(
    of_flow_delete_strict_t *obj,
    of_list_action_t *actions);
extern void of_flow_delete_strict_actions_bind(
    of_flow_delete_strict_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_flow_delete_strict_actions_get(
    of_flow_delete_strict_t *obj);

/* Unified accessor functions for of_flow_mod */

extern void of_flow_mod_xid_set(
    of_flow_mod_t *obj,
    uint32_t xid);
extern void of_flow_mod_xid_get(
    of_flow_mod_t *obj,
    uint32_t *xid);

extern void of_flow_mod_cookie_set(
    of_flow_mod_t *obj,
    uint64_t cookie);
extern void of_flow_mod_cookie_get(
    of_flow_mod_t *obj,
    uint64_t *cookie);

extern void of_flow_mod_cookie_mask_set(
    of_flow_mod_t *obj,
    uint64_t cookie_mask);
extern void of_flow_mod_cookie_mask_get(
    of_flow_mod_t *obj,
    uint64_t *cookie_mask);

extern void of_flow_mod_table_id_set(
    of_flow_mod_t *obj,
    uint8_t table_id);
extern void of_flow_mod_table_id_get(
    of_flow_mod_t *obj,
    uint8_t *table_id);

extern void of_flow_mod_idle_timeout_set(
    of_flow_mod_t *obj,
    uint16_t idle_timeout);
extern void of_flow_mod_idle_timeout_get(
    of_flow_mod_t *obj,
    uint16_t *idle_timeout);

extern void of_flow_mod_hard_timeout_set(
    of_flow_mod_t *obj,
    uint16_t hard_timeout);
extern void of_flow_mod_hard_timeout_get(
    of_flow_mod_t *obj,
    uint16_t *hard_timeout);

extern void of_flow_mod_priority_set(
    of_flow_mod_t *obj,
    uint16_t priority);
extern void of_flow_mod_priority_get(
    of_flow_mod_t *obj,
    uint16_t *priority);

extern void of_flow_mod_buffer_id_set(
    of_flow_mod_t *obj,
    uint32_t buffer_id);
extern void of_flow_mod_buffer_id_get(
    of_flow_mod_t *obj,
    uint32_t *buffer_id);

extern void of_flow_mod_out_port_set(
    of_flow_mod_t *obj,
    of_port_no_t out_port);
extern void of_flow_mod_out_port_get(
    of_flow_mod_t *obj,
    of_port_no_t *out_port);

extern void of_flow_mod_out_group_set(
    of_flow_mod_t *obj,
    uint32_t out_group);
extern void of_flow_mod_out_group_get(
    of_flow_mod_t *obj,
    uint32_t *out_group);

extern void of_flow_mod_flags_set(
    of_flow_mod_t *obj,
    uint16_t flags);
extern void of_flow_mod_flags_get(
    of_flow_mod_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_flow_mod_match_set(
    of_flow_mod_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_mod_match_get(
    of_flow_mod_t *obj,
    of_match_t *match);

extern int WARN_UNUSED_RESULT of_flow_mod_instructions_set(
    of_flow_mod_t *obj,
    of_list_instruction_t *instructions);
extern void of_flow_mod_instructions_bind(
    of_flow_mod_t *obj,
    of_list_instruction_t *instructions);
extern of_list_instruction_t *of_flow_mod_instructions_get(
    of_flow_mod_t *obj);

extern int WARN_UNUSED_RESULT of_flow_mod_actions_set(
    of_flow_mod_t *obj,
    of_list_action_t *actions);
extern void of_flow_mod_actions_bind(
    of_flow_mod_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_flow_mod_actions_get(
    of_flow_mod_t *obj);

/* Unified accessor functions for of_flow_mod_failed_error_msg */

extern void of_flow_mod_failed_error_msg_xid_set(
    of_flow_mod_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_flow_mod_failed_error_msg_xid_get(
    of_flow_mod_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_flow_mod_failed_error_msg_code_set(
    of_flow_mod_failed_error_msg_t *obj,
    uint16_t code);
extern void of_flow_mod_failed_error_msg_code_get(
    of_flow_mod_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_flow_mod_failed_error_msg_data_set(
    of_flow_mod_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_flow_mod_failed_error_msg_data_get(
    of_flow_mod_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_flow_modify */

extern void of_flow_modify_xid_set(
    of_flow_modify_t *obj,
    uint32_t xid);
extern void of_flow_modify_xid_get(
    of_flow_modify_t *obj,
    uint32_t *xid);

extern void of_flow_modify_cookie_set(
    of_flow_modify_t *obj,
    uint64_t cookie);
extern void of_flow_modify_cookie_get(
    of_flow_modify_t *obj,
    uint64_t *cookie);

extern void of_flow_modify_cookie_mask_set(
    of_flow_modify_t *obj,
    uint64_t cookie_mask);
extern void of_flow_modify_cookie_mask_get(
    of_flow_modify_t *obj,
    uint64_t *cookie_mask);

extern void of_flow_modify_table_id_set(
    of_flow_modify_t *obj,
    uint8_t table_id);
extern void of_flow_modify_table_id_get(
    of_flow_modify_t *obj,
    uint8_t *table_id);

extern void of_flow_modify_idle_timeout_set(
    of_flow_modify_t *obj,
    uint16_t idle_timeout);
extern void of_flow_modify_idle_timeout_get(
    of_flow_modify_t *obj,
    uint16_t *idle_timeout);

extern void of_flow_modify_hard_timeout_set(
    of_flow_modify_t *obj,
    uint16_t hard_timeout);
extern void of_flow_modify_hard_timeout_get(
    of_flow_modify_t *obj,
    uint16_t *hard_timeout);

extern void of_flow_modify_priority_set(
    of_flow_modify_t *obj,
    uint16_t priority);
extern void of_flow_modify_priority_get(
    of_flow_modify_t *obj,
    uint16_t *priority);

extern void of_flow_modify_buffer_id_set(
    of_flow_modify_t *obj,
    uint32_t buffer_id);
extern void of_flow_modify_buffer_id_get(
    of_flow_modify_t *obj,
    uint32_t *buffer_id);

extern void of_flow_modify_out_port_set(
    of_flow_modify_t *obj,
    of_port_no_t out_port);
extern void of_flow_modify_out_port_get(
    of_flow_modify_t *obj,
    of_port_no_t *out_port);

extern void of_flow_modify_out_group_set(
    of_flow_modify_t *obj,
    uint32_t out_group);
extern void of_flow_modify_out_group_get(
    of_flow_modify_t *obj,
    uint32_t *out_group);

extern void of_flow_modify_flags_set(
    of_flow_modify_t *obj,
    uint16_t flags);
extern void of_flow_modify_flags_get(
    of_flow_modify_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_flow_modify_match_set(
    of_flow_modify_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_modify_match_get(
    of_flow_modify_t *obj,
    of_match_t *match);

extern int WARN_UNUSED_RESULT of_flow_modify_instructions_set(
    of_flow_modify_t *obj,
    of_list_instruction_t *instructions);
extern void of_flow_modify_instructions_bind(
    of_flow_modify_t *obj,
    of_list_instruction_t *instructions);
extern of_list_instruction_t *of_flow_modify_instructions_get(
    of_flow_modify_t *obj);

extern int WARN_UNUSED_RESULT of_flow_modify_actions_set(
    of_flow_modify_t *obj,
    of_list_action_t *actions);
extern void of_flow_modify_actions_bind(
    of_flow_modify_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_flow_modify_actions_get(
    of_flow_modify_t *obj);

/* Unified accessor functions for of_flow_modify_strict */

extern void of_flow_modify_strict_xid_set(
    of_flow_modify_strict_t *obj,
    uint32_t xid);
extern void of_flow_modify_strict_xid_get(
    of_flow_modify_strict_t *obj,
    uint32_t *xid);

extern void of_flow_modify_strict_cookie_set(
    of_flow_modify_strict_t *obj,
    uint64_t cookie);
extern void of_flow_modify_strict_cookie_get(
    of_flow_modify_strict_t *obj,
    uint64_t *cookie);

extern void of_flow_modify_strict_cookie_mask_set(
    of_flow_modify_strict_t *obj,
    uint64_t cookie_mask);
extern void of_flow_modify_strict_cookie_mask_get(
    of_flow_modify_strict_t *obj,
    uint64_t *cookie_mask);

extern void of_flow_modify_strict_table_id_set(
    of_flow_modify_strict_t *obj,
    uint8_t table_id);
extern void of_flow_modify_strict_table_id_get(
    of_flow_modify_strict_t *obj,
    uint8_t *table_id);

extern void of_flow_modify_strict_idle_timeout_set(
    of_flow_modify_strict_t *obj,
    uint16_t idle_timeout);
extern void of_flow_modify_strict_idle_timeout_get(
    of_flow_modify_strict_t *obj,
    uint16_t *idle_timeout);

extern void of_flow_modify_strict_hard_timeout_set(
    of_flow_modify_strict_t *obj,
    uint16_t hard_timeout);
extern void of_flow_modify_strict_hard_timeout_get(
    of_flow_modify_strict_t *obj,
    uint16_t *hard_timeout);

extern void of_flow_modify_strict_priority_set(
    of_flow_modify_strict_t *obj,
    uint16_t priority);
extern void of_flow_modify_strict_priority_get(
    of_flow_modify_strict_t *obj,
    uint16_t *priority);

extern void of_flow_modify_strict_buffer_id_set(
    of_flow_modify_strict_t *obj,
    uint32_t buffer_id);
extern void of_flow_modify_strict_buffer_id_get(
    of_flow_modify_strict_t *obj,
    uint32_t *buffer_id);

extern void of_flow_modify_strict_out_port_set(
    of_flow_modify_strict_t *obj,
    of_port_no_t out_port);
extern void of_flow_modify_strict_out_port_get(
    of_flow_modify_strict_t *obj,
    of_port_no_t *out_port);

extern void of_flow_modify_strict_out_group_set(
    of_flow_modify_strict_t *obj,
    uint32_t out_group);
extern void of_flow_modify_strict_out_group_get(
    of_flow_modify_strict_t *obj,
    uint32_t *out_group);

extern void of_flow_modify_strict_flags_set(
    of_flow_modify_strict_t *obj,
    uint16_t flags);
extern void of_flow_modify_strict_flags_get(
    of_flow_modify_strict_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_flow_modify_strict_match_set(
    of_flow_modify_strict_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_modify_strict_match_get(
    of_flow_modify_strict_t *obj,
    of_match_t *match);

extern int WARN_UNUSED_RESULT of_flow_modify_strict_instructions_set(
    of_flow_modify_strict_t *obj,
    of_list_instruction_t *instructions);
extern void of_flow_modify_strict_instructions_bind(
    of_flow_modify_strict_t *obj,
    of_list_instruction_t *instructions);
extern of_list_instruction_t *of_flow_modify_strict_instructions_get(
    of_flow_modify_strict_t *obj);

extern int WARN_UNUSED_RESULT of_flow_modify_strict_actions_set(
    of_flow_modify_strict_t *obj,
    of_list_action_t *actions);
extern void of_flow_modify_strict_actions_bind(
    of_flow_modify_strict_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_flow_modify_strict_actions_get(
    of_flow_modify_strict_t *obj);

/* Unified accessor functions for of_flow_removed */

extern void of_flow_removed_xid_set(
    of_flow_removed_t *obj,
    uint32_t xid);
extern void of_flow_removed_xid_get(
    of_flow_removed_t *obj,
    uint32_t *xid);

extern void of_flow_removed_cookie_set(
    of_flow_removed_t *obj,
    uint64_t cookie);
extern void of_flow_removed_cookie_get(
    of_flow_removed_t *obj,
    uint64_t *cookie);

extern void of_flow_removed_priority_set(
    of_flow_removed_t *obj,
    uint16_t priority);
extern void of_flow_removed_priority_get(
    of_flow_removed_t *obj,
    uint16_t *priority);

extern void of_flow_removed_reason_set(
    of_flow_removed_t *obj,
    uint8_t reason);
extern void of_flow_removed_reason_get(
    of_flow_removed_t *obj,
    uint8_t *reason);

extern void of_flow_removed_table_id_set(
    of_flow_removed_t *obj,
    uint8_t table_id);
extern void of_flow_removed_table_id_get(
    of_flow_removed_t *obj,
    uint8_t *table_id);

extern void of_flow_removed_duration_sec_set(
    of_flow_removed_t *obj,
    uint32_t duration_sec);
extern void of_flow_removed_duration_sec_get(
    of_flow_removed_t *obj,
    uint32_t *duration_sec);

extern void of_flow_removed_duration_nsec_set(
    of_flow_removed_t *obj,
    uint32_t duration_nsec);
extern void of_flow_removed_duration_nsec_get(
    of_flow_removed_t *obj,
    uint32_t *duration_nsec);

extern void of_flow_removed_idle_timeout_set(
    of_flow_removed_t *obj,
    uint16_t idle_timeout);
extern void of_flow_removed_idle_timeout_get(
    of_flow_removed_t *obj,
    uint16_t *idle_timeout);

extern void of_flow_removed_hard_timeout_set(
    of_flow_removed_t *obj,
    uint16_t hard_timeout);
extern void of_flow_removed_hard_timeout_get(
    of_flow_removed_t *obj,
    uint16_t *hard_timeout);

extern void of_flow_removed_packet_count_set(
    of_flow_removed_t *obj,
    uint64_t packet_count);
extern void of_flow_removed_packet_count_get(
    of_flow_removed_t *obj,
    uint64_t *packet_count);

extern void of_flow_removed_byte_count_set(
    of_flow_removed_t *obj,
    uint64_t byte_count);
extern void of_flow_removed_byte_count_get(
    of_flow_removed_t *obj,
    uint64_t *byte_count);

extern int WARN_UNUSED_RESULT of_flow_removed_match_set(
    of_flow_removed_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_removed_match_get(
    of_flow_removed_t *obj,
    of_match_t *match);

/* Unified accessor functions for of_flow_stats_reply */

extern void of_flow_stats_reply_xid_set(
    of_flow_stats_reply_t *obj,
    uint32_t xid);
extern void of_flow_stats_reply_xid_get(
    of_flow_stats_reply_t *obj,
    uint32_t *xid);

extern void of_flow_stats_reply_flags_set(
    of_flow_stats_reply_t *obj,
    uint16_t flags);
extern void of_flow_stats_reply_flags_get(
    of_flow_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_flow_stats_reply_entries_set(
    of_flow_stats_reply_t *obj,
    of_list_flow_stats_entry_t *entries);
extern void of_flow_stats_reply_entries_bind(
    of_flow_stats_reply_t *obj,
    of_list_flow_stats_entry_t *entries);
extern of_list_flow_stats_entry_t *of_flow_stats_reply_entries_get(
    of_flow_stats_reply_t *obj);

/* Unified accessor functions for of_flow_stats_request */

extern void of_flow_stats_request_xid_set(
    of_flow_stats_request_t *obj,
    uint32_t xid);
extern void of_flow_stats_request_xid_get(
    of_flow_stats_request_t *obj,
    uint32_t *xid);

extern void of_flow_stats_request_flags_set(
    of_flow_stats_request_t *obj,
    uint16_t flags);
extern void of_flow_stats_request_flags_get(
    of_flow_stats_request_t *obj,
    uint16_t *flags);

extern void of_flow_stats_request_table_id_set(
    of_flow_stats_request_t *obj,
    uint8_t table_id);
extern void of_flow_stats_request_table_id_get(
    of_flow_stats_request_t *obj,
    uint8_t *table_id);

extern void of_flow_stats_request_out_port_set(
    of_flow_stats_request_t *obj,
    of_port_no_t out_port);
extern void of_flow_stats_request_out_port_get(
    of_flow_stats_request_t *obj,
    of_port_no_t *out_port);

extern void of_flow_stats_request_out_group_set(
    of_flow_stats_request_t *obj,
    uint32_t out_group);
extern void of_flow_stats_request_out_group_get(
    of_flow_stats_request_t *obj,
    uint32_t *out_group);

extern void of_flow_stats_request_cookie_set(
    of_flow_stats_request_t *obj,
    uint64_t cookie);
extern void of_flow_stats_request_cookie_get(
    of_flow_stats_request_t *obj,
    uint64_t *cookie);

extern void of_flow_stats_request_cookie_mask_set(
    of_flow_stats_request_t *obj,
    uint64_t cookie_mask);
extern void of_flow_stats_request_cookie_mask_get(
    of_flow_stats_request_t *obj,
    uint64_t *cookie_mask);

extern int WARN_UNUSED_RESULT of_flow_stats_request_match_set(
    of_flow_stats_request_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_stats_request_match_get(
    of_flow_stats_request_t *obj,
    of_match_t *match);

/* Unified accessor functions for of_get_config_reply */

extern void of_get_config_reply_xid_set(
    of_get_config_reply_t *obj,
    uint32_t xid);
extern void of_get_config_reply_xid_get(
    of_get_config_reply_t *obj,
    uint32_t *xid);

extern void of_get_config_reply_flags_set(
    of_get_config_reply_t *obj,
    uint16_t flags);
extern void of_get_config_reply_flags_get(
    of_get_config_reply_t *obj,
    uint16_t *flags);

extern void of_get_config_reply_miss_send_len_set(
    of_get_config_reply_t *obj,
    uint16_t miss_send_len);
extern void of_get_config_reply_miss_send_len_get(
    of_get_config_reply_t *obj,
    uint16_t *miss_send_len);

/* Unified accessor functions for of_get_config_request */

extern void of_get_config_request_xid_set(
    of_get_config_request_t *obj,
    uint32_t xid);
extern void of_get_config_request_xid_get(
    of_get_config_request_t *obj,
    uint32_t *xid);

/* Unified accessor functions for of_group_add */

extern void of_group_add_xid_set(
    of_group_add_t *obj,
    uint32_t xid);
extern void of_group_add_xid_get(
    of_group_add_t *obj,
    uint32_t *xid);

extern void of_group_add_group_type_set(
    of_group_add_t *obj,
    uint8_t group_type);
extern void of_group_add_group_type_get(
    of_group_add_t *obj,
    uint8_t *group_type);

extern void of_group_add_group_id_set(
    of_group_add_t *obj,
    uint32_t group_id);
extern void of_group_add_group_id_get(
    of_group_add_t *obj,
    uint32_t *group_id);

extern int WARN_UNUSED_RESULT of_group_add_buckets_set(
    of_group_add_t *obj,
    of_list_bucket_t *buckets);
extern void of_group_add_buckets_bind(
    of_group_add_t *obj,
    of_list_bucket_t *buckets);
extern of_list_bucket_t *of_group_add_buckets_get(
    of_group_add_t *obj);

/* Unified accessor functions for of_group_delete */

extern void of_group_delete_xid_set(
    of_group_delete_t *obj,
    uint32_t xid);
extern void of_group_delete_xid_get(
    of_group_delete_t *obj,
    uint32_t *xid);

extern void of_group_delete_group_type_set(
    of_group_delete_t *obj,
    uint8_t group_type);
extern void of_group_delete_group_type_get(
    of_group_delete_t *obj,
    uint8_t *group_type);

extern void of_group_delete_group_id_set(
    of_group_delete_t *obj,
    uint32_t group_id);
extern void of_group_delete_group_id_get(
    of_group_delete_t *obj,
    uint32_t *group_id);

extern int WARN_UNUSED_RESULT of_group_delete_buckets_set(
    of_group_delete_t *obj,
    of_list_bucket_t *buckets);
extern void of_group_delete_buckets_bind(
    of_group_delete_t *obj,
    of_list_bucket_t *buckets);
extern of_list_bucket_t *of_group_delete_buckets_get(
    of_group_delete_t *obj);

/* Unified accessor functions for of_group_desc_stats_reply */

extern void of_group_desc_stats_reply_xid_set(
    of_group_desc_stats_reply_t *obj,
    uint32_t xid);
extern void of_group_desc_stats_reply_xid_get(
    of_group_desc_stats_reply_t *obj,
    uint32_t *xid);

extern void of_group_desc_stats_reply_flags_set(
    of_group_desc_stats_reply_t *obj,
    uint16_t flags);
extern void of_group_desc_stats_reply_flags_get(
    of_group_desc_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_group_desc_stats_reply_entries_set(
    of_group_desc_stats_reply_t *obj,
    of_list_group_desc_stats_entry_t *entries);
extern void of_group_desc_stats_reply_entries_bind(
    of_group_desc_stats_reply_t *obj,
    of_list_group_desc_stats_entry_t *entries);
extern of_list_group_desc_stats_entry_t *of_group_desc_stats_reply_entries_get(
    of_group_desc_stats_reply_t *obj);

/* Unified accessor functions for of_group_desc_stats_request */

extern void of_group_desc_stats_request_xid_set(
    of_group_desc_stats_request_t *obj,
    uint32_t xid);
extern void of_group_desc_stats_request_xid_get(
    of_group_desc_stats_request_t *obj,
    uint32_t *xid);

extern void of_group_desc_stats_request_flags_set(
    of_group_desc_stats_request_t *obj,
    uint16_t flags);
extern void of_group_desc_stats_request_flags_get(
    of_group_desc_stats_request_t *obj,
    uint16_t *flags);

/* Unified accessor functions for of_group_features_stats_reply */

extern void of_group_features_stats_reply_xid_set(
    of_group_features_stats_reply_t *obj,
    uint32_t xid);
extern void of_group_features_stats_reply_xid_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *xid);

extern void of_group_features_stats_reply_flags_set(
    of_group_features_stats_reply_t *obj,
    uint16_t flags);
extern void of_group_features_stats_reply_flags_get(
    of_group_features_stats_reply_t *obj,
    uint16_t *flags);

extern void of_group_features_stats_reply_types_set(
    of_group_features_stats_reply_t *obj,
    uint32_t types);
extern void of_group_features_stats_reply_types_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *types);

extern void of_group_features_stats_reply_capabilities_set(
    of_group_features_stats_reply_t *obj,
    uint32_t capabilities);
extern void of_group_features_stats_reply_capabilities_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *capabilities);

extern void of_group_features_stats_reply_max_groups_all_set(
    of_group_features_stats_reply_t *obj,
    uint32_t max_groups_all);
extern void of_group_features_stats_reply_max_groups_all_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *max_groups_all);

extern void of_group_features_stats_reply_max_groups_select_set(
    of_group_features_stats_reply_t *obj,
    uint32_t max_groups_select);
extern void of_group_features_stats_reply_max_groups_select_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *max_groups_select);

extern void of_group_features_stats_reply_max_groups_indirect_set(
    of_group_features_stats_reply_t *obj,
    uint32_t max_groups_indirect);
extern void of_group_features_stats_reply_max_groups_indirect_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *max_groups_indirect);

extern void of_group_features_stats_reply_max_groups_ff_set(
    of_group_features_stats_reply_t *obj,
    uint32_t max_groups_ff);
extern void of_group_features_stats_reply_max_groups_ff_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *max_groups_ff);

extern void of_group_features_stats_reply_actions_all_set(
    of_group_features_stats_reply_t *obj,
    uint32_t actions_all);
extern void of_group_features_stats_reply_actions_all_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *actions_all);

extern void of_group_features_stats_reply_actions_select_set(
    of_group_features_stats_reply_t *obj,
    uint32_t actions_select);
extern void of_group_features_stats_reply_actions_select_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *actions_select);

extern void of_group_features_stats_reply_actions_indirect_set(
    of_group_features_stats_reply_t *obj,
    uint32_t actions_indirect);
extern void of_group_features_stats_reply_actions_indirect_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *actions_indirect);

extern void of_group_features_stats_reply_actions_ff_set(
    of_group_features_stats_reply_t *obj,
    uint32_t actions_ff);
extern void of_group_features_stats_reply_actions_ff_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *actions_ff);

/* Unified accessor functions for of_group_features_stats_request */

extern void of_group_features_stats_request_xid_set(
    of_group_features_stats_request_t *obj,
    uint32_t xid);
extern void of_group_features_stats_request_xid_get(
    of_group_features_stats_request_t *obj,
    uint32_t *xid);

extern void of_group_features_stats_request_flags_set(
    of_group_features_stats_request_t *obj,
    uint16_t flags);
extern void of_group_features_stats_request_flags_get(
    of_group_features_stats_request_t *obj,
    uint16_t *flags);

/* Unified accessor functions for of_group_mod */

extern void of_group_mod_xid_set(
    of_group_mod_t *obj,
    uint32_t xid);
extern void of_group_mod_xid_get(
    of_group_mod_t *obj,
    uint32_t *xid);

extern void of_group_mod_group_type_set(
    of_group_mod_t *obj,
    uint8_t group_type);
extern void of_group_mod_group_type_get(
    of_group_mod_t *obj,
    uint8_t *group_type);

extern void of_group_mod_group_id_set(
    of_group_mod_t *obj,
    uint32_t group_id);
extern void of_group_mod_group_id_get(
    of_group_mod_t *obj,
    uint32_t *group_id);

extern int WARN_UNUSED_RESULT of_group_mod_buckets_set(
    of_group_mod_t *obj,
    of_list_bucket_t *buckets);
extern void of_group_mod_buckets_bind(
    of_group_mod_t *obj,
    of_list_bucket_t *buckets);
extern of_list_bucket_t *of_group_mod_buckets_get(
    of_group_mod_t *obj);

/* Unified accessor functions for of_group_mod_failed_error_msg */

extern void of_group_mod_failed_error_msg_xid_set(
    of_group_mod_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_group_mod_failed_error_msg_xid_get(
    of_group_mod_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_group_mod_failed_error_msg_code_set(
    of_group_mod_failed_error_msg_t *obj,
    uint16_t code);
extern void of_group_mod_failed_error_msg_code_get(
    of_group_mod_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_group_mod_failed_error_msg_data_set(
    of_group_mod_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_group_mod_failed_error_msg_data_get(
    of_group_mod_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_group_modify */

extern void of_group_modify_xid_set(
    of_group_modify_t *obj,
    uint32_t xid);
extern void of_group_modify_xid_get(
    of_group_modify_t *obj,
    uint32_t *xid);

extern void of_group_modify_group_type_set(
    of_group_modify_t *obj,
    uint8_t group_type);
extern void of_group_modify_group_type_get(
    of_group_modify_t *obj,
    uint8_t *group_type);

extern void of_group_modify_group_id_set(
    of_group_modify_t *obj,
    uint32_t group_id);
extern void of_group_modify_group_id_get(
    of_group_modify_t *obj,
    uint32_t *group_id);

extern int WARN_UNUSED_RESULT of_group_modify_buckets_set(
    of_group_modify_t *obj,
    of_list_bucket_t *buckets);
extern void of_group_modify_buckets_bind(
    of_group_modify_t *obj,
    of_list_bucket_t *buckets);
extern of_list_bucket_t *of_group_modify_buckets_get(
    of_group_modify_t *obj);

/* Unified accessor functions for of_group_stats_reply */

extern void of_group_stats_reply_xid_set(
    of_group_stats_reply_t *obj,
    uint32_t xid);
extern void of_group_stats_reply_xid_get(
    of_group_stats_reply_t *obj,
    uint32_t *xid);

extern void of_group_stats_reply_flags_set(
    of_group_stats_reply_t *obj,
    uint16_t flags);
extern void of_group_stats_reply_flags_get(
    of_group_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_group_stats_reply_entries_set(
    of_group_stats_reply_t *obj,
    of_list_group_stats_entry_t *entries);
extern void of_group_stats_reply_entries_bind(
    of_group_stats_reply_t *obj,
    of_list_group_stats_entry_t *entries);
extern of_list_group_stats_entry_t *of_group_stats_reply_entries_get(
    of_group_stats_reply_t *obj);

/* Unified accessor functions for of_group_stats_request */

extern void of_group_stats_request_xid_set(
    of_group_stats_request_t *obj,
    uint32_t xid);
extern void of_group_stats_request_xid_get(
    of_group_stats_request_t *obj,
    uint32_t *xid);

extern void of_group_stats_request_flags_set(
    of_group_stats_request_t *obj,
    uint16_t flags);
extern void of_group_stats_request_flags_get(
    of_group_stats_request_t *obj,
    uint16_t *flags);

extern void of_group_stats_request_group_id_set(
    of_group_stats_request_t *obj,
    uint32_t group_id);
extern void of_group_stats_request_group_id_get(
    of_group_stats_request_t *obj,
    uint32_t *group_id);

/* Unified accessor functions for of_hello */

extern void of_hello_xid_set(
    of_hello_t *obj,
    uint32_t xid);
extern void of_hello_xid_get(
    of_hello_t *obj,
    uint32_t *xid);

extern int WARN_UNUSED_RESULT of_hello_elements_set(
    of_hello_t *obj,
    of_list_hello_elem_t *elements);
extern void of_hello_elements_bind(
    of_hello_t *obj,
    of_list_hello_elem_t *elements);
extern of_list_hello_elem_t *of_hello_elements_get(
    of_hello_t *obj);

/* Unified accessor functions for of_hello_failed_error_msg */

extern void of_hello_failed_error_msg_xid_set(
    of_hello_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_hello_failed_error_msg_xid_get(
    of_hello_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_hello_failed_error_msg_code_set(
    of_hello_failed_error_msg_t *obj,
    uint16_t code);
extern void of_hello_failed_error_msg_code_get(
    of_hello_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_hello_failed_error_msg_data_set(
    of_hello_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_hello_failed_error_msg_data_get(
    of_hello_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_meter_add */

extern void of_meter_add_xid_set(
    of_meter_add_t *obj,
    uint32_t xid);
extern void of_meter_add_xid_get(
    of_meter_add_t *obj,
    uint32_t *xid);

extern void of_meter_add_flags_set(
    of_meter_add_t *obj,
    uint16_t flags);
extern void of_meter_add_flags_get(
    of_meter_add_t *obj,
    uint16_t *flags);

extern void of_meter_add_meter_id_set(
    of_meter_add_t *obj,
    uint32_t meter_id);
extern void of_meter_add_meter_id_get(
    of_meter_add_t *obj,
    uint32_t *meter_id);

extern int WARN_UNUSED_RESULT of_meter_add_meters_set(
    of_meter_add_t *obj,
    of_list_meter_band_t *meters);
extern void of_meter_add_meters_bind(
    of_meter_add_t *obj,
    of_list_meter_band_t *meters);
extern of_list_meter_band_t *of_meter_add_meters_get(
    of_meter_add_t *obj);

/* Unified accessor functions for of_meter_config_stats_reply */

extern void of_meter_config_stats_reply_xid_set(
    of_meter_config_stats_reply_t *obj,
    uint32_t xid);
extern void of_meter_config_stats_reply_xid_get(
    of_meter_config_stats_reply_t *obj,
    uint32_t *xid);

extern void of_meter_config_stats_reply_flags_set(
    of_meter_config_stats_reply_t *obj,
    uint16_t flags);
extern void of_meter_config_stats_reply_flags_get(
    of_meter_config_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_meter_config_stats_reply_entries_set(
    of_meter_config_stats_reply_t *obj,
    of_list_meter_band_t *entries);
extern void of_meter_config_stats_reply_entries_bind(
    of_meter_config_stats_reply_t *obj,
    of_list_meter_band_t *entries);
extern of_list_meter_band_t *of_meter_config_stats_reply_entries_get(
    of_meter_config_stats_reply_t *obj);

/* Unified accessor functions for of_meter_config_stats_request */

extern void of_meter_config_stats_request_xid_set(
    of_meter_config_stats_request_t *obj,
    uint32_t xid);
extern void of_meter_config_stats_request_xid_get(
    of_meter_config_stats_request_t *obj,
    uint32_t *xid);

extern void of_meter_config_stats_request_flags_set(
    of_meter_config_stats_request_t *obj,
    uint16_t flags);
extern void of_meter_config_stats_request_flags_get(
    of_meter_config_stats_request_t *obj,
    uint16_t *flags);

extern void of_meter_config_stats_request_meter_id_set(
    of_meter_config_stats_request_t *obj,
    uint32_t meter_id);
extern void of_meter_config_stats_request_meter_id_get(
    of_meter_config_stats_request_t *obj,
    uint32_t *meter_id);

/* Unified accessor functions for of_meter_delete */

extern void of_meter_delete_xid_set(
    of_meter_delete_t *obj,
    uint32_t xid);
extern void of_meter_delete_xid_get(
    of_meter_delete_t *obj,
    uint32_t *xid);

extern void of_meter_delete_flags_set(
    of_meter_delete_t *obj,
    uint16_t flags);
extern void of_meter_delete_flags_get(
    of_meter_delete_t *obj,
    uint16_t *flags);

extern void of_meter_delete_meter_id_set(
    of_meter_delete_t *obj,
    uint32_t meter_id);
extern void of_meter_delete_meter_id_get(
    of_meter_delete_t *obj,
    uint32_t *meter_id);

extern int WARN_UNUSED_RESULT of_meter_delete_meters_set(
    of_meter_delete_t *obj,
    of_list_meter_band_t *meters);
extern void of_meter_delete_meters_bind(
    of_meter_delete_t *obj,
    of_list_meter_band_t *meters);
extern of_list_meter_band_t *of_meter_delete_meters_get(
    of_meter_delete_t *obj);

/* Unified accessor functions for of_meter_features_stats_reply */

extern void of_meter_features_stats_reply_xid_set(
    of_meter_features_stats_reply_t *obj,
    uint32_t xid);
extern void of_meter_features_stats_reply_xid_get(
    of_meter_features_stats_reply_t *obj,
    uint32_t *xid);

extern void of_meter_features_stats_reply_flags_set(
    of_meter_features_stats_reply_t *obj,
    uint16_t flags);
extern void of_meter_features_stats_reply_flags_get(
    of_meter_features_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_meter_features_stats_reply_features_set(
    of_meter_features_stats_reply_t *obj,
    of_meter_features_t *features);
extern void of_meter_features_stats_reply_features_bind(
    of_meter_features_stats_reply_t *obj,
    of_meter_features_t *features);
extern of_meter_features_t *of_meter_features_stats_reply_features_get(
    of_meter_features_stats_reply_t *obj);

/* Unified accessor functions for of_meter_features_stats_request */

extern void of_meter_features_stats_request_xid_set(
    of_meter_features_stats_request_t *obj,
    uint32_t xid);
extern void of_meter_features_stats_request_xid_get(
    of_meter_features_stats_request_t *obj,
    uint32_t *xid);

extern void of_meter_features_stats_request_flags_set(
    of_meter_features_stats_request_t *obj,
    uint16_t flags);
extern void of_meter_features_stats_request_flags_get(
    of_meter_features_stats_request_t *obj,
    uint16_t *flags);

/* Unified accessor functions for of_meter_mod */

extern void of_meter_mod_xid_set(
    of_meter_mod_t *obj,
    uint32_t xid);
extern void of_meter_mod_xid_get(
    of_meter_mod_t *obj,
    uint32_t *xid);

extern void of_meter_mod_flags_set(
    of_meter_mod_t *obj,
    uint16_t flags);
extern void of_meter_mod_flags_get(
    of_meter_mod_t *obj,
    uint16_t *flags);

extern void of_meter_mod_meter_id_set(
    of_meter_mod_t *obj,
    uint32_t meter_id);
extern void of_meter_mod_meter_id_get(
    of_meter_mod_t *obj,
    uint32_t *meter_id);

extern int WARN_UNUSED_RESULT of_meter_mod_meters_set(
    of_meter_mod_t *obj,
    of_list_meter_band_t *meters);
extern void of_meter_mod_meters_bind(
    of_meter_mod_t *obj,
    of_list_meter_band_t *meters);
extern of_list_meter_band_t *of_meter_mod_meters_get(
    of_meter_mod_t *obj);

/* Unified accessor functions for of_meter_mod_failed_error_msg */

extern void of_meter_mod_failed_error_msg_xid_set(
    of_meter_mod_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_meter_mod_failed_error_msg_xid_get(
    of_meter_mod_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_meter_mod_failed_error_msg_code_set(
    of_meter_mod_failed_error_msg_t *obj,
    uint16_t code);
extern void of_meter_mod_failed_error_msg_code_get(
    of_meter_mod_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_meter_mod_failed_error_msg_data_set(
    of_meter_mod_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_meter_mod_failed_error_msg_data_get(
    of_meter_mod_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_meter_modify */

extern void of_meter_modify_xid_set(
    of_meter_modify_t *obj,
    uint32_t xid);
extern void of_meter_modify_xid_get(
    of_meter_modify_t *obj,
    uint32_t *xid);

extern void of_meter_modify_flags_set(
    of_meter_modify_t *obj,
    uint16_t flags);
extern void of_meter_modify_flags_get(
    of_meter_modify_t *obj,
    uint16_t *flags);

extern void of_meter_modify_meter_id_set(
    of_meter_modify_t *obj,
    uint32_t meter_id);
extern void of_meter_modify_meter_id_get(
    of_meter_modify_t *obj,
    uint32_t *meter_id);

extern int WARN_UNUSED_RESULT of_meter_modify_meters_set(
    of_meter_modify_t *obj,
    of_list_meter_band_t *meters);
extern void of_meter_modify_meters_bind(
    of_meter_modify_t *obj,
    of_list_meter_band_t *meters);
extern of_list_meter_band_t *of_meter_modify_meters_get(
    of_meter_modify_t *obj);

/* Unified accessor functions for of_meter_stats_reply */

extern void of_meter_stats_reply_xid_set(
    of_meter_stats_reply_t *obj,
    uint32_t xid);
extern void of_meter_stats_reply_xid_get(
    of_meter_stats_reply_t *obj,
    uint32_t *xid);

extern void of_meter_stats_reply_flags_set(
    of_meter_stats_reply_t *obj,
    uint16_t flags);
extern void of_meter_stats_reply_flags_get(
    of_meter_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_meter_stats_reply_entries_set(
    of_meter_stats_reply_t *obj,
    of_list_meter_stats_t *entries);
extern void of_meter_stats_reply_entries_bind(
    of_meter_stats_reply_t *obj,
    of_list_meter_stats_t *entries);
extern of_list_meter_stats_t *of_meter_stats_reply_entries_get(
    of_meter_stats_reply_t *obj);

/* Unified accessor functions for of_meter_stats_request */

extern void of_meter_stats_request_xid_set(
    of_meter_stats_request_t *obj,
    uint32_t xid);
extern void of_meter_stats_request_xid_get(
    of_meter_stats_request_t *obj,
    uint32_t *xid);

extern void of_meter_stats_request_flags_set(
    of_meter_stats_request_t *obj,
    uint16_t flags);
extern void of_meter_stats_request_flags_get(
    of_meter_stats_request_t *obj,
    uint16_t *flags);

extern void of_meter_stats_request_meter_id_set(
    of_meter_stats_request_t *obj,
    uint32_t meter_id);
extern void of_meter_stats_request_meter_id_get(
    of_meter_stats_request_t *obj,
    uint32_t *meter_id);

/* Unified accessor functions for of_nicira_controller_role_reply */

extern void of_nicira_controller_role_reply_xid_set(
    of_nicira_controller_role_reply_t *obj,
    uint32_t xid);
extern void of_nicira_controller_role_reply_xid_get(
    of_nicira_controller_role_reply_t *obj,
    uint32_t *xid);

extern void of_nicira_controller_role_reply_experimenter_set(
    of_nicira_controller_role_reply_t *obj,
    uint32_t experimenter);
extern void of_nicira_controller_role_reply_experimenter_get(
    of_nicira_controller_role_reply_t *obj,
    uint32_t *experimenter);

extern void of_nicira_controller_role_reply_subtype_set(
    of_nicira_controller_role_reply_t *obj,
    uint32_t subtype);
extern void of_nicira_controller_role_reply_subtype_get(
    of_nicira_controller_role_reply_t *obj,
    uint32_t *subtype);

extern void of_nicira_controller_role_reply_role_set(
    of_nicira_controller_role_reply_t *obj,
    uint32_t role);
extern void of_nicira_controller_role_reply_role_get(
    of_nicira_controller_role_reply_t *obj,
    uint32_t *role);

/* Unified accessor functions for of_nicira_controller_role_request */

extern void of_nicira_controller_role_request_xid_set(
    of_nicira_controller_role_request_t *obj,
    uint32_t xid);
extern void of_nicira_controller_role_request_xid_get(
    of_nicira_controller_role_request_t *obj,
    uint32_t *xid);

extern void of_nicira_controller_role_request_experimenter_set(
    of_nicira_controller_role_request_t *obj,
    uint32_t experimenter);
extern void of_nicira_controller_role_request_experimenter_get(
    of_nicira_controller_role_request_t *obj,
    uint32_t *experimenter);

extern void of_nicira_controller_role_request_subtype_set(
    of_nicira_controller_role_request_t *obj,
    uint32_t subtype);
extern void of_nicira_controller_role_request_subtype_get(
    of_nicira_controller_role_request_t *obj,
    uint32_t *subtype);

extern void of_nicira_controller_role_request_role_set(
    of_nicira_controller_role_request_t *obj,
    uint32_t role);
extern void of_nicira_controller_role_request_role_get(
    of_nicira_controller_role_request_t *obj,
    uint32_t *role);

/* Unified accessor functions for of_nicira_header */

extern void of_nicira_header_xid_set(
    of_nicira_header_t *obj,
    uint32_t xid);
extern void of_nicira_header_xid_get(
    of_nicira_header_t *obj,
    uint32_t *xid);

extern void of_nicira_header_experimenter_set(
    of_nicira_header_t *obj,
    uint32_t experimenter);
extern void of_nicira_header_experimenter_get(
    of_nicira_header_t *obj,
    uint32_t *experimenter);

extern void of_nicira_header_subtype_set(
    of_nicira_header_t *obj,
    uint32_t subtype);
extern void of_nicira_header_subtype_get(
    of_nicira_header_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_ofdpa_class_based_ctr_mod_msg */

extern void of_ofdpa_class_based_ctr_mod_msg_xid_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t xid);
extern void of_ofdpa_class_based_ctr_mod_msg_xid_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *xid);

extern void of_ofdpa_class_based_ctr_mod_msg_experimenter_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_class_based_ctr_mod_msg_experimenter_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_class_based_ctr_mod_msg_subtype_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t subtype);
extern void of_ofdpa_class_based_ctr_mod_msg_subtype_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_class_based_ctr_mod_msg_mod_command_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_class_based_ctr_mod_msg_mod_command_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_class_based_ctr_mod_msg_block_index_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t block_index);
extern void of_ofdpa_class_based_ctr_mod_msg_block_index_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *block_index);

extern void of_ofdpa_class_based_ctr_mod_msg_packets_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint64_t packets);
extern void of_ofdpa_class_based_ctr_mod_msg_packets_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint64_t *packets);

extern void of_ofdpa_class_based_ctr_mod_msg_bytes_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint64_t bytes);
extern void of_ofdpa_class_based_ctr_mod_msg_bytes_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint64_t *bytes);

extern void of_ofdpa_class_based_ctr_mod_msg_traffic_class_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_class_based_ctr_mod_msg_traffic_class_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint8_t *traffic_class);

/* Unified accessor functions for of_ofdpa_class_based_ctr_multipart_reply */

extern void of_ofdpa_class_based_ctr_multipart_reply_xid_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_class_based_ctr_multipart_reply_xid_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_class_based_ctr_multipart_reply_flags_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_class_based_ctr_multipart_reply_flags_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_class_based_ctr_multipart_reply_experimenter_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_class_based_ctr_multipart_reply_experimenter_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_class_based_ctr_multipart_reply_subtype_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_class_based_ctr_multipart_reply_subtype_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_class_based_ctr_multipart_reply_block_index_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t block_index);
extern void of_ofdpa_class_based_ctr_multipart_reply_block_index_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t *block_index);

extern void of_ofdpa_class_based_ctr_multipart_reply_packets_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t packets);
extern void of_ofdpa_class_based_ctr_multipart_reply_packets_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t *packets);

extern void of_ofdpa_class_based_ctr_multipart_reply_bytes_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t bytes);
extern void of_ofdpa_class_based_ctr_multipart_reply_bytes_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t *bytes);

extern void of_ofdpa_class_based_ctr_multipart_reply_reference_count_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t reference_count);
extern void of_ofdpa_class_based_ctr_multipart_reply_reference_count_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t *reference_count);

extern void of_ofdpa_class_based_ctr_multipart_reply_traffic_class_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_class_based_ctr_multipart_reply_traffic_class_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint8_t *traffic_class);

/* Unified accessor functions for of_ofdpa_class_based_ctr_multipart_request */

extern void of_ofdpa_class_based_ctr_multipart_request_xid_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_class_based_ctr_multipart_request_xid_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_class_based_ctr_multipart_request_flags_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_class_based_ctr_multipart_request_flags_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_class_based_ctr_multipart_request_experimenter_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_class_based_ctr_multipart_request_experimenter_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_class_based_ctr_multipart_request_subtype_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_class_based_ctr_multipart_request_subtype_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_class_based_ctr_multipart_request_block_index_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t block_index);
extern void of_ofdpa_class_based_ctr_multipart_request_block_index_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t *block_index);

extern void of_ofdpa_class_based_ctr_multipart_request_traffic_class_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_class_based_ctr_multipart_request_traffic_class_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint8_t *traffic_class);

/* Unified accessor functions for of_ofdpa_color_based_ctr_mod_msg */

extern void of_ofdpa_color_based_ctr_mod_msg_xid_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t xid);
extern void of_ofdpa_color_based_ctr_mod_msg_xid_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *xid);

extern void of_ofdpa_color_based_ctr_mod_msg_experimenter_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_color_based_ctr_mod_msg_experimenter_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_color_based_ctr_mod_msg_subtype_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t subtype);
extern void of_ofdpa_color_based_ctr_mod_msg_subtype_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_color_based_ctr_mod_msg_mod_command_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_color_based_ctr_mod_msg_mod_command_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_color_based_ctr_mod_msg_block_index_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t block_index);
extern void of_ofdpa_color_based_ctr_mod_msg_block_index_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *block_index);

extern void of_ofdpa_color_based_ctr_mod_msg_packets_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint64_t packets);
extern void of_ofdpa_color_based_ctr_mod_msg_packets_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint64_t *packets);

extern void of_ofdpa_color_based_ctr_mod_msg_bytes_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint64_t bytes);
extern void of_ofdpa_color_based_ctr_mod_msg_bytes_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint64_t *bytes);

extern void of_ofdpa_color_based_ctr_mod_msg_color_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint8_t color);
extern void of_ofdpa_color_based_ctr_mod_msg_color_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_ofdpa_color_based_ctr_multipart_reply */

extern void of_ofdpa_color_based_ctr_multipart_reply_xid_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_color_based_ctr_multipart_reply_xid_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_color_based_ctr_multipart_reply_flags_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_color_based_ctr_multipart_reply_flags_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_color_based_ctr_multipart_reply_experimenter_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_color_based_ctr_multipart_reply_experimenter_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_color_based_ctr_multipart_reply_subtype_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_color_based_ctr_multipart_reply_subtype_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_color_based_ctr_multipart_reply_block_index_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t block_index);
extern void of_ofdpa_color_based_ctr_multipart_reply_block_index_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t *block_index);

extern void of_ofdpa_color_based_ctr_multipart_reply_packets_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t packets);
extern void of_ofdpa_color_based_ctr_multipart_reply_packets_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t *packets);

extern void of_ofdpa_color_based_ctr_multipart_reply_bytes_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t bytes);
extern void of_ofdpa_color_based_ctr_multipart_reply_bytes_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t *bytes);

extern void of_ofdpa_color_based_ctr_multipart_reply_reference_count_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t reference_count);
extern void of_ofdpa_color_based_ctr_multipart_reply_reference_count_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t *reference_count);

extern void of_ofdpa_color_based_ctr_multipart_reply_color_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint8_t color);
extern void of_ofdpa_color_based_ctr_multipart_reply_color_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_ofdpa_color_based_ctr_multipart_request */

extern void of_ofdpa_color_based_ctr_multipart_request_xid_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_color_based_ctr_multipart_request_xid_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_color_based_ctr_multipart_request_flags_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_color_based_ctr_multipart_request_flags_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_color_based_ctr_multipart_request_experimenter_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_color_based_ctr_multipart_request_experimenter_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_color_based_ctr_multipart_request_subtype_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_color_based_ctr_multipart_request_subtype_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_color_based_ctr_multipart_request_block_index_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t block_index);
extern void of_ofdpa_color_based_ctr_multipart_request_block_index_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *block_index);

extern void of_ofdpa_color_based_ctr_multipart_request_color_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint8_t color);
extern void of_ofdpa_color_based_ctr_multipart_request_color_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_ofdpa_l2_interface_remark_action_mod_msg */

extern void of_ofdpa_l2_interface_remark_action_mod_msg_xid_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t xid);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_xid_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *xid);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_experimenter_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_experimenter_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_subtype_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t subtype);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_subtype_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_mod_command_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_mod_command_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_index_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t index);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_index_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *index);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_traffic_class_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_traffic_class_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_color_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t color);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_color_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *color);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_dscp_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t dscp);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_dscp_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *dscp);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_vlan_pcp_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_vlan_pcp_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_l2_interface_remark_action_mod_msg_vlan_dei_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_l2_interface_remark_action_mod_msg_vlan_dei_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_l2_interface_remark_action_multipart_reply */

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_xid_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_xid_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_flags_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_flags_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_experimenter_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_experimenter_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_subtype_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_subtype_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_index_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t index);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_index_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *index);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_traffic_class_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_traffic_class_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_color_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t color);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_color_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *color);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_mpls_tc_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t mpls_tc);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_mpls_tc_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *mpls_tc);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_vlan_pcp_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_vlan_pcp_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_l2_interface_remark_action_multipart_reply_vlan_dei_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_l2_interface_remark_action_multipart_reply_vlan_dei_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_l2_interface_remark_action_multipart_request */

extern void of_ofdpa_l2_interface_remark_action_multipart_request_xid_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_xid_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_flags_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_flags_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_experimenter_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_experimenter_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_subtype_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_subtype_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_index_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t index);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_index_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *index);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_traffic_class_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_traffic_class_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_color_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t color);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_color_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *color);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_mpls_tc_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t mpls_tc);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_mpls_tc_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *mpls_tc);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_vlan_pcp_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_vlan_pcp_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_l2_interface_remark_action_multipart_request_vlan_dei_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_l2_interface_remark_action_multipart_request_vlan_dei_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_mpls_tunnel_label_multipart_reply */

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_xid_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_xid_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_flags_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_flags_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_experimenter_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_experimenter_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_subtype_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_subtype_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_index_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t index);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_index_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *index);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_traffic_class_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_traffic_class_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_color_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t color);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_color_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *color);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_mpls_tc_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t mpls_tc);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_mpls_tc_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *mpls_tc);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_vlan_pcp_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_vlan_pcp_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_mpls_tunnel_label_multipart_reply_vlan_dei_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_mpls_tunnel_label_multipart_reply_vlan_dei_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_mpls_tunnel_label_multipart_request */

extern void of_ofdpa_mpls_tunnel_label_multipart_request_xid_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_xid_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_flags_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_flags_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_experimenter_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_experimenter_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_subtype_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_subtype_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_index_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t index);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_index_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *index);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_traffic_class_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_traffic_class_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_color_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t color);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_color_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *color);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_mpls_tc_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t mpls_tc);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_mpls_tc_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *mpls_tc);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_vlan_pcp_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_vlan_pcp_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_mpls_tunnel_label_multipart_request_vlan_dei_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_mpls_tunnel_label_multipart_request_vlan_dei_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_mpls_tunnel_label_remark_action_mod_msg */

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_xid_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t xid);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_xid_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *xid);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_experimenter_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_experimenter_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_subtype_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t subtype);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_subtype_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_mod_command_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_mod_command_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_index_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t index);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_index_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *index);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_traffic_class_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_traffic_class_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_color_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t color);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_color_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *color);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_mpls_tc_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t mpls_tc);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_mpls_tc_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *mpls_tc);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_pcp_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_pcp_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_dei_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_dei_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_mpls_vpn_label_remark_action_mod_msg */

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_xid_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t xid);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_xid_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *xid);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_experimenter_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_experimenter_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_subtype_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t subtype);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_subtype_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_mod_command_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_mod_command_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_index_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t index);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_index_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *index);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_traffic_class_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_traffic_class_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_color_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t color);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_color_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *color);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_mpls_tc_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t mpls_tc);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_mpls_tc_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *mpls_tc);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_pcp_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_pcp_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_dei_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_dei_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_mpls_vpn_label_remark_action_multipart_reply */

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_xid_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_xid_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_flags_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_flags_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_experimenter_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_experimenter_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_subtype_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_subtype_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_index_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t index);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_index_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *index);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_traffic_class_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_traffic_class_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_color_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t color);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_color_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *color);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_mpls_tc_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t mpls_tc);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_mpls_tc_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *mpls_tc);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_pcp_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_pcp_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_dei_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_dei_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_mpls_vpn_label_remark_action_multipart_request */

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_xid_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_xid_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_flags_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_flags_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_experimenter_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_experimenter_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_subtype_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_subtype_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_index_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t index);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_index_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *index);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *traffic_class);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_color_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t color);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_color_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *color);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_mpls_tc_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t mpls_tc);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_mpls_tc_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *mpls_tc);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_pcp_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t vlan_pcp);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_pcp_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *vlan_pcp);

extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_dei_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t vlan_dei);
extern void of_ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_dei_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *vlan_dei);

/* Unified accessor functions for of_ofdpa_oam_dataplane_ctr_mod_msg */

extern void of_ofdpa_oam_dataplane_ctr_mod_msg_xid_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t xid);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_xid_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *xid);

extern void of_ofdpa_oam_dataplane_ctr_mod_msg_experimenter_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_experimenter_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_oam_dataplane_ctr_mod_msg_subtype_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t subtype);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_subtype_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_oam_dataplane_ctr_mod_msg_mod_command_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_mod_command_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_oam_dataplane_ctr_mod_msg_lmep_id_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t lmep_id);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_lmep_id_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *lmep_id);

extern void of_ofdpa_oam_dataplane_ctr_mod_msg_transmit_packets_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t transmit_packets);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_transmit_packets_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t *transmit_packets);

extern void of_ofdpa_oam_dataplane_ctr_mod_msg_receive_packets_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t receive_packets);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_receive_packets_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t *receive_packets);

extern void of_ofdpa_oam_dataplane_ctr_mod_msg_traffic_class_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_oam_dataplane_ctr_mod_msg_traffic_class_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint8_t *traffic_class);

/* Unified accessor functions for of_ofdpa_oam_dataplane_ctr_multipart_reply */

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_xid_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_xid_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_flags_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_flags_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_experimenter_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_experimenter_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_subtype_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_subtype_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_lmep_id_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t lmep_id);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_lmep_id_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *lmep_id);

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_receive_packets_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t receive_packets);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_receive_packets_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *receive_packets);

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_transmit_packets_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t transmit_packets);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_transmit_packets_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *transmit_packets);

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_reference_count_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t reference_count);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_reference_count_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *reference_count);

extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_traffic_class_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_oam_dataplane_ctr_multipart_reply_traffic_class_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint8_t *traffic_class);

/* Unified accessor functions for of_ofdpa_oam_dataplane_ctr_multipart_request */

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_xid_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_xid_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_flags_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_flags_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_experimenter_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_experimenter_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_subtype_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_subtype_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_lmep_id_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t lmep_id);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_lmep_id_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *lmep_id);

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_transmit_packets_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t transmit_packets);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_transmit_packets_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *transmit_packets);

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_receive_packets_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t receive_packets);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_receive_packets_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *receive_packets);

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_reference_count_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t reference_count);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_reference_count_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *reference_count);

extern void of_ofdpa_oam_dataplane_ctr_multipart_request_traffic_class_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint8_t traffic_class);
extern void of_ofdpa_oam_dataplane_ctr_multipart_request_traffic_class_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint8_t *traffic_class);

/* Unified accessor functions for of_ofdpa_oam_drop_status_mod_msg */

extern void of_ofdpa_oam_drop_status_mod_msg_xid_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t xid);
extern void of_ofdpa_oam_drop_status_mod_msg_xid_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *xid);

extern void of_ofdpa_oam_drop_status_mod_msg_experimenter_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_oam_drop_status_mod_msg_experimenter_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_oam_drop_status_mod_msg_subtype_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t subtype);
extern void of_ofdpa_oam_drop_status_mod_msg_subtype_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_oam_drop_status_mod_msg_mod_command_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_oam_drop_status_mod_msg_mod_command_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_oam_drop_status_mod_msg_index_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t index);
extern void of_ofdpa_oam_drop_status_mod_msg_index_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *index);

extern void of_ofdpa_oam_drop_status_mod_msg_entry_type_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t entry_type);
extern void of_ofdpa_oam_drop_status_mod_msg_entry_type_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t *entry_type);

extern void of_ofdpa_oam_drop_status_mod_msg_drop_status_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t drop_status);
extern void of_ofdpa_oam_drop_status_mod_msg_drop_status_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t *drop_status);

/* Unified accessor functions for of_ofdpa_oam_drop_status_multipart_reply */

extern void of_ofdpa_oam_drop_status_multipart_reply_xid_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_oam_drop_status_multipart_reply_xid_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_oam_drop_status_multipart_reply_flags_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_oam_drop_status_multipart_reply_flags_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_oam_drop_status_multipart_reply_experimenter_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_oam_drop_status_multipart_reply_experimenter_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_oam_drop_status_multipart_reply_subtype_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_oam_drop_status_multipart_reply_subtype_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_oam_drop_status_multipart_reply_index_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t index);
extern void of_ofdpa_oam_drop_status_multipart_reply_index_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *index);

extern void of_ofdpa_oam_drop_status_multipart_reply_entry_type_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t entry_type);
extern void of_ofdpa_oam_drop_status_multipart_reply_entry_type_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t *entry_type);

extern void of_ofdpa_oam_drop_status_multipart_reply_drop_status_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t drop_status);
extern void of_ofdpa_oam_drop_status_multipart_reply_drop_status_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t *drop_status);

/* Unified accessor functions for of_ofdpa_oam_drop_status_multipart_request */

extern void of_ofdpa_oam_drop_status_multipart_request_xid_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_oam_drop_status_multipart_request_xid_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_oam_drop_status_multipart_request_flags_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_oam_drop_status_multipart_request_flags_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_oam_drop_status_multipart_request_experimenter_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_oam_drop_status_multipart_request_experimenter_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_oam_drop_status_multipart_request_subtype_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_oam_drop_status_multipart_request_subtype_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_oam_drop_status_multipart_request_mod_command_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_oam_drop_status_multipart_request_mod_command_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_oam_drop_status_multipart_request_index_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t index);
extern void of_ofdpa_oam_drop_status_multipart_request_index_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *index);

extern void of_ofdpa_oam_drop_status_multipart_request_entry_type_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t entry_type);
extern void of_ofdpa_oam_drop_status_multipart_request_entry_type_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t *entry_type);

extern void of_ofdpa_oam_drop_status_multipart_request_drop_status_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t drop_status);
extern void of_ofdpa_oam_drop_status_multipart_request_drop_status_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t *drop_status);

/* Unified accessor functions for of_ofdpa_port_experimenter_prop_scheduling */

extern void of_ofdpa_port_experimenter_prop_scheduling_xid_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t xid);
extern void of_ofdpa_port_experimenter_prop_scheduling_xid_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *xid);

extern void of_ofdpa_port_experimenter_prop_scheduling_port_no_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t port_no);
extern void of_ofdpa_port_experimenter_prop_scheduling_port_no_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *port_no);

extern void of_ofdpa_port_experimenter_prop_scheduling_hw_addr_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    of_mac_addr_t hw_addr);
extern void of_ofdpa_port_experimenter_prop_scheduling_hw_addr_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    of_mac_addr_t *hw_addr);

extern void of_ofdpa_port_experimenter_prop_scheduling_config_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t config);
extern void of_ofdpa_port_experimenter_prop_scheduling_config_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *config);

extern void of_ofdpa_port_experimenter_prop_scheduling_mask_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t mask);
extern void of_ofdpa_port_experimenter_prop_scheduling_mask_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *mask);

extern void of_ofdpa_port_experimenter_prop_scheduling_pm_type_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t pm_type);
extern void of_ofdpa_port_experimenter_prop_scheduling_pm_type_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *pm_type);

extern void of_ofdpa_port_experimenter_prop_scheduling_experimenter_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_port_experimenter_prop_scheduling_experimenter_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_port_experimenter_prop_scheduling_subtype_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t subtype);
extern void of_ofdpa_port_experimenter_prop_scheduling_subtype_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_port_experimenter_prop_scheduling_mode_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t mode);
extern void of_ofdpa_port_experimenter_prop_scheduling_mode_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *mode);

extern void of_ofdpa_port_experimenter_prop_scheduling_parameter_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t parameter);
extern void of_ofdpa_port_experimenter_prop_scheduling_parameter_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *parameter);

/* Unified accessor functions for of_ofdpa_queue_desc_prop_congestion_multipart_reply */

extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_xid_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_xid_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_flags_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_flags_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_experimenter_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_experimenter_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_subtype_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_subtype_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_mode_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t mode);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_reply_mode_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t *mode);

/* Unified accessor functions for of_ofdpa_queue_desc_prop_congestion_multipart_request */

extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_xid_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_xid_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_flags_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_flags_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_experimenter_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_experimenter_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_subtype_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_subtype_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_mode_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t mode);
extern void of_ofdpa_queue_desc_prop_congestion_multipart_request_mode_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t *mode);

/* Unified accessor functions for of_ofdpa_queue_desc_prop_parent_multipart_reply */

extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_xid_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t xid);
extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_xid_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *xid);

extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_flags_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t flags);
extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_flags_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t *flags);

extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_experimenter_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_experimenter_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_subtype_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t subtype);
extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_subtype_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_scheduler_level_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t scheduler_level);
extern void of_ofdpa_queue_desc_prop_parent_multipart_reply_scheduler_level_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t *scheduler_level);

/* Unified accessor functions for of_ofdpa_queue_desc_prop_parent_multipart_request */

extern void of_ofdpa_queue_desc_prop_parent_multipart_request_xid_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_queue_desc_prop_parent_multipart_request_xid_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_queue_desc_prop_parent_multipart_request_flags_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_queue_desc_prop_parent_multipart_request_flags_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_queue_desc_prop_parent_multipart_request_experimenter_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_queue_desc_prop_parent_multipart_request_experimenter_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_queue_desc_prop_parent_multipart_request_subtype_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_queue_desc_prop_parent_multipart_request_subtype_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_queue_desc_prop_parent_multipart_request_scheduler_level_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t scheduler_level);
extern void of_ofdpa_queue_desc_prop_parent_multipart_request_scheduler_level_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t *scheduler_level);

/* Unified accessor functions for of_ofdpa_queue_mod */

extern void of_ofdpa_queue_mod_xid_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t xid);
extern void of_ofdpa_queue_mod_xid_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *xid);

extern void of_ofdpa_queue_mod_experimenter_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_queue_mod_experimenter_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_queue_mod_subtype_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t subtype);
extern void of_ofdpa_queue_mod_subtype_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_queue_mod_mod_command_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_queue_mod_mod_command_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_queue_mod_port_no_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t port_no);
extern void of_ofdpa_queue_mod_port_no_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *port_no);

extern void of_ofdpa_queue_mod_queue_id_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t queue_id);
extern void of_ofdpa_queue_mod_queue_id_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *queue_id);

/* Unified accessor functions for of_ofdpa_scheduler_node_mod_msg */

extern void of_ofdpa_scheduler_node_mod_msg_xid_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t xid);
extern void of_ofdpa_scheduler_node_mod_msg_xid_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *xid);

extern void of_ofdpa_scheduler_node_mod_msg_experimenter_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_scheduler_node_mod_msg_experimenter_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_scheduler_node_mod_msg_subtype_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t subtype);
extern void of_ofdpa_scheduler_node_mod_msg_subtype_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_scheduler_node_mod_msg_mod_command_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t mod_command);
extern void of_ofdpa_scheduler_node_mod_msg_mod_command_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *mod_command);

extern void of_ofdpa_scheduler_node_mod_msg_level_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t level);
extern void of_ofdpa_scheduler_node_mod_msg_level_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *level);

extern void of_ofdpa_scheduler_node_mod_msg_port_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t port);
extern void of_ofdpa_scheduler_node_mod_msg_port_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *port);

extern void of_ofdpa_scheduler_node_mod_msg_max_rate_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t max_rate);
extern void of_ofdpa_scheduler_node_mod_msg_max_rate_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *max_rate);

extern void of_ofdpa_scheduler_node_mod_msg_min_rate_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t min_rate);
extern void of_ofdpa_scheduler_node_mod_msg_min_rate_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *min_rate);

extern void of_ofdpa_scheduler_node_mod_msg_mode_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t mode);
extern void of_ofdpa_scheduler_node_mod_msg_mode_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *mode);

extern void of_ofdpa_scheduler_node_mod_msg_weight_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t weight);
extern void of_ofdpa_scheduler_node_mod_msg_weight_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *weight);

/* Unified accessor functions for of_ofdpa_scheduler_node_multipart_request */

extern void of_ofdpa_scheduler_node_multipart_request_xid_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t xid);
extern void of_ofdpa_scheduler_node_multipart_request_xid_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *xid);

extern void of_ofdpa_scheduler_node_multipart_request_flags_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t flags);
extern void of_ofdpa_scheduler_node_multipart_request_flags_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *flags);

extern void of_ofdpa_scheduler_node_multipart_request_experimenter_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_scheduler_node_multipart_request_experimenter_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_scheduler_node_multipart_request_subtype_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t subtype);
extern void of_ofdpa_scheduler_node_multipart_request_subtype_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_scheduler_node_multipart_request_port_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t port);
extern void of_ofdpa_scheduler_node_multipart_request_port_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *port);

extern void of_ofdpa_scheduler_node_multipart_request_level_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t level);
extern void of_ofdpa_scheduler_node_multipart_request_level_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *level);

extern void of_ofdpa_scheduler_node_multipart_request_max_rate_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t max_rate);
extern void of_ofdpa_scheduler_node_multipart_request_max_rate_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *max_rate);

extern void of_ofdpa_scheduler_node_multipart_request_min_rate_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t min_rate);
extern void of_ofdpa_scheduler_node_multipart_request_min_rate_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *min_rate);

extern void of_ofdpa_scheduler_node_multipart_request_mode_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t mode);
extern void of_ofdpa_scheduler_node_multipart_request_mode_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *mode);

extern void of_ofdpa_scheduler_node_multipart_request_weight_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t weight);
extern void of_ofdpa_scheduler_node_multipart_request_weight_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *weight);

/* Unified accessor functions for of_ofdpa_scheduler_node_multipart_response */

extern void of_ofdpa_scheduler_node_multipart_response_xid_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t xid);
extern void of_ofdpa_scheduler_node_multipart_response_xid_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *xid);

extern void of_ofdpa_scheduler_node_multipart_response_flags_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t flags);
extern void of_ofdpa_scheduler_node_multipart_response_flags_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *flags);

extern void of_ofdpa_scheduler_node_multipart_response_experimenter_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t experimenter);
extern void of_ofdpa_scheduler_node_multipart_response_experimenter_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *experimenter);

extern void of_ofdpa_scheduler_node_multipart_response_subtype_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t subtype);
extern void of_ofdpa_scheduler_node_multipart_response_subtype_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *subtype);

extern void of_ofdpa_scheduler_node_multipart_response_port_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t port);
extern void of_ofdpa_scheduler_node_multipart_response_port_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *port);

extern void of_ofdpa_scheduler_node_multipart_response_level_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t level);
extern void of_ofdpa_scheduler_node_multipart_response_level_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *level);

extern void of_ofdpa_scheduler_node_multipart_response_max_rate_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t max_rate);
extern void of_ofdpa_scheduler_node_multipart_response_max_rate_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *max_rate);

extern void of_ofdpa_scheduler_node_multipart_response_min_rate_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t min_rate);
extern void of_ofdpa_scheduler_node_multipart_response_min_rate_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *min_rate);

extern void of_ofdpa_scheduler_node_multipart_response_mode_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t mode);
extern void of_ofdpa_scheduler_node_multipart_response_mode_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *mode);

extern void of_ofdpa_scheduler_node_multipart_response_weight_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t weight);
extern void of_ofdpa_scheduler_node_multipart_response_weight_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *weight);

/* Unified accessor functions for of_packet_in */

extern void of_packet_in_xid_set(
    of_packet_in_t *obj,
    uint32_t xid);
extern void of_packet_in_xid_get(
    of_packet_in_t *obj,
    uint32_t *xid);

extern void of_packet_in_buffer_id_set(
    of_packet_in_t *obj,
    uint32_t buffer_id);
extern void of_packet_in_buffer_id_get(
    of_packet_in_t *obj,
    uint32_t *buffer_id);

extern void of_packet_in_total_len_set(
    of_packet_in_t *obj,
    uint16_t total_len);
extern void of_packet_in_total_len_get(
    of_packet_in_t *obj,
    uint16_t *total_len);

extern void of_packet_in_reason_set(
    of_packet_in_t *obj,
    uint8_t reason);
extern void of_packet_in_reason_get(
    of_packet_in_t *obj,
    uint8_t *reason);

extern void of_packet_in_table_id_set(
    of_packet_in_t *obj,
    uint8_t table_id);
extern void of_packet_in_table_id_get(
    of_packet_in_t *obj,
    uint8_t *table_id);

extern void of_packet_in_cookie_set(
    of_packet_in_t *obj,
    uint64_t cookie);
extern void of_packet_in_cookie_get(
    of_packet_in_t *obj,
    uint64_t *cookie);

extern int WARN_UNUSED_RESULT of_packet_in_match_set(
    of_packet_in_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_packet_in_match_get(
    of_packet_in_t *obj,
    of_match_t *match);

extern int WARN_UNUSED_RESULT of_packet_in_data_set(
    of_packet_in_t *obj,
    of_octets_t *data);
extern void of_packet_in_data_get(
    of_packet_in_t *obj,
    of_octets_t *data);

extern void of_packet_in_in_port_set(
    of_packet_in_t *obj,
    of_port_no_t in_port);
extern void of_packet_in_in_port_get(
    of_packet_in_t *obj,
    of_port_no_t *in_port);

extern void of_packet_in_in_phy_port_set(
    of_packet_in_t *obj,
    of_port_no_t in_phy_port);
extern void of_packet_in_in_phy_port_get(
    of_packet_in_t *obj,
    of_port_no_t *in_phy_port);

/* Unified accessor functions for of_packet_out */

extern void of_packet_out_xid_set(
    of_packet_out_t *obj,
    uint32_t xid);
extern void of_packet_out_xid_get(
    of_packet_out_t *obj,
    uint32_t *xid);

extern void of_packet_out_buffer_id_set(
    of_packet_out_t *obj,
    uint32_t buffer_id);
extern void of_packet_out_buffer_id_get(
    of_packet_out_t *obj,
    uint32_t *buffer_id);

extern void of_packet_out_in_port_set(
    of_packet_out_t *obj,
    of_port_no_t in_port);
extern void of_packet_out_in_port_get(
    of_packet_out_t *obj,
    of_port_no_t *in_port);

extern int WARN_UNUSED_RESULT of_packet_out_actions_set(
    of_packet_out_t *obj,
    of_list_action_t *actions);
extern void of_packet_out_actions_bind(
    of_packet_out_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_packet_out_actions_get(
    of_packet_out_t *obj);

extern int WARN_UNUSED_RESULT of_packet_out_data_set(
    of_packet_out_t *obj,
    of_octets_t *data);
extern void of_packet_out_data_get(
    of_packet_out_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_port_desc_stats_reply */

extern void of_port_desc_stats_reply_xid_set(
    of_port_desc_stats_reply_t *obj,
    uint32_t xid);
extern void of_port_desc_stats_reply_xid_get(
    of_port_desc_stats_reply_t *obj,
    uint32_t *xid);

extern void of_port_desc_stats_reply_flags_set(
    of_port_desc_stats_reply_t *obj,
    uint16_t flags);
extern void of_port_desc_stats_reply_flags_get(
    of_port_desc_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_port_desc_stats_reply_entries_set(
    of_port_desc_stats_reply_t *obj,
    of_list_port_desc_t *entries);
extern void of_port_desc_stats_reply_entries_bind(
    of_port_desc_stats_reply_t *obj,
    of_list_port_desc_t *entries);
extern of_list_port_desc_t *of_port_desc_stats_reply_entries_get(
    of_port_desc_stats_reply_t *obj);

/* Unified accessor functions for of_port_desc_stats_request */

extern void of_port_desc_stats_request_xid_set(
    of_port_desc_stats_request_t *obj,
    uint32_t xid);
extern void of_port_desc_stats_request_xid_get(
    of_port_desc_stats_request_t *obj,
    uint32_t *xid);

extern void of_port_desc_stats_request_flags_set(
    of_port_desc_stats_request_t *obj,
    uint16_t flags);
extern void of_port_desc_stats_request_flags_get(
    of_port_desc_stats_request_t *obj,
    uint16_t *flags);

/* Unified accessor functions for of_port_mod */

extern void of_port_mod_xid_set(
    of_port_mod_t *obj,
    uint32_t xid);
extern void of_port_mod_xid_get(
    of_port_mod_t *obj,
    uint32_t *xid);

extern void of_port_mod_port_no_set(
    of_port_mod_t *obj,
    of_port_no_t port_no);
extern void of_port_mod_port_no_get(
    of_port_mod_t *obj,
    of_port_no_t *port_no);

extern void of_port_mod_hw_addr_set(
    of_port_mod_t *obj,
    of_mac_addr_t hw_addr);
extern void of_port_mod_hw_addr_get(
    of_port_mod_t *obj,
    of_mac_addr_t *hw_addr);

extern void of_port_mod_config_set(
    of_port_mod_t *obj,
    uint32_t config);
extern void of_port_mod_config_get(
    of_port_mod_t *obj,
    uint32_t *config);

extern void of_port_mod_mask_set(
    of_port_mod_t *obj,
    uint32_t mask);
extern void of_port_mod_mask_get(
    of_port_mod_t *obj,
    uint32_t *mask);

extern void of_port_mod_advertise_set(
    of_port_mod_t *obj,
    uint32_t advertise);
extern void of_port_mod_advertise_get(
    of_port_mod_t *obj,
    uint32_t *advertise);

/* Unified accessor functions for of_port_mod_failed_error_msg */

extern void of_port_mod_failed_error_msg_xid_set(
    of_port_mod_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_port_mod_failed_error_msg_xid_get(
    of_port_mod_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_port_mod_failed_error_msg_code_set(
    of_port_mod_failed_error_msg_t *obj,
    uint16_t code);
extern void of_port_mod_failed_error_msg_code_get(
    of_port_mod_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_port_mod_failed_error_msg_data_set(
    of_port_mod_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_port_mod_failed_error_msg_data_get(
    of_port_mod_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_port_stats_reply */

extern void of_port_stats_reply_xid_set(
    of_port_stats_reply_t *obj,
    uint32_t xid);
extern void of_port_stats_reply_xid_get(
    of_port_stats_reply_t *obj,
    uint32_t *xid);

extern void of_port_stats_reply_flags_set(
    of_port_stats_reply_t *obj,
    uint16_t flags);
extern void of_port_stats_reply_flags_get(
    of_port_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_port_stats_reply_entries_set(
    of_port_stats_reply_t *obj,
    of_list_port_stats_entry_t *entries);
extern void of_port_stats_reply_entries_bind(
    of_port_stats_reply_t *obj,
    of_list_port_stats_entry_t *entries);
extern of_list_port_stats_entry_t *of_port_stats_reply_entries_get(
    of_port_stats_reply_t *obj);

/* Unified accessor functions for of_port_stats_request */

extern void of_port_stats_request_xid_set(
    of_port_stats_request_t *obj,
    uint32_t xid);
extern void of_port_stats_request_xid_get(
    of_port_stats_request_t *obj,
    uint32_t *xid);

extern void of_port_stats_request_flags_set(
    of_port_stats_request_t *obj,
    uint16_t flags);
extern void of_port_stats_request_flags_get(
    of_port_stats_request_t *obj,
    uint16_t *flags);

extern void of_port_stats_request_port_no_set(
    of_port_stats_request_t *obj,
    of_port_no_t port_no);
extern void of_port_stats_request_port_no_get(
    of_port_stats_request_t *obj,
    of_port_no_t *port_no);

/* Unified accessor functions for of_port_status */

extern void of_port_status_xid_set(
    of_port_status_t *obj,
    uint32_t xid);
extern void of_port_status_xid_get(
    of_port_status_t *obj,
    uint32_t *xid);

extern void of_port_status_reason_set(
    of_port_status_t *obj,
    uint8_t reason);
extern void of_port_status_reason_get(
    of_port_status_t *obj,
    uint8_t *reason);

extern void of_port_status_desc_set(
    of_port_status_t *obj,
    of_port_desc_t *desc);
extern void of_port_status_desc_bind(
    of_port_status_t *obj,
    of_port_desc_t *desc);
extern of_port_desc_t *of_port_status_desc_get(
    of_port_status_t *obj);

/* Unified accessor functions for of_queue_get_config_reply */

extern void of_queue_get_config_reply_xid_set(
    of_queue_get_config_reply_t *obj,
    uint32_t xid);
extern void of_queue_get_config_reply_xid_get(
    of_queue_get_config_reply_t *obj,
    uint32_t *xid);

extern void of_queue_get_config_reply_port_set(
    of_queue_get_config_reply_t *obj,
    of_port_no_t port);
extern void of_queue_get_config_reply_port_get(
    of_queue_get_config_reply_t *obj,
    of_port_no_t *port);

extern int WARN_UNUSED_RESULT of_queue_get_config_reply_queues_set(
    of_queue_get_config_reply_t *obj,
    of_list_packet_queue_t *queues);
extern void of_queue_get_config_reply_queues_bind(
    of_queue_get_config_reply_t *obj,
    of_list_packet_queue_t *queues);
extern of_list_packet_queue_t *of_queue_get_config_reply_queues_get(
    of_queue_get_config_reply_t *obj);

/* Unified accessor functions for of_queue_get_config_request */

extern void of_queue_get_config_request_xid_set(
    of_queue_get_config_request_t *obj,
    uint32_t xid);
extern void of_queue_get_config_request_xid_get(
    of_queue_get_config_request_t *obj,
    uint32_t *xid);

extern void of_queue_get_config_request_port_set(
    of_queue_get_config_request_t *obj,
    of_port_no_t port);
extern void of_queue_get_config_request_port_get(
    of_queue_get_config_request_t *obj,
    of_port_no_t *port);

/* Unified accessor functions for of_queue_op_failed_error_msg */

extern void of_queue_op_failed_error_msg_xid_set(
    of_queue_op_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_queue_op_failed_error_msg_xid_get(
    of_queue_op_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_queue_op_failed_error_msg_code_set(
    of_queue_op_failed_error_msg_t *obj,
    uint16_t code);
extern void of_queue_op_failed_error_msg_code_get(
    of_queue_op_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_queue_op_failed_error_msg_data_set(
    of_queue_op_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_queue_op_failed_error_msg_data_get(
    of_queue_op_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_queue_stats_reply */

extern void of_queue_stats_reply_xid_set(
    of_queue_stats_reply_t *obj,
    uint32_t xid);
extern void of_queue_stats_reply_xid_get(
    of_queue_stats_reply_t *obj,
    uint32_t *xid);

extern void of_queue_stats_reply_flags_set(
    of_queue_stats_reply_t *obj,
    uint16_t flags);
extern void of_queue_stats_reply_flags_get(
    of_queue_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_queue_stats_reply_entries_set(
    of_queue_stats_reply_t *obj,
    of_list_queue_stats_entry_t *entries);
extern void of_queue_stats_reply_entries_bind(
    of_queue_stats_reply_t *obj,
    of_list_queue_stats_entry_t *entries);
extern of_list_queue_stats_entry_t *of_queue_stats_reply_entries_get(
    of_queue_stats_reply_t *obj);

/* Unified accessor functions for of_queue_stats_request */

extern void of_queue_stats_request_xid_set(
    of_queue_stats_request_t *obj,
    uint32_t xid);
extern void of_queue_stats_request_xid_get(
    of_queue_stats_request_t *obj,
    uint32_t *xid);

extern void of_queue_stats_request_flags_set(
    of_queue_stats_request_t *obj,
    uint16_t flags);
extern void of_queue_stats_request_flags_get(
    of_queue_stats_request_t *obj,
    uint16_t *flags);

extern void of_queue_stats_request_port_no_set(
    of_queue_stats_request_t *obj,
    of_port_no_t port_no);
extern void of_queue_stats_request_port_no_get(
    of_queue_stats_request_t *obj,
    of_port_no_t *port_no);

extern void of_queue_stats_request_queue_id_set(
    of_queue_stats_request_t *obj,
    uint32_t queue_id);
extern void of_queue_stats_request_queue_id_get(
    of_queue_stats_request_t *obj,
    uint32_t *queue_id);

/* Unified accessor functions for of_role_reply */

extern void of_role_reply_xid_set(
    of_role_reply_t *obj,
    uint32_t xid);
extern void of_role_reply_xid_get(
    of_role_reply_t *obj,
    uint32_t *xid);

extern void of_role_reply_role_set(
    of_role_reply_t *obj,
    uint32_t role);
extern void of_role_reply_role_get(
    of_role_reply_t *obj,
    uint32_t *role);

extern void of_role_reply_generation_id_set(
    of_role_reply_t *obj,
    uint64_t generation_id);
extern void of_role_reply_generation_id_get(
    of_role_reply_t *obj,
    uint64_t *generation_id);

/* Unified accessor functions for of_role_request */

extern void of_role_request_xid_set(
    of_role_request_t *obj,
    uint32_t xid);
extern void of_role_request_xid_get(
    of_role_request_t *obj,
    uint32_t *xid);

extern void of_role_request_role_set(
    of_role_request_t *obj,
    uint32_t role);
extern void of_role_request_role_get(
    of_role_request_t *obj,
    uint32_t *role);

extern void of_role_request_generation_id_set(
    of_role_request_t *obj,
    uint64_t generation_id);
extern void of_role_request_generation_id_get(
    of_role_request_t *obj,
    uint64_t *generation_id);

/* Unified accessor functions for of_role_request_failed_error_msg */

extern void of_role_request_failed_error_msg_xid_set(
    of_role_request_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_role_request_failed_error_msg_xid_get(
    of_role_request_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_role_request_failed_error_msg_code_set(
    of_role_request_failed_error_msg_t *obj,
    uint16_t code);
extern void of_role_request_failed_error_msg_code_get(
    of_role_request_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_role_request_failed_error_msg_data_set(
    of_role_request_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_role_request_failed_error_msg_data_get(
    of_role_request_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_set_config */

extern void of_set_config_xid_set(
    of_set_config_t *obj,
    uint32_t xid);
extern void of_set_config_xid_get(
    of_set_config_t *obj,
    uint32_t *xid);

extern void of_set_config_flags_set(
    of_set_config_t *obj,
    uint16_t flags);
extern void of_set_config_flags_get(
    of_set_config_t *obj,
    uint16_t *flags);

extern void of_set_config_miss_send_len_set(
    of_set_config_t *obj,
    uint16_t miss_send_len);
extern void of_set_config_miss_send_len_get(
    of_set_config_t *obj,
    uint16_t *miss_send_len);

/* Unified accessor functions for of_stats_reply */

extern void of_stats_reply_xid_set(
    of_stats_reply_t *obj,
    uint32_t xid);
extern void of_stats_reply_xid_get(
    of_stats_reply_t *obj,
    uint32_t *xid);

extern void of_stats_reply_flags_set(
    of_stats_reply_t *obj,
    uint16_t flags);
extern void of_stats_reply_flags_get(
    of_stats_reply_t *obj,
    uint16_t *flags);

/* Unified accessor functions for of_stats_request */

extern void of_stats_request_xid_set(
    of_stats_request_t *obj,
    uint32_t xid);
extern void of_stats_request_xid_get(
    of_stats_request_t *obj,
    uint32_t *xid);

extern void of_stats_request_flags_set(
    of_stats_request_t *obj,
    uint16_t flags);
extern void of_stats_request_flags_get(
    of_stats_request_t *obj,
    uint16_t *flags);

/* Unified accessor functions for of_switch_config_failed_error_msg */

extern void of_switch_config_failed_error_msg_xid_set(
    of_switch_config_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_switch_config_failed_error_msg_xid_get(
    of_switch_config_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_switch_config_failed_error_msg_code_set(
    of_switch_config_failed_error_msg_t *obj,
    uint16_t code);
extern void of_switch_config_failed_error_msg_code_get(
    of_switch_config_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_switch_config_failed_error_msg_data_set(
    of_switch_config_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_switch_config_failed_error_msg_data_get(
    of_switch_config_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_table_features_failed_error_msg */

extern void of_table_features_failed_error_msg_xid_set(
    of_table_features_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_table_features_failed_error_msg_xid_get(
    of_table_features_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_table_features_failed_error_msg_code_set(
    of_table_features_failed_error_msg_t *obj,
    uint16_t code);
extern void of_table_features_failed_error_msg_code_get(
    of_table_features_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_table_features_failed_error_msg_data_set(
    of_table_features_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_table_features_failed_error_msg_data_get(
    of_table_features_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_table_features_stats_reply */

extern void of_table_features_stats_reply_xid_set(
    of_table_features_stats_reply_t *obj,
    uint32_t xid);
extern void of_table_features_stats_reply_xid_get(
    of_table_features_stats_reply_t *obj,
    uint32_t *xid);

extern void of_table_features_stats_reply_flags_set(
    of_table_features_stats_reply_t *obj,
    uint16_t flags);
extern void of_table_features_stats_reply_flags_get(
    of_table_features_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_table_features_stats_reply_entries_set(
    of_table_features_stats_reply_t *obj,
    of_list_table_features_t *entries);
extern void of_table_features_stats_reply_entries_bind(
    of_table_features_stats_reply_t *obj,
    of_list_table_features_t *entries);
extern of_list_table_features_t *of_table_features_stats_reply_entries_get(
    of_table_features_stats_reply_t *obj);

/* Unified accessor functions for of_table_features_stats_request */

extern void of_table_features_stats_request_xid_set(
    of_table_features_stats_request_t *obj,
    uint32_t xid);
extern void of_table_features_stats_request_xid_get(
    of_table_features_stats_request_t *obj,
    uint32_t *xid);

extern void of_table_features_stats_request_flags_set(
    of_table_features_stats_request_t *obj,
    uint16_t flags);
extern void of_table_features_stats_request_flags_get(
    of_table_features_stats_request_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_table_features_stats_request_entries_set(
    of_table_features_stats_request_t *obj,
    of_list_table_features_t *entries);
extern void of_table_features_stats_request_entries_bind(
    of_table_features_stats_request_t *obj,
    of_list_table_features_t *entries);
extern of_list_table_features_t *of_table_features_stats_request_entries_get(
    of_table_features_stats_request_t *obj);

/* Unified accessor functions for of_table_mod */

extern void of_table_mod_xid_set(
    of_table_mod_t *obj,
    uint32_t xid);
extern void of_table_mod_xid_get(
    of_table_mod_t *obj,
    uint32_t *xid);

extern void of_table_mod_table_id_set(
    of_table_mod_t *obj,
    uint8_t table_id);
extern void of_table_mod_table_id_get(
    of_table_mod_t *obj,
    uint8_t *table_id);

extern void of_table_mod_config_set(
    of_table_mod_t *obj,
    uint32_t config);
extern void of_table_mod_config_get(
    of_table_mod_t *obj,
    uint32_t *config);

/* Unified accessor functions for of_table_mod_failed_error_msg */

extern void of_table_mod_failed_error_msg_xid_set(
    of_table_mod_failed_error_msg_t *obj,
    uint32_t xid);
extern void of_table_mod_failed_error_msg_xid_get(
    of_table_mod_failed_error_msg_t *obj,
    uint32_t *xid);

extern void of_table_mod_failed_error_msg_code_set(
    of_table_mod_failed_error_msg_t *obj,
    uint16_t code);
extern void of_table_mod_failed_error_msg_code_get(
    of_table_mod_failed_error_msg_t *obj,
    uint16_t *code);

extern int WARN_UNUSED_RESULT of_table_mod_failed_error_msg_data_set(
    of_table_mod_failed_error_msg_t *obj,
    of_octets_t *data);
extern void of_table_mod_failed_error_msg_data_get(
    of_table_mod_failed_error_msg_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_table_stats_reply */

extern void of_table_stats_reply_xid_set(
    of_table_stats_reply_t *obj,
    uint32_t xid);
extern void of_table_stats_reply_xid_get(
    of_table_stats_reply_t *obj,
    uint32_t *xid);

extern void of_table_stats_reply_flags_set(
    of_table_stats_reply_t *obj,
    uint16_t flags);
extern void of_table_stats_reply_flags_get(
    of_table_stats_reply_t *obj,
    uint16_t *flags);

extern int WARN_UNUSED_RESULT of_table_stats_reply_entries_set(
    of_table_stats_reply_t *obj,
    of_list_table_stats_entry_t *entries);
extern void of_table_stats_reply_entries_bind(
    of_table_stats_reply_t *obj,
    of_list_table_stats_entry_t *entries);
extern of_list_table_stats_entry_t *of_table_stats_reply_entries_get(
    of_table_stats_reply_t *obj);

/* Unified accessor functions for of_table_stats_request */

extern void of_table_stats_request_xid_set(
    of_table_stats_request_t *obj,
    uint32_t xid);
extern void of_table_stats_request_xid_get(
    of_table_stats_request_t *obj,
    uint32_t *xid);

extern void of_table_stats_request_flags_set(
    of_table_stats_request_t *obj,
    uint16_t flags);
extern void of_table_stats_request_flags_get(
    of_table_stats_request_t *obj,
    uint16_t *flags);

/* Unified accessor functions for of_action_bsn */

extern void of_action_bsn_experimenter_set(
    of_action_bsn_t *obj,
    uint32_t experimenter);
extern void of_action_bsn_experimenter_get(
    of_action_bsn_t *obj,
    uint32_t *experimenter);

extern void of_action_bsn_subtype_set(
    of_action_bsn_t *obj,
    uint32_t subtype);
extern void of_action_bsn_subtype_get(
    of_action_bsn_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_action_bsn_mirror */

extern void of_action_bsn_mirror_experimenter_set(
    of_action_bsn_mirror_t *obj,
    uint32_t experimenter);
extern void of_action_bsn_mirror_experimenter_get(
    of_action_bsn_mirror_t *obj,
    uint32_t *experimenter);

extern void of_action_bsn_mirror_subtype_set(
    of_action_bsn_mirror_t *obj,
    uint32_t subtype);
extern void of_action_bsn_mirror_subtype_get(
    of_action_bsn_mirror_t *obj,
    uint32_t *subtype);

extern void of_action_bsn_mirror_dest_port_set(
    of_action_bsn_mirror_t *obj,
    uint32_t dest_port);
extern void of_action_bsn_mirror_dest_port_get(
    of_action_bsn_mirror_t *obj,
    uint32_t *dest_port);

extern void of_action_bsn_mirror_vlan_tag_set(
    of_action_bsn_mirror_t *obj,
    uint32_t vlan_tag);
extern void of_action_bsn_mirror_vlan_tag_get(
    of_action_bsn_mirror_t *obj,
    uint32_t *vlan_tag);

extern void of_action_bsn_mirror_copy_stage_set(
    of_action_bsn_mirror_t *obj,
    uint8_t copy_stage);
extern void of_action_bsn_mirror_copy_stage_get(
    of_action_bsn_mirror_t *obj,
    uint8_t *copy_stage);

/* Unified accessor functions for of_action_bsn_set_tunnel_dst */

extern void of_action_bsn_set_tunnel_dst_experimenter_set(
    of_action_bsn_set_tunnel_dst_t *obj,
    uint32_t experimenter);
extern void of_action_bsn_set_tunnel_dst_experimenter_get(
    of_action_bsn_set_tunnel_dst_t *obj,
    uint32_t *experimenter);

extern void of_action_bsn_set_tunnel_dst_subtype_set(
    of_action_bsn_set_tunnel_dst_t *obj,
    uint32_t subtype);
extern void of_action_bsn_set_tunnel_dst_subtype_get(
    of_action_bsn_set_tunnel_dst_t *obj,
    uint32_t *subtype);

extern void of_action_bsn_set_tunnel_dst_dst_set(
    of_action_bsn_set_tunnel_dst_t *obj,
    uint32_t dst);
extern void of_action_bsn_set_tunnel_dst_dst_get(
    of_action_bsn_set_tunnel_dst_t *obj,
    uint32_t *dst);

/* Unified accessor functions for of_action_copy_ttl_in */

/* Unified accessor functions for of_action_copy_ttl_out */

/* Unified accessor functions for of_action_dec_mpls_ttl */

/* Unified accessor functions for of_action_dec_nw_ttl */

/* Unified accessor functions for of_action_enqueue */

extern void of_action_enqueue_port_set(
    of_action_enqueue_t *obj,
    of_port_no_t port);
extern void of_action_enqueue_port_get(
    of_action_enqueue_t *obj,
    of_port_no_t *port);

extern void of_action_enqueue_queue_id_set(
    of_action_enqueue_t *obj,
    uint32_t queue_id);
extern void of_action_enqueue_queue_id_get(
    of_action_enqueue_t *obj,
    uint32_t *queue_id);

/* Unified accessor functions for of_action_experimenter */

extern void of_action_experimenter_experimenter_set(
    of_action_experimenter_t *obj,
    uint32_t experimenter);
extern void of_action_experimenter_experimenter_get(
    of_action_experimenter_t *obj,
    uint32_t *experimenter);

extern int WARN_UNUSED_RESULT of_action_experimenter_data_set(
    of_action_experimenter_t *obj,
    of_octets_t *data);
extern void of_action_experimenter_data_get(
    of_action_experimenter_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_action_group */

extern void of_action_group_group_id_set(
    of_action_group_t *obj,
    uint32_t group_id);
extern void of_action_group_group_id_get(
    of_action_group_t *obj,
    uint32_t *group_id);

/* Unified accessor functions for of_action_header */

/* Unified accessor functions for of_action_id_bsn */

extern void of_action_id_bsn_experimenter_set(
    of_action_id_bsn_t *obj,
    uint32_t experimenter);
extern void of_action_id_bsn_experimenter_get(
    of_action_id_bsn_t *obj,
    uint32_t *experimenter);

extern void of_action_id_bsn_subtype_set(
    of_action_id_bsn_t *obj,
    uint32_t subtype);
extern void of_action_id_bsn_subtype_get(
    of_action_id_bsn_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_action_id_bsn_mirror */

extern void of_action_id_bsn_mirror_experimenter_set(
    of_action_id_bsn_mirror_t *obj,
    uint32_t experimenter);
extern void of_action_id_bsn_mirror_experimenter_get(
    of_action_id_bsn_mirror_t *obj,
    uint32_t *experimenter);

extern void of_action_id_bsn_mirror_subtype_set(
    of_action_id_bsn_mirror_t *obj,
    uint32_t subtype);
extern void of_action_id_bsn_mirror_subtype_get(
    of_action_id_bsn_mirror_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_action_id_bsn_set_tunnel_dst */

extern void of_action_id_bsn_set_tunnel_dst_experimenter_set(
    of_action_id_bsn_set_tunnel_dst_t *obj,
    uint32_t experimenter);
extern void of_action_id_bsn_set_tunnel_dst_experimenter_get(
    of_action_id_bsn_set_tunnel_dst_t *obj,
    uint32_t *experimenter);

extern void of_action_id_bsn_set_tunnel_dst_subtype_set(
    of_action_id_bsn_set_tunnel_dst_t *obj,
    uint32_t subtype);
extern void of_action_id_bsn_set_tunnel_dst_subtype_get(
    of_action_id_bsn_set_tunnel_dst_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_action_id_copy_ttl_in */

/* Unified accessor functions for of_action_id_copy_ttl_out */

/* Unified accessor functions for of_action_id_dec_mpls_ttl */

/* Unified accessor functions for of_action_id_dec_nw_ttl */

/* Unified accessor functions for of_action_id_experimenter */

extern void of_action_id_experimenter_experimenter_set(
    of_action_id_experimenter_t *obj,
    uint32_t experimenter);
extern void of_action_id_experimenter_experimenter_get(
    of_action_id_experimenter_t *obj,
    uint32_t *experimenter);

/* Unified accessor functions for of_action_id_group */

/* Unified accessor functions for of_action_id_header */

/* Unified accessor functions for of_action_id_nicira */

extern void of_action_id_nicira_experimenter_set(
    of_action_id_nicira_t *obj,
    uint32_t experimenter);
extern void of_action_id_nicira_experimenter_get(
    of_action_id_nicira_t *obj,
    uint32_t *experimenter);

extern void of_action_id_nicira_subtype_set(
    of_action_id_nicira_t *obj,
    uint16_t subtype);
extern void of_action_id_nicira_subtype_get(
    of_action_id_nicira_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_nicira_dec_ttl */

extern void of_action_id_nicira_dec_ttl_experimenter_set(
    of_action_id_nicira_dec_ttl_t *obj,
    uint32_t experimenter);
extern void of_action_id_nicira_dec_ttl_experimenter_get(
    of_action_id_nicira_dec_ttl_t *obj,
    uint32_t *experimenter);

extern void of_action_id_nicira_dec_ttl_subtype_set(
    of_action_id_nicira_dec_ttl_t *obj,
    uint16_t subtype);
extern void of_action_id_nicira_dec_ttl_subtype_get(
    of_action_id_nicira_dec_ttl_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa */

extern void of_action_id_ofdpa_experimenter_set(
    of_action_id_ofdpa_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_experimenter_get(
    of_action_id_ofdpa_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_subtype_set(
    of_action_id_ofdpa_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_subtype_get(
    of_action_id_ofdpa_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_check_drop_status */

extern void of_action_id_ofdpa_check_drop_status_experimenter_set(
    of_action_id_ofdpa_check_drop_status_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_check_drop_status_experimenter_get(
    of_action_id_ofdpa_check_drop_status_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_check_drop_status_subtype_set(
    of_action_id_ofdpa_check_drop_status_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_check_drop_status_subtype_get(
    of_action_id_ofdpa_check_drop_status_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_class_based_count */

extern void of_action_id_ofdpa_class_based_count_experimenter_set(
    of_action_id_ofdpa_class_based_count_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_class_based_count_experimenter_get(
    of_action_id_ofdpa_class_based_count_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_class_based_count_subtype_set(
    of_action_id_ofdpa_class_based_count_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_class_based_count_subtype_get(
    of_action_id_ofdpa_class_based_count_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_color_based_count */

extern void of_action_id_ofdpa_color_based_count_experimenter_set(
    of_action_id_ofdpa_color_based_count_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_color_based_count_experimenter_get(
    of_action_id_ofdpa_color_based_count_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_color_based_count_subtype_set(
    of_action_id_ofdpa_color_based_count_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_color_based_count_subtype_get(
    of_action_id_ofdpa_color_based_count_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_color_based_count_1 */

extern void of_action_id_ofdpa_color_based_count_1_experimenter_set(
    of_action_id_ofdpa_color_based_count_1_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_color_based_count_1_experimenter_get(
    of_action_id_ofdpa_color_based_count_1_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_color_based_count_1_subtype_set(
    of_action_id_ofdpa_color_based_count_1_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_color_based_count_1_subtype_get(
    of_action_id_ofdpa_color_based_count_1_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_color_based_count_2 */

extern void of_action_id_ofdpa_color_based_count_2_experimenter_set(
    of_action_id_ofdpa_color_based_count_2_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_color_based_count_2_experimenter_get(
    of_action_id_ofdpa_color_based_count_2_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_color_based_count_2_subtype_set(
    of_action_id_ofdpa_color_based_count_2_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_color_based_count_2_subtype_get(
    of_action_id_ofdpa_color_based_count_2_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_color_based_count_3 */

extern void of_action_id_ofdpa_color_based_count_3_experimenter_set(
    of_action_id_ofdpa_color_based_count_3_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_color_based_count_3_experimenter_get(
    of_action_id_ofdpa_color_based_count_3_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_color_based_count_3_subtype_set(
    of_action_id_ofdpa_color_based_count_3_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_color_based_count_3_subtype_get(
    of_action_id_ofdpa_color_based_count_3_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_copy_tc_in */

extern void of_action_id_ofdpa_copy_tc_in_experimenter_set(
    of_action_id_ofdpa_copy_tc_in_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_copy_tc_in_experimenter_get(
    of_action_id_ofdpa_copy_tc_in_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_copy_tc_in_subtype_set(
    of_action_id_ofdpa_copy_tc_in_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_copy_tc_in_subtype_get(
    of_action_id_ofdpa_copy_tc_in_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_copy_tc_out */

extern void of_action_id_ofdpa_copy_tc_out_experimenter_set(
    of_action_id_ofdpa_copy_tc_out_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_copy_tc_out_experimenter_get(
    of_action_id_ofdpa_copy_tc_out_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_copy_tc_out_subtype_set(
    of_action_id_ofdpa_copy_tc_out_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_copy_tc_out_subtype_get(
    of_action_id_ofdpa_copy_tc_out_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_dec_ttl_mtu */

extern void of_action_id_ofdpa_dec_ttl_mtu_experimenter_set(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_dec_ttl_mtu_experimenter_get(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_dec_ttl_mtu_subtype_set(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_dec_ttl_mtu_subtype_get(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_oam_lm_rx_count */

extern void of_action_id_ofdpa_oam_lm_rx_count_experimenter_set(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_oam_lm_rx_count_experimenter_get(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_oam_lm_rx_count_subtype_set(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_oam_lm_rx_count_subtype_get(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_oam_lm_tx_count */

extern void of_action_id_ofdpa_oam_lm_tx_count_experimenter_set(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_oam_lm_tx_count_experimenter_get(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_oam_lm_tx_count_subtype_set(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_oam_lm_tx_count_subtype_get(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_pop_cw */

extern void of_action_id_ofdpa_pop_cw_experimenter_set(
    of_action_id_ofdpa_pop_cw_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_pop_cw_experimenter_get(
    of_action_id_ofdpa_pop_cw_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_pop_cw_subtype_set(
    of_action_id_ofdpa_pop_cw_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_pop_cw_subtype_get(
    of_action_id_ofdpa_pop_cw_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_pop_l2hdr */

extern void of_action_id_ofdpa_pop_l2hdr_experimenter_set(
    of_action_id_ofdpa_pop_l2hdr_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_pop_l2hdr_experimenter_get(
    of_action_id_ofdpa_pop_l2hdr_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_pop_l2hdr_subtype_set(
    of_action_id_ofdpa_pop_l2hdr_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_pop_l2hdr_subtype_get(
    of_action_id_ofdpa_pop_l2hdr_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_push_cw */

extern void of_action_id_ofdpa_push_cw_experimenter_set(
    of_action_id_ofdpa_push_cw_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_push_cw_experimenter_get(
    of_action_id_ofdpa_push_cw_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_push_cw_subtype_set(
    of_action_id_ofdpa_push_cw_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_push_cw_subtype_get(
    of_action_id_ofdpa_push_cw_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_push_l2hdr */

extern void of_action_id_ofdpa_push_l2hdr_experimenter_set(
    of_action_id_ofdpa_push_l2hdr_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_push_l2hdr_experimenter_get(
    of_action_id_ofdpa_push_l2hdr_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_push_l2hdr_subtype_set(
    of_action_id_ofdpa_push_l2hdr_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_push_l2hdr_subtype_get(
    of_action_id_ofdpa_push_l2hdr_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_set_counter_fields */

extern void of_action_id_ofdpa_set_counter_fields_experimenter_set(
    of_action_id_ofdpa_set_counter_fields_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_set_counter_fields_experimenter_get(
    of_action_id_ofdpa_set_counter_fields_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_set_counter_fields_subtype_set(
    of_action_id_ofdpa_set_counter_fields_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_set_counter_fields_subtype_get(
    of_action_id_ofdpa_set_counter_fields_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_set_mpls_pcpdei_from_table */

extern void of_action_id_ofdpa_set_mpls_pcpdei_from_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_set_mpls_pcpdei_from_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_set_mpls_pcpdei_from_table_subtype_set(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_set_mpls_pcpdei_from_table_subtype_get(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table */

extern void of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_subtype_set(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_subtype_get(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_set_mpls_tc_from_tunnel_table */

extern void of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_subtype_set(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_subtype_get(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_set_mpls_tc_from_vpn_table */

extern void of_action_id_ofdpa_set_mpls_tc_from_vpn_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_set_mpls_tc_from_vpn_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_set_mpls_tc_from_vpn_table_subtype_set(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_set_mpls_tc_from_vpn_table_subtype_get(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_ofdpa_set_pcpdei_from_table */

extern void of_action_id_ofdpa_set_pcpdei_from_table_experimenter_set(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    uint32_t experimenter);
extern void of_action_id_ofdpa_set_pcpdei_from_table_experimenter_get(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    uint32_t *experimenter);

extern void of_action_id_ofdpa_set_pcpdei_from_table_subtype_set(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t subtype);
extern void of_action_id_ofdpa_set_pcpdei_from_table_subtype_get(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_onf */

extern void of_action_id_onf_experimenter_set(
    of_action_id_onf_t *obj,
    uint32_t experimenter);
extern void of_action_id_onf_experimenter_get(
    of_action_id_onf_t *obj,
    uint32_t *experimenter);

extern void of_action_id_onf_subtype_set(
    of_action_id_onf_t *obj,
    uint16_t subtype);
extern void of_action_id_onf_subtype_get(
    of_action_id_onf_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_onf_copy_field */

extern void of_action_id_onf_copy_field_experimenter_set(
    of_action_id_onf_copy_field_t *obj,
    uint32_t experimenter);
extern void of_action_id_onf_copy_field_experimenter_get(
    of_action_id_onf_copy_field_t *obj,
    uint32_t *experimenter);

extern void of_action_id_onf_copy_field_subtype_set(
    of_action_id_onf_copy_field_t *obj,
    uint16_t subtype);
extern void of_action_id_onf_copy_field_subtype_get(
    of_action_id_onf_copy_field_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_id_output */

/* Unified accessor functions for of_action_id_pop_mpls */

/* Unified accessor functions for of_action_id_pop_pbb */

/* Unified accessor functions for of_action_id_pop_vlan */

/* Unified accessor functions for of_action_id_push_mpls */

/* Unified accessor functions for of_action_id_push_pbb */

/* Unified accessor functions for of_action_id_push_vlan */

/* Unified accessor functions for of_action_id_set_field */

/* Unified accessor functions for of_action_id_set_mpls_ttl */

/* Unified accessor functions for of_action_id_set_nw_ttl */

/* Unified accessor functions for of_action_id_set_queue */

/* Unified accessor functions for of_action_nicira */

extern void of_action_nicira_experimenter_set(
    of_action_nicira_t *obj,
    uint32_t experimenter);
extern void of_action_nicira_experimenter_get(
    of_action_nicira_t *obj,
    uint32_t *experimenter);

extern void of_action_nicira_subtype_set(
    of_action_nicira_t *obj,
    uint16_t subtype);
extern void of_action_nicira_subtype_get(
    of_action_nicira_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_nicira_dec_ttl */

extern void of_action_nicira_dec_ttl_experimenter_set(
    of_action_nicira_dec_ttl_t *obj,
    uint32_t experimenter);
extern void of_action_nicira_dec_ttl_experimenter_get(
    of_action_nicira_dec_ttl_t *obj,
    uint32_t *experimenter);

extern void of_action_nicira_dec_ttl_subtype_set(
    of_action_nicira_dec_ttl_t *obj,
    uint16_t subtype);
extern void of_action_nicira_dec_ttl_subtype_get(
    of_action_nicira_dec_ttl_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa */

extern void of_action_ofdpa_experimenter_set(
    of_action_ofdpa_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_experimenter_get(
    of_action_ofdpa_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_subtype_set(
    of_action_ofdpa_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_subtype_get(
    of_action_ofdpa_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa_check_drop_status */

extern void of_action_ofdpa_check_drop_status_experimenter_set(
    of_action_ofdpa_check_drop_status_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_check_drop_status_experimenter_get(
    of_action_ofdpa_check_drop_status_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_check_drop_status_subtype_set(
    of_action_ofdpa_check_drop_status_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_check_drop_status_subtype_get(
    of_action_ofdpa_check_drop_status_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_check_drop_status_drop_type_set(
    of_action_ofdpa_check_drop_status_t *obj,
    uint16_t drop_type);
extern void of_action_ofdpa_check_drop_status_drop_type_get(
    of_action_ofdpa_check_drop_status_t *obj,
    uint16_t *drop_type);

extern void of_action_ofdpa_check_drop_status_drop_index_set(
    of_action_ofdpa_check_drop_status_t *obj,
    uint32_t drop_index);
extern void of_action_ofdpa_check_drop_status_drop_index_get(
    of_action_ofdpa_check_drop_status_t *obj,
    uint32_t *drop_index);

/* Unified accessor functions for of_action_ofdpa_class_based_count */

extern void of_action_ofdpa_class_based_count_experimenter_set(
    of_action_ofdpa_class_based_count_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_class_based_count_experimenter_get(
    of_action_ofdpa_class_based_count_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_class_based_count_subtype_set(
    of_action_ofdpa_class_based_count_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_class_based_count_subtype_get(
    of_action_ofdpa_class_based_count_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_class_based_count_index_set(
    of_action_ofdpa_class_based_count_t *obj,
    uint32_t index);
extern void of_action_ofdpa_class_based_count_index_get(
    of_action_ofdpa_class_based_count_t *obj,
    uint32_t *index);

/* Unified accessor functions for of_action_ofdpa_color_based_count */

extern void of_action_ofdpa_color_based_count_experimenter_set(
    of_action_ofdpa_color_based_count_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_color_based_count_experimenter_get(
    of_action_ofdpa_color_based_count_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_color_based_count_subtype_set(
    of_action_ofdpa_color_based_count_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_color_based_count_subtype_get(
    of_action_ofdpa_color_based_count_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_color_based_count_index_set(
    of_action_ofdpa_color_based_count_t *obj,
    uint32_t index);
extern void of_action_ofdpa_color_based_count_index_get(
    of_action_ofdpa_color_based_count_t *obj,
    uint32_t *index);

/* Unified accessor functions for of_action_ofdpa_color_based_count_1 */

extern void of_action_ofdpa_color_based_count_1_experimenter_set(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_color_based_count_1_experimenter_get(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_color_based_count_1_subtype_set(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_color_based_count_1_subtype_get(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_color_based_count_1_index_set(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint32_t index);
extern void of_action_ofdpa_color_based_count_1_index_get(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint32_t *index);

/* Unified accessor functions for of_action_ofdpa_color_based_count_2 */

extern void of_action_ofdpa_color_based_count_2_experimenter_set(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_color_based_count_2_experimenter_get(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_color_based_count_2_subtype_set(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_color_based_count_2_subtype_get(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_color_based_count_2_index_set(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint32_t index);
extern void of_action_ofdpa_color_based_count_2_index_get(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint32_t *index);

/* Unified accessor functions for of_action_ofdpa_color_based_count_3 */

extern void of_action_ofdpa_color_based_count_3_experimenter_set(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_color_based_count_3_experimenter_get(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_color_based_count_3_subtype_set(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_color_based_count_3_subtype_get(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_color_based_count_3_index_set(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint32_t index);
extern void of_action_ofdpa_color_based_count_3_index_get(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint32_t *index);

/* Unified accessor functions for of_action_ofdpa_copy_tc_in */

extern void of_action_ofdpa_copy_tc_in_experimenter_set(
    of_action_ofdpa_copy_tc_in_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_copy_tc_in_experimenter_get(
    of_action_ofdpa_copy_tc_in_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_copy_tc_in_subtype_set(
    of_action_ofdpa_copy_tc_in_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_copy_tc_in_subtype_get(
    of_action_ofdpa_copy_tc_in_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa_copy_tc_out */

extern void of_action_ofdpa_copy_tc_out_experimenter_set(
    of_action_ofdpa_copy_tc_out_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_copy_tc_out_experimenter_get(
    of_action_ofdpa_copy_tc_out_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_copy_tc_out_subtype_set(
    of_action_ofdpa_copy_tc_out_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_copy_tc_out_subtype_get(
    of_action_ofdpa_copy_tc_out_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa_dec_ttl_mtu */

extern void of_action_ofdpa_dec_ttl_mtu_experimenter_set(
    of_action_ofdpa_dec_ttl_mtu_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_dec_ttl_mtu_experimenter_get(
    of_action_ofdpa_dec_ttl_mtu_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_dec_ttl_mtu_subtype_set(
    of_action_ofdpa_dec_ttl_mtu_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_dec_ttl_mtu_subtype_get(
    of_action_ofdpa_dec_ttl_mtu_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa_oam_lm_rx_count */

extern void of_action_ofdpa_oam_lm_rx_count_experimenter_set(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_oam_lm_rx_count_experimenter_get(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_oam_lm_rx_count_subtype_set(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_oam_lm_rx_count_subtype_get(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_oam_lm_rx_count_traffic_class_set(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint8_t traffic_class);
extern void of_action_ofdpa_oam_lm_rx_count_traffic_class_get(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint8_t *traffic_class);

extern void of_action_ofdpa_oam_lm_rx_count_lmep_id_set(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t lmep_id);
extern void of_action_ofdpa_oam_lm_rx_count_lmep_id_get(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t *lmep_id);

/* Unified accessor functions for of_action_ofdpa_oam_lm_tx_count */

extern void of_action_ofdpa_oam_lm_tx_count_experimenter_set(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_oam_lm_tx_count_experimenter_get(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_oam_lm_tx_count_subtype_set(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_oam_lm_tx_count_subtype_get(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_oam_lm_tx_count_traffic_class_set(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint8_t traffic_class);
extern void of_action_ofdpa_oam_lm_tx_count_traffic_class_get(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint8_t *traffic_class);

extern void of_action_ofdpa_oam_lm_tx_count_lmep_id_set(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t lmep_id);
extern void of_action_ofdpa_oam_lm_tx_count_lmep_id_get(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t *lmep_id);

/* Unified accessor functions for of_action_ofdpa_pop_cw */

extern void of_action_ofdpa_pop_cw_experimenter_set(
    of_action_ofdpa_pop_cw_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_pop_cw_experimenter_get(
    of_action_ofdpa_pop_cw_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_pop_cw_subtype_set(
    of_action_ofdpa_pop_cw_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_pop_cw_subtype_get(
    of_action_ofdpa_pop_cw_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa_pop_l2hdr */

extern void of_action_ofdpa_pop_l2hdr_experimenter_set(
    of_action_ofdpa_pop_l2hdr_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_pop_l2hdr_experimenter_get(
    of_action_ofdpa_pop_l2hdr_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_pop_l2hdr_subtype_set(
    of_action_ofdpa_pop_l2hdr_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_pop_l2hdr_subtype_get(
    of_action_ofdpa_pop_l2hdr_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa_push_cw */

extern void of_action_ofdpa_push_cw_experimenter_set(
    of_action_ofdpa_push_cw_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_push_cw_experimenter_get(
    of_action_ofdpa_push_cw_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_push_cw_subtype_set(
    of_action_ofdpa_push_cw_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_push_cw_subtype_get(
    of_action_ofdpa_push_cw_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa_push_l2hdr */

extern void of_action_ofdpa_push_l2hdr_experimenter_set(
    of_action_ofdpa_push_l2hdr_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_push_l2hdr_experimenter_get(
    of_action_ofdpa_push_l2hdr_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_push_l2hdr_subtype_set(
    of_action_ofdpa_push_l2hdr_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_push_l2hdr_subtype_get(
    of_action_ofdpa_push_l2hdr_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_ofdpa_set_counter_fields */

extern void of_action_ofdpa_set_counter_fields_experimenter_set(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_set_counter_fields_experimenter_get(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_set_counter_fields_subtype_set(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_set_counter_fields_subtype_get(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_set_counter_fields_traffic_class_set(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint8_t traffic_class);
extern void of_action_ofdpa_set_counter_fields_traffic_class_get(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint8_t *traffic_class);

extern void of_action_ofdpa_set_counter_fields_lmep_id_set(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint32_t lmep_id);
extern void of_action_ofdpa_set_counter_fields_lmep_id_get(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint32_t *lmep_id);

/* Unified accessor functions for of_action_ofdpa_set_mpls_pcpdei_from_table */

extern void of_action_ofdpa_set_mpls_pcpdei_from_table_experimenter_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_set_mpls_pcpdei_from_table_experimenter_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_set_mpls_pcpdei_from_table_subtype_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_set_mpls_pcpdei_from_table_subtype_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_set_mpls_pcpdei_from_table_qos_index_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t qos_index);
extern void of_action_ofdpa_set_mpls_pcpdei_from_table_qos_index_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t *qos_index);

extern void of_action_ofdpa_set_mpls_pcpdei_from_table_traffic_class_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t traffic_class);
extern void of_action_ofdpa_set_mpls_pcpdei_from_table_traffic_class_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t *traffic_class);

extern void of_action_ofdpa_set_mpls_pcpdei_from_table_color_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t color);
extern void of_action_ofdpa_set_mpls_pcpdei_from_table_color_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table */

extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_experimenter_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_experimenter_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_subtype_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_subtype_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_qos_index_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t qos_index);
extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_qos_index_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t *qos_index);

extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_traffic_class_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t traffic_class);
extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_traffic_class_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t *traffic_class);

extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_color_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t color);
extern void of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_color_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_action_ofdpa_set_mpls_tc_from_tunnel_table */

extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_experimenter_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_experimenter_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_subtype_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_subtype_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_qos_index_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t qos_index);
extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_qos_index_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t *qos_index);

extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_traffic_class_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t traffic_class);
extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_traffic_class_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t *traffic_class);

extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_color_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t color);
extern void of_action_ofdpa_set_mpls_tc_from_tunnel_table_color_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_action_ofdpa_set_mpls_tc_from_vpn_table */

extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_experimenter_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_experimenter_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_subtype_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_subtype_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_qos_index_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t qos_index);
extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_qos_index_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t *qos_index);

extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_traffic_class_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t traffic_class);
extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_traffic_class_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t *traffic_class);

extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_color_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t color);
extern void of_action_ofdpa_set_mpls_tc_from_vpn_table_color_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_action_ofdpa_set_pcpdei_from_table */

extern void of_action_ofdpa_set_pcpdei_from_table_experimenter_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint32_t experimenter);
extern void of_action_ofdpa_set_pcpdei_from_table_experimenter_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint32_t *experimenter);

extern void of_action_ofdpa_set_pcpdei_from_table_subtype_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t subtype);
extern void of_action_ofdpa_set_pcpdei_from_table_subtype_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t *subtype);

extern void of_action_ofdpa_set_pcpdei_from_table_qos_index_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t qos_index);
extern void of_action_ofdpa_set_pcpdei_from_table_qos_index_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t *qos_index);

extern void of_action_ofdpa_set_pcpdei_from_table_traffic_class_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t traffic_class);
extern void of_action_ofdpa_set_pcpdei_from_table_traffic_class_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t *traffic_class);

extern void of_action_ofdpa_set_pcpdei_from_table_color_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t color);
extern void of_action_ofdpa_set_pcpdei_from_table_color_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_action_onf */

extern void of_action_onf_experimenter_set(
    of_action_onf_t *obj,
    uint32_t experimenter);
extern void of_action_onf_experimenter_get(
    of_action_onf_t *obj,
    uint32_t *experimenter);

extern void of_action_onf_subtype_set(
    of_action_onf_t *obj,
    uint16_t subtype);
extern void of_action_onf_subtype_get(
    of_action_onf_t *obj,
    uint16_t *subtype);

/* Unified accessor functions for of_action_onf_copy_field */

extern void of_action_onf_copy_field_experimenter_set(
    of_action_onf_copy_field_t *obj,
    uint32_t experimenter);
extern void of_action_onf_copy_field_experimenter_get(
    of_action_onf_copy_field_t *obj,
    uint32_t *experimenter);

extern void of_action_onf_copy_field_subtype_set(
    of_action_onf_copy_field_t *obj,
    uint16_t subtype);
extern void of_action_onf_copy_field_subtype_get(
    of_action_onf_copy_field_t *obj,
    uint16_t *subtype);

extern void of_action_onf_copy_field_n_bits_set(
    of_action_onf_copy_field_t *obj,
    uint16_t n_bits);
extern void of_action_onf_copy_field_n_bits_get(
    of_action_onf_copy_field_t *obj,
    uint16_t *n_bits);

extern void of_action_onf_copy_field_src_offset_set(
    of_action_onf_copy_field_t *obj,
    uint16_t src_offset);
extern void of_action_onf_copy_field_src_offset_get(
    of_action_onf_copy_field_t *obj,
    uint16_t *src_offset);

extern void of_action_onf_copy_field_dst_offset_set(
    of_action_onf_copy_field_t *obj,
    uint16_t dst_offset);
extern void of_action_onf_copy_field_dst_offset_get(
    of_action_onf_copy_field_t *obj,
    uint16_t *dst_offset);

/* Unified accessor functions for of_action_output */

extern void of_action_output_port_set(
    of_action_output_t *obj,
    of_port_no_t port);
extern void of_action_output_port_get(
    of_action_output_t *obj,
    of_port_no_t *port);

extern void of_action_output_max_len_set(
    of_action_output_t *obj,
    uint16_t max_len);
extern void of_action_output_max_len_get(
    of_action_output_t *obj,
    uint16_t *max_len);

/* Unified accessor functions for of_action_pop_mpls */

extern void of_action_pop_mpls_ethertype_set(
    of_action_pop_mpls_t *obj,
    uint16_t ethertype);
extern void of_action_pop_mpls_ethertype_get(
    of_action_pop_mpls_t *obj,
    uint16_t *ethertype);

/* Unified accessor functions for of_action_pop_pbb */

/* Unified accessor functions for of_action_pop_vlan */

/* Unified accessor functions for of_action_push_mpls */

extern void of_action_push_mpls_ethertype_set(
    of_action_push_mpls_t *obj,
    uint16_t ethertype);
extern void of_action_push_mpls_ethertype_get(
    of_action_push_mpls_t *obj,
    uint16_t *ethertype);

/* Unified accessor functions for of_action_push_pbb */

extern void of_action_push_pbb_ethertype_set(
    of_action_push_pbb_t *obj,
    uint16_t ethertype);
extern void of_action_push_pbb_ethertype_get(
    of_action_push_pbb_t *obj,
    uint16_t *ethertype);

/* Unified accessor functions for of_action_push_vlan */

extern void of_action_push_vlan_ethertype_set(
    of_action_push_vlan_t *obj,
    uint16_t ethertype);
extern void of_action_push_vlan_ethertype_get(
    of_action_push_vlan_t *obj,
    uint16_t *ethertype);

/* Unified accessor functions for of_action_set_dl_dst */

extern void of_action_set_dl_dst_dl_addr_set(
    of_action_set_dl_dst_t *obj,
    of_mac_addr_t dl_addr);
extern void of_action_set_dl_dst_dl_addr_get(
    of_action_set_dl_dst_t *obj,
    of_mac_addr_t *dl_addr);

/* Unified accessor functions for of_action_set_dl_src */

extern void of_action_set_dl_src_dl_addr_set(
    of_action_set_dl_src_t *obj,
    of_mac_addr_t dl_addr);
extern void of_action_set_dl_src_dl_addr_get(
    of_action_set_dl_src_t *obj,
    of_mac_addr_t *dl_addr);

/* Unified accessor functions for of_action_set_field */

extern int WARN_UNUSED_RESULT of_action_set_field_field_set(
    of_action_set_field_t *obj,
    of_octets_t *field);
extern void of_action_set_field_field_get(
    of_action_set_field_t *obj,
    of_octets_t *field);

/* Unified accessor functions for of_action_set_mpls_label */

extern void of_action_set_mpls_label_mpls_label_set(
    of_action_set_mpls_label_t *obj,
    uint32_t mpls_label);
extern void of_action_set_mpls_label_mpls_label_get(
    of_action_set_mpls_label_t *obj,
    uint32_t *mpls_label);

/* Unified accessor functions for of_action_set_mpls_tc */

extern void of_action_set_mpls_tc_mpls_tc_set(
    of_action_set_mpls_tc_t *obj,
    uint8_t mpls_tc);
extern void of_action_set_mpls_tc_mpls_tc_get(
    of_action_set_mpls_tc_t *obj,
    uint8_t *mpls_tc);

/* Unified accessor functions for of_action_set_mpls_ttl */

extern void of_action_set_mpls_ttl_mpls_ttl_set(
    of_action_set_mpls_ttl_t *obj,
    uint8_t mpls_ttl);
extern void of_action_set_mpls_ttl_mpls_ttl_get(
    of_action_set_mpls_ttl_t *obj,
    uint8_t *mpls_ttl);

/* Unified accessor functions for of_action_set_nw_dst */

extern void of_action_set_nw_dst_nw_addr_set(
    of_action_set_nw_dst_t *obj,
    uint32_t nw_addr);
extern void of_action_set_nw_dst_nw_addr_get(
    of_action_set_nw_dst_t *obj,
    uint32_t *nw_addr);

/* Unified accessor functions for of_action_set_nw_ecn */

extern void of_action_set_nw_ecn_nw_ecn_set(
    of_action_set_nw_ecn_t *obj,
    uint8_t nw_ecn);
extern void of_action_set_nw_ecn_nw_ecn_get(
    of_action_set_nw_ecn_t *obj,
    uint8_t *nw_ecn);

/* Unified accessor functions for of_action_set_nw_src */

extern void of_action_set_nw_src_nw_addr_set(
    of_action_set_nw_src_t *obj,
    uint32_t nw_addr);
extern void of_action_set_nw_src_nw_addr_get(
    of_action_set_nw_src_t *obj,
    uint32_t *nw_addr);

/* Unified accessor functions for of_action_set_nw_tos */

extern void of_action_set_nw_tos_nw_tos_set(
    of_action_set_nw_tos_t *obj,
    uint8_t nw_tos);
extern void of_action_set_nw_tos_nw_tos_get(
    of_action_set_nw_tos_t *obj,
    uint8_t *nw_tos);

/* Unified accessor functions for of_action_set_nw_ttl */

extern void of_action_set_nw_ttl_nw_ttl_set(
    of_action_set_nw_ttl_t *obj,
    uint8_t nw_ttl);
extern void of_action_set_nw_ttl_nw_ttl_get(
    of_action_set_nw_ttl_t *obj,
    uint8_t *nw_ttl);

/* Unified accessor functions for of_action_set_queue */

extern void of_action_set_queue_queue_id_set(
    of_action_set_queue_t *obj,
    uint32_t queue_id);
extern void of_action_set_queue_queue_id_get(
    of_action_set_queue_t *obj,
    uint32_t *queue_id);

/* Unified accessor functions for of_action_set_tp_dst */

extern void of_action_set_tp_dst_tp_port_set(
    of_action_set_tp_dst_t *obj,
    uint16_t tp_port);
extern void of_action_set_tp_dst_tp_port_get(
    of_action_set_tp_dst_t *obj,
    uint16_t *tp_port);

/* Unified accessor functions for of_action_set_tp_src */

extern void of_action_set_tp_src_tp_port_set(
    of_action_set_tp_src_t *obj,
    uint16_t tp_port);
extern void of_action_set_tp_src_tp_port_get(
    of_action_set_tp_src_t *obj,
    uint16_t *tp_port);

/* Unified accessor functions for of_action_set_vlan_pcp */

extern void of_action_set_vlan_pcp_vlan_pcp_set(
    of_action_set_vlan_pcp_t *obj,
    uint8_t vlan_pcp);
extern void of_action_set_vlan_pcp_vlan_pcp_get(
    of_action_set_vlan_pcp_t *obj,
    uint8_t *vlan_pcp);

/* Unified accessor functions for of_action_set_vlan_vid */

extern void of_action_set_vlan_vid_vlan_vid_set(
    of_action_set_vlan_vid_t *obj,
    uint16_t vlan_vid);
extern void of_action_set_vlan_vid_vlan_vid_get(
    of_action_set_vlan_vid_t *obj,
    uint16_t *vlan_vid);

/* Unified accessor functions for of_action_strip_vlan */

/* Unified accessor functions for of_bsn_controller_connection */

extern void of_bsn_controller_connection_state_set(
    of_bsn_controller_connection_t *obj,
    uint8_t state);
extern void of_bsn_controller_connection_state_get(
    of_bsn_controller_connection_t *obj,
    uint8_t *state);

extern void of_bsn_controller_connection_auxiliary_id_set(
    of_bsn_controller_connection_t *obj,
    uint8_t auxiliary_id);
extern void of_bsn_controller_connection_auxiliary_id_get(
    of_bsn_controller_connection_t *obj,
    uint8_t *auxiliary_id);

extern void of_bsn_controller_connection_role_set(
    of_bsn_controller_connection_t *obj,
    uint32_t role);
extern void of_bsn_controller_connection_role_get(
    of_bsn_controller_connection_t *obj,
    uint32_t *role);

extern void of_bsn_controller_connection_uri_set(
    of_bsn_controller_connection_t *obj,
    of_desc_str_t uri);
extern void of_bsn_controller_connection_uri_get(
    of_bsn_controller_connection_t *obj,
    of_desc_str_t *uri);

/* Unified accessor functions for of_bsn_debug_counter_desc_stats_entry */

extern void of_bsn_debug_counter_desc_stats_entry_counter_id_set(
    of_bsn_debug_counter_desc_stats_entry_t *obj,
    uint64_t counter_id);
extern void of_bsn_debug_counter_desc_stats_entry_counter_id_get(
    of_bsn_debug_counter_desc_stats_entry_t *obj,
    uint64_t *counter_id);

extern void of_bsn_debug_counter_desc_stats_entry_name_set(
    of_bsn_debug_counter_desc_stats_entry_t *obj,
    of_str64_t name);
extern void of_bsn_debug_counter_desc_stats_entry_name_get(
    of_bsn_debug_counter_desc_stats_entry_t *obj,
    of_str64_t *name);

extern void of_bsn_debug_counter_desc_stats_entry_description_set(
    of_bsn_debug_counter_desc_stats_entry_t *obj,
    of_desc_str_t description);
extern void of_bsn_debug_counter_desc_stats_entry_description_get(
    of_bsn_debug_counter_desc_stats_entry_t *obj,
    of_desc_str_t *description);

/* Unified accessor functions for of_bsn_debug_counter_stats_entry */

extern void of_bsn_debug_counter_stats_entry_counter_id_set(
    of_bsn_debug_counter_stats_entry_t *obj,
    uint64_t counter_id);
extern void of_bsn_debug_counter_stats_entry_counter_id_get(
    of_bsn_debug_counter_stats_entry_t *obj,
    uint64_t *counter_id);

extern void of_bsn_debug_counter_stats_entry_value_set(
    of_bsn_debug_counter_stats_entry_t *obj,
    uint64_t value);
extern void of_bsn_debug_counter_stats_entry_value_get(
    of_bsn_debug_counter_stats_entry_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_bsn_flow_checksum_bucket_stats_entry */

extern void of_bsn_flow_checksum_bucket_stats_entry_checksum_set(
    of_bsn_flow_checksum_bucket_stats_entry_t *obj,
    uint64_t checksum);
extern void of_bsn_flow_checksum_bucket_stats_entry_checksum_get(
    of_bsn_flow_checksum_bucket_stats_entry_t *obj,
    uint64_t *checksum);

/* Unified accessor functions for of_bsn_gentable_bucket_stats_entry */

extern void of_bsn_gentable_bucket_stats_entry_checksum_set(
    of_bsn_gentable_bucket_stats_entry_t *obj,
    of_checksum_128_t checksum);
extern void of_bsn_gentable_bucket_stats_entry_checksum_get(
    of_bsn_gentable_bucket_stats_entry_t *obj,
    of_checksum_128_t *checksum);

/* Unified accessor functions for of_bsn_gentable_desc_stats_entry */

extern void of_bsn_gentable_desc_stats_entry_table_id_set(
    of_bsn_gentable_desc_stats_entry_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_desc_stats_entry_table_id_get(
    of_bsn_gentable_desc_stats_entry_t *obj,
    uint16_t *table_id);

extern void of_bsn_gentable_desc_stats_entry_name_set(
    of_bsn_gentable_desc_stats_entry_t *obj,
    of_table_name_t name);
extern void of_bsn_gentable_desc_stats_entry_name_get(
    of_bsn_gentable_desc_stats_entry_t *obj,
    of_table_name_t *name);

extern void of_bsn_gentable_desc_stats_entry_buckets_size_set(
    of_bsn_gentable_desc_stats_entry_t *obj,
    uint32_t buckets_size);
extern void of_bsn_gentable_desc_stats_entry_buckets_size_get(
    of_bsn_gentable_desc_stats_entry_t *obj,
    uint32_t *buckets_size);

extern void of_bsn_gentable_desc_stats_entry_max_entries_set(
    of_bsn_gentable_desc_stats_entry_t *obj,
    uint32_t max_entries);
extern void of_bsn_gentable_desc_stats_entry_max_entries_get(
    of_bsn_gentable_desc_stats_entry_t *obj,
    uint32_t *max_entries);

/* Unified accessor functions for of_bsn_gentable_entry_desc_stats_entry */

extern void of_bsn_gentable_entry_desc_stats_entry_checksum_set(
    of_bsn_gentable_entry_desc_stats_entry_t *obj,
    of_checksum_128_t checksum);
extern void of_bsn_gentable_entry_desc_stats_entry_checksum_get(
    of_bsn_gentable_entry_desc_stats_entry_t *obj,
    of_checksum_128_t *checksum);

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_desc_stats_entry_key_set(
    of_bsn_gentable_entry_desc_stats_entry_t *obj,
    of_list_bsn_tlv_t *key);
extern void of_bsn_gentable_entry_desc_stats_entry_key_bind(
    of_bsn_gentable_entry_desc_stats_entry_t *obj,
    of_list_bsn_tlv_t *key);
extern of_list_bsn_tlv_t *of_bsn_gentable_entry_desc_stats_entry_key_get(
    of_bsn_gentable_entry_desc_stats_entry_t *obj);

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_desc_stats_entry_value_set(
    of_bsn_gentable_entry_desc_stats_entry_t *obj,
    of_list_bsn_tlv_t *value);
extern void of_bsn_gentable_entry_desc_stats_entry_value_bind(
    of_bsn_gentable_entry_desc_stats_entry_t *obj,
    of_list_bsn_tlv_t *value);
extern of_list_bsn_tlv_t *of_bsn_gentable_entry_desc_stats_entry_value_get(
    of_bsn_gentable_entry_desc_stats_entry_t *obj);

/* Unified accessor functions for of_bsn_gentable_entry_stats_entry */

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_stats_entry_key_set(
    of_bsn_gentable_entry_stats_entry_t *obj,
    of_list_bsn_tlv_t *key);
extern void of_bsn_gentable_entry_stats_entry_key_bind(
    of_bsn_gentable_entry_stats_entry_t *obj,
    of_list_bsn_tlv_t *key);
extern of_list_bsn_tlv_t *of_bsn_gentable_entry_stats_entry_key_get(
    of_bsn_gentable_entry_stats_entry_t *obj);

extern int WARN_UNUSED_RESULT of_bsn_gentable_entry_stats_entry_stats_set(
    of_bsn_gentable_entry_stats_entry_t *obj,
    of_list_bsn_tlv_t *stats);
extern void of_bsn_gentable_entry_stats_entry_stats_bind(
    of_bsn_gentable_entry_stats_entry_t *obj,
    of_list_bsn_tlv_t *stats);
extern of_list_bsn_tlv_t *of_bsn_gentable_entry_stats_entry_stats_get(
    of_bsn_gentable_entry_stats_entry_t *obj);

/* Unified accessor functions for of_bsn_gentable_stats_entry */

extern void of_bsn_gentable_stats_entry_table_id_set(
    of_bsn_gentable_stats_entry_t *obj,
    uint16_t table_id);
extern void of_bsn_gentable_stats_entry_table_id_get(
    of_bsn_gentable_stats_entry_t *obj,
    uint16_t *table_id);

extern void of_bsn_gentable_stats_entry_entry_count_set(
    of_bsn_gentable_stats_entry_t *obj,
    uint32_t entry_count);
extern void of_bsn_gentable_stats_entry_entry_count_get(
    of_bsn_gentable_stats_entry_t *obj,
    uint32_t *entry_count);

extern void of_bsn_gentable_stats_entry_checksum_set(
    of_bsn_gentable_stats_entry_t *obj,
    of_checksum_128_t checksum);
extern void of_bsn_gentable_stats_entry_checksum_get(
    of_bsn_gentable_stats_entry_t *obj,
    of_checksum_128_t *checksum);

/* Unified accessor functions for of_bsn_interface */

extern void of_bsn_interface_hw_addr_set(
    of_bsn_interface_t *obj,
    of_mac_addr_t hw_addr);
extern void of_bsn_interface_hw_addr_get(
    of_bsn_interface_t *obj,
    of_mac_addr_t *hw_addr);

extern void of_bsn_interface_name_set(
    of_bsn_interface_t *obj,
    of_port_name_t name);
extern void of_bsn_interface_name_get(
    of_bsn_interface_t *obj,
    of_port_name_t *name);

extern void of_bsn_interface_ipv4_addr_set(
    of_bsn_interface_t *obj,
    of_ipv4_t ipv4_addr);
extern void of_bsn_interface_ipv4_addr_get(
    of_bsn_interface_t *obj,
    of_ipv4_t *ipv4_addr);

extern void of_bsn_interface_ipv4_netmask_set(
    of_bsn_interface_t *obj,
    of_ipv4_t ipv4_netmask);
extern void of_bsn_interface_ipv4_netmask_get(
    of_bsn_interface_t *obj,
    of_ipv4_t *ipv4_netmask);

/* Unified accessor functions for of_bsn_lacp_stats_entry */

extern void of_bsn_lacp_stats_entry_port_no_set(
    of_bsn_lacp_stats_entry_t *obj,
    of_port_no_t port_no);
extern void of_bsn_lacp_stats_entry_port_no_get(
    of_bsn_lacp_stats_entry_t *obj,
    of_port_no_t *port_no);

extern void of_bsn_lacp_stats_entry_actor_sys_priority_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t actor_sys_priority);
extern void of_bsn_lacp_stats_entry_actor_sys_priority_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t *actor_sys_priority);

extern void of_bsn_lacp_stats_entry_actor_sys_mac_set(
    of_bsn_lacp_stats_entry_t *obj,
    of_mac_addr_t actor_sys_mac);
extern void of_bsn_lacp_stats_entry_actor_sys_mac_get(
    of_bsn_lacp_stats_entry_t *obj,
    of_mac_addr_t *actor_sys_mac);

extern void of_bsn_lacp_stats_entry_actor_port_priority_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t actor_port_priority);
extern void of_bsn_lacp_stats_entry_actor_port_priority_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t *actor_port_priority);

extern void of_bsn_lacp_stats_entry_actor_port_num_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t actor_port_num);
extern void of_bsn_lacp_stats_entry_actor_port_num_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t *actor_port_num);

extern void of_bsn_lacp_stats_entry_actor_key_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t actor_key);
extern void of_bsn_lacp_stats_entry_actor_key_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t *actor_key);

extern void of_bsn_lacp_stats_entry_convergence_status_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint8_t convergence_status);
extern void of_bsn_lacp_stats_entry_convergence_status_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint8_t *convergence_status);

extern void of_bsn_lacp_stats_entry_partner_sys_priority_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t partner_sys_priority);
extern void of_bsn_lacp_stats_entry_partner_sys_priority_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t *partner_sys_priority);

extern void of_bsn_lacp_stats_entry_partner_sys_mac_set(
    of_bsn_lacp_stats_entry_t *obj,
    of_mac_addr_t partner_sys_mac);
extern void of_bsn_lacp_stats_entry_partner_sys_mac_get(
    of_bsn_lacp_stats_entry_t *obj,
    of_mac_addr_t *partner_sys_mac);

extern void of_bsn_lacp_stats_entry_partner_port_priority_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t partner_port_priority);
extern void of_bsn_lacp_stats_entry_partner_port_priority_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t *partner_port_priority);

extern void of_bsn_lacp_stats_entry_partner_port_num_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t partner_port_num);
extern void of_bsn_lacp_stats_entry_partner_port_num_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t *partner_port_num);

extern void of_bsn_lacp_stats_entry_partner_key_set(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t partner_key);
extern void of_bsn_lacp_stats_entry_partner_key_get(
    of_bsn_lacp_stats_entry_t *obj,
    uint16_t *partner_key);

/* Unified accessor functions for of_bsn_port_counter_stats_entry */

extern void of_bsn_port_counter_stats_entry_port_no_set(
    of_bsn_port_counter_stats_entry_t *obj,
    of_port_no_t port_no);
extern void of_bsn_port_counter_stats_entry_port_no_get(
    of_bsn_port_counter_stats_entry_t *obj,
    of_port_no_t *port_no);

extern int WARN_UNUSED_RESULT of_bsn_port_counter_stats_entry_values_set(
    of_bsn_port_counter_stats_entry_t *obj,
    of_list_uint64_t *values);
extern void of_bsn_port_counter_stats_entry_values_bind(
    of_bsn_port_counter_stats_entry_t *obj,
    of_list_uint64_t *values);
extern of_list_uint64_t *of_bsn_port_counter_stats_entry_values_get(
    of_bsn_port_counter_stats_entry_t *obj);

/* Unified accessor functions for of_bsn_switch_pipeline_stats_entry */

extern void of_bsn_switch_pipeline_stats_entry_pipeline_set(
    of_bsn_switch_pipeline_stats_entry_t *obj,
    of_desc_str_t pipeline);
extern void of_bsn_switch_pipeline_stats_entry_pipeline_get(
    of_bsn_switch_pipeline_stats_entry_t *obj,
    of_desc_str_t *pipeline);

/* Unified accessor functions for of_bsn_table_checksum_stats_entry */

extern void of_bsn_table_checksum_stats_entry_table_id_set(
    of_bsn_table_checksum_stats_entry_t *obj,
    uint8_t table_id);
extern void of_bsn_table_checksum_stats_entry_table_id_get(
    of_bsn_table_checksum_stats_entry_t *obj,
    uint8_t *table_id);

extern void of_bsn_table_checksum_stats_entry_checksum_set(
    of_bsn_table_checksum_stats_entry_t *obj,
    uint64_t checksum);
extern void of_bsn_table_checksum_stats_entry_checksum_get(
    of_bsn_table_checksum_stats_entry_t *obj,
    uint64_t *checksum);

/* Unified accessor functions for of_bsn_tlv_broadcast_query_timeout */

extern void of_bsn_tlv_broadcast_query_timeout_value_set(
    of_bsn_tlv_broadcast_query_timeout_t *obj,
    uint32_t value);
extern void of_bsn_tlv_broadcast_query_timeout_value_get(
    of_bsn_tlv_broadcast_query_timeout_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_bsn_tlv_circuit_id */

extern int WARN_UNUSED_RESULT of_bsn_tlv_circuit_id_value_set(
    of_bsn_tlv_circuit_id_t *obj,
    of_octets_t *value);
extern void of_bsn_tlv_circuit_id_value_get(
    of_bsn_tlv_circuit_id_t *obj,
    of_octets_t *value);

/* Unified accessor functions for of_bsn_tlv_header */

/* Unified accessor functions for of_bsn_tlv_idle_notification */

/* Unified accessor functions for of_bsn_tlv_idle_time */

extern void of_bsn_tlv_idle_time_value_set(
    of_bsn_tlv_idle_time_t *obj,
    uint64_t value);
extern void of_bsn_tlv_idle_time_value_get(
    of_bsn_tlv_idle_time_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_bsn_tlv_idle_timeout */

extern void of_bsn_tlv_idle_timeout_value_set(
    of_bsn_tlv_idle_timeout_t *obj,
    uint32_t value);
extern void of_bsn_tlv_idle_timeout_value_get(
    of_bsn_tlv_idle_timeout_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_bsn_tlv_ipv4 */

extern void of_bsn_tlv_ipv4_value_set(
    of_bsn_tlv_ipv4_t *obj,
    of_ipv4_t value);
extern void of_bsn_tlv_ipv4_value_get(
    of_bsn_tlv_ipv4_t *obj,
    of_ipv4_t *value);

/* Unified accessor functions for of_bsn_tlv_mac */

extern void of_bsn_tlv_mac_value_set(
    of_bsn_tlv_mac_t *obj,
    of_mac_addr_t value);
extern void of_bsn_tlv_mac_value_get(
    of_bsn_tlv_mac_t *obj,
    of_mac_addr_t *value);

/* Unified accessor functions for of_bsn_tlv_miss_packets */

extern void of_bsn_tlv_miss_packets_value_set(
    of_bsn_tlv_miss_packets_t *obj,
    uint64_t value);
extern void of_bsn_tlv_miss_packets_value_get(
    of_bsn_tlv_miss_packets_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_bsn_tlv_port */

extern void of_bsn_tlv_port_value_set(
    of_bsn_tlv_port_t *obj,
    of_port_no_t value);
extern void of_bsn_tlv_port_value_get(
    of_bsn_tlv_port_t *obj,
    of_port_no_t *value);

/* Unified accessor functions for of_bsn_tlv_reply_packets */

extern void of_bsn_tlv_reply_packets_value_set(
    of_bsn_tlv_reply_packets_t *obj,
    uint64_t value);
extern void of_bsn_tlv_reply_packets_value_get(
    of_bsn_tlv_reply_packets_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_bsn_tlv_request_packets */

extern void of_bsn_tlv_request_packets_value_set(
    of_bsn_tlv_request_packets_t *obj,
    uint64_t value);
extern void of_bsn_tlv_request_packets_value_get(
    of_bsn_tlv_request_packets_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_bsn_tlv_rx_packets */

extern void of_bsn_tlv_rx_packets_value_set(
    of_bsn_tlv_rx_packets_t *obj,
    uint64_t value);
extern void of_bsn_tlv_rx_packets_value_get(
    of_bsn_tlv_rx_packets_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_bsn_tlv_tx_packets */

extern void of_bsn_tlv_tx_packets_value_set(
    of_bsn_tlv_tx_packets_t *obj,
    uint64_t value);
extern void of_bsn_tlv_tx_packets_value_get(
    of_bsn_tlv_tx_packets_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_bsn_tlv_udf_anchor */

extern void of_bsn_tlv_udf_anchor_value_set(
    of_bsn_tlv_udf_anchor_t *obj,
    uint16_t value);
extern void of_bsn_tlv_udf_anchor_value_get(
    of_bsn_tlv_udf_anchor_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_bsn_tlv_udf_id */

extern void of_bsn_tlv_udf_id_value_set(
    of_bsn_tlv_udf_id_t *obj,
    uint16_t value);
extern void of_bsn_tlv_udf_id_value_get(
    of_bsn_tlv_udf_id_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_bsn_tlv_udf_length */

extern void of_bsn_tlv_udf_length_value_set(
    of_bsn_tlv_udf_length_t *obj,
    uint16_t value);
extern void of_bsn_tlv_udf_length_value_get(
    of_bsn_tlv_udf_length_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_bsn_tlv_udf_offset */

extern void of_bsn_tlv_udf_offset_value_set(
    of_bsn_tlv_udf_offset_t *obj,
    uint16_t value);
extern void of_bsn_tlv_udf_offset_value_get(
    of_bsn_tlv_udf_offset_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_bsn_tlv_unicast_query_timeout */

extern void of_bsn_tlv_unicast_query_timeout_value_set(
    of_bsn_tlv_unicast_query_timeout_t *obj,
    uint32_t value);
extern void of_bsn_tlv_unicast_query_timeout_value_get(
    of_bsn_tlv_unicast_query_timeout_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_bsn_tlv_vlan_vid */

extern void of_bsn_tlv_vlan_vid_value_set(
    of_bsn_tlv_vlan_vid_t *obj,
    uint16_t value);
extern void of_bsn_tlv_vlan_vid_value_get(
    of_bsn_tlv_vlan_vid_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_bsn_vlan_counter_stats_entry */

extern void of_bsn_vlan_counter_stats_entry_vlan_vid_set(
    of_bsn_vlan_counter_stats_entry_t *obj,
    uint16_t vlan_vid);
extern void of_bsn_vlan_counter_stats_entry_vlan_vid_get(
    of_bsn_vlan_counter_stats_entry_t *obj,
    uint16_t *vlan_vid);

extern int WARN_UNUSED_RESULT of_bsn_vlan_counter_stats_entry_values_set(
    of_bsn_vlan_counter_stats_entry_t *obj,
    of_list_uint64_t *values);
extern void of_bsn_vlan_counter_stats_entry_values_bind(
    of_bsn_vlan_counter_stats_entry_t *obj,
    of_list_uint64_t *values);
extern of_list_uint64_t *of_bsn_vlan_counter_stats_entry_values_get(
    of_bsn_vlan_counter_stats_entry_t *obj);

/* Unified accessor functions for of_bsn_vport_header */

/* Unified accessor functions for of_bsn_vport_q_in_q */

extern void of_bsn_vport_q_in_q_port_no_set(
    of_bsn_vport_q_in_q_t *obj,
    uint32_t port_no);
extern void of_bsn_vport_q_in_q_port_no_get(
    of_bsn_vport_q_in_q_t *obj,
    uint32_t *port_no);

extern void of_bsn_vport_q_in_q_ingress_tpid_set(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t ingress_tpid);
extern void of_bsn_vport_q_in_q_ingress_tpid_get(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t *ingress_tpid);

extern void of_bsn_vport_q_in_q_ingress_vlan_id_set(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t ingress_vlan_id);
extern void of_bsn_vport_q_in_q_ingress_vlan_id_get(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t *ingress_vlan_id);

extern void of_bsn_vport_q_in_q_egress_tpid_set(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t egress_tpid);
extern void of_bsn_vport_q_in_q_egress_tpid_get(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t *egress_tpid);

extern void of_bsn_vport_q_in_q_egress_vlan_id_set(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t egress_vlan_id);
extern void of_bsn_vport_q_in_q_egress_vlan_id_get(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t *egress_vlan_id);

extern void of_bsn_vport_q_in_q_if_name_set(
    of_bsn_vport_q_in_q_t *obj,
    of_port_name_t if_name);
extern void of_bsn_vport_q_in_q_if_name_get(
    of_bsn_vport_q_in_q_t *obj,
    of_port_name_t *if_name);

/* Unified accessor functions for of_bucket */

extern void of_bucket_weight_set(
    of_bucket_t *obj,
    uint16_t weight);
extern void of_bucket_weight_get(
    of_bucket_t *obj,
    uint16_t *weight);

extern void of_bucket_watch_port_set(
    of_bucket_t *obj,
    of_port_no_t watch_port);
extern void of_bucket_watch_port_get(
    of_bucket_t *obj,
    of_port_no_t *watch_port);

extern void of_bucket_watch_group_set(
    of_bucket_t *obj,
    uint32_t watch_group);
extern void of_bucket_watch_group_get(
    of_bucket_t *obj,
    uint32_t *watch_group);

extern int WARN_UNUSED_RESULT of_bucket_actions_set(
    of_bucket_t *obj,
    of_list_action_t *actions);
extern void of_bucket_actions_bind(
    of_bucket_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_bucket_actions_get(
    of_bucket_t *obj);

/* Unified accessor functions for of_bucket_counter */

extern void of_bucket_counter_packet_count_set(
    of_bucket_counter_t *obj,
    uint64_t packet_count);
extern void of_bucket_counter_packet_count_get(
    of_bucket_counter_t *obj,
    uint64_t *packet_count);

extern void of_bucket_counter_byte_count_set(
    of_bucket_counter_t *obj,
    uint64_t byte_count);
extern void of_bucket_counter_byte_count_get(
    of_bucket_counter_t *obj,
    uint64_t *byte_count);

/* Unified accessor functions for of_flow_stats_entry */

extern void of_flow_stats_entry_table_id_set(
    of_flow_stats_entry_t *obj,
    uint8_t table_id);
extern void of_flow_stats_entry_table_id_get(
    of_flow_stats_entry_t *obj,
    uint8_t *table_id);

extern void of_flow_stats_entry_duration_sec_set(
    of_flow_stats_entry_t *obj,
    uint32_t duration_sec);
extern void of_flow_stats_entry_duration_sec_get(
    of_flow_stats_entry_t *obj,
    uint32_t *duration_sec);

extern void of_flow_stats_entry_duration_nsec_set(
    of_flow_stats_entry_t *obj,
    uint32_t duration_nsec);
extern void of_flow_stats_entry_duration_nsec_get(
    of_flow_stats_entry_t *obj,
    uint32_t *duration_nsec);

extern void of_flow_stats_entry_priority_set(
    of_flow_stats_entry_t *obj,
    uint16_t priority);
extern void of_flow_stats_entry_priority_get(
    of_flow_stats_entry_t *obj,
    uint16_t *priority);

extern void of_flow_stats_entry_idle_timeout_set(
    of_flow_stats_entry_t *obj,
    uint16_t idle_timeout);
extern void of_flow_stats_entry_idle_timeout_get(
    of_flow_stats_entry_t *obj,
    uint16_t *idle_timeout);

extern void of_flow_stats_entry_hard_timeout_set(
    of_flow_stats_entry_t *obj,
    uint16_t hard_timeout);
extern void of_flow_stats_entry_hard_timeout_get(
    of_flow_stats_entry_t *obj,
    uint16_t *hard_timeout);

extern void of_flow_stats_entry_flags_set(
    of_flow_stats_entry_t *obj,
    uint16_t flags);
extern void of_flow_stats_entry_flags_get(
    of_flow_stats_entry_t *obj,
    uint16_t *flags);

extern void of_flow_stats_entry_cookie_set(
    of_flow_stats_entry_t *obj,
    uint64_t cookie);
extern void of_flow_stats_entry_cookie_get(
    of_flow_stats_entry_t *obj,
    uint64_t *cookie);

extern void of_flow_stats_entry_packet_count_set(
    of_flow_stats_entry_t *obj,
    uint64_t packet_count);
extern void of_flow_stats_entry_packet_count_get(
    of_flow_stats_entry_t *obj,
    uint64_t *packet_count);

extern void of_flow_stats_entry_byte_count_set(
    of_flow_stats_entry_t *obj,
    uint64_t byte_count);
extern void of_flow_stats_entry_byte_count_get(
    of_flow_stats_entry_t *obj,
    uint64_t *byte_count);

extern int WARN_UNUSED_RESULT of_flow_stats_entry_match_set(
    of_flow_stats_entry_t *obj,
    of_match_t *match);
extern int WARN_UNUSED_RESULT of_flow_stats_entry_match_get(
    of_flow_stats_entry_t *obj,
    of_match_t *match);

extern int WARN_UNUSED_RESULT of_flow_stats_entry_instructions_set(
    of_flow_stats_entry_t *obj,
    of_list_instruction_t *instructions);
extern void of_flow_stats_entry_instructions_bind(
    of_flow_stats_entry_t *obj,
    of_list_instruction_t *instructions);
extern of_list_instruction_t *of_flow_stats_entry_instructions_get(
    of_flow_stats_entry_t *obj);

extern int WARN_UNUSED_RESULT of_flow_stats_entry_actions_set(
    of_flow_stats_entry_t *obj,
    of_list_action_t *actions);
extern void of_flow_stats_entry_actions_bind(
    of_flow_stats_entry_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_flow_stats_entry_actions_get(
    of_flow_stats_entry_t *obj);

/* Unified accessor functions for of_group_desc_stats_entry */

extern void of_group_desc_stats_entry_group_type_set(
    of_group_desc_stats_entry_t *obj,
    uint8_t group_type);
extern void of_group_desc_stats_entry_group_type_get(
    of_group_desc_stats_entry_t *obj,
    uint8_t *group_type);

extern void of_group_desc_stats_entry_group_id_set(
    of_group_desc_stats_entry_t *obj,
    uint32_t group_id);
extern void of_group_desc_stats_entry_group_id_get(
    of_group_desc_stats_entry_t *obj,
    uint32_t *group_id);

extern int WARN_UNUSED_RESULT of_group_desc_stats_entry_buckets_set(
    of_group_desc_stats_entry_t *obj,
    of_list_bucket_t *buckets);
extern void of_group_desc_stats_entry_buckets_bind(
    of_group_desc_stats_entry_t *obj,
    of_list_bucket_t *buckets);
extern of_list_bucket_t *of_group_desc_stats_entry_buckets_get(
    of_group_desc_stats_entry_t *obj);

/* Unified accessor functions for of_group_stats_entry */

extern void of_group_stats_entry_group_id_set(
    of_group_stats_entry_t *obj,
    uint32_t group_id);
extern void of_group_stats_entry_group_id_get(
    of_group_stats_entry_t *obj,
    uint32_t *group_id);

extern void of_group_stats_entry_ref_count_set(
    of_group_stats_entry_t *obj,
    uint32_t ref_count);
extern void of_group_stats_entry_ref_count_get(
    of_group_stats_entry_t *obj,
    uint32_t *ref_count);

extern void of_group_stats_entry_packet_count_set(
    of_group_stats_entry_t *obj,
    uint64_t packet_count);
extern void of_group_stats_entry_packet_count_get(
    of_group_stats_entry_t *obj,
    uint64_t *packet_count);

extern void of_group_stats_entry_byte_count_set(
    of_group_stats_entry_t *obj,
    uint64_t byte_count);
extern void of_group_stats_entry_byte_count_get(
    of_group_stats_entry_t *obj,
    uint64_t *byte_count);

extern void of_group_stats_entry_duration_sec_set(
    of_group_stats_entry_t *obj,
    uint32_t duration_sec);
extern void of_group_stats_entry_duration_sec_get(
    of_group_stats_entry_t *obj,
    uint32_t *duration_sec);

extern void of_group_stats_entry_duration_nsec_set(
    of_group_stats_entry_t *obj,
    uint32_t duration_nsec);
extern void of_group_stats_entry_duration_nsec_get(
    of_group_stats_entry_t *obj,
    uint32_t *duration_nsec);

extern int WARN_UNUSED_RESULT of_group_stats_entry_bucket_stats_set(
    of_group_stats_entry_t *obj,
    of_list_bucket_counter_t *bucket_stats);
extern void of_group_stats_entry_bucket_stats_bind(
    of_group_stats_entry_t *obj,
    of_list_bucket_counter_t *bucket_stats);
extern of_list_bucket_counter_t *of_group_stats_entry_bucket_stats_get(
    of_group_stats_entry_t *obj);

/* Unified accessor functions for of_header */

extern void of_header_xid_set(
    of_header_t *obj,
    uint32_t xid);
extern void of_header_xid_get(
    of_header_t *obj,
    uint32_t *xid);

/* Unified accessor functions for of_hello_elem_header */

/* Unified accessor functions for of_hello_elem_versionbitmap */

extern int WARN_UNUSED_RESULT of_hello_elem_versionbitmap_bitmaps_set(
    of_hello_elem_versionbitmap_t *obj,
    of_list_uint32_t *bitmaps);
extern void of_hello_elem_versionbitmap_bitmaps_bind(
    of_hello_elem_versionbitmap_t *obj,
    of_list_uint32_t *bitmaps);
extern of_list_uint32_t *of_hello_elem_versionbitmap_bitmaps_get(
    of_hello_elem_versionbitmap_t *obj);

/* Unified accessor functions for of_instruction_apply_actions */

extern int WARN_UNUSED_RESULT of_instruction_apply_actions_actions_set(
    of_instruction_apply_actions_t *obj,
    of_list_action_t *actions);
extern void of_instruction_apply_actions_actions_bind(
    of_instruction_apply_actions_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_instruction_apply_actions_actions_get(
    of_instruction_apply_actions_t *obj);

/* Unified accessor functions for of_instruction_bsn */

extern void of_instruction_bsn_experimenter_set(
    of_instruction_bsn_t *obj,
    uint32_t experimenter);
extern void of_instruction_bsn_experimenter_get(
    of_instruction_bsn_t *obj,
    uint32_t *experimenter);

extern void of_instruction_bsn_subtype_set(
    of_instruction_bsn_t *obj,
    uint32_t subtype);
extern void of_instruction_bsn_subtype_get(
    of_instruction_bsn_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_bsn_arp_offload */

extern void of_instruction_bsn_arp_offload_experimenter_set(
    of_instruction_bsn_arp_offload_t *obj,
    uint32_t experimenter);
extern void of_instruction_bsn_arp_offload_experimenter_get(
    of_instruction_bsn_arp_offload_t *obj,
    uint32_t *experimenter);

extern void of_instruction_bsn_arp_offload_subtype_set(
    of_instruction_bsn_arp_offload_t *obj,
    uint32_t subtype);
extern void of_instruction_bsn_arp_offload_subtype_get(
    of_instruction_bsn_arp_offload_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_bsn_deny */

extern void of_instruction_bsn_deny_experimenter_set(
    of_instruction_bsn_deny_t *obj,
    uint32_t experimenter);
extern void of_instruction_bsn_deny_experimenter_get(
    of_instruction_bsn_deny_t *obj,
    uint32_t *experimenter);

extern void of_instruction_bsn_deny_subtype_set(
    of_instruction_bsn_deny_t *obj,
    uint32_t subtype);
extern void of_instruction_bsn_deny_subtype_get(
    of_instruction_bsn_deny_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_bsn_dhcp_offload */

extern void of_instruction_bsn_dhcp_offload_experimenter_set(
    of_instruction_bsn_dhcp_offload_t *obj,
    uint32_t experimenter);
extern void of_instruction_bsn_dhcp_offload_experimenter_get(
    of_instruction_bsn_dhcp_offload_t *obj,
    uint32_t *experimenter);

extern void of_instruction_bsn_dhcp_offload_subtype_set(
    of_instruction_bsn_dhcp_offload_t *obj,
    uint32_t subtype);
extern void of_instruction_bsn_dhcp_offload_subtype_get(
    of_instruction_bsn_dhcp_offload_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_bsn_disable_split_horizon_check */

extern void of_instruction_bsn_disable_split_horizon_check_experimenter_set(
    of_instruction_bsn_disable_split_horizon_check_t *obj,
    uint32_t experimenter);
extern void of_instruction_bsn_disable_split_horizon_check_experimenter_get(
    of_instruction_bsn_disable_split_horizon_check_t *obj,
    uint32_t *experimenter);

extern void of_instruction_bsn_disable_split_horizon_check_subtype_set(
    of_instruction_bsn_disable_split_horizon_check_t *obj,
    uint32_t subtype);
extern void of_instruction_bsn_disable_split_horizon_check_subtype_get(
    of_instruction_bsn_disable_split_horizon_check_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_bsn_disable_src_mac_check */

extern void of_instruction_bsn_disable_src_mac_check_experimenter_set(
    of_instruction_bsn_disable_src_mac_check_t *obj,
    uint32_t experimenter);
extern void of_instruction_bsn_disable_src_mac_check_experimenter_get(
    of_instruction_bsn_disable_src_mac_check_t *obj,
    uint32_t *experimenter);

extern void of_instruction_bsn_disable_src_mac_check_subtype_set(
    of_instruction_bsn_disable_src_mac_check_t *obj,
    uint32_t subtype);
extern void of_instruction_bsn_disable_src_mac_check_subtype_get(
    of_instruction_bsn_disable_src_mac_check_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_bsn_packet_of_death */

extern void of_instruction_bsn_packet_of_death_experimenter_set(
    of_instruction_bsn_packet_of_death_t *obj,
    uint32_t experimenter);
extern void of_instruction_bsn_packet_of_death_experimenter_get(
    of_instruction_bsn_packet_of_death_t *obj,
    uint32_t *experimenter);

extern void of_instruction_bsn_packet_of_death_subtype_set(
    of_instruction_bsn_packet_of_death_t *obj,
    uint32_t subtype);
extern void of_instruction_bsn_packet_of_death_subtype_get(
    of_instruction_bsn_packet_of_death_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_bsn_permit */

extern void of_instruction_bsn_permit_experimenter_set(
    of_instruction_bsn_permit_t *obj,
    uint32_t experimenter);
extern void of_instruction_bsn_permit_experimenter_get(
    of_instruction_bsn_permit_t *obj,
    uint32_t *experimenter);

extern void of_instruction_bsn_permit_subtype_set(
    of_instruction_bsn_permit_t *obj,
    uint32_t subtype);
extern void of_instruction_bsn_permit_subtype_get(
    of_instruction_bsn_permit_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_clear_actions */

/* Unified accessor functions for of_instruction_experimenter */

extern void of_instruction_experimenter_experimenter_set(
    of_instruction_experimenter_t *obj,
    uint32_t experimenter);
extern void of_instruction_experimenter_experimenter_get(
    of_instruction_experimenter_t *obj,
    uint32_t *experimenter);

extern int WARN_UNUSED_RESULT of_instruction_experimenter_data_set(
    of_instruction_experimenter_t *obj,
    of_octets_t *data);
extern void of_instruction_experimenter_data_get(
    of_instruction_experimenter_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_instruction_goto_table */

extern void of_instruction_goto_table_table_id_set(
    of_instruction_goto_table_t *obj,
    uint8_t table_id);
extern void of_instruction_goto_table_table_id_get(
    of_instruction_goto_table_t *obj,
    uint8_t *table_id);

/* Unified accessor functions for of_instruction_header */

/* Unified accessor functions for of_instruction_id_apply_actions */

/* Unified accessor functions for of_instruction_id_bsn */

extern void of_instruction_id_bsn_experimenter_set(
    of_instruction_id_bsn_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_bsn_experimenter_get(
    of_instruction_id_bsn_t *obj,
    uint32_t *experimenter);

extern void of_instruction_id_bsn_subtype_set(
    of_instruction_id_bsn_t *obj,
    uint32_t subtype);
extern void of_instruction_id_bsn_subtype_get(
    of_instruction_id_bsn_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_id_bsn_arp_offload */

extern void of_instruction_id_bsn_arp_offload_experimenter_set(
    of_instruction_id_bsn_arp_offload_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_bsn_arp_offload_experimenter_get(
    of_instruction_id_bsn_arp_offload_t *obj,
    uint32_t *experimenter);

extern void of_instruction_id_bsn_arp_offload_subtype_set(
    of_instruction_id_bsn_arp_offload_t *obj,
    uint32_t subtype);
extern void of_instruction_id_bsn_arp_offload_subtype_get(
    of_instruction_id_bsn_arp_offload_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_id_bsn_deny */

extern void of_instruction_id_bsn_deny_experimenter_set(
    of_instruction_id_bsn_deny_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_bsn_deny_experimenter_get(
    of_instruction_id_bsn_deny_t *obj,
    uint32_t *experimenter);

extern void of_instruction_id_bsn_deny_subtype_set(
    of_instruction_id_bsn_deny_t *obj,
    uint32_t subtype);
extern void of_instruction_id_bsn_deny_subtype_get(
    of_instruction_id_bsn_deny_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_id_bsn_dhcp_offload */

extern void of_instruction_id_bsn_dhcp_offload_experimenter_set(
    of_instruction_id_bsn_dhcp_offload_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_bsn_dhcp_offload_experimenter_get(
    of_instruction_id_bsn_dhcp_offload_t *obj,
    uint32_t *experimenter);

extern void of_instruction_id_bsn_dhcp_offload_subtype_set(
    of_instruction_id_bsn_dhcp_offload_t *obj,
    uint32_t subtype);
extern void of_instruction_id_bsn_dhcp_offload_subtype_get(
    of_instruction_id_bsn_dhcp_offload_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_id_bsn_disable_split_horizon_check */

extern void of_instruction_id_bsn_disable_split_horizon_check_experimenter_set(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_bsn_disable_split_horizon_check_experimenter_get(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    uint32_t *experimenter);

extern void of_instruction_id_bsn_disable_split_horizon_check_subtype_set(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    uint32_t subtype);
extern void of_instruction_id_bsn_disable_split_horizon_check_subtype_get(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_id_bsn_disable_src_mac_check */

extern void of_instruction_id_bsn_disable_src_mac_check_experimenter_set(
    of_instruction_id_bsn_disable_src_mac_check_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_bsn_disable_src_mac_check_experimenter_get(
    of_instruction_id_bsn_disable_src_mac_check_t *obj,
    uint32_t *experimenter);

extern void of_instruction_id_bsn_disable_src_mac_check_subtype_set(
    of_instruction_id_bsn_disable_src_mac_check_t *obj,
    uint32_t subtype);
extern void of_instruction_id_bsn_disable_src_mac_check_subtype_get(
    of_instruction_id_bsn_disable_src_mac_check_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_id_bsn_packet_of_death */

extern void of_instruction_id_bsn_packet_of_death_experimenter_set(
    of_instruction_id_bsn_packet_of_death_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_bsn_packet_of_death_experimenter_get(
    of_instruction_id_bsn_packet_of_death_t *obj,
    uint32_t *experimenter);

extern void of_instruction_id_bsn_packet_of_death_subtype_set(
    of_instruction_id_bsn_packet_of_death_t *obj,
    uint32_t subtype);
extern void of_instruction_id_bsn_packet_of_death_subtype_get(
    of_instruction_id_bsn_packet_of_death_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_id_bsn_permit */

extern void of_instruction_id_bsn_permit_experimenter_set(
    of_instruction_id_bsn_permit_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_bsn_permit_experimenter_get(
    of_instruction_id_bsn_permit_t *obj,
    uint32_t *experimenter);

extern void of_instruction_id_bsn_permit_subtype_set(
    of_instruction_id_bsn_permit_t *obj,
    uint32_t subtype);
extern void of_instruction_id_bsn_permit_subtype_get(
    of_instruction_id_bsn_permit_t *obj,
    uint32_t *subtype);

/* Unified accessor functions for of_instruction_id_clear_actions */

/* Unified accessor functions for of_instruction_id_experimenter */

extern void of_instruction_id_experimenter_experimenter_set(
    of_instruction_id_experimenter_t *obj,
    uint32_t experimenter);
extern void of_instruction_id_experimenter_experimenter_get(
    of_instruction_id_experimenter_t *obj,
    uint32_t *experimenter);

/* Unified accessor functions for of_instruction_id_goto_table */

/* Unified accessor functions for of_instruction_id_header */

/* Unified accessor functions for of_instruction_id_meter */

/* Unified accessor functions for of_instruction_id_write_actions */

/* Unified accessor functions for of_instruction_id_write_metadata */

/* Unified accessor functions for of_instruction_meter */

extern void of_instruction_meter_meter_id_set(
    of_instruction_meter_t *obj,
    uint32_t meter_id);
extern void of_instruction_meter_meter_id_get(
    of_instruction_meter_t *obj,
    uint32_t *meter_id);

/* Unified accessor functions for of_instruction_write_actions */

extern int WARN_UNUSED_RESULT of_instruction_write_actions_actions_set(
    of_instruction_write_actions_t *obj,
    of_list_action_t *actions);
extern void of_instruction_write_actions_actions_bind(
    of_instruction_write_actions_t *obj,
    of_list_action_t *actions);
extern of_list_action_t *of_instruction_write_actions_actions_get(
    of_instruction_write_actions_t *obj);

/* Unified accessor functions for of_instruction_write_metadata */

extern void of_instruction_write_metadata_metadata_set(
    of_instruction_write_metadata_t *obj,
    uint64_t metadata);
extern void of_instruction_write_metadata_metadata_get(
    of_instruction_write_metadata_t *obj,
    uint64_t *metadata);

extern void of_instruction_write_metadata_metadata_mask_set(
    of_instruction_write_metadata_t *obj,
    uint64_t metadata_mask);
extern void of_instruction_write_metadata_metadata_mask_get(
    of_instruction_write_metadata_t *obj,
    uint64_t *metadata_mask);

/* Unified accessor functions for of_match_v1 */

extern void of_match_v1_wildcards_set(
    of_match_v1_t *obj,
    of_wc_bmap_t wildcards);
extern void of_match_v1_wildcards_get(
    of_match_v1_t *obj,
    of_wc_bmap_t *wildcards);

extern void of_match_v1_in_port_set(
    of_match_v1_t *obj,
    of_port_no_t in_port);
extern void of_match_v1_in_port_get(
    of_match_v1_t *obj,
    of_port_no_t *in_port);

extern void of_match_v1_eth_src_set(
    of_match_v1_t *obj,
    of_mac_addr_t eth_src);
extern void of_match_v1_eth_src_get(
    of_match_v1_t *obj,
    of_mac_addr_t *eth_src);

extern void of_match_v1_eth_dst_set(
    of_match_v1_t *obj,
    of_mac_addr_t eth_dst);
extern void of_match_v1_eth_dst_get(
    of_match_v1_t *obj,
    of_mac_addr_t *eth_dst);

extern void of_match_v1_vlan_vid_set(
    of_match_v1_t *obj,
    uint16_t vlan_vid);
extern void of_match_v1_vlan_vid_get(
    of_match_v1_t *obj,
    uint16_t *vlan_vid);

extern void of_match_v1_vlan_pcp_set(
    of_match_v1_t *obj,
    uint8_t vlan_pcp);
extern void of_match_v1_vlan_pcp_get(
    of_match_v1_t *obj,
    uint8_t *vlan_pcp);

extern void of_match_v1_eth_type_set(
    of_match_v1_t *obj,
    uint16_t eth_type);
extern void of_match_v1_eth_type_get(
    of_match_v1_t *obj,
    uint16_t *eth_type);

extern void of_match_v1_ip_dscp_set(
    of_match_v1_t *obj,
    uint8_t ip_dscp);
extern void of_match_v1_ip_dscp_get(
    of_match_v1_t *obj,
    uint8_t *ip_dscp);

extern void of_match_v1_ip_proto_set(
    of_match_v1_t *obj,
    uint8_t ip_proto);
extern void of_match_v1_ip_proto_get(
    of_match_v1_t *obj,
    uint8_t *ip_proto);

extern void of_match_v1_ipv4_src_set(
    of_match_v1_t *obj,
    of_ipv4_t ipv4_src);
extern void of_match_v1_ipv4_src_get(
    of_match_v1_t *obj,
    of_ipv4_t *ipv4_src);

extern void of_match_v1_ipv4_dst_set(
    of_match_v1_t *obj,
    of_ipv4_t ipv4_dst);
extern void of_match_v1_ipv4_dst_get(
    of_match_v1_t *obj,
    of_ipv4_t *ipv4_dst);

extern void of_match_v1_tcp_src_set(
    of_match_v1_t *obj,
    uint16_t tcp_src);
extern void of_match_v1_tcp_src_get(
    of_match_v1_t *obj,
    uint16_t *tcp_src);

extern void of_match_v1_tcp_dst_set(
    of_match_v1_t *obj,
    uint16_t tcp_dst);
extern void of_match_v1_tcp_dst_get(
    of_match_v1_t *obj,
    uint16_t *tcp_dst);

/* Unified accessor functions for of_match_v2 */

extern void of_match_v2_in_port_set(
    of_match_v2_t *obj,
    of_port_no_t in_port);
extern void of_match_v2_in_port_get(
    of_match_v2_t *obj,
    of_port_no_t *in_port);

extern void of_match_v2_wildcards_set(
    of_match_v2_t *obj,
    of_wc_bmap_t wildcards);
extern void of_match_v2_wildcards_get(
    of_match_v2_t *obj,
    of_wc_bmap_t *wildcards);

extern void of_match_v2_eth_src_set(
    of_match_v2_t *obj,
    of_mac_addr_t eth_src);
extern void of_match_v2_eth_src_get(
    of_match_v2_t *obj,
    of_mac_addr_t *eth_src);

extern void of_match_v2_eth_src_mask_set(
    of_match_v2_t *obj,
    of_mac_addr_t eth_src_mask);
extern void of_match_v2_eth_src_mask_get(
    of_match_v2_t *obj,
    of_mac_addr_t *eth_src_mask);

extern void of_match_v2_eth_dst_set(
    of_match_v2_t *obj,
    of_mac_addr_t eth_dst);
extern void of_match_v2_eth_dst_get(
    of_match_v2_t *obj,
    of_mac_addr_t *eth_dst);

extern void of_match_v2_eth_dst_mask_set(
    of_match_v2_t *obj,
    of_mac_addr_t eth_dst_mask);
extern void of_match_v2_eth_dst_mask_get(
    of_match_v2_t *obj,
    of_mac_addr_t *eth_dst_mask);

extern void of_match_v2_vlan_vid_set(
    of_match_v2_t *obj,
    uint16_t vlan_vid);
extern void of_match_v2_vlan_vid_get(
    of_match_v2_t *obj,
    uint16_t *vlan_vid);

extern void of_match_v2_vlan_pcp_set(
    of_match_v2_t *obj,
    uint8_t vlan_pcp);
extern void of_match_v2_vlan_pcp_get(
    of_match_v2_t *obj,
    uint8_t *vlan_pcp);

extern void of_match_v2_eth_type_set(
    of_match_v2_t *obj,
    uint16_t eth_type);
extern void of_match_v2_eth_type_get(
    of_match_v2_t *obj,
    uint16_t *eth_type);

extern void of_match_v2_ip_dscp_set(
    of_match_v2_t *obj,
    uint8_t ip_dscp);
extern void of_match_v2_ip_dscp_get(
    of_match_v2_t *obj,
    uint8_t *ip_dscp);

extern void of_match_v2_ip_proto_set(
    of_match_v2_t *obj,
    uint8_t ip_proto);
extern void of_match_v2_ip_proto_get(
    of_match_v2_t *obj,
    uint8_t *ip_proto);

extern void of_match_v2_ipv4_src_set(
    of_match_v2_t *obj,
    of_ipv4_t ipv4_src);
extern void of_match_v2_ipv4_src_get(
    of_match_v2_t *obj,
    of_ipv4_t *ipv4_src);

extern void of_match_v2_ipv4_src_mask_set(
    of_match_v2_t *obj,
    of_ipv4_t ipv4_src_mask);
extern void of_match_v2_ipv4_src_mask_get(
    of_match_v2_t *obj,
    of_ipv4_t *ipv4_src_mask);

extern void of_match_v2_ipv4_dst_set(
    of_match_v2_t *obj,
    of_ipv4_t ipv4_dst);
extern void of_match_v2_ipv4_dst_get(
    of_match_v2_t *obj,
    of_ipv4_t *ipv4_dst);

extern void of_match_v2_ipv4_dst_mask_set(
    of_match_v2_t *obj,
    of_ipv4_t ipv4_dst_mask);
extern void of_match_v2_ipv4_dst_mask_get(
    of_match_v2_t *obj,
    of_ipv4_t *ipv4_dst_mask);

extern void of_match_v2_tcp_src_set(
    of_match_v2_t *obj,
    uint16_t tcp_src);
extern void of_match_v2_tcp_src_get(
    of_match_v2_t *obj,
    uint16_t *tcp_src);

extern void of_match_v2_tcp_dst_set(
    of_match_v2_t *obj,
    uint16_t tcp_dst);
extern void of_match_v2_tcp_dst_get(
    of_match_v2_t *obj,
    uint16_t *tcp_dst);

extern void of_match_v2_mpls_label_set(
    of_match_v2_t *obj,
    uint32_t mpls_label);
extern void of_match_v2_mpls_label_get(
    of_match_v2_t *obj,
    uint32_t *mpls_label);

extern void of_match_v2_mpls_tc_set(
    of_match_v2_t *obj,
    uint8_t mpls_tc);
extern void of_match_v2_mpls_tc_get(
    of_match_v2_t *obj,
    uint8_t *mpls_tc);

extern void of_match_v2_metadata_set(
    of_match_v2_t *obj,
    uint64_t metadata);
extern void of_match_v2_metadata_get(
    of_match_v2_t *obj,
    uint64_t *metadata);

extern void of_match_v2_metadata_mask_set(
    of_match_v2_t *obj,
    uint64_t metadata_mask);
extern void of_match_v2_metadata_mask_get(
    of_match_v2_t *obj,
    uint64_t *metadata_mask);

/* Unified accessor functions for of_match_v3 */

extern int WARN_UNUSED_RESULT of_match_v3_oxm_list_set(
    of_match_v3_t *obj,
    of_list_oxm_t *oxm_list);
extern void of_match_v3_oxm_list_bind(
    of_match_v3_t *obj,
    of_list_oxm_t *oxm_list);
extern of_list_oxm_t *of_match_v3_oxm_list_get(
    of_match_v3_t *obj);

/* Unified accessor functions for of_meter_band_drop */

extern void of_meter_band_drop_rate_set(
    of_meter_band_drop_t *obj,
    uint32_t rate);
extern void of_meter_band_drop_rate_get(
    of_meter_band_drop_t *obj,
    uint32_t *rate);

extern void of_meter_band_drop_burst_size_set(
    of_meter_band_drop_t *obj,
    uint32_t burst_size);
extern void of_meter_band_drop_burst_size_get(
    of_meter_band_drop_t *obj,
    uint32_t *burst_size);

/* Unified accessor functions for of_meter_band_dscp_remark */

extern void of_meter_band_dscp_remark_rate_set(
    of_meter_band_dscp_remark_t *obj,
    uint32_t rate);
extern void of_meter_band_dscp_remark_rate_get(
    of_meter_band_dscp_remark_t *obj,
    uint32_t *rate);

extern void of_meter_band_dscp_remark_burst_size_set(
    of_meter_band_dscp_remark_t *obj,
    uint32_t burst_size);
extern void of_meter_band_dscp_remark_burst_size_get(
    of_meter_band_dscp_remark_t *obj,
    uint32_t *burst_size);

extern void of_meter_band_dscp_remark_prec_level_set(
    of_meter_band_dscp_remark_t *obj,
    uint8_t prec_level);
extern void of_meter_band_dscp_remark_prec_level_get(
    of_meter_band_dscp_remark_t *obj,
    uint8_t *prec_level);

/* Unified accessor functions for of_meter_band_experimenter */

extern void of_meter_band_experimenter_rate_set(
    of_meter_band_experimenter_t *obj,
    uint32_t rate);
extern void of_meter_band_experimenter_rate_get(
    of_meter_band_experimenter_t *obj,
    uint32_t *rate);

extern void of_meter_band_experimenter_burst_size_set(
    of_meter_band_experimenter_t *obj,
    uint32_t burst_size);
extern void of_meter_band_experimenter_burst_size_get(
    of_meter_band_experimenter_t *obj,
    uint32_t *burst_size);

extern void of_meter_band_experimenter_experimenter_set(
    of_meter_band_experimenter_t *obj,
    uint32_t experimenter);
extern void of_meter_band_experimenter_experimenter_get(
    of_meter_band_experimenter_t *obj,
    uint32_t *experimenter);

extern int WARN_UNUSED_RESULT of_meter_band_experimenter_data_set(
    of_meter_band_experimenter_t *obj,
    of_octets_t *data);
extern void of_meter_band_experimenter_data_get(
    of_meter_band_experimenter_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_meter_band_header */

/* Unified accessor functions for of_meter_band_ofdpa_color_set */

extern void of_meter_band_ofdpa_color_set_rate_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t rate);
extern void of_meter_band_ofdpa_color_set_rate_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t *rate);

extern void of_meter_band_ofdpa_color_set_burst_size_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t burst_size);
extern void of_meter_band_ofdpa_color_set_burst_size_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t *burst_size);

extern void of_meter_band_ofdpa_color_set_experimenter_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t experimenter);
extern void of_meter_band_ofdpa_color_set_experimenter_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t *experimenter);

extern void of_meter_band_ofdpa_color_set_subtype_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint16_t subtype);
extern void of_meter_band_ofdpa_color_set_subtype_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint16_t *subtype);

extern void of_meter_band_ofdpa_color_set_mode_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t mode);
extern void of_meter_band_ofdpa_color_set_mode_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t *mode);

extern void of_meter_band_ofdpa_color_set_color_aware_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t color_aware);
extern void of_meter_band_ofdpa_color_set_color_aware_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t *color_aware);

extern void of_meter_band_ofdpa_color_set_color_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t color);
extern void of_meter_band_ofdpa_color_set_color_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t *color);

/* Unified accessor functions for of_meter_band_stats */

extern void of_meter_band_stats_packet_band_count_set(
    of_meter_band_stats_t *obj,
    uint64_t packet_band_count);
extern void of_meter_band_stats_packet_band_count_get(
    of_meter_band_stats_t *obj,
    uint64_t *packet_band_count);

extern void of_meter_band_stats_byte_band_count_set(
    of_meter_band_stats_t *obj,
    uint64_t byte_band_count);
extern void of_meter_band_stats_byte_band_count_get(
    of_meter_band_stats_t *obj,
    uint64_t *byte_band_count);

/* Unified accessor functions for of_meter_config */

extern void of_meter_config_flags_set(
    of_meter_config_t *obj,
    uint16_t flags);
extern void of_meter_config_flags_get(
    of_meter_config_t *obj,
    uint16_t *flags);

extern void of_meter_config_meter_id_set(
    of_meter_config_t *obj,
    uint32_t meter_id);
extern void of_meter_config_meter_id_get(
    of_meter_config_t *obj,
    uint32_t *meter_id);

extern int WARN_UNUSED_RESULT of_meter_config_entries_set(
    of_meter_config_t *obj,
    of_list_meter_band_t *entries);
extern void of_meter_config_entries_bind(
    of_meter_config_t *obj,
    of_list_meter_band_t *entries);
extern of_list_meter_band_t *of_meter_config_entries_get(
    of_meter_config_t *obj);

/* Unified accessor functions for of_meter_features */

extern void of_meter_features_max_meter_set(
    of_meter_features_t *obj,
    uint32_t max_meter);
extern void of_meter_features_max_meter_get(
    of_meter_features_t *obj,
    uint32_t *max_meter);

extern void of_meter_features_band_types_set(
    of_meter_features_t *obj,
    uint32_t band_types);
extern void of_meter_features_band_types_get(
    of_meter_features_t *obj,
    uint32_t *band_types);

extern void of_meter_features_capabilities_set(
    of_meter_features_t *obj,
    uint32_t capabilities);
extern void of_meter_features_capabilities_get(
    of_meter_features_t *obj,
    uint32_t *capabilities);

extern void of_meter_features_max_bands_set(
    of_meter_features_t *obj,
    uint8_t max_bands);
extern void of_meter_features_max_bands_get(
    of_meter_features_t *obj,
    uint8_t *max_bands);

extern void of_meter_features_max_color_set(
    of_meter_features_t *obj,
    uint8_t max_color);
extern void of_meter_features_max_color_get(
    of_meter_features_t *obj,
    uint8_t *max_color);

/* Unified accessor functions for of_meter_stats */

extern void of_meter_stats_meter_id_set(
    of_meter_stats_t *obj,
    uint32_t meter_id);
extern void of_meter_stats_meter_id_get(
    of_meter_stats_t *obj,
    uint32_t *meter_id);

extern void of_meter_stats_flow_count_set(
    of_meter_stats_t *obj,
    uint32_t flow_count);
extern void of_meter_stats_flow_count_get(
    of_meter_stats_t *obj,
    uint32_t *flow_count);

extern void of_meter_stats_packet_in_count_set(
    of_meter_stats_t *obj,
    uint64_t packet_in_count);
extern void of_meter_stats_packet_in_count_get(
    of_meter_stats_t *obj,
    uint64_t *packet_in_count);

extern void of_meter_stats_byte_in_count_set(
    of_meter_stats_t *obj,
    uint64_t byte_in_count);
extern void of_meter_stats_byte_in_count_get(
    of_meter_stats_t *obj,
    uint64_t *byte_in_count);

extern void of_meter_stats_duration_sec_set(
    of_meter_stats_t *obj,
    uint32_t duration_sec);
extern void of_meter_stats_duration_sec_get(
    of_meter_stats_t *obj,
    uint32_t *duration_sec);

extern void of_meter_stats_duration_nsec_set(
    of_meter_stats_t *obj,
    uint32_t duration_nsec);
extern void of_meter_stats_duration_nsec_get(
    of_meter_stats_t *obj,
    uint32_t *duration_nsec);

extern int WARN_UNUSED_RESULT of_meter_stats_band_stats_set(
    of_meter_stats_t *obj,
    of_list_meter_band_stats_t *band_stats);
extern void of_meter_stats_band_stats_bind(
    of_meter_stats_t *obj,
    of_list_meter_band_stats_t *band_stats);
extern of_list_meter_band_stats_t *of_meter_stats_band_stats_get(
    of_meter_stats_t *obj);

/* Unified accessor functions for of_oxm_arp_op */

extern void of_oxm_arp_op_value_set(
    of_oxm_arp_op_t *obj,
    uint16_t value);
extern void of_oxm_arp_op_value_get(
    of_oxm_arp_op_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_arp_op_masked */

extern void of_oxm_arp_op_masked_value_set(
    of_oxm_arp_op_masked_t *obj,
    uint16_t value);
extern void of_oxm_arp_op_masked_value_get(
    of_oxm_arp_op_masked_t *obj,
    uint16_t *value);

extern void of_oxm_arp_op_masked_value_mask_set(
    of_oxm_arp_op_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_arp_op_masked_value_mask_get(
    of_oxm_arp_op_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_oxm_arp_sha */

extern void of_oxm_arp_sha_value_set(
    of_oxm_arp_sha_t *obj,
    of_mac_addr_t value);
extern void of_oxm_arp_sha_value_get(
    of_oxm_arp_sha_t *obj,
    of_mac_addr_t *value);

/* Unified accessor functions for of_oxm_arp_sha_masked */

extern void of_oxm_arp_sha_masked_value_set(
    of_oxm_arp_sha_masked_t *obj,
    of_mac_addr_t value);
extern void of_oxm_arp_sha_masked_value_get(
    of_oxm_arp_sha_masked_t *obj,
    of_mac_addr_t *value);

extern void of_oxm_arp_sha_masked_value_mask_set(
    of_oxm_arp_sha_masked_t *obj,
    of_mac_addr_t value_mask);
extern void of_oxm_arp_sha_masked_value_mask_get(
    of_oxm_arp_sha_masked_t *obj,
    of_mac_addr_t *value_mask);

/* Unified accessor functions for of_oxm_arp_spa */

extern void of_oxm_arp_spa_value_set(
    of_oxm_arp_spa_t *obj,
    uint32_t value);
extern void of_oxm_arp_spa_value_get(
    of_oxm_arp_spa_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_arp_spa_masked */

extern void of_oxm_arp_spa_masked_value_set(
    of_oxm_arp_spa_masked_t *obj,
    uint32_t value);
extern void of_oxm_arp_spa_masked_value_get(
    of_oxm_arp_spa_masked_t *obj,
    uint32_t *value);

extern void of_oxm_arp_spa_masked_value_mask_set(
    of_oxm_arp_spa_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_arp_spa_masked_value_mask_get(
    of_oxm_arp_spa_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_arp_tha */

extern void of_oxm_arp_tha_value_set(
    of_oxm_arp_tha_t *obj,
    of_mac_addr_t value);
extern void of_oxm_arp_tha_value_get(
    of_oxm_arp_tha_t *obj,
    of_mac_addr_t *value);

/* Unified accessor functions for of_oxm_arp_tha_masked */

extern void of_oxm_arp_tha_masked_value_set(
    of_oxm_arp_tha_masked_t *obj,
    of_mac_addr_t value);
extern void of_oxm_arp_tha_masked_value_get(
    of_oxm_arp_tha_masked_t *obj,
    of_mac_addr_t *value);

extern void of_oxm_arp_tha_masked_value_mask_set(
    of_oxm_arp_tha_masked_t *obj,
    of_mac_addr_t value_mask);
extern void of_oxm_arp_tha_masked_value_mask_get(
    of_oxm_arp_tha_masked_t *obj,
    of_mac_addr_t *value_mask);

/* Unified accessor functions for of_oxm_arp_tpa */

extern void of_oxm_arp_tpa_value_set(
    of_oxm_arp_tpa_t *obj,
    uint32_t value);
extern void of_oxm_arp_tpa_value_get(
    of_oxm_arp_tpa_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_arp_tpa_masked */

extern void of_oxm_arp_tpa_masked_value_set(
    of_oxm_arp_tpa_masked_t *obj,
    uint32_t value);
extern void of_oxm_arp_tpa_masked_value_get(
    of_oxm_arp_tpa_masked_t *obj,
    uint32_t *value);

extern void of_oxm_arp_tpa_masked_value_mask_set(
    of_oxm_arp_tpa_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_arp_tpa_masked_value_mask_get(
    of_oxm_arp_tpa_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_egr_port_group_id */

extern void of_oxm_bsn_egr_port_group_id_value_set(
    of_oxm_bsn_egr_port_group_id_t *obj,
    uint32_t value);
extern void of_oxm_bsn_egr_port_group_id_value_get(
    of_oxm_bsn_egr_port_group_id_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_egr_port_group_id_masked */

extern void of_oxm_bsn_egr_port_group_id_masked_value_set(
    of_oxm_bsn_egr_port_group_id_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_egr_port_group_id_masked_value_get(
    of_oxm_bsn_egr_port_group_id_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_egr_port_group_id_masked_value_mask_set(
    of_oxm_bsn_egr_port_group_id_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_egr_port_group_id_masked_value_mask_get(
    of_oxm_bsn_egr_port_group_id_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_global_vrf_allowed */

extern void of_oxm_bsn_global_vrf_allowed_value_set(
    of_oxm_bsn_global_vrf_allowed_t *obj,
    uint8_t value);
extern void of_oxm_bsn_global_vrf_allowed_value_get(
    of_oxm_bsn_global_vrf_allowed_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_bsn_global_vrf_allowed_masked */

extern void of_oxm_bsn_global_vrf_allowed_masked_value_set(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    uint8_t value);
extern void of_oxm_bsn_global_vrf_allowed_masked_value_get(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    uint8_t *value);

extern void of_oxm_bsn_global_vrf_allowed_masked_value_mask_set(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_bsn_global_vrf_allowed_masked_value_mask_get(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_in_ports_128 */

extern void of_oxm_bsn_in_ports_128_value_set(
    of_oxm_bsn_in_ports_128_t *obj,
    of_bitmap_128_t value);
extern void of_oxm_bsn_in_ports_128_value_get(
    of_oxm_bsn_in_ports_128_t *obj,
    of_bitmap_128_t *value);

/* Unified accessor functions for of_oxm_bsn_in_ports_128_masked */

extern void of_oxm_bsn_in_ports_128_masked_value_set(
    of_oxm_bsn_in_ports_128_masked_t *obj,
    of_bitmap_128_t value);
extern void of_oxm_bsn_in_ports_128_masked_value_get(
    of_oxm_bsn_in_ports_128_masked_t *obj,
    of_bitmap_128_t *value);

extern void of_oxm_bsn_in_ports_128_masked_value_mask_set(
    of_oxm_bsn_in_ports_128_masked_t *obj,
    of_bitmap_128_t value_mask);
extern void of_oxm_bsn_in_ports_128_masked_value_mask_get(
    of_oxm_bsn_in_ports_128_masked_t *obj,
    of_bitmap_128_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_l3_dst_class_id */

extern void of_oxm_bsn_l3_dst_class_id_value_set(
    of_oxm_bsn_l3_dst_class_id_t *obj,
    uint32_t value);
extern void of_oxm_bsn_l3_dst_class_id_value_get(
    of_oxm_bsn_l3_dst_class_id_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_l3_dst_class_id_masked */

extern void of_oxm_bsn_l3_dst_class_id_masked_value_set(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_l3_dst_class_id_masked_value_get(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_l3_dst_class_id_masked_value_mask_set(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_l3_dst_class_id_masked_value_mask_get(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_l3_interface_class_id */

extern void of_oxm_bsn_l3_interface_class_id_value_set(
    of_oxm_bsn_l3_interface_class_id_t *obj,
    uint32_t value);
extern void of_oxm_bsn_l3_interface_class_id_value_get(
    of_oxm_bsn_l3_interface_class_id_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_l3_interface_class_id_masked */

extern void of_oxm_bsn_l3_interface_class_id_masked_value_set(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_l3_interface_class_id_masked_value_get(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_l3_interface_class_id_masked_value_mask_set(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_l3_interface_class_id_masked_value_mask_get(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_l3_src_class_id */

extern void of_oxm_bsn_l3_src_class_id_value_set(
    of_oxm_bsn_l3_src_class_id_t *obj,
    uint32_t value);
extern void of_oxm_bsn_l3_src_class_id_value_get(
    of_oxm_bsn_l3_src_class_id_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_l3_src_class_id_masked */

extern void of_oxm_bsn_l3_src_class_id_masked_value_set(
    of_oxm_bsn_l3_src_class_id_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_l3_src_class_id_masked_value_get(
    of_oxm_bsn_l3_src_class_id_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_l3_src_class_id_masked_value_mask_set(
    of_oxm_bsn_l3_src_class_id_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_l3_src_class_id_masked_value_mask_get(
    of_oxm_bsn_l3_src_class_id_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_lag_id */

extern void of_oxm_bsn_lag_id_value_set(
    of_oxm_bsn_lag_id_t *obj,
    uint32_t value);
extern void of_oxm_bsn_lag_id_value_get(
    of_oxm_bsn_lag_id_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_lag_id_masked */

extern void of_oxm_bsn_lag_id_masked_value_set(
    of_oxm_bsn_lag_id_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_lag_id_masked_value_get(
    of_oxm_bsn_lag_id_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_lag_id_masked_value_mask_set(
    of_oxm_bsn_lag_id_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_lag_id_masked_value_mask_get(
    of_oxm_bsn_lag_id_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_udf0 */

extern void of_oxm_bsn_udf0_value_set(
    of_oxm_bsn_udf0_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf0_value_get(
    of_oxm_bsn_udf0_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_udf0_masked */

extern void of_oxm_bsn_udf0_masked_value_set(
    of_oxm_bsn_udf0_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf0_masked_value_get(
    of_oxm_bsn_udf0_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_udf0_masked_value_mask_set(
    of_oxm_bsn_udf0_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_udf0_masked_value_mask_get(
    of_oxm_bsn_udf0_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_udf1 */

extern void of_oxm_bsn_udf1_value_set(
    of_oxm_bsn_udf1_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf1_value_get(
    of_oxm_bsn_udf1_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_udf1_masked */

extern void of_oxm_bsn_udf1_masked_value_set(
    of_oxm_bsn_udf1_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf1_masked_value_get(
    of_oxm_bsn_udf1_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_udf1_masked_value_mask_set(
    of_oxm_bsn_udf1_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_udf1_masked_value_mask_get(
    of_oxm_bsn_udf1_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_udf2 */

extern void of_oxm_bsn_udf2_value_set(
    of_oxm_bsn_udf2_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf2_value_get(
    of_oxm_bsn_udf2_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_udf2_masked */

extern void of_oxm_bsn_udf2_masked_value_set(
    of_oxm_bsn_udf2_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf2_masked_value_get(
    of_oxm_bsn_udf2_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_udf2_masked_value_mask_set(
    of_oxm_bsn_udf2_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_udf2_masked_value_mask_get(
    of_oxm_bsn_udf2_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_udf3 */

extern void of_oxm_bsn_udf3_value_set(
    of_oxm_bsn_udf3_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf3_value_get(
    of_oxm_bsn_udf3_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_udf3_masked */

extern void of_oxm_bsn_udf3_masked_value_set(
    of_oxm_bsn_udf3_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf3_masked_value_get(
    of_oxm_bsn_udf3_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_udf3_masked_value_mask_set(
    of_oxm_bsn_udf3_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_udf3_masked_value_mask_get(
    of_oxm_bsn_udf3_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_udf4 */

extern void of_oxm_bsn_udf4_value_set(
    of_oxm_bsn_udf4_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf4_value_get(
    of_oxm_bsn_udf4_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_udf4_masked */

extern void of_oxm_bsn_udf4_masked_value_set(
    of_oxm_bsn_udf4_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf4_masked_value_get(
    of_oxm_bsn_udf4_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_udf4_masked_value_mask_set(
    of_oxm_bsn_udf4_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_udf4_masked_value_mask_get(
    of_oxm_bsn_udf4_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_udf5 */

extern void of_oxm_bsn_udf5_value_set(
    of_oxm_bsn_udf5_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf5_value_get(
    of_oxm_bsn_udf5_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_udf5_masked */

extern void of_oxm_bsn_udf5_masked_value_set(
    of_oxm_bsn_udf5_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf5_masked_value_get(
    of_oxm_bsn_udf5_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_udf5_masked_value_mask_set(
    of_oxm_bsn_udf5_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_udf5_masked_value_mask_get(
    of_oxm_bsn_udf5_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_udf6 */

extern void of_oxm_bsn_udf6_value_set(
    of_oxm_bsn_udf6_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf6_value_get(
    of_oxm_bsn_udf6_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_udf6_masked */

extern void of_oxm_bsn_udf6_masked_value_set(
    of_oxm_bsn_udf6_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf6_masked_value_get(
    of_oxm_bsn_udf6_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_udf6_masked_value_mask_set(
    of_oxm_bsn_udf6_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_udf6_masked_value_mask_get(
    of_oxm_bsn_udf6_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_udf7 */

extern void of_oxm_bsn_udf7_value_set(
    of_oxm_bsn_udf7_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf7_value_get(
    of_oxm_bsn_udf7_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_udf7_masked */

extern void of_oxm_bsn_udf7_masked_value_set(
    of_oxm_bsn_udf7_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_udf7_masked_value_get(
    of_oxm_bsn_udf7_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_udf7_masked_value_mask_set(
    of_oxm_bsn_udf7_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_udf7_masked_value_mask_get(
    of_oxm_bsn_udf7_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_bsn_vrf */

extern void of_oxm_bsn_vrf_value_set(
    of_oxm_bsn_vrf_t *obj,
    uint32_t value);
extern void of_oxm_bsn_vrf_value_get(
    of_oxm_bsn_vrf_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_bsn_vrf_masked */

extern void of_oxm_bsn_vrf_masked_value_set(
    of_oxm_bsn_vrf_masked_t *obj,
    uint32_t value);
extern void of_oxm_bsn_vrf_masked_value_get(
    of_oxm_bsn_vrf_masked_t *obj,
    uint32_t *value);

extern void of_oxm_bsn_vrf_masked_value_mask_set(
    of_oxm_bsn_vrf_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_bsn_vrf_masked_value_mask_get(
    of_oxm_bsn_vrf_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_eth_dst */

extern void of_oxm_eth_dst_value_set(
    of_oxm_eth_dst_t *obj,
    of_mac_addr_t value);
extern void of_oxm_eth_dst_value_get(
    of_oxm_eth_dst_t *obj,
    of_mac_addr_t *value);

/* Unified accessor functions for of_oxm_eth_dst_masked */

extern void of_oxm_eth_dst_masked_value_set(
    of_oxm_eth_dst_masked_t *obj,
    of_mac_addr_t value);
extern void of_oxm_eth_dst_masked_value_get(
    of_oxm_eth_dst_masked_t *obj,
    of_mac_addr_t *value);

extern void of_oxm_eth_dst_masked_value_mask_set(
    of_oxm_eth_dst_masked_t *obj,
    of_mac_addr_t value_mask);
extern void of_oxm_eth_dst_masked_value_mask_get(
    of_oxm_eth_dst_masked_t *obj,
    of_mac_addr_t *value_mask);

/* Unified accessor functions for of_oxm_eth_src */

extern void of_oxm_eth_src_value_set(
    of_oxm_eth_src_t *obj,
    of_mac_addr_t value);
extern void of_oxm_eth_src_value_get(
    of_oxm_eth_src_t *obj,
    of_mac_addr_t *value);

/* Unified accessor functions for of_oxm_eth_src_masked */

extern void of_oxm_eth_src_masked_value_set(
    of_oxm_eth_src_masked_t *obj,
    of_mac_addr_t value);
extern void of_oxm_eth_src_masked_value_get(
    of_oxm_eth_src_masked_t *obj,
    of_mac_addr_t *value);

extern void of_oxm_eth_src_masked_value_mask_set(
    of_oxm_eth_src_masked_t *obj,
    of_mac_addr_t value_mask);
extern void of_oxm_eth_src_masked_value_mask_get(
    of_oxm_eth_src_masked_t *obj,
    of_mac_addr_t *value_mask);

/* Unified accessor functions for of_oxm_eth_type */

extern void of_oxm_eth_type_value_set(
    of_oxm_eth_type_t *obj,
    uint16_t value);
extern void of_oxm_eth_type_value_get(
    of_oxm_eth_type_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_eth_type_masked */

extern void of_oxm_eth_type_masked_value_set(
    of_oxm_eth_type_masked_t *obj,
    uint16_t value);
extern void of_oxm_eth_type_masked_value_get(
    of_oxm_eth_type_masked_t *obj,
    uint16_t *value);

extern void of_oxm_eth_type_masked_value_mask_set(
    of_oxm_eth_type_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_eth_type_masked_value_mask_get(
    of_oxm_eth_type_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_oxm_header */

/* Unified accessor functions for of_oxm_icmpv4_code */

extern void of_oxm_icmpv4_code_value_set(
    of_oxm_icmpv4_code_t *obj,
    uint8_t value);
extern void of_oxm_icmpv4_code_value_get(
    of_oxm_icmpv4_code_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_icmpv4_code_masked */

extern void of_oxm_icmpv4_code_masked_value_set(
    of_oxm_icmpv4_code_masked_t *obj,
    uint8_t value);
extern void of_oxm_icmpv4_code_masked_value_get(
    of_oxm_icmpv4_code_masked_t *obj,
    uint8_t *value);

extern void of_oxm_icmpv4_code_masked_value_mask_set(
    of_oxm_icmpv4_code_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_icmpv4_code_masked_value_mask_get(
    of_oxm_icmpv4_code_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_icmpv4_type */

extern void of_oxm_icmpv4_type_value_set(
    of_oxm_icmpv4_type_t *obj,
    uint8_t value);
extern void of_oxm_icmpv4_type_value_get(
    of_oxm_icmpv4_type_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_icmpv4_type_masked */

extern void of_oxm_icmpv4_type_masked_value_set(
    of_oxm_icmpv4_type_masked_t *obj,
    uint8_t value);
extern void of_oxm_icmpv4_type_masked_value_get(
    of_oxm_icmpv4_type_masked_t *obj,
    uint8_t *value);

extern void of_oxm_icmpv4_type_masked_value_mask_set(
    of_oxm_icmpv4_type_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_icmpv4_type_masked_value_mask_get(
    of_oxm_icmpv4_type_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_icmpv6_code */

extern void of_oxm_icmpv6_code_value_set(
    of_oxm_icmpv6_code_t *obj,
    uint8_t value);
extern void of_oxm_icmpv6_code_value_get(
    of_oxm_icmpv6_code_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_icmpv6_code_masked */

extern void of_oxm_icmpv6_code_masked_value_set(
    of_oxm_icmpv6_code_masked_t *obj,
    uint8_t value);
extern void of_oxm_icmpv6_code_masked_value_get(
    of_oxm_icmpv6_code_masked_t *obj,
    uint8_t *value);

extern void of_oxm_icmpv6_code_masked_value_mask_set(
    of_oxm_icmpv6_code_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_icmpv6_code_masked_value_mask_get(
    of_oxm_icmpv6_code_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_icmpv6_type */

extern void of_oxm_icmpv6_type_value_set(
    of_oxm_icmpv6_type_t *obj,
    uint8_t value);
extern void of_oxm_icmpv6_type_value_get(
    of_oxm_icmpv6_type_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_icmpv6_type_masked */

extern void of_oxm_icmpv6_type_masked_value_set(
    of_oxm_icmpv6_type_masked_t *obj,
    uint8_t value);
extern void of_oxm_icmpv6_type_masked_value_get(
    of_oxm_icmpv6_type_masked_t *obj,
    uint8_t *value);

extern void of_oxm_icmpv6_type_masked_value_mask_set(
    of_oxm_icmpv6_type_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_icmpv6_type_masked_value_mask_get(
    of_oxm_icmpv6_type_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_in_phy_port */

extern void of_oxm_in_phy_port_value_set(
    of_oxm_in_phy_port_t *obj,
    of_port_no_t value);
extern void of_oxm_in_phy_port_value_get(
    of_oxm_in_phy_port_t *obj,
    of_port_no_t *value);

/* Unified accessor functions for of_oxm_in_phy_port_masked */

extern void of_oxm_in_phy_port_masked_value_set(
    of_oxm_in_phy_port_masked_t *obj,
    of_port_no_t value);
extern void of_oxm_in_phy_port_masked_value_get(
    of_oxm_in_phy_port_masked_t *obj,
    of_port_no_t *value);

extern void of_oxm_in_phy_port_masked_value_mask_set(
    of_oxm_in_phy_port_masked_t *obj,
    of_port_no_t value_mask);
extern void of_oxm_in_phy_port_masked_value_mask_get(
    of_oxm_in_phy_port_masked_t *obj,
    of_port_no_t *value_mask);

/* Unified accessor functions for of_oxm_in_port */

extern void of_oxm_in_port_value_set(
    of_oxm_in_port_t *obj,
    of_port_no_t value);
extern void of_oxm_in_port_value_get(
    of_oxm_in_port_t *obj,
    of_port_no_t *value);

/* Unified accessor functions for of_oxm_in_port_masked */

extern void of_oxm_in_port_masked_value_set(
    of_oxm_in_port_masked_t *obj,
    of_port_no_t value);
extern void of_oxm_in_port_masked_value_get(
    of_oxm_in_port_masked_t *obj,
    of_port_no_t *value);

extern void of_oxm_in_port_masked_value_mask_set(
    of_oxm_in_port_masked_t *obj,
    of_port_no_t value_mask);
extern void of_oxm_in_port_masked_value_mask_get(
    of_oxm_in_port_masked_t *obj,
    of_port_no_t *value_mask);

/* Unified accessor functions for of_oxm_ip_dscp */

extern void of_oxm_ip_dscp_value_set(
    of_oxm_ip_dscp_t *obj,
    uint8_t value);
extern void of_oxm_ip_dscp_value_get(
    of_oxm_ip_dscp_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ip_dscp_masked */

extern void of_oxm_ip_dscp_masked_value_set(
    of_oxm_ip_dscp_masked_t *obj,
    uint8_t value);
extern void of_oxm_ip_dscp_masked_value_get(
    of_oxm_ip_dscp_masked_t *obj,
    uint8_t *value);

extern void of_oxm_ip_dscp_masked_value_mask_set(
    of_oxm_ip_dscp_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_ip_dscp_masked_value_mask_get(
    of_oxm_ip_dscp_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_ip_ecn */

extern void of_oxm_ip_ecn_value_set(
    of_oxm_ip_ecn_t *obj,
    uint8_t value);
extern void of_oxm_ip_ecn_value_get(
    of_oxm_ip_ecn_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ip_ecn_masked */

extern void of_oxm_ip_ecn_masked_value_set(
    of_oxm_ip_ecn_masked_t *obj,
    uint8_t value);
extern void of_oxm_ip_ecn_masked_value_get(
    of_oxm_ip_ecn_masked_t *obj,
    uint8_t *value);

extern void of_oxm_ip_ecn_masked_value_mask_set(
    of_oxm_ip_ecn_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_ip_ecn_masked_value_mask_get(
    of_oxm_ip_ecn_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_ip_proto */

extern void of_oxm_ip_proto_value_set(
    of_oxm_ip_proto_t *obj,
    uint8_t value);
extern void of_oxm_ip_proto_value_get(
    of_oxm_ip_proto_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ip_proto_masked */

extern void of_oxm_ip_proto_masked_value_set(
    of_oxm_ip_proto_masked_t *obj,
    uint8_t value);
extern void of_oxm_ip_proto_masked_value_get(
    of_oxm_ip_proto_masked_t *obj,
    uint8_t *value);

extern void of_oxm_ip_proto_masked_value_mask_set(
    of_oxm_ip_proto_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_ip_proto_masked_value_mask_get(
    of_oxm_ip_proto_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_ipv4_dst */

extern void of_oxm_ipv4_dst_value_set(
    of_oxm_ipv4_dst_t *obj,
    of_ipv4_t value);
extern void of_oxm_ipv4_dst_value_get(
    of_oxm_ipv4_dst_t *obj,
    of_ipv4_t *value);

/* Unified accessor functions for of_oxm_ipv4_dst_masked */

extern void of_oxm_ipv4_dst_masked_value_set(
    of_oxm_ipv4_dst_masked_t *obj,
    of_ipv4_t value);
extern void of_oxm_ipv4_dst_masked_value_get(
    of_oxm_ipv4_dst_masked_t *obj,
    of_ipv4_t *value);

extern void of_oxm_ipv4_dst_masked_value_mask_set(
    of_oxm_ipv4_dst_masked_t *obj,
    of_ipv4_t value_mask);
extern void of_oxm_ipv4_dst_masked_value_mask_get(
    of_oxm_ipv4_dst_masked_t *obj,
    of_ipv4_t *value_mask);

/* Unified accessor functions for of_oxm_ipv4_src */

extern void of_oxm_ipv4_src_value_set(
    of_oxm_ipv4_src_t *obj,
    of_ipv4_t value);
extern void of_oxm_ipv4_src_value_get(
    of_oxm_ipv4_src_t *obj,
    of_ipv4_t *value);

/* Unified accessor functions for of_oxm_ipv4_src_masked */

extern void of_oxm_ipv4_src_masked_value_set(
    of_oxm_ipv4_src_masked_t *obj,
    of_ipv4_t value);
extern void of_oxm_ipv4_src_masked_value_get(
    of_oxm_ipv4_src_masked_t *obj,
    of_ipv4_t *value);

extern void of_oxm_ipv4_src_masked_value_mask_set(
    of_oxm_ipv4_src_masked_t *obj,
    of_ipv4_t value_mask);
extern void of_oxm_ipv4_src_masked_value_mask_get(
    of_oxm_ipv4_src_masked_t *obj,
    of_ipv4_t *value_mask);

/* Unified accessor functions for of_oxm_ipv6_dst */

extern void of_oxm_ipv6_dst_value_set(
    of_oxm_ipv6_dst_t *obj,
    of_ipv6_t value);
extern void of_oxm_ipv6_dst_value_get(
    of_oxm_ipv6_dst_t *obj,
    of_ipv6_t *value);

/* Unified accessor functions for of_oxm_ipv6_dst_masked */

extern void of_oxm_ipv6_dst_masked_value_set(
    of_oxm_ipv6_dst_masked_t *obj,
    of_ipv6_t value);
extern void of_oxm_ipv6_dst_masked_value_get(
    of_oxm_ipv6_dst_masked_t *obj,
    of_ipv6_t *value);

extern void of_oxm_ipv6_dst_masked_value_mask_set(
    of_oxm_ipv6_dst_masked_t *obj,
    of_ipv6_t value_mask);
extern void of_oxm_ipv6_dst_masked_value_mask_get(
    of_oxm_ipv6_dst_masked_t *obj,
    of_ipv6_t *value_mask);

/* Unified accessor functions for of_oxm_ipv6_flabel */

extern void of_oxm_ipv6_flabel_value_set(
    of_oxm_ipv6_flabel_t *obj,
    uint32_t value);
extern void of_oxm_ipv6_flabel_value_get(
    of_oxm_ipv6_flabel_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_ipv6_flabel_masked */

extern void of_oxm_ipv6_flabel_masked_value_set(
    of_oxm_ipv6_flabel_masked_t *obj,
    uint32_t value);
extern void of_oxm_ipv6_flabel_masked_value_get(
    of_oxm_ipv6_flabel_masked_t *obj,
    uint32_t *value);

extern void of_oxm_ipv6_flabel_masked_value_mask_set(
    of_oxm_ipv6_flabel_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_ipv6_flabel_masked_value_mask_get(
    of_oxm_ipv6_flabel_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_ipv6_nd_sll */

extern void of_oxm_ipv6_nd_sll_value_set(
    of_oxm_ipv6_nd_sll_t *obj,
    of_mac_addr_t value);
extern void of_oxm_ipv6_nd_sll_value_get(
    of_oxm_ipv6_nd_sll_t *obj,
    of_mac_addr_t *value);

/* Unified accessor functions for of_oxm_ipv6_nd_sll_masked */

extern void of_oxm_ipv6_nd_sll_masked_value_set(
    of_oxm_ipv6_nd_sll_masked_t *obj,
    of_mac_addr_t value);
extern void of_oxm_ipv6_nd_sll_masked_value_get(
    of_oxm_ipv6_nd_sll_masked_t *obj,
    of_mac_addr_t *value);

extern void of_oxm_ipv6_nd_sll_masked_value_mask_set(
    of_oxm_ipv6_nd_sll_masked_t *obj,
    of_mac_addr_t value_mask);
extern void of_oxm_ipv6_nd_sll_masked_value_mask_get(
    of_oxm_ipv6_nd_sll_masked_t *obj,
    of_mac_addr_t *value_mask);

/* Unified accessor functions for of_oxm_ipv6_nd_target */

extern void of_oxm_ipv6_nd_target_value_set(
    of_oxm_ipv6_nd_target_t *obj,
    of_ipv6_t value);
extern void of_oxm_ipv6_nd_target_value_get(
    of_oxm_ipv6_nd_target_t *obj,
    of_ipv6_t *value);

/* Unified accessor functions for of_oxm_ipv6_nd_target_masked */

extern void of_oxm_ipv6_nd_target_masked_value_set(
    of_oxm_ipv6_nd_target_masked_t *obj,
    of_ipv6_t value);
extern void of_oxm_ipv6_nd_target_masked_value_get(
    of_oxm_ipv6_nd_target_masked_t *obj,
    of_ipv6_t *value);

extern void of_oxm_ipv6_nd_target_masked_value_mask_set(
    of_oxm_ipv6_nd_target_masked_t *obj,
    of_ipv6_t value_mask);
extern void of_oxm_ipv6_nd_target_masked_value_mask_get(
    of_oxm_ipv6_nd_target_masked_t *obj,
    of_ipv6_t *value_mask);

/* Unified accessor functions for of_oxm_ipv6_nd_tll */

extern void of_oxm_ipv6_nd_tll_value_set(
    of_oxm_ipv6_nd_tll_t *obj,
    of_mac_addr_t value);
extern void of_oxm_ipv6_nd_tll_value_get(
    of_oxm_ipv6_nd_tll_t *obj,
    of_mac_addr_t *value);

/* Unified accessor functions for of_oxm_ipv6_nd_tll_masked */

extern void of_oxm_ipv6_nd_tll_masked_value_set(
    of_oxm_ipv6_nd_tll_masked_t *obj,
    of_mac_addr_t value);
extern void of_oxm_ipv6_nd_tll_masked_value_get(
    of_oxm_ipv6_nd_tll_masked_t *obj,
    of_mac_addr_t *value);

extern void of_oxm_ipv6_nd_tll_masked_value_mask_set(
    of_oxm_ipv6_nd_tll_masked_t *obj,
    of_mac_addr_t value_mask);
extern void of_oxm_ipv6_nd_tll_masked_value_mask_get(
    of_oxm_ipv6_nd_tll_masked_t *obj,
    of_mac_addr_t *value_mask);

/* Unified accessor functions for of_oxm_ipv6_src */

extern void of_oxm_ipv6_src_value_set(
    of_oxm_ipv6_src_t *obj,
    of_ipv6_t value);
extern void of_oxm_ipv6_src_value_get(
    of_oxm_ipv6_src_t *obj,
    of_ipv6_t *value);

/* Unified accessor functions for of_oxm_ipv6_src_masked */

extern void of_oxm_ipv6_src_masked_value_set(
    of_oxm_ipv6_src_masked_t *obj,
    of_ipv6_t value);
extern void of_oxm_ipv6_src_masked_value_get(
    of_oxm_ipv6_src_masked_t *obj,
    of_ipv6_t *value);

extern void of_oxm_ipv6_src_masked_value_mask_set(
    of_oxm_ipv6_src_masked_t *obj,
    of_ipv6_t value_mask);
extern void of_oxm_ipv6_src_masked_value_mask_get(
    of_oxm_ipv6_src_masked_t *obj,
    of_ipv6_t *value_mask);

/* Unified accessor functions for of_oxm_metadata */

extern void of_oxm_metadata_value_set(
    of_oxm_metadata_t *obj,
    uint64_t value);
extern void of_oxm_metadata_value_get(
    of_oxm_metadata_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_oxm_metadata_masked */

extern void of_oxm_metadata_masked_value_set(
    of_oxm_metadata_masked_t *obj,
    uint64_t value);
extern void of_oxm_metadata_masked_value_get(
    of_oxm_metadata_masked_t *obj,
    uint64_t *value);

extern void of_oxm_metadata_masked_value_mask_set(
    of_oxm_metadata_masked_t *obj,
    uint64_t value_mask);
extern void of_oxm_metadata_masked_value_mask_get(
    of_oxm_metadata_masked_t *obj,
    uint64_t *value_mask);

/* Unified accessor functions for of_oxm_mpls_bos */

extern void of_oxm_mpls_bos_value_set(
    of_oxm_mpls_bos_t *obj,
    uint8_t value);
extern void of_oxm_mpls_bos_value_get(
    of_oxm_mpls_bos_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_mpls_bos_masked */

extern void of_oxm_mpls_bos_masked_value_set(
    of_oxm_mpls_bos_masked_t *obj,
    uint8_t value);
extern void of_oxm_mpls_bos_masked_value_get(
    of_oxm_mpls_bos_masked_t *obj,
    uint8_t *value);

extern void of_oxm_mpls_bos_masked_value_mask_set(
    of_oxm_mpls_bos_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_mpls_bos_masked_value_mask_get(
    of_oxm_mpls_bos_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_mpls_label */

extern void of_oxm_mpls_label_value_set(
    of_oxm_mpls_label_t *obj,
    uint32_t value);
extern void of_oxm_mpls_label_value_get(
    of_oxm_mpls_label_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_mpls_label_masked */

extern void of_oxm_mpls_label_masked_value_set(
    of_oxm_mpls_label_masked_t *obj,
    uint32_t value);
extern void of_oxm_mpls_label_masked_value_get(
    of_oxm_mpls_label_masked_t *obj,
    uint32_t *value);

extern void of_oxm_mpls_label_masked_value_mask_set(
    of_oxm_mpls_label_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_mpls_label_masked_value_mask_get(
    of_oxm_mpls_label_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_mpls_tc */

extern void of_oxm_mpls_tc_value_set(
    of_oxm_mpls_tc_t *obj,
    uint8_t value);
extern void of_oxm_mpls_tc_value_get(
    of_oxm_mpls_tc_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_mpls_tc_masked */

extern void of_oxm_mpls_tc_masked_value_set(
    of_oxm_mpls_tc_masked_t *obj,
    uint8_t value);
extern void of_oxm_mpls_tc_masked_value_get(
    of_oxm_mpls_tc_masked_t *obj,
    uint8_t *value);

extern void of_oxm_mpls_tc_masked_value_mask_set(
    of_oxm_mpls_tc_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_mpls_tc_masked_value_mask_get(
    of_oxm_mpls_tc_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_ofdpa_allow_vlan_translation */

extern void of_oxm_ofdpa_allow_vlan_translation_experimenter_set(
    of_oxm_ofdpa_allow_vlan_translation_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_allow_vlan_translation_experimenter_get(
    of_oxm_ofdpa_allow_vlan_translation_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_allow_vlan_translation_value_set(
    of_oxm_ofdpa_allow_vlan_translation_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_allow_vlan_translation_value_get(
    of_oxm_ofdpa_allow_vlan_translation_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_color */

extern void of_oxm_ofdpa_color_experimenter_set(
    of_oxm_ofdpa_color_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_color_experimenter_get(
    of_oxm_ofdpa_color_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_color_value_set(
    of_oxm_ofdpa_color_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_color_value_get(
    of_oxm_ofdpa_color_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_color_actions_index */

extern void of_oxm_ofdpa_color_actions_index_experimenter_set(
    of_oxm_ofdpa_color_actions_index_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_color_actions_index_experimenter_get(
    of_oxm_ofdpa_color_actions_index_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_color_actions_index_value_set(
    of_oxm_ofdpa_color_actions_index_t *obj,
    uint32_t value);
extern void of_oxm_ofdpa_color_actions_index_value_get(
    of_oxm_ofdpa_color_actions_index_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_ofdpa_dei */

extern void of_oxm_ofdpa_dei_experimenter_set(
    of_oxm_ofdpa_dei_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_dei_experimenter_get(
    of_oxm_ofdpa_dei_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_dei_value_set(
    of_oxm_ofdpa_dei_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_dei_value_get(
    of_oxm_ofdpa_dei_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_l3_in_port */

extern void of_oxm_ofdpa_l3_in_port_experimenter_set(
    of_oxm_ofdpa_l3_in_port_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_l3_in_port_experimenter_get(
    of_oxm_ofdpa_l3_in_port_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_l3_in_port_value_set(
    of_oxm_ofdpa_l3_in_port_t *obj,
    uint32_t value);
extern void of_oxm_ofdpa_l3_in_port_value_get(
    of_oxm_ofdpa_l3_in_port_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_ofdpa_lmep_id */

extern void of_oxm_ofdpa_lmep_id_experimenter_set(
    of_oxm_ofdpa_lmep_id_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_lmep_id_experimenter_get(
    of_oxm_ofdpa_lmep_id_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_lmep_id_value_set(
    of_oxm_ofdpa_lmep_id_t *obj,
    uint32_t value);
extern void of_oxm_ofdpa_lmep_id_value_get(
    of_oxm_ofdpa_lmep_id_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_ofdpa_mpls_ach_channel */

extern void of_oxm_ofdpa_mpls_ach_channel_experimenter_set(
    of_oxm_ofdpa_mpls_ach_channel_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_mpls_ach_channel_experimenter_get(
    of_oxm_ofdpa_mpls_ach_channel_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_mpls_ach_channel_value_set(
    of_oxm_ofdpa_mpls_ach_channel_t *obj,
    uint16_t value);
extern void of_oxm_ofdpa_mpls_ach_channel_value_get(
    of_oxm_ofdpa_mpls_ach_channel_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_ofdpa_mpls_data_first_nibble */

extern void of_oxm_ofdpa_mpls_data_first_nibble_experimenter_set(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_mpls_data_first_nibble_experimenter_get(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_mpls_data_first_nibble_value_set(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_mpls_data_first_nibble_value_get(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_mpls_l2_port */

extern void of_oxm_ofdpa_mpls_l2_port_experimenter_set(
    of_oxm_ofdpa_mpls_l2_port_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_mpls_l2_port_experimenter_get(
    of_oxm_ofdpa_mpls_l2_port_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_mpls_l2_port_value_set(
    of_oxm_ofdpa_mpls_l2_port_t *obj,
    uint32_t value);
extern void of_oxm_ofdpa_mpls_l2_port_value_get(
    of_oxm_ofdpa_mpls_l2_port_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_oxm_ofdpa_mpls_l2_port_masked */

extern void of_oxm_ofdpa_mpls_l2_port_masked_experimenter_set(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_mpls_l2_port_masked_experimenter_get(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_mpls_l2_port_masked_value_set(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t value);
extern void of_oxm_ofdpa_mpls_l2_port_masked_value_get(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t *value);

extern void of_oxm_ofdpa_mpls_l2_port_masked_value_mask_set(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t value_mask);
extern void of_oxm_ofdpa_mpls_l2_port_masked_value_mask_get(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t *value_mask);

/* Unified accessor functions for of_oxm_ofdpa_mpls_next_label_is_gal */

extern void of_oxm_ofdpa_mpls_next_label_is_gal_experimenter_set(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_mpls_next_label_is_gal_experimenter_get(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_mpls_next_label_is_gal_value_set(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_mpls_next_label_is_gal_value_get(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_mpls_ttl */

extern void of_oxm_ofdpa_mpls_ttl_experimenter_set(
    of_oxm_ofdpa_mpls_ttl_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_mpls_ttl_experimenter_get(
    of_oxm_ofdpa_mpls_ttl_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_mpls_ttl_value_set(
    of_oxm_ofdpa_mpls_ttl_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_mpls_ttl_value_get(
    of_oxm_ofdpa_mpls_ttl_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_mpls_type */

extern void of_oxm_ofdpa_mpls_type_experimenter_set(
    of_oxm_ofdpa_mpls_type_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_mpls_type_experimenter_get(
    of_oxm_ofdpa_mpls_type_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_mpls_type_value_set(
    of_oxm_ofdpa_mpls_type_t *obj,
    uint16_t value);
extern void of_oxm_ofdpa_mpls_type_value_get(
    of_oxm_ofdpa_mpls_type_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_ofdpa_oam_y1731_mdl */

extern void of_oxm_ofdpa_oam_y1731_mdl_experimenter_set(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_oam_y1731_mdl_experimenter_get(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_oam_y1731_mdl_value_set(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_oam_y1731_mdl_value_get(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_oam_y1731_opcode */

extern void of_oxm_ofdpa_oam_y1731_opcode_experimenter_set(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_oam_y1731_opcode_experimenter_get(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_oam_y1731_opcode_value_set(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_oam_y1731_opcode_value_get(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_ovid */

extern void of_oxm_ofdpa_ovid_experimenter_set(
    of_oxm_ofdpa_ovid_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_ovid_experimenter_get(
    of_oxm_ofdpa_ovid_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_ovid_value_set(
    of_oxm_ofdpa_ovid_t *obj,
    uint16_t value);
extern void of_oxm_ofdpa_ovid_value_get(
    of_oxm_ofdpa_ovid_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_ofdpa_protection_index */

extern void of_oxm_ofdpa_protection_index_experimenter_set(
    of_oxm_ofdpa_protection_index_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_protection_index_experimenter_get(
    of_oxm_ofdpa_protection_index_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_protection_index_value_set(
    of_oxm_ofdpa_protection_index_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_protection_index_value_get(
    of_oxm_ofdpa_protection_index_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_qos_index */

extern void of_oxm_ofdpa_qos_index_experimenter_set(
    of_oxm_ofdpa_qos_index_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_qos_index_experimenter_get(
    of_oxm_ofdpa_qos_index_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_qos_index_value_set(
    of_oxm_ofdpa_qos_index_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_qos_index_value_get(
    of_oxm_ofdpa_qos_index_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_rx_timestamp */

extern void of_oxm_ofdpa_rx_timestamp_experimenter_set(
    of_oxm_ofdpa_rx_timestamp_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_rx_timestamp_experimenter_get(
    of_oxm_ofdpa_rx_timestamp_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_rx_timestamp_value_set(
    of_oxm_ofdpa_rx_timestamp_t *obj,
    uint64_t value);
extern void of_oxm_ofdpa_rx_timestamp_value_get(
    of_oxm_ofdpa_rx_timestamp_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_oxm_ofdpa_rxfcl */

extern void of_oxm_ofdpa_rxfcl_experimenter_set(
    of_oxm_ofdpa_rxfcl_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_rxfcl_experimenter_get(
    of_oxm_ofdpa_rxfcl_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_rxfcl_value_set(
    of_oxm_ofdpa_rxfcl_t *obj,
    uint64_t value);
extern void of_oxm_ofdpa_rxfcl_value_get(
    of_oxm_ofdpa_rxfcl_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_oxm_ofdpa_traffic_class */

extern void of_oxm_ofdpa_traffic_class_experimenter_set(
    of_oxm_ofdpa_traffic_class_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_traffic_class_experimenter_get(
    of_oxm_ofdpa_traffic_class_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_traffic_class_value_set(
    of_oxm_ofdpa_traffic_class_t *obj,
    uint8_t value);
extern void of_oxm_ofdpa_traffic_class_value_get(
    of_oxm_ofdpa_traffic_class_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_ofdpa_txfcl */

extern void of_oxm_ofdpa_txfcl_experimenter_set(
    of_oxm_ofdpa_txfcl_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_txfcl_experimenter_get(
    of_oxm_ofdpa_txfcl_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_txfcl_value_set(
    of_oxm_ofdpa_txfcl_t *obj,
    uint64_t value);
extern void of_oxm_ofdpa_txfcl_value_get(
    of_oxm_ofdpa_txfcl_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_oxm_ofdpa_vrf */

extern void of_oxm_ofdpa_vrf_experimenter_set(
    of_oxm_ofdpa_vrf_t *obj,
    uint32_t experimenter);
extern void of_oxm_ofdpa_vrf_experimenter_get(
    of_oxm_ofdpa_vrf_t *obj,
    uint32_t *experimenter);

extern void of_oxm_ofdpa_vrf_value_set(
    of_oxm_ofdpa_vrf_t *obj,
    uint16_t value);
extern void of_oxm_ofdpa_vrf_value_get(
    of_oxm_ofdpa_vrf_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_onf_actset_output */

extern void of_oxm_onf_actset_output_experimenter_set(
    of_oxm_onf_actset_output_t *obj,
    uint32_t experimenter);
extern void of_oxm_onf_actset_output_experimenter_get(
    of_oxm_onf_actset_output_t *obj,
    uint32_t *experimenter);

extern void of_oxm_onf_actset_output_value_set(
    of_oxm_onf_actset_output_t *obj,
    uint64_t value);
extern void of_oxm_onf_actset_output_value_get(
    of_oxm_onf_actset_output_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_oxm_sctp_dst */

extern void of_oxm_sctp_dst_value_set(
    of_oxm_sctp_dst_t *obj,
    uint16_t value);
extern void of_oxm_sctp_dst_value_get(
    of_oxm_sctp_dst_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_sctp_dst_masked */

extern void of_oxm_sctp_dst_masked_value_set(
    of_oxm_sctp_dst_masked_t *obj,
    uint16_t value);
extern void of_oxm_sctp_dst_masked_value_get(
    of_oxm_sctp_dst_masked_t *obj,
    uint16_t *value);

extern void of_oxm_sctp_dst_masked_value_mask_set(
    of_oxm_sctp_dst_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_sctp_dst_masked_value_mask_get(
    of_oxm_sctp_dst_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_oxm_sctp_src */

extern void of_oxm_sctp_src_value_set(
    of_oxm_sctp_src_t *obj,
    uint16_t value);
extern void of_oxm_sctp_src_value_get(
    of_oxm_sctp_src_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_sctp_src_masked */

extern void of_oxm_sctp_src_masked_value_set(
    of_oxm_sctp_src_masked_t *obj,
    uint16_t value);
extern void of_oxm_sctp_src_masked_value_get(
    of_oxm_sctp_src_masked_t *obj,
    uint16_t *value);

extern void of_oxm_sctp_src_masked_value_mask_set(
    of_oxm_sctp_src_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_sctp_src_masked_value_mask_get(
    of_oxm_sctp_src_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_oxm_tcp_dst */

extern void of_oxm_tcp_dst_value_set(
    of_oxm_tcp_dst_t *obj,
    uint16_t value);
extern void of_oxm_tcp_dst_value_get(
    of_oxm_tcp_dst_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_tcp_dst_masked */

extern void of_oxm_tcp_dst_masked_value_set(
    of_oxm_tcp_dst_masked_t *obj,
    uint16_t value);
extern void of_oxm_tcp_dst_masked_value_get(
    of_oxm_tcp_dst_masked_t *obj,
    uint16_t *value);

extern void of_oxm_tcp_dst_masked_value_mask_set(
    of_oxm_tcp_dst_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_tcp_dst_masked_value_mask_get(
    of_oxm_tcp_dst_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_oxm_tcp_src */

extern void of_oxm_tcp_src_value_set(
    of_oxm_tcp_src_t *obj,
    uint16_t value);
extern void of_oxm_tcp_src_value_get(
    of_oxm_tcp_src_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_tcp_src_masked */

extern void of_oxm_tcp_src_masked_value_set(
    of_oxm_tcp_src_masked_t *obj,
    uint16_t value);
extern void of_oxm_tcp_src_masked_value_get(
    of_oxm_tcp_src_masked_t *obj,
    uint16_t *value);

extern void of_oxm_tcp_src_masked_value_mask_set(
    of_oxm_tcp_src_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_tcp_src_masked_value_mask_get(
    of_oxm_tcp_src_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_oxm_tunnel_id */

extern void of_oxm_tunnel_id_value_set(
    of_oxm_tunnel_id_t *obj,
    uint64_t value);
extern void of_oxm_tunnel_id_value_get(
    of_oxm_tunnel_id_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_oxm_tunnel_id_masked */

extern void of_oxm_tunnel_id_masked_value_set(
    of_oxm_tunnel_id_masked_t *obj,
    uint64_t value);
extern void of_oxm_tunnel_id_masked_value_get(
    of_oxm_tunnel_id_masked_t *obj,
    uint64_t *value);

extern void of_oxm_tunnel_id_masked_value_mask_set(
    of_oxm_tunnel_id_masked_t *obj,
    uint64_t value_mask);
extern void of_oxm_tunnel_id_masked_value_mask_get(
    of_oxm_tunnel_id_masked_t *obj,
    uint64_t *value_mask);

/* Unified accessor functions for of_oxm_udp_dst */

extern void of_oxm_udp_dst_value_set(
    of_oxm_udp_dst_t *obj,
    uint16_t value);
extern void of_oxm_udp_dst_value_get(
    of_oxm_udp_dst_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_udp_dst_masked */

extern void of_oxm_udp_dst_masked_value_set(
    of_oxm_udp_dst_masked_t *obj,
    uint16_t value);
extern void of_oxm_udp_dst_masked_value_get(
    of_oxm_udp_dst_masked_t *obj,
    uint16_t *value);

extern void of_oxm_udp_dst_masked_value_mask_set(
    of_oxm_udp_dst_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_udp_dst_masked_value_mask_get(
    of_oxm_udp_dst_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_oxm_udp_src */

extern void of_oxm_udp_src_value_set(
    of_oxm_udp_src_t *obj,
    uint16_t value);
extern void of_oxm_udp_src_value_get(
    of_oxm_udp_src_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_udp_src_masked */

extern void of_oxm_udp_src_masked_value_set(
    of_oxm_udp_src_masked_t *obj,
    uint16_t value);
extern void of_oxm_udp_src_masked_value_get(
    of_oxm_udp_src_masked_t *obj,
    uint16_t *value);

extern void of_oxm_udp_src_masked_value_mask_set(
    of_oxm_udp_src_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_udp_src_masked_value_mask_get(
    of_oxm_udp_src_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_oxm_vlan_pcp */

extern void of_oxm_vlan_pcp_value_set(
    of_oxm_vlan_pcp_t *obj,
    uint8_t value);
extern void of_oxm_vlan_pcp_value_get(
    of_oxm_vlan_pcp_t *obj,
    uint8_t *value);

/* Unified accessor functions for of_oxm_vlan_pcp_masked */

extern void of_oxm_vlan_pcp_masked_value_set(
    of_oxm_vlan_pcp_masked_t *obj,
    uint8_t value);
extern void of_oxm_vlan_pcp_masked_value_get(
    of_oxm_vlan_pcp_masked_t *obj,
    uint8_t *value);

extern void of_oxm_vlan_pcp_masked_value_mask_set(
    of_oxm_vlan_pcp_masked_t *obj,
    uint8_t value_mask);
extern void of_oxm_vlan_pcp_masked_value_mask_get(
    of_oxm_vlan_pcp_masked_t *obj,
    uint8_t *value_mask);

/* Unified accessor functions for of_oxm_vlan_vid */

extern void of_oxm_vlan_vid_value_set(
    of_oxm_vlan_vid_t *obj,
    uint16_t value);
extern void of_oxm_vlan_vid_value_get(
    of_oxm_vlan_vid_t *obj,
    uint16_t *value);

/* Unified accessor functions for of_oxm_vlan_vid_masked */

extern void of_oxm_vlan_vid_masked_value_set(
    of_oxm_vlan_vid_masked_t *obj,
    uint16_t value);
extern void of_oxm_vlan_vid_masked_value_get(
    of_oxm_vlan_vid_masked_t *obj,
    uint16_t *value);

extern void of_oxm_vlan_vid_masked_value_mask_set(
    of_oxm_vlan_vid_masked_t *obj,
    uint16_t value_mask);
extern void of_oxm_vlan_vid_masked_value_mask_get(
    of_oxm_vlan_vid_masked_t *obj,
    uint16_t *value_mask);

/* Unified accessor functions for of_packet_queue */

extern void of_packet_queue_queue_id_set(
    of_packet_queue_t *obj,
    uint32_t queue_id);
extern void of_packet_queue_queue_id_get(
    of_packet_queue_t *obj,
    uint32_t *queue_id);

extern void of_packet_queue_port_set(
    of_packet_queue_t *obj,
    of_port_no_t port);
extern void of_packet_queue_port_get(
    of_packet_queue_t *obj,
    of_port_no_t *port);

extern int WARN_UNUSED_RESULT of_packet_queue_properties_set(
    of_packet_queue_t *obj,
    of_list_queue_prop_t *properties);
extern void of_packet_queue_properties_bind(
    of_packet_queue_t *obj,
    of_list_queue_prop_t *properties);
extern of_list_queue_prop_t *of_packet_queue_properties_get(
    of_packet_queue_t *obj);

/* Unified accessor functions for of_port_desc */

extern void of_port_desc_port_no_set(
    of_port_desc_t *obj,
    of_port_no_t port_no);
extern void of_port_desc_port_no_get(
    of_port_desc_t *obj,
    of_port_no_t *port_no);

extern void of_port_desc_hw_addr_set(
    of_port_desc_t *obj,
    of_mac_addr_t hw_addr);
extern void of_port_desc_hw_addr_get(
    of_port_desc_t *obj,
    of_mac_addr_t *hw_addr);

extern void of_port_desc_name_set(
    of_port_desc_t *obj,
    of_port_name_t name);
extern void of_port_desc_name_get(
    of_port_desc_t *obj,
    of_port_name_t *name);

extern void of_port_desc_config_set(
    of_port_desc_t *obj,
    uint32_t config);
extern void of_port_desc_config_get(
    of_port_desc_t *obj,
    uint32_t *config);

extern void of_port_desc_state_set(
    of_port_desc_t *obj,
    uint32_t state);
extern void of_port_desc_state_get(
    of_port_desc_t *obj,
    uint32_t *state);

extern void of_port_desc_curr_set(
    of_port_desc_t *obj,
    uint32_t curr);
extern void of_port_desc_curr_get(
    of_port_desc_t *obj,
    uint32_t *curr);

extern void of_port_desc_advertised_set(
    of_port_desc_t *obj,
    uint32_t advertised);
extern void of_port_desc_advertised_get(
    of_port_desc_t *obj,
    uint32_t *advertised);

extern void of_port_desc_supported_set(
    of_port_desc_t *obj,
    uint32_t supported);
extern void of_port_desc_supported_get(
    of_port_desc_t *obj,
    uint32_t *supported);

extern void of_port_desc_peer_set(
    of_port_desc_t *obj,
    uint32_t peer);
extern void of_port_desc_peer_get(
    of_port_desc_t *obj,
    uint32_t *peer);

extern void of_port_desc_curr_speed_set(
    of_port_desc_t *obj,
    uint32_t curr_speed);
extern void of_port_desc_curr_speed_get(
    of_port_desc_t *obj,
    uint32_t *curr_speed);

extern void of_port_desc_max_speed_set(
    of_port_desc_t *obj,
    uint32_t max_speed);
extern void of_port_desc_max_speed_get(
    of_port_desc_t *obj,
    uint32_t *max_speed);

/* Unified accessor functions for of_port_stats_entry */

extern void of_port_stats_entry_port_no_set(
    of_port_stats_entry_t *obj,
    of_port_no_t port_no);
extern void of_port_stats_entry_port_no_get(
    of_port_stats_entry_t *obj,
    of_port_no_t *port_no);

extern void of_port_stats_entry_rx_packets_set(
    of_port_stats_entry_t *obj,
    uint64_t rx_packets);
extern void of_port_stats_entry_rx_packets_get(
    of_port_stats_entry_t *obj,
    uint64_t *rx_packets);

extern void of_port_stats_entry_tx_packets_set(
    of_port_stats_entry_t *obj,
    uint64_t tx_packets);
extern void of_port_stats_entry_tx_packets_get(
    of_port_stats_entry_t *obj,
    uint64_t *tx_packets);

extern void of_port_stats_entry_rx_bytes_set(
    of_port_stats_entry_t *obj,
    uint64_t rx_bytes);
extern void of_port_stats_entry_rx_bytes_get(
    of_port_stats_entry_t *obj,
    uint64_t *rx_bytes);

extern void of_port_stats_entry_tx_bytes_set(
    of_port_stats_entry_t *obj,
    uint64_t tx_bytes);
extern void of_port_stats_entry_tx_bytes_get(
    of_port_stats_entry_t *obj,
    uint64_t *tx_bytes);

extern void of_port_stats_entry_rx_dropped_set(
    of_port_stats_entry_t *obj,
    uint64_t rx_dropped);
extern void of_port_stats_entry_rx_dropped_get(
    of_port_stats_entry_t *obj,
    uint64_t *rx_dropped);

extern void of_port_stats_entry_tx_dropped_set(
    of_port_stats_entry_t *obj,
    uint64_t tx_dropped);
extern void of_port_stats_entry_tx_dropped_get(
    of_port_stats_entry_t *obj,
    uint64_t *tx_dropped);

extern void of_port_stats_entry_rx_errors_set(
    of_port_stats_entry_t *obj,
    uint64_t rx_errors);
extern void of_port_stats_entry_rx_errors_get(
    of_port_stats_entry_t *obj,
    uint64_t *rx_errors);

extern void of_port_stats_entry_tx_errors_set(
    of_port_stats_entry_t *obj,
    uint64_t tx_errors);
extern void of_port_stats_entry_tx_errors_get(
    of_port_stats_entry_t *obj,
    uint64_t *tx_errors);

extern void of_port_stats_entry_rx_frame_err_set(
    of_port_stats_entry_t *obj,
    uint64_t rx_frame_err);
extern void of_port_stats_entry_rx_frame_err_get(
    of_port_stats_entry_t *obj,
    uint64_t *rx_frame_err);

extern void of_port_stats_entry_rx_over_err_set(
    of_port_stats_entry_t *obj,
    uint64_t rx_over_err);
extern void of_port_stats_entry_rx_over_err_get(
    of_port_stats_entry_t *obj,
    uint64_t *rx_over_err);

extern void of_port_stats_entry_rx_crc_err_set(
    of_port_stats_entry_t *obj,
    uint64_t rx_crc_err);
extern void of_port_stats_entry_rx_crc_err_get(
    of_port_stats_entry_t *obj,
    uint64_t *rx_crc_err);

extern void of_port_stats_entry_collisions_set(
    of_port_stats_entry_t *obj,
    uint64_t collisions);
extern void of_port_stats_entry_collisions_get(
    of_port_stats_entry_t *obj,
    uint64_t *collisions);

extern void of_port_stats_entry_duration_sec_set(
    of_port_stats_entry_t *obj,
    uint32_t duration_sec);
extern void of_port_stats_entry_duration_sec_get(
    of_port_stats_entry_t *obj,
    uint32_t *duration_sec);

extern void of_port_stats_entry_duration_nsec_set(
    of_port_stats_entry_t *obj,
    uint32_t duration_nsec);
extern void of_port_stats_entry_duration_nsec_get(
    of_port_stats_entry_t *obj,
    uint32_t *duration_nsec);

/* Unified accessor functions for of_queue_prop_experimenter */

extern void of_queue_prop_experimenter_experimenter_set(
    of_queue_prop_experimenter_t *obj,
    uint32_t experimenter);
extern void of_queue_prop_experimenter_experimenter_get(
    of_queue_prop_experimenter_t *obj,
    uint32_t *experimenter);

extern int WARN_UNUSED_RESULT of_queue_prop_experimenter_data_set(
    of_queue_prop_experimenter_t *obj,
    of_octets_t *data);
extern void of_queue_prop_experimenter_data_get(
    of_queue_prop_experimenter_t *obj,
    of_octets_t *data);

/* Unified accessor functions for of_queue_prop_header */

/* Unified accessor functions for of_queue_prop_max_rate */

extern void of_queue_prop_max_rate_rate_set(
    of_queue_prop_max_rate_t *obj,
    uint16_t rate);
extern void of_queue_prop_max_rate_rate_get(
    of_queue_prop_max_rate_t *obj,
    uint16_t *rate);

/* Unified accessor functions for of_queue_prop_min_rate */

extern void of_queue_prop_min_rate_rate_set(
    of_queue_prop_min_rate_t *obj,
    uint16_t rate);
extern void of_queue_prop_min_rate_rate_get(
    of_queue_prop_min_rate_t *obj,
    uint16_t *rate);

/* Unified accessor functions for of_queue_stats_entry */

extern void of_queue_stats_entry_port_no_set(
    of_queue_stats_entry_t *obj,
    of_port_no_t port_no);
extern void of_queue_stats_entry_port_no_get(
    of_queue_stats_entry_t *obj,
    of_port_no_t *port_no);

extern void of_queue_stats_entry_queue_id_set(
    of_queue_stats_entry_t *obj,
    uint32_t queue_id);
extern void of_queue_stats_entry_queue_id_get(
    of_queue_stats_entry_t *obj,
    uint32_t *queue_id);

extern void of_queue_stats_entry_tx_bytes_set(
    of_queue_stats_entry_t *obj,
    uint64_t tx_bytes);
extern void of_queue_stats_entry_tx_bytes_get(
    of_queue_stats_entry_t *obj,
    uint64_t *tx_bytes);

extern void of_queue_stats_entry_tx_packets_set(
    of_queue_stats_entry_t *obj,
    uint64_t tx_packets);
extern void of_queue_stats_entry_tx_packets_get(
    of_queue_stats_entry_t *obj,
    uint64_t *tx_packets);

extern void of_queue_stats_entry_tx_errors_set(
    of_queue_stats_entry_t *obj,
    uint64_t tx_errors);
extern void of_queue_stats_entry_tx_errors_get(
    of_queue_stats_entry_t *obj,
    uint64_t *tx_errors);

extern void of_queue_stats_entry_duration_sec_set(
    of_queue_stats_entry_t *obj,
    uint32_t duration_sec);
extern void of_queue_stats_entry_duration_sec_get(
    of_queue_stats_entry_t *obj,
    uint32_t *duration_sec);

extern void of_queue_stats_entry_duration_nsec_set(
    of_queue_stats_entry_t *obj,
    uint32_t duration_nsec);
extern void of_queue_stats_entry_duration_nsec_get(
    of_queue_stats_entry_t *obj,
    uint32_t *duration_nsec);

/* Unified accessor functions for of_table_feature_prop_apply_actions */

extern int WARN_UNUSED_RESULT of_table_feature_prop_apply_actions_action_ids_set(
    of_table_feature_prop_apply_actions_t *obj,
    of_list_action_id_t *action_ids);
extern void of_table_feature_prop_apply_actions_action_ids_bind(
    of_table_feature_prop_apply_actions_t *obj,
    of_list_action_id_t *action_ids);
extern of_list_action_id_t *of_table_feature_prop_apply_actions_action_ids_get(
    of_table_feature_prop_apply_actions_t *obj);

/* Unified accessor functions for of_table_feature_prop_apply_actions_miss */

extern int WARN_UNUSED_RESULT of_table_feature_prop_apply_actions_miss_action_ids_set(
    of_table_feature_prop_apply_actions_miss_t *obj,
    of_list_action_id_t *action_ids);
extern void of_table_feature_prop_apply_actions_miss_action_ids_bind(
    of_table_feature_prop_apply_actions_miss_t *obj,
    of_list_action_id_t *action_ids);
extern of_list_action_id_t *of_table_feature_prop_apply_actions_miss_action_ids_get(
    of_table_feature_prop_apply_actions_miss_t *obj);

/* Unified accessor functions for of_table_feature_prop_apply_setfield */

extern int WARN_UNUSED_RESULT of_table_feature_prop_apply_setfield_oxm_ids_set(
    of_table_feature_prop_apply_setfield_t *obj,
    of_list_uint32_t *oxm_ids);
extern void of_table_feature_prop_apply_setfield_oxm_ids_bind(
    of_table_feature_prop_apply_setfield_t *obj,
    of_list_uint32_t *oxm_ids);
extern of_list_uint32_t *of_table_feature_prop_apply_setfield_oxm_ids_get(
    of_table_feature_prop_apply_setfield_t *obj);

/* Unified accessor functions for of_table_feature_prop_apply_setfield_miss */

extern int WARN_UNUSED_RESULT of_table_feature_prop_apply_setfield_miss_oxm_ids_set(
    of_table_feature_prop_apply_setfield_miss_t *obj,
    of_list_uint32_t *oxm_ids);
extern void of_table_feature_prop_apply_setfield_miss_oxm_ids_bind(
    of_table_feature_prop_apply_setfield_miss_t *obj,
    of_list_uint32_t *oxm_ids);
extern of_list_uint32_t *of_table_feature_prop_apply_setfield_miss_oxm_ids_get(
    of_table_feature_prop_apply_setfield_miss_t *obj);

/* Unified accessor functions for of_table_feature_prop_experimenter */

extern void of_table_feature_prop_experimenter_experimenter_set(
    of_table_feature_prop_experimenter_t *obj,
    uint32_t experimenter);
extern void of_table_feature_prop_experimenter_experimenter_get(
    of_table_feature_prop_experimenter_t *obj,
    uint32_t *experimenter);

extern void of_table_feature_prop_experimenter_subtype_set(
    of_table_feature_prop_experimenter_t *obj,
    uint32_t subtype);
extern void of_table_feature_prop_experimenter_subtype_get(
    of_table_feature_prop_experimenter_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_table_feature_prop_experimenter_experimenter_data_set(
    of_table_feature_prop_experimenter_t *obj,
    of_octets_t *experimenter_data);
extern void of_table_feature_prop_experimenter_experimenter_data_get(
    of_table_feature_prop_experimenter_t *obj,
    of_octets_t *experimenter_data);

/* Unified accessor functions for of_table_feature_prop_experimenter_miss */

extern void of_table_feature_prop_experimenter_miss_experimenter_set(
    of_table_feature_prop_experimenter_miss_t *obj,
    uint32_t experimenter);
extern void of_table_feature_prop_experimenter_miss_experimenter_get(
    of_table_feature_prop_experimenter_miss_t *obj,
    uint32_t *experimenter);

extern void of_table_feature_prop_experimenter_miss_subtype_set(
    of_table_feature_prop_experimenter_miss_t *obj,
    uint32_t subtype);
extern void of_table_feature_prop_experimenter_miss_subtype_get(
    of_table_feature_prop_experimenter_miss_t *obj,
    uint32_t *subtype);

extern int WARN_UNUSED_RESULT of_table_feature_prop_experimenter_miss_experimenter_data_set(
    of_table_feature_prop_experimenter_miss_t *obj,
    of_octets_t *experimenter_data);
extern void of_table_feature_prop_experimenter_miss_experimenter_data_get(
    of_table_feature_prop_experimenter_miss_t *obj,
    of_octets_t *experimenter_data);

/* Unified accessor functions for of_table_feature_prop_header */

/* Unified accessor functions for of_table_feature_prop_instructions */

extern int WARN_UNUSED_RESULT of_table_feature_prop_instructions_instruction_ids_set(
    of_table_feature_prop_instructions_t *obj,
    of_list_instruction_id_t *instruction_ids);
extern void of_table_feature_prop_instructions_instruction_ids_bind(
    of_table_feature_prop_instructions_t *obj,
    of_list_instruction_id_t *instruction_ids);
extern of_list_instruction_id_t *of_table_feature_prop_instructions_instruction_ids_get(
    of_table_feature_prop_instructions_t *obj);

/* Unified accessor functions for of_table_feature_prop_instructions_miss */

extern int WARN_UNUSED_RESULT of_table_feature_prop_instructions_miss_instruction_ids_set(
    of_table_feature_prop_instructions_miss_t *obj,
    of_list_instruction_id_t *instruction_ids);
extern void of_table_feature_prop_instructions_miss_instruction_ids_bind(
    of_table_feature_prop_instructions_miss_t *obj,
    of_list_instruction_id_t *instruction_ids);
extern of_list_instruction_id_t *of_table_feature_prop_instructions_miss_instruction_ids_get(
    of_table_feature_prop_instructions_miss_t *obj);

/* Unified accessor functions for of_table_feature_prop_match */

extern int WARN_UNUSED_RESULT of_table_feature_prop_match_oxm_ids_set(
    of_table_feature_prop_match_t *obj,
    of_list_uint32_t *oxm_ids);
extern void of_table_feature_prop_match_oxm_ids_bind(
    of_table_feature_prop_match_t *obj,
    of_list_uint32_t *oxm_ids);
extern of_list_uint32_t *of_table_feature_prop_match_oxm_ids_get(
    of_table_feature_prop_match_t *obj);

/* Unified accessor functions for of_table_feature_prop_next_tables */

extern int WARN_UNUSED_RESULT of_table_feature_prop_next_tables_next_table_ids_set(
    of_table_feature_prop_next_tables_t *obj,
    of_list_uint8_t *next_table_ids);
extern void of_table_feature_prop_next_tables_next_table_ids_bind(
    of_table_feature_prop_next_tables_t *obj,
    of_list_uint8_t *next_table_ids);
extern of_list_uint8_t *of_table_feature_prop_next_tables_next_table_ids_get(
    of_table_feature_prop_next_tables_t *obj);

/* Unified accessor functions for of_table_feature_prop_next_tables_miss */

extern int WARN_UNUSED_RESULT of_table_feature_prop_next_tables_miss_next_table_ids_set(
    of_table_feature_prop_next_tables_miss_t *obj,
    of_list_uint8_t *next_table_ids);
extern void of_table_feature_prop_next_tables_miss_next_table_ids_bind(
    of_table_feature_prop_next_tables_miss_t *obj,
    of_list_uint8_t *next_table_ids);
extern of_list_uint8_t *of_table_feature_prop_next_tables_miss_next_table_ids_get(
    of_table_feature_prop_next_tables_miss_t *obj);

/* Unified accessor functions for of_table_feature_prop_wildcards */

extern int WARN_UNUSED_RESULT of_table_feature_prop_wildcards_oxm_ids_set(
    of_table_feature_prop_wildcards_t *obj,
    of_list_uint32_t *oxm_ids);
extern void of_table_feature_prop_wildcards_oxm_ids_bind(
    of_table_feature_prop_wildcards_t *obj,
    of_list_uint32_t *oxm_ids);
extern of_list_uint32_t *of_table_feature_prop_wildcards_oxm_ids_get(
    of_table_feature_prop_wildcards_t *obj);

/* Unified accessor functions for of_table_feature_prop_write_actions */

extern int WARN_UNUSED_RESULT of_table_feature_prop_write_actions_action_ids_set(
    of_table_feature_prop_write_actions_t *obj,
    of_list_action_id_t *action_ids);
extern void of_table_feature_prop_write_actions_action_ids_bind(
    of_table_feature_prop_write_actions_t *obj,
    of_list_action_id_t *action_ids);
extern of_list_action_id_t *of_table_feature_prop_write_actions_action_ids_get(
    of_table_feature_prop_write_actions_t *obj);

/* Unified accessor functions for of_table_feature_prop_write_actions_miss */

extern int WARN_UNUSED_RESULT of_table_feature_prop_write_actions_miss_action_ids_set(
    of_table_feature_prop_write_actions_miss_t *obj,
    of_list_action_id_t *action_ids);
extern void of_table_feature_prop_write_actions_miss_action_ids_bind(
    of_table_feature_prop_write_actions_miss_t *obj,
    of_list_action_id_t *action_ids);
extern of_list_action_id_t *of_table_feature_prop_write_actions_miss_action_ids_get(
    of_table_feature_prop_write_actions_miss_t *obj);

/* Unified accessor functions for of_table_feature_prop_write_setfield */

extern int WARN_UNUSED_RESULT of_table_feature_prop_write_setfield_oxm_ids_set(
    of_table_feature_prop_write_setfield_t *obj,
    of_list_uint32_t *oxm_ids);
extern void of_table_feature_prop_write_setfield_oxm_ids_bind(
    of_table_feature_prop_write_setfield_t *obj,
    of_list_uint32_t *oxm_ids);
extern of_list_uint32_t *of_table_feature_prop_write_setfield_oxm_ids_get(
    of_table_feature_prop_write_setfield_t *obj);

/* Unified accessor functions for of_table_feature_prop_write_setfield_miss */

extern int WARN_UNUSED_RESULT of_table_feature_prop_write_setfield_miss_oxm_ids_set(
    of_table_feature_prop_write_setfield_miss_t *obj,
    of_list_uint32_t *oxm_ids);
extern void of_table_feature_prop_write_setfield_miss_oxm_ids_bind(
    of_table_feature_prop_write_setfield_miss_t *obj,
    of_list_uint32_t *oxm_ids);
extern of_list_uint32_t *of_table_feature_prop_write_setfield_miss_oxm_ids_get(
    of_table_feature_prop_write_setfield_miss_t *obj);

/* Unified accessor functions for of_table_features */

extern void of_table_features_table_id_set(
    of_table_features_t *obj,
    uint8_t table_id);
extern void of_table_features_table_id_get(
    of_table_features_t *obj,
    uint8_t *table_id);

extern void of_table_features_name_set(
    of_table_features_t *obj,
    of_table_name_t name);
extern void of_table_features_name_get(
    of_table_features_t *obj,
    of_table_name_t *name);

extern void of_table_features_metadata_match_set(
    of_table_features_t *obj,
    uint64_t metadata_match);
extern void of_table_features_metadata_match_get(
    of_table_features_t *obj,
    uint64_t *metadata_match);

extern void of_table_features_metadata_write_set(
    of_table_features_t *obj,
    uint64_t metadata_write);
extern void of_table_features_metadata_write_get(
    of_table_features_t *obj,
    uint64_t *metadata_write);

extern void of_table_features_config_set(
    of_table_features_t *obj,
    uint32_t config);
extern void of_table_features_config_get(
    of_table_features_t *obj,
    uint32_t *config);

extern void of_table_features_max_entries_set(
    of_table_features_t *obj,
    uint32_t max_entries);
extern void of_table_features_max_entries_get(
    of_table_features_t *obj,
    uint32_t *max_entries);

extern int WARN_UNUSED_RESULT of_table_features_properties_set(
    of_table_features_t *obj,
    of_list_table_feature_prop_t *properties);
extern void of_table_features_properties_bind(
    of_table_features_t *obj,
    of_list_table_feature_prop_t *properties);
extern of_list_table_feature_prop_t *of_table_features_properties_get(
    of_table_features_t *obj);

/* Unified accessor functions for of_table_stats_entry */

extern void of_table_stats_entry_table_id_set(
    of_table_stats_entry_t *obj,
    uint8_t table_id);
extern void of_table_stats_entry_table_id_get(
    of_table_stats_entry_t *obj,
    uint8_t *table_id);

extern void of_table_stats_entry_active_count_set(
    of_table_stats_entry_t *obj,
    uint32_t active_count);
extern void of_table_stats_entry_active_count_get(
    of_table_stats_entry_t *obj,
    uint32_t *active_count);

extern void of_table_stats_entry_lookup_count_set(
    of_table_stats_entry_t *obj,
    uint64_t lookup_count);
extern void of_table_stats_entry_lookup_count_get(
    of_table_stats_entry_t *obj,
    uint64_t *lookup_count);

extern void of_table_stats_entry_matched_count_set(
    of_table_stats_entry_t *obj,
    uint64_t matched_count);
extern void of_table_stats_entry_matched_count_get(
    of_table_stats_entry_t *obj,
    uint64_t *matched_count);

extern void of_table_stats_entry_name_set(
    of_table_stats_entry_t *obj,
    of_table_name_t name);
extern void of_table_stats_entry_name_get(
    of_table_stats_entry_t *obj,
    of_table_name_t *name);

extern void of_table_stats_entry_match_set(
    of_table_stats_entry_t *obj,
    of_match_bmap_t match);
extern void of_table_stats_entry_match_get(
    of_table_stats_entry_t *obj,
    of_match_bmap_t *match);

extern void of_table_stats_entry_wildcards_set(
    of_table_stats_entry_t *obj,
    of_wc_bmap_t wildcards);
extern void of_table_stats_entry_wildcards_get(
    of_table_stats_entry_t *obj,
    of_wc_bmap_t *wildcards);

extern void of_table_stats_entry_write_actions_set(
    of_table_stats_entry_t *obj,
    uint32_t write_actions);
extern void of_table_stats_entry_write_actions_get(
    of_table_stats_entry_t *obj,
    uint32_t *write_actions);

extern void of_table_stats_entry_apply_actions_set(
    of_table_stats_entry_t *obj,
    uint32_t apply_actions);
extern void of_table_stats_entry_apply_actions_get(
    of_table_stats_entry_t *obj,
    uint32_t *apply_actions);

extern void of_table_stats_entry_write_setfields_set(
    of_table_stats_entry_t *obj,
    uint64_t write_setfields);
extern void of_table_stats_entry_write_setfields_get(
    of_table_stats_entry_t *obj,
    uint64_t *write_setfields);

extern void of_table_stats_entry_apply_setfields_set(
    of_table_stats_entry_t *obj,
    uint64_t apply_setfields);
extern void of_table_stats_entry_apply_setfields_get(
    of_table_stats_entry_t *obj,
    uint64_t *apply_setfields);

extern void of_table_stats_entry_metadata_match_set(
    of_table_stats_entry_t *obj,
    uint64_t metadata_match);
extern void of_table_stats_entry_metadata_match_get(
    of_table_stats_entry_t *obj,
    uint64_t *metadata_match);

extern void of_table_stats_entry_metadata_write_set(
    of_table_stats_entry_t *obj,
    uint64_t metadata_write);
extern void of_table_stats_entry_metadata_write_get(
    of_table_stats_entry_t *obj,
    uint64_t *metadata_write);

extern void of_table_stats_entry_instructions_set(
    of_table_stats_entry_t *obj,
    uint32_t instructions);
extern void of_table_stats_entry_instructions_get(
    of_table_stats_entry_t *obj,
    uint32_t *instructions);

extern void of_table_stats_entry_config_set(
    of_table_stats_entry_t *obj,
    uint32_t config);
extern void of_table_stats_entry_config_get(
    of_table_stats_entry_t *obj,
    uint32_t *config);

extern void of_table_stats_entry_max_entries_set(
    of_table_stats_entry_t *obj,
    uint32_t max_entries);
extern void of_table_stats_entry_max_entries_get(
    of_table_stats_entry_t *obj,
    uint32_t *max_entries);

/* Unified accessor functions for of_uint32 */

extern void of_uint32_value_set(
    of_uint32_t *obj,
    uint32_t value);
extern void of_uint32_value_get(
    of_uint32_t *obj,
    uint32_t *value);

/* Unified accessor functions for of_uint64 */

extern void of_uint64_value_set(
    of_uint64_t *obj,
    uint64_t value);
extern void of_uint64_value_get(
    of_uint64_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_uint8 */

extern void of_uint8_value_set(
    of_uint8_t *obj,
    uint8_t value);
extern void of_uint8_value_get(
    of_uint8_t *obj,
    uint8_t *value);

/* Unified accessor functions for ofp_queue_desc */

extern void ofp_queue_desc_port_no_set(
    ofp_queue_desc_t *obj,
    uint32_t port_no);
extern void ofp_queue_desc_port_no_get(
    ofp_queue_desc_t *obj,
    uint32_t *port_no);

extern void ofp_queue_desc_queue_id_set(
    ofp_queue_desc_t *obj,
    uint32_t queue_id);
extern void ofp_queue_desc_queue_id_get(
    ofp_queue_desc_t *obj,
    uint32_t *queue_id);

/* Unified accessor functions for onf_oxm_packet_regs0 */

extern void onf_oxm_packet_regs0_value_set(
    onf_oxm_packet_regs0_t *obj,
    uint64_t value);
extern void onf_oxm_packet_regs0_value_get(
    onf_oxm_packet_regs0_t *obj,
    uint64_t *value);

/* Unified accessor functions for onf_oxm_packet_regs1 */

extern void onf_oxm_packet_regs1_value_set(
    onf_oxm_packet_regs1_t *obj,
    uint64_t value);
extern void onf_oxm_packet_regs1_value_get(
    onf_oxm_packet_regs1_t *obj,
    uint64_t *value);

/* Unified accessor functions for onf_oxm_packet_regs2 */

extern void onf_oxm_packet_regs2_value_set(
    onf_oxm_packet_regs2_t *obj,
    uint64_t value);
extern void onf_oxm_packet_regs2_value_get(
    onf_oxm_packet_regs2_t *obj,
    uint64_t *value);

/* Unified accessor functions for onf_oxm_packet_regs3 */

extern void onf_oxm_packet_regs3_value_set(
    onf_oxm_packet_regs3_t *obj,
    uint64_t value);
extern void onf_oxm_packet_regs3_value_get(
    onf_oxm_packet_regs3_t *obj,
    uint64_t *value);

/* Unified accessor functions for of_list_action */

extern int of_list_action_first(
    of_list_action_t *list, of_action_t *obj);
extern int of_list_action_next(
    of_list_action_t *list, of_action_t *obj);
extern int of_list_action_append_bind(
    of_list_action_t *list, of_action_t *obj);
extern int of_list_action_append(
    of_list_action_t *list, of_action_t *obj);

/**
 * Iteration macro for list of type of_list_action
 * @param list Pointer to the list being iterated over of
 * type of_list_action
 * @param elt Pointer to an element of type of_action
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_ACTION_ITER(list, elt, rv)  \
    for ((rv) = of_list_action_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_action_next((list), (elt)))

/* Unified accessor functions for of_list_action_id */

extern int of_list_action_id_first(
    of_list_action_id_t *list, of_action_id_t *obj);
extern int of_list_action_id_next(
    of_list_action_id_t *list, of_action_id_t *obj);
extern int of_list_action_id_append_bind(
    of_list_action_id_t *list, of_action_id_t *obj);
extern int of_list_action_id_append(
    of_list_action_id_t *list, of_action_id_t *obj);

/**
 * Iteration macro for list of type of_list_action_id
 * @param list Pointer to the list being iterated over of
 * type of_list_action_id
 * @param elt Pointer to an element of type of_action_id
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_ACTION_ID_ITER(list, elt, rv)  \
    for ((rv) = of_list_action_id_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_action_id_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_controller_connection */

extern int of_list_bsn_controller_connection_first(
    of_list_bsn_controller_connection_t *list, of_bsn_controller_connection_t *obj);
extern int of_list_bsn_controller_connection_next(
    of_list_bsn_controller_connection_t *list, of_bsn_controller_connection_t *obj);
extern int of_list_bsn_controller_connection_append_bind(
    of_list_bsn_controller_connection_t *list, of_bsn_controller_connection_t *obj);
extern int of_list_bsn_controller_connection_append(
    of_list_bsn_controller_connection_t *list, of_bsn_controller_connection_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_controller_connection
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_controller_connection
 * @param elt Pointer to an element of type of_bsn_controller_connection
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_CONTROLLER_CONNECTION_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_controller_connection_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_controller_connection_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_debug_counter_desc_stats_entry */

extern int of_list_bsn_debug_counter_desc_stats_entry_first(
    of_list_bsn_debug_counter_desc_stats_entry_t *list, of_bsn_debug_counter_desc_stats_entry_t *obj);
extern int of_list_bsn_debug_counter_desc_stats_entry_next(
    of_list_bsn_debug_counter_desc_stats_entry_t *list, of_bsn_debug_counter_desc_stats_entry_t *obj);
extern int of_list_bsn_debug_counter_desc_stats_entry_append_bind(
    of_list_bsn_debug_counter_desc_stats_entry_t *list, of_bsn_debug_counter_desc_stats_entry_t *obj);
extern int of_list_bsn_debug_counter_desc_stats_entry_append(
    of_list_bsn_debug_counter_desc_stats_entry_t *list, of_bsn_debug_counter_desc_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_debug_counter_desc_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_debug_counter_desc_stats_entry
 * @param elt Pointer to an element of type of_bsn_debug_counter_desc_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_DEBUG_COUNTER_DESC_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_debug_counter_desc_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_debug_counter_desc_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_debug_counter_stats_entry */

extern int of_list_bsn_debug_counter_stats_entry_first(
    of_list_bsn_debug_counter_stats_entry_t *list, of_bsn_debug_counter_stats_entry_t *obj);
extern int of_list_bsn_debug_counter_stats_entry_next(
    of_list_bsn_debug_counter_stats_entry_t *list, of_bsn_debug_counter_stats_entry_t *obj);
extern int of_list_bsn_debug_counter_stats_entry_append_bind(
    of_list_bsn_debug_counter_stats_entry_t *list, of_bsn_debug_counter_stats_entry_t *obj);
extern int of_list_bsn_debug_counter_stats_entry_append(
    of_list_bsn_debug_counter_stats_entry_t *list, of_bsn_debug_counter_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_debug_counter_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_debug_counter_stats_entry
 * @param elt Pointer to an element of type of_bsn_debug_counter_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_DEBUG_COUNTER_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_debug_counter_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_debug_counter_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_flow_checksum_bucket_stats_entry */

extern int of_list_bsn_flow_checksum_bucket_stats_entry_first(
    of_list_bsn_flow_checksum_bucket_stats_entry_t *list, of_bsn_flow_checksum_bucket_stats_entry_t *obj);
extern int of_list_bsn_flow_checksum_bucket_stats_entry_next(
    of_list_bsn_flow_checksum_bucket_stats_entry_t *list, of_bsn_flow_checksum_bucket_stats_entry_t *obj);
extern int of_list_bsn_flow_checksum_bucket_stats_entry_append_bind(
    of_list_bsn_flow_checksum_bucket_stats_entry_t *list, of_bsn_flow_checksum_bucket_stats_entry_t *obj);
extern int of_list_bsn_flow_checksum_bucket_stats_entry_append(
    of_list_bsn_flow_checksum_bucket_stats_entry_t *list, of_bsn_flow_checksum_bucket_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_flow_checksum_bucket_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_flow_checksum_bucket_stats_entry
 * @param elt Pointer to an element of type of_bsn_flow_checksum_bucket_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_FLOW_CHECKSUM_BUCKET_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_flow_checksum_bucket_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_flow_checksum_bucket_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_gentable_bucket_stats_entry */

extern int of_list_bsn_gentable_bucket_stats_entry_first(
    of_list_bsn_gentable_bucket_stats_entry_t *list, of_bsn_gentable_bucket_stats_entry_t *obj);
extern int of_list_bsn_gentable_bucket_stats_entry_next(
    of_list_bsn_gentable_bucket_stats_entry_t *list, of_bsn_gentable_bucket_stats_entry_t *obj);
extern int of_list_bsn_gentable_bucket_stats_entry_append_bind(
    of_list_bsn_gentable_bucket_stats_entry_t *list, of_bsn_gentable_bucket_stats_entry_t *obj);
extern int of_list_bsn_gentable_bucket_stats_entry_append(
    of_list_bsn_gentable_bucket_stats_entry_t *list, of_bsn_gentable_bucket_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_gentable_bucket_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_gentable_bucket_stats_entry
 * @param elt Pointer to an element of type of_bsn_gentable_bucket_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_GENTABLE_BUCKET_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_gentable_bucket_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_gentable_bucket_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_gentable_desc_stats_entry */

extern int of_list_bsn_gentable_desc_stats_entry_first(
    of_list_bsn_gentable_desc_stats_entry_t *list, of_bsn_gentable_desc_stats_entry_t *obj);
extern int of_list_bsn_gentable_desc_stats_entry_next(
    of_list_bsn_gentable_desc_stats_entry_t *list, of_bsn_gentable_desc_stats_entry_t *obj);
extern int of_list_bsn_gentable_desc_stats_entry_append_bind(
    of_list_bsn_gentable_desc_stats_entry_t *list, of_bsn_gentable_desc_stats_entry_t *obj);
extern int of_list_bsn_gentable_desc_stats_entry_append(
    of_list_bsn_gentable_desc_stats_entry_t *list, of_bsn_gentable_desc_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_gentable_desc_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_gentable_desc_stats_entry
 * @param elt Pointer to an element of type of_bsn_gentable_desc_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_GENTABLE_DESC_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_gentable_desc_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_gentable_desc_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_gentable_entry_desc_stats_entry */

extern int of_list_bsn_gentable_entry_desc_stats_entry_first(
    of_list_bsn_gentable_entry_desc_stats_entry_t *list, of_bsn_gentable_entry_desc_stats_entry_t *obj);
extern int of_list_bsn_gentable_entry_desc_stats_entry_next(
    of_list_bsn_gentable_entry_desc_stats_entry_t *list, of_bsn_gentable_entry_desc_stats_entry_t *obj);
extern int of_list_bsn_gentable_entry_desc_stats_entry_append_bind(
    of_list_bsn_gentable_entry_desc_stats_entry_t *list, of_bsn_gentable_entry_desc_stats_entry_t *obj);
extern int of_list_bsn_gentable_entry_desc_stats_entry_append(
    of_list_bsn_gentable_entry_desc_stats_entry_t *list, of_bsn_gentable_entry_desc_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_gentable_entry_desc_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_gentable_entry_desc_stats_entry
 * @param elt Pointer to an element of type of_bsn_gentable_entry_desc_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_GENTABLE_ENTRY_DESC_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_gentable_entry_desc_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_gentable_entry_desc_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_gentable_entry_stats_entry */

extern int of_list_bsn_gentable_entry_stats_entry_first(
    of_list_bsn_gentable_entry_stats_entry_t *list, of_bsn_gentable_entry_stats_entry_t *obj);
extern int of_list_bsn_gentable_entry_stats_entry_next(
    of_list_bsn_gentable_entry_stats_entry_t *list, of_bsn_gentable_entry_stats_entry_t *obj);
extern int of_list_bsn_gentable_entry_stats_entry_append_bind(
    of_list_bsn_gentable_entry_stats_entry_t *list, of_bsn_gentable_entry_stats_entry_t *obj);
extern int of_list_bsn_gentable_entry_stats_entry_append(
    of_list_bsn_gentable_entry_stats_entry_t *list, of_bsn_gentable_entry_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_gentable_entry_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_gentable_entry_stats_entry
 * @param elt Pointer to an element of type of_bsn_gentable_entry_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_GENTABLE_ENTRY_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_gentable_entry_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_gentable_entry_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_gentable_stats_entry */

extern int of_list_bsn_gentable_stats_entry_first(
    of_list_bsn_gentable_stats_entry_t *list, of_bsn_gentable_stats_entry_t *obj);
extern int of_list_bsn_gentable_stats_entry_next(
    of_list_bsn_gentable_stats_entry_t *list, of_bsn_gentable_stats_entry_t *obj);
extern int of_list_bsn_gentable_stats_entry_append_bind(
    of_list_bsn_gentable_stats_entry_t *list, of_bsn_gentable_stats_entry_t *obj);
extern int of_list_bsn_gentable_stats_entry_append(
    of_list_bsn_gentable_stats_entry_t *list, of_bsn_gentable_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_gentable_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_gentable_stats_entry
 * @param elt Pointer to an element of type of_bsn_gentable_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_GENTABLE_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_gentable_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_gentable_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_interface */

extern int of_list_bsn_interface_first(
    of_list_bsn_interface_t *list, of_bsn_interface_t *obj);
extern int of_list_bsn_interface_next(
    of_list_bsn_interface_t *list, of_bsn_interface_t *obj);
extern int of_list_bsn_interface_append_bind(
    of_list_bsn_interface_t *list, of_bsn_interface_t *obj);
extern int of_list_bsn_interface_append(
    of_list_bsn_interface_t *list, of_bsn_interface_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_interface
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_interface
 * @param elt Pointer to an element of type of_bsn_interface
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_INTERFACE_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_interface_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_interface_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_lacp_stats_entry */

extern int of_list_bsn_lacp_stats_entry_first(
    of_list_bsn_lacp_stats_entry_t *list, of_bsn_lacp_stats_entry_t *obj);
extern int of_list_bsn_lacp_stats_entry_next(
    of_list_bsn_lacp_stats_entry_t *list, of_bsn_lacp_stats_entry_t *obj);
extern int of_list_bsn_lacp_stats_entry_append_bind(
    of_list_bsn_lacp_stats_entry_t *list, of_bsn_lacp_stats_entry_t *obj);
extern int of_list_bsn_lacp_stats_entry_append(
    of_list_bsn_lacp_stats_entry_t *list, of_bsn_lacp_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_lacp_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_lacp_stats_entry
 * @param elt Pointer to an element of type of_bsn_lacp_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_LACP_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_lacp_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_lacp_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_port_counter_stats_entry */

extern int of_list_bsn_port_counter_stats_entry_first(
    of_list_bsn_port_counter_stats_entry_t *list, of_bsn_port_counter_stats_entry_t *obj);
extern int of_list_bsn_port_counter_stats_entry_next(
    of_list_bsn_port_counter_stats_entry_t *list, of_bsn_port_counter_stats_entry_t *obj);
extern int of_list_bsn_port_counter_stats_entry_append_bind(
    of_list_bsn_port_counter_stats_entry_t *list, of_bsn_port_counter_stats_entry_t *obj);
extern int of_list_bsn_port_counter_stats_entry_append(
    of_list_bsn_port_counter_stats_entry_t *list, of_bsn_port_counter_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_port_counter_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_port_counter_stats_entry
 * @param elt Pointer to an element of type of_bsn_port_counter_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_PORT_COUNTER_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_port_counter_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_port_counter_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_switch_pipeline_stats_entry */

extern int of_list_bsn_switch_pipeline_stats_entry_first(
    of_list_bsn_switch_pipeline_stats_entry_t *list, of_bsn_switch_pipeline_stats_entry_t *obj);
extern int of_list_bsn_switch_pipeline_stats_entry_next(
    of_list_bsn_switch_pipeline_stats_entry_t *list, of_bsn_switch_pipeline_stats_entry_t *obj);
extern int of_list_bsn_switch_pipeline_stats_entry_append_bind(
    of_list_bsn_switch_pipeline_stats_entry_t *list, of_bsn_switch_pipeline_stats_entry_t *obj);
extern int of_list_bsn_switch_pipeline_stats_entry_append(
    of_list_bsn_switch_pipeline_stats_entry_t *list, of_bsn_switch_pipeline_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_switch_pipeline_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_switch_pipeline_stats_entry
 * @param elt Pointer to an element of type of_bsn_switch_pipeline_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_SWITCH_PIPELINE_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_switch_pipeline_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_switch_pipeline_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_table_checksum_stats_entry */

extern int of_list_bsn_table_checksum_stats_entry_first(
    of_list_bsn_table_checksum_stats_entry_t *list, of_bsn_table_checksum_stats_entry_t *obj);
extern int of_list_bsn_table_checksum_stats_entry_next(
    of_list_bsn_table_checksum_stats_entry_t *list, of_bsn_table_checksum_stats_entry_t *obj);
extern int of_list_bsn_table_checksum_stats_entry_append_bind(
    of_list_bsn_table_checksum_stats_entry_t *list, of_bsn_table_checksum_stats_entry_t *obj);
extern int of_list_bsn_table_checksum_stats_entry_append(
    of_list_bsn_table_checksum_stats_entry_t *list, of_bsn_table_checksum_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_table_checksum_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_table_checksum_stats_entry
 * @param elt Pointer to an element of type of_bsn_table_checksum_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_TABLE_CHECKSUM_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_table_checksum_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_table_checksum_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_tlv */

extern int of_list_bsn_tlv_first(
    of_list_bsn_tlv_t *list, of_bsn_tlv_t *obj);
extern int of_list_bsn_tlv_next(
    of_list_bsn_tlv_t *list, of_bsn_tlv_t *obj);
extern int of_list_bsn_tlv_append_bind(
    of_list_bsn_tlv_t *list, of_bsn_tlv_t *obj);
extern int of_list_bsn_tlv_append(
    of_list_bsn_tlv_t *list, of_bsn_tlv_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_tlv
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_tlv
 * @param elt Pointer to an element of type of_bsn_tlv
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_TLV_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_tlv_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_tlv_next((list), (elt)))

/* Unified accessor functions for of_list_bsn_vlan_counter_stats_entry */

extern int of_list_bsn_vlan_counter_stats_entry_first(
    of_list_bsn_vlan_counter_stats_entry_t *list, of_bsn_vlan_counter_stats_entry_t *obj);
extern int of_list_bsn_vlan_counter_stats_entry_next(
    of_list_bsn_vlan_counter_stats_entry_t *list, of_bsn_vlan_counter_stats_entry_t *obj);
extern int of_list_bsn_vlan_counter_stats_entry_append_bind(
    of_list_bsn_vlan_counter_stats_entry_t *list, of_bsn_vlan_counter_stats_entry_t *obj);
extern int of_list_bsn_vlan_counter_stats_entry_append(
    of_list_bsn_vlan_counter_stats_entry_t *list, of_bsn_vlan_counter_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_bsn_vlan_counter_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_bsn_vlan_counter_stats_entry
 * @param elt Pointer to an element of type of_bsn_vlan_counter_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BSN_VLAN_COUNTER_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_bsn_vlan_counter_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bsn_vlan_counter_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_bucket */

extern int of_list_bucket_first(
    of_list_bucket_t *list, of_bucket_t *obj);
extern int of_list_bucket_next(
    of_list_bucket_t *list, of_bucket_t *obj);
extern int of_list_bucket_append_bind(
    of_list_bucket_t *list, of_bucket_t *obj);
extern int of_list_bucket_append(
    of_list_bucket_t *list, of_bucket_t *obj);

/**
 * Iteration macro for list of type of_list_bucket
 * @param list Pointer to the list being iterated over of
 * type of_list_bucket
 * @param elt Pointer to an element of type of_bucket
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BUCKET_ITER(list, elt, rv)  \
    for ((rv) = of_list_bucket_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bucket_next((list), (elt)))

/* Unified accessor functions for of_list_bucket_counter */

extern int of_list_bucket_counter_first(
    of_list_bucket_counter_t *list, of_bucket_counter_t *obj);
extern int of_list_bucket_counter_next(
    of_list_bucket_counter_t *list, of_bucket_counter_t *obj);
extern int of_list_bucket_counter_append_bind(
    of_list_bucket_counter_t *list, of_bucket_counter_t *obj);
extern int of_list_bucket_counter_append(
    of_list_bucket_counter_t *list, of_bucket_counter_t *obj);

/**
 * Iteration macro for list of type of_list_bucket_counter
 * @param list Pointer to the list being iterated over of
 * type of_list_bucket_counter
 * @param elt Pointer to an element of type of_bucket_counter
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_BUCKET_COUNTER_ITER(list, elt, rv)  \
    for ((rv) = of_list_bucket_counter_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_bucket_counter_next((list), (elt)))

/* Unified accessor functions for of_list_flow_stats_entry */

extern int of_list_flow_stats_entry_first(
    of_list_flow_stats_entry_t *list, of_flow_stats_entry_t *obj);
extern int of_list_flow_stats_entry_next(
    of_list_flow_stats_entry_t *list, of_flow_stats_entry_t *obj);
extern int of_list_flow_stats_entry_append_bind(
    of_list_flow_stats_entry_t *list, of_flow_stats_entry_t *obj);
extern int of_list_flow_stats_entry_append(
    of_list_flow_stats_entry_t *list, of_flow_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_flow_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_flow_stats_entry
 * @param elt Pointer to an element of type of_flow_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_FLOW_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_flow_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_flow_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_group_desc_stats_entry */

extern int of_list_group_desc_stats_entry_first(
    of_list_group_desc_stats_entry_t *list, of_group_desc_stats_entry_t *obj);
extern int of_list_group_desc_stats_entry_next(
    of_list_group_desc_stats_entry_t *list, of_group_desc_stats_entry_t *obj);
extern int of_list_group_desc_stats_entry_append_bind(
    of_list_group_desc_stats_entry_t *list, of_group_desc_stats_entry_t *obj);
extern int of_list_group_desc_stats_entry_append(
    of_list_group_desc_stats_entry_t *list, of_group_desc_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_group_desc_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_group_desc_stats_entry
 * @param elt Pointer to an element of type of_group_desc_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_GROUP_DESC_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_group_desc_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_group_desc_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_group_stats_entry */

extern int of_list_group_stats_entry_first(
    of_list_group_stats_entry_t *list, of_group_stats_entry_t *obj);
extern int of_list_group_stats_entry_next(
    of_list_group_stats_entry_t *list, of_group_stats_entry_t *obj);
extern int of_list_group_stats_entry_append_bind(
    of_list_group_stats_entry_t *list, of_group_stats_entry_t *obj);
extern int of_list_group_stats_entry_append(
    of_list_group_stats_entry_t *list, of_group_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_group_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_group_stats_entry
 * @param elt Pointer to an element of type of_group_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_GROUP_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_group_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_group_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_hello_elem */

extern int of_list_hello_elem_first(
    of_list_hello_elem_t *list, of_hello_elem_t *obj);
extern int of_list_hello_elem_next(
    of_list_hello_elem_t *list, of_hello_elem_t *obj);
extern int of_list_hello_elem_append_bind(
    of_list_hello_elem_t *list, of_hello_elem_t *obj);
extern int of_list_hello_elem_append(
    of_list_hello_elem_t *list, of_hello_elem_t *obj);

/**
 * Iteration macro for list of type of_list_hello_elem
 * @param list Pointer to the list being iterated over of
 * type of_list_hello_elem
 * @param elt Pointer to an element of type of_hello_elem
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_HELLO_ELEM_ITER(list, elt, rv)  \
    for ((rv) = of_list_hello_elem_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_hello_elem_next((list), (elt)))

/* Unified accessor functions for of_list_instruction */

extern int of_list_instruction_first(
    of_list_instruction_t *list, of_instruction_t *obj);
extern int of_list_instruction_next(
    of_list_instruction_t *list, of_instruction_t *obj);
extern int of_list_instruction_append_bind(
    of_list_instruction_t *list, of_instruction_t *obj);
extern int of_list_instruction_append(
    of_list_instruction_t *list, of_instruction_t *obj);

/**
 * Iteration macro for list of type of_list_instruction
 * @param list Pointer to the list being iterated over of
 * type of_list_instruction
 * @param elt Pointer to an element of type of_instruction
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_INSTRUCTION_ITER(list, elt, rv)  \
    for ((rv) = of_list_instruction_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_instruction_next((list), (elt)))

/* Unified accessor functions for of_list_instruction_id */

extern int of_list_instruction_id_first(
    of_list_instruction_id_t *list, of_instruction_id_t *obj);
extern int of_list_instruction_id_next(
    of_list_instruction_id_t *list, of_instruction_id_t *obj);
extern int of_list_instruction_id_append_bind(
    of_list_instruction_id_t *list, of_instruction_id_t *obj);
extern int of_list_instruction_id_append(
    of_list_instruction_id_t *list, of_instruction_id_t *obj);

/**
 * Iteration macro for list of type of_list_instruction_id
 * @param list Pointer to the list being iterated over of
 * type of_list_instruction_id
 * @param elt Pointer to an element of type of_instruction_id
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_INSTRUCTION_ID_ITER(list, elt, rv)  \
    for ((rv) = of_list_instruction_id_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_instruction_id_next((list), (elt)))

/* Unified accessor functions for of_list_meter_band */

extern int of_list_meter_band_first(
    of_list_meter_band_t *list, of_meter_band_t *obj);
extern int of_list_meter_band_next(
    of_list_meter_band_t *list, of_meter_band_t *obj);
extern int of_list_meter_band_append_bind(
    of_list_meter_band_t *list, of_meter_band_t *obj);
extern int of_list_meter_band_append(
    of_list_meter_band_t *list, of_meter_band_t *obj);

/**
 * Iteration macro for list of type of_list_meter_band
 * @param list Pointer to the list being iterated over of
 * type of_list_meter_band
 * @param elt Pointer to an element of type of_meter_band
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_METER_BAND_ITER(list, elt, rv)  \
    for ((rv) = of_list_meter_band_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_meter_band_next((list), (elt)))

/* Unified accessor functions for of_list_meter_band_stats */

extern int of_list_meter_band_stats_first(
    of_list_meter_band_stats_t *list, of_meter_band_stats_t *obj);
extern int of_list_meter_band_stats_next(
    of_list_meter_band_stats_t *list, of_meter_band_stats_t *obj);
extern int of_list_meter_band_stats_append_bind(
    of_list_meter_band_stats_t *list, of_meter_band_stats_t *obj);
extern int of_list_meter_band_stats_append(
    of_list_meter_band_stats_t *list, of_meter_band_stats_t *obj);

/**
 * Iteration macro for list of type of_list_meter_band_stats
 * @param list Pointer to the list being iterated over of
 * type of_list_meter_band_stats
 * @param elt Pointer to an element of type of_meter_band_stats
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_METER_BAND_STATS_ITER(list, elt, rv)  \
    for ((rv) = of_list_meter_band_stats_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_meter_band_stats_next((list), (elt)))

/* Unified accessor functions for of_list_meter_stats */

extern int of_list_meter_stats_first(
    of_list_meter_stats_t *list, of_meter_stats_t *obj);
extern int of_list_meter_stats_next(
    of_list_meter_stats_t *list, of_meter_stats_t *obj);
extern int of_list_meter_stats_append_bind(
    of_list_meter_stats_t *list, of_meter_stats_t *obj);
extern int of_list_meter_stats_append(
    of_list_meter_stats_t *list, of_meter_stats_t *obj);

/**
 * Iteration macro for list of type of_list_meter_stats
 * @param list Pointer to the list being iterated over of
 * type of_list_meter_stats
 * @param elt Pointer to an element of type of_meter_stats
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_METER_STATS_ITER(list, elt, rv)  \
    for ((rv) = of_list_meter_stats_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_meter_stats_next((list), (elt)))

/* Unified accessor functions for of_list_oxm */

extern int of_list_oxm_first(
    of_list_oxm_t *list, of_oxm_t *obj);
extern int of_list_oxm_next(
    of_list_oxm_t *list, of_oxm_t *obj);
extern int of_list_oxm_append_bind(
    of_list_oxm_t *list, of_oxm_t *obj);
extern int of_list_oxm_append(
    of_list_oxm_t *list, of_oxm_t *obj);

/**
 * Iteration macro for list of type of_list_oxm
 * @param list Pointer to the list being iterated over of
 * type of_list_oxm
 * @param elt Pointer to an element of type of_oxm
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_OXM_ITER(list, elt, rv)  \
    for ((rv) = of_list_oxm_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_oxm_next((list), (elt)))

/* Unified accessor functions for of_list_packet_queue */

extern int of_list_packet_queue_first(
    of_list_packet_queue_t *list, of_packet_queue_t *obj);
extern int of_list_packet_queue_next(
    of_list_packet_queue_t *list, of_packet_queue_t *obj);
extern int of_list_packet_queue_append_bind(
    of_list_packet_queue_t *list, of_packet_queue_t *obj);
extern int of_list_packet_queue_append(
    of_list_packet_queue_t *list, of_packet_queue_t *obj);

/**
 * Iteration macro for list of type of_list_packet_queue
 * @param list Pointer to the list being iterated over of
 * type of_list_packet_queue
 * @param elt Pointer to an element of type of_packet_queue
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_PACKET_QUEUE_ITER(list, elt, rv)  \
    for ((rv) = of_list_packet_queue_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_packet_queue_next((list), (elt)))

/* Unified accessor functions for of_list_port_desc */

extern int of_list_port_desc_first(
    of_list_port_desc_t *list, of_port_desc_t *obj);
extern int of_list_port_desc_next(
    of_list_port_desc_t *list, of_port_desc_t *obj);
extern int of_list_port_desc_append_bind(
    of_list_port_desc_t *list, of_port_desc_t *obj);
extern int of_list_port_desc_append(
    of_list_port_desc_t *list, of_port_desc_t *obj);

/**
 * Iteration macro for list of type of_list_port_desc
 * @param list Pointer to the list being iterated over of
 * type of_list_port_desc
 * @param elt Pointer to an element of type of_port_desc
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_PORT_DESC_ITER(list, elt, rv)  \
    for ((rv) = of_list_port_desc_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_port_desc_next((list), (elt)))

/* Unified accessor functions for of_list_port_stats_entry */

extern int of_list_port_stats_entry_first(
    of_list_port_stats_entry_t *list, of_port_stats_entry_t *obj);
extern int of_list_port_stats_entry_next(
    of_list_port_stats_entry_t *list, of_port_stats_entry_t *obj);
extern int of_list_port_stats_entry_append_bind(
    of_list_port_stats_entry_t *list, of_port_stats_entry_t *obj);
extern int of_list_port_stats_entry_append(
    of_list_port_stats_entry_t *list, of_port_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_port_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_port_stats_entry
 * @param elt Pointer to an element of type of_port_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_PORT_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_port_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_port_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_queue_prop */

extern int of_list_queue_prop_first(
    of_list_queue_prop_t *list, of_queue_prop_t *obj);
extern int of_list_queue_prop_next(
    of_list_queue_prop_t *list, of_queue_prop_t *obj);
extern int of_list_queue_prop_append_bind(
    of_list_queue_prop_t *list, of_queue_prop_t *obj);
extern int of_list_queue_prop_append(
    of_list_queue_prop_t *list, of_queue_prop_t *obj);

/**
 * Iteration macro for list of type of_list_queue_prop
 * @param list Pointer to the list being iterated over of
 * type of_list_queue_prop
 * @param elt Pointer to an element of type of_queue_prop
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_QUEUE_PROP_ITER(list, elt, rv)  \
    for ((rv) = of_list_queue_prop_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_queue_prop_next((list), (elt)))

/* Unified accessor functions for of_list_queue_stats_entry */

extern int of_list_queue_stats_entry_first(
    of_list_queue_stats_entry_t *list, of_queue_stats_entry_t *obj);
extern int of_list_queue_stats_entry_next(
    of_list_queue_stats_entry_t *list, of_queue_stats_entry_t *obj);
extern int of_list_queue_stats_entry_append_bind(
    of_list_queue_stats_entry_t *list, of_queue_stats_entry_t *obj);
extern int of_list_queue_stats_entry_append(
    of_list_queue_stats_entry_t *list, of_queue_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_queue_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_queue_stats_entry
 * @param elt Pointer to an element of type of_queue_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_QUEUE_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_queue_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_queue_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_table_feature_prop */

extern int of_list_table_feature_prop_first(
    of_list_table_feature_prop_t *list, of_table_feature_prop_t *obj);
extern int of_list_table_feature_prop_next(
    of_list_table_feature_prop_t *list, of_table_feature_prop_t *obj);
extern int of_list_table_feature_prop_append_bind(
    of_list_table_feature_prop_t *list, of_table_feature_prop_t *obj);
extern int of_list_table_feature_prop_append(
    of_list_table_feature_prop_t *list, of_table_feature_prop_t *obj);

/**
 * Iteration macro for list of type of_list_table_feature_prop
 * @param list Pointer to the list being iterated over of
 * type of_list_table_feature_prop
 * @param elt Pointer to an element of type of_table_feature_prop
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_TABLE_FEATURE_PROP_ITER(list, elt, rv)  \
    for ((rv) = of_list_table_feature_prop_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_table_feature_prop_next((list), (elt)))

/* Unified accessor functions for of_list_table_features */

extern int of_list_table_features_first(
    of_list_table_features_t *list, of_table_features_t *obj);
extern int of_list_table_features_next(
    of_list_table_features_t *list, of_table_features_t *obj);
extern int of_list_table_features_append_bind(
    of_list_table_features_t *list, of_table_features_t *obj);
extern int of_list_table_features_append(
    of_list_table_features_t *list, of_table_features_t *obj);

/**
 * Iteration macro for list of type of_list_table_features
 * @param list Pointer to the list being iterated over of
 * type of_list_table_features
 * @param elt Pointer to an element of type of_table_features
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_TABLE_FEATURES_ITER(list, elt, rv)  \
    for ((rv) = of_list_table_features_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_table_features_next((list), (elt)))

/* Unified accessor functions for of_list_table_stats_entry */

extern int of_list_table_stats_entry_first(
    of_list_table_stats_entry_t *list, of_table_stats_entry_t *obj);
extern int of_list_table_stats_entry_next(
    of_list_table_stats_entry_t *list, of_table_stats_entry_t *obj);
extern int of_list_table_stats_entry_append_bind(
    of_list_table_stats_entry_t *list, of_table_stats_entry_t *obj);
extern int of_list_table_stats_entry_append(
    of_list_table_stats_entry_t *list, of_table_stats_entry_t *obj);

/**
 * Iteration macro for list of type of_list_table_stats_entry
 * @param list Pointer to the list being iterated over of
 * type of_list_table_stats_entry
 * @param elt Pointer to an element of type of_table_stats_entry
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_TABLE_STATS_ENTRY_ITER(list, elt, rv)  \
    for ((rv) = of_list_table_stats_entry_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_table_stats_entry_next((list), (elt)))

/* Unified accessor functions for of_list_uint32 */

extern int of_list_uint32_first(
    of_list_uint32_t *list, of_uint32_t *obj);
extern int of_list_uint32_next(
    of_list_uint32_t *list, of_uint32_t *obj);
extern int of_list_uint32_append_bind(
    of_list_uint32_t *list, of_uint32_t *obj);
extern int of_list_uint32_append(
    of_list_uint32_t *list, of_uint32_t *obj);

/**
 * Iteration macro for list of type of_list_uint32
 * @param list Pointer to the list being iterated over of
 * type of_list_uint32
 * @param elt Pointer to an element of type of_uint32
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_UINT32_ITER(list, elt, rv)  \
    for ((rv) = of_list_uint32_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_uint32_next((list), (elt)))

/* Unified accessor functions for of_list_uint64 */

extern int of_list_uint64_first(
    of_list_uint64_t *list, of_uint64_t *obj);
extern int of_list_uint64_next(
    of_list_uint64_t *list, of_uint64_t *obj);
extern int of_list_uint64_append_bind(
    of_list_uint64_t *list, of_uint64_t *obj);
extern int of_list_uint64_append(
    of_list_uint64_t *list, of_uint64_t *obj);

/**
 * Iteration macro for list of type of_list_uint64
 * @param list Pointer to the list being iterated over of
 * type of_list_uint64
 * @param elt Pointer to an element of type of_uint64
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_UINT64_ITER(list, elt, rv)  \
    for ((rv) = of_list_uint64_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_uint64_next((list), (elt)))

/* Unified accessor functions for of_list_uint8 */

extern int of_list_uint8_first(
    of_list_uint8_t *list, of_uint8_t *obj);
extern int of_list_uint8_next(
    of_list_uint8_t *list, of_uint8_t *obj);
extern int of_list_uint8_append_bind(
    of_list_uint8_t *list, of_uint8_t *obj);
extern int of_list_uint8_append(
    of_list_uint8_t *list, of_uint8_t *obj);

/**
 * Iteration macro for list of type of_list_uint8
 * @param list Pointer to the list being iterated over of
 * type of_list_uint8
 * @param elt Pointer to an element of type of_uint8
 * @param rv On exiting the loop will have the value OF_ERROR_RANGE.
 */
#define OF_LIST_UINT8_ITER(list, elt, rv)  \
    for ((rv) = of_list_uint8_first((list), (elt));   \
         (rv) == OF_ERROR_NONE;   \
         (rv) = of_list_uint8_next((list), (elt)))

/**
 * Inheritance super class for of_queue_prop
 *
 * This class is the union of of_queue_prop classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_queue_prop_u {
    of_queue_prop_header_t header; /* Generic instance */
    of_queue_prop_experimenter_t experimenter;
    of_queue_prop_max_rate_t max_rate;
    of_queue_prop_min_rate_t min_rate;
};

/**
 * Inheritance super class for of_hello_elem
 *
 * This class is the union of of_hello_elem classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_hello_elem_u {
    of_hello_elem_header_t header; /* Generic instance */
    of_hello_elem_versionbitmap_t versionbitmap;
};

/**
 * Inheritance super class for of_bsn_vport
 *
 * This class is the union of of_bsn_vport classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_bsn_vport_u {
    of_bsn_vport_header_t header; /* Generic instance */
    of_bsn_vport_q_in_q_t q_in_q;
};

/**
 * Inheritance super class for of_table_feature_prop
 *
 * This class is the union of of_table_feature_prop classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_table_feature_prop_u {
    of_table_feature_prop_header_t header; /* Generic instance */
    of_table_feature_prop_apply_actions_t apply_actions;
    of_table_feature_prop_apply_actions_miss_t apply_actions_miss;
    of_table_feature_prop_apply_setfield_t apply_setfield;
    of_table_feature_prop_apply_setfield_miss_t apply_setfield_miss;
    of_table_feature_prop_experimenter_t experimenter;
    of_table_feature_prop_experimenter_miss_t experimenter_miss;
    of_table_feature_prop_instructions_t instructions;
    of_table_feature_prop_instructions_miss_t instructions_miss;
    of_table_feature_prop_match_t match;
    of_table_feature_prop_next_tables_t next_tables;
    of_table_feature_prop_next_tables_miss_t next_tables_miss;
    of_table_feature_prop_wildcards_t wildcards;
    of_table_feature_prop_write_actions_t write_actions;
    of_table_feature_prop_write_actions_miss_t write_actions_miss;
    of_table_feature_prop_write_setfield_t write_setfield;
    of_table_feature_prop_write_setfield_miss_t write_setfield_miss;
};

/**
 * Inheritance super class for of_action
 *
 * This class is the union of of_action classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_action_u {
    of_action_header_t header; /* Generic instance */
    of_action_bsn_t bsn;
    of_action_bsn_mirror_t bsn_mirror;
    of_action_bsn_set_tunnel_dst_t bsn_set_tunnel_dst;
    of_action_copy_ttl_in_t copy_ttl_in;
    of_action_copy_ttl_out_t copy_ttl_out;
    of_action_dec_mpls_ttl_t dec_mpls_ttl;
    of_action_dec_nw_ttl_t dec_nw_ttl;
    of_action_enqueue_t enqueue;
    of_action_experimenter_t experimenter;
    of_action_group_t group;
    of_action_nicira_t nicira;
    of_action_nicira_dec_ttl_t nicira_dec_ttl;
    of_action_ofdpa_t ofdpa;
    of_action_ofdpa_check_drop_status_t ofdpa_check_drop_status;
    of_action_ofdpa_class_based_count_t ofdpa_class_based_count;
    of_action_ofdpa_color_based_count_t ofdpa_color_based_count;
    of_action_ofdpa_color_based_count_1_t ofdpa_color_based_count_1;
    of_action_ofdpa_color_based_count_2_t ofdpa_color_based_count_2;
    of_action_ofdpa_color_based_count_3_t ofdpa_color_based_count_3;
    of_action_ofdpa_copy_tc_in_t ofdpa_copy_tc_in;
    of_action_ofdpa_copy_tc_out_t ofdpa_copy_tc_out;
    of_action_ofdpa_dec_ttl_mtu_t ofdpa_dec_ttl_mtu;
    of_action_ofdpa_oam_lm_rx_count_t ofdpa_oam_lm_rx_count;
    of_action_ofdpa_oam_lm_tx_count_t ofdpa_oam_lm_tx_count;
    of_action_ofdpa_pop_cw_t ofdpa_pop_cw;
    of_action_ofdpa_pop_l2hdr_t ofdpa_pop_l2hdr;
    of_action_ofdpa_push_cw_t ofdpa_push_cw;
    of_action_ofdpa_push_l2hdr_t ofdpa_push_l2hdr;
    of_action_ofdpa_set_counter_fields_t ofdpa_set_counter_fields;
    of_action_ofdpa_set_mpls_pcpdei_from_table_t ofdpa_set_mpls_pcpdei_from_table;
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t ofdpa_set_mpls_pcpdei_from_tunnel_table;
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t ofdpa_set_mpls_tc_from_tunnel_table;
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t ofdpa_set_mpls_tc_from_vpn_table;
    of_action_ofdpa_set_pcpdei_from_table_t ofdpa_set_pcpdei_from_table;
    of_action_onf_t onf;
    of_action_onf_copy_field_t onf_copy_field;
    of_action_output_t output;
    of_action_pop_mpls_t pop_mpls;
    of_action_pop_pbb_t pop_pbb;
    of_action_pop_vlan_t pop_vlan;
    of_action_push_mpls_t push_mpls;
    of_action_push_pbb_t push_pbb;
    of_action_push_vlan_t push_vlan;
    of_action_set_dl_dst_t set_dl_dst;
    of_action_set_dl_src_t set_dl_src;
    of_action_set_field_t set_field;
    of_action_set_mpls_label_t set_mpls_label;
    of_action_set_mpls_tc_t set_mpls_tc;
    of_action_set_mpls_ttl_t set_mpls_ttl;
    of_action_set_nw_dst_t set_nw_dst;
    of_action_set_nw_ecn_t set_nw_ecn;
    of_action_set_nw_src_t set_nw_src;
    of_action_set_nw_tos_t set_nw_tos;
    of_action_set_nw_ttl_t set_nw_ttl;
    of_action_set_queue_t set_queue;
    of_action_set_tp_dst_t set_tp_dst;
    of_action_set_tp_src_t set_tp_src;
    of_action_set_vlan_pcp_t set_vlan_pcp;
    of_action_set_vlan_vid_t set_vlan_vid;
    of_action_strip_vlan_t strip_vlan;
};

/**
 * Inheritance super class for of_instruction
 *
 * This class is the union of of_instruction classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_instruction_u {
    of_instruction_header_t header; /* Generic instance */
    of_instruction_apply_actions_t apply_actions;
    of_instruction_bsn_t bsn;
    of_instruction_bsn_arp_offload_t bsn_arp_offload;
    of_instruction_bsn_deny_t bsn_deny;
    of_instruction_bsn_dhcp_offload_t bsn_dhcp_offload;
    of_instruction_bsn_disable_split_horizon_check_t bsn_disable_split_horizon_check;
    of_instruction_bsn_disable_src_mac_check_t bsn_disable_src_mac_check;
    of_instruction_bsn_packet_of_death_t bsn_packet_of_death;
    of_instruction_bsn_permit_t bsn_permit;
    of_instruction_clear_actions_t clear_actions;
    of_instruction_experimenter_t experimenter;
    of_instruction_goto_table_t goto_table;
    of_instruction_meter_t meter;
    of_instruction_write_actions_t write_actions;
    of_instruction_write_metadata_t write_metadata;
};

/**
 * Inheritance super class for of_bsn_tlv
 *
 * This class is the union of of_bsn_tlv classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_bsn_tlv_u {
    of_bsn_tlv_header_t header; /* Generic instance */
    of_bsn_tlv_broadcast_query_timeout_t broadcast_query_timeout;
    of_bsn_tlv_circuit_id_t circuit_id;
    of_bsn_tlv_idle_notification_t idle_notification;
    of_bsn_tlv_idle_time_t idle_time;
    of_bsn_tlv_idle_timeout_t idle_timeout;
    of_bsn_tlv_ipv4_t ipv4;
    of_bsn_tlv_mac_t mac;
    of_bsn_tlv_miss_packets_t miss_packets;
    of_bsn_tlv_port_t port;
    of_bsn_tlv_reply_packets_t reply_packets;
    of_bsn_tlv_request_packets_t request_packets;
    of_bsn_tlv_rx_packets_t rx_packets;
    of_bsn_tlv_tx_packets_t tx_packets;
    of_bsn_tlv_udf_anchor_t udf_anchor;
    of_bsn_tlv_udf_id_t udf_id;
    of_bsn_tlv_udf_length_t udf_length;
    of_bsn_tlv_udf_offset_t udf_offset;
    of_bsn_tlv_unicast_query_timeout_t unicast_query_timeout;
    of_bsn_tlv_vlan_vid_t vlan_vid;
};

/**
 * Inheritance super class for of_instruction_id
 *
 * This class is the union of of_instruction_id classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_instruction_id_u {
    of_instruction_id_header_t header; /* Generic instance */
    of_instruction_id_apply_actions_t apply_actions;
    of_instruction_id_bsn_t bsn;
    of_instruction_id_bsn_arp_offload_t bsn_arp_offload;
    of_instruction_id_bsn_deny_t bsn_deny;
    of_instruction_id_bsn_dhcp_offload_t bsn_dhcp_offload;
    of_instruction_id_bsn_disable_split_horizon_check_t bsn_disable_split_horizon_check;
    of_instruction_id_bsn_disable_src_mac_check_t bsn_disable_src_mac_check;
    of_instruction_id_bsn_packet_of_death_t bsn_packet_of_death;
    of_instruction_id_bsn_permit_t bsn_permit;
    of_instruction_id_clear_actions_t clear_actions;
    of_instruction_id_experimenter_t experimenter;
    of_instruction_id_goto_table_t goto_table;
    of_instruction_id_meter_t meter;
    of_instruction_id_write_actions_t write_actions;
    of_instruction_id_write_metadata_t write_metadata;
};

/**
 * Inheritance super class for of_meter_band
 *
 * This class is the union of of_meter_band classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_meter_band_u {
    of_meter_band_header_t header; /* Generic instance */
    of_meter_band_drop_t drop;
    of_meter_band_dscp_remark_t dscp_remark;
    of_meter_band_experimenter_t experimenter;
    of_meter_band_ofdpa_color_set_t ofdpa_color_set;
};

/**
 * Inheritance super class for of_action_id
 *
 * This class is the union of of_action_id classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_action_id_u {
    of_action_id_header_t header; /* Generic instance */
    of_action_id_bsn_t bsn;
    of_action_id_bsn_mirror_t bsn_mirror;
    of_action_id_bsn_set_tunnel_dst_t bsn_set_tunnel_dst;
    of_action_id_copy_ttl_in_t copy_ttl_in;
    of_action_id_copy_ttl_out_t copy_ttl_out;
    of_action_id_dec_mpls_ttl_t dec_mpls_ttl;
    of_action_id_dec_nw_ttl_t dec_nw_ttl;
    of_action_id_experimenter_t experimenter;
    of_action_id_group_t group;
    of_action_id_nicira_t nicira;
    of_action_id_nicira_dec_ttl_t nicira_dec_ttl;
    of_action_id_ofdpa_t ofdpa;
    of_action_id_ofdpa_check_drop_status_t ofdpa_check_drop_status;
    of_action_id_ofdpa_class_based_count_t ofdpa_class_based_count;
    of_action_id_ofdpa_color_based_count_t ofdpa_color_based_count;
    of_action_id_ofdpa_color_based_count_1_t ofdpa_color_based_count_1;
    of_action_id_ofdpa_color_based_count_2_t ofdpa_color_based_count_2;
    of_action_id_ofdpa_color_based_count_3_t ofdpa_color_based_count_3;
    of_action_id_ofdpa_copy_tc_in_t ofdpa_copy_tc_in;
    of_action_id_ofdpa_copy_tc_out_t ofdpa_copy_tc_out;
    of_action_id_ofdpa_dec_ttl_mtu_t ofdpa_dec_ttl_mtu;
    of_action_id_ofdpa_oam_lm_rx_count_t ofdpa_oam_lm_rx_count;
    of_action_id_ofdpa_oam_lm_tx_count_t ofdpa_oam_lm_tx_count;
    of_action_id_ofdpa_pop_cw_t ofdpa_pop_cw;
    of_action_id_ofdpa_pop_l2hdr_t ofdpa_pop_l2hdr;
    of_action_id_ofdpa_push_cw_t ofdpa_push_cw;
    of_action_id_ofdpa_push_l2hdr_t ofdpa_push_l2hdr;
    of_action_id_ofdpa_set_counter_fields_t ofdpa_set_counter_fields;
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t ofdpa_set_mpls_pcpdei_from_table;
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t ofdpa_set_mpls_pcpdei_from_tunnel_table;
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t ofdpa_set_mpls_tc_from_tunnel_table;
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t ofdpa_set_mpls_tc_from_vpn_table;
    of_action_id_ofdpa_set_pcpdei_from_table_t ofdpa_set_pcpdei_from_table;
    of_action_id_onf_t onf;
    of_action_id_onf_copy_field_t onf_copy_field;
    of_action_id_output_t output;
    of_action_id_pop_mpls_t pop_mpls;
    of_action_id_pop_pbb_t pop_pbb;
    of_action_id_pop_vlan_t pop_vlan;
    of_action_id_push_mpls_t push_mpls;
    of_action_id_push_pbb_t push_pbb;
    of_action_id_push_vlan_t push_vlan;
    of_action_id_set_field_t set_field;
    of_action_id_set_mpls_ttl_t set_mpls_ttl;
    of_action_id_set_nw_ttl_t set_nw_ttl;
    of_action_id_set_queue_t set_queue;
};

/**
 * Inheritance super class for of_oxm
 *
 * This class is the union of of_oxm classes.  You can refer
 * to it untyped by refering to the member 'header' whose structure
 * is common across all sub-classes.
 */

union of_oxm_u {
    of_oxm_header_t header; /* Generic instance */
    of_oxm_arp_op_t arp_op;
    of_oxm_arp_op_masked_t arp_op_masked;
    of_oxm_arp_sha_t arp_sha;
    of_oxm_arp_sha_masked_t arp_sha_masked;
    of_oxm_arp_spa_t arp_spa;
    of_oxm_arp_spa_masked_t arp_spa_masked;
    of_oxm_arp_tha_t arp_tha;
    of_oxm_arp_tha_masked_t arp_tha_masked;
    of_oxm_arp_tpa_t arp_tpa;
    of_oxm_arp_tpa_masked_t arp_tpa_masked;
    of_oxm_bsn_egr_port_group_id_t bsn_egr_port_group_id;
    of_oxm_bsn_egr_port_group_id_masked_t bsn_egr_port_group_id_masked;
    of_oxm_bsn_global_vrf_allowed_t bsn_global_vrf_allowed;
    of_oxm_bsn_global_vrf_allowed_masked_t bsn_global_vrf_allowed_masked;
    of_oxm_bsn_in_ports_128_t bsn_in_ports_128;
    of_oxm_bsn_in_ports_128_masked_t bsn_in_ports_128_masked;
    of_oxm_bsn_l3_dst_class_id_t bsn_l3_dst_class_id;
    of_oxm_bsn_l3_dst_class_id_masked_t bsn_l3_dst_class_id_masked;
    of_oxm_bsn_l3_interface_class_id_t bsn_l3_interface_class_id;
    of_oxm_bsn_l3_interface_class_id_masked_t bsn_l3_interface_class_id_masked;
    of_oxm_bsn_l3_src_class_id_t bsn_l3_src_class_id;
    of_oxm_bsn_l3_src_class_id_masked_t bsn_l3_src_class_id_masked;
    of_oxm_bsn_lag_id_t bsn_lag_id;
    of_oxm_bsn_lag_id_masked_t bsn_lag_id_masked;
    of_oxm_bsn_udf0_t bsn_udf0;
    of_oxm_bsn_udf0_masked_t bsn_udf0_masked;
    of_oxm_bsn_udf1_t bsn_udf1;
    of_oxm_bsn_udf1_masked_t bsn_udf1_masked;
    of_oxm_bsn_udf2_t bsn_udf2;
    of_oxm_bsn_udf2_masked_t bsn_udf2_masked;
    of_oxm_bsn_udf3_t bsn_udf3;
    of_oxm_bsn_udf3_masked_t bsn_udf3_masked;
    of_oxm_bsn_udf4_t bsn_udf4;
    of_oxm_bsn_udf4_masked_t bsn_udf4_masked;
    of_oxm_bsn_udf5_t bsn_udf5;
    of_oxm_bsn_udf5_masked_t bsn_udf5_masked;
    of_oxm_bsn_udf6_t bsn_udf6;
    of_oxm_bsn_udf6_masked_t bsn_udf6_masked;
    of_oxm_bsn_udf7_t bsn_udf7;
    of_oxm_bsn_udf7_masked_t bsn_udf7_masked;
    of_oxm_bsn_vrf_t bsn_vrf;
    of_oxm_bsn_vrf_masked_t bsn_vrf_masked;
    of_oxm_eth_dst_t eth_dst;
    of_oxm_eth_dst_masked_t eth_dst_masked;
    of_oxm_eth_src_t eth_src;
    of_oxm_eth_src_masked_t eth_src_masked;
    of_oxm_eth_type_t eth_type;
    of_oxm_eth_type_masked_t eth_type_masked;
    of_oxm_icmpv4_code_t icmpv4_code;
    of_oxm_icmpv4_code_masked_t icmpv4_code_masked;
    of_oxm_icmpv4_type_t icmpv4_type;
    of_oxm_icmpv4_type_masked_t icmpv4_type_masked;
    of_oxm_icmpv6_code_t icmpv6_code;
    of_oxm_icmpv6_code_masked_t icmpv6_code_masked;
    of_oxm_icmpv6_type_t icmpv6_type;
    of_oxm_icmpv6_type_masked_t icmpv6_type_masked;
    of_oxm_in_phy_port_t in_phy_port;
    of_oxm_in_phy_port_masked_t in_phy_port_masked;
    of_oxm_in_port_t in_port;
    of_oxm_in_port_masked_t in_port_masked;
    of_oxm_ip_dscp_t ip_dscp;
    of_oxm_ip_dscp_masked_t ip_dscp_masked;
    of_oxm_ip_ecn_t ip_ecn;
    of_oxm_ip_ecn_masked_t ip_ecn_masked;
    of_oxm_ip_proto_t ip_proto;
    of_oxm_ip_proto_masked_t ip_proto_masked;
    of_oxm_ipv4_dst_t ipv4_dst;
    of_oxm_ipv4_dst_masked_t ipv4_dst_masked;
    of_oxm_ipv4_src_t ipv4_src;
    of_oxm_ipv4_src_masked_t ipv4_src_masked;
    of_oxm_ipv6_dst_t ipv6_dst;
    of_oxm_ipv6_dst_masked_t ipv6_dst_masked;
    of_oxm_ipv6_flabel_t ipv6_flabel;
    of_oxm_ipv6_flabel_masked_t ipv6_flabel_masked;
    of_oxm_ipv6_nd_sll_t ipv6_nd_sll;
    of_oxm_ipv6_nd_sll_masked_t ipv6_nd_sll_masked;
    of_oxm_ipv6_nd_target_t ipv6_nd_target;
    of_oxm_ipv6_nd_target_masked_t ipv6_nd_target_masked;
    of_oxm_ipv6_nd_tll_t ipv6_nd_tll;
    of_oxm_ipv6_nd_tll_masked_t ipv6_nd_tll_masked;
    of_oxm_ipv6_src_t ipv6_src;
    of_oxm_ipv6_src_masked_t ipv6_src_masked;
    of_oxm_metadata_t metadata;
    of_oxm_metadata_masked_t metadata_masked;
    of_oxm_mpls_bos_t mpls_bos;
    of_oxm_mpls_bos_masked_t mpls_bos_masked;
    of_oxm_mpls_label_t mpls_label;
    of_oxm_mpls_label_masked_t mpls_label_masked;
    of_oxm_mpls_tc_t mpls_tc;
    of_oxm_mpls_tc_masked_t mpls_tc_masked;
    of_oxm_ofdpa_allow_vlan_translation_t ofdpa_allow_vlan_translation;
    of_oxm_ofdpa_color_t ofdpa_color;
    of_oxm_ofdpa_color_actions_index_t ofdpa_color_actions_index;
    of_oxm_ofdpa_dei_t ofdpa_dei;
    of_oxm_ofdpa_l3_in_port_t ofdpa_l3_in_port;
    of_oxm_ofdpa_lmep_id_t ofdpa_lmep_id;
    of_oxm_ofdpa_mpls_ach_channel_t ofdpa_mpls_ach_channel;
    of_oxm_ofdpa_mpls_data_first_nibble_t ofdpa_mpls_data_first_nibble;
    of_oxm_ofdpa_mpls_l2_port_t ofdpa_mpls_l2_port;
    of_oxm_ofdpa_mpls_l2_port_masked_t ofdpa_mpls_l2_port_masked;
    of_oxm_ofdpa_mpls_next_label_is_gal_t ofdpa_mpls_next_label_is_gal;
    of_oxm_ofdpa_mpls_ttl_t ofdpa_mpls_ttl;
    of_oxm_ofdpa_mpls_type_t ofdpa_mpls_type;
    of_oxm_ofdpa_oam_y1731_mdl_t ofdpa_oam_y1731_mdl;
    of_oxm_ofdpa_oam_y1731_opcode_t ofdpa_oam_y1731_opcode;
    of_oxm_ofdpa_ovid_t ofdpa_ovid;
    of_oxm_ofdpa_protection_index_t ofdpa_protection_index;
    of_oxm_ofdpa_qos_index_t ofdpa_qos_index;
    of_oxm_ofdpa_rx_timestamp_t ofdpa_rx_timestamp;
    of_oxm_ofdpa_rxfcl_t ofdpa_rxfcl;
    of_oxm_ofdpa_traffic_class_t ofdpa_traffic_class;
    of_oxm_ofdpa_txfcl_t ofdpa_txfcl;
    of_oxm_ofdpa_vrf_t ofdpa_vrf;
    of_oxm_onf_actset_output_t onf_actset_output;
    onf_oxm_packet_regs0_t onf_packet_regs0;
    of_oxm_sctp_dst_t sctp_dst;
    of_oxm_sctp_dst_masked_t sctp_dst_masked;
    of_oxm_sctp_src_t sctp_src;
    of_oxm_sctp_src_masked_t sctp_src_masked;
    of_oxm_tcp_dst_t tcp_dst;
    of_oxm_tcp_dst_masked_t tcp_dst_masked;
    of_oxm_tcp_src_t tcp_src;
    of_oxm_tcp_src_masked_t tcp_src_masked;
    of_oxm_tunnel_id_t tunnel_id;
    of_oxm_tunnel_id_masked_t tunnel_id_masked;
    of_oxm_udp_dst_t udp_dst;
    of_oxm_udp_dst_masked_t udp_dst_masked;
    of_oxm_udp_src_t udp_src;
    of_oxm_udp_src_masked_t udp_src_masked;
    of_oxm_vlan_pcp_t vlan_pcp;
    of_oxm_vlan_pcp_masked_t vlan_pcp_masked;
    of_oxm_vlan_vid_t vlan_vid;
    of_oxm_vlan_vid_masked_t vlan_vid_masked;
};


#endif
