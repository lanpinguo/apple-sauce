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
api_files := $(filter-out ofdpa_client.c,$(notdir $(wildcard $(OFDPA_BASE)/application/api/*.c)))
ofdb_files := $(notdir $(wildcard $(OFDPA_BASE)/application/ofdb/*.c))
avl_files := $(notdir $(wildcard $(OFDPA_BASE)/util/avl/*.c))
msgq_files := $(notdir $(wildcard $(OFDPA_BASE)/util/msgq/*.c))
dot1ag_files := $(notdir $(wildcard $(OFDPA_BASE)/application/oam/dot1ag/*.c))
g8131_files := $(notdir $(wildcard $(OFDPA_BASE)/application/oam/g8131/*.c))
sal_files := $(notdir $(wildcard $(OFDPA_BASE)/util/sal/linux/*.c))
util_files := $(notdir $(wildcard $(OFDPA_BASE)/util/*.c))

core_files := $(api_files) $(ofdb_files) $(avl_files) $(msgq_files) $(dot1ag_files) $(g8131_files) $(sal_files) $(util_files)

searchdirs := $(realpath $(OUTPATH)/rpc)$(shell find $(realpath $(OFDPA_BASE)/application) -mindepth 1 -type d -printf ":%p"):$(realpath $(OFDPA_BASE)/datapath):$(realpath $(OFDPA_BASE)/util/avl):$(realpath $(OFDPA_BASE)/util/rpc/rpcsrv/include):$(realpath $(OFDPA_BASE)/util/msgq):$(realpath $(OFDPA_BASE)/application/oam/dot1ag/include):$(realpath $(OFDPA_BASE)/application/oam/include):$(realpath $(OFDPA_BASE)/application/oam/g8131/include:$(realpath $(OFDPA_BASE)/application/oam/redundancy/include)):$(realpath $(OFDPA_BASE)/util/sal/linux):$(realpath $(OFDPA_BASE)/adpl/inc):$(realpath $(OFDPA_BASE)/util):$(realpath $(OFDPA_BASE)/util/netmap/usr/include)

vpath %.c $(searchdirs)
vpath %.h $(searchdirs)

export CFLAGS += $(OFDPA_CFLAGS)
export CPATH += $(searchdirs):$(realpath $(OFDPA_BASE)/systems/platform)

$(core_archive): $(core_files:.c=.o)
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

