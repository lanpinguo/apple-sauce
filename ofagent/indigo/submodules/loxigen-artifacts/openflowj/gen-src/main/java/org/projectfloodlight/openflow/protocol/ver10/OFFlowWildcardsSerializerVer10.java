// Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University
// Copyright (c) 2011, 2012 Open Networking Foundation
// Copyright (c) 2012, 2013 Big Switch Networks, Inc.
// This library was generated by the LoxiGen Compiler.
// See the file LICENSE.txt which should have been included in the source distribution

// Automatically generated by LOXI from template const_set_serializer.java
// Do not modify

package org.projectfloodlight.openflow.protocol.ver10;

import org.projectfloodlight.openflow.protocol.*;
import org.projectfloodlight.openflow.protocol.action.*;
import org.projectfloodlight.openflow.protocol.actionid.*;
import org.projectfloodlight.openflow.protocol.bsntlv.*;
import org.projectfloodlight.openflow.protocol.errormsg.*;
import org.projectfloodlight.openflow.protocol.meterband.*;
import org.projectfloodlight.openflow.protocol.instruction.*;
import org.projectfloodlight.openflow.protocol.instructionid.*;
import org.projectfloodlight.openflow.protocol.match.*;
import org.projectfloodlight.openflow.protocol.oxm.*;
import org.projectfloodlight.openflow.protocol.queueprop.*;
import org.projectfloodlight.openflow.types.*;
import org.projectfloodlight.openflow.util.*;
import org.projectfloodlight.openflow.exceptions.*;
import org.projectfloodlight.openflow.protocol.OFFlowWildcards;
import java.util.Set;
import org.jboss.netty.buffer.ChannelBuffer;
import com.google.common.hash.PrimitiveSink;
import java.util.EnumSet;
import java.util.Collections;


public class OFFlowWildcardsSerializerVer10 {

    public final static int IN_PORT_VAL = 0x1;
    public final static int DL_VLAN_VAL = 0x2;
    public final static int DL_SRC_VAL = 0x4;
    public final static int DL_DST_VAL = 0x8;
    public final static int DL_TYPE_VAL = 0x10;
    public final static int NW_PROTO_VAL = 0x20;
    public final static int TP_SRC_VAL = 0x40;
    public final static int TP_DST_VAL = 0x80;
    public final static int NW_SRC_ALL_VAL = 0x2000;
    public final static int NW_SRC_MASK_VAL = 0x3f00;
    public final static int NW_DST_ALL_VAL = 0x80000;
    public final static int NW_DST_MASK_VAL = 0xfc000;
    public final static int DL_VLAN_PCP_VAL = 0x100000;
    public final static int NW_TOS_VAL = 0x200000;
    public final static int ALL_VAL = 0x3fffff;

    public static Set<OFFlowWildcards> readFrom(ChannelBuffer bb) throws OFParseError {
        try {
            return ofWireValue(bb.readInt());
        } catch (IllegalArgumentException e) {
            throw new OFParseError(e);
        }
    }

    public static void writeTo(ChannelBuffer bb, Set<OFFlowWildcards> set) {
        bb.writeInt(toWireValue(set));
    }

    public static void putTo(Set<OFFlowWildcards> set, PrimitiveSink sink) {
        sink.putInt(toWireValue(set));
    }


    public static Set<OFFlowWildcards> ofWireValue(int val) {
        EnumSet<OFFlowWildcards> set = EnumSet.noneOf(OFFlowWildcards.class);

        if((val & IN_PORT_VAL) != 0)
            set.add(OFFlowWildcards.IN_PORT);
        if((val & DL_VLAN_VAL) != 0)
            set.add(OFFlowWildcards.DL_VLAN);
        if((val & DL_SRC_VAL) != 0)
            set.add(OFFlowWildcards.DL_SRC);
        if((val & DL_DST_VAL) != 0)
            set.add(OFFlowWildcards.DL_DST);
        if((val & DL_TYPE_VAL) != 0)
            set.add(OFFlowWildcards.DL_TYPE);
        if((val & NW_PROTO_VAL) != 0)
            set.add(OFFlowWildcards.NW_PROTO);
        if((val & TP_SRC_VAL) != 0)
            set.add(OFFlowWildcards.TP_SRC);
        if((val & TP_DST_VAL) != 0)
            set.add(OFFlowWildcards.TP_DST);
        if((val & NW_SRC_ALL_VAL) != 0)
            set.add(OFFlowWildcards.NW_SRC_ALL);
        if((val & NW_SRC_MASK_VAL) != 0)
            set.add(OFFlowWildcards.NW_SRC_MASK);
        if((val & NW_DST_ALL_VAL) != 0)
            set.add(OFFlowWildcards.NW_DST_ALL);
        if((val & NW_DST_MASK_VAL) != 0)
            set.add(OFFlowWildcards.NW_DST_MASK);
        if((val & DL_VLAN_PCP_VAL) != 0)
            set.add(OFFlowWildcards.DL_VLAN_PCP);
        if((val & NW_TOS_VAL) != 0)
            set.add(OFFlowWildcards.NW_TOS);
        if((val & ALL_VAL) != 0)
            set.add(OFFlowWildcards.ALL);
        return Collections.unmodifiableSet(set);
    }

    public static int toWireValue(Set<OFFlowWildcards> set) {
        int wireValue = 0;

        for(OFFlowWildcards e: set) {
            switch(e) {
                case IN_PORT:
                    wireValue |= IN_PORT_VAL;
                    break;
                case DL_VLAN:
                    wireValue |= DL_VLAN_VAL;
                    break;
                case DL_SRC:
                    wireValue |= DL_SRC_VAL;
                    break;
                case DL_DST:
                    wireValue |= DL_DST_VAL;
                    break;
                case DL_TYPE:
                    wireValue |= DL_TYPE_VAL;
                    break;
                case NW_PROTO:
                    wireValue |= NW_PROTO_VAL;
                    break;
                case TP_SRC:
                    wireValue |= TP_SRC_VAL;
                    break;
                case TP_DST:
                    wireValue |= TP_DST_VAL;
                    break;
                case NW_SRC_ALL:
                    wireValue |= NW_SRC_ALL_VAL;
                    break;
                case NW_SRC_MASK:
                    wireValue |= NW_SRC_MASK_VAL;
                    break;
                case NW_DST_ALL:
                    wireValue |= NW_DST_ALL_VAL;
                    break;
                case NW_DST_MASK:
                    wireValue |= NW_DST_MASK_VAL;
                    break;
                case DL_VLAN_PCP:
                    wireValue |= DL_VLAN_PCP_VAL;
                    break;
                case NW_TOS:
                    wireValue |= NW_TOS_VAL;
                    break;
                case ALL:
                    wireValue |= ALL_VAL;
                    break;
                default:
                    throw new IllegalArgumentException("Illegal enum value for type OFFlowWildcards in version 1.0: " + e);
            }
        }
        return wireValue;
    }

}
