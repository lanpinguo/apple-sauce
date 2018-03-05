#### Termination Flow ####


from ryu.base import app_manager
from ryu.controller import dpset
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ofproto_v1_3
import sys

class VPWS_Termination(app_manager.RyuApp):

    _CONTEXTS = {'dpset': dpset.DPSet}
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(VPWS_Termination, self).__init__(*args, **kwargs)

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
            return gid
           
        def encode_l3_multicast_gid(grp_type, vlan_id, index):
            grp_type = grp_type & 0x0f
            vlan_id = vlan_id & 0x0fff
            index = index & 0xffff
            gid = grp_type << 28 | vlan_id << 16 |index
            print 'gid %x' % gid
            return gid

        def encode_mpls_gid(grp_type, grp_subtype, index):
            grp_type = grp_type & 0x0f
            grp_subtype = grp_subtype & 0x0f
            index = index & 0x00ffffff
            gid = grp_type << 28 | grp_subtype << 24 | index
            print 'gid %x' % gid
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
        GRP_TYPE_L2_UNFILTERED_INTERFACE = 11

        GRP_SUBTYPE_MPLS_LABEL_INTERFACE = 0
        #
        GRP_SUBTYPE_MPLS_LABEL_L2_VPN = 1
        GRP_SUBTYPE_MPLS_LABEL_L3_VPN = 2
        GRP_SUBTYPE_MPLS_LABEL_TUNNEL1 = 3
        GRP_SUBTYPE_MPLS_LABEL_TUNNEL2 = 4
        GRP_SUBTYPE_MPLS_LABEL_SWAP = 5

        # L2 Unfiltered Interface Bucket List:
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 group-mod cmd=add,type=ind,group=0xb0000005 weight=0,port=any,group=any output=5
        '''
        grp_type  = GRP_TYPE_L2_UNFILTERED_INTERFACE
        l2_vlan_id = 0
        l2_port = 5
        
        l2_unfiltered_interface_group_id = encode_l2_interface_gid(grp_type, l2_vlan_id, l2_port)

        actions = []
        buckets = []

        action = dp.ofproto_parser.OFPActionOutput(l2_port)
        actions.append(action)

        bucket = dp.ofproto_parser.OFPBucket(
            weight = 0,
            watch_port = dp.ofproto.OFPP_ANY,
            watch_group =  dp.ofproto.OFPG_ANY,
            actions = actions
        )
        buckets.append(bucket)

        l2_unfiltered_interface_group = dp.ofproto_parser.OFPGroupMod(
            dp,
            dp.ofproto.OFPGC_ADD,
            dp.ofproto.OFPGT_INDIRECT,
            l2_unfiltered_interface_group_id,
            buckets
            )

        dp.send_msg(l2_unfiltered_interface_group)
        print "message sent"

        # MPLS Flow 1
        # Add MPLS flow to match tunnel termination label 0x177
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 flow-mod cmd=add,table=24 mpls_label=0x177,mpls_bos=0 apply:pop_mpls=0x8847,mpls_dec,ttl_in,exp=ofdpa:copy_tc_in goto:25
        '''
        
        flow_table = TABLE_MPLS1
        goto_table = TABLE_MPLS2
        
        fields = {}
        fields['eth_type'] = 0x8847
        fields['mpls_label'] = 0x177
        fields['mpls_bos'] = 0
        matches = dp.ofproto_parser.OFPMatch(**fields)
        
        instructions = []
        actions = []
    
        action = dp.ofproto_parser.OFPActionPopMpls(0x8847)
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionDecMplsTtl()
        actions.append(action)
        
        action = dp.ofproto_parser.OFDPAActionCopyTcIn()
        actions.append(action)        
        
        action = dp.ofproto_parser.OFPActionCopyTtlIn()
        actions.append(action)
        
        instruction = dp.ofproto_parser.OFPInstructionActions(dp.ofproto.OFPIT_APPLY_ACTIONS, actions)
        instructions.append(instruction)
        
        instruction = dp.ofproto_parser.OFPInstructionGotoTable(goto_table)
        instructions.append(instruction)
        
        flow = dp.ofproto_parser.OFPFlowMod (
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
        dp.send_msg(flow)
        print "message sent"

        # MPLS Flow 1
        # Add MPLS flow to match VPN termination label 0x77 and set the L2 Unfiltered Interface group above
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 flow-mod cmd=add,table=24 
        mpls_label=0x77,mpls_bos=1 apply:pop_mpls=0x8847,mpls_dec,set_field=ofdpa_mpls_l2_port:0x20001,set_field=tunn_id:0x10001,exp=ofdpa:pop_cw write:group=0xb0000005 goto:60
        '''
        
        flow_table = TABLE_MPLS1
        goto_table = TABLE_POLICY_ACL
        
        fields = {}
        fields['eth_type'] = 0x8847
        fields['mpls_label'] = 0x77
        fields['mpls_bos'] = 1 
        matches = dp.ofproto_parser.OFPMatch(**fields)
        
        instructions = []
        actions = []

        action = dp.ofproto_parser.OFPActionPopMpls(0x8847)
        actions.append(action)
        
        action = dp.ofproto_parser.OFDPAActionPopL2Header()
        actions.append(action)        
        
        action = dp.ofproto_parser.OFPActionDecMplsTtl()
        actions.append(action)
        
        action = dp.ofproto_parser.OFPActionSetField(ofdpa_l2_port=0x20001)
        actions.append(action)

        action = dp.ofproto_parser.OFPActionSetField(tunnel_id=0x10001)
        actions.append(action)

        action = dp.ofproto_parser.OFDPAActionPopCw()
        actions.append(action)
        
        instruction = dp.ofproto_parser.OFPInstructionActions(dp.ofproto.OFPIT_APPLY_ACTIONS, actions)
        instructions.append(instruction)
        
        actions = []
        
        action = dp.ofproto_parser.OFPActionGroup(l2_unfiltered_interface_group_id)
        actions.append(action)
        
        instruction = dp.ofproto_parser.OFPInstructionActions(dp.ofproto.OFPIT_WRITE_ACTIONS, actions)
        instructions.append(instruction)
        
        instruction = dp.ofproto_parser.OFPInstructionGotoTable(goto_table)
        instructions.append(instruction)
        
        flow = dp.ofproto_parser.OFPFlowMod (
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
        dp.send_msg(flow)
        print "message sent"
        
        # Termination MAC Flow
        # Set termination of MPLS for dest MAC (src in MPLS Intf), port 6
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 flow-mod cmd=add,table=20 in_port=6,eth_type=0x8847,vlan_vid=101,eth_dst=00:01:02:03:04:05/ff:ff:ff:ff:ff:ff goto:23
        '''
        flow_table = TABLE_TERMINATION_MAC
        goto_table = TABLE_MPLS0
        
        fields = {}
        fields['in_port'] = 0x06
        fields['vlan_vid'] = 101
        fields['eth_type'] = 0x8847
        fields['eth_dst'] = '00:01:02:03:04:05'
        matches = dp.ofproto_parser.OFPMatch(**fields)
        
        instructions = []
        actions = []
        
        instruction = dp.ofproto_parser.OFPInstructionGotoTable(goto_table)
        instructions.append(instruction)        
        
        flow = dp.ofproto_parser.OFPFlowMod (
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
        dp.send_msg(flow)
        print "message sent"
        
        # VLAN Flow
        # Create a VLAN flow on interface 10 (set ingress VLAN pbmp), VLAN 101 (tagged), goto table Term MAC (default)
        '''
        ./utilities/dpctl tcp:10.156.12.164:6634 flow-mod cmd=add,table=10 in_port=6,vlan_vid=0x65 goto:20
        '''
        
        flow_table = TABLE_VLAN
        goto_table = TABLE_TERMINATION_MAC
        
        fields = {}
        fields['in_port'] = 0x06
        fields['vlan_vid'] = (0x1065,0x1fff)
        matches = dp.ofproto_parser.OFPMatch(**fields)
        
        instructions = []
        actions = []
       
        instruction = dp.ofproto_parser.OFPInstructionGotoTable(goto_table)
        instructions.append(instruction)        
        
        flow = dp.ofproto_parser.OFPFlowMod (
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
        dp.send_msg(flow)
        print "message sent"
