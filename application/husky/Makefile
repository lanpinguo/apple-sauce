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
#
# Do not include ofdpa_client.c.  That file is for the shared client library only
#
source_files := $(notdir $(wildcard $(OFDPA_BASE)/application/husky/*.c))


core_files := $(source_files) 

searchdirs := $(realpath $(OUTPATH)/rpc)$(shell find $(realpath $(OFDPA_BASE)/application) -mindepth 1 -type d -printf ":%p"):$(realpath $(OFDPA_BASE)/datapath):$(realpath $(OFDPA_BASE)/util/avl):$(realpath $(OFDPA_BASE)/util/rpc/rpcsrv/include):$(realpath $(OFDPA_BASE)/util/msgq):$(realpath $(OFDPA_BASE)/application/oam/dot1ag/include):$(realpath $(OFDPA_BASE)/application/oam/include):$(realpath $(OFDPA_BASE)/application/oam/g8131/include:$(realpath $(OFDPA_BASE)/application/oam/redundancy/include)):$(realpath $(OFDPA_BASE)/util/sal/linux):$(realpath $(OFDPA_BASE)/adpl/inc):$(realpath $(OFDPA_BASE)/util):$(realpath $(OFDPA_BASE)/util/netmap/usr/include):$(realpath $(OFDPA_BASE)/util/rpc/rpcsrv/include):$(realpath $(OFDPA_BASE)/util/rpc/rpccom):$(realpath $(OF_AGENT_BASE_DIR)/modules/SocketManager/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/submodules/infra/modules/AIM/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/indigo/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/submodules/loxigen-artifacts/loci/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/Configuration/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/OFConnectionManager/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/OFStateManager/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/submodules/bigcode/modules/BigData/BigList/module/inc):$(realpath $(OFDPA_BASE)/application/api/include):$(realpath $(OFDPA_BASE)/ofagent/ofdpadriver/include)

vpath %.c $(searchdirs)
vpath %.h $(searchdirs)

export CFLAGS += $(OFDPA_CFLAGS)
export CPATH += $(searchdirs):$(realpath $(OFDPA_BASE)/systems/platform)

$(application_archive): $(core_files:.c=.o)
ifneq ($(dmode),)
	@echo prereqs = $+
endif
	$(RM) -f $@
	$(AR) cq $@ $^

#
# This rule builds the dependency files
#
%.d: %.c
	@set -e; $(RM) -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$

-include $(core_files:.c=.d)

