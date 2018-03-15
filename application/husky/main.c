/********************************************************************************/
/*   @@BEGAIN_INTERNAL_LEGAL@@                                          		*/
/*                                                                      		*/
/*                   Copyright(C) Description                           		*/
/* Raisecom  Science & Technology Development Co.,Ltd. Beijing, China   		*/
/*     Unpublished work-rights reserved under the China Copyright Act.  		*/
/*     Use,duplication, or disclosure by the government is subject to   		*/
/*     restrictions set forth in the CDTT commercial license agreement. 		*/
/*                                                                      		*/
/*   @@END_INTERNAL_LEGAL@@                                             		*/
/***********************************************************************		*/
/*   Filename 	:main.c                                           			    */
/*   Author    	:HuShouqiang                                         			  */
/*   Date       :2015-12-24                                           			*/
/*   Version   	:1.0                                                    		*/
/*   Purpose    :                                                       		*/
/********************************************************************************/
/********************************************************************************/

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


#define DEFAULT_PORT        6633

const char *argp_program_version = "ofagent service v1.1";

/* The options we understand. */
static struct argp_option options[] =
{
  { "add",               'a',       "IP",     0, "Add a controller to switch",                   0 },
  { "port",              'p',       "PORT",   0, "socket port",                                  0 },
  { "delete",            'd',       "ID",     0, "delete a controller from switch.",             0 },
  { "list",              'l',       0,        0, "Lists the current controller configuration.",  0 },
  { 0 }
};




/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct in_addr ip_addr;
  uint32_t      port;
  int32_t       cxnid;

  
  switch (key)
  {
    case 'a': 
      break;

    case 'p':                           
    
      break;

    case 'd':
      break;

    case 'l':
      break;

    case ARGP_KEY_ARG:
      argp_error(state, "Invalid argument\"%s\"", arg);
      //printf("argument\"%s\"",arg);
      break;

    case ARGP_KEY_NO_ARGS:
      break;

    case ARGP_KEY_END:
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
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
  int i;
  int rc;
  char client_name[] = "ofdpa controller cli";
  char docBuffer[1000];
  char argsDocBuffer[300];
  int idle_time = 0;

  /* Our argp parser. */
  struct argp argp =
  {
    .args_doc = argsDocBuffer,
    .doc      = docBuffer,
    .options  = options,
    .parser   = parse_opt,
  };


  strcpy(argsDocBuffer, "[IP] [PORT]");

  strcpy(docBuffer, "Add a controller connection to switch.\vDefault values:\n");
  i = strlen(docBuffer);
  i += sprintf(&docBuffer[i], "PORT     = %d\n", DEFAULT_PORT);
  i += sprintf(&docBuffer[i], "\n");



  

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, 0);


  
  /* Initialize ofagent State machine */
  ADPL_Init();  

  datapathInit();

	level2DebugInfoEnable();


	//ofdbInit();

  
  rpc_server_start();
  
  while(1)
  {
    sleep(2);
    //printf("\r\nidle task is running \r\n");
    idle_time++;
  }

}















