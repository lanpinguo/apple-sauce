/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
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
* @filename     flowtable_dump.c
*
* @purpose      Dump flow tables. Uses RPC calls.
*
* @component    Unit Test
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

#define DEFAULT_COUNT        0

const char *argp_program_version = "flowtable_dump v1.1";

/* The options we understand. */
static struct argp_option options[] =
{
  { "count",               'c', "COUNT",     0, "Number of entries from start of table. (0 for all)",       0 },
  { "verbose",             'v',       0,     0, "Print stats for empty flow tables.",                       0 },
  { "list",                'l',       0,     0, "Lists table IDs for supported flow tables and exits.",     0 },
  { 0 }
};

static int count;
static OFDPA_FLOW_TABLE_ID_t tableId;
static int tableIdSpecified = 0;
static int showEmptyTables = 0;
static int showValidTableIds = 0;

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  OFDPA_ERROR_t rc;

  switch (key)
  {
    case 'c':                           /* count */
      errno = 0;
      count = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid count \"%s\"", arg);
        return errno;
      }
      break;

    case 'l':
      showValidTableIds = 1;
      break;

    case 'v':
      showEmptyTables = 1;
      break;

    case ARGP_KEY_ARG:
      errno = 0;
      tableId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid table ID argument\"%s\"", arg);
        return errno;
      }
      rc = ofdpaFlowTableSupported(tableId);
      if (rc != OFDPA_E_NONE)
      {
        argp_error(state, "Unsupported table ID (%d) (ofdpaFlowTableSupported() returns %d)", tableId, rc);
        return ENOTSUP;
      }

      tableIdSpecified = 1;
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

void dumpFlowTable(OFDPA_FLOW_TABLE_ID_t tableId, int entryPrintLimit, int *entriesPrinted)
{
  int i;
  OFDPA_ERROR_t rc;
  char buffer[700];
  ofdpaFlowTableInfo_t info;
  ofdpaFlowEntry_t flow;
  ofdpaFlowEntryStats_t flowStats;

  memset(&info, 0, sizeof(info));
  rc = ofdpaFlowTableInfoGet(tableId, &info);

  if (!showEmptyTables &&
      (info.numEntries == 0))
  {
    return;
  }

  printf("Table ID %d (%s): ", tableId, ofdpaFlowTableNameGet(tableId));

  if (entryPrintLimit == 0)
  {
    sprintf(buffer, "all entries");
  }
  else if (entryPrintLimit == 1)
  {
    sprintf(buffer, "up to 1 entry");
  }
  else
  {
    sprintf(buffer, "up to %d entries", entryPrintLimit);
  }

  printf("  Retrieving %s. ", buffer);

  if (rc != OFDPA_E_NONE)
  {
    printf("Could not retrieve OF-DPA table info with ID %d. (rc = %d)\r\n", tableId, rc);
  }
  else
  {
    printf("Max entries = %d, Current entries = %d.\r\n", info.maxEntries, info.numEntries);
  }

  rc = ofdpaFlowEntryInit(tableId, &flow);
  if (rc != OFDPA_E_NONE)
  {
    printf("Bad return code trying to initialize flow. rc = %d.\r\n", rc);
    return;
  }

  rc = ofdpaFlowStatsGet(&flow, &flowStats);
  if (rc != OFDPA_E_NONE)
  {
    rc = ofdpaFlowNextGet(&flow, &flow);
  }

  i = 0;

  while ((rc == OFDPA_E_NONE) &&
         ((entryPrintLimit == 0) || (i < entryPrintLimit)))
  {
    rc = ofdpaFlowEntryDecode(&flow, buffer, sizeof(buffer));
		
    printf("-- %s%s\r\n", buffer, (rc == OFDPA_E_FULL) ? " -- OUTPUT TRUNCATED! --":"");

		memset(&flowStats, 0, sizeof(flowStats));
		rc = ofdpaFlowStatsGet(&flow, &flowStats);
		if (rc == OFDPA_E_NONE)	{
			printf("##Stats:durationSec = %d ,revBytes = 0x%llx,revPkt = 0x%llx\r\n\r\n",
				flowStats.durationSec,flowStats.receivedBytes,flowStats.receivedPackets);
		}
		else {
			printf("##Stats:durationSec = ## ,revBytes = ##,revPkt = ##\r\n\r\n");
		}
    i++;

    rc = ofdpaFlowNextGet(&flow, &flow);
  }

  /* blank line between tables */
  printf("\r\n");
  *entriesPrinted = i;
}

int main(int argc, char *argv[])
{
  int i, j, entriesPrinted, totalPrinted, remainingCount;
  int rc;
  char client_name[] = "ofdpa flowtable_dump client";
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

  count = DEFAULT_COUNT;

  strcpy(argsDocBuffer, "[table_ID]");

  strcpy(docBuffer, "Prints entries in the OF-DPA flow tables. Specify table ID to print content of a single table. "
         "If no argument given, content of all tables are printed.\vDefault values:\n");
  i = strlen(docBuffer);
  i += sprintf(&docBuffer[i], "COUNT     = %d\n", DEFAULT_COUNT);
  i += sprintf(&docBuffer[i], "\n");

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, 0);

  if (showValidTableIds)
  {
    printf("Valid flow table IDs:\r\n");
    for (j = 0; j < 256; j++)
    {
      if (ofdpaFlowTableSupported(j) == OFDPA_E_NONE)
      {
        printf("  %d - %s\r\n", j, ofdpaFlowTableNameGet(j));
      }
    }
    printf("\r\n");
    return 0;
  }

  totalPrinted = 0;

  if (tableIdSpecified)
  {
    dumpFlowTable(tableId, count, &totalPrinted);
  }
  else
  {
    remainingCount = count;

    for (j = 0; j < 256; j++)
    {
      if (ofdpaFlowTableSupported(j) == OFDPA_E_NONE)
      {
        entriesPrinted = 0;

        dumpFlowTable(j, remainingCount, &entriesPrinted);

        totalPrinted += entriesPrinted;

        if (count != 0)
        {
          if (remainingCount > entriesPrinted)
          {
            remainingCount -= entriesPrinted;
          }
          else
          {
            /* printed the requested number of total entries */
            break;
          }
        }
      }
    }
  }

  /* 
   * if not printing empty table stats and no flow entries found, we haven't printed anything at all
   * so print a message letting the user know we are responsive
   */
  if ((showEmptyTables == 0) && (totalPrinted == 0))
  {
    printf("No flow entries found.\r\n");
  }
  return 0;
}
