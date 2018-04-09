
import sys
import os
import socket
import time
import struct
import select
import logging
from threading import Thread
from threading import Lock
from threading import Condition

RCV_SIZE_DEFAULT = 32768
LISTEN_QUEUE_SIZE = 1


class Controller(Thread):


	def __init__(self, switch=None, host='127.0.0.1', port=6633, max_pkts=1024):
		Thread.__init__(self)
		# Socket related
		self.rcv_size = RCV_SIZE_DEFAULT
		self.listen_socket = None
		self.switch_socket = None
		self.switch_addr = None
		self.connect_cv = Condition()
		self.message_cv = Condition()
		self.tx_lock = Lock()


		# Counters
		self.socket_errors = 0
		self.parse_errors = 0
		self.packets_total = 0
		self.packets_expired = 0
		self.packets_handled = 0
		self.poll_discards = 0

		# State
		self.sync = Lock()
		self.handlers = {}
		self.keep_alive = False
		self.active = True
		self.initial_hello = True

		# OpenFlow message/packet queue
		# Protected by the packets_cv lock / condition variable
		self.packets = []
		self.packets_cv = Condition()
		self.packet_in_count = 0

		# Settings
		self.max_pkts = max_pkts
		self.switch = switch
		self.passive = not self.switch
		self.host = host
		self.port = port
		self.dbg_state = "init"
		self.filter_packet_in = False # Drop "excessive" packet ins
		self.pkt_in_run = 0 # Count on run of packet ins
		self.pkt_in_filter_limit = 50 # Count on run of packet ins
		self.pkt_in_dropped = 0 # Total dropped packet ins
		self.transact_to = 15 # Transact timeout default value; add to config



		self.buffered_input = ""

		# Create listen socket
		if self.passive:
			print("Create/listen at " + self.host + ":" +
							 str(self.port))
			ai = socket.getaddrinfo(self.host, self.port, socket.AF_UNSPEC,
									socket.SOCK_STREAM, 0, socket.AI_PASSIVE)
			# Use first returned addrinfo
			(family, socktype, proto, name, sockaddr) = ai[0]
			self.listen_socket = socket.socket(family, socktype)
			self.listen_socket.setsockopt(socket.SOL_SOCKET,
										  socket.SO_REUSEADDR, 1)
			self.listen_socket.bind(sockaddr)
			self.listen_socket.listen(LISTEN_QUEUE_SIZE)

	def filter_packet(self, rawmsg, hdr):
		"""
		Check if packet should be filtered

		Currently filters packet in messages
		@return Boolean, True if packet should be dropped
		"""
		# XXX didn't actually check for packet-in...
		return False
		# Add check for packet in and rate limit
		if self.filter_packet_in:
			# If we were dropping packets, report number dropped
			# TODO dont drop expected packet ins
			if self.pkt_in_run > self.pkt_in_filter_limit:
				self.logger.debug("Dropped %d packet ins (%d total)"
							% ((self.pkt_in_run - 
								self.pkt_in_filter_limit),
								self.pkt_in_dropped))
			self.pkt_in_run = 0

		return False

	def _pkt_handle(self, pkt):
		"""
		Check for all packet handling conditions

		Parse and verify message 
		Check if XID matches something waiting
		Check if message is being expected for a poll operation
		Check if keep alive is on and message is an echo request
		Check if any registered handler wants the packet
		Enqueue if none of those conditions is met

		an echo request in case keep_alive is true, followed by
		registered message handlers.
		@param pkt The raw packet (string) which may contain multiple OF msgs
		"""

		# snag any left over data from last read()
		print pkt


	def _socket_ready_handle(self, s):
		"""
		Handle an input-ready socket

		@param s The socket object that is ready
		@returns 0 on success, -1 on error
		"""

		if self.passive and s and s == self.listen_socket:
			if self.switch_socket:
				print("Ignoring incoming connection; already connected to switch")
				(sock, addr) = self.listen_socket.accept()
				sock.close()
				return 0

			try:
				(sock, addr) = self.listen_socket.accept()
			except:
				print("Error on listen socket accept")
				return -1
			print(self.host+":"+str(self.port)+": Incoming connection from "+str(addr))

			with self.connect_cv:
				(self.switch_socket, self.switch_addr) = (sock, addr)
				self.switch_socket.setsockopt(socket.IPPROTO_TCP,
											  socket.TCP_NODELAY, True)
				self.connect_cv.notify() # Notify anyone waiting

			# Prevent further connections
			self.listen_socket.close()
			self.listen_socket = None

		elif s and s == self.switch_socket:
			for idx in range(3): # debug: try a couple of times
				try:
					pkt = self.switch_socket.recv(self.rcv_size)
				except:
					print("Error on switch read")
					return -1
	  
				if not self.active:
					return 0
	  
				if len(pkt) == 0:
					print("Zero-length switch read, %d" % idx)
				else:
					break

			if len(pkt) == 0: # Still no packet
				print("Zero-length switch read; continue")
				print(str(self))
				return 0

			self._pkt_handle(pkt)
			
		else:
			print("Unknown socket ready: " + str(s))
			return -1

		return 0

	def active_connect(self):
		"""
		Actively connect to a switch IP addr
		"""
		try:
			print("Trying active connection to %s" % self.switch)
			soc = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
			soc.connect((self.switch))
			print("Connected to " + self.switch + " on " +
						 str(self.port))
			#soc.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, True)
			self.switch_addr = (self.switch, self.port)
			return soc
		except (StandardError, socket.error), e:
			print("Could not connect to %s at %d:: %s" % 
							  (self.switch, self.port, str(e)))
		return None

	def connect(self, timeout=-1):
		"""
		Connect to the switch

		@param timeout Block for up to timeout seconds. Pass -1 for the default.
		@return Boolean, True if connected
		"""

		if not self.passive:  # Do active connection now
			print("Attempting to connect to %s on port %s" %
							 (self.switch, str(self.port)))
			soc = self.active_connect()
			if soc:
				#print("Connected to %s" % self.switch)
				self.dbg_state = "running"
				self.switch_socket = soc
				self.wakeup()
			else:
				print("Could not actively connect to switch %s",
								  self.switch)
				self.active = False
		else:
			pass

		return self.switch_socket is not None		
		
	def wakeup(self):
		"""
		Wake up the event loop, presumably from another thread.
		"""
		pass

	def sockets(self):
		"""
		Return list of sockets to select on.
		"""
		socs = [self.listen_socket, self.switch_socket]
		return [x for x in socs if x]

	def run(self):
		"""
		Activity function for class

		Assumes connection to switch already exists.  Listens on
		switch_socket for messages until an error (or zero len pkt)
		occurs.

		When there is a message on the socket, check for handlers; queue the
		packet if no one handles the packet.

		See note for controller describing the limitation of a single
		connection for now.
		"""

		self.dbg_state = "running"

		while self.active:
			try:
				sel_in, sel_out, sel_err = \
					select.select(self.sockets(), [], self.sockets(), 1)
			except:
				print sys.exc_info()
				print("Select error, disconnecting")
				self.disconnect()

			for s in sel_err:
				print("Got socket error on: " + str(s) + ", disconnecting")
				self.disconnect()

			for s in sel_in:
				if self._socket_ready_handle(s) == -1:
					self.disconnect()

		# End of main loop
		self.dbg_state = "closing"
		print("Exiting controller thread")
		self.shutdown()


		
	def disconnect(self, timeout=-1):
		"""
		If connected to a switch, disconnect.
		"""
		if self.switch_socket:
			self.switch_socket.close()
			self.switch_socket = None
			self.switch_addr = None
			with self.packets_cv:
				self.packets = []
			with self.connect_cv:
				self.connect_cv.notifyAll()

	def wait_disconnected(self, timeout=-1):
		"""
		@param timeout Block for up to timeout seconds. Pass -1 for the default.
		@return Boolean, True if disconnected
		"""

		with self.connect_cv:
			ofutils.timed_wait(self.connect_cv, 
							   lambda: True if not self.switch_socket else None, 
							   timeout=timeout)
		return self.switch_socket is None
		
	def kill(self):
		"""
		Force the controller thread to quit
		"""
		self.active = False
		self.wakeup()
		self.join()

	def shutdown(self):
		"""
		Shutdown the controller closing all sockets

		@todo Might want to synchronize shutdown with self.sync...
		"""

		self.active = False
		try:
			self.switch_socket.shutdown(socket.SHUT_RDWR)
		except:
			print("Ignoring switch soc shutdown error")
		self.switch_socket = None

		if self.listen_socket != None:
			try:
				self.listen_socket.shutdown(socket.SHUT_RDWR)
			except:
				print("Ignoring listen soc shutdown error")
			self.listen_socket = None

		# Wakeup condition variables on which controller may be wait

		with self.connect_cv:
			self.connect_cv.notifyAll()

		self.wakeup()
		self.dbg_state = "down"

	def register(self, msg_type, handler):
		"""
		Register a callback to receive a specific message type.

		Only one handler may be registered for a given message type.

		WARNING:  A lock is held during the handler call back, so 
		the handler should not make any blocking calls

		@param msg_type The type of message to receive.  May be DEFAULT 
		for all non-handled packets.  The special type, the string "all"
		will send all packets to the handler.
		@param handler The function to call when a message of the given 
		type is received.
		"""
		# Should check type is valid
		if not handler and msg_type in self.handlers.keys():
			del self.handlers[msg_type]
			return
		self.handlers[msg_type] = handler





	def message_send(self, msg):
		"""
		Send the message to the switch

		@param msg A string or OpenFlow message object to be forwarded to
		the switch.
		"""

		if not self.switch_socket:
			# Sending a string indicates the message is ready to go
			return -1
			

		outpkt = msg

		with self.tx_lock:
			if self.switch_socket.sendall(outpkt) is not None:
				raise AssertionError("failed to send message to switch")

		return 0 # for backwards compatibility

	def clear_queue(self):
		"""
		Clear the input queue and report the number of messages
		that were in it
		"""
		enqueued_pkt_count = len(self.packets)
		with self.packets_cv:
			self.packets = []
		return enqueued_pkt_count

	def __str__(self):
		string = "Controller:\n"
		string += "  state			 " + self.dbg_state + "\n"
		string += "  switch_addr	 " + str(self.switch_addr) + "\n"
		string += "  pending pkts	 " + str(len(self.packets)) + "\n"
		string += "  total pkts 	 " + str(self.packets_total) + "\n"
		string += "  expired pkts	 " + str(self.packets_expired) + "\n"
		string += "  handled pkts	 " + str(self.packets_handled) + "\n"
		string += "  poll discards	 " + str(self.poll_discards) + "\n"
		string += "  parse errors	 " + str(self.parse_errors) + "\n"
		string += "  sock errrors	 " + str(self.socket_errors) + "\n"
		string += "  max pkts		 " + str(self.max_pkts) + "\n"
		string += "  target switch	 " + str(self.switch) + "\n"
		string += "  host			 " + str(self.host) + "\n"
		string += "  port			 " + str(self.port) + "\n"
		string += "  keep_alive 	 " + str(self.keep_alive) + "\n"
		string += "  pkt_in_run 	 " + str(self.pkt_in_run) + "\n"
		string += "  pkt_in_dropped  " + str(self.pkt_in_dropped) + "\n"
		return string

	def show(self):
		print str(self)

		
		
		
if __name__ == "__main__":
	"""
	self test
	"""
	server = Controller(switch='/tmp/datapath_port_manager')

	server.start()
	server.connect()
	while True:
		cmd = raw_input('cmd: ')
		if cmd == 'exit':
			break
		elif cmd == 'start':
			server.connect()
		elif '@' in cmd:
			if server.message_send(cmd[1:]) == 0:
				print('send ok')
			else:
				print('send failed')
		elif len(cmd) == 0:
			pass
		else:
			print('unknown cmd') 

	server.kill()
	

		
