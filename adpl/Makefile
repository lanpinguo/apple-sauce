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
#api_files := $(filter-out ofdpa_client.c,$(notdir $(wildcard $(OFDPA_BASE)/adpl/src/*.c)))
#ofdb_files := $(notdir $(wildcard $(OFDPA_BASE)/application/ofdb/*.c))
#datapath_files := $(notdir $(wildcard $(OFDPA_BASE)/application/datapath/*.c))
#avl_files := $(notdir $(wildcard $(OFDPA_BASE)/util/avl/*.c))
#msgq_files := $(notdir $(wildcard $(OFDPA_BASE)/util/msgq/*.c))
#dot1ag_files := $(notdir $(wildcard $(OFDPA_BASE)/application/oam/dot1ag/*.c))
#g8131_files := $(notdir $(wildcard $(OFDPA_BASE)/application/oam/g8131/*.c))
api_files := $(notdir $(wildcard $(OFDPA_BASE)/adpl/src/*.c))

#core_files := $(api_files) $(ofdb_files) $(datapath_files) $(avl_files) $(msgq_files) $(dot1ag_files) $(g8131_files)
core_files := $(api_files) 

searchdirs := $(realpath $(OUTPATH)/rpc)$(shell find $(realpath $(OFDPA_BASE)/adpl) -mindepth 1 -type d -printf ":%p"):$(realpath $(OFDPA_BASE)/application/api/include):$(realpath $(OFDPA_BASE)/application/datapath):$(realpath $(OFDPA_BASE)/application/ofdb):$(realpath $(OFDPA_BASE)/adpl/inc):$(realpath $(OFDPA_BASE)/mapping/include):$(realpath $(OFDPA_BASE)/util/avl):$(realpath $(OFDPA_BASE)/util/rpc/rpcsrv/include):$(realpath $(OFDPA_BASE)/util/msgq):$(realpath $(OFDPA_BASE)/application/oam/dot1ag/include):$(realpath $(OFDPA_BASE)/application/oam/include):$(realpath $(OFDPA_BASE)/application/oam/g8131/include):$(realpath $(OFDPA_BASE)/ofagent/$(OF_AGENT)/modules/SocketManager/module/inc):$(realpath $(OFDPA_BASE)/ofagent/$(OF_AGENT)/submodules/infra/modules/AIM/module/inc):$(realpath $(OFDPA_BASE)/ofagent/$(OF_AGENT)/modules/indigo/module/inc):$(realpath $(OFDPA_BASE)/ofagent/$(OF_AGENT)/submodules/loxigen-artifacts/loci/inc):$(realpath $(OFDPA_BASE)/ofagent/$(OF_AGENT)/modules/Configuration/module/inc):$(realpath $(OFDPA_BASE)/ofagent/$(OF_AGENT)/submodules/bigcode/modules/BigData/BigList/module/inc):$(realpath $(OFDPA_BASE)/ofagent/$(OF_AGENT)/modules/OFConnectionManager/module/inc):$(realpath $(OFDPA_BASE)/ofagent/$(OF_AGENT)/modules/OFStateManager/module/inc):$(realpath $(OFDPA_BASE)/ofagent/ofdpadriver/include):$(realpath $(OFDPA_BASE)/util/sal/linux)

vpath %.c $(searchdirs)
vpath %.h $(searchdirs)

export CFLAGS += $(OFDPA_CFLAGS)
export CPATH += $(searchdirs):$(realpath $(OFDPA_BASE)/systems/platform)

$(of_adpl_archive): $(core_files:.c=.o)
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

pclint_inc.lnt:
	@echo $(subst :, ,$(subst /,\\,$(subst $(ROOT_DIR),-i$(PCLINT_DIR),$(patsubst -I%,-i%,$(CPATH))))) >$(OFDPA_BASE)/adpl/$@
pclint_flag.lnt:
	@echo $(patsubst -D%,-d%,$(filter -D%,$(CFLAGS))) >$(OFDPA_BASE)/adpl/$@