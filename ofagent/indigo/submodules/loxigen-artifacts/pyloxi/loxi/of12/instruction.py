# Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University
# Copyright (c) 2011, 2012 Open Networking Foundation
# Copyright (c) 2012, 2013 Big Switch Networks, Inc.
# See the file LICENSE.pyloxi which should have been included in the source distribution

# Automatically generated by LOXI from template module.py
# Do not modify

import struct
import loxi
import const
import common
import action
import instruction
import oxm
import util
import loxi.generic_util

class instruction(loxi.OFObject):
    subtypes = {}


    def __init__(self, type=None):
        if type != None:
            self.type = type
        else:
            self.type = 0
        return

    def pack(self):
        packed = []
        packed.append(struct.pack("!H", self.type))
        packed.append(struct.pack("!H", 0)) # placeholder for len at index 1
        packed.append('\x00' * 4)
        length = sum([len(x) for x in packed])
        packed[1] = struct.pack("!H", length)
        return ''.join(packed)

    @staticmethod
    def unpack(reader):
        subtype, = reader.peek('!H', 0)
        subclass = instruction.subtypes.get(subtype)
        if subclass:
            return subclass.unpack(reader)

        obj = instruction()
        obj.type = reader.read("!H")[0]
        _len = reader.read("!H")[0]
        orig_reader = reader
        reader = orig_reader.slice(_len - (2 + 2))
        reader.skip(4)
        return obj

    def __eq__(self, other):
        if type(self) != type(other): return False
        if self.type != other.type: return False
        return True

    def pretty_print(self, q):
        q.text("instruction {")
        with q.group():
            with q.indent(2):
                q.breakable()
            q.breakable()
        q.text('}')


class apply_actions(instruction):
    type = 4

    def __init__(self, actions=None):
        if actions != None:
            self.actions = actions
        else:
            self.actions = []
        return

    def pack(self):
        packed = []
        packed.append(struct.pack("!H", self.type))
        packed.append(struct.pack("!H", 0)) # placeholder for len at index 1
        packed.append('\x00' * 4)
        packed.append(loxi.generic_util.pack_list(self.actions))
        length = sum([len(x) for x in packed])
        packed[1] = struct.pack("!H", length)
        return ''.join(packed)

    @staticmethod
    def unpack(reader):
        obj = apply_actions()
        _type = reader.read("!H")[0]
        assert(_type == 4)
        _len = reader.read("!H")[0]
        orig_reader = reader
        reader = orig_reader.slice(_len - (2 + 2))
        reader.skip(4)
        obj.actions = loxi.generic_util.unpack_list(reader, action.action.unpack)
        return obj

    def __eq__(self, other):
        if type(self) != type(other): return False
        if self.actions != other.actions: return False
        return True

    def pretty_print(self, q):
        q.text("apply_actions {")
        with q.group():
            with q.indent(2):
                q.breakable()
                q.text("actions = ");
                q.pp(self.actions)
            q.breakable()
        q.text('}')

instruction.subtypes[4] = apply_actions

class clear_actions(instruction):
    type = 5

    def __init__(self):
        return

    def pack(self):
        packed = []
        packed.append(struct.pack("!H", self.type))
        packed.append(struct.pack("!H", 0)) # placeholder for len at index 1
        packed.append('\x00' * 4)
        length = sum([len(x) for x in packed])
        packed[1] = struct.pack("!H", length)
        return ''.join(packed)

    @staticmethod
    def unpack(reader):
        obj = clear_actions()
        _type = reader.read("!H")[0]
        assert(_type == 5)
        _len = reader.read("!H")[0]
        orig_reader = reader
        reader = orig_reader.slice(_len - (2 + 2))
        reader.skip(4)
        return obj

    def __eq__(self, other):
        if type(self) != type(other): return False
        return True

    def pretty_print(self, q):
        q.text("clear_actions {")
        with q.group():
            with q.indent(2):
                q.breakable()
            q.breakable()
        q.text('}')

instruction.subtypes[5] = clear_actions

class experimenter(instruction):
    subtypes = {}

    type = 65535

    def __init__(self, experimenter=None, data=None):
        if experimenter != None:
            self.experimenter = experimenter
        else:
            self.experimenter = 0
        if data != None:
            self.data = data
        else:
            self.data = ''
        return

    def pack(self):
        packed = []
        packed.append(struct.pack("!H", self.type))
        packed.append(struct.pack("!H", 0)) # placeholder for len at index 1
        packed.append(struct.pack("!L", self.experimenter))
        packed.append(self.data)
        length = sum([len(x) for x in packed])
        packed[1] = struct.pack("!H", length)
        return ''.join(packed)

    @staticmethod
    def unpack(reader):
        subtype, = reader.peek('!L', 4)
        subclass = experimenter.subtypes.get(subtype)
        if subclass:
            return subclass.unpack(reader)

        obj = experimenter()
        _type = reader.read("!H")[0]
        assert(_type == 65535)
        _len = reader.read("!H")[0]
        orig_reader = reader
        reader = orig_reader.slice(_len - (2 + 2))
        obj.experimenter = reader.read("!L")[0]
        obj.data = str(reader.read_all())
        return obj

    def __eq__(self, other):
        if type(self) != type(other): return False
        if self.experimenter != other.experimenter: return False
        if self.data != other.data: return False
        return True

    def pretty_print(self, q):
        q.text("experimenter {")
        with q.group():
            with q.indent(2):
                q.breakable()
                q.text("data = ");
                q.pp(self.data)
            q.breakable()
        q.text('}')

instruction.subtypes[65535] = experimenter

class goto_table(instruction):
    type = 1

    def __init__(self, table_id=None):
        if table_id != None:
            self.table_id = table_id
        else:
            self.table_id = 0
        return

    def pack(self):
        packed = []
        packed.append(struct.pack("!H", self.type))
        packed.append(struct.pack("!H", 0)) # placeholder for len at index 1
        packed.append(struct.pack("!B", self.table_id))
        packed.append('\x00' * 3)
        length = sum([len(x) for x in packed])
        packed[1] = struct.pack("!H", length)
        return ''.join(packed)

    @staticmethod
    def unpack(reader):
        obj = goto_table()
        _type = reader.read("!H")[0]
        assert(_type == 1)
        _len = reader.read("!H")[0]
        orig_reader = reader
        reader = orig_reader.slice(_len - (2 + 2))
        obj.table_id = reader.read("!B")[0]
        reader.skip(3)
        return obj

    def __eq__(self, other):
        if type(self) != type(other): return False
        if self.table_id != other.table_id: return False
        return True

    def pretty_print(self, q):
        q.text("goto_table {")
        with q.group():
            with q.indent(2):
                q.breakable()
                q.text("table_id = ");
                q.text("%#x" % self.table_id)
            q.breakable()
        q.text('}')

instruction.subtypes[1] = goto_table

class write_actions(instruction):
    type = 3

    def __init__(self, actions=None):
        if actions != None:
            self.actions = actions
        else:
            self.actions = []
        return

    def pack(self):
        packed = []
        packed.append(struct.pack("!H", self.type))
        packed.append(struct.pack("!H", 0)) # placeholder for len at index 1
        packed.append('\x00' * 4)
        packed.append(loxi.generic_util.pack_list(self.actions))
        length = sum([len(x) for x in packed])
        packed[1] = struct.pack("!H", length)
        return ''.join(packed)

    @staticmethod
    def unpack(reader):
        obj = write_actions()
        _type = reader.read("!H")[0]
        assert(_type == 3)
        _len = reader.read("!H")[0]
        orig_reader = reader
        reader = orig_reader.slice(_len - (2 + 2))
        reader.skip(4)
        obj.actions = loxi.generic_util.unpack_list(reader, action.action.unpack)
        return obj

    def __eq__(self, other):
        if type(self) != type(other): return False
        if self.actions != other.actions: return False
        return True

    def pretty_print(self, q):
        q.text("write_actions {")
        with q.group():
            with q.indent(2):
                q.breakable()
                q.text("actions = ");
                q.pp(self.actions)
            q.breakable()
        q.text('}')

instruction.subtypes[3] = write_actions

class write_metadata(instruction):
    type = 2

    def __init__(self, metadata=None, metadata_mask=None):
        if metadata != None:
            self.metadata = metadata
        else:
            self.metadata = 0
        if metadata_mask != None:
            self.metadata_mask = metadata_mask
        else:
            self.metadata_mask = 0
        return

    def pack(self):
        packed = []
        packed.append(struct.pack("!H", self.type))
        packed.append(struct.pack("!H", 0)) # placeholder for len at index 1
        packed.append('\x00' * 4)
        packed.append(struct.pack("!Q", self.metadata))
        packed.append(struct.pack("!Q", self.metadata_mask))
        length = sum([len(x) for x in packed])
        packed[1] = struct.pack("!H", length)
        return ''.join(packed)

    @staticmethod
    def unpack(reader):
        obj = write_metadata()
        _type = reader.read("!H")[0]
        assert(_type == 2)
        _len = reader.read("!H")[0]
        orig_reader = reader
        reader = orig_reader.slice(_len - (2 + 2))
        reader.skip(4)
        obj.metadata = reader.read("!Q")[0]
        obj.metadata_mask = reader.read("!Q")[0]
        return obj

    def __eq__(self, other):
        if type(self) != type(other): return False
        if self.metadata != other.metadata: return False
        if self.metadata_mask != other.metadata_mask: return False
        return True

    def pretty_print(self, q):
        q.text("write_metadata {")
        with q.group():
            with q.indent(2):
                q.breakable()
                q.text("metadata = ");
                q.text("%#x" % self.metadata)
                q.text(","); q.breakable()
                q.text("metadata_mask = ");
                q.text("%#x" % self.metadata_mask)
            q.breakable()
        q.text('}')

instruction.subtypes[2] = write_metadata


