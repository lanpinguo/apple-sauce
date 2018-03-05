#! /usr/bin/env python

"""
  Example API calls to use the packet reception API to receive packets. It prints data from the
  received packet and then sends it back out the port specified in the variable outPortNum.

  This script serves as a simple example illustrating how these APIs may be used.
"""
from OFDPA_python import *

def main():
    rc = ofdpaClientInitialize("Packet Receive Example")
    if rc == OFDPA_E_NONE:
        rc = ofdpaClientPktSockBind()

        # variables used
        inPortNum = 1
        outPortNum = 2

        pktBufferSize_p = new_uint32_tp()
        ofdpaMaxPktSizeGet(pktBufferSize_p)
        pktBufferSize = uint32_tp_value(pktBufferSize_p)
        delete_uint32_tp(pktBufferSize_p)

        # enter loop to receive and display packets
        packet = ofdpaPacket_t()
        packet.pktData.size = pktBufferSize

        BuffDesc_pstart_setbytes(packet.pktData, None)

        print "Receiving Packets... (use ctrl-C to stop)"

        while rc == OFDPA_E_NONE:
          rc = ofdpaPktReceive(None, packet)
          if rc == OFDPA_E_NONE:
            print 'Packet received on port ' + str(packet.inPortNum) + ': length ' + str(packet.pktData.size) + ' table ID ' + str(packet.tableId)

            for i in xrange(packet.pktData.size):
              if ((i != 0) and ((i % 16) == 0)):
                print
              print "%02x" % BuffDesc_getbyte(packet.pktData, i),
            print

            # pare off the FCS bytes from received packet, new FCS will be generated upon transmission
            packet.pktData.size = packet.pktData.size-4

            # Fiddle with last byte so we can tell the packet is one we sent from
            # CPU vs just being forwarded in hardware.
            lastByte = BuffDesc_getbyte(packet.pktData, packet.pktData.size-1)
            lastByte = lastByte + 1
            BuffDesc_setbyte(packet.pktData, packet.pktData.size-1, lastByte)

            print 'Sending received packet back out on port ' + str(outPortNum)
            rc1 = ofdpaPktSend(packet.pktData, 0, outPortNum, packet.inPortNum)
            if rc1 != OFDPA_E_NONE:
              print 'Error from ofdpaPktSend(): rc = ' + str(rc1)
            # reset packet buffer size to max allocated
            packet.pktData.size = pktBufferSize
          else:
            print "Error from ofdpaPktReceive(). rc = " + str(rc)

    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
