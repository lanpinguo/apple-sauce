#!/bin/sh

#This script is assumed to be run on the host where ODL controller is running.
#To run the scripts from a remote machine, change the controller IP accordingly.
controller_ip=127.0.0.1
#Node ID: When OpenDayLight controller starts, this is the switch ID known 
#to the controller. Check "printNodes" at the OSGI interface
openflow_node=openflow:55930


#PUT Vlan Flow 1
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_vlan_1.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/table/10/flow/1

#PUT Vlan Flow 2
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_vlan_2.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/table/10/flow/2


#PUT Vlan Flow 3
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_vlan_5.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/table/10/flow/3

#PUT L2 Interface Group 1
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_group_l2_intf_0xb0003.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/group/720899


#PUT L2 Interface Group 2 
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_group_l2_intf_0xb0004.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/group/720900


#PUT Termination Mac Flow Table 
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_termination_mac.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/table/20/flow/1

#PUT L3 Unicast Group
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_group_l3_ucast_0x20000001.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/group/536870913


#PUT L3 Unicast Group
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_group_l3_ucast_0x20000002.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/group/536870914

#PUT L3 ECMP Group
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_group_l3_ecmp_0x70000001.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/group/1879048193

#PUT Unicast Routing Group
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_ucast_routing.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/table/30/flow/1

#PUT Policy ACL flow
curl -v -H "Content-Type: application/json" -H "Accept: application/json" -X PUT --data "@$(pwd)/../conf/te_tests/te_add_acl.json" http://$controller_ip:8080/restconf/config/opendaylight-inventory:nodes/node/$openflow_node/table/60/flow/1

