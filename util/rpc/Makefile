#
# This makefile builds the RPC libraries.  The output is the shared
# library for the client app and the linkable objects for the server
# application.
#

searchdirs = $(realpath $(OUTPATH)/rpc)$(shell find $(rpc_support_dir) -mindepth 1 -type d -printf ":%p"):$(realpath $(OFDPA_BASE)/util)$(shell find $(realpath $(OFDPA_BASE)/application) -mindepth 1 -type d -printf ":%p"):$(realpath $(OUTPATH)rpc)::$(realpath $(OFDPA_BASE)/datapath)

vpath %.o $(realpath $(OUTPATH)/rpc)
vpath %.d $(realpath $(OUTPATH)/rpc)
vpath %.c $(searchdirs)
vpath %.h $(searchdirs)

export CFLAGS += $(OFDPA_CFLAGS) -fPIC -DOFDPA_RPC
export CPATH += $(realpath $(OUTPATH)/rpc):$(searchdirs)

generated_client_code = $(notdir $(wildcard $(realpath $(OUTPATH)/rpc)/rpcclt*.c))

client_code = $(notdir $(wildcard $(rpc_support_dir)/rpcclt/*.c))

generated_server_code = $(notdir $(wildcard $(realpath $(OUTPATH)/rpc)/rpcsrv*.c))
server_code = $(notdir $(wildcard $(rpc_support_dir)/rpcsrv/*.c))

common_code = $(notdir $(wildcard $(rpc_support_dir)/rpccom/*.c))

#
# These files will already have been compiled elsewhere, but not with
# PIC which is necessary for a shared library.  Hence they get
# recompiled again here for linking into the client shared library.
#
client_non_rpc_code = $(client_only_code) ofdpa_common.c

ifeq ($(rpcgen_input_files),)
   $(error No input files for rpcgen!)
endif

#
# This must be the first (i.e., default) target.
#
.PHONY: all
all : $(rpc_server_library) $(rpc_client_library)

#
# This rule creates the server object archive library.
#
$(rpc_server_library) : $(server_code:.c=.o) $(generated_server_code:.c=.o) $(common_code:.c=.o)
	$(AR) -rcs $@ $+

#
# The client library depends upon the .o files.
# The .o files are built using implicit rules.
#
$(rpc_client_library) : $(common_code:.c=.o) $(generated_client_code:.c=.o) $(client_code:.c=.o) $(client_non_rpc_code:.c=.o)
ifneq ($(dmode),)
	@echo
	@echo CPATH = $(CPATH)
	@echo CFLAGS = $(CFLAGS)
	@echo
endif
	$(CC) $(CFLAGS) -shared -Wl,-soname,$(@F) -o $@ $+ -lc
	cd $(DELIVERABLES_DIR);ln -sf $(@F) $(basename $(basename $(basename $(@F)))).so

#
# This rule builds the dependency files
#
%.d: %.c
	@set -e; $(RM) -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$

#
# include all the generated dependency files
#
-include rpccom_support.d $(generated_client_code:.c=.d) $(client_code:.c=.d) $(server_code:.c=.d) $(generated_server_code:.c=.d) $(common_code:.c=.d) $(client_non_rpc_code:.c=.d)
