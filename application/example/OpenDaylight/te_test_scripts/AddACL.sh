#!/bin/sh

#This script is assumed to be run on the host where ODL controller is running.
#To run the scripts from a remote machine, change the controller IP accordingly.
controller_ip=127.0.0.1
#Node ID: When OpenDayLight controller starts, this is the switch ID known 
#to the controller. Check "printNodes" at the OSGI interface
openflow_node=openflow:55930

#PUT Policy ACL flow
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_acl.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/table/60/flow/1
