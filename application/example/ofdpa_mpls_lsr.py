#! /usr/bin/env python

""" 
This script sets up MPLS LSR forwarding by configuring the associated flows and groups.

Ingress packet configuration:
Destination MAC = 'localMacAddress'
VLAN Id = 'srcVlanId'
Ingress port = 'srcPort'
Ethertype = 'etherType'
Topmost MPLS label
  Value = 'srclabel'
  BOS = 0
Inner MPLS label, if any
  Value = someValue
  BOS = 1
  
Switch MPLS LSR configuration is done in main().
Incoming topmost label 'srclabel' is swapped with label 'destlabel'.

Expected Egress packet:
Destination MAC = 'destMacAddress'
Source MAC = 'localMacAddress'
VLAN Id = 'destVlanId'
Topmost label value = 'destlabel'
Topmost label TTL should be decremented by 1.
Egress port = 'destPort'
Inner label stays as it is
  
This script invokes OF-DPA API services via RPC. The RPC calls are served by the ofdpa
process running on the switch.

"""
from OFDPA_python import *

def main():
    rc = ofdpaClientInitialize("OFDPA_example")
    if rc == OFDPA_E_NONE:

        # Variables used to set up MPLS LSR scenario
        destVlanId = 1
        srcVlanId = 2
        destPort = 2
        srcPort = 3
        localMacAddress = "00:01:02:03:04:05"
        destMacAddress = "00:01:03:05:07:09"
        index1 = 1
        destlabel = 0x1ff
        srclabel = 0x177
        etherType = 0x8847
        
        
        # Set up L2 Interface group. This configures the egress port.

        l2IntfGroupId = new_uint32_tp()

        l2IfaceGroupEntry  = ofdpaGroupEntry_t()
        l2IfaceGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(l2IntfGroupId, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
        ofdpaGroupVlanSet(l2IntfGroupId, destVlanId)
        ofdpaGroupPortIdSet(l2IntfGroupId, destPort)

        l2IfaceGroupEntry.groupId = uint32_tp_value(l2IntfGroupId)

        l2IfaceGroupBucket.groupId = l2IfaceGroupEntry.groupId
        l2IfaceGroupBucket.bucketIndex = 0
        l2IfaceGroupBucket.bucketData.l2Interface.outputPort = destPort

        rc = ofdpaGroupAdd(l2IfaceGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add L2 interface group. rc = %d " % (rc)
          return

        rc = ofdpaGroupBucketEntryAdd(l2IfaceGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add L2 interface group bucket. rc = %d " % (rc)
          return

        # Set up MPLS Interface group. This configures the source and destination MAC and the VLAN ID of the egress packet.

        mplsIntfGroupId = new_uint32_tp()

        mplsIfaceGroupEntry  = ofdpaGroupEntry_t()
        mplsIfaceGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(mplsIntfGroupId, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
        ofdpaGroupMplsSubTypeSet(mplsIntfGroupId, OFDPA_MPLS_INTERFACE)
        ofdpaGroupIndexSet(mplsIntfGroupId, index1)

        mplsIfaceGroupEntry.groupId = uint32_tp_value(mplsIntfGroupId)

        mplsIfaceGroupBucket.groupId = mplsIfaceGroupEntry.groupId
        mplsIfaceGroupBucket.bucketIndex = 0
        mplsIfaceGroupBucket.bucketData.mplsInterface.vlanId = (destVlanId | OFDPA_VID_PRESENT)
        MACAddress_set(mplsIfaceGroupBucket.bucketData.mplsInterface.srcMac, localMacAddress)
        MACAddress_set(mplsIfaceGroupBucket.bucketData.mplsInterface.dstMac, destMacAddress)
        mplsIfaceGroupBucket.referenceGroupId = uint32_tp_value(l2IntfGroupId)

        rc = ofdpaGroupAdd(mplsIfaceGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS interface group. rc = %d " % (rc)
          return
        
        rc = ofdpaGroupBucketEntryAdd(mplsIfaceGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS interface group bucket. rc = %d " % (rc)
          return
       

        # Set up MPLS Swap group. This configures the swap label and the reference MPLS Interface group to be used. 
        
        mplsSwapGroupId = new_uint32_tp()

        mplsSwapGroupEntry  = ofdpaGroupEntry_t()
        mplsSwapGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(mplsSwapGroupId, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
        ofdpaGroupMplsSubTypeSet(mplsSwapGroupId, OFDPA_MPLS_SWAP_LABEL)
        ofdpaGroupIndexSet(mplsSwapGroupId, index1)

        mplsSwapGroupEntry.groupId = uint32_tp_value(mplsSwapGroupId)

        mplsSwapGroupBucket.groupId = mplsSwapGroupEntry.groupId
        mplsSwapGroupBucket.bucketIndex = 0
        mplsSwapGroupBucket.bucketData.mplsLabel.mplsLabel = destlabel
        mplsSwapGroupBucket.referenceGroupId = uint32_tp_value(mplsIntfGroupId)

        rc = ofdpaGroupAdd(mplsSwapGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS swap group. rc = %d " % (rc)
          return
        
        rc = ofdpaGroupBucketEntryAdd(mplsSwapGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS swap group bucket. rc = %d " % (rc)
          return
        

        # Set up MPLS1 flow entry. This configures the matching ingress label and the reference Swap Group to be used for egress swap label.
        mpls1FlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_MPLS_1, mpls1FlowEntry)
        mpls1FlowEntry.flowData.mplsFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_ACL_POLICY
        mpls1FlowEntry.flowData.mplsFlowEntry.groupID = uint32_tp_value(mplsSwapGroupId)
        mpls1FlowEntry.flowData.mplsFlowEntry.decrementTtlAction = 1
        
        mpls1FlowEntry.flowData.mplsFlowEntry.match_criteria.etherType = etherType
        mpls1FlowEntry.flowData.mplsFlowEntry.match_criteria.mplsLabel = srclabel

        rc = ofdpaFlowAdd(mpls1FlowEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS 1 flow entry. rc = %d " % (rc)
          return
        
        # Set up Termination MAC flow entry. This configures the matching destination MAC, VLAN Id and ingress interface of the ingress packet to set it up for MPLS processing.
        
        termMacFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_TERMINATION_MAC, termMacFlowEntry)
        termMacFlowEntry.flowData.terminationMacFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_MPLS_1
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.inPort = srcPort
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.inPortMask = OFDPA_INPORT_EXACT_MASK
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | srcVlanId)
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.etherType = etherType
        MACAddress_set(termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.destMac, localMacAddress)
        MACAddress_set(termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.destMacMask, "ff:ff:ff:ff:ff:ff")

        rc = ofdpaFlowAdd(termMacFlowEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add Termination MAC flow entry. rc = %d " % (rc)
          return

        # Set up VLAN Filtering flow entries. This configures the VLAN participation of the ingress port.
        vlanFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_VLAN, vlanFlowEntry)
        vlanFlowEntry.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.inPort = srcPort
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | srcVlanId)
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)

        rc = ofdpaFlowAdd(vlanFlowEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add VLAN flow entry. rc = %d " % (rc)
          return

        # clean up
        delete_uint32_tp(l2IntfGroupId)
        delete_uint32_tp(mplsIntfGroupId)
        delete_uint32_tp(mplsSwapGroupId)

    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
