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
Implements Actions
'''

def create_actions(dp,config):
    LOG.debug('ACTIONS:')
    actions = []
    '''
    Actions config is a list
    '''
    LOG.debug('actions config: %s', config)
    for action in config:
        for key in action.keys():
            if key[0] == '#':
                continue
            val = action[key]
            LOG.debug('key: %s', key)
            LOG.debug('val: %s', val)

            if (key == 'output'):
                actions.append(action_output(dp, val))

            elif (key == 'copy_ttl_in'):
                actions.append(action_copy_ttl_in(dp))

            elif (key == 'set_mpls_ttl'):
                actions.append(action_set_mpls_ttl(dp, val))

            elif (key == 'dec_mpls_ttl'):
                actions.append(action_dec_mpls_ttl(dp))

            elif (key == 'push_vlan'):
                actions.append(action_push_vlan(dp))

            elif (key == 'pop_vlan'):
                actions.append(action_pop_vlan(dp))

            elif (key == 'push_mpls'):
                actions.append(action_push_mpls(dp))

            elif (key == 'pop_mpls'):
                actions.append(action_pop_mpls(dp, val))

            elif(key == 'group'):
                actions.append(action_group(dp, val))

            elif (key == 'dec_nw_ttl'):
                actions.append(action_dec_nw_ttl(dp))

            elif (key == 'set_field'):
                actions.append(action_set_field(dp, val))

            elif (key == 'ofdpa_push_l2_header'):
                actions.append(ofdpa_action_push_l2_header(dp))

            elif (key == 'ofdpa_pop_l2_header'):
                actions.append(ofdpa_action_pop_l2_header(dp))

            elif (key == 'ofdpa_push_cw'):
                actions.append(ofdpa_action_push_cw(dp))

            elif (key == 'ofdpa_pop_cw_or_ach'):
                actions.append(ofdpa_action_pop_cw_or_ach(dp))

            elif (key == 'ofdpa_copy_tc_in'):
                actions.append(ofdpa_action_copy_tc_in(dp))

            elif (key == 'ofdpa_set_tc_from_vpn_table'):
                actions.append(ofdpa_action_set_tc_from_vpn_table(dp, val))

            elif (key == 'ofdpa_set_tc_from_tunnel_table'):
                actions.append(ofdpa_action_set_tc_from_tunnel_table(dp, val))

            elif (key == 'ofdpa_copy_tc_out'):
                actions.append(ofdpa_action_copy_tc_out(dp))

            elif (key == 'ofdpa_set_pcpdei_from_vpn_table'):
                actions.append(ofdpa_action_set_pcpdei_from_vpn_table(dp, val))

            elif (key == 'ofdpa_set_pcpdei_from_tunnel_table'):
                actions.append(ofdpa_action_set_pcpdei_from_tunnel_table(dp, val))

            else:
                raise Exception('Wrong action name:', key)

    LOG.debug('actions: %s', actions)
    return actions

def action_output(dp, config):
    return dp.ofproto_parser.OFPActionOutput(int(config['port'], 0))

def action_copy_ttl_out(dp):
    return dp.ofproto_parser.OFPActionCopyTtlOut()

def action_copy_ttl_in(dp):
    return dp.ofproto_parser.OFPActionCopyTtlIn()

def action_set_mpls_ttl(dp, config):
    return dp.ofproto_parser.OFPActionSetMplsTtl(int(config['mpls_ttl'], 0))

def action_dec_mpls_ttl(dp):
    return dp.ofproto_parser.OFPActionDecMplsTtl()

def action_push_vlan(dp):
    return dp.ofproto_parser.OFPActionPushVlan()

def action_pop_vlan(dp):
    return dp.ofproto_parser.OFPActionPopVlan()

def action_push_mpls(dp):
    return dp.ofproto_parser.OFPActionPushMpls()

def action_pop_mpls(dp, config):
    return dp.ofproto_parser.OFPActionPopMpls(int(config['ethertype'], 0))

def action_set_queue(dp, config):
    return dp.ofproto_parser.OFPActionSetQueue(int(config['queue_id'], 0))

def action_group(dp,config):
    return dp.ofproto_parser.OFPActionGroup(int(config['group_id'], 0))

def action_set_nw_ttl(dp,config):
    return dp.ofproto_parser.OFPActionSetNwTtl(int(config['nw_ttl'], 0))

def action_dec_nw_ttl(dp):
    return dp.ofproto_parser.OFPActionDecNwTtl()

def action_set_field(dp, config):
    LOG.debug('set_field config: %s', config)
    fields = {}
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

        if (key == 'eth_dst') or (key == 'eth_src'):
            LOG.debug('1 key:val %s:%s', key, val)
            key_mask = key + '_mask'
            if  key_mask in config.keys():
                LOG.debug('key_mask: %s', key_mask)
                val_mask = config[key_mask]
                val_bin = Utils.mac_str_to_bin(val)
                val_mask_bin = Utils.mac_str_to_bin(val_mask)
                LOG.debug('val_bin: %x val_bin_mask: %x', val_bin, val_mask_bin)
                val_bin = val_bin | val_mask_bin
                val = Utils.mac_bin_to_str(val_bin)
                LOG.debug('val: %s', val)
                #fields[key] = ('de:ad:be:ef:00:01', '00:00:00:ff:ff:ff')#val_mask
            fields[key] = val
        elif (key[:5] == 'ipv4_'):
            LOG.debug('2 key:val %s:%s', key, val)
            fields[key] = Utils.ip_to_long(val)
        elif (key == 'ipv6_src') or (key == 'ipv6_dst'):
            LOG.debug('3 key:val %s:%s', key, val)
            fields[key] = Utils.int_from_ipv6(val)
        else:
            LOG.debug('4 key:val %s:%s', key, val)
            fields[key] = int(val, 0)

    LOG.debug('fields: %s', fields)
    return dp.ofproto_parser.OFPActionSetField(**fields)

'''
OFDPA extensions
'''
def ofdpa_action_push_l2_header(dp):
    return dp.ofproto_parser.OFDPAActionPushL2Header()

def ofdpa_action_pop_l2_header(dp):
    return dp.ofproto_parser.OFDPAActionPopL2Header()

def ofdpa_action_push_cw(dp):
    return dp.ofproto_parser.OFDPAActionPushCw()

def ofdpa_action_pop_cw_or_ach(dp):
    return dp.ofproto_parser.OFDPAActionPopCwOrAch()

def ofdpa_action_copy_tc_in(dp):
    return dp.ofproto_parser.OFDPAActionCopyTcIn()

def ofdpa_action_copy_tc_out(dp):
    return dp.ofproto_parser.OFDPAActionCopyTcOut()

def ofdpa_action_set_tc_from_vpn_table(dp, config):
    return dp.ofproto_parser.OFDPAActionSetTcFromVpnTable(int(config['qos'], 0))

def ofdpa_action_set_tc_from_tunnel_table(dp, config):
    return dp.ofproto_parser.OFDPAActionSetMplsTcFromTunnelTable(int(config['qos'], 0))

def ofdpa_action_set_pcpdei_from_vpn_table(dp, config):
    return dp.ofproto_parser.OFDPAActionSetPcpDeiFromVpnTable(int(config['qos'], 0))

def ofdpa_action_set_pcpdei_from_tunnel_table(dp, config):
    return dp.ofproto_parser.OFDPAActionSetPcpDeiFromTunnelTable(int(config['qos'], 0))

