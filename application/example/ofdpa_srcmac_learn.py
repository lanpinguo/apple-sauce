#! /usr/bin/env python

""" 
Set Source Mac Learning Mode to enable Controller Managed Learning
Modes:
  1 = Enable - Copy the packets with unknown source address to CPU.
  0 = Disable - Switch the packets with unknown source address.


This script invokes OF-DPA API services via RPC. The RPC calls are served by the ofdpa
process running on the switch.

"""
from OFDPA_python import *

def main():
    rc = ofdpaClientInitialize("OFDPA_example")
    if rc == OFDPA_E_NONE:
        print """Please select mode:
                 1 = Enable - Copy the packets with unknown source address to CPU.
                 0 = Disable - Switch the packets with unknown source address."""

        mode = raw_input('Enter mode: ')
        mode = int(mode)

        srcMacLearnModeCfg = ofdpaSrcMacLearnModeCfg_t()
        srcMacLearnModeCfg.destPortNum = OFDPA_PORT_CONTROLLER # Allowed destination port is only Controller port

        rc = ofdpaSourceMacLearningSet(mode, srcMacLearnModeCfg)
        if rc != OFDPA_E_NONE:
            print "Could not set Source Mac Learn Mode. rc = %d" %(rc)
            return   
        print "Source Mac Learn Mode set successfully."

        mode_p = new_OFDPA_CONTROL_tp()	
        srcMacLearnModeCfg.destPortNum = 0
        rc = ofdpaSourceMacLearningGet(mode_p, srcMacLearnModeCfg)
        if rc != OFDPA_E_NONE:
            print "Could not get Source Mac Learn Mode. rc = %d" %(rc)
            return

        print "Mode: %d" %(OFDPA_CONTROL_tp_value(mode_p))
        print "Destination Port: ",
        if srcMacLearnModeCfg.destPortNum == OFDPA_PORT_CONTROLLER:
            print "CONTROLLER"
        else:
            print "%d" %(srcMacLearnModeCfg.destPortNum)	

        #cleanup
        delete_OFDPA_CONTROL_tp(mode_p)
			
    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
