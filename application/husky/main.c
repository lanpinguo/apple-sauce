/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : main.c
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/5/17
  Last Modified :
  Description   : app main entry
  Function List :
              main
              print_version
              rpc_server_start
  History       :
  1.Date        : 2018/5/17
    Author      : lanpinguo
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include <libgen.h>
#include <signal.h>
#include <linux/kernel.h>
#include <arpa/inet.h>
#include <argp.h>
#include <regex.h>
#include <pthread.h>
#include "ofdpa_util.h"

#include "ofdpa_api.h"
#include <SocketManager/socketmanager.h>
#include <AIM/aim.h>
#include <AIM/aim_pvs_syslog.h>
#include <BigList/biglist.h>
#include <indigo/port_manager.h>
#include <indigo/types.h>
#include <indigo/of_state_manager.h>
#include <SocketManager/socketmanager.h>
#include <OFConnectionManager/ofconnectionmanager.h>
#include <OFStateManager/ofstatemanager.h>
#include <indigo/forwarding.h>
#include <ind_ofdpa_util.h>
#include <AIM/aim_log.h>
#include "of_adpl_api.h"
#include "sal_thread.h"
#include "rpcclt_api.h"
#include "rpc_sockmap.h"
#include "rpcsrv_task.h"


#include <readline/readline.h>
#include <readline/history.h>
#include "commands.h"
#include "readinput.h"

void level2DebugInfoEnable(void);

#define DEFAULT_PORT        6633

#define VERSION							"1.0"


#define PROMPT "Pure> "

volatile int done;
extern int multiline;
extern char* last_tmpfile;
extern COMMAND commands[];




void print_version() {
	fprintf(stdout, "ofagent service, version %s\n", VERSION);
	//fprintf(stdout, "%s\n", RCSID);
	fprintf(stdout, "compile time: %s, %s\n", __DATE__, __TIME__);
}




int rpc_server_start(void)
{
  void *threadPtr;

  
  printf("\r\nStarting rpcsrvTask.\r\n");
  threadPtr = sal_thread_create("rpcsrvTask",SAL_THREAD_STKSZ, 60, rpcsrvTask, NULL);
  if (NULL == threadPtr)
  {
    printf("Failed starting rpcsrvTask.\r\n");
    return OFDPA_E_FAIL;
  }

  return OFDPA_E_NONE;
}



int datapathInit(void);



int main(int argc, char *argv[])
{
  int rc;
  //int idle_time = 0;
	HIST_ENTRY* hent;
	char* cmd, *cmdline, *cmdstart;
	int i, j;


 
  /* Initialize ofagent State machine */
  ADPL_Init();  

  datapathInit();

	level2DebugInfoEnable();

 
  rpc_server_start();
  
  while(1)
  {
    //sleep(2);
    //printf("\r\nidle task is running \r\n");
    //idle_time++;

		/* get the command from user */
		cmdline = readline(PROMPT);
	
		/* EOF -> exit */
		if (cmdline == NULL) {
			done = 1;
			cmdline = strdup ("quit");
		}
	
		/* empty line -> wait for another command */
		if (*cmdline == 0) {
			free(cmdline);
			continue;
		}
	
		/* Isolate the command word. */
		for (i = 0; cmdline[i] && whitespace (cmdline[i]); i++);
		cmdstart = cmdline + i;
		for (j = 0; cmdline[i] && !whitespace (cmdline[i]); i++, j++);
		cmd = strndup(cmdstart, j);
	
		/* parse the command line */
		for (i = 0; commands[i].name; i++) {
			if (strcmp(cmd, commands[i].name) == 0) {
				break;
			}
		}
	
		/* execute the command if any valid specified */
		if (commands[i].name) {
			if (where_history() < history_length) {
				hent = history_get(where_history()+1);
				if (hent == NULL) {
					ERROR("main", "Internal error (%s:%d).", __FILE__, __LINE__);
					return EXIT_FAILURE;
				}
				commands[i].func((const char*)cmdstart, hent->timestamp, stdout, stdin);
			} else {
				commands[i].func((const char*)cmdstart, NULL, stdout, stdin);
			}
		} else {
			/* if unknown command specified, tell it to user */
			fprintf(stdout, "%s: no such command, type 'help' for more information.\n", cmd);
		}
	
		hent = history_get(history_length);
		/* whether to save the last command */
		if (hent == NULL || strcmp(hent->line, cmdline) != 0) {
			add_history(cmdline);
			hent = history_get(history_length);
			if (hent == NULL) {
				ERROR("main", "Internal error (%s:%d).", __FILE__, __LINE__);
				return EXIT_FAILURE;
			}
			if (last_tmpfile != NULL) {
				free(hent->timestamp);
				hent->timestamp = strdup(last_tmpfile);
			}
	
		/* whether to at least replace the tmpfile of the command from the history with this new one */
		} else if (last_tmpfile != NULL && (hent = current_history()) != NULL && strlen(hent->timestamp) != 0) {
			free(hent->timestamp);
			hent->timestamp = strdup(last_tmpfile);
		}
	
		free(last_tmpfile);
		last_tmpfile = NULL;
		free(cmdline);
		free(cmd);
	}
	
	store_config();
	
	clear_history();
	free(history_list());
	
	/* cannot call, causes invalid free (seriously readline?), but would free ~650 kb */
	//rl_discard_keymap(rl_get_keymap_by_name("emacs"));
	rl_discard_keymap(rl_get_keymap_by_name("vi-insert"));
	
	rl_expand_prompt(NULL);
	rl_free(rl_prompt);
	
}















