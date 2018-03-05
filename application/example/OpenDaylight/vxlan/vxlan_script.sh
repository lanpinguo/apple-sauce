#!/bin/sh

set -x

# Script to set up VxLAN tunnel using OF-DPA
of_controller="10.18.20.219";
CONTROLLER_PORT=8181;


set up L2 Interface group on next hop physical ports
curl -i -v -u admin:admin -H "Content-Type: application/json" -X PUT -d @l2_intf_group.json http://10.18.20.219:8181/restconf/config/opendaylight-inventory:nodes/node/openflow:55930/group/13107202

set up VLAN flow admitting tagged traffic to next port
curl -i -v -u admin:admin -H "Content-Type: application/json" -X PUT -d @vlanFlow.json http://10.18.20.219:8181/restconf/config/opendaylight-inventory:nodes/node/openflow:55930/table/10/flow/1
curl -i -v -u admin:admin -H "Content-Type: application/json" -X PUT -d @vlanFlow_1.json http://10.18.20.219:8181/restconf/config/opendaylight-inventory:nodes/node/openflow:55930/table/10/flow/2

#set up ingress port flow
curl -i -v -u admin:admin -H "Content-Type: application/json" -X PUT -d @ingressPortFlow.json http://10.18.20.219:8181/restconf/config/opendaylight-inventory:nodes/node/openflow:55930/table/0/flow/1


curl -i -v -u admin:admin -H "Content-Type: application/json" -X PUT -d @bridgingFlow.json http://10.18.20.219:8181/restconf/config/opendaylight-inventory:nodes/node/openflow:55930/table/50/flow/1

curl -i -v -u admin:admin -H "Content-Type: application/json" -X PUT -d @bridgingFlow_1.json http://10.18.20.219:8181/restconf/config/opendaylight-inventory:nodes/node/openflow:55930/table/50/flow/2
