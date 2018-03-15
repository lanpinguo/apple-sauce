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
* @filename     client_grouptable_dump.c
*
* @purpose      Print entries in the group table.
*
* @component    OF-DPA
*
* @comments
*
* @create
*
* @end
*
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <argp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <libgen.h>

#include "ofdpa_api.h"
#include "ofdpa_datatypes.h"

#define VERSION              1.0

typedef struct
{
  int count;
  OFDPA_GROUP_ENTRY_TYPE_t groupType;
  int groupTypeOptionGiven;

} arguments_t;

/* The options we understand. */
static struct argp_option options[] =
{
  { "count", 'c', "COUNT",     0, "Number of entries to display. If not specified or set to 0, all entries displayed.", 0 },
  { "type",  't', "GROUPTYPE", 0, "Group entry type to list. If not specified, all types displayed.",                   0 },
  { 0 }
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  /* Get the INPUT argument from `argp_parse', which we
     know is a pointer to our arguments structure. */
  arguments_t *arguments = state->input;

  switch (key)
  {
    case 'c':                           /* count */
      errno = 0;
      arguments->count = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid count \"%s\"", arg);
        return errno;
      }
      break;

    case 't':
      errno = 0;
      arguments->groupType = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid group type \"%s\"", arg);
        return errno;
      }
      arguments->groupTypeOptionGiven = 1;
      break;

    case ARGP_KEY_ARG:
    case ARGP_KEY_NO_ARGS:
    case ARGP_KEY_END:
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

void groupTableList(OFDPA_GROUP_ENTRY_TYPE_t groupType, int groupTypeSpecified, int count)
{
  OFDPA_ERROR_t rc;
  ofdpaGroupEntry_t groupEntry;
  ofdpaGroupEntryStats_t groupStats;
  ofdpaGroupBucketEntry_t bucketEntry;
  OFDPA_GROUP_ENTRY_TYPE_t currentEntryType;
  ofdpaPrettyPrintBuf_t buf;
  int entriesDisplayedCount = 0;

  memset(&groupEntry, 0, sizeof(groupEntry));

  ofdpaGroupTypeSet(&groupEntry.groupId, groupType);

  /* it is possible that the groupId is an exact match for an entry */
  if ((rc = ofdpaGroupStatsGet(groupEntry.groupId, &groupStats)) != OFDPA_E_NONE)
  {
    rc = ofdpaGroupNextGet(groupEntry.groupId, &groupEntry);
  }

  /* found at least one entry, proceed */
  if (OFDPA_E_NONE == rc)
  {
    do
    {
      /* if groupType specified, only iterate of group entries of that type */
      if (groupTypeSpecified != 0)
      {
        if ((OFDPA_E_NONE != ofdpaGroupTypeGet(groupEntry.groupId, &currentEntryType)) ||
            (groupType != currentEntryType))
        {
          /* retrieved group entry not of requested type, done */
          break;
        }
      }

      /* group entry matches criteria, display it */
      ofdpaGroupDecode(groupEntry.groupId, buf.data, sizeof(buf.data));
      printf("\r\ngroupId = 0x%08x (%s): ", groupEntry.groupId, buf.data);

      rc = ofdpaGroupStatsGet(groupEntry.groupId, &groupStats);
      if (rc != OFDPA_E_NONE)
      {
        printf("Error retrieving group entry stats. (rc = %d)\r\n", rc);
      }
      else
      {
        printf("duration: %d, refCount:%d\r\n", groupStats.duration, groupStats.refCount);
      }

      memset(&bucketEntry, 0, sizeof(bucketEntry));

      if (ofdpaGroupBucketEntryFirstGet(groupEntry.groupId, &bucketEntry) == OFDPA_E_NONE)
      {
        do
        {
          printf("\t");
          rc = ofdpaGroupBucketEntryPrint(&bucketEntry, &buf);

          if (rc == OFDPA_E_NONE)
          {
            printf("%s", buf.data);
          }
          else if (rc == OFDPA_E_FULL)
          {
            printf("%s (** TRUNCATED OUTPUT **)", buf.data);
          }
          else
          {
            printf("Error decoding bucketEntry");
          }

          printf("\r\n");
        } while (ofdpaGroupBucketEntryNextGet(bucketEntry.groupId, bucketEntry.bucketIndex, &bucketEntry) == OFDPA_E_NONE);
      }

      entriesDisplayedCount++;
      if ((count != 0) && (entriesDisplayedCount >= count))
      {
        break;
      }

    } while (ofdpaGroupNextGet(groupEntry.groupId, &groupEntry) == OFDPA_E_NONE);
  }

  if (entriesDisplayedCount == 0)
  {
    printf("No entries found.\r\n");
  }

  return;
}

int main(int argc, char *argv[])
{
  OFDPA_ERROR_t  rc;
  char           client_name[20] = "group_table_dump";
  char           docBuffer[300];
  char           versionBuf[100];
  arguments_t    arguments;

  memset(&arguments, 0, sizeof(arguments));

  /* Our argp parser. */
  struct argp argp =
  {
    .doc     = docBuffer,
    .options = options,
    .parser  = parse_opt,
  };

  sprintf(versionBuf, "%s v%.1f", basename(strdup(__FILE__)), VERSION);
  argp_program_version = versionBuf;

  strcpy(docBuffer, "\nLists group entries.\n");

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  groupTableList(arguments.groupType, arguments.groupTypeOptionGiven, arguments.count);

  return rc;
}
