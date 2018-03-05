/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"


void
of_oxm_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 0)); /* type_len */
        switch (value) {
        case 0x30010:
            *id = OF_OXM_BSN_IN_PORTS_128;
            break;
        case 0x30120:
            *id = OF_OXM_BSN_IN_PORTS_128_MASKED;
            break;
        case 0x30204:
            *id = OF_OXM_BSN_LAG_ID;
            break;
        case 0x30308:
            *id = OF_OXM_BSN_LAG_ID_MASKED;
            break;
        case 0x30404:
            *id = OF_OXM_BSN_VRF;
            break;
        case 0x30508:
            *id = OF_OXM_BSN_VRF_MASKED;
            break;
        case 0x30601:
            *id = OF_OXM_BSN_GLOBAL_VRF_ALLOWED;
            break;
        case 0x30702:
            *id = OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED;
            break;
        case 0x30804:
            *id = OF_OXM_BSN_L3_INTERFACE_CLASS_ID;
            break;
        case 0x30908:
            *id = OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED;
            break;
        case 0x30a04:
            *id = OF_OXM_BSN_L3_SRC_CLASS_ID;
            break;
        case 0x30b08:
            *id = OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED;
            break;
        case 0x30c04:
            *id = OF_OXM_BSN_L3_DST_CLASS_ID;
            break;
        case 0x30d08:
            *id = OF_OXM_BSN_L3_DST_CLASS_ID_MASKED;
            break;
        case 0x30e04:
            *id = OF_OXM_BSN_EGR_PORT_GROUP_ID;
            break;
        case 0x30f08:
            *id = OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED;
            break;
        case 0x31004:
            *id = OF_OXM_BSN_UDF0;
            break;
        case 0x31108:
            *id = OF_OXM_BSN_UDF0_MASKED;
            break;
        case 0x31204:
            *id = OF_OXM_BSN_UDF1;
            break;
        case 0x31308:
            *id = OF_OXM_BSN_UDF1_MASKED;
            break;
        case 0x31404:
            *id = OF_OXM_BSN_UDF2;
            break;
        case 0x31508:
            *id = OF_OXM_BSN_UDF2_MASKED;
            break;
        case 0x31604:
            *id = OF_OXM_BSN_UDF3;
            break;
        case 0x31708:
            *id = OF_OXM_BSN_UDF3_MASKED;
            break;
        case 0x31804:
            *id = OF_OXM_BSN_UDF4;
            break;
        case 0x31908:
            *id = OF_OXM_BSN_UDF4_MASKED;
            break;
        case 0x31a04:
            *id = OF_OXM_BSN_UDF5;
            break;
        case 0x31b08:
            *id = OF_OXM_BSN_UDF5_MASKED;
            break;
        case 0x31c04:
            *id = OF_OXM_BSN_UDF6;
            break;
        case 0x31d08:
            *id = OF_OXM_BSN_UDF6_MASKED;
            break;
        case 0x31e04:
            *id = OF_OXM_BSN_UDF7;
            break;
        case 0x31f08:
            *id = OF_OXM_BSN_UDF7_MASKED;
            break;
        case 0x80000004:
            *id = OF_OXM_IN_PORT;
            break;
        case 0x80000108:
            *id = OF_OXM_IN_PORT_MASKED;
            break;
        case 0x80000204:
            *id = OF_OXM_IN_PHY_PORT;
            break;
        case 0x80000308:
            *id = OF_OXM_IN_PHY_PORT_MASKED;
            break;
        case 0x80000408:
            *id = OF_OXM_METADATA;
            break;
        case 0x80000510:
            *id = OF_OXM_METADATA_MASKED;
            break;
        case 0x80000606:
            *id = OF_OXM_ETH_DST;
            break;
        case 0x8000070c:
            *id = OF_OXM_ETH_DST_MASKED;
            break;
        case 0x80000806:
            *id = OF_OXM_ETH_SRC;
            break;
        case 0x8000090c:
            *id = OF_OXM_ETH_SRC_MASKED;
            break;
        case 0x80000a02:
            *id = OF_OXM_ETH_TYPE;
            break;
        case 0x80000b04:
            *id = OF_OXM_ETH_TYPE_MASKED;
            break;
        case 0x80000c02:
            *id = OF_OXM_VLAN_VID;
            break;
        case 0x80000d04:
            *id = OF_OXM_VLAN_VID_MASKED;
            break;
        case 0x80000e01:
            *id = OF_OXM_VLAN_PCP;
            break;
        case 0x80000f02:
            *id = OF_OXM_VLAN_PCP_MASKED;
            break;
        case 0x80001001:
            *id = OF_OXM_IP_DSCP;
            break;
        case 0x80001102:
            *id = OF_OXM_IP_DSCP_MASKED;
            break;
        case 0x80001201:
            *id = OF_OXM_IP_ECN;
            break;
        case 0x80001302:
            *id = OF_OXM_IP_ECN_MASKED;
            break;
        case 0x80001401:
            *id = OF_OXM_IP_PROTO;
            break;
        case 0x80001502:
            *id = OF_OXM_IP_PROTO_MASKED;
            break;
        case 0x80001604:
            *id = OF_OXM_IPV4_SRC;
            break;
        case 0x80001708:
            *id = OF_OXM_IPV4_SRC_MASKED;
            break;
        case 0x80001804:
            *id = OF_OXM_IPV4_DST;
            break;
        case 0x80001908:
            *id = OF_OXM_IPV4_DST_MASKED;
            break;
        case 0x80001a02:
            *id = OF_OXM_TCP_SRC;
            break;
        case 0x80001b04:
            *id = OF_OXM_TCP_SRC_MASKED;
            break;
        case 0x80001c02:
            *id = OF_OXM_TCP_DST;
            break;
        case 0x80001d04:
            *id = OF_OXM_TCP_DST_MASKED;
            break;
        case 0x80001e02:
            *id = OF_OXM_UDP_SRC;
            break;
        case 0x80001f04:
            *id = OF_OXM_UDP_SRC_MASKED;
            break;
        case 0x80002002:
            *id = OF_OXM_UDP_DST;
            break;
        case 0x80002104:
            *id = OF_OXM_UDP_DST_MASKED;
            break;
        case 0x80002202:
            *id = OF_OXM_SCTP_SRC;
            break;
        case 0x80002304:
            *id = OF_OXM_SCTP_SRC_MASKED;
            break;
        case 0x80002402:
            *id = OF_OXM_SCTP_DST;
            break;
        case 0x80002504:
            *id = OF_OXM_SCTP_DST_MASKED;
            break;
        case 0x80002601:
            *id = OF_OXM_ICMPV4_TYPE;
            break;
        case 0x80002702:
            *id = OF_OXM_ICMPV4_TYPE_MASKED;
            break;
        case 0x80002801:
            *id = OF_OXM_ICMPV4_CODE;
            break;
        case 0x80002902:
            *id = OF_OXM_ICMPV4_CODE_MASKED;
            break;
        case 0x80002a02:
            *id = OF_OXM_ARP_OP;
            break;
        case 0x80002b04:
            *id = OF_OXM_ARP_OP_MASKED;
            break;
        case 0x80002c04:
            *id = OF_OXM_ARP_SPA;
            break;
        case 0x80002d08:
            *id = OF_OXM_ARP_SPA_MASKED;
            break;
        case 0x80002e04:
            *id = OF_OXM_ARP_TPA;
            break;
        case 0x80002f08:
            *id = OF_OXM_ARP_TPA_MASKED;
            break;
        case 0x80003006:
            *id = OF_OXM_ARP_SHA;
            break;
        case 0x8000310c:
            *id = OF_OXM_ARP_SHA_MASKED;
            break;
        case 0x80003206:
            *id = OF_OXM_ARP_THA;
            break;
        case 0x8000330c:
            *id = OF_OXM_ARP_THA_MASKED;
            break;
        case 0x80003410:
            *id = OF_OXM_IPV6_SRC;
            break;
        case 0x80003520:
            *id = OF_OXM_IPV6_SRC_MASKED;
            break;
        case 0x80003610:
            *id = OF_OXM_IPV6_DST;
            break;
        case 0x80003720:
            *id = OF_OXM_IPV6_DST_MASKED;
            break;
        case 0x80003804:
            *id = OF_OXM_IPV6_FLABEL;
            break;
        case 0x80003908:
            *id = OF_OXM_IPV6_FLABEL_MASKED;
            break;
        case 0x80003a01:
            *id = OF_OXM_ICMPV6_TYPE;
            break;
        case 0x80003b02:
            *id = OF_OXM_ICMPV6_TYPE_MASKED;
            break;
        case 0x80003c01:
            *id = OF_OXM_ICMPV6_CODE;
            break;
        case 0x80003d02:
            *id = OF_OXM_ICMPV6_CODE_MASKED;
            break;
        case 0x80003e10:
            *id = OF_OXM_IPV6_ND_TARGET;
            break;
        case 0x80003f20:
            *id = OF_OXM_IPV6_ND_TARGET_MASKED;
            break;
        case 0x80004006:
            *id = OF_OXM_IPV6_ND_SLL;
            break;
        case 0x8000410c:
            *id = OF_OXM_IPV6_ND_SLL_MASKED;
            break;
        case 0x80004206:
            *id = OF_OXM_IPV6_ND_TLL;
            break;
        case 0x8000430c:
            *id = OF_OXM_IPV6_ND_TLL_MASKED;
            break;
        case 0x80004404:
            *id = OF_OXM_MPLS_LABEL;
            break;
        case 0x80004508:
            *id = OF_OXM_MPLS_LABEL_MASKED;
            break;
        case 0x80004601:
            *id = OF_OXM_MPLS_TC;
            break;
        case 0x80004702:
            *id = OF_OXM_MPLS_TC_MASKED;
            break;
        case 0x80004801:
            *id = OF_OXM_MPLS_BOS;
            break;
        case 0x80004902:
            *id = OF_OXM_MPLS_BOS_MASKED;
            break;
        case 0x80004c08:
            *id = OF_OXM_TUNNEL_ID;
            break;
        case 0x80004d10:
            *id = OF_OXM_TUNNEL_ID_MASKED;
            break;
        default:
            *id = OF_OXM;
            break;
        }
        break;
    }
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 0)); /* type_len */
        switch (value) {
        case 0x30010:
            *id = OF_OXM_BSN_IN_PORTS_128;
            break;
        case 0x30120:
            *id = OF_OXM_BSN_IN_PORTS_128_MASKED;
            break;
        case 0x30204:
            *id = OF_OXM_BSN_LAG_ID;
            break;
        case 0x30308:
            *id = OF_OXM_BSN_LAG_ID_MASKED;
            break;
        case 0x30404:
            *id = OF_OXM_BSN_VRF;
            break;
        case 0x30508:
            *id = OF_OXM_BSN_VRF_MASKED;
            break;
        case 0x30601:
            *id = OF_OXM_BSN_GLOBAL_VRF_ALLOWED;
            break;
        case 0x30702:
            *id = OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED;
            break;
        case 0x30804:
            *id = OF_OXM_BSN_L3_INTERFACE_CLASS_ID;
            break;
        case 0x30908:
            *id = OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED;
            break;
        case 0x30a04:
            *id = OF_OXM_BSN_L3_SRC_CLASS_ID;
            break;
        case 0x30b08:
            *id = OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED;
            break;
        case 0x30c04:
            *id = OF_OXM_BSN_L3_DST_CLASS_ID;
            break;
        case 0x30d08:
            *id = OF_OXM_BSN_L3_DST_CLASS_ID_MASKED;
            break;
        case 0x30e04:
            *id = OF_OXM_BSN_EGR_PORT_GROUP_ID;
            break;
        case 0x30f08:
            *id = OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED;
            break;
        case 0x31004:
            *id = OF_OXM_BSN_UDF0;
            break;
        case 0x31108:
            *id = OF_OXM_BSN_UDF0_MASKED;
            break;
        case 0x31204:
            *id = OF_OXM_BSN_UDF1;
            break;
        case 0x31308:
            *id = OF_OXM_BSN_UDF1_MASKED;
            break;
        case 0x31404:
            *id = OF_OXM_BSN_UDF2;
            break;
        case 0x31508:
            *id = OF_OXM_BSN_UDF2_MASKED;
            break;
        case 0x31604:
            *id = OF_OXM_BSN_UDF3;
            break;
        case 0x31708:
            *id = OF_OXM_BSN_UDF3_MASKED;
            break;
        case 0x31804:
            *id = OF_OXM_BSN_UDF4;
            break;
        case 0x31908:
            *id = OF_OXM_BSN_UDF4_MASKED;
            break;
        case 0x31a04:
            *id = OF_OXM_BSN_UDF5;
            break;
        case 0x31b08:
            *id = OF_OXM_BSN_UDF5_MASKED;
            break;
        case 0x31c04:
            *id = OF_OXM_BSN_UDF6;
            break;
        case 0x31d08:
            *id = OF_OXM_BSN_UDF6_MASKED;
            break;
        case 0x31e04:
            *id = OF_OXM_BSN_UDF7;
            break;
        case 0x31f08:
            *id = OF_OXM_BSN_UDF7_MASKED;
            break;
        case 0x80000004:
            *id = OF_OXM_IN_PORT;
            break;
        case 0x80000108:
            *id = OF_OXM_IN_PORT_MASKED;
            break;
        case 0x80000204:
            *id = OF_OXM_IN_PHY_PORT;
            break;
        case 0x80000308:
            *id = OF_OXM_IN_PHY_PORT_MASKED;
            break;
        case 0x80000408:
            *id = OF_OXM_METADATA;
            break;
        case 0x80000510:
            *id = OF_OXM_METADATA_MASKED;
            break;
        case 0x80000606:
            *id = OF_OXM_ETH_DST;
            break;
        case 0x8000070c:
            *id = OF_OXM_ETH_DST_MASKED;
            break;
        case 0x80000806:
            *id = OF_OXM_ETH_SRC;
            break;
        case 0x8000090c:
            *id = OF_OXM_ETH_SRC_MASKED;
            break;
        case 0x80000a02:
            *id = OF_OXM_ETH_TYPE;
            break;
        case 0x80000b04:
            *id = OF_OXM_ETH_TYPE_MASKED;
            break;
        case 0x80000c02:
            *id = OF_OXM_VLAN_VID;
            break;
        case 0x80000d04:
            *id = OF_OXM_VLAN_VID_MASKED;
            break;
        case 0x80000e01:
            *id = OF_OXM_VLAN_PCP;
            break;
        case 0x80000f02:
            *id = OF_OXM_VLAN_PCP_MASKED;
            break;
        case 0x80001001:
            *id = OF_OXM_IP_DSCP;
            break;
        case 0x80001102:
            *id = OF_OXM_IP_DSCP_MASKED;
            break;
        case 0x80001201:
            *id = OF_OXM_IP_ECN;
            break;
        case 0x80001302:
            *id = OF_OXM_IP_ECN_MASKED;
            break;
        case 0x80001401:
            *id = OF_OXM_IP_PROTO;
            break;
        case 0x80001502:
            *id = OF_OXM_IP_PROTO_MASKED;
            break;
        case 0x80001604:
            *id = OF_OXM_IPV4_SRC;
            break;
        case 0x80001708:
            *id = OF_OXM_IPV4_SRC_MASKED;
            break;
        case 0x80001804:
            *id = OF_OXM_IPV4_DST;
            break;
        case 0x80001908:
            *id = OF_OXM_IPV4_DST_MASKED;
            break;
        case 0x80001a02:
            *id = OF_OXM_TCP_SRC;
            break;
        case 0x80001b04:
            *id = OF_OXM_TCP_SRC_MASKED;
            break;
        case 0x80001c02:
            *id = OF_OXM_TCP_DST;
            break;
        case 0x80001d04:
            *id = OF_OXM_TCP_DST_MASKED;
            break;
        case 0x80001e02:
            *id = OF_OXM_UDP_SRC;
            break;
        case 0x80001f04:
            *id = OF_OXM_UDP_SRC_MASKED;
            break;
        case 0x80002002:
            *id = OF_OXM_UDP_DST;
            break;
        case 0x80002104:
            *id = OF_OXM_UDP_DST_MASKED;
            break;
        case 0x80002202:
            *id = OF_OXM_SCTP_SRC;
            break;
        case 0x80002304:
            *id = OF_OXM_SCTP_SRC_MASKED;
            break;
        case 0x80002402:
            *id = OF_OXM_SCTP_DST;
            break;
        case 0x80002504:
            *id = OF_OXM_SCTP_DST_MASKED;
            break;
        case 0x80002601:
            *id = OF_OXM_ICMPV4_TYPE;
            break;
        case 0x80002702:
            *id = OF_OXM_ICMPV4_TYPE_MASKED;
            break;
        case 0x80002801:
            *id = OF_OXM_ICMPV4_CODE;
            break;
        case 0x80002902:
            *id = OF_OXM_ICMPV4_CODE_MASKED;
            break;
        case 0x80002a02:
            *id = OF_OXM_ARP_OP;
            break;
        case 0x80002b04:
            *id = OF_OXM_ARP_OP_MASKED;
            break;
        case 0x80002c04:
            *id = OF_OXM_ARP_SPA;
            break;
        case 0x80002d08:
            *id = OF_OXM_ARP_SPA_MASKED;
            break;
        case 0x80002e04:
            *id = OF_OXM_ARP_TPA;
            break;
        case 0x80002f08:
            *id = OF_OXM_ARP_TPA_MASKED;
            break;
        case 0x80003006:
            *id = OF_OXM_ARP_SHA;
            break;
        case 0x8000310c:
            *id = OF_OXM_ARP_SHA_MASKED;
            break;
        case 0x80003206:
            *id = OF_OXM_ARP_THA;
            break;
        case 0x8000330c:
            *id = OF_OXM_ARP_THA_MASKED;
            break;
        case 0x80003410:
            *id = OF_OXM_IPV6_SRC;
            break;
        case 0x80003520:
            *id = OF_OXM_IPV6_SRC_MASKED;
            break;
        case 0x80003610:
            *id = OF_OXM_IPV6_DST;
            break;
        case 0x80003720:
            *id = OF_OXM_IPV6_DST_MASKED;
            break;
        case 0x80003804:
            *id = OF_OXM_IPV6_FLABEL;
            break;
        case 0x80003908:
            *id = OF_OXM_IPV6_FLABEL_MASKED;
            break;
        case 0x80003a01:
            *id = OF_OXM_ICMPV6_TYPE;
            break;
        case 0x80003b02:
            *id = OF_OXM_ICMPV6_TYPE_MASKED;
            break;
        case 0x80003c01:
            *id = OF_OXM_ICMPV6_CODE;
            break;
        case 0x80003d02:
            *id = OF_OXM_ICMPV6_CODE_MASKED;
            break;
        case 0x80003e10:
            *id = OF_OXM_IPV6_ND_TARGET;
            break;
        case 0x80003f20:
            *id = OF_OXM_IPV6_ND_TARGET_MASKED;
            break;
        case 0x80004006:
            *id = OF_OXM_IPV6_ND_SLL;
            break;
        case 0x8000410c:
            *id = OF_OXM_IPV6_ND_SLL_MASKED;
            break;
        case 0x80004206:
            *id = OF_OXM_IPV6_ND_TLL;
            break;
        case 0x8000430c:
            *id = OF_OXM_IPV6_ND_TLL_MASKED;
            break;
        case 0x80004404:
            *id = OF_OXM_MPLS_LABEL;
            break;
        case 0x80004508:
            *id = OF_OXM_MPLS_LABEL_MASKED;
            break;
        case 0x80004601:
            *id = OF_OXM_MPLS_TC;
            break;
        case 0x80004702:
            *id = OF_OXM_MPLS_TC_MASKED;
            break;
        case 0x80004801:
            *id = OF_OXM_MPLS_BOS;
            break;
        case 0x80004902:
            *id = OF_OXM_MPLS_BOS_MASKED;
            break;
        case 0x80004c08:
            *id = OF_OXM_TUNNEL_ID;
            break;
        case 0x80004d10:
            *id = OF_OXM_TUNNEL_ID_MASKED;
            break;
        case 0x80010008:
            *id = ONF_OXM_PACKET_REGS0;
            break;
        case 0x80010208:
            *id = ONF_OXM_PACKET_REGS1;
            break;
        case 0x80010408:
            *id = ONF_OXM_PACKET_REGS2;
            break;
        case 0x80010608:
            *id = ONF_OXM_PACKET_REGS3;
            break;
        case 0xffff0206:
            *id = OF_OXM_OFDPA_VRF;
            break;
        case 0xffff0405:
            *id = OF_OXM_OFDPA_TRAFFIC_CLASS;
            break;
        case 0xffff0605:
            *id = OF_OXM_OFDPA_COLOR;
            break;
        case 0xffff0805:
            *id = OF_OXM_OFDPA_DEI;
            break;
        case 0xffff0a05:
            *id = OF_OXM_OFDPA_QOS_INDEX;
            break;
        case 0xffff0c08:
            *id = OF_OXM_OFDPA_LMEP_ID;
            break;
        case 0xffff0e05:
            *id = OF_OXM_OFDPA_MPLS_TTL;
            break;
        case 0xffff1008:
            *id = OF_OXM_OFDPA_MPLS_L2_PORT;
            break;
        case 0xffff110c:
            *id = OF_OXM_OFDPA_MPLS_L2_PORT_MASKED;
            break;
        case 0xffff1208:
            *id = OF_OXM_OFDPA_L3_IN_PORT;
            break;
        case 0xffff1406:
            *id = OF_OXM_OFDPA_OVID;
            break;
        case 0xffff1605:
            *id = OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE;
            break;
        case 0xffff1806:
            *id = OF_OXM_OFDPA_MPLS_ACH_CHANNEL;
            break;
        case 0xffff1a05:
            *id = OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL;
            break;
        case 0xffff1c05:
            *id = OF_OXM_OFDPA_OAM_Y1731_MDL;
            break;
        case 0xffff1e05:
            *id = OF_OXM_OFDPA_OAM_Y1731_OPCODE;
            break;
        case 0xffff2008:
            *id = OF_OXM_OFDPA_COLOR_ACTIONS_INDEX;
            break;
        case 0xffff220c:
            *id = OF_OXM_OFDPA_TXFCL;
            break;
        case 0xffff240c:
            *id = OF_OXM_OFDPA_RXFCL;
            break;
        case 0xffff260c:
            *id = OF_OXM_OFDPA_RX_TIMESTAMP;
            break;
        case 0xffff2a05:
            *id = OF_OXM_OFDPA_PROTECTION_INDEX;
            break;
        case 0xffff2e06:
            *id = OF_OXM_OFDPA_MPLS_TYPE;
            break;
        case 0xffff3005:
            *id = OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION;
            break;
        case 0xffff560C:
            *id = OF_OXM_ONF_ACTSET_OUTPUT;
            break;
        default:
            *id = OF_OXM;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }

}


/**
 * \defgroup of_oxm of_oxm
 */

/**
 * Create a new of_oxm object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm
 */

of_oxm_t *
of_oxm_new(of_version_t version)
{
    of_oxm_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM] + of_object_extra_len[version][OF_OXM];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_oxm.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_init(of_oxm_t *obj_p,
    of_version_t version, int bytes, int clean_wire)
{
    of_oxm_header_t *obj;

    obj = &obj_p->header;  /* Need instantiable subclass */

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM] + of_object_extra_len[version][OF_OXM];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM;

    /* Set up the object's function pointers */

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}

/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_action_set_field_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x19); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_set_field of_action_set_field
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_set_field_push_wire_values(of_action_set_field_t *obj)
{

    of_action_set_field_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_set_field object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_action_set_field
 */

of_action_set_field_t *
of_action_set_field_new(of_version_t version)
{
    of_action_set_field_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_SET_FIELD] + of_object_extra_len[version][OF_ACTION_SET_FIELD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_set_field_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_set_field_init(obj, version, bytes, 0);

    if (of_action_set_field_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_set_field.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_action_set_field_init(of_action_set_field_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_SET_FIELD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_SET_FIELD] + of_object_extra_len[version][OF_ACTION_SET_FIELD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_SET_FIELD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_set_field_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get field from an object of type of_action_set_field.
 * @param obj Pointer to an object of type of_action_set_field.
 * @param field Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_action_set_field_field_get(
    of_action_set_field_t *obj,
    of_octets_t *field)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_ACTION_SET_FIELD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);
    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    field->bytes = cur_len;
    field->data = OF_WIRE_BUFFER_INDEX(wbuf, abs_offset);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set field in an object of type of_action_set_field.
 * @param obj Pointer to an object of type of_action_set_field.
 * @param field The value to write into the object
 */
int WARN_UNUSED_RESULT
of_action_set_field_field_set(
    of_action_set_field_t *obj,
    of_octets_t *field)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_ACTION_SET_FIELD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);
    new_len = field->bytes;
    of_wire_buffer_grow(wbuf, abs_offset + (new_len - cur_len));
    of_wire_buffer_octets_data_set(wbuf, abs_offset, field, cur_len);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_experimenter_error_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* err_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_experimenter_error_msg of_experimenter_error_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_experimenter_error_msg_push_wire_values(of_experimenter_error_msg_t *obj)
{

    of_experimenter_error_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_experimenter_error_msg object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_experimenter_error_msg
 */

of_experimenter_error_msg_t *
of_experimenter_error_msg_new(of_version_t version)
{
    of_experimenter_error_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_EXPERIMENTER_ERROR_MSG] + of_object_extra_len[version][OF_EXPERIMENTER_ERROR_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_experimenter_error_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_experimenter_error_msg_init(obj, version, bytes, 0);

    if (of_experimenter_error_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_experimenter_error_msg.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_experimenter_error_msg_init(of_experimenter_error_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_EXPERIMENTER_ERROR_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_EXPERIMENTER_ERROR_MSG] + of_object_extra_len[version][OF_EXPERIMENTER_ERROR_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_EXPERIMENTER_ERROR_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_experimenter_error_msg_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new of_experimenter_error_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_experimenter_error_msg
 */

of_experimenter_error_msg_t *
of_experimenter_error_msg_new_from_message(of_message_t msg)
{
    of_experimenter_error_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_experimenter_error_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_experimenter_error_msg_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type of_experimenter_error_msg.
 * @param obj Pointer to an object of type of_experimenter_error_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_experimenter_error_msg_xid_get(
    of_experimenter_error_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type of_experimenter_error_msg.
 * @param obj Pointer to an object of type of_experimenter_error_msg.
 * @param xid The value to write into the object
 */
void
of_experimenter_error_msg_xid_set(
    of_experimenter_error_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_experimenter_error_msg.
 * @param obj Pointer to an object of type of_experimenter_error_msg.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_experimenter_error_msg_subtype_get(
    of_experimenter_error_msg_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_experimenter_error_msg.
 * @param obj Pointer to an object of type of_experimenter_error_msg.
 * @param subtype The value to write into the object
 */
void
of_experimenter_error_msg_subtype_set(
    of_experimenter_error_msg_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type of_experimenter_error_msg.
 * @param obj Pointer to an object of type of_experimenter_error_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_experimenter_error_msg_experimenter_get(
    of_experimenter_error_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_experimenter_error_msg.
 * @param obj Pointer to an object of type of_experimenter_error_msg.
 * @param experimenter The value to write into the object
 */
void
of_experimenter_error_msg_experimenter_set(
    of_experimenter_error_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get data from an object of type of_experimenter_error_msg.
 * @param obj Pointer to an object of type of_experimenter_error_msg.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_experimenter_error_msg_data_get(
    of_experimenter_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);
    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    data->bytes = cur_len;
    data->data = OF_WIRE_BUFFER_INDEX(wbuf, abs_offset);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set data in an object of type of_experimenter_error_msg.
 * @param obj Pointer to an object of type of_experimenter_error_msg.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_experimenter_error_msg_data_set(
    of_experimenter_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);
    new_len = data->bytes;
    of_wire_buffer_grow(wbuf, abs_offset + (new_len - cur_len));
    of_wire_buffer_octets_data_set(wbuf, abs_offset, data, cur_len);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_group_features_stats_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x8); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_group_features_stats_reply of_group_features_stats_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_group_features_stats_reply_push_wire_values(of_group_features_stats_reply_t *obj)
{

    of_group_features_stats_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_group_features_stats_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_group_features_stats_reply
 */

of_group_features_stats_reply_t *
of_group_features_stats_reply_new(of_version_t version)
{
    of_group_features_stats_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_GROUP_FEATURES_STATS_REPLY] + of_object_extra_len[version][OF_GROUP_FEATURES_STATS_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_group_features_stats_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_group_features_stats_reply_init(obj, version, bytes, 0);

    if (of_group_features_stats_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_group_features_stats_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_group_features_stats_reply_init(of_group_features_stats_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_GROUP_FEATURES_STATS_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_GROUP_FEATURES_STATS_REPLY] + of_object_extra_len[version][OF_GROUP_FEATURES_STATS_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_GROUP_FEATURES_STATS_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_group_features_stats_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new of_group_features_stats_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_group_features_stats_reply
 */

of_group_features_stats_reply_t *
of_group_features_stats_reply_new_from_message(of_message_t msg)
{
    of_group_features_stats_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_group_features_stats_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_group_features_stats_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_xid_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param xid The value to write into the object
 */
void
of_group_features_stats_reply_xid_set(
    of_group_features_stats_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_group_features_stats_reply_flags_get(
    of_group_features_stats_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param flags The value to write into the object
 */
void
of_group_features_stats_reply_flags_set(
    of_group_features_stats_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get types from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param types Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_types_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *types)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, types);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set types in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param types The value to write into the object
 */
void
of_group_features_stats_reply_types_set(
    of_group_features_stats_reply_t *obj,
    uint32_t types)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, types);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get capabilities from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param capabilities Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_capabilities_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *capabilities)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, capabilities);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set capabilities in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param capabilities The value to write into the object
 */
void
of_group_features_stats_reply_capabilities_set(
    of_group_features_stats_reply_t *obj,
    uint32_t capabilities)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, capabilities);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_groups_all from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param max_groups_all Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_max_groups_all_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *max_groups_all)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, max_groups_all);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_groups_all in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param max_groups_all The value to write into the object
 */
void
of_group_features_stats_reply_max_groups_all_set(
    of_group_features_stats_reply_t *obj,
    uint32_t max_groups_all)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, max_groups_all);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_groups_select from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param max_groups_select Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_max_groups_select_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *max_groups_select)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, max_groups_select);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_groups_select in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param max_groups_select The value to write into the object
 */
void
of_group_features_stats_reply_max_groups_select_set(
    of_group_features_stats_reply_t *obj,
    uint32_t max_groups_select)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, max_groups_select);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_groups_indirect from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param max_groups_indirect Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_max_groups_indirect_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *max_groups_indirect)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, max_groups_indirect);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_groups_indirect in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param max_groups_indirect The value to write into the object
 */
void
of_group_features_stats_reply_max_groups_indirect_set(
    of_group_features_stats_reply_t *obj,
    uint32_t max_groups_indirect)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, max_groups_indirect);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_groups_ff from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param max_groups_ff Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_max_groups_ff_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *max_groups_ff)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, max_groups_ff);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_groups_ff in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param max_groups_ff The value to write into the object
 */
void
of_group_features_stats_reply_max_groups_ff_set(
    of_group_features_stats_reply_t *obj,
    uint32_t max_groups_ff)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, max_groups_ff);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get actions_all from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param actions_all Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_actions_all_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *actions_all)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, actions_all);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set actions_all in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param actions_all The value to write into the object
 */
void
of_group_features_stats_reply_actions_all_set(
    of_group_features_stats_reply_t *obj,
    uint32_t actions_all)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, actions_all);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get actions_select from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param actions_select Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_actions_select_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *actions_select)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, actions_select);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set actions_select in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param actions_select The value to write into the object
 */
void
of_group_features_stats_reply_actions_select_set(
    of_group_features_stats_reply_t *obj,
    uint32_t actions_select)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, actions_select);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get actions_indirect from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param actions_indirect Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_actions_indirect_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *actions_indirect)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, actions_indirect);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set actions_indirect in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param actions_indirect The value to write into the object
 */
void
of_group_features_stats_reply_actions_indirect_set(
    of_group_features_stats_reply_t *obj,
    uint32_t actions_indirect)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, actions_indirect);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get actions_ff from an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param actions_ff Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_reply_actions_ff_get(
    of_group_features_stats_reply_t *obj,
    uint32_t *actions_ff)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 52;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, actions_ff);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set actions_ff in an object of type of_group_features_stats_reply.
 * @param obj Pointer to an object of type of_group_features_stats_reply.
 * @param actions_ff The value to write into the object
 */
void
of_group_features_stats_reply_actions_ff_set(
    of_group_features_stats_reply_t *obj,
    uint32_t actions_ff)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 52;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, actions_ff);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_group_features_stats_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x8); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_group_features_stats_request of_group_features_stats_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_group_features_stats_request_push_wire_values(of_group_features_stats_request_t *obj)
{

    of_group_features_stats_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_group_features_stats_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_group_features_stats_request
 */

of_group_features_stats_request_t *
of_group_features_stats_request_new(of_version_t version)
{
    of_group_features_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_GROUP_FEATURES_STATS_REQUEST] + of_object_extra_len[version][OF_GROUP_FEATURES_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_group_features_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_group_features_stats_request_init(obj, version, bytes, 0);

    if (of_group_features_stats_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_group_features_stats_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_group_features_stats_request_init(of_group_features_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_GROUP_FEATURES_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_GROUP_FEATURES_STATS_REQUEST] + of_object_extra_len[version][OF_GROUP_FEATURES_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_GROUP_FEATURES_STATS_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_group_features_stats_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new of_group_features_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_group_features_stats_request
 */

of_group_features_stats_request_t *
of_group_features_stats_request_new_from_message(of_message_t msg)
{
    of_group_features_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_group_features_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_group_features_stats_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type of_group_features_stats_request.
 * @param obj Pointer to an object of type of_group_features_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_group_features_stats_request_xid_get(
    of_group_features_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type of_group_features_stats_request.
 * @param obj Pointer to an object of type of_group_features_stats_request.
 * @param xid The value to write into the object
 */
void
of_group_features_stats_request_xid_set(
    of_group_features_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_group_features_stats_request.
 * @param obj Pointer to an object of type of_group_features_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_group_features_stats_request_flags_get(
    of_group_features_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type of_group_features_stats_request.
 * @param obj Pointer to an object of type of_group_features_stats_request.
 * @param flags The value to write into the object
 */
void
of_group_features_stats_request_flags_set(
    of_group_features_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GROUP_FEATURES_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_match_v3_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x1); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_match_v3 of_match_v3
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_match_v3_push_wire_values(of_match_v3_t *obj)
{

    of_match_v3_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_match_v3 object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_match_v3
 */

of_match_v3_t *
of_match_v3_new(of_version_t version)
{
    of_match_v3_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_MATCH_V3] + of_object_extra_len[version][OF_MATCH_V3];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_match_v3_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_match_v3_init(obj, version, bytes, 0);

    if (of_match_v3_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_match_v3.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_match_v3_init(of_match_v3_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_MATCH_V3] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_MATCH_V3] + of_object_extra_len[version][OF_MATCH_V3];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_MATCH_V3;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_match_v3_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_oxm_t to the parent of type of_match_v3 for
 * member oxm_list
 * @param obj Pointer to an object of type of_match_v3.
 * @param oxm_list Pointer to the child object of type
 * of_list_oxm_t to be filled out.
 * \ingroup of_match_v3
 *
 * The parameter oxm_list is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_match_v3_oxm_list_bind(
    of_match_v3_t *obj,
    of_list_oxm_t *oxm_list)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_MATCH_V3);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = wire_match_len(obj, 0) - 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_oxm_init(oxm_list, obj->version, 0, 1);
    /* Attach to parent */
    oxm_list->parent = (of_object_t *)obj;
    oxm_list->wire_object.wbuf = obj->wire_object.wbuf;
    oxm_list->wire_object.obj_offset = abs_offset;
    oxm_list->wire_object.owned = 0;
    oxm_list->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of oxm_list into a new variable of type of_list_oxm_t from
 * a of_match_v3 instance.
 *
 * @param obj Pointer to the source of type of_match_v3_t
 * @returns A pointer to a new instance of type of_list_oxm_t whose contents
 * match that of oxm_list from source
 * @returns NULL if an error occurs
 */
of_list_oxm_t *
of_match_v3_oxm_list_get(of_match_v3_t *obj) {
    of_list_oxm_t _oxm_list;
    of_list_oxm_t *_oxm_list_ptr;

    of_match_v3_oxm_list_bind(obj, &_oxm_list);
    _oxm_list_ptr = (of_list_oxm_t *)of_object_dup(&_oxm_list);
    return _oxm_list_ptr;
}

/**
 * Set oxm_list in an object of type of_match_v3.
 * @param obj Pointer to an object of type of_match_v3.
 * @param oxm_list Pointer to the child of type of_list_oxm_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_match_v3_oxm_list_set(
    of_match_v3_t *obj,
    of_list_oxm_t *oxm_list)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_MATCH_V3);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = wire_match_len(obj, 0) - 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = oxm_list->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == oxm_list->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(oxm_list, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(oxm_list, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (oxm_list->wire_length_set != NULL) {
        oxm_list->wire_length_set((of_object_t *)oxm_list, oxm_list->length);
    }

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_op_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80002a02); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_op of_oxm_arp_op
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_op_push_wire_values(of_oxm_arp_op_t *obj)
{

    of_oxm_arp_op_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_op object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_op
 */

of_oxm_arp_op_t *
of_oxm_arp_op_new(of_version_t version)
{
    of_oxm_arp_op_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_OP] + of_object_extra_len[version][OF_OXM_ARP_OP];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_op_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_op_init(obj, version, bytes, 0);

    if (of_oxm_arp_op_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_op.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_op_init(of_oxm_arp_op_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_OP] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_OP] + of_object_extra_len[version][OF_OXM_ARP_OP];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_OP;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_op_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_op.
 * @param obj Pointer to an object of type of_oxm_arp_op.
 * @param value Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_oxm_arp_op_value_get(
    of_oxm_arp_op_t *obj,
    uint16_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_OP);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_op.
 * @param obj Pointer to an object of type of_oxm_arp_op.
 * @param value The value to write into the object
 */
void
of_oxm_arp_op_value_set(
    of_oxm_arp_op_t *obj,
    uint16_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_OP);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_op_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80002b04); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_op_masked of_oxm_arp_op_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_op_masked_push_wire_values(of_oxm_arp_op_masked_t *obj)
{

    of_oxm_arp_op_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_op_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_op_masked
 */

of_oxm_arp_op_masked_t *
of_oxm_arp_op_masked_new(of_version_t version)
{
    of_oxm_arp_op_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_OP_MASKED] + of_object_extra_len[version][OF_OXM_ARP_OP_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_op_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_op_masked_init(obj, version, bytes, 0);

    if (of_oxm_arp_op_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_op_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_op_masked_init(of_oxm_arp_op_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_OP_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_OP_MASKED] + of_object_extra_len[version][OF_OXM_ARP_OP_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_OP_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_op_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_op_masked.
 * @param obj Pointer to an object of type of_oxm_arp_op_masked.
 * @param value Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_oxm_arp_op_masked_value_get(
    of_oxm_arp_op_masked_t *obj,
    uint16_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_OP_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_op_masked.
 * @param obj Pointer to an object of type of_oxm_arp_op_masked.
 * @param value The value to write into the object
 */
void
of_oxm_arp_op_masked_value_set(
    of_oxm_arp_op_masked_t *obj,
    uint16_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_OP_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_arp_op_masked.
 * @param obj Pointer to an object of type of_oxm_arp_op_masked.
 * @param value_mask Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_oxm_arp_op_masked_value_mask_get(
    of_oxm_arp_op_masked_t *obj,
    uint16_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_OP_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 6;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_arp_op_masked.
 * @param obj Pointer to an object of type of_oxm_arp_op_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_arp_op_masked_value_mask_set(
    of_oxm_arp_op_masked_t *obj,
    uint16_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_OP_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 6;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_sha_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80003006); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_sha of_oxm_arp_sha
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_sha_push_wire_values(of_oxm_arp_sha_t *obj)
{

    of_oxm_arp_sha_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_sha object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_sha
 */

of_oxm_arp_sha_t *
of_oxm_arp_sha_new(of_version_t version)
{
    of_oxm_arp_sha_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_SHA] + of_object_extra_len[version][OF_OXM_ARP_SHA];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_sha_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_sha_init(obj, version, bytes, 0);

    if (of_oxm_arp_sha_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_sha.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_sha_init(of_oxm_arp_sha_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_SHA] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_SHA] + of_object_extra_len[version][OF_OXM_ARP_SHA];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_SHA;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_sha_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_sha.
 * @param obj Pointer to an object of type of_oxm_arp_sha.
 * @param value Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_oxm_arp_sha_value_get(
    of_oxm_arp_sha_t *obj,
    of_mac_addr_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SHA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_sha.
 * @param obj Pointer to an object of type of_oxm_arp_sha.
 * @param value The value to write into the object
 */
void
of_oxm_arp_sha_value_set(
    of_oxm_arp_sha_t *obj,
    of_mac_addr_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SHA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_sha_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x8000310c); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_sha_masked of_oxm_arp_sha_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_sha_masked_push_wire_values(of_oxm_arp_sha_masked_t *obj)
{

    of_oxm_arp_sha_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_sha_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_sha_masked
 */

of_oxm_arp_sha_masked_t *
of_oxm_arp_sha_masked_new(of_version_t version)
{
    of_oxm_arp_sha_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_SHA_MASKED] + of_object_extra_len[version][OF_OXM_ARP_SHA_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_sha_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_sha_masked_init(obj, version, bytes, 0);

    if (of_oxm_arp_sha_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_sha_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_sha_masked_init(of_oxm_arp_sha_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_SHA_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_SHA_MASKED] + of_object_extra_len[version][OF_OXM_ARP_SHA_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_SHA_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_sha_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_sha_masked.
 * @param obj Pointer to an object of type of_oxm_arp_sha_masked.
 * @param value Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_oxm_arp_sha_masked_value_get(
    of_oxm_arp_sha_masked_t *obj,
    of_mac_addr_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SHA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_sha_masked.
 * @param obj Pointer to an object of type of_oxm_arp_sha_masked.
 * @param value The value to write into the object
 */
void
of_oxm_arp_sha_masked_value_set(
    of_oxm_arp_sha_masked_t *obj,
    of_mac_addr_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SHA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_arp_sha_masked.
 * @param obj Pointer to an object of type of_oxm_arp_sha_masked.
 * @param value_mask Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_oxm_arp_sha_masked_value_mask_get(
    of_oxm_arp_sha_masked_t *obj,
    of_mac_addr_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SHA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_arp_sha_masked.
 * @param obj Pointer to an object of type of_oxm_arp_sha_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_arp_sha_masked_value_mask_set(
    of_oxm_arp_sha_masked_t *obj,
    of_mac_addr_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SHA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_spa_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80002c04); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_spa of_oxm_arp_spa
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_spa_push_wire_values(of_oxm_arp_spa_t *obj)
{

    of_oxm_arp_spa_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_spa object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_spa
 */

of_oxm_arp_spa_t *
of_oxm_arp_spa_new(of_version_t version)
{
    of_oxm_arp_spa_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_SPA] + of_object_extra_len[version][OF_OXM_ARP_SPA];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_spa_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_spa_init(obj, version, bytes, 0);

    if (of_oxm_arp_spa_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_spa.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_spa_init(of_oxm_arp_spa_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_SPA] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_SPA] + of_object_extra_len[version][OF_OXM_ARP_SPA];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_SPA;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_spa_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_spa.
 * @param obj Pointer to an object of type of_oxm_arp_spa.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_arp_spa_value_get(
    of_oxm_arp_spa_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SPA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_spa.
 * @param obj Pointer to an object of type of_oxm_arp_spa.
 * @param value The value to write into the object
 */
void
of_oxm_arp_spa_value_set(
    of_oxm_arp_spa_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SPA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_spa_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80002d08); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_spa_masked of_oxm_arp_spa_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_spa_masked_push_wire_values(of_oxm_arp_spa_masked_t *obj)
{

    of_oxm_arp_spa_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_spa_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_spa_masked
 */

of_oxm_arp_spa_masked_t *
of_oxm_arp_spa_masked_new(of_version_t version)
{
    of_oxm_arp_spa_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_SPA_MASKED] + of_object_extra_len[version][OF_OXM_ARP_SPA_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_spa_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_spa_masked_init(obj, version, bytes, 0);

    if (of_oxm_arp_spa_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_spa_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_spa_masked_init(of_oxm_arp_spa_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_SPA_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_SPA_MASKED] + of_object_extra_len[version][OF_OXM_ARP_SPA_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_SPA_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_spa_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_spa_masked.
 * @param obj Pointer to an object of type of_oxm_arp_spa_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_arp_spa_masked_value_get(
    of_oxm_arp_spa_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SPA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_spa_masked.
 * @param obj Pointer to an object of type of_oxm_arp_spa_masked.
 * @param value The value to write into the object
 */
void
of_oxm_arp_spa_masked_value_set(
    of_oxm_arp_spa_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SPA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_arp_spa_masked.
 * @param obj Pointer to an object of type of_oxm_arp_spa_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_arp_spa_masked_value_mask_get(
    of_oxm_arp_spa_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SPA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_arp_spa_masked.
 * @param obj Pointer to an object of type of_oxm_arp_spa_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_arp_spa_masked_value_mask_set(
    of_oxm_arp_spa_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_SPA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_tha_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80003206); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_tha of_oxm_arp_tha
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_tha_push_wire_values(of_oxm_arp_tha_t *obj)
{

    of_oxm_arp_tha_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_tha object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_tha
 */

of_oxm_arp_tha_t *
of_oxm_arp_tha_new(of_version_t version)
{
    of_oxm_arp_tha_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_THA] + of_object_extra_len[version][OF_OXM_ARP_THA];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_tha_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_tha_init(obj, version, bytes, 0);

    if (of_oxm_arp_tha_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_tha.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_tha_init(of_oxm_arp_tha_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_THA] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_THA] + of_object_extra_len[version][OF_OXM_ARP_THA];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_THA;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_tha_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_tha.
 * @param obj Pointer to an object of type of_oxm_arp_tha.
 * @param value Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_oxm_arp_tha_value_get(
    of_oxm_arp_tha_t *obj,
    of_mac_addr_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_THA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_tha.
 * @param obj Pointer to an object of type of_oxm_arp_tha.
 * @param value The value to write into the object
 */
void
of_oxm_arp_tha_value_set(
    of_oxm_arp_tha_t *obj,
    of_mac_addr_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_THA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_tha_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x8000330c); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_tha_masked of_oxm_arp_tha_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_tha_masked_push_wire_values(of_oxm_arp_tha_masked_t *obj)
{

    of_oxm_arp_tha_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_tha_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_tha_masked
 */

of_oxm_arp_tha_masked_t *
of_oxm_arp_tha_masked_new(of_version_t version)
{
    of_oxm_arp_tha_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_THA_MASKED] + of_object_extra_len[version][OF_OXM_ARP_THA_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_tha_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_tha_masked_init(obj, version, bytes, 0);

    if (of_oxm_arp_tha_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_tha_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_tha_masked_init(of_oxm_arp_tha_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_THA_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_THA_MASKED] + of_object_extra_len[version][OF_OXM_ARP_THA_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_THA_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_tha_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_tha_masked.
 * @param obj Pointer to an object of type of_oxm_arp_tha_masked.
 * @param value Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_oxm_arp_tha_masked_value_get(
    of_oxm_arp_tha_masked_t *obj,
    of_mac_addr_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_THA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_tha_masked.
 * @param obj Pointer to an object of type of_oxm_arp_tha_masked.
 * @param value The value to write into the object
 */
void
of_oxm_arp_tha_masked_value_set(
    of_oxm_arp_tha_masked_t *obj,
    of_mac_addr_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_THA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_arp_tha_masked.
 * @param obj Pointer to an object of type of_oxm_arp_tha_masked.
 * @param value_mask Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_oxm_arp_tha_masked_value_mask_get(
    of_oxm_arp_tha_masked_t *obj,
    of_mac_addr_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_THA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_arp_tha_masked.
 * @param obj Pointer to an object of type of_oxm_arp_tha_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_arp_tha_masked_value_mask_set(
    of_oxm_arp_tha_masked_t *obj,
    of_mac_addr_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_THA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_tpa_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80002e04); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_tpa of_oxm_arp_tpa
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_tpa_push_wire_values(of_oxm_arp_tpa_t *obj)
{

    of_oxm_arp_tpa_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_tpa object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_tpa
 */

of_oxm_arp_tpa_t *
of_oxm_arp_tpa_new(of_version_t version)
{
    of_oxm_arp_tpa_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_TPA] + of_object_extra_len[version][OF_OXM_ARP_TPA];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_tpa_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_tpa_init(obj, version, bytes, 0);

    if (of_oxm_arp_tpa_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_tpa.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_tpa_init(of_oxm_arp_tpa_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_TPA] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_TPA] + of_object_extra_len[version][OF_OXM_ARP_TPA];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_TPA;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_tpa_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_tpa.
 * @param obj Pointer to an object of type of_oxm_arp_tpa.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_arp_tpa_value_get(
    of_oxm_arp_tpa_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_TPA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_tpa.
 * @param obj Pointer to an object of type of_oxm_arp_tpa.
 * @param value The value to write into the object
 */
void
of_oxm_arp_tpa_value_set(
    of_oxm_arp_tpa_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_TPA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_arp_tpa_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80002f08); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_arp_tpa_masked of_oxm_arp_tpa_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_arp_tpa_masked_push_wire_values(of_oxm_arp_tpa_masked_t *obj)
{

    of_oxm_arp_tpa_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_arp_tpa_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_arp_tpa_masked
 */

of_oxm_arp_tpa_masked_t *
of_oxm_arp_tpa_masked_new(of_version_t version)
{
    of_oxm_arp_tpa_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ARP_TPA_MASKED] + of_object_extra_len[version][OF_OXM_ARP_TPA_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_arp_tpa_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_arp_tpa_masked_init(obj, version, bytes, 0);

    if (of_oxm_arp_tpa_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_arp_tpa_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_arp_tpa_masked_init(of_oxm_arp_tpa_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ARP_TPA_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ARP_TPA_MASKED] + of_object_extra_len[version][OF_OXM_ARP_TPA_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ARP_TPA_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_arp_tpa_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_arp_tpa_masked.
 * @param obj Pointer to an object of type of_oxm_arp_tpa_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_arp_tpa_masked_value_get(
    of_oxm_arp_tpa_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_TPA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_arp_tpa_masked.
 * @param obj Pointer to an object of type of_oxm_arp_tpa_masked.
 * @param value The value to write into the object
 */
void
of_oxm_arp_tpa_masked_value_set(
    of_oxm_arp_tpa_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_TPA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_arp_tpa_masked.
 * @param obj Pointer to an object of type of_oxm_arp_tpa_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_arp_tpa_masked_value_mask_get(
    of_oxm_arp_tpa_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_TPA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_arp_tpa_masked.
 * @param obj Pointer to an object of type of_oxm_arp_tpa_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_arp_tpa_masked_value_mask_set(
    of_oxm_arp_tpa_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ARP_TPA_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_egr_port_group_id_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30e04); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_egr_port_group_id of_oxm_bsn_egr_port_group_id
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_egr_port_group_id_push_wire_values(of_oxm_bsn_egr_port_group_id_t *obj)
{

    of_oxm_bsn_egr_port_group_id_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_egr_port_group_id object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_egr_port_group_id
 */

of_oxm_bsn_egr_port_group_id_t *
of_oxm_bsn_egr_port_group_id_new(of_version_t version)
{
    of_oxm_bsn_egr_port_group_id_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID] + of_object_extra_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_egr_port_group_id_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_egr_port_group_id_init(obj, version, bytes, 0);

    if (of_oxm_bsn_egr_port_group_id_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_egr_port_group_id.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_egr_port_group_id_init(of_oxm_bsn_egr_port_group_id_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID] + of_object_extra_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_EGR_PORT_GROUP_ID;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_egr_port_group_id_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_egr_port_group_id.
 * @param obj Pointer to an object of type of_oxm_bsn_egr_port_group_id.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_egr_port_group_id_value_get(
    of_oxm_bsn_egr_port_group_id_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_EGR_PORT_GROUP_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_egr_port_group_id.
 * @param obj Pointer to an object of type of_oxm_bsn_egr_port_group_id.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_egr_port_group_id_value_set(
    of_oxm_bsn_egr_port_group_id_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_EGR_PORT_GROUP_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_egr_port_group_id_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30f08); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_egr_port_group_id_masked of_oxm_bsn_egr_port_group_id_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_egr_port_group_id_masked_push_wire_values(of_oxm_bsn_egr_port_group_id_masked_t *obj)
{

    of_oxm_bsn_egr_port_group_id_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_egr_port_group_id_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_egr_port_group_id_masked
 */

of_oxm_bsn_egr_port_group_id_masked_t *
of_oxm_bsn_egr_port_group_id_masked_new(of_version_t version)
{
    of_oxm_bsn_egr_port_group_id_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_egr_port_group_id_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_egr_port_group_id_masked_init(obj, version, bytes, 0);

    if (of_oxm_bsn_egr_port_group_id_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_egr_port_group_id_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_egr_port_group_id_masked_init(of_oxm_bsn_egr_port_group_id_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_egr_port_group_id_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_egr_port_group_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_egr_port_group_id_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_egr_port_group_id_masked_value_get(
    of_oxm_bsn_egr_port_group_id_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_egr_port_group_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_egr_port_group_id_masked.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_egr_port_group_id_masked_value_set(
    of_oxm_bsn_egr_port_group_id_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_bsn_egr_port_group_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_egr_port_group_id_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_egr_port_group_id_masked_value_mask_get(
    of_oxm_bsn_egr_port_group_id_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_bsn_egr_port_group_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_egr_port_group_id_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_bsn_egr_port_group_id_masked_value_mask_set(
    of_oxm_bsn_egr_port_group_id_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_EGR_PORT_GROUP_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_global_vrf_allowed_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30601); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_global_vrf_allowed of_oxm_bsn_global_vrf_allowed
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_global_vrf_allowed_push_wire_values(of_oxm_bsn_global_vrf_allowed_t *obj)
{

    of_oxm_bsn_global_vrf_allowed_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_global_vrf_allowed object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_global_vrf_allowed
 */

of_oxm_bsn_global_vrf_allowed_t *
of_oxm_bsn_global_vrf_allowed_new(of_version_t version)
{
    of_oxm_bsn_global_vrf_allowed_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED] + of_object_extra_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_global_vrf_allowed_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_global_vrf_allowed_init(obj, version, bytes, 0);

    if (of_oxm_bsn_global_vrf_allowed_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_global_vrf_allowed.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_global_vrf_allowed_init(of_oxm_bsn_global_vrf_allowed_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED] + of_object_extra_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_GLOBAL_VRF_ALLOWED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_global_vrf_allowed_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_global_vrf_allowed.
 * @param obj Pointer to an object of type of_oxm_bsn_global_vrf_allowed.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_bsn_global_vrf_allowed_value_get(
    of_oxm_bsn_global_vrf_allowed_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_GLOBAL_VRF_ALLOWED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_global_vrf_allowed.
 * @param obj Pointer to an object of type of_oxm_bsn_global_vrf_allowed.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_global_vrf_allowed_value_set(
    of_oxm_bsn_global_vrf_allowed_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_GLOBAL_VRF_ALLOWED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_global_vrf_allowed_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30702); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_global_vrf_allowed_masked of_oxm_bsn_global_vrf_allowed_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_global_vrf_allowed_masked_push_wire_values(of_oxm_bsn_global_vrf_allowed_masked_t *obj)
{

    of_oxm_bsn_global_vrf_allowed_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_global_vrf_allowed_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_global_vrf_allowed_masked
 */

of_oxm_bsn_global_vrf_allowed_masked_t *
of_oxm_bsn_global_vrf_allowed_masked_new(of_version_t version)
{
    of_oxm_bsn_global_vrf_allowed_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED] + of_object_extra_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_global_vrf_allowed_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_global_vrf_allowed_masked_init(obj, version, bytes, 0);

    if (of_oxm_bsn_global_vrf_allowed_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_global_vrf_allowed_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_global_vrf_allowed_masked_init(of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED] + of_object_extra_len[version][OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_global_vrf_allowed_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_global_vrf_allowed_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_global_vrf_allowed_masked.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_bsn_global_vrf_allowed_masked_value_get(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_global_vrf_allowed_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_global_vrf_allowed_masked.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_global_vrf_allowed_masked_value_set(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_bsn_global_vrf_allowed_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_global_vrf_allowed_masked.
 * @param value_mask Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_bsn_global_vrf_allowed_masked_value_mask_get(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    uint8_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 5;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_bsn_global_vrf_allowed_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_global_vrf_allowed_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_bsn_global_vrf_allowed_masked_value_mask_set(
    of_oxm_bsn_global_vrf_allowed_masked_t *obj,
    uint8_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_GLOBAL_VRF_ALLOWED_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 5;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_in_ports_128_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30010); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_in_ports_128 of_oxm_bsn_in_ports_128
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_in_ports_128_push_wire_values(of_oxm_bsn_in_ports_128_t *obj)
{

    of_oxm_bsn_in_ports_128_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_in_ports_128 object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_in_ports_128
 */

of_oxm_bsn_in_ports_128_t *
of_oxm_bsn_in_ports_128_new(of_version_t version)
{
    of_oxm_bsn_in_ports_128_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_IN_PORTS_128] + of_object_extra_len[version][OF_OXM_BSN_IN_PORTS_128];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_in_ports_128_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_in_ports_128_init(obj, version, bytes, 0);

    if (of_oxm_bsn_in_ports_128_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_in_ports_128.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_in_ports_128_init(of_oxm_bsn_in_ports_128_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_IN_PORTS_128] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_IN_PORTS_128] + of_object_extra_len[version][OF_OXM_BSN_IN_PORTS_128];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_IN_PORTS_128;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_in_ports_128_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_in_ports_128.
 * @param obj Pointer to an object of type of_oxm_bsn_in_ports_128.
 * @param value Pointer to the child object of type
 * of_bitmap_128_t to be filled out.
 *
 */
void
of_oxm_bsn_in_ports_128_value_get(
    of_oxm_bsn_in_ports_128_t *obj,
    of_bitmap_128_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_IN_PORTS_128);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_bitmap_128_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_in_ports_128.
 * @param obj Pointer to an object of type of_oxm_bsn_in_ports_128.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_in_ports_128_value_set(
    of_oxm_bsn_in_ports_128_t *obj,
    of_bitmap_128_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_IN_PORTS_128);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_bitmap_128_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_in_ports_128_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30120); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_in_ports_128_masked of_oxm_bsn_in_ports_128_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_in_ports_128_masked_push_wire_values(of_oxm_bsn_in_ports_128_masked_t *obj)
{

    of_oxm_bsn_in_ports_128_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_in_ports_128_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_in_ports_128_masked
 */

of_oxm_bsn_in_ports_128_masked_t *
of_oxm_bsn_in_ports_128_masked_new(of_version_t version)
{
    of_oxm_bsn_in_ports_128_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_IN_PORTS_128_MASKED] + of_object_extra_len[version][OF_OXM_BSN_IN_PORTS_128_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_in_ports_128_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_in_ports_128_masked_init(obj, version, bytes, 0);

    if (of_oxm_bsn_in_ports_128_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_in_ports_128_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_in_ports_128_masked_init(of_oxm_bsn_in_ports_128_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_IN_PORTS_128_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_IN_PORTS_128_MASKED] + of_object_extra_len[version][OF_OXM_BSN_IN_PORTS_128_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_IN_PORTS_128_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_in_ports_128_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_in_ports_128_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_in_ports_128_masked.
 * @param value Pointer to the child object of type
 * of_bitmap_128_t to be filled out.
 *
 */
void
of_oxm_bsn_in_ports_128_masked_value_get(
    of_oxm_bsn_in_ports_128_masked_t *obj,
    of_bitmap_128_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_IN_PORTS_128_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_bitmap_128_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_in_ports_128_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_in_ports_128_masked.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_in_ports_128_masked_value_set(
    of_oxm_bsn_in_ports_128_masked_t *obj,
    of_bitmap_128_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_IN_PORTS_128_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_bitmap_128_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_bsn_in_ports_128_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_in_ports_128_masked.
 * @param value_mask Pointer to the child object of type
 * of_bitmap_128_t to be filled out.
 *
 */
void
of_oxm_bsn_in_ports_128_masked_value_mask_get(
    of_oxm_bsn_in_ports_128_masked_t *obj,
    of_bitmap_128_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_IN_PORTS_128_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_bitmap_128_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_bsn_in_ports_128_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_in_ports_128_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_bsn_in_ports_128_masked_value_mask_set(
    of_oxm_bsn_in_ports_128_masked_t *obj,
    of_bitmap_128_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_IN_PORTS_128_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_bitmap_128_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_l3_dst_class_id_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30c04); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_l3_dst_class_id of_oxm_bsn_l3_dst_class_id
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_l3_dst_class_id_push_wire_values(of_oxm_bsn_l3_dst_class_id_t *obj)
{

    of_oxm_bsn_l3_dst_class_id_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_l3_dst_class_id object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_l3_dst_class_id
 */

of_oxm_bsn_l3_dst_class_id_t *
of_oxm_bsn_l3_dst_class_id_new(of_version_t version)
{
    of_oxm_bsn_l3_dst_class_id_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_DST_CLASS_ID] + of_object_extra_len[version][OF_OXM_BSN_L3_DST_CLASS_ID];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_l3_dst_class_id_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_l3_dst_class_id_init(obj, version, bytes, 0);

    if (of_oxm_bsn_l3_dst_class_id_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_l3_dst_class_id.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_l3_dst_class_id_init(of_oxm_bsn_l3_dst_class_id_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_L3_DST_CLASS_ID] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_DST_CLASS_ID] + of_object_extra_len[version][OF_OXM_BSN_L3_DST_CLASS_ID];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_L3_DST_CLASS_ID;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_l3_dst_class_id_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_l3_dst_class_id.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_dst_class_id.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_dst_class_id_value_get(
    of_oxm_bsn_l3_dst_class_id_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_DST_CLASS_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_l3_dst_class_id.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_dst_class_id.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_l3_dst_class_id_value_set(
    of_oxm_bsn_l3_dst_class_id_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_DST_CLASS_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_l3_dst_class_id_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30d08); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_l3_dst_class_id_masked of_oxm_bsn_l3_dst_class_id_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_l3_dst_class_id_masked_push_wire_values(of_oxm_bsn_l3_dst_class_id_masked_t *obj)
{

    of_oxm_bsn_l3_dst_class_id_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_l3_dst_class_id_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_l3_dst_class_id_masked
 */

of_oxm_bsn_l3_dst_class_id_masked_t *
of_oxm_bsn_l3_dst_class_id_masked_new(of_version_t version)
{
    of_oxm_bsn_l3_dst_class_id_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_DST_CLASS_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_L3_DST_CLASS_ID_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_l3_dst_class_id_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_l3_dst_class_id_masked_init(obj, version, bytes, 0);

    if (of_oxm_bsn_l3_dst_class_id_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_l3_dst_class_id_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_l3_dst_class_id_masked_init(of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_L3_DST_CLASS_ID_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_DST_CLASS_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_L3_DST_CLASS_ID_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_L3_DST_CLASS_ID_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_l3_dst_class_id_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_l3_dst_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_dst_class_id_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_dst_class_id_masked_value_get(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_DST_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_l3_dst_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_dst_class_id_masked.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_l3_dst_class_id_masked_value_set(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_DST_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_bsn_l3_dst_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_dst_class_id_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_dst_class_id_masked_value_mask_get(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_DST_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_bsn_l3_dst_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_dst_class_id_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_bsn_l3_dst_class_id_masked_value_mask_set(
    of_oxm_bsn_l3_dst_class_id_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_DST_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_l3_interface_class_id_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30804); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_l3_interface_class_id of_oxm_bsn_l3_interface_class_id
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_l3_interface_class_id_push_wire_values(of_oxm_bsn_l3_interface_class_id_t *obj)
{

    of_oxm_bsn_l3_interface_class_id_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_l3_interface_class_id object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_l3_interface_class_id
 */

of_oxm_bsn_l3_interface_class_id_t *
of_oxm_bsn_l3_interface_class_id_new(of_version_t version)
{
    of_oxm_bsn_l3_interface_class_id_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID] + of_object_extra_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_l3_interface_class_id_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_l3_interface_class_id_init(obj, version, bytes, 0);

    if (of_oxm_bsn_l3_interface_class_id_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_l3_interface_class_id.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_l3_interface_class_id_init(of_oxm_bsn_l3_interface_class_id_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID] + of_object_extra_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_L3_INTERFACE_CLASS_ID;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_l3_interface_class_id_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_l3_interface_class_id.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_interface_class_id.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_interface_class_id_value_get(
    of_oxm_bsn_l3_interface_class_id_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_INTERFACE_CLASS_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_l3_interface_class_id.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_interface_class_id.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_l3_interface_class_id_value_set(
    of_oxm_bsn_l3_interface_class_id_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_INTERFACE_CLASS_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_l3_interface_class_id_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30908); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_l3_interface_class_id_masked of_oxm_bsn_l3_interface_class_id_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_l3_interface_class_id_masked_push_wire_values(of_oxm_bsn_l3_interface_class_id_masked_t *obj)
{

    of_oxm_bsn_l3_interface_class_id_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_l3_interface_class_id_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_l3_interface_class_id_masked
 */

of_oxm_bsn_l3_interface_class_id_masked_t *
of_oxm_bsn_l3_interface_class_id_masked_new(of_version_t version)
{
    of_oxm_bsn_l3_interface_class_id_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_l3_interface_class_id_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_l3_interface_class_id_masked_init(obj, version, bytes, 0);

    if (of_oxm_bsn_l3_interface_class_id_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_l3_interface_class_id_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_l3_interface_class_id_masked_init(of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_l3_interface_class_id_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_l3_interface_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_interface_class_id_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_interface_class_id_masked_value_get(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_l3_interface_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_interface_class_id_masked.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_l3_interface_class_id_masked_value_set(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_bsn_l3_interface_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_interface_class_id_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_interface_class_id_masked_value_mask_get(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_bsn_l3_interface_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_interface_class_id_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_bsn_l3_interface_class_id_masked_value_mask_set(
    of_oxm_bsn_l3_interface_class_id_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_INTERFACE_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_l3_src_class_id_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30a04); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_l3_src_class_id of_oxm_bsn_l3_src_class_id
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_l3_src_class_id_push_wire_values(of_oxm_bsn_l3_src_class_id_t *obj)
{

    of_oxm_bsn_l3_src_class_id_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_l3_src_class_id object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_l3_src_class_id
 */

of_oxm_bsn_l3_src_class_id_t *
of_oxm_bsn_l3_src_class_id_new(of_version_t version)
{
    of_oxm_bsn_l3_src_class_id_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID] + of_object_extra_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_l3_src_class_id_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_l3_src_class_id_init(obj, version, bytes, 0);

    if (of_oxm_bsn_l3_src_class_id_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_l3_src_class_id.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_l3_src_class_id_init(of_oxm_bsn_l3_src_class_id_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID] + of_object_extra_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_L3_SRC_CLASS_ID;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_l3_src_class_id_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_l3_src_class_id.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_src_class_id.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_src_class_id_value_get(
    of_oxm_bsn_l3_src_class_id_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_SRC_CLASS_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_l3_src_class_id.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_src_class_id.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_l3_src_class_id_value_set(
    of_oxm_bsn_l3_src_class_id_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_SRC_CLASS_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_l3_src_class_id_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30b08); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_l3_src_class_id_masked of_oxm_bsn_l3_src_class_id_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_l3_src_class_id_masked_push_wire_values(of_oxm_bsn_l3_src_class_id_masked_t *obj)
{

    of_oxm_bsn_l3_src_class_id_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_l3_src_class_id_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_l3_src_class_id_masked
 */

of_oxm_bsn_l3_src_class_id_masked_t *
of_oxm_bsn_l3_src_class_id_masked_new(of_version_t version)
{
    of_oxm_bsn_l3_src_class_id_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_l3_src_class_id_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_l3_src_class_id_masked_init(obj, version, bytes, 0);

    if (of_oxm_bsn_l3_src_class_id_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_l3_src_class_id_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_l3_src_class_id_masked_init(of_oxm_bsn_l3_src_class_id_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_l3_src_class_id_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_l3_src_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_src_class_id_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_src_class_id_masked_value_get(
    of_oxm_bsn_l3_src_class_id_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_l3_src_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_src_class_id_masked.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_l3_src_class_id_masked_value_set(
    of_oxm_bsn_l3_src_class_id_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_bsn_l3_src_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_src_class_id_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_l3_src_class_id_masked_value_mask_get(
    of_oxm_bsn_l3_src_class_id_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_bsn_l3_src_class_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_l3_src_class_id_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_bsn_l3_src_class_id_masked_value_mask_set(
    of_oxm_bsn_l3_src_class_id_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_L3_SRC_CLASS_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_lag_id_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30204); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_lag_id of_oxm_bsn_lag_id
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_lag_id_push_wire_values(of_oxm_bsn_lag_id_t *obj)
{

    of_oxm_bsn_lag_id_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_lag_id object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_lag_id
 */

of_oxm_bsn_lag_id_t *
of_oxm_bsn_lag_id_new(of_version_t version)
{
    of_oxm_bsn_lag_id_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_LAG_ID] + of_object_extra_len[version][OF_OXM_BSN_LAG_ID];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_lag_id_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_lag_id_init(obj, version, bytes, 0);

    if (of_oxm_bsn_lag_id_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_lag_id.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_lag_id_init(of_oxm_bsn_lag_id_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_LAG_ID] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_LAG_ID] + of_object_extra_len[version][OF_OXM_BSN_LAG_ID];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_LAG_ID;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_lag_id_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_lag_id.
 * @param obj Pointer to an object of type of_oxm_bsn_lag_id.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_lag_id_value_get(
    of_oxm_bsn_lag_id_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_LAG_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_lag_id.
 * @param obj Pointer to an object of type of_oxm_bsn_lag_id.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_lag_id_value_set(
    of_oxm_bsn_lag_id_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_LAG_ID);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_lag_id_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x30308); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_lag_id_masked of_oxm_bsn_lag_id_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_lag_id_masked_push_wire_values(of_oxm_bsn_lag_id_masked_t *obj)
{

    of_oxm_bsn_lag_id_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_lag_id_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_lag_id_masked
 */

of_oxm_bsn_lag_id_masked_t *
of_oxm_bsn_lag_id_masked_new(of_version_t version)
{
    of_oxm_bsn_lag_id_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_LAG_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_LAG_ID_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_lag_id_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_lag_id_masked_init(obj, version, bytes, 0);

    if (of_oxm_bsn_lag_id_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_lag_id_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_lag_id_masked_init(of_oxm_bsn_lag_id_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_LAG_ID_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_LAG_ID_MASKED] + of_object_extra_len[version][OF_OXM_BSN_LAG_ID_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_LAG_ID_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_lag_id_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_lag_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_lag_id_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_lag_id_masked_value_get(
    of_oxm_bsn_lag_id_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_LAG_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_lag_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_lag_id_masked.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_lag_id_masked_value_set(
    of_oxm_bsn_lag_id_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_LAG_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_bsn_lag_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_lag_id_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_lag_id_masked_value_mask_get(
    of_oxm_bsn_lag_id_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_LAG_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_bsn_lag_id_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_lag_id_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_bsn_lag_id_masked_value_mask_set(
    of_oxm_bsn_lag_id_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_LAG_ID_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_udf0_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x31004); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_udf0 of_oxm_bsn_udf0
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_udf0_push_wire_values(of_oxm_bsn_udf0_t *obj)
{

    of_oxm_bsn_udf0_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_udf0 object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_udf0
 */

of_oxm_bsn_udf0_t *
of_oxm_bsn_udf0_new(of_version_t version)
{
    of_oxm_bsn_udf0_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_UDF0] + of_object_extra_len[version][OF_OXM_BSN_UDF0];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_udf0_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_udf0_init(obj, version, bytes, 0);

    if (of_oxm_bsn_udf0_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_udf0.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_udf0_init(of_oxm_bsn_udf0_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_UDF0] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_UDF0] + of_object_extra_len[version][OF_OXM_BSN_UDF0];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_UDF0;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_udf0_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_udf0.
 * @param obj Pointer to an object of type of_oxm_bsn_udf0.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_udf0_value_get(
    of_oxm_bsn_udf0_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_UDF0);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_udf0.
 * @param obj Pointer to an object of type of_oxm_bsn_udf0.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_udf0_value_set(
    of_oxm_bsn_udf0_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_UDF0);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_oxm_bsn_udf0_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x31108); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_bsn_udf0_masked of_oxm_bsn_udf0_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_bsn_udf0_masked_push_wire_values(of_oxm_bsn_udf0_masked_t *obj)
{

    of_oxm_bsn_udf0_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_bsn_udf0_masked object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_oxm_bsn_udf0_masked
 */

of_oxm_bsn_udf0_masked_t *
of_oxm_bsn_udf0_masked_new(of_version_t version)
{
    of_oxm_bsn_udf0_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_BSN_UDF0_MASKED] + of_object_extra_len[version][OF_OXM_BSN_UDF0_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_bsn_udf0_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_bsn_udf0_masked_init(obj, version, bytes, 0);

    if (of_oxm_bsn_udf0_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_bsn_udf0_masked.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_oxm_bsn_udf0_masked_init(of_oxm_bsn_udf0_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_BSN_UDF0_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_BSN_UDF0_MASKED] + of_object_extra_len[version][OF_OXM_BSN_UDF0_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_BSN_UDF0_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_bsn_udf0_masked_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_oxm_bsn_udf0_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_udf0_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_udf0_masked_value_get(
    of_oxm_bsn_udf0_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_UDF0_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_bsn_udf0_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_udf0_masked.
 * @param value The value to write into the object
 */
void
of_oxm_bsn_udf0_masked_value_set(
    of_oxm_bsn_udf0_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_UDF0_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_bsn_udf0_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_udf0_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_bsn_udf0_masked_value_mask_get(
    of_oxm_bsn_udf0_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_UDF0_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_bsn_udf0_masked.
 * @param obj Pointer to an object of type of_oxm_bsn_udf0_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_bsn_udf0_masked_value_mask_set(
    of_oxm_bsn_udf0_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_BSN_UDF0_MASKED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
