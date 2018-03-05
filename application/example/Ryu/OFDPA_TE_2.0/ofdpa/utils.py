#*********************************************************************
#
# (C) Copyright Broadcom Corporation 2013-2015
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
import struct
import socket

LOG = logging.getLogger('ofdpa')

'''
Different useful staff
'''

def ip_to_long(ip):
    '''
    Convert string representation
    of IP addr to binary value
    '''
    return struct.unpack("!L", socket.inet_aton(ip))[0]

def int_from_ipv6(addr):
    hi, lo = struct.unpack('!QQ', socket.inet_pton(socket.AF_INET6, addr))
    return (hi << 64) | lo

def mac_str_to_bin(mac_str):
    return int(mac_str.replace(':', ''), 16)

def mac_bin_to_str(mac_bin):
    h = '{:x}'.format(mac_bin)
    return ':'.join(h[i:i+2] for i in range(0, 12, 2))

def mask_ntob(mask):
    '''
    Create mask
    for IP address
    '''
    return (0xffffffff << (32 - mask)) & 0xffffffff

def string_to_int_tuple(val):
    '''
    Convert string to int or tuple of ints.
    '''
    int_lst = []
    str_lst = val.split(',')
    if (len(str_lst) == 1):
        return int(val, 0)
    else:
        for s in str_lst:
            int_lst.append(int(s, 0))
    return tuple(int_lst)

def get_table(table_name):
    '''
    Return number of table by it name.
    '''

    tables_ofdpa1 = {
    'port' : 'TABLE_INGRESS_PORT',
    'vlan' : 'TABLE_VLAN',
    'mac' : 'TABLE_TERMINATION_MAC',
    'unicast' : 'TABLE_UNICAST_ROUTING',
    'multicast' : 'TABLE_MULTICAST_ROUTING',
    'bridging' : 'TABLE_BRIDGING',
    'acl' : 'TABLE_POLICY_ACL'
    }

    tables_ofdpa2 = {
    'TABLE_INGRESS_PORT' : 0,
    'TABLE_PORT_DSCP_TRUST' : 5,
    'TABLE_PORT_PCP_TRUST' : 6,
    'TABLE_TUNNEL_DSCP_TRUST' : 7,
    'TABLE_TUNNEL_PCP_TRUST' : 8,
    'TABLE_INJECTED_OAM' : 9,
    'TABLE_VLAN' : 10,
    'TABLE_VLAN1' : 11,
    'TABLE_INGRESS_MAINTENANCE_POINT' : 12,
    'TABLE_MPLS_L2_PORT' : 13,
    'TABLE_MPLS_DSCP_TRUST' : 15,
    'TABLE_MPLS_PCP_TRUST' : 16,
    'TABLE_L2_POLICER' : 18,
    'TABLE_L2_POLICER_ACTIONS' : 19,
    'TABLE_TERMINATION_MAC' : 20,
    'TABLE_L3_TYPE' : 21,
    'TABLE_MPLS0' : 23,
    'TABLE_MPLS1' : 24,
    'TABLE_MPLS2' : 25,
    'TABLE_MPLS-TP_MAINTENANCE_POINT' : 26,
    'TABLE_MPLS_L3_TYPE' : 27,
    'TABLE_MPLS_LABEL_TRUST' : 28,
    'TABLE_MPLS_TYPE' : 29,
    'TABLE_UNICAST_ROUTING' : 30,
    'TABLE_MULTICAST_ROUTING' : 40,
    'TABLE_BRIDGING' : 50,
    'TABLE_POLICY_ACL' : 60,
    'TABLE_COLOR_BASED_ACTIONS' : 65,
    'TABLE_EGRESS_VLAN' : 210,
    'TABLE_EGRESS_VLAN1' : 211,
    'TABLE_EGRESS_MAINTENANCE_POINT' : 226,
    'TABLE_EGRESS_DSCP_PCP_REMARK' : 230,
    'TABLE_EGRESS_TPID' : 235,
    'TABLE_L2_INTERFACE_DSCP_REMARK' : 237,
    'TABLE_L2_INTERFACE_1P_PRI_REMARK' : 238,
    'TABLE_MPLS_QOS' : 239,
    'TABLE_MPLS_VPN_TABLE_EXP_REMARK' : 240,
    'TABLE_MPLS_VPN_TABLE_1P_PRI_REMARK' : 241,
    'TABLE_MPLS_TUNNEL_TABLE_EXP_REMARK' : 242,
    'TABLE_MPLS_TUNNEL_TABLE_1P_PRI_REMARK' : 243,
    'TABLE_SRC_MAC_LEARNING' : 254
    }

    LOG.debug('table_name: %s', table_name)

    '''
    For compatibility:
    translate ofdpa1 names to ofdpa2
    '''
    if table_name in tables_ofdpa1:
        table_name = tables_ofdpa1[table_name]

    table_name.upper()

    LOG.debug('table_name: %s', table_name)

    if table_name in tables_ofdpa2:
        return tables_ofdpa2[table_name]
    else:
        LOG.error('Wrong table name %s ignore it', table_name)
        return None

def get_mod_command(dp, cmd):
    if(cmd == 'add'):
        return dp.ofproto.OFPFC_ADD

    elif(cmd == 'mod'):
        return dp.ofproto.OFPFC_MODIFY

    elif(cmd == 'mods'):
        return dp.ofproto.OFPFC_MODIFY_STRICT

    elif(cmd == 'del'):
        return dp.ofproto.OFPFC_DELETE

    elif(cmd == 'dels'):
        return dp.ofproto.OFPFC_DELETE_STRICT

    else:
        raise Exception('Wrong command', cmd)

def get_group_mod_command(dp, cmd):
    if(cmd == 'add'):
        return dp.ofproto.OFPGC_ADD

    elif(cmd == 'mod'):
        return dp.ofproto.OFPGC_MODIFY

    elif(cmd == 'del'):
        return dp.ofproto.OFPGC_DELETE

    else:
        raise Exception('Wrong command', cmd)

def get_mod_group(dp, group):
    if(group == 'all'):
        return dp.ofproto.OFPP_ALL
    if(group == 'any'):
        return dp.ofproto.OFPP_ANY
    else:
        return int(group, 0)

def get_mod_type(dp, grp_type):
    if(grp_type == 'all'):
        return dp.ofproto.OFPGT_ALL
    elif(grp_type == 'select'):
        return dp.ofproto.OFPGT_SELECT
    elif(grp_type == 'indirect'):
        return dp.ofproto.OFPGT_INDIRECT
    elif(grp_type == 'ff'):
        return dp.ofproto.OFPGT_FF
    else:
        raise Exception('Wrong group type', grp_type)

def get_mod_port(dp, port):
    if (port == 'in'):
        return dp.ofproto.OFPP_IN_PORT
    elif (port == 'table'):
        return dp.ofproto.OFPP_TABLE
    elif (port == 'normal'):
        return dp.ofproto.OFPP_NORMAL
    elif (port == 'flood'):
        return dp.ofproto.OFPP_FLOOD
    elif (port == 'all'):
        return dp.ofproto.OFPP_ALL
    elif (port == 'controller'):
        return dp.ofproto.OFPP_CONTROLLER
    elif (port == 'local'):
        return dp.ofproto.OFPP_LOCAL
    elif (port == 'any'):
        return dp.ofproto.OFPP_ANY
    else:
        return int(port, 0)
