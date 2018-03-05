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
* @filename     client_oam_dump.c
*
* @purpose      Display OAM configuration and status.
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
#include <netinet/in.h>
#include <arpa/inet.h>


struct
{
  OFDPA_OAM_MEG_TYPE_t type;
  char *name;
} oamMegTypeList[] =
{
  {OFDPA_OAM_MEG_TYPE_ETHERNET,      "Ethernet"},
  {OFDPA_OAM_MEG_TYPE_G8113_1,       "MPLS-TP"},
};
#define MEG_TYPE_LIST_SIZE (sizeof(oamMegTypeList)/sizeof(oamMegTypeList[0]))

char *getMegTypeName(OFDPA_OAM_MEG_TYPE_t type)
{
  int i;

  for (i = 0; i < MEG_TYPE_LIST_SIZE; i++)
  {
    if (oamMegTypeList[i].type == type)
    {
      return(oamMegTypeList[i].name);
    }
  }
  return("Unknown");
}

void displayMeg(uint32_t megIndex,
                ofdpaOamMegConfig_t *config,
                ofdpaOamMegStatus_t *status)
{
  int i;
  char buffer[50];

  memset(buffer, 0, sizeof(buffer));

  printf("MEG Index = %d\r\n", megIndex);
  printf("\tType          = %s\r\n", getMegTypeName(config->megType));

  printf("\tMEG ID        = ");
  for (i = 0; i < sizeof(config->megId); i++)
  {
    if ((i != 0) && (i%16 == 0))
    {
      printf("\r\n\t                ");
    }
    printf("%02x ", config->megId[i]);
  }
  printf("\r\n");

  printf("\tLevel         = %d\r\n", config->level);
  printf("\tMgd Inst Type = %d\r\n", config->managedInstanceType);
  printf("\tPrim VID      = %d\r\n", config->primVid);
  printf("\tMIP creation  = %d\r\n", config->mipCreation);
  printf("\tCCM interval  = %d\r\n", config->ccmInterval);

  if (status)
  {
    printf("\tStatus:\n");
    printf("\t\tReference count = %d\n", status->refCount);
  }
}

void displayMep(uint32_t lmepId,
                ofdpaOamMepConfig_t *config,
                ofdpaOamMepStatus_t *status)
{
  char buffer[50];
  ofdpaMacAddr_t *mac;

  memset(buffer, 0, sizeof(buffer));

  printf("LMEP_ID = %d\r\n", lmepId);
  printf("\tMEG Index       = %d\r\n", config->megIndex);
  printf("\tMEP ID          = %d\r\n", config->mepId);
  printf("\tifIndex         = %d\r\n", config->ifIndex);
  switch (config->direction)
  {
    case OFDPA_MP_DIRECTION_UP:
      snprintf(buffer, sizeof(buffer), "UP");
      break;
    case OFDPA_MP_DIRECTION_DOWN:
      snprintf(buffer, sizeof(buffer), "DOWN");
      break;
    default:
      snprintf(buffer, sizeof(buffer), "UNKNOWN");
      break;
  }
  printf("\tdirection       = %s (%d)\r\n", buffer, config->direction);
  mac = &config->macAddress;
  printf("\tMAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
         mac->addr[0], mac->addr[1], mac->addr[2], mac->addr[3], mac->addr[4], mac->addr[5]);
  printf("\tCCM Priority    = %d\r\n", config->ccmPriority);
  if (config->mlp.mlpRole != OFDPA_MLP_ROLE_NONE)
  {
    switch (config->mlp.mlpRole)
    {
      case OFDPA_MLP_ROLE_PRIMARY:
        snprintf(buffer, sizeof(buffer), "PRIMARY");
        break;
      case OFDPA_MLP_ROLE_BACKUP:
        snprintf(buffer, sizeof(buffer), "BACKUP");
        break;
      default:
        snprintf(buffer, sizeof(buffer), "UNKNOWN");
        break;
    }
    printf("\tMLP Configuration:\r\n");
    printf("\t\tMLP Role        = %s (%d)\r\n", buffer, config->mlp.mlpRole);
    printf("\t\tlivenessPortId  = 0x%x\r\n", config->mlp.u.livenessLogicalPortId);
  }

  if (status)
  {
    printf("\tStatus:\n");
    printf("\t\tReference count  = %d\n", status->refCount);
  }
}

void displayMip(uint32_t localMpId,
                ofdpaOamMipConfig_t *config,
                ofdpaOamMipStatus_t *status)
{
  char buffer[50];
  ofdpaMacAddr_t *mac;

  memset(buffer, 0, sizeof(buffer));

  printf("MIP_ID = %d\r\n", localMpId);
  printf("\tMEG Index       = %d\r\n", config->megIndex);
  printf("\tifIndex         = %d\r\n", config->ifIndex);
  mac = &config->macAddress;
  printf("\tMAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
         mac->addr[0], mac->addr[1], mac->addr[2], mac->addr[3], mac->addr[4], mac->addr[5]);

}

void displayRMp(uint32_t rmpId,
                ofdpaOamRemoteMpConfig_t *config)
{
  char buffer[200];
  ofdpaMacAddr_t *mac;

  memset(buffer, 0, sizeof(buffer));

  printf("rmpId = %d\r\n", rmpId);
  printf("\tLMEP_ID = %d\r\n", config->lmepId);
  printf("\ttype = %s\r\n", (config->rmpType == OFDPA_OAM_MP_TYPE_MEP) ? "MEP":"MIP");
  printf("\tMEP ID = %d\r\n", config->mepId);
  mac = &config->macAddress;
  printf("\tMAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
         mac->addr[0], mac->addr[1], mac->addr[2], mac->addr[3], mac->addr[4], mac->addr[5]);
}

void displayMlpGroup(uint32_t mlpIndex, ofdpaOamMLPGroupConfig_t *config, ofdpaOamMLPGroupStatus_t *status)
{
  OFDPA_ERROR_t rc;
  uint32_t lmepId;
  int first;

  printf("mlpIndex = %d - arch = %s\r\n",
         mlpIndex, (config->mlpArchitecture == OFDPA_MLP_ARCH_1_TO_1) ? "1:1":"1+1");
  printf("\tholdOffTime = %d, wtrTime = %d, revertiveMode = %s\r\n",
         config->holdOffTime, config->wtrTime, config->revertiveMode ? "TRUE":"FALSE");

  printf("\tStatus: refCount = %d\r\n",
         status->refCount);

  lmepId = 0;
  if ((rc = ofdpaOamMepProtectionGet(mlpIndex, lmepId)) != OFDPA_E_NONE)
  {
    rc = ofdpaOamMepProtectionNextGet(mlpIndex, lmepId, &lmepId);
  }

  if (rc == OFDPA_E_NONE)
  {
    first = 1;
    /* display protection MEPs */
    printf("\t\tlmepId: ");
    do
    {
      if (!first)
      {
        printf(", ");
      }
      printf("%d", lmepId);
      first = 0;
    } while (ofdpaOamMepProtectionNextGet(mlpIndex, lmepId, &lmepId) == OFDPA_E_NONE);

    printf("\r\n");
  }
}

void displayMegEntries(void)
{
  OFDPA_ERROR_t rc;
  uint32_t megIndex;
  ofdpaOamMegConfig_t config;
  ofdpaOamMegStatus_t status;

  megIndex = 0;

  if ((rc = ofdpaOamMegGet(megIndex, NULL, NULL)) != OFDPA_E_NONE)
  {
    if ((rc = ofdpaOamMegNextGet(megIndex, &megIndex)) != OFDPA_E_NONE)
    {
      printf("No MEG entries found.\n");
      return;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    /* found an entry, display and iterate */
    do
    {
      if ((rc = ofdpaOamMegGet(megIndex, &config, &status)) == OFDPA_E_NONE)
      {
        displayMeg(megIndex, &config, &status);
      }
      else
      {
        printf("Error retrieving data for MEG entry. (index = %x, rc = %d)\n",
               megIndex, rc);
      }
    } while (ofdpaOamMegNextGet(megIndex, &megIndex) == OFDPA_E_NONE);
  }
}

void displayMepEntries(void)
{
  OFDPA_ERROR_t rc;
  uint32_t lmepId;
  ofdpaOamMepConfig_t config;
  ofdpaOamMepStatus_t status;

  lmepId = 0;

  if ((rc = ofdpaOamMepGet(lmepId, NULL, NULL)) != OFDPA_E_NONE)
  {
    if ((rc = ofdpaOamMepNextGet(lmepId, &lmepId)) != OFDPA_E_NONE)
    {
      printf("No MEP entries found.\n");
      return;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    /* found an entry, display and iterate */
    do
    {
      if ((rc = ofdpaOamMepGet(lmepId, &config, &status)) == OFDPA_E_NONE)
      {
        displayMep(lmepId, &config, &status);
      }
      else
      {
        printf("Error retrieving data for MEP entry. (lmepId = %x, rc = %d)\n",
               lmepId, rc);
      }
    } while (ofdpaOamMepNextGet(lmepId, &lmepId) == OFDPA_E_NONE);
  }
}

void displayMipEntries(void)
{
  OFDPA_ERROR_t rc;
  uint32_t localMpId;
  ofdpaOamMipConfig_t config;
  ofdpaOamMipStatus_t status;

  localMpId = 0;

  if ((rc = ofdpaOamMipGet(localMpId, NULL, NULL)) != OFDPA_E_NONE)
  {
    if ((rc = ofdpaOamMipNextGet(localMpId, &localMpId)) != OFDPA_E_NONE)
    {
      printf("No MIP entries found.\n");
      return;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    /* found an entry, display and iterate */
    do
    {
      if ((rc = ofdpaOamMipGet(localMpId, &config, &status)) == OFDPA_E_NONE)
      {
        displayMip(localMpId, &config, &status);
      }
      else
      {
        printf("Error retrieving data for MIP entry. (localMpId = %x, rc = %d)\n",
               localMpId, rc);
      }
    } while (ofdpaOamMipNextGet(localMpId, &localMpId) == OFDPA_E_NONE);
  }
}

void displayRMpEntries(void)
{
  OFDPA_ERROR_t rc;
  uint32_t rmpId;
  ofdpaOamRemoteMpConfig_t config;

  rmpId = 0;

  if ((rc = ofdpaOamRemoteMpGet(rmpId, NULL)) != OFDPA_E_NONE)
  {
    if ((rc = ofdpaOamRemoteMpNextGet(rmpId, &rmpId)) != OFDPA_E_NONE)
    {
      printf("No Remote MP entries found.\n");
      return;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    /* found an entry, display and iterate */
    do
    {
      if ((rc = ofdpaOamRemoteMpGet(rmpId, &config)) == OFDPA_E_NONE)
      {
        displayRMp(rmpId, &config);
      }
      else
      {
        printf("Error retrieving data for Remote MP entry. (rmId = %x, rc = %d)\n",
               rmpId, rc);
      }
    } while (ofdpaOamRemoteMpNextGet(rmpId, &rmpId) == OFDPA_E_NONE);
  }
}

void displayMlpGroupEntries(void)
{
  OFDPA_ERROR_t rc;
  uint32_t mlpIndex;
  ofdpaOamMLPGroupConfig_t config;
  ofdpaOamMLPGroupStatus_t status;

  mlpIndex = 0;

  if ((rc = ofdpaOamMLPGroupGet(mlpIndex, NULL, NULL)) != OFDPA_E_NONE)
  {
    if ((rc = ofdpaOamMLPGroupNextGet(mlpIndex, &mlpIndex)) != OFDPA_E_NONE)
    {
      printf("No MLP Group entries found.\n");
      return;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    /* found an entry, display and iterate */
    do
    {
      if ((rc = ofdpaOamMLPGroupGet(mlpIndex, &config, &status)) == OFDPA_E_NONE)
      {
        displayMlpGroup(mlpIndex, &config, &status);
      }
      else
      {
        printf("Error retrieving data for MLP entry. (mlpIndex = %x, rc = %d)\n",
               mlpIndex, rc);
      }
    } while (ofdpaOamMLPGroupNextGet(mlpIndex, &mlpIndex) == OFDPA_E_NONE);
  }
}

int main(int argc, char *argv[])
{
  OFDPA_ERROR_t rc;
  char client_name[] = "ofdpa oam dump client";

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    printf("Error from ofdpaClientInitialize(). (rc == %d)", rc);
    return rc;
  }

  printf("--- MEG Table ---\n");
  displayMegEntries();
  printf("--- MEP Table ---\n");
  displayMepEntries();
  printf("--- MIP Table ---\n");
  displayMipEntries();
  printf("--- Remote MP Table ---\n");
  displayRMpEntries();
  printf("--- MLP Group Table ---\n");
  displayMlpGroupEntries();

  return 0;
}
