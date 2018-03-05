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
* @filename     client_port_table_dump.c
*
* @purpose      Display port configuration and status.
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
#include <inttypes.h>
#include <argp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const char *argp_program_version = "client_port_table_dump v1.1";

/* The options we understand. */
static struct argp_option options[] =
{
  { "physical",            'p',       0,     0, "Print physical port entries.",                      0 },
  { "tunnel",              't',       0,     0, "Print overlay logical port entries.",               0 },
  { "oam_protect",         'o',       0,     0, "Print OAM liveness logical port entries.",          0 },
  { "stats",               's',       0,     0, "Print statistics for port entries.",                0 },
  { "queue_stats",         'q',       0,     0, "Print queue statistics for physical port entries.", 0 },
  { 0 }
};

static int showAll = 1;
static int showSinglePort = 0;
static int showPhysical = 0;
static int showTunnel = 0;
static int showOam = 0;
static int showStats = 0;
static int showQueues = 0;
static uint32_t singlePortId;

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  uint32_t portState;

  switch (key)
  {
    case 'p':
      showPhysical = 1;
      showAll = 0;
      break;

    case 't':
      showTunnel = 1;
      showAll = 0;
      break;

    case 'o':
      showOam = 1;
      showAll = 0;
      break;

    case 's':
      showStats = 1;
      break;

    case 'q':
      showQueues = 1;
      break;

    case ARGP_KEY_ARG:
      errno = 0;
      singlePortId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid portNum argument\"%s\"", arg);
        return errno;
      }
      if (ofdpaPortStateGet(singlePortId, &portState) != OFDPA_E_NONE)
      {
        argp_error(state, "Specified port does not exist (%d, 0x%x)", singlePortId, singlePortId);
        return ENOTSUP;
      }

      showSinglePort = 1;

      break;
    case ARGP_KEY_NO_ARGS:
    case ARGP_KEY_END:
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

struct
{
  uint32_t type;
  char    *text;
} portTypeList[] =
{
  {OFDPA_PORT_TYPE_PHYSICAL,                             "physical"},
  {OFDPA_PORT_TYPE_LOGICAL_TUNNEL,                       "tunnel"},
  {OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT, "protection liveness"},
};
#define PORT_TYPE_LIST_SIZE (sizeof(portTypeList)/sizeof(portTypeList[0]))

char *getPortTypeText(uint32_t portType)
{
  int i;

  for (i=0; i < PORT_TYPE_LIST_SIZE; i++)
  {
    if (portTypeList[i].type == portType)
    {
      return(portTypeList[i].text);
    }
  }
  return("Unknown");
}

void displayPortNumName(uint32_t portNum)
{
  uint32_t portIndex, portType;
  ofdpa_buffdesc portName;
  char portNameBuffer[OFDPA_PORT_NAME_STRING_SIZE];

  memset(portNameBuffer, 0, sizeof(portNameBuffer));
  portName.size = sizeof(portNameBuffer);
  portName.pstart = portNameBuffer;

  ofdpaPortTypeGet(portNum, &portType);
  ofdpaPortIndexGet(portNum, &portIndex);
  ofdpaPortNameGet(portNum, &portName);

  printf("0x%08x (type: %s (0x%x), index = %d) | %s:",
         portNum, getPortTypeText(portType),portType, portIndex, portNameBuffer);
}

void displayPhysicalPort(uint32_t portNum)
{
  OFDPA_ERROR_t rc;
  ofdpaMacAddr_t mac;
  OFDPA_PORT_STATE_t state = 0;
  OFDPA_PORT_CONFIG_t config = 0;
  uint32_t maxSpeed = 0;
  uint32_t currentSpeed = 0;
  ofdpaPortFeature_t feature;
  ofdpaPortStats_t stats;
  uint32_t numQueues;
  uint32_t queueId;
  ofdpaPortQueueStats_t qStats;

  memset(&feature, 0, sizeof(feature));
  memset(&stats, 0, sizeof(stats));

  ofdpaPortConfigGet(portNum, &config);
  ofdpaPortStateGet(portNum, &state);
  ofdpaPortMacGet(portNum, &mac);
  ofdpaPortMaxSpeedGet(portNum, &maxSpeed);
  ofdpaPortCurrSpeedGet(portNum, &currentSpeed);
  ofdpaPortFeatureGet(portNum, &feature);

  displayPortNumName(portNum);

  printf("\r\n\t");
  printf("config = 0x%08x, ", config);
  printf("state = 0x%08x, ", state);
  printf("mac = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x, ",
         mac.addr[0], mac.addr[1], mac.addr[2], mac.addr[3], mac.addr[4], mac.addr[5]);
  printf("max. speed = %d kbps, ", maxSpeed);
  printf("current speed = %d kbps", currentSpeed);
  printf("\r\n\t");
  printf("CurrFeature: 0x%x, ", feature.curr);
  printf("AdvertFeature: 0x%x, ", feature.advertised);
  printf("SupportedFeature: 0x%x, ", feature.supported);
  printf("PeerFeature: 0x%x", feature.peer);

  if (showStats)
  {
    rc = ofdpaPortStatsGet(portNum, &stats);

    if (rc == OFDPA_E_NONE)
    {
      /* handle the way OF-DPA indicates unsupported counters */
      if (stats.rx_packets == ~0)
        stats.rx_packets = 0;

      if (stats.tx_packets == ~0)
        stats.tx_packets = 0;

      if (stats.rx_bytes == ~0)
        stats.rx_bytes = 0;

      if (stats.tx_bytes == ~0)
        stats.tx_bytes = 0;

      if (stats.rx_errors == ~0)
        stats.rx_errors = 0;

      if (stats.tx_errors == ~0)
        stats.tx_errors = 0;

      if (stats.rx_drops == ~0)
        stats.rx_drops = 0;

      if (stats.tx_drops == ~0)
        stats.tx_drops = 0;

      if (stats.rx_frame_err == ~0)
        stats.rx_frame_err = 0;

      if (stats.rx_over_err == ~0)
        stats.rx_over_err = 0;

      if (stats.rx_crc_err == ~0)
        stats.rx_crc_err = 0;

      if (stats.collisions == ~0)
        stats.collisions = 0;

      printf("\r\n\t");
      printf("Stats:");
      printf("\t rx_packets = %"PRIu64", tx_packets = %"PRIu64", rx_bytes = %"PRIu64", tx_bytes = %"PRIu64",\r\n\t"
             "\t rx_errors = %"PRIu64", tx_errors = %"PRIu64", rx_drops = %"PRIu64", tx_drops = %"PRIu64",\r\n\t"
             "\t rx_frame_err = %"PRIu64", rx_over_err = %"PRIu64", rx_crc_err = %"PRIu64", collisions = %"PRIu64"",
             stats.rx_packets, stats.tx_packets, stats.rx_bytes, stats.tx_bytes,
             stats.rx_errors, stats.tx_errors, stats.rx_drops, stats.tx_drops,
             stats.rx_frame_err, stats.rx_over_err, stats.rx_crc_err, stats.collisions);
    }
    else
    {
      printf("\r\n\t");
      printf("Error retrieving stats: rc = %d", rc);
    }
  }
  if (showQueues)
  {
    rc = ofdpaNumQueuesGet(portNum, &numQueues);
    if (rc != OFDPA_E_NONE)
    {
      printf("\r\n\t");
      printf("Error retrieving number of queues: rc = %d", rc);
    }
    else
    {
      printf("\r\n\t");
      printf("Queue Stats:");
      printf("\t");

      for (queueId = 0; queueId < numQueues; queueId++)
      {
        rc = ofdpaQueueStatsGet(portNum, queueId, &qStats);
        if (rc != OFDPA_E_NONE)
        {
          printf("Queue %d ERROR: rc = %d", queueId, rc);
        }
        else
        {
          printf("qID = %d, txBytes = %"PRIu64", txPkts = %"PRIu64,
                 queueId, qStats.txBytes, qStats.txPkts);
        }
        /* print seperator */
        if (queueId < (numQueues - 1))
        {
          if ((queueId+1) % 4)
          {
            printf(" | ");
          }
          else
          {
            printf("\r\n\t\t\t");
          }
        }
      }
    }
  }
  printf("\r\n");
}

void displayTunnelPortData(uint32_t portId,
                           ofdpaTunnelPortConfig_t *config,
                           ofdpaTunnelPortStatus_t *status)
{
  char buffer[50];
  ofdpaAccessPortConfig_t      *access;
  ofdpaEndpointConfig_t        *endpoint;
  struct in_addr           ipv4_addr;

  memset(buffer, 0, sizeof(buffer));

  switch (config->tunnelProtocol)
  {
  case OFDPA_TUNNEL_PROTO_VXLAN:
    sprintf(buffer, "vxlan");
    break;
  default:
    sprintf(buffer, "unknown");
    break;
  }

  printf("\r\n\t");
  printf("Protocol = %s, ", buffer);
  if (config->type == OFDPA_TUNNEL_PORT_TYPE_ACCESS)
  {
    access = &config->configData.access;
    printf("Type = ACCESS PORT, ");
    printf("Phys port = %d, ", access->physicalPortNum);
    printf("VLAN ID = %d, ", access->vlanId);
    printf("Untagged = %s, ", access->untagged ? "True" : "False");
    printf("ETAG = %d, ", access->etag);
    printf("Use ETAG = %s", access->useEtag ? "True" : "False");
  }
  else if (config->type == OFDPA_TUNNEL_PORT_TYPE_ENDPOINT)
  {
    endpoint = &config->configData.endpoint;
    printf("Type = ENDPOINT, ");

    ipv4_addr.s_addr = htonl(endpoint->remoteEndpoint);
    if (NULL != inet_ntop(AF_INET, &ipv4_addr, buffer, sizeof(buffer)))
      printf("Remote IP = %s, ", buffer);
    else
      printf("Error converting Remote IP for output., ");


    ipv4_addr.s_addr = htonl(endpoint->localEndpoint);
    if (NULL != inet_ntop(AF_INET, &ipv4_addr, buffer, sizeof(buffer)))
      printf("Local IP = %s, ", buffer);
    else
      printf("Error converting Local IP for output., ");

    printf("TTL = %d, ", endpoint->ttl);
    printf("ECMP = %s, ", endpoint->ecmp ? "True" : "False");
    printf("Nexthop ID = %d", endpoint->nextHopId);

    /* protocol specific stuff */
    if (config->tunnelProtocol == OFDPA_TUNNEL_PROTO_VXLAN)
    {
      ofdpaVxlanProtoInfo_t *vxlan;

      vxlan = &endpoint->protocolInfo.vxlan;

      printf("\r\n\t");
      printf("Terminator dest UDP port = %d, ", vxlan->terminatorUdpDstPort);
      printf("Initiator dest UDP port = %d, ", vxlan->initiatorUdpDstPort);
      printf("Source UDP port if no entropy = %d, ", vxlan->udpSrcPortIfNoEntropy);
      printf("Use entropy = %s", vxlan->useEntropy ? "True" : "False");
    }
    else
    {
      printf("\r\n\t");
      printf("Unknown tunnelProtocol identifier (%d)", config->tunnelProtocol);
    }
  }
  if (status)
  {
    printf("\r\n\t");
    printf("Status: ");
    printf("Reference count = %d, ", status->refCount);
    printf("Tenant count = %d", status->tenantCount);
  }
}

void displayTunnelPort(uint32_t portNum)
{
  OFDPA_ERROR_t rc;
  ofdpaTunnelPortConfig_t config;
  ofdpaTunnelPortStatus_t status;

  displayPortNumName(portNum);

  if ((rc = ofdpaTunnelPortGet(portNum, &config, &status)) == OFDPA_E_NONE)
  {
    if (showStats)
    {
      displayTunnelPortData(portNum, &config, &status);
    }
    else
    {
      displayTunnelPortData(portNum, &config, NULL);
    }
  }
  else
  {
    printf("Error from ofdpaTunnelPortGet(). rc = %d", rc);
  }
  printf("\r\n");
}

void displayOamProtPort(uint32_t portNum)
{
  OFDPA_PORT_STATE_t state = 0;
  OFDPA_PORT_CONFIG_t config = 0;

  ofdpaPortStateGet(portNum, &state);
  ofdpaPortConfigGet(portNum, &config);

  displayPortNumName(portNum);

  printf("\r\n\t");
  printf("config = 0x%08x, ", config);
  printf("state = 0x%08x", state);

  printf("\r\n");
}

int main(int argc, char *argv[])
{
  OFDPA_ERROR_t rc;
  char client_name[] = "ofdpa client";
  uint32_t portNum;
  uint32_t portType;

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

  strcpy(argsDocBuffer, "[portNum]");

  strcpy(docBuffer, "Prints entries in the OF-DPA port table. Optionally limit output to a specific port type or to a specific port ID. "
         "If no option or argument given, all port entries are printed. Specifying a specific port ID takes precedence over port type. "
         "Optionally print port statistics and port queue statistics as appropriate for port type.\n");

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    printf("Error from ofdpaClientInitialize(). (rc == %d)", rc);
    return rc;
  }

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, 0);

  if (showSinglePort)
  {
    portNum = singlePortId;
    /* port ID already checked in parser */
    rc = OFDPA_E_NONE;
  }
  else
  {
    portNum = 0;
    rc = ofdpaPortNextGet(portNum, &portNum);
  }

  while (rc == OFDPA_E_NONE)
  {
    ofdpaPortTypeGet(portNum, &portType);
    switch (portType)
    {
      case OFDPA_PORT_TYPE_PHYSICAL:
        if (showAll || showPhysical || showSinglePort)
        {
          displayPhysicalPort(portNum);
        }
        break;
      case OFDPA_PORT_TYPE_LOGICAL_TUNNEL:
        if (showAll || showTunnel || showSinglePort)
        {
          displayTunnelPort(portNum);
        }
        break;
      case OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT:
        if (showAll || showOam || showSinglePort)
        {
          displayOamProtPort(portNum);
        }
        break;
      default:
        printf("Unhandled OF-DPA port type. portNum = %d (0x%08x), portType = %d (0x%04x)\r\n",
               portNum, portNum, portType, portType);
        break;
    }
    if (showSinglePort)
    {
      break;
    }
    rc = ofdpaPortNextGet(portNum, &portNum);
  }

  return 0;
}
