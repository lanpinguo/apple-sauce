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
import json

LOG = logging.getLogger('ofdpa')

'''
Read predefined main configuration file
and extract from it and parse:
Working set - list of flows/groups in order of it setup
Configuration files - descriptions of separate flow or group
All of files in the JSON format.
All of values are strings.    
'''

def get_working_set(filename):
    main_config_file = open(filename)
    main_config = json.load(main_config_file)
    LOG.debug('main_config: %s', main_config)
    main_config_file.close()

    working_set_name = main_config['config_directory']
    working_set_name += '/'
    working_set_name += main_config['working_set']

    LOG.info('working_set_name: %s', working_set_name)
    working_set_file = open(working_set_name)
    working_set = json.load(working_set_file)
    working_set_file.close()
    LOG.debug('working_set: %s', working_set)
    LOG.info("===============================================================================")
    return main_config['config_directory'], working_set['working_set']

def get_config(filename):
    config_file = open(filename)
    config = json.load(config_file)
    config_file.close()
    return config

def get_config_type(config):
    return config.keys()

def get_flow_mod(config):
    return config['flow_mod']

def get_group_mod(config):
    return config['group_mod']

def get_matches(config):
    return config['match']

def get_instr_config(config):
    return config['instructions']

def get_buckets_config(config):
    return config['buckets']
