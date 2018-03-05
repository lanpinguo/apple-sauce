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
Implements Buckets object
'''

def create_buckets(dp, config):
    LOG.debug('BUCKETS:')
    buckets = []
    '''
    Buckets config is a list
    '''
    LOG.debug('buckets config: %s', config)
    for entry in config:
        LOG.debug('entry: %s ', entry)
        
        actions = Actions.create_actions(dp, entry['actions'])

        bucket = dp.ofproto_parser.OFPBucket(
            weight = int(entry['weight'], 0),
            watch_port = Utils.get_mod_port(dp,entry['watch_port']),
            watch_group = Utils.get_mod_group(dp,entry['watch_group']),
            actions = actions
        ) 
        LOG.debug('bucket: %s', bucket)
        buckets.append(bucket)
        LOG.debug('buckets: %s', buckets)
    return buckets
