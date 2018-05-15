/*********************************************************************
*
* (C) Copyright Pure Corporation 2017-2019
*
**********************************************************************
*
* @filename     datapath.h
*
* @purpose      Openflow Datapath Definitions
*
* @component    openflow
*
* @comments     none
*
* @create       04/04/2013
*
* @end
*
**********************************************************************/
#ifndef INCLUDE_DATAPATH_H
#define INCLUDE_DATAPATH_H
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>


/* UNIX datagram socket "address" for client and server. */
#define OFDPA_EVENT_CLIENT_ADDR   "/tmp/ofdpa_event_client"
#define OFDPA_EVENT_SERVER_ADDR   "/tmp/ofdpa_event_server"


#define OFDPA_PIPE_PORT_TABLE_ADDR   "/tmp/datapath_port_table"



#define OFDPA_DATAPATH_MAX_FLOWS_AGE   10000
#define OFDPA_DATAPATH_FLOWS_AGE_INTERVAL_USEC   5000000


#define OFDB_OAM_EVENT_MAX_TYPE 54
#define OFDB_OAM_EVENT_PRESENT  (0x10000000)


typedef enum
{
  OFDB_OAM_EVENT_NONE = 0,
  OFDB_OAM_EVENT_LOC,
  OFDB_OAM_EVENT_AIS,
  OFDB_OAM_EVENT_RDI,
  OFDB_OAM_EVENT_SD,
  OFDB_OAM_EVENT_SF,
  OFDB_OAM_EVENT_LCK,
  OFDB_OAM_EVENT_MMG,
  OFDB_OAM_EVENT_UNM,
  OFDB_OAM_EVENT_UNP,
  OFDB_OAM_EVENT_CSF,
} OFDB_OAM_EVENT_TYPE_t;


typedef enum
{
  OFDB_BFD_EVENT_NONE = 0,
  OFDB_BFD_EVENT_DOWN,
  OFDB_BFD_EVENT_UP, 
} OFDB_BFD_EVENT_TYPE_t;



typedef enum
{
  DPA_EVENT_NOTIFICATION_MSG  = 0x0001,
  DPA_EVENT_PORT_STATUS_MSG   ,
  DPA_EVENT_OAM_STATUS_MSG    ,
  DPA_EVENT_ETH_STATUS_MSG   ,
  DPA_EVENT_APS_STATUS_MSG   ,
  DPA_EVENT_NTU_STATUS_MSG   ,
  DPA_EVENT_MISC_STATUS_MSG   ,  
  DPA_EVENT_SFP_STATUS_MSG,/*yansh add ?a¨¢?¨ºy¡Á?????*/
  DPA_EVENT_BFD_STATUS_MSG,
  DPA_EVENT_PW_REDUN_STATUS_MSG,
  DPA_EVENT_STATUS_MSG_MAX,
} DPA_EVENT_MSG_TYPE_t;



typedef struct
{
  DPA_EVENT_MSG_TYPE_t msgType;
  uint32_t             msgSubType;
  uint32_t             cookie[2];
} dpaEventMsg_t;


/*  key tableId (byte 1), group (byte 2~5), misc manager (bype 6~8) ,
**  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
** | | | | | | | | | | | | | | | | | |
**  -  -  -  -  -  -  -  -  -  -  -  - 
*/
struct PIPE_ENTRY_ADDR
{
	uint64_t						key;
	struct sockaddr_un  sockaddr;
	uint32_t						addrlen;
	char								*sockPath;

};

#define OFDPA_INVALID_KEY 					0xFFFFFFFFFFFFFFFF
#define OFDPA_PORT_MANAGER_ID 			(1UL<<40)
#define OFDPA_ING_ACT_EXECUTOR			(2UL<<40)  
#define OFDPA_EGR_ACT_EXECUTOR			(3UL<<40)  
#define OFDPA_NODE_DEBUG_DIAG						(4UL<<40)  

#define OFDPA_GROUP_TABLE					(1UL<<8)  


/* Flow Table Operation function prototype */
typedef OFDPA_ERROR_t (*flowAdd_fn)(ofdpaFlowEntry_t *flow);
typedef OFDPA_ERROR_t (*flowStatsGet_fn)(ofdpaFlowEntry_t *flow, ofdpaFlowEntryStats_t *flowStats);
typedef OFDPA_ERROR_t (*flowNextGet_fn)(ofdpaFlowEntry_t *flow, ofdpaFlowEntry_t *nextFlow);
typedef OFDPA_ERROR_t (*flowEntryPrint_fn)(ofdpaFlowEntry_t *flow, ofdpaPrettyPrintBuf_t *buf);
typedef uint32_t (*flowTableEntryCountGet_fn)(void);
typedef uint32_t (*flowTableMaxCountGet_fn)(void);


typedef struct ofdpaPipeTblNodeOps_s
{
	flowAdd_fn										flowAdd;
	flowStatsGet_fn								flowStatsGet;
	flowNextGet_fn								flowNextGet;
	flowEntryPrint_fn							flowEntryPrint;
	flowTableEntryCountGet_fn			flowTableEntryCountGet;
	flowTableMaxCountGet_fn				flowTableMaxCountGet;
	
}ofdpaTblPipeNodeOps_t;

typedef struct ofdpaPipeTblNode_s
{
	OFDPA_FLOW_TABLE_ID_t					tableId;
	struct ofdpaPipeTblNode_s 		*this;
	ofdpaTblPipeNodeOps_t					ops;
	uint32_t											valid;
}ofdpaTblPipeNode_t;


/****************************EXPORT API ****************************************/

#define DP_FT_ADD_ACTION_TO_HOLDER(pHolder,pAct) \
{ \
	rv = dpAddAct2Holder(pHolder,pAct); \
	if(rv != OFDPA_E_NONE){ \
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC, \
											 "Add action failed, rc = %d!\r\n", rv); \
		return rv; \
	} \
}while(0)

inline int dpGet32BitsAlignedValue(int value);
OFDPA_ERROR_t dpFlowTblPipeNodeRegister(OFDPA_FLOW_TABLE_ID_t 	tableId,ofdpaTblPipeNodeOps_t *ops);

uint32_t getPktLen(ofdpaPktCb_t *pcb);
void *getFeild(ofdpaPktCb_t *pcb, int type);
void dump_pcb(ofdpaPktCb_t *pcb);
void dump_pkt(const char *_p, int len);
int dpMallocMemFromPktPool(ofdpaPktCb_t *pcb, int len);
OFDPA_ERROR_t datapathPipeMsgSend(int srcSockFd, ofdpaPcbMsg_t *msg);
int datapathInit(void);
struct PIPE_ENTRY_ADDR* datapathPipeTransferEntryGet(uint64_t key);
OFDPA_ERROR_t datapathEventNotificationSend(dpaEventMsg_t *pMsg);
OFDPA_ERROR_t  dpPipeNodeSocketCreate(	uint64_t nodeKey , int *sockId);


ofdpaActHolder_t *dpActHolderMalloc(uint32_t numAct);
ofdpaActSetHolder_t *dpActSetHolderMalloc(uint32_t numAct);
ofdpaGrpPipeNode_t *dpGrpNodeMalloc(ofdpaGrpType_e type, uint32_t numActBukt);
ofdpaActBucket_t *dpGrpBucketMalloc(uint32_t numAct);
OFDPA_ERROR_t dpBindBukt2Grp(ofdpaGrpPipeNode_t *pGrp, ofdpaActBucket_t *pBukt);
OFDPA_ERROR_t dpAddAct2Holder(ofdpaActHolder_t * pHolder , ofdpaAct_t *pAct);
ofdpaActHolder_t *dpActHolderMalloc(uint32_t numAct);
ofdpaActSetHolder_t *dpActSetHolderMalloc(uint32_t numAct);




#include "dp_pipe_act_func_list.h"



#endif /* INCLUDE_DATAPATH_H */
