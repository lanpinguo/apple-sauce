#*********************************************************************
#
# (C) Copyright Broadcom Corporation 2013-2014
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#*********************************************************************
#   This is a script intended
#   to use by Ryu OpenFlow controller

import sys
import logging

from ryu.base import app_manager
from ryu.controller import dpset
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ofproto_v1_3

import ofdpa.mods as Mods
import ofdpa.flow_description as FlowDescriptionReader

ryu_loggers = logging.Logger.manager.loggerDict

def ryu_loggers_on(on):
    for key in ryu_loggers.keys():
        ryu_logger = logging.getLogger(key)
        ryu_logger.propagate = on          

LOG = logging.getLogger('ofdpa')

LOG.setLevel(logging.DEBUG)
#LOG.setLevel(LOGging.INFO)

fmt = logging.Formatter("[%(levelname)s]\t%(message)s")

ch = logging.StreamHandler()
ch.setLevel(logging.INFO)
ch.setFormatter(fmt)
LOG.addHandler(ch)

fh = logging.FileHandler('ofdpa_te.log', 'w')
fh.setLevel(logging.DEBUG)
fh.setFormatter(fmt)
LOG.addHandler(fh)

class OfdpaTe2(app_manager.RyuApp):
    
    _CONTEXTS = {'dpset': dpset.DPSet}
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    CONFIG_FILE = 'conf/ofdpa_te.json'

    def __init__(self, *args, **kwargs):
        super(OfdpaTe2, self).__init__(*args, **kwargs)

    @set_ev_cls(dpset.EventDP, dpset.DPSET_EV_DISPATCHER)
    def handler_datapath(self, ev):
        #ryu_loggers_on(False)
        LOG.info("===============================================================================")
        LOG.info("EventDP")
        LOG.info("Datapath Id: %i - 0x%x", ev.dp.id, ev.dp.id)
        LOG.info("===============================================================================")        
        if ev.enter:
            self.build_packets(ev.dp)

    def build_packets(self, dp):
        config_dir, working_set = FlowDescriptionReader.get_working_set(self.CONFIG_FILE)
        
        for filename in working_set:
            if filename[0] == '#':
                continue
            #ryu_loggers_on(False)
            full_filename = config_dir + '/' + filename
            LOG.info("processing file: %s",  full_filename)

            config = FlowDescriptionReader.get_config(full_filename)

            for config_type in FlowDescriptionReader.get_config_type(config):
                
                if (config_type == "flow_mod"):
                    mod_config = FlowDescriptionReader.get_flow_mod(config)
                    mod = Mods.create_flow_mod(dp, mod_config)
                    
                elif (config_type == "group_mod"):
                    mod_config = FlowDescriptionReader.get_group_mod(config)
                    mod = Mods.create_group_mod(dp, mod_config)
                    
                else:
                    raise Exception()
                    LOG.exception("Wrong configuration type name:", config_type)
                
            LOG.debug("mod length: %i", sys.getsizeof(mod))
            dp.send_msg(mod)
            LOG.info("message sent")
            LOG.info("===============================================================================")
            
            #ryu_loggers_on(True)            
            
if __name__ == '__main__':
    pass
