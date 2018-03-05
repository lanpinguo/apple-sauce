/*********************************************************************
*
* (C) Copyright Pure Corporation 2015-2018
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************
*
* @filename     controller.c
*
* @purpose      configure controller paramaters. Uses RPC calls.
*
* @component    cli
*
* @comments
*
* @create
*
* @end
*
**********************************************************************/
#include "ofdpa_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <arpa/inet.h>

#define DEFAULT_PORT        6633

const char *argp_program_version = "controller v1.1";

/* The options we understand. */
static struct argp_option options[] =
{
  { "add",               'a',       "IP",     0, "Add a controller to switch",                   0 },
  { "port",              'p',       "PORT",   0, "socket port",                                  0 },
  { "delete",            'd',       "ID",     0, "delete a controller from switch.",             0 },
  { "list",              'l',       0,        0, "Lists the current controller configuration.",  0 },
  { 0 }
};

ofdpaAgentCxnParam_t    cxn_cfg = 
{
  .l4_port  = DEFAULT_PORT,
  .role     = ofdpaOfagentRoleClient,
};

int32_t       cxn_id = -1;


/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct in_addr ip_addr;
  uint32_t      port;
  int32_t       cxnid;

  
  switch (key)
  {
    case 'a': 
    	if(inet_pton(AF_INET, arg, &ip_addr) != 1)
      {
        argp_error(state, "Invalid ip \"%s\"", arg);
        return errno;
      }
      memcpy(cxn_cfg.controler_ip,&ip_addr,sizeof(cxn_cfg.controler_ip));
      break;

    case 'p':                           
      errno = 0;
      port = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid port \"%s\"", arg);
        return errno;
      }
    	cxn_cfg.l4_port = port;
     
      break;

    case 'd':
      errno = 0;
      cxnid = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid id \"%s\"", arg);
        return errno;
      }
    	cxn_id = cxnid;
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

void add_cxn(ofdpaAgentCxnParam_t *cxn_cfg)
{
	OFDPA_ERROR_t           rc;
  uint32_t                of_cxn_id;

	
	rc = ofdpaOfagentCxnAdd(cxn_cfg,&of_cxn_id);
	if ( rc == OFDPA_E_NONE )
	{
		printf("Add controlr successfully , cxn id :%d\r\n",of_cxn_id);
	}
}

void remove_cxn(uint32_t of_cxn_id)
{
	OFDPA_ERROR_t           rc;
	
	rc = ofdpaOfagentCxnRemove(of_cxn_id);
	if ( rc == OFDPA_E_NONE )
	{
		printf("Remove controlr successfully , cxn id :%d\r\n",of_cxn_id);
	}
}


int main(int argc, char *argv[])
{
  int i;
  int rc;
  char client_name[] = "ofdpa controller cli";
  char docBuffer[1000];
  char argsDocBuffer[300];

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

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, 0);

  if(cxn_id != -1)
  {
    remove_cxn(cxn_id);
  }
  else 
  {
    add_cxn(&cxn_cfg);
  }


  return 0;
}

