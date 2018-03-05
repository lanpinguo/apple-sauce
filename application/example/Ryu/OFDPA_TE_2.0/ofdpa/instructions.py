#*********************************************************************
#
# (C) Copyright Broadcom Corporation 2013-2014
#
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#*********************************************************************


import logging

import ofdpa.actions as Actions
import ofdpa.utils as Utils

LOG = logging.getLogger('ofdpa')

'''
Implements Instructions object
'''

def create_instructions(dp, config):
    LOG.debug('INSTRUCTIONS:')
    instructions = []
    '''
    Instruction config 
    is a list of dictionaries
    '''
    LOG.debug('instructions config: %s', config)
    
    for instruction in config:
        for key in instruction.keys():
            if key[0] == '#':
                continue
            val = instruction[key]
            LOG.debug('key: %s', key)
            LOG.debug('val: %s', val)

            if (key  == 'apply'):
                instructions.append(process_apply(dp,val))
            elif (key  == 'write'):
                instructions.append(process_write(dp,val))
            elif (key  == 'goto'):
                instructions.append(process_goto(dp,val))
            elif (key  == 'meter'):
                instructions.append(process_meter(dp,val))
            elif (key  == 'metadata'):
                instructions.append(process_metadata(dp,val))
            elif (key  == 'clear'):
                instructions.append(process_clear(dp,val))
            else:
                raise Exception('Wrong instruction name:', key)

    LOG.debug( 'instructions: %s', instructions)
    return instructions

def process_apply(dp,config):
    act_type = dp.ofproto.OFPIT_APPLY_ACTIONS
    for entry in config:
        actions = Actions.create_actions(dp, entry['actions'])
    return dp.ofproto_parser.OFPInstructionActions(act_type, actions)

def process_write(dp,config):
    act_type = dp.ofproto.OFPIT_WRITE_ACTIONS
    for entry in config:
        actions = Actions.create_actions(dp, entry['actions'])
    return dp.ofproto_parser.OFPInstructionActions(act_type,  actions)

def process_clear(dp,config):
    act_type = dp.ofproto.OFPIT_CLEAR_ACTIONS
    for entry in config:
        actions = Actions.create_actions(dp, entry['actions'])
    return dp.ofproto_parser.OFPInstructionActions(act_type, actions)

def process_goto(dp,config):
    table = Utils.get_table(config['table'])
    return dp.ofproto_parser.OFPInstructionGotoTable(table)

def process_meter(dp,config):
    meter_id = int(config['meter_id'])
    return dp.ofproto_parser.OFPInstructionMeter(meter_id)

def process_metadata(dp,config):
    metadata = int(config['metadata'])
    mask = int(config['mask'])
    return dp.ofproto_parser.OFPInstructionWriteMetadata(metadata,mask)

