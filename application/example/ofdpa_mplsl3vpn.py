#! /usr/bin/env python

""" 
 Create MPLS L3 VPN flows

 Parameters:
      Customer physical port number (default 3)
      Network physical port number (default 4)
      Customer VLAN (default 5)
      Network VLAN (default 6)
      Initiator VPN Label (default 0xff)
      Initiator Tunnel Label (default 0x1ff)
      Terminator VPN Label (default 0x77)
      Terminator Tunnel Label (default 0x177)
     Source MAC (default 00:01:02:03:04:05)
      Destination MAC (default 00:01:03:05:07:09)
      Local Source MAC (default 00:11:22:33:44:55)
      Local Destination MAC (default 00:11:33:55:77:99)
      Source IP (default 17.34.51.68)
      Destination IP (default 17.34.51.85)
      VRF (default 1)

Needed (listed in source -> destination order):
 Initiation Flow:
   VLAN Flow
   Termination MAC Flow
   Unicast Routing Flow --> needs MPLS L3 VPN Label Group
   MPLS L3 VPN Label Group --> needs MPLS Tunnel Label 1 Group
   MPLS Tunnel Label 1 Group --> needs MPLS Interface Group
   MPLS Interface Group --> needs L2 Interface Group
   L2 Interface Group
Termination Flow:
   VLAN Flow
   Termination MAC Flow
   MPLS 1 Flow (inner tunnel label)
   MPLS 1 Flow (VPN label)
   Unicast Routing Flow --> needs L3 Unicast Group
   L3 Unicast Group --> needs L2 Interface Group
   L2 Interface Group

This script invokes OF-DPA API services via RPC. The RPC calls are served by the ofdpa
process running on the switch.

"""
from OFDPA_python import *

# add group and bucket
def addGroupBucket(groupEntry, groupBucket):
    rc = ofdpaGroupAdd(groupEntry)
    if rc != OFDPA_E_NONE:
        print "Failed to add group entry. rc = %d" %(rc)
        return rc

    rc = ofdpaGroupBucketEntryAdd(groupBucket)
    if rc != OFDPA_E_NONE:
        print "Failed to add bucket to the group. rc = %d" %(rc)
        return rc    

    return OFDPA_E_NONE 


# add l2 interface group
def installL2InterfaceGroup(vlan, port, groupId1_p):

    uint32_tp_assign(groupId1_p, 0)
    groupEntry = ofdpaGroupEntry_t()
    groupBucket = ofdpaGroupBucketEntry_t()

    ofdpaGroupTypeSet(groupId1_p, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
    ofdpaGroupVlanSet(groupId1_p, vlan)
    ofdpaGroupPortIdSet(groupId1_p, port)
    groupEntry.groupId = uint32_tp_value(groupId1_p)

    groupBucket.groupId = groupEntry.groupId
    groupBucket.bucketIndex = 0
    groupBucket.bucketData.l2Interface.outputPort = port
    return addGroupBucket(groupEntry, groupBucket)

        
# add mpls interface group
def installMplsInterfaceGroup(index, refGroupId, vlan, srcMac, dstMac, groupId1_p):
    uint32_tp_assign(groupId1_p, 0)
    groupEntry = ofdpaGroupEntry_t()
    groupBucket = ofdpaGroupBucketEntry_t()
        
    ofdpaGroupTypeSet(groupId1_p, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
    ofdpaGroupMplsSubTypeSet(groupId1_p, OFDPA_MPLS_INTERFACE)
    ofdpaGroupIndexSet(groupId1_p, index)
    groupEntry.groupId = uint32_tp_value(groupId1_p)

    groupBucket.groupId = groupEntry.groupId
    groupBucket.bucketIndex = 0
    groupBucket.referenceGroupId = refGroupId
    groupBucket.bucketData.mplsInterface.vlanId = (vlan | OFDPA_VID_PRESENT)
    MACAddress_set(groupBucket.bucketData.mplsInterface.srcMac, srcMac)
    MACAddress_set(groupBucket.bucketData.mplsInterface.dstMac, dstMac)
    return addGroupBucket(groupEntry, groupBucket)

        
# add mpls label group
def installMplsLabelGroup(index, refGroupId, bos, mplsLabel, groupId1_p, subtype):
    uint32_tp_assign(groupId1_p, 0)
    groupEntry = ofdpaGroupEntry_t()
    groupBucket = ofdpaGroupBucketEntry_t()
        
    ofdpaGroupTypeSet(groupId1_p, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
    ofdpaGroupMplsSubTypeSet(groupId1_p, subtype)
    ofdpaGroupIndexSet(groupId1_p, index)
    groupEntry.groupId = uint32_tp_value(groupId1_p)

    groupBucket.groupId = groupEntry.groupId
    groupBucket.bucketIndex = 0
    groupBucket.referenceGroupId = refGroupId
    groupBucket.bucketData.mplsLabel.mplsLabel = mplsLabel
    groupBucket.bucketData.mplsLabel.mplsBOS = bos
    return addGroupBucket(groupEntry, groupBucket)      
        
        
# add l3 unicast group
def installL3UnicastGroup(index, refGroupId, vlan, srcMac, dstMac, groupId1_p):
    uint32_tp_assign(groupId1_p, 0)
    groupEntry = ofdpaGroupEntry_t()
    groupBucket = ofdpaGroupBucketEntry_t()
        
    ofdpaGroupTypeSet(groupId1_p, OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST)
    ofdpaGroupIndexSet(groupId1_p, index)
    groupEntry.groupId = uint32_tp_value(groupId1_p)

    groupBucket.groupId = groupEntry.groupId
    groupBucket.bucketIndex = 0
    groupBucket.referenceGroupId = refGroupId
    groupBucket.bucketData.l3Unicast.vlanId = (vlan | OFDPA_VID_PRESENT)
    MACAddress_set(groupBucket.bucketData.l3Unicast.srcMac, srcMac)
    MACAddress_set(groupBucket.bucketData.l3Unicast.dstMac, dstMac)
    return addGroupBucket(groupEntry, groupBucket)  
    
# initialize vlan flow    
def initializeVlanFlow(port, vlan, flow):
    ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_VLAN, flow)
    flow.flowData.vlanFlowEntry.match_criteria.inPort =  port
    flow.flowData.vlanFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | vlan)
    flow.flowData.vlanFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)
    flow.flowData.vlanFlowEntry.setVlanIdAction = 0

# initialize termination mac flow    
def initializeTermMacFlow(port, vlan, dstMac, etherType, flow):
    ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_TERMINATION_MAC, flow)
    flow.flowData.terminationMacFlowEntry.match_criteria.inPort =  port
    flow.flowData.terminationMacFlowEntry.match_criteria.inPortMask =  OFDPA_INPORT_EXACT_MASK
    flow.flowData.terminationMacFlowEntry.match_criteria.vlanId =  (OFDPA_VID_PRESENT | vlan)
    flow.flowData.terminationMacFlowEntry.match_criteria.vlanIdMask =  (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)
    flow.flowData.terminationMacFlowEntry.match_criteria.etherType =  etherType
    MACAddress_set(flow.flowData.terminationMacFlowEntry.match_criteria.destMac, dstMac)
    MACAddress_set(flow.flowData.terminationMacFlowEntry.match_criteria.destMacMask, "ff:ff:ff:ff:ff:ff")

# initialize unicast routing flow        
def initializeUnicastRoutingFlow(vrf, dstIp4, flow):
    ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING, flow)
    flow.flowData.unicastRoutingFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_ACL_POLICY
    flow.flowData.unicastRoutingFlowEntry.match_criteria.etherType = 0x0800
    flow.flowData.unicastRoutingFlowEntry.match_criteria.vrf = vrf
    flow.flowData.unicastRoutingFlowEntry.match_criteria.vrfMask = OFDPA_VRF_VALUE_MASK
    flow.flowData.unicastRoutingFlowEntry.match_criteria.dstIp4 = dstIp4 
    flow.flowData.unicastRoutingFlowEntry.match_criteria.dstIp4Mask = 0xffffffff

# initialize mpls flow
def initializeMplsFlow(mplsLabel, bos, newEthertype, flow):
    ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_MPLS_1, flow)
    flow.flowData.mplsFlowEntry.match_criteria.etherType = 0x8847
    flow.flowData.mplsFlowEntry.match_criteria.mplsBos = bos
    flow.flowData.mplsFlowEntry.match_criteria.mplsLabel = mplsLabel
    flow.flowData.mplsFlowEntry.decrementTtlAction = 1
    flow.flowData.mplsFlowEntry.popLabelAction = 1
    flow.flowData.mplsFlowEntry.newEtherType = newEthertype
    
    
def main():
    rc = ofdpaClientInitialize("OFDPA_example")
    
    if rc == OFDPA_E_NONE:
        # variables used to set up routing scenario
        cust_port               = 3
        net_port                = 4
        cust_vlan               = 5
        net_vlan                = 6
        vpn_label_init          = 0xff
        tunnel1_label_init      = 0x1ff
        vpn_label_term          = 0x77
        tunnel1_label_term      = 0x177
        source_mac              = "00:01:02:03:04:05"
        destination_mac         = "00:01:03:05:07:09"
        local_source_mac        = "00:11:22:33:44:55"
        local_destination_mac   = "00:11:33:55:77:99"
        vrf                     = 1
        source_ip               = 0x11223344 # 17.34.51.68
        dest_ip                 = 0x11223355 # 17.34.51.85
        BOS_FALSE               = 0
        BOS_TRUE                = 1
        
    # MPLS L3 VPN initiation flows
        # install groups
        l2InterfaceGroupId_p = new_uint32_tp()
        rc = installL2InterfaceGroup(net_vlan, net_port, l2InterfaceGroupId_p)
        if rc != OFDPA_E_NONE:
            print "Installation of L2 interface group failed. rc = %d" %(rc)
            return
        
        mplsInterfaceGroupId_p = new_uint32_tp()
        rc = installMplsInterfaceGroup(0, uint32_tp_value(l2InterfaceGroupId_p), net_vlan, source_mac, destination_mac, mplsInterfaceGroupId_p)
        if rc != OFDPA_E_NONE:
            print "Installation of MPLS interface group failed. rc = %d" %(rc)
            return        

        mplsTunnelLabelGroupId_p = new_uint32_tp()
        rc = installMplsLabelGroup(0, uint32_tp_value(mplsInterfaceGroupId_p), BOS_FALSE, tunnel1_label_init, mplsTunnelLabelGroupId_p, OFDPA_MPLS_TUNNEL_LABEL1)
        if rc != OFDPA_E_NONE:
            print "Installation of MPLS tunnel label group failed. rc = %d" %(rc)
            return        
        
        mplsL3VpnLabelGroupId_p = new_uint32_tp()
        rc = installMplsLabelGroup(0, uint32_tp_value(mplsTunnelLabelGroupId_p), BOS_TRUE, vpn_label_init, mplsL3VpnLabelGroupId_p, OFDPA_MPLS_L3_VPN_LABEL)
        if rc != OFDPA_E_NONE:
            print "Installation of MPLS L3 VPN label group failed. rc = %d" %(rc)
            return        
        
        # vlan flow
        vlanPortFlow = ofdpaFlowEntry_t()
        initializeVlanFlow(cust_port, cust_vlan, vlanPortFlow)
        vlanPortFlow.flowData.vlanFlowEntry.vrfAction = 1
        vlanPortFlow.flowData.vlanFlowEntry.vrf = vrf
        vlanPortFlow.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC
        rc = ofdpaFlowAdd(vlanPortFlow)
        if rc != OFDPA_E_NONE:
            print "Installation of vlan flow failed. rc = %d" %(rc)
            return                
        
        # termination mac flow 
        terminationMacFlow = ofdpaFlowEntry_t()
        initializeTermMacFlow(cust_port, cust_vlan, local_destination_mac, 0x0800, terminationMacFlow)
        terminationMacFlow.flowData.terminationMacFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING
        rc = ofdpaFlowAdd(terminationMacFlow)
        if rc != OFDPA_E_NONE:
            print "Installation of termination mac flow failed. rc = %d" %(rc)
            return        
        
        # unicast routing flow
        unicastRoutingFlow = ofdpaFlowEntry_t()
        initializeUnicastRoutingFlow(vrf, dest_ip, unicastRoutingFlow)
        unicastRoutingFlow.flowData.unicastRoutingFlowEntry.groupID = uint32_tp_value(mplsL3VpnLabelGroupId_p)
        rc = ofdpaFlowAdd(unicastRoutingFlow)
        if rc != OFDPA_E_NONE:
            print "Installation of unicast routing flow failed. rc = %d" %(rc)
            return        
        
    # MPLS L3 VPN termination flows
        # install groups
        l2InterfaceGroupIdTerm_p = new_uint32_tp()
        rc = installL2InterfaceGroup(cust_vlan, cust_port, l2InterfaceGroupIdTerm_p)
        if rc != OFDPA_E_NONE:
            print "Installation L2 interface group failed. rc = %d" %(rc)
            return        
        
        l3UnicastGroupIdTerm_p  = new_uint32_tp()
        rc = installL3UnicastGroup(0, uint32_tp_value(l2InterfaceGroupIdTerm_p), cust_vlan, local_destination_mac, local_source_mac, l3UnicastGroupIdTerm_p)
        if rc != OFDPA_E_NONE:
            print "Installation of L3 unicast group failed. rc = %d" %(rc)
            return        

        # vlan flow
        vlanPortTermFlow = ofdpaFlowEntry_t()
        initializeVlanFlow(net_port, net_vlan, vlanPortTermFlow)
        vlanPortTermFlow.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC
        rc = ofdpaFlowAdd(vlanPortTermFlow)
        if rc != OFDPA_E_NONE:
            print "Installation of vlan flow for MPLS L3 VPN termination flow failed. rc = %d" %(rc)
            return                        
        
        # termination mac flow
        terminationMacTermFlow = ofdpaFlowEntry_t()
        initializeTermMacFlow(net_port, net_vlan, source_mac, 0x8847, terminationMacTermFlow)
        terminationMacTermFlow.flowData.terminationMacFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_MPLS_0
        rc = ofdpaFlowAdd(terminationMacTermFlow)        
        if rc != OFDPA_E_NONE:
            print "Installation of termination mac for MPLS L3 VPN termination flow failed. rc = %d" %(rc)
            return            
        
        # mpls tunnel flow
        mplsFirstTermFlow = ofdpaFlowEntry_t()
        initializeMplsFlow(tunnel1_label_term, BOS_FALSE, 0x8847, mplsFirstTermFlow)
        mplsFirstTermFlow.flowData.mplsFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_MPLS_2
        rc = ofdpaFlowAdd(mplsFirstTermFlow)
        if rc != OFDPA_E_NONE:
            print "Installation of MPLS tunnel flow failed. rc = %d" %(rc)
            return                    
        
        # mpls l3 vpn flow
        mplsSecondTermFlow = ofdpaFlowEntry_t()
        initializeMplsFlow(vpn_label_term, BOS_TRUE, 0x0800, mplsSecondTermFlow)
        mplsSecondTermFlow.flowData.mplsFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING
        mplsSecondTermFlow.flowData.mplsFlowEntry.match_criteria.mplsDataFirstNibble = OFDPA_MPLS_DATA_FIRST_NIBBLE_IPV4
        mplsSecondTermFlow.flowData.mplsFlowEntry.match_criteria.mplsDataFirstNibbleMask = OFDPA_MPLS_DATA_FIRST_NIBBLE_EXACT_MASK
        mplsSecondTermFlow.flowData.mplsFlowEntry.vrf = vrf
        mplsSecondTermFlow.flowData.mplsFlowEntry.vrfAction = 1
        rc = ofdpaFlowAdd(mplsSecondTermFlow)
        if rc != OFDPA_E_NONE:
            print "Installation of MPLS VPN flow failed. rc = %d" %(rc)
            return                            
        
        # unicast routing flow
        unicastRoutingTermFlow = ofdpaFlowEntry_t()
        initializeUnicastRoutingFlow(vrf, source_ip, unicastRoutingTermFlow)
        unicastRoutingTermFlow.flowData.unicastRoutingFlowEntry.match_criteria.vrf = vrf
        unicastRoutingTermFlow.flowData.unicastRoutingFlowEntry.match_criteria.vrfMask = OFDPA_VRF_VALUE_MASK
        unicastRoutingTermFlow.flowData.unicastRoutingFlowEntry.groupID = uint32_tp_value(l3UnicastGroupIdTerm_p)
        rc = ofdpaFlowAdd(unicastRoutingTermFlow)
        if rc != OFDPA_E_NONE:
            print "Installation of unicast routing for MPLS L3 VPN flow failed. rc = %d" %(rc)
            return                
        
        # if we are here flows were added in proper way
        print "MPLS L3 VPN flows are configured successfully"
        
        # clean up
        delete_uint32_tp(l2InterfaceGroupId_p)
        delete_uint32_tp(mplsInterfaceGroupId_p)
        delete_uint32_tp(mplsTunnelLabelGroupId_p)
        delete_uint32_tp(mplsL3VpnLabelGroupId_p)
        
        delete_uint32_tp(l2InterfaceGroupIdTerm_p)
        delete_uint32_tp(l3UnicastGroupIdTerm_p)

    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
