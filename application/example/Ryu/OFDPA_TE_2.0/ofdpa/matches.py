#*********************************************************************
#
# (C) Copyright Broadcom Corporation 2013-2014
#
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#*********************************************************************

import logging

import ofdpa.utils as Utils

LOG = logging.getLogger('ofdpa')

'''
Implements Matches object
'''
matches_int1 = (
            'icmpv4_code',
            'icmpv4_type',
            'icmpv6_code',
            'icmpv6_type',
            'ip_dscp',
            'ip_ecn',
            'ip_proto',
            'mpls_bos',
            'mpls_tc',
            'vlan_pcp',
            'ofdpa_allow_vlan_translation',
            'ofdpa_color',
            'ofdpa_dei',
            'ofdpa_eth_sub_type',
            'ofdpa_mpls_data_first_nibble',
            'ofdpa_mpls_next_label_is_gal',
            'ofdpa_mpls_ttl',
            'ofdpa_oam_y1731_mdl',
            'ofdpa_oam_y1731_opcode',
            'ofdpa_protection_index',
            'ofdpa_qos_index',
            'ofdpa_traffic_class',
            )

matches_int2 = (
            'eth_type',
            'sctp_dst',
            'sctp_src',
            'tcp_dst',
            'tcp_src',
            'udp_dst',
            'udp_src',
            'vlan_vid',
            'ofdpa_mpls_ach_channel',
            'ofdpa_mpls_type',
            'ofdpa_ovid',
            'ofdpa_vrf',
            )

matches_int4 = (
            'actset_output',
            'in_port',
            'ipv6_flabel',
            'mpls_label',
            'ofdpa_color_actions_index',
            'ofdpa_l3_in_port',
            'ofdpa_lmep_id',
            'ofdpa_mpls_l2_port',
            )

matches_int8 = (
            'tunnel_id',
            )

matches_mac = (
            'eth_dst',
            'eth_src',
                )

matches_ipv4 = (
            'arp_spa',
            'ipv4_dst',
            'ipv4_src',
            )

matches_ipv6 = (
            'ipv6_dst',
            'ipv6_src',
              )

matches_int = matches_int1 + matches_int2 + matches_int4 + matches_int8
matches_ip = matches_ipv4 + matches_ipv6
matches_all = matches_int + matches_mac + matches_ip

def create_matches(dp, config):
    LOG.debug('MATCHES:')
    fields = {}
    '''
    Matches config
    is a list
    '''
    LOG.debug('matches config: %s', config)

    for key in config.keys():
        if key[0] == '#':
            continue
        val = config[key]
        LOG.debug('key: %s', key)
        LOG.debug('val: %s', val)

        '''
        Process outdated '*_mask' keys.
        '''
        if (key[-5:] == '_mask'):
            LOG.debug('ignore: %s', key)
            continue

        if key not in matches_all:
            raise Exception('Wrong match name:', key)

        if key in matches_int:
            fields[key] = fields[key] = Utils.string_to_int_tuple(val)

        if key in matches_mac:
            key_mask = key + '_mask'
            if  key_mask in config.keys():
                LOG.debug('key_mask: %s', key_mask)
                val_mask = (val, config[key_mask])
                LOG.debug('val_mask: %s', val_mask)
                fields[key] = val_mask
            else:
                fields[key] = val

        if key in matches_ipv4:
            fields[key] = Utils.ip_to_long(val)

        if key in matches_ipv6:
            fields[key] = Utils.int_from_ipv6(val)

        LOG.debug('fields: %s', fields)

    matches = dp.ofproto_parser.OFPMatch(**fields)

    LOG.debug('matches: %s', matches)
    return matches






