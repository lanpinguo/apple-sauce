#! /usr/bin/env python

"""
  Example API calls to set up flows in the Policy ACL flow table to match all packets entering
  this port with an Output CONTROLLER action.

  Note that this is just one of many ways the OF-DPA pipeline can be programmed to direct a flow
  to the CONTROLLER port.

  This script serves as a simple example illustrating how these APIs may be used.
"""
from OFDPA_python import *

def main():
    rc = ofdpaClientInitialize("ACL Output CONTROLLER")
    if rc == OFDPA_E_NONE:

        # variables used
        inPortNum = 1

        # set up Policy ACL flow entry to send matched frames to CONTROLLER port
        aclFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_ACL_POLICY, aclFlowEntry)
        aclFlowEntry.flowData.policyAclFlowEntry.match_criteria.inPort = inPortNum
        aclFlowEntry.flowData.policyAclFlowEntry.match_criteria.inPortMask = (OFDPA_INPORT_EXACT_MASK)
        aclFlowEntry.flowData.policyAclFlowEntry.match_criteria.etherTypeMask = (OFDPA_ETHERTYPE_ALL_MASK)
        aclFlowEntry.flowData.policyAclFlowEntry.outputPort = (OFDPA_PORT_CONTROLLER)

        rc = ofdpaFlowAdd(aclFlowEntry)
        if rc != OFDPA_E_NONE:
          print "Error from ofdpaFlowAdd(). rc = ", rc

    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
