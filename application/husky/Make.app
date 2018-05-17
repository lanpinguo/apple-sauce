# -*- mode: makefile-gmake; -*-
#/********************************************************************************/
#/*   @@BEGAIN_INTERNAL_LEGAL@@                                          		*/
#/*                                                                      		*/
#/*                   Copyright(C) Description                           		*/
#/* Raisecom  Science & Technology Development Co.,Ltd. Beijing, China   		*/
#/*     Unpublished work-rights reserved under the China Copyright Act.  		*/
#/*     Use,duplication, or disclosure by the government is subject to   		*/
#/*     restrictions set forth in the CDTT commercial license agreement. 		*/
#/*                                                                      		*/
#/*   @@END_INTERNAL_LEGAL@@                                             		*/
#/***********************************************************************		*/
#/*   Author    	：HuShouqiang                                         			*/
#/*   Date      	：2015-12-25                                           			*/
#/*   Version   	：1.0                                                    		*/
#/*   Purpose    ：                                                       		*/
#/********************************************************************************/
#/********************************************************************************/*********************************************************************

# This makefile will build the integrated ofagentapp and ofdpa
# application.  There are two files created as a result of this
# Makefile: ofagentapp and ofagentapp.dbg.  The first has the debug symbols
# stripped out and the second includes all the debug symbols.  There
# is a large difference in file size making it much quicker to load
# the stripped version on the switch.
#


export ofagentapp_executable = $(abspath $(OUTPATH)/husky)

archives = $(wildcard $(realpath $(LIBRARIES_DIR))/*.a)
archives += $(wildcard $(realpath $(OF_AGENT_OUTPUT_DIR))/ofagent.a)
archives += $(wildcard $(realpath $(OF_AGENT_OUTPUT_DIR))/ofdpadriver.a)
archives += $(wildcard $(realpath $(OUTPATH))/rpc/rpc_server.a)

export CFLAGS += $(OFDPA_CFLAGS) -DOFAGENT_APP -DOFDPA_FIXUP

version_file = version.o

#version = $(L7_REL_NUM).$(L7_VER_NUM).$(L7_MAINT_NUM).$(L7_BUILD_NUM)
version = 1.0.0.0

#
# This must be the first target (the default).
#
$(ofagentapp_executable): %: %.dbg
	$(CROSS_COMPILE)objcopy --strip-unneeded $< $@



##$(ofagentapp_executable).dbg: $(ofagentapp_archive) $(ofagent_driver_archive) $(ofagent_archive) $(realpath $(LIBRARIES_DIR)/$(client_only_code:.c=.o)) $(archives) $(rpc_server_library)
$(ofagentapp_executable).dbg: $(application_archive) $(ofagent_driver_archive) $(ofagent_archive) $(archives)
	@echo const char *versionBuf = \"version ${version} -- Built on `date '+%a %b %e %Y at %H:%M:%S %Z'`\"\; > $(version_file:.o=.c)
	$(CC) $(CFLAGS) -c -o $(version_file) $(version_file:.o=.c)
	$(CC) $(CFLAGS) -o $@ \
		$(OFDPA_LDFLAGS) -Wl,--relax -Wl,--start-group $^ $(version_file) -Wl,--end-group \
		-lrt -lm -I$(SDK)/include -Wl,-Bstatic $(OSLIBS_STATIC) \
		-Wl,-Bdynamic -lc $(OSLIBS) -lreadline -lcurses





