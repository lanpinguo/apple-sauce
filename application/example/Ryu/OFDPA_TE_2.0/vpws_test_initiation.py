# To change this license header, choose License Headers in Project Properties.
# To change this template file, choose Tools | Templates
# and open the template in the editor.

from ryu.base import app_manager
from ryu.controller import dpset
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ofproto_v1_3
import sys
#
class VPWS_Initiation(app_manager.RyuApp):



    _CONTEXTS = {'dpset': dpset.DPSet}
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(VPWS_Initiation, self).__init__(*args, **kwargs)

    @set_ev_cls(dpset.EventDP, dpset.DPSET_EV_DISPATCHER)
    def handler_datapath(self, ev):
        print "=Event DP="
        print "dpid: %i" % ev.dp.id
        if ev.enter:
            self.build_packets(ev.dp)

    '''
    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def packet_in_handler(self, ev):
        print "=Event PacketIn="
    '''

    def build_packets(self, dp):

        def encode_l2_interface_gid(grp_type, vlan_id, port):
            print grp_type
            grp_type = grp_type & 0x0f
            vlan_id = vlan_id & 0x0fff
            port = port & 0xffff
            gid = grp_type << 28 | vlan_id << 16 | port
            print 'gid %x' % gid 
            #print bin(gid)
            return gid
           
        def encode_l3_multicast_gid(grp_type, vlan_id, index):
            grp_type = grp_type & 0x0f
            vlan_id = vlan_id & 0x0fff
            index = index & 0xffff
            gid = grp_type << 28 | vlan_id << 16 |index
            print 'gid %x' % gid
            #print bin(gid)
            return gid

        def encode_mpls_gid(grp_type, grp_subtype, index):
            grp_type = grp_type & 0x0f
            grp_subtype = grp_subtype & 0x0f
            index = index & 0x00ffffff
            gid = grp_type << 28 | grp_subtype << 24 | index
            print 'gid %x' % gid
            #print bin(gid)
            return gid
            
        TABLE_INGRESS_PORT = 0
        TABLE_PORT_DSCP_TRUST = 5
        TABLE_PORT_PCP_TRUST = 6
        TABLE_TUNNEL_DSCP_TRUST = 7
        TABLE_TUNNEL_PCP_TRUST = 8
        TABLE_VLAN = 10
        TABLE_VLAN1 = 11
        TABLE_MPLS_L2_PORT = 13
        TABLE_MPLS_DSCP_TRUST = 15
        TABLE_MPLS_PCP_TRUST = 16
        TABLE_TERMINATION_MAC = 20
        TABLE_MPLS0 = 23
        TABLE_MPLS1 = 24
        TABLE_MPLS2 = 25
        TABLE_BFD = 27
        TABLE_UNICAST_ROUTING = 30
        TABLE_MULTICAST_ROUTING = 40
        TABLE_BRIDGING = 50
        TABLE_POLICY_ACL = 60
        TABLE_L2_INTERFACE_DSCP_REMARK = 237
        TABLE_L2_INTERFACE_1P_PRI_REMARK = 238
        TABLE_MPLS_QOS = 239
        TABLE_MPLS_VPN_TABLE_EXP_REMARK = 240
        TABLE_MPLS_VPN_TABLE_1P_PRI_REMARK = 241
        TABLE_MPLS_TUNNEL_TABLE_EXP_REMARK = 242
        TABLE_MPLS_TUNNEL_TABLE_1P_PRI_REMARK = 243
        TABLE_SRC_MAC_LEARNING = 254

        GRP_TYPE_L2_INTERFACE = 0
        GRP_TYPE_REWRITE = 1
        GRP_TYPE_L3_UNICAST = 2
        GRP_TYPE_L2_MULTICAST = 3
        GRP_TYPE_L2_FLOOD = 4
        GRP_TYPE_L3_INTERFACE = 5
        GRP_TYPE_L3_MULTICAST = 6
        GRP_TYPE_L3_ECMP = 7
        GRP_TYPE_L2_OVERLAY = 8
        GRP_TYPE_MPLS_LABEL = 9

        GRP_SUBTYPE_MPLS_LABEL_INTERFACE = 0
        GRP_SUBTYPE_MPLS_LABEL_L2_VPN = 1
        GRP_SUBTYPE_MPLS_LABEL_L3_VPN = 2
        GRP_SUBTYPE_MPLS_LABEL_TUNNEL1 = 3
        GRP_SUBTYPE_MPLS_LABEL_TUNNEL2 = 4
        GRP_SUBTYPE_MPLS_LABEL_SWAP = 5

        # L2 Interface Bucket List:
        # Add bucket list entry to group 0x65000a, index 1, output port 6, do not send untagged
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 group-mod cmd=add,type=ind,group=0x650006 weight=0,port=any,group=any output=6
        '''

        grp_type  = GRP_TYPE_L2_INTERFACE
        l2_vlan_id = 101
        l2_port = 6

        actions = []
        buckets = []
        
        l2_interface_group_id = encode_l2_interface_gid(grp_type, l2_vlan_id, l2_port)

        action = dp.ofproto_parser.OFPActionOutput(l2_port)
        actions.append(action)        

        bucket = dp.ofproto_parser.OFPBucket(
            weight = 0,
            watch_port = dp.ofproto.OFPP_ANY,
            watch_group =  dp.ofproto.OFPG_ANY,
            actions = actions
        )
        buckets.append(bucket)

        l2_interface_group = dp.ofproto_parser.OFPGroupMod(
            dp,
            dp.ofproto.OFPGC_ADD,
            dp.ofproto.OFPGT_INDIRECT,
            l2_interface_group_id,
            buckets
            )

        dp.send_msg(l2_interface_group)
        print "message sent"

        # MPLS Interface Bucket List:
        # Add bucket list entry to group 0x90000000, index 1, VLAN 101, reference group is the above L2 Interface group
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 group-mod cmd=add,type=ind,group=0x90000000 weight=0,port=any,group=any 
        set_field=vlan_vid:101,set_field=eth_src:00:01:02:03:04:05,set_field=eth_dst:00:01:02:03:04:06,group=0x650006
        '''

        actions = []
        buckets = []
        
        action = dp.ofproto_parser.OFPActionSetField(vlan_vid=l2_vlan_id)
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionSetField(eth_src="00:01:02:03:04:05")
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionSetField(eth_dst="00:01:02:03:04:06")
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionGroup(l2_interface_group_id)
        actions.append(action)

        bucket = dp.ofproto_parser.OFPBucket(
            weight = 0,
            watch_port =  dp.ofproto.OFPP_ANY,
            watch_group =  dp.ofproto.OFPG_ANY,
            actions = actions
        )
        buckets.append(bucket)

        grp_type = GRP_TYPE_MPLS_LABEL
        grp_subtype = GRP_SUBTYPE_MPLS_LABEL_INTERFACE
        grp_index = 0

        mpls_interface_group_id = encode_mpls_gid(grp_type, grp_subtype, grp_index)

        mpls_interface_group = dp.ofproto_parser.OFPGroupMod(
            dp,
            dp.ofproto.OFPGC_ADD,
            dp.ofproto.OFPGT_INDIRECT,
            mpls_interface_group_id,
            buckets
            )

        dp.send_msg(mpls_interface_group)
        print "message sent"

        # MPLS Tunnel Label 1 Bucket List:
        # Add bucket list entry to group 0x91000000, index 1, BOS, VPN initiation label 0xff, reference group is above MPLS Interface group
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 group-mod cmd=add,type=ind,group=0x93000000 weight=0,port=any,group=any 
        push_mpls=0x8847,set_field=mpls_label:0x1ff,set_field=mpls_bos:0,exp=ofdpa:copy_tc_out,ttl_out,group=0x90000000
        '''

        actions = []
        buckets = []

        action = dp.ofproto_parser.OFPActionPushMpls(0x8847)
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionSetField(mpls_label=0x1ff)
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionSetField(mpls_bos=0x1ff)
        actions.append(action)
        
        action = dp.ofproto_parser.OFDPAActionCopyTcOut()
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionCopyTtlOut()
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionGroup(mpls_interface_group_id)
        actions.append(action)

       
        bucket = dp.ofproto_parser.OFPBucket(
            weight = 0,
            watch_port =  dp.ofproto.OFPP_ANY,
            watch_group =  dp.ofproto.OFPG_ANY,
            actions = actions
        )
        buckets.append(bucket)

        grp_type = GRP_TYPE_MPLS_LABEL
        grp_subtype = GRP_SUBTYPE_MPLS_LABEL_TUNNEL1
        grp_index = 0

        mpls_tunnel_1_group_id = encode_mpls_gid(grp_type, grp_subtype, grp_index)
        
        mpls_tunnel_1_group = dp.ofproto_parser.OFPGroupMod(
            dp,
            dp.ofproto.OFPGC_ADD,
            dp.ofproto.OFPGT_INDIRECT,
            mpls_tunnel_1_group_id,
            buckets
            )

        dp.send_msg(mpls_tunnel_1_group)
        print "message sent"
        
        # MPLS L2 VPN Label Bucket List:
        # Add bucket list entry to group 0x91000000, index 1, BOS, VPN initiation label 0xff, reference group is above MPLS Tunnel group
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 group-mod cmd=add,type=ind,group=0x91000000 weight=0,port=any,group=any 
        exp=ofdpa:push_l2hdr,push_mpls=0x8847,exp=ofdpa:push_cw,set_field=mpls_label:0xff,set_field=mpls_bos:1,set_field=mpls_tc:4,,mpls_ttl=14,group=0x93000000
        '''


        actions = []
        buckets = []

        action = dp.ofproto_parser.OFDPAActionPushL2Header()
        actions.append(action)

        action = dp.ofproto_parser.OFPActionPushMpls(0x8847)
        actions.append(action)
        
        action = dp.ofproto_parser.OFDPAActionPushCw()
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionSetField(mpls_label=0x1ff)
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionSetField(mpls_bos=1)
        actions.append(action)

        action = dp.ofproto_parser.OFPActionSetField(mpls_tc=4)
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionSetMplsTtl(14)
        actions.append(action)                
        
        action = dp.ofproto_parser.OFPActionGroup(mpls_tunnel_1_group_id)
        actions.append(action)
       
        bucket = dp.ofproto_parser.OFPBucket(
            weight = 0,
            watch_port =  dp.ofproto.OFPP_ANY,
            watch_group =  dp.ofproto.OFPG_ANY,
            actions = actions
        )
        buckets.append(bucket)

        grp_type = GRP_TYPE_MPLS_LABEL
        grp_subtype = GRP_SUBTYPE_MPLS_LABEL_L2_VPN
        grp_index = 0

        mpls_label_l2_group_id = encode_mpls_gid(grp_type, grp_subtype, grp_index)
        
        mpls_label_l2_group = dp.ofproto_parser.OFPGroupMod(
            dp,
            dp.ofproto.OFPGC_ADD,
            dp.ofproto.OFPGT_INDIRECT,
            mpls_label_l2_group_id,
            buckets
            )

        dp.send_msg(mpls_label_l2_group)
        print "message sent"
        
        # MPLS L2 Port Flow
        # MPLS L2 Port 1 (type VPWS), goto table Policy ACL, group MPLS L2 VPN Label (above)
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 flow-mod cmd=add,table=13 ofdpa_mpls_l2_port=0x1,tunn_id=0x10001 goto:60 write:group=0x91000000
        '''

        flow_table = TABLE_MPLS_L2_PORT
        goto_table = TABLE_POLICY_ACL

        fields = {}
        fields['ofdpa_l2_port'] = 0x01
        fields['tunnel_id'] = 0x10001
        matches = dp.ofproto_parser.OFPMatch(**fields)
        
        instructions = []
        actions = []
        
        action = dp.ofproto_parser.OFPActionGroup(mpls_label_l2_group_id)
        actions.append(action)
        
        instruction = dp.ofproto_parser.OFPInstructionActions(dp.ofproto.OFPIT_WRITE_ACTIONS, actions)
        instructions.append(instruction)
        
        instruction = dp.ofproto_parser.OFPInstructionGotoTable(goto_table)
        instructions.append(instruction)
        
        mpls_l2_port_flow = dp.ofproto_parser.OFPFlowMod (
            dp,
            cookie = 0,
            cookie_mask = 0,
            table_id = flow_table,
            command = dp.ofproto.OFPFC_ADD,
            idle_timeout = 0,
            hard_timeout = 0,
            priority = 0,
            buffer_id = 0,
            out_port = dp.ofproto.OFPP_ANY,
            out_group = dp.ofproto.OFPP_ANY,
            flags=0,
            match=matches,
            instructions=instructions
            )
        dp.send_msg(mpls_l2_port_flow)
        print "message sent"
        
        # VLAN Flow
        # Create a VLAN flow on interface 9 (set ingress VLAN pbmp), all VLANs (ID and mask are 0), goto table MPLS L2 Port
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 flow-mod cmd=add,table=10 in_port=5 apply:set_field=ofdpa_mpls_l2_port:0x1,set_field=tunn_id:0x10001 goto:13
        '''

        flow_table = TABLE_VLAN
        goto_table = TABLE_MPLS_L2_PORT
        
        fields = {}
        fields['in_port'] = 0x01
        matches = dp.ofproto_parser.OFPMatch(**fields)
        
        instructions = []
        actions = []
    
        action = dp.ofproto_parser.OFPActionSetField(ofdpa_l2_port=0x1)
        actions.append(action)    
        
        action = dp.ofproto_parser.OFPActionSetField(tunnel_id=0x10001)
        actions.append(action)    
        
        instruction = dp.ofproto_parser.OFPInstructionActions(dp.ofproto.OFPIT_APPLY_ACTIONS, actions)
        instructions.append(instruction)        
        
        instruction = dp.ofproto_parser.OFPInstructionGotoTable(goto_table)
        instructions.append(instruction)        
        
        vlan_flow = dp.ofproto_parser.OFPFlowMod (
            dp,
            cookie = 0,
            cookie_mask = 0,
            table_id = flow_table,
            command = dp.ofproto.OFPFC_ADD,
            idle_timeout = 0,
            hard_timeout = 0,
            priority = 0,
            buffer_id = 0,
            out_port = dp.ofproto.OFPP_ANY,
            out_group = dp.ofproto.OFPP_ANY,
            flags=0,
            match=matches,
            instructions=instructions
            )
        dp.send_msg(vlan_flow)
        print "message sent"




