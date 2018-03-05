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

export CFLAGS += $(OFDPA_CFLAGS) -DOFAGENT_APP -DOFDPA_FIXUP


ofdpa_driver_files = $(notdir $(wildcard $(OFDPA_BASE)/ofagent/ofdpadriver/*.c))

searchdirs = $(realpath $(OFDPA_BASE)/ofagent/ofdpadriver):$(realpath $(OF_AGENT_BASE_DIR)/modules/indigo/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/loci/inc):$(realpath $(OFDPA_BASE)/ofagent/ofdpadriver/include):$(realpath $(OF_AGENT_BASE_DIR)/submodules/infra/modules/AIM/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/OFStateManager/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/submodules/loxigen-artifacts/loci/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/Configuration/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/OFConnectionManager/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/modules/OFStateManager/module/inc):$(realpath $(OF_AGENT_BASE_DIR)/submodules/bigcode/modules/BigData/BigList/module/inc):$(realpath $(OFDPA_BASE)/application/api/include)
vpath %.c $(searchdirs)

export CPATH += $(searchdirs)

.PHONY: all

$(ofagent_driver_archive): $(ofdpa_driver_files:.c=.o)
ifneq ($(dmode),)
	@echo prereqs = $+
endif
	$(RM) -f $(ofagent_driver_archive)
	$(AR) cq $(ofagent_driver_archive) $^

#
# This rule builds the dependency files
#
%.d: %.c
	@set -e; $(RM) -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$

-include $(ofdpa_driver_files:.c=.d)
