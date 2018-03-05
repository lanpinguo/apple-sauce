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

client_files = $(notdir $(wildcard $(OFDPA_BASE)/application/cli/*.c))
#script_files = $(wildcard $(OFDPA_BASE)/application/cli/*.py)

searchdirs = $(realpath $(rpc_support_dir)/rpccom)$(shell find $(realpath $(OFDPA_BASE)/application) -mindepth 1 -type d -printf ":%p"):$(realpath $(OFDPA_BASE)/mapping/include)

targets = $(addprefix $(DELIVERABLES_DIR)/,$(basename $(client_files)))
vpath %.c $(realpath $(OFDPA_BASE)/application/cli/)
vpath %.h $(searchdirs)

export CFLAGS += $(OFDPA_CFLAGS)
export CPATH += $(searchdirs):$(realpath $(SDK)/include)

.PHONY: all
all: $(targets)
#	cp --preserve=timestamps --force $(script_files) $(DELIVERABLES_DIR)
#	chmod u+w $(addprefix $(DELIVERABLES_DIR)/,$(notdir $(script_files)))

.SECONDEXPANSION:

$(targets) : $$(addsuffix .o,$$(notdir $$@))
	$(CC) $(CFLAGS) -o $@ -L$(DELIVERABLES_DIR) -Wl,-Bstatic $(abspath $(addprefix $(OUTPATH)/cli/,$^)) -Wl,-Bdynamic -lc -lrpc_client $(CLIENT_LDFLAGS)

#
# This rule builds the dependency files
#
%.d: %.c
	@set -e; $(RM) -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$

-include $(client_files:.c=.d)
