/************************************************************************
     Pure company          版权所有                          
*************************************************************************
   @BEGAIN_INTERNAL_LEGAL@                                             
                                                                         
  Raisecom  Science & Technology Development Co.,Ltd. Beijing, China     
  Unpublished work-rights reserved under the China Copyright Act.        
  Use,duplication, or disclosure by the government is subject to         
  restrictions set forth in the raisecom commercial license agreement.   
                                                                         
   @END_INTERNAL_LEGAL@                                              
***********************************************************************
  文 件 名   : of_adpl_api.h
  版 本 号   : 初稿
  作    者   : HuShouqiang
  生成日期   : 2016年3月17日
  最近修改   :
  功能描述   : YANG模型底层支撑API接口
  函数列表   :
  修改历史   :
  日    期   : 2016年3月17日
  作    者   : HuShouqiang
  修改内容   : 创建文件

***************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
               time.h
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

#ifndef _OF_ADPL_API_H 
#define _OF_ADPL_API_H


#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifndef __cplusplus
#define printf(...) 			printf(__VA_ARGS__)
#endif

/*------------------------------------------------------------------------------------*/
/* Interface APIs */


#define ADPL_PORT_NAME_LENGTH_MAX		  16 

#define ADPL_PORT_DESCP_LENGTH_MAX		64
/** MAC address length */
#define ADPL_MAC_ADDR_LEN 				6

/** DPID length */
#define ADPL_DPID_LEN 					8

#define ADPL_LINE_PORT_NUM 2

/** ADPL uses these enumerators to indicate the error codes. */
typedef enum
{
  /** Success. */
  ADPL_E_NONE = 0,
  /** Error in RPC. */
  ADPL_E_RPC                  = -20,
  /** Internal error. */
  ADPL_E_INTERNAL             = -21,
  /** Invalid parameter. */
  ADPL_E_PARAM                = -22,
  /** Parameter constraint violated. */
  ADPL_E_ERROR                = -23,
  /** Maximum count is already reached or table full. */
  ADPL_E_FULL                 = -24,
  /** Already exists. */
  ADPL_E_EXISTS               = -25,
  /** Operation Timeout. */
  ADPL_E_TIMEOUT              = -26,
  /** Operation Fail. */
  ADPL_E_FAIL                 = -27,
  /** Disabled. */
  ADPL_E_DISABLED             = -28,
  /** Parameter/feature is not supported. */
  ADPL_E_UNAVAIL              = -29,
  /** Parameter not found. */
  ADPL_E_NOT_FOUND            = -30,
  /** Nothing to report or table is empty. */
  ADPL_E_EMPTY                = -31,
  /** Request denied. */
  ADPL_E_REQUEST_DENIED       = -32,
  /** Not implemented. */
  ADPL_NOT_IMPLEMENTED_YET    = -33,
} ADPL_ERROR_t;

typedef enum
{
  ADPL_FALSE = 0,
  ADPL_TRUE
} ADPL_BOOL;

typedef enum
{
  ADPL_GET = 0,
  ADPL_SET
} ADPL_MODE;

typedef enum adpl_of_version_e {
    ADPL_OF_VERSION_UNKNOWN = 0,
    ADPL_OF_VERSION_1_0 = 1,
    ADPL_OF_VERSION_1_1 = 2,
    ADPL_OF_VERSION_1_2 = 3,
    ADPL_OF_VERSION_1_3 = 4
} adpl_of_version_t;


typedef enum
{
  /** Port type Physical */
  ADPL_PORT_TYPE_PHYSICAL = 0,
  /** Port type Logical Tunnel */
  ADPL_PORT_TYPE_LOGICAL_TUNNEL = 1,
  /** Port type OAM Protection Liveness Logical Tunnel */
  ADPL_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT = 0xf000,
  /** Port type OutBand Management port */
  ADPL_PORT_TYPE_OUTBAND_PORT = 0x100,
  /** Port type InBand Management port */
  ADPL_PORT_TYPE_INBAND_PORT = 0x101,
} ADPL_PORT_TYPE_t;

/** Current state of the port. These are read by
* the controller.
*/
typedef enum
{
  ADPL_PORT_STATE_LINK_DOWN = 1 << 0,   /**< No physical link present. */
  ADPL_PORT_STATE_BLOCKED   = 1 << 1,   /**< Port is blocked. */
  ADPL_PORT_STATE_LIVE      = 1 << 2,   /**< Live for Fast Failover Group. */
} ADPL_PORT_STATE_t;


typedef enum
{
  ADPL_PORT_OP_STATUS_UP 		= 1,
  ADPL_PORT_OP_STATUS_DOWN   	= 2,
  ADPL_PORT_OP_STATUS_TESTING   = 3,
  ADPL_PORT_OP_STATUS_UNKNOWN   = 4,
  ADPL_PORT_OP_STATUS_DORMANT   = 5,
  ADPL_PORT_OP_STATUS_NOPRESENT = 6, /*not-present*/
  ADPL_PORT_OP_STATUS_LLDOWN    = 7, /*lower-layer-down*/
} ADPL_PORT_OP_STATUS_t;

typedef enum
{
  ADPL_PORT_ADMIN_UP 		= 1,
  ADPL_PORT_ADMIN_DOWN   	= 2,
  ADPL_PORT_ADMIN_TESTING   = 3,
} ADPL_PORT_ADMIN_t;

typedef enum
{
  ADPL_PORT_ADMIN_STATUS_UP = 0,
  ADPL_PORT_ADMIN_STATUS_DOWN,
  ADPL_PORT_ADMIN_STATUS_END  
} ADPL_PORT_ADMIN_STATUS_t;

typedef enum
{
  ADPL_PORT_SPEED_AUTO = 0,
  ADPL_PORT_SPEED_10M,
  ADPL_PORT_SPEED_100M,
  ADPL_PORT_SPEED_1000M,
  ADPL_PORT_SPEED_10G,
  ADPL_PORT_SPEED_END  
} ADPL_PORT_SPEED_t;

/*leishenghua modify 20150518, 和yang文件保持一致, 1为全双工，2为半双工*/
typedef enum
{
  ADPL_PORT_DUPLEX_AUTO = 0,
  ADPL_PORT_DUPLEX_FULL,  
  ADPL_PORT_DUPLEX_HALF,
  ADPL_PORT_DUPLEX_END  
} ADPL_PORT_DUPLEX_t;

typedef enum
{
  ADPL_DEVINFO_SLOT_STATUS_NULL = 0,
  ADPL_DEVINFO_SLOT_STATUS_OFFLINE,
  ADPL_DEVINFO_SLOT_STATUS_POWER,
  ADPL_DEVINFO_SLOT_STATUS_MISMATCH,
  ADPL_DEVINFO_SLOT_STATUS_INSERVICE,
} ADPL_DEVINFO_SLOT_STATUS_t;

/** MAC address ,Big endian**/
typedef struct adplMacAddr_s
{
  /** MAC address */
  uint8_t     addr[ADPL_MAC_ADDR_LEN];
} adplMacAddr_t;

/*  add: by yanshi, 2017/2/15   ManuFunctureAll制造厂商信息 */
typedef struct adplManuFunctureAll_s
{   
	/**1 添加slot信息 **/
	unsigned short  slot;   
	/*2 我公司对应型号*/
	unsigned char  ucRaisecomVendor[16]; 
     /**3 提供软件版本号码 **/     
	unsigned char  ucSoftVersion[32];
    /**4 提供厂商序列号 **/
	unsigned char  ucRasiecomSN[64]   ;         
    /**5 提供厂商硬件版本信息 */
	unsigned char  ucHardversion[16]; /* 我公司对应的模块型号*/
	/** 关于命明mac_2    **/
    /**6 提供mac_address2 信息 */
	adplMacAddr_t aucMac2nd[6];/*mac*/
    /**7 提供mac_address3 信息 */
    adplMacAddr_t aucMac3rd[6];	    
} adplManuFunctureAll_t;
ADPL_ERROR_t ADPL_DevManufacture(adplManuFunctureAll_t *pManufacture);

typedef unsigned char   uchar_t;
/** DPID ,Big endian*/
typedef struct adplDpid_s
{
  /** DPID  */
  char     addr[ADPL_DPID_LEN];
} adplDpid_t;

/** DPID ,Big endian*/

typedef union adplDpidUnion_u
{
  /** DPID  */
  char     addr[ADPL_DPID_LEN];
  uint64_t dpid;
} adplDpidUnion_t;

/** Intrinsic buffer descriptor.
 *
 * NOTE: This is intended to be used as a basic data type that can be
 *       passed directly between functions -- keep it small and simple.
 *       To use this as an IN/OUT or OUT value, pass a pointer to the
 *       element so that the called routine can update the 'size' field
 *       with the actual content length being output.
 *
 * NOTE: When setting the 'size' field to indicate the amount of data
 *       contained in the buffer, the conventional usage is to account
 *       for all bytes including any string termination characters
 *       (e.g. strlen()+1).
 */
typedef struct
{
  /** total buffer size (IN) / content length (OUT) */
  uint32_t                size;
  /** ptr to buffer starting location */
  char                   *pstart;
} adpl_buffdesc;


struct ofdpaSocketHandle_s
{
  int     eventSockFd;
  int     pktSockFd;
} ;



#if 0
/****************************************************************
 *
 * Controller connection definitions
 *
 ****************************************************************/

/**
 * Type of protocol to use for connection
 *
 * ADPL_CXN_PROTO_INVALID A marker used to indicate an undefined protocol
 * ADPL_CXN_PROTO_TCP_OVER_IPV4 Use TCP over IPv4 for the connection
 */

typedef enum adpl_cxn_protocol_e {
    ADPL_CXN_PROTO_INVALID            = -1,
    ADPL_CXN_PROTO_TCP_OVER_IPV4      = 0
} adpl_cxn_protocol_t;

/**
 * Parameters for protocol, depending on type.
 *
 * adpl_cxn_params_header_t holds the common part (just the
 * protocol identifier for demultiplexing the union below).
 *
 * To add a type, make sure it starts with a adpl_cxn_protocol_t value
 * initially and then add the parameters needed for that protocol.
 */

typedef struct adpl_cxn_params_header_s {
    adpl_cxn_protocol_t protocol;
} adpl_cxn_params_header_t;

/**
 * TCP over IPv4 parameters
 *     controller_ip IPv4 address of controller
 *     controller_port Port number to use for connection
 */
#define ADPL_CXN_IPV4_STRING_LEN 32
typedef struct adpl_cxn_params_tcp_over_ipv4_s {
    adpl_cxn_protocol_t protocol;
    char controller_ip[ADPL_CXN_IPV4_STRING_LEN];
    uint16_t controller_port;
} adpl_cxn_params_tcp_over_ipv4_t;

/**
 * The super class for connection parameters
 */

typedef union adpl_cxn_protocol_params_u {
    adpl_cxn_params_header_t header;
    adpl_cxn_params_tcp_over_ipv4_t tcp_over_ipv4;
} adpl_cxn_protocol_params_t;

/**
 * Structure for connection configuration
 * @param version The version for the connection (negotiation TBD)
 * @param flags OF specific flags sent in switch config structure
 * @param miss_send_len OF specific value sent in switch config structure
 * @param cxn_priority TBD but probably the priority of connection attempts
 * when connecting serially.
 * @param local Boolean indicates that this is a local connection; see below
 * @param listen This socket listens on a port and accepts new connections
 * @param periodic_echo_ms For non-local connections, period of echo requests;
 * set to 0 to disable.
 * @param reset_echo_count For non-local connections, if this number of
 * consecutive echo replies is not received, the connection is closed.
 *
 * For listen connections, the parameters of the original connection
 * instance are copied to the new connections.
 *
 * See the switch_config structure in the OpenFlow protocol specification
 * for information on flags and miss_send_len.
 *
 * Also, for flags, see OF_FRAG and OF_INVALID_TTL in loci_base.h
 *
 * Local connections are intended to represent UI and other
 * management connections, but handled thru the same machinery. Details
 * are implementation specific, but typically:
 *
 * (1) It listens on its socket (listen is set)
 * (2) It does not require a handshake
 * (3) Async messages such as packet-in or port-status are not sent
 * (4) It can exist simultaneously with remote connections
 * (5) It may only be open for one transaction at a time
 *
 * Remote connections are usually active connect (non-listen) controller
 * connections that require a handshake to continue processing.  Echo
 * requests may be done on these connections as a keepalive.
 */

typedef struct adpl_cxn_config_params_s {
    adpl_of_version_t version;
    int cxn_priority;
    int local;
    int listen;
    uint32_t periodic_echo_ms;
    uint32_t reset_echo_count;
} adpl_cxn_config_params_t;

/****************************************************************
 *
 * Connection state structures
 *
 ****************************************************************/

/**
 * Connection state values
 *    ADPL_CXN_S_DISCONNECTED  Not connected
 *    ADPL_CXN_S_CONNECTING    TCP Connection established
 *    ADPL_CXN_S_HANDSHAKE_COMPLETE  OpenFlow handshake (features) complete
 *    ADPL_CXN_S_CLOSING       Connection broken, cleaning up
 *                                 outstanding operations
 */

typedef enum adpl_cxn_state_e {
    ADPL_CXN_S_DISCONNECTED           = 0,
    ADPL_CXN_S_CONNECTING             = 1,
    ADPL_CXN_S_HANDSHAKE_COMPLETE     = 2,
    ADPL_CXN_S_CLOSING                = 3,
    ADPL_CXN_S_COUNT                  = 4
} adpl_cxn_state_t;

/**
 * Connection roles
 *   Only applicable if the instance is connected.
 *   Needs doc.
 */

typedef enum adpl_cxn_role_e {
    ADPL_CXN_R_UNKNOWN                = 0,
    ADPL_CXN_R_MASTER                 = 1,
    ADPL_CXN_R_SLAVE                  = 2,
    ADPL_CXN_R_EQUAL                  = 3,
    ADPL_CXN_R_COUNT                  = 4
} adpl_cxn_role_t;

/**
 * Status of a connection instance.
 *    state The current connection state (see adpl_cxn_state_t)
 *    role The HA role, if applicable; only valid if state is CONNECTED
 *    negotiated_version If connected, the version used by the connection
 *    disconnect_count Number of times controller has disconnected from switch
 *    forced_disconnect_count Number of times the switch disconnected
 *    bytes_in Number of bytes read in since last connect
 *    bytes_out Number of bytes written in since last connect
 *    messages_in Number of messages received since last connect
 *    messages_out Number of messages sent to controller since last connect
 */


typedef struct adpl_cxn_status_s {
    /* Current status of connection */
    adpl_cxn_state_t state;
    adpl_cxn_role_t role;
    adpl_of_version_t negotiated_version;
    uint32_t disconnect_count;
    uint32_t forced_disconnect_count;
    uint64_t bytes_in;
    uint64_t bytes_out;
    uint64_t messages_in;
    uint64_t messages_out;
    uint64_t packet_in_drop;
    uint64_t flow_removed_drop;
} adpl_cxn_status_t;
#endif

typedef enum ADPL_OFAGENT_ROLE_e
{
    ADPL_OFAGENT_ROLE_SERVER = 0,
	ADPL_OFAGENT_ROLE_CLIENT =1
}ADPL_OFAGENT_ROLE_t;

typedef struct adplAgentCxnParam_s
{
  uint8_t 		controler_ip[4];          
  uint32_t 	  l4_port;
  uint32_t    periodic_echo_ms;
  uint32_t    reset_echo_count;
  uint32_t    pre_reset_echo_count;
	ADPL_OFAGENT_ROLE_t	role;
    
}adplAgentCxnParam_t;

/** Port statistics API structure.
 *  Unsupported counters return the value 0xFFFFFFFFFFFFFFFF indicating the counter data is invalid.
 */
typedef struct adplPortStats_s
{
	/** Received Packets */
	uint64_t rx_packets;
	
	/** Transmitted Packets */
	uint64_t tx_packets;
	
	/** Received Bytes */
	uint64_t rx_bytes;
	
	/** Transmitted Bytes */
	uint64_t tx_bytes;
	
	/** Received Errors */
	uint64_t rx_errors;
	
	/** Transmitted Errors */
	uint64_t tx_errors;
	
	/** Received Packets Dropped */
	uint64_t rx_drops;
	
	/** Transmit Packets Dropped */
	uint64_t tx_drops;
	
	/** Received Frame Errors */
	uint64_t rx_frame_err;
	
	/** Received Frame Overrun Errors */
	uint64_t rx_over_err;
	
	/** Received Packets with CRC Errors */
	uint64_t rx_crc_err;
	
	/** Transmit collisions */
	uint64_t collisions;
	
	
	/*Added by HuShouqiang 20160317 start:*/
	
	/** Received Ucast Packets */
	uint64_t in_ucast_packets;
	
	/** Received Broadcast Packets */
	uint64_t in_broadcast_packets;
	
	/** Received Multicast Packets */
	uint64_t in_multicast_packets;
	
	/** Transmitted Ucast Packets */
	uint64_t out_ucast_packets;
	
	/** Transmitted Broadcast Packets */
	uint64_t out_broadcast_packets;
	
	/** Transmitted Multicast Packets */
	uint64_t out_multicast_packets;
	
	/*Added by HuShouqiang 20160317 end*/
	
	
	/** Time port has been alive in seconds */
	uint32_t duration_seconds;
} adplPortStats_t;



/*------------------------------------------------------------------------------------*/
/* OAM APIs */

#define ADPL_MEG_ID_LENGTH 					48
#define ADPL_LTR_EGRESS_ID_SIZE 			8
#define ADPL_ORG_SPECIFIC_TLV_SIZE 			1500
#define ADPL_ORG_SENDER_ID_TLV_SIZE 		1500
#define ADPL_OAM_PM_FRAME_LEN_MIN        	64
#define ADPL_OAM_PM_FRAME_LEN_MAX        	9600
#define ADPL_OAM_ICC_MIP_ID_LENGTH       	14 /**< number of octets needed to store ICC-based MIP ID */

typedef enum
{
  ADPL_MP_DIRECTION_DOWN = 1,
  ADPL_MP_DIRECTION_UP   = 2

} ADPL_MP_DIRECTION_t;

typedef enum
{
/* 802.1ag clauses 12.14.6.1.3:e, 20.8.1 and 21.6.1.3 */
  ADPL_CCM_INTERVAL_Invalid = 0, /* No CCMs are sent (disabled). */
  ADPL_CCM_INTERVAL_300Hz   = 1, /* CCMs are sent every 3 1/3 milliseconds (300Hz). */
  ADPL_CCM_INTERVAL_10ms    = 2, /* CCMs are sent every 10 milliseconds. */
  ADPL_CCM_INTERVAL_100ms   = 3, /* CCMs are sent every 100 milliseconds. */
  ADPL_CCM_INTERVAL_1s      = 4, /* CCMs are sent every 1 second. */
  ADPL_CCM_INTERVAL_10s     = 5, /* CCMs are sent every 10 seconds. */
  ADPL_CCM_INTERVAL_1min    = 6, /* CCMs are sent every minute. */
  ADPL_CCM_INTERVAL_10min   = 7  /* CCMs are sent every 10 minutes. */

} ADPL_CCM_INTERVAL_t;

typedef enum
{
  ADPL_OAM_MEG_TYPE_ETHERNET,
  ADPL_OAM_MEG_TYPE_G8113_1

} ADPL_OAM_MEG_TYPE_t;

typedef enum
{
  ADPL_OAM_PM_TOOL_TYPE_CCM = 0,
  ADPL_OAM_PM_TOOL_TYPE_LM  = 1,
  ADPL_OAM_PM_TOOL_TYPE_SLM = 2

} ADPL_OAM_PM_TOOL_TYPE_t;

typedef enum
{
  ADPL_OAM_MP_TYPE_MEP = 0,
  ADPL_OAM_MP_TYPE_MIP = 1

} ADPL_OAM_MP_TYPE_t;

typedef enum
{
  ADPL_OAM_PM_ROLE_INITIATOR = 0,
  ADPL_OAM_PM_ROLE_RESPONDER = 1,
  ADPL_OAM_PM_ROLE_BOTH      = 2

} ADPL_OAM_PM_ROLE_t;

typedef enum
{
  ADPL_OAM_PM_STATE_STOPPED = 0,
  ADPL_OAM_PM_STATE_RUNNING = 1

} ADPL_OAM_PM_STATE_t;

typedef enum
{
  ADPL_OAM_DM_TYPE_PROACTIVE = 1,
  ADPL_OAM_DM_TYPE_ONDEMAND  = 2

} ADPL_OAM_DM_TYPE_t;

/* MEG Entry */
typedef struct adplOamMegConfig_s
{
  ADPL_OAM_MEG_TYPE_t       megType;
  char                       megId[ADPL_MEG_ID_LENGTH];
  uint16_t                   level;
  uint16_t                   managedInstanceType;
  uint32_t                   primVid;
  uint32_t                   mipCreation;
  ADPL_CCM_INTERVAL_t       ccmInterval;
  char                  resource_id[48];  // config from d-controller

} adplOamMegConfig_t;

typedef struct adplOamMegStatus_s
{
  uint32_t                   refCount;
} adplOamMegStatus_t;

typedef enum
{
  ADPL_MLP_ROLE_NONE    = 0,
  ADPL_MLP_ROLE_PRIMARY = 1,
  ADPL_MLP_ROLE_BACKUP  = 2,
} ADPL_MLP_ROLE_t;

typedef struct adplOamMlpConfig_s
{
  ADPL_MLP_ROLE_t           mlpRole;
  union
  {
    uint32_t                 livenessLogicalPortId;  /**< used for head-end (1:1) protection */
    uint32_t                 dropStatusId;           /**< used for tail-end (1+1) protection */
  } u;
} adplOamMlpConfig_t;

typedef struct adplMlpConfig_s
{
  ADPL_MLP_ROLE_t           mlpRole;
  union
  {
    uint32_t                 livenessLogicalPortId;  /**< used for head-end (1:1) protection */
    uint32_t                 dropStatusId;           /**< used for tail-end (1+1) protection */
  } u;
} adplMlpConfig_t;


typedef struct adplOamMepConfig_s
{
  uint32_t                   megIndex;                     /**< index of the MEG table entry associated with this MEP entry */
  uint16_t                   mepId;                        /**< value placed in the MEP ID field of OAM PDUs  */
  uint32_t                   ifIndex;
  ADPL_MP_DIRECTION_t       direction;
  adplMacAddr_t             macAddress;                   /**< used as source MAC in Ethernet OAM PDU headers  */

  uint32_t                   ccmPriority;                  /**< used for PCP in Ethernet OAM and TC value in MPLS OAM  */

  adplMlpConfig_t                mlp;

} adplOamMepConfig_t;

typedef struct adplMlpheadendcfg_s
{
    uint32_t livenessport;
    ADPL_MLP_ROLE_t role;
    uint32_t lmepid;
}adplMlpheadendcfg_t;

typedef struct adplOamMepStatus_s
{
  uint32_t                refCount;

  uint32_t                ccmFramesSent;
  uint32_t                ccmFramesReceived;
  uint32_t			      timeout;

} adplOamMepStatus_t;

typedef struct adplOamMipConfig_s
{
  uint32_t                   megIndex;                     /**< index of the MEG table entry associated with this MEP entry */
  uint32_t                   ifIndex;
  ADPL_MP_DIRECTION_t       direction;
  uint8_t                    mipIdTlvData[ADPL_OAM_ICC_MIP_ID_LENGTH]; /**< content of the Target/Replying MEP/MIP Identifier for local MIP */
  adplMacAddr_t             macAddress;                   /**< used as source MAC in Ethernet OAM PDU headers  */

} adplOamMipConfig_t;

typedef struct adplOamMipStatus_s
{
  uint32_t                refCount;

} adplOamMipStatus_t;

typedef struct adplOamRemoteMpConfig_s
{
  uint32_t            lmepId;           /**< LMEP_ID with which this remote MP is associated */
  ADPL_OAM_MP_TYPE_t rmpType;
  uint16_t            mepId;            /**< value found in the MEP ID field of OAM PDUs from remote MEP  */
  uint8_t             mipIdTlvData[ADPL_OAM_ICC_MIP_ID_LENGTH]; /**< content of the Target/Replying MEP/MIP Identifier for remote MIP  */
  uint32_t            ttlToMip;         /**< number of hops to the remote BHH MIP */
  adplMacAddr_t      macAddress;       /**< used as destination MAC in Ethernet OAM PDU headers  */

} adplOamRemoteMpConfig_t;

typedef enum
{
  ADPL_OAM_LB_TEST_TLV_NULL = 0,
  ADPL_OAM_LB_TEST_TLV_NULL_CRC32,
  ADPL_OAM_LB_TEST_TLV_PRBS,
  ADPL_OAM_LB_TEST_TLV_PRBS_CRC32
} ADPL_OAM_LB_TEST_TLV_TYPE_t;

/* Added by JiaoYong, 2017/1/6 */
typedef enum
{
  ADPL_LB_STATE_NULL = 0,/*LB未创建或者已经删除*/
  ADPL_LB_STATE_CREATED,/*LB已创建*/
  ADPL_LB_STATE_STARTED,/*LB已开始*/
  ADPL_LB_STATE_STOPPED /*LB已停止*/ 
} ADPL_LB_STATE_t;

typedef enum
{
  ADPL_OAM_LB_DISCOVERY_NONE = 0,
  ADPL_OAM_LB_DISCOVERY_INGRESS,
  ADPL_OAM_LB_DISCOVERY_EGRESS,
  ADPL_OAM_LB_DISCOVERY_ICC_MEPID,
  ADPL_OAM_LB_DISCOVERY_ICC_MIPID,
  ADPL_OAM_LB_DISCOVERY_RESERVED
} ADPL_OAM_LB_DISCOVERY_TYPE_t;

typedef struct adplOamLbConfig_s
{
  uint32_t                      lbDestMpId;                  /**< lmpId of loopback message destination */
  uint32_t                      lbPeriod;                    /**< period between LBM messages in ms > */
  uint32_t                      lbPriority;                  /**< used for PCP in Ethernet OAM and TC value in MPLS OAM  */
  uint32_t                      lbDropEligibility;
  uint32_t                      lbNbrPacketsToSend;
  uint32_t                      lbPacketLength;
  ADPL_BOOL                     lbTestPatternTlvPresent;
  ADPL_OAM_LB_TEST_TLV_TYPE_t   lbTestPatternTlvType;
  ADPL_OAM_LB_DISCOVERY_TYPE_t  lbDiscovery;
  uint32_t                      lbTtl;
  ADPL_LB_STATE_t               lbState;/*JiaoYong 20170106*/
} adplOamLbConfig_t;

typedef struct adplOamLbStatus_s
{
  ADPL_BOOL              lbResultOk;
  uint32_t                lbrIn;
  uint32_t                lbrInOutOfOrder;
  uint32_t                lbrBadMsdu;

} adplOamLbStatus_t;

typedef struct adplOamLckConfig_s
{
  uint32_t                   lckClientMegLevel;
  uint32_t                   lckPeriod;
  uint32_t                   lckTrafficClass; /**< used for PCP in Ethernet OAM and TC value in MPLS OAM  */

} adplOamLckConfig_t;

typedef struct adplOamAisConfig_s
{
  uint32_t                   aisClientMegLevel;
  ADPL_CCM_INTERVAL_t       aisPeriod;
  uint32_t                   aisPriority;                  /**< used for PCP in Ethernet OAM and TC value in MPLS OAM  */

} adplOamAisConfig_t;

typedef struct adplOamCsfConfig_s
{
  ADPL_CCM_INTERVAL_t       csfPeriod;                    /**< only ADPL_CCM_INTERVAL_1s and ADPL_CCM_INTERVAL_1min are accepted for this parameter */
  uint32_t                   csfPriority;                  /**< used for PCP in Ethernet OAM and TC value in MPLS OAM  */

} adplOamCsfConfig_t;

typedef struct adplOamLtConfig_s
{
  uint32_t                        ltDestMpId;
  uint32_t                        ltPriority;
  uint32_t                        ltTtl;
} adplOamLtConfig_t;

typedef struct adplOamLtStatus_s
{
  uint32_t                ltResultOK;
  uint32_t                ltUnexpLtrIn;
} adplOamLtStatus_t;

typedef enum
{
  ADPL_OAM_RMEP_STATE_IDLE   = 1,
  ADPL_OAM_RMEP_STATE_START  = 2,
  ADPL_OAM_RMEP_STATE_FAILED = 3,
  ADPL_OAM_RMEP_STATE_OK     = 4
} ADPL_OAM_RMEP_STATE_t;

typedef struct adplOamCcmDatabaseEntry_s
{
  ADPL_OAM_RMEP_STATE_t  remoteMepState;
  adplMacAddr_t          remoteMepMacAddr;
  uint32_t                lastUpdateTime;

} adplOamCcmDatabaseEntry_t;



typedef struct adplOamLtrEntry_s
{
  uint32_t                   ttl;
  uint32_t                   forwarded;
  uint32_t                   terminalMep;
  uint8_t                    lastEgressId[ADPL_LTR_EGRESS_ID_SIZE];
  uint8_t                    nextEgressId[ADPL_LTR_EGRESS_ID_SIZE];
  uint32_t                   relayAction;
  uint32_t                   ingressAction;
  adplMacAddr_t             ingressMac;
  uint32_t                   ingressPortIdSubtype;
  uint32_t                   ingressPortId;
  uint32_t                   egressAction;
  adplMacAddr_t             egressMac;
  uint32_t                   egressPortIdSubtype;
  uint32_t                   egressPortId;
  adpl_buffdesc             organizationSpecificTlv;
  adpl_buffdesc             senderIdTlv;

} adplOamLtrEntry_t;

typedef struct adplOamProLmConfig_s
{
  ADPL_OAM_PM_TOOL_TYPE_t        toolType;                         /**< 0: CCM, 1: LM, 2: SLM */
  uint32_t                        proLmDestMpId;
  ADPL_CCM_INTERVAL_t            period;                           /**< 0: default (100ms), 1: 3.33ms, 2: 10ms, 3: 100ms, 4: 1s, 5: 10s, 6: 60s, 7: 600s */
  uint32_t                        trafficClass;                     /**< Priority value for MPLS Ethernet OAM and PHB value for MPLS G8113.1 OAM */
  uint32_t                        testId;                           /**< test identifier used in SLM based loss measurement */
} adplOamProLmConfig_t;

typedef struct adplOamProLmStatus_s
{
  ADPL_OAM_PM_STATE_t proLmState;
} adplOamProLmStatus_t;

typedef struct adplOamOdLmConfig_s
{
  ADPL_OAM_PM_TOOL_TYPE_t        toolType;                         /**< 0: CCM, 1: LM, 2: SLM */
  uint32_t                        odLmDestMpId;
  ADPL_CCM_INTERVAL_t            period;                           /**< 0: default (100ms), 1: 3.33ms, 2: 10ms, 3: 100ms, 4: 1s, 5: 10s, 6: 60s, 7: 600s */
  uint32_t                        trafficClass;                     /**< Priority value for MPLS Ethernet OAM and PHB value for MPLS G8113.1 OAM */
  ADPL_OAM_PM_ROLE_t             role;                             /**< 0: initiator, 1: responder, 2: both */
  uint32_t                        testId;                           /**< test identifier used in SLM based loss measurement */
  uint32_t                        enable;					/*1: enable, 0: disable*/ 			
} adplOamOdLmConfig_t;

typedef struct adplOamOdLmStatus_s
{
  ADPL_OAM_PM_STATE_t odLmState;
} adplOamOdLmStatus_t;

typedef struct adplOamProDmConfig_s
{
  uint32_t                        proDmDestMpId;
  ADPL_CCM_INTERVAL_t            period;                           /**< 0: default (100ms), 1: 3.33ms, 2: 10ms, 3: 100ms, 4: 1s, 5: 10s, 6: 60s, 7: 600s */
  uint32_t                        trafficClass;                     /**< Priority value for MPLS Ethernet OAM and PHB value for MPLS G8113.1 OAM */
  uint32_t                        frameLen;                         /**< 64 - 9600, Ethernet minimum PDU may differ in BHH OAM DMM PDU*/
} adplOamProDmConfig_t;

typedef struct adplOamProDmStatus_s
{
  ADPL_OAM_PM_STATE_t proDmState;
} adplOamProDmStatus_t;

typedef struct adplOamOdDmConfig_s
{
  uint32_t                        odDmDestMpId;
  ADPL_CCM_INTERVAL_t            period;                           /**< 0: default (100ms), 1: 3.33ms, 2: 10ms, 3: 100ms, 4: 1s, 5: 10s, 6: 60s, 7: 600s */
  uint32_t                        trafficClass;                     /**< Priority value for MPLS Ethernet OAM and PHB value for MPLS G8113.1 OAM */
  uint32_t                        frameLen;                         /**< 64 - 9600: Ethernet minimum PDU may differ in BHH OAM DMM PDU.*/
  ADPL_OAM_PM_ROLE_t             role;                             /**< 0: initiator, 1: responder, 2: both */
  uint32_t                        enable;					/*1: enable, 0: disable*/ 
} adplOamOdDmConfig_t;

typedef struct adplOamOdDmStatus_s
{
  ADPL_OAM_PM_STATE_t odDmState;     /* this is a placeholder member, the correct status elements to be reported need to be designed */
} adplOamOdDmStatus_t;

typedef enum
{
  ADPL_OAM_BIN_MODE_15M =  0,
  ADPL_OAM_BIN_MODE_24H =  1,
  ADPL_OAM_BIN_MODE_MAX =  2
} ADPL_OAM_BIN_MODE_t;


typedef struct adplOamProLmCounterBin_s
{
  uint32_t                mN_FLR;      /**< Minimum Near-end Frame Loss Ratio  */
  uint32_t                aN_FLR;      /**< Average Near-end Frame Loss Ratio  */
  uint32_t                xN_FLR;      /**< Maximum Near-end Frame Loss Ratio  */
  uint32_t                mF_FLR;      /**< Minimum Far-end Frame Loss Ratio  */
  uint32_t                aF_FLR;      /**< Average Far-end Frame Loss Ratio  */
  uint32_t                xF_FLR;      /**< Maximum Far-end Frame Loss Ratio  */
} adplOamProLmCounterBin_t;

typedef struct adplOamProLmCounters_s
{
  adplOamProLmCounterBin_t bin_15min;
  adplOamProLmCounterBin_t bin_24hr;
} adplOamProLmCounters_t;

typedef struct adplOamOdLmSnapshot_s
{
  uint32_t                tNTFCnt;     /**< Total Near-end Transmitted Frame Count */
  uint32_t                tNLFCnt;     /**< Total Near-end Lost Frame Count */
  uint32_t                tNFLR;       /**< Total Near-end Frame Loss Ratio ,  no use*/
  uint32_t                tFTFCnt;     /**< Total Far-end Transmitted Frame Count */
  uint32_t                tFLFCnt;     /**< Total Far-end Lost Frame Count */
  uint32_t                tFFLR;       /**< Total Far-end Frame Loss Ratio ,  no use*/
  uint32_t                tLMCnt;      /**< tx Loss Measurement Count */
  uint32_t                rLMCnt;      /**< rx Loss Measurement Count */
} adplOamOdLmSnapshot_t;

typedef struct adplOamProDmCounterBin_s
{
  uint32_t                mB_FD;        /**< Minimum Bidirectional Frame Delay */
  uint32_t                aB_FD;        /**< Average Bidirectional Frame Delay */
  uint32_t                xB_FD;        /**< Maximum Bidirectional Frame Delay */
  uint32_t                mN_FDV;       /**< Minimum Near-end Frame Delay Variation */
  uint32_t                aN_FDV;       /**< Average Near-end Frame Delay Variation */
  uint32_t                xN_FDV;       /**< Maximum Near-end Frame Delay Variation */
  uint32_t                mF_FDV;       /**< Minimum Far-end Frame Delay Variation */
  uint32_t                aF_FDV;       /**< Average Far-end Frame Delay Variation */
  uint32_t                xF_FDV;       /**< Maximum Far-end Frame Delay Variation */
} adplOamProDmCounterBin_t;

typedef struct adplOamProDmCounters_s
{
  adplOamProDmCounterBin_t bin_15min;
  adplOamProDmCounterBin_t bin_24hr;
} adplOamProDmCounters_t;

typedef struct adplOamOdDmSnapshot_s
{
  uint32_t                tN_FD;        /**< Total Near-end Frame Delay ,  no use*/
  uint32_t                tF_FD;        /**< Total Far-end Frame Delay ,  no use */
  uint32_t                tB_FD;        /**< Total Bidirectional Frame Delay */
  uint32_t                elapsedTime;  /**< Time since Delay Measurement began ,  no use*/
  uint32_t                tFDMCnt;      /**< Number of tx delay measurements */
  uint32_t                rFDMCnt;      /**< Number of rx delay measurements */
} adplOamOdDmSnapshot_t;

typedef enum
{
  ADPL_PROTECTION_SWITCH_REQUEST_TYPE_CLEAR            = 2,
  ADPL_PROTECTION_SWITCH_REQUEST_TYPE_MANUAL_LOCK      = 3,
  ADPL_PROTECTION_SWITCH_REQUEST_TYPE_FORCE_SWITCH     = 4,
  ADPL_PROTECTION_SWITCH_REQUEST_TYPE_MANUAL_SWITCH_P  = 5,
  ADPL_PROTECTION_SWITCH_REQUEST_TYPE_MANUAL_SWITCH    = 6,
} ADPL_PROTECTION_SWITCH_REQUEST_TYPE_t;

typedef enum
{
  ADPL_MLP_ARCH_1_TO_1   = 0,  /**< 1:1 */
  ADPL_MLP_ARCH_1_PLUS_1 = 1,  /**< 1+1 */
} ADPL_MLP_ARCH_t;

typedef struct adplOamMLPGroupConfig_s
{
  ADPL_MLP_ARCH_t  mlpArchitecture; /**< specifies protection architecture (1:1, 1+1) */
  uint32_t          holdOffTime;     /**< 0 to 10 seconds, expressed in 100ms steps */
  uint32_t          wtrTime;         /**< 5 to 12 minutes, expressed in minutes */
  ADPL_BOOL        revertiveMode;   /**< set to ADPL_FALSE for non-revertive operation, ADPL_TRUE for revertive operation */
  char          resource_id[48];  // config from d-controller

} adplOamMLPGroupConfig_t;

typedef struct adplOamMLPGroupStatus_s
{
  uint32_t lastLReq;      /*last local request*/
  uint32_t lastRReq;      /*last remote request*/
  uint32_t lastSReq;      /*last send request, it's used for continuing sending APS packets*/

  uint32_t sfwState;      /*local SF-W exist flag: True/False*/
  uint32_t sfpState;      /*local SF-P exist flag: True/False*/
  uint32_t sdwState;      /*local SD-W exist flag: True/False*/
  uint32_t sdpState;      /*local SD-P exist flag: True/False*/

  uint32_t workingStatus; /*working LSP link status: UP/DOWN*/
  uint32_t standbyStatus; /*standby LSP link status: UP/DOWN*/

  uint8_t holdOffBlock;   /*Received events are blocked by hold-off-timer*/
  uint32_t dfop_cm;       /*ALARM: dFOP-CM*/
  uint32_t dfop_pm;       /*ALARM: dFOP-PM*/
  uint32_t dfop_nr;       /*ALARM: dFOP-NR*/
  uint32_t dfop_to;       /*ALARM: dFOP-TO*/

  uint32_t                refCount;

} adplOamMLPGroupStatus_t;


typedef struct adplPwRedunConfig_s
{
  uint32_t  		mode; /*master/slave, independent*/
  uint8_t 			name[48];		/*resource id*/
  uint32_t          wtrTime;         /**< 0 to 12 minutes, expressed in minutes */
  ADPL_BOOL        revertiveMode;   /**< set to ADPL_FALSE for non-revertive operation, ADPL_TRUE for revertive operation */
  uint32_t			command;

} adplPwRedunConfig_t;

typedef struct adplPwRedunStatus_s
{
  ADPL_MLP_ROLE_t role;      /*working entry*/
  uint32_t switchCount;     
  uint32_t lastSwtichTime;      
 
} adplPwRedunStatus_t;


/** OAM event type */
typedef enum
{
  ADPL_OAM_EVENT_ON_DEMAND_LM_SESSION_COMPLETED = 1 << 0,
  ADPL_OAM_EVENT_ON_DEMAND_DM_SESSION_COMPLETED = 1 << 1

} ADPL_OAM_EVENT_MASK_t;

/** OAM events */
typedef struct adplOamEvent_s
{
  ADPL_OAM_EVENT_MASK_t   eventMask;

  uint32_t             megIndex;
  uint32_t             mepId;

} adplOamEvent_t;

/**cxn status */
typedef struct adpl_OfCxnStatus_s {
    uint8_t verson[8];
    uint8_t role[8];
    uint8_t state[16];
    uint32_t MsgIn;
    uint32_t MsgOut;
}adpl_ofCxnStatus_t;

typedef ADPL_ERROR_t (*ADPL_EventCallback)(uint32_t eventType,uint32_t cookie);
typedef void (*ADPL_pkt_ready_callback_f)(
    int socket_id,
    void *cookie,
    int read_ready,
    int write_ready,
    int error_seen);

/** Packet In reason codes  */
typedef enum
{
  /** No Match */
  ADPL_PACKET_IN_REASON_NO_MATCH = 0,
  /** Action */
  ADPL_PACKET_IN_REASON_ACTION,
  /** Invalid TTL */
  ADPL_PACKET_IN_REASON_INVALID_TTL,

  ADPL_PACKET_IN_REASON_OAM,

} ADPL_PACKET_IN_REASON_t;

/** Flow Table Id enumerator */
typedef enum
{
  ADPL_FLOW_TABLE_ID_INGRESS_PORT                      =    0,  /**< Ingress Port Table */
  ADPL_FLOW_TABLE_ID_PORT_DSCP_TRUST                   =    5,  /**< Port DSCP Trust Table */
  ADPL_FLOW_TABLE_ID_PORT_PCP_TRUST                    =    6,  /**< Port PCP Trust Table */
  ADPL_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST                 =    7,  /**< Tunnel DSCP Trust Table */
  ADPL_FLOW_TABLE_ID_TUNNEL_PCP_TRUST                  =    8,  /**< Tunnel PCP Trust Table */
  ADPL_FLOW_TABLE_ID_INJECTED_OAM                      =    9,  /**< Injected OAM Table */
  ADPL_FLOW_TABLE_ID_VLAN                              =   10,  /**< VLAN Table */
  ADPL_FLOW_TABLE_ID_VLAN_1                            =   11,  /**< VLAN 1 Table */
  ADPL_FLOW_TABLE_ID_MAINTENANCE_POINT                 =   12,  /**< Maintenance Point Flow Table */
  ADPL_FLOW_TABLE_ID_MPLS_L2_PORT                      =   13,  /**< MPLS L2 Port Table */
  ADPL_FLOW_TABLE_ID_MPLS_DSCP_TRUST                   =   15,  /**< MPLS QoS DSCP Trust Table */
  ADPL_FLOW_TABLE_ID_MPLS_PCP_TRUST                    =   16,  /**< MPLS QoS PCP Trust Table */
  ADPL_FLOW_TABLE_ID_L2_POLICER                        =   18,  /**< L2 Policer */
  ADPL_FLOW_TABLE_ID_L2_POLICER_ACTIONS                =   19,  /**< L2 Policer Actions */
  ADPL_FLOW_TABLE_ID_TERMINATION_MAC                   =   20,  /**< Termination MAC Table */
  ADPL_FLOW_TABLE_ID_L3_TYPE                           =   21,  /**< L3 Type Table */
  ADPL_FLOW_TABLE_ID_MPLS_0                            =   23,  /**< MPLS 0 Table */
  ADPL_FLOW_TABLE_ID_MPLS_1                            =   24,  /**< MPLS 1 Table */
  ADPL_FLOW_TABLE_ID_MPLS_2                            =   25,  /**< MPLS 2 Table */
  ADPL_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT            =   26,  /**< MPLS-TP Maintenance Point Flow Table */
  ADPL_FLOW_TABLE_ID_MPLS_L3_TYPE                      =   27,  /**< MPLS L3 Type Flow Table */
  ADPL_FLOW_TABLE_ID_MPLS_TYPE                         =   29,  /**< MPLS Type Flow Table */
  ADPL_FLOW_TABLE_ID_UNICAST_ROUTING                   =   30,  /**< Unicast Routing Table */
  ADPL_FLOW_TABLE_ID_MULTICAST_ROUTING                 =   40,  /**< Multicast Routing Table */
  ADPL_FLOW_TABLE_ID_BRIDGING                          =   50,  /**< Bridging Table */
  ADPL_FLOW_TABLE_ID_ACL_POLICY                        =   60,  /**< ACL Table */
  ADPL_FLOW_TABLE_ID_COLOR_BASED_ACTIONS               =   65,  /**< Color Based Actions */
  ADPL_FLOW_TABLE_ID_EGRESS_VLAN                       =  210,  /**< Egress VLAN Table */
  ADPL_FLOW_TABLE_ID_EGRESS_VLAN_1                     =  211,  /**< Egress VLAN 1 Table */
  ADPL_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT          =  226,  /**< Egress Maintenance Point Flow Table */
  ADPL_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK            =  230,  /**< Egress DSCP PCP Remark Flow Table */
  ADPL_FLOW_TABLE_ID_EGRESS_TPID                       =  235,  /**< Egress TPID Flow Table */

} ADPL_FLOW_TABLE_ID_t;

/** Packet  */
typedef struct adplPacket_s
{
  /** Reason */
  ADPL_PACKET_IN_REASON_t  reason;

  /** Flow Table Id */
  ADPL_FLOW_TABLE_ID_t     tableId;

  /** Input port */
  uint32_t                  inPortNum;

  /** Packet */
  adpl_buffdesc            pktData;
} adplPacket_t;



/* Structure that defines the meta data that the driver includes when it
 * sends a received packet to a client. One instance of this structure
 * preceeds the packet itself. */
typedef struct adplPktMetaData_s
{
  /* One of OFDPA_PACKET_IN_REASON_t */
  uint32_t rxReason;

  /* One of OFDPA_FLOW_TABLE_ID_t */
  uint32_t tableId;

  /* Ingress physical port. OFDPA port number.  */
  uint32_t inPort;

  /* Packet length in bytes (not including meta-data) */
  uint32_t pktLen;

  uint32_t acl_rule;                    /* Rule number which matched */
  uint32_t timestamp;                   /* For time sync or OAM DM */
  uint32_t timestamp_upper;             /* For OAM DM */

  uint16_t vlanId;                      /* VLAN on which PDU was received */
  uint16_t innerVlanId;                 /* Inner VLAN if present */

  uint8_t  vlanPri;                     /* VLAN tag priority  */
  uint8_t  innerVlanPri;                /* Inner VLAN tag priority */
  uint8_t  cos;                         /* Received CoS */
  uint8_t  prio_int;                    /* Internal priority of the packet. */

  uint32_t lmepId;                      /* For OAM */
  uint8_t  oamOpcode;                   /* For OAM Opcode */
  uint8_t  oamReplacementOffset;        /* For OAM LM and DM */

  uint32_t megIndex;                    /* For OAM */
  uint32_t outPort;                     /* For OAM Up MEPs */
  uint8_t  cwPresent;                   /* For OAM received packets */

  /* Pointer to packet data if the packet does not immediately follow the meta data.  NULL otherwise. */
  uint8_t *data;

} adplPktMetaData_t;


typedef int (* adplPacketCbFunction)(adplPktMetaData_t *packet);


#define BFD_KEY_MAX_LEN 16

/*会话配置类型*/
typedef enum 
{
    BFD_CFG_STATIC = 1,      /* 静态会话 */
    BFD_CFG_DYNAMIC,         /* 动态会话 */
    BFD_CFG_MAX
}bfdCfgType_e;


typedef struct adplBfdConfig_s
{
  uint32_t                   ifIndex;  
  uint8_t 					name[48];		/*resource id*/
  bfdCfgType_e 				eCfgType;         /*配置类型*/  
  uint32_t 					localDis;            				/*会话本地标识*/
  uint32_t 					remoteDis;           				/*会话远端标识*/
  uint32_t 					desiredMinTxInter;	 /*会话最小发送间隔,单位us*/
  uint32_t 					requestMinRxInter;	 /*会话最小接收间隔,单位us*/
  uint32_t 					detectMult; 	   	/*会话检测倍数*/

  uint8_t					BfdSessDstAddr[4];
  uint8_t					BfdSessSrcAddr[4];
  uint32_t 					ulPwTtl;              /**<PW TTL*/
  
  long lAuthType;		 /*会话认证类型*/
  long lAuthKeyID;		 /*会话认证key-id*/
  char acAuthKey[BFD_KEY_MAX_LEN + 1];/*会话认证key*/
  
  adplMlpConfig_t                mlp;

} adplBfdConfig_t;


/*BFD会话状态枚举*/
typedef enum 
{
    bfdStateAdminDown = 0,
    bfdStateDown,
    bfdStateInit,
    bfdStateUp,
    bfdStateMax
} adpl_bfdSessionState_t;

/*BFD会话诊断码枚举*/
typedef enum 
{
    bfdDiagCodeNone = 0,
    bfdDiagCodeCtrlDetectTimeExpired,
    bfdDiagCodeEchoFailed,
    bfdDiagCodeNeighbourSignalledDown,
    bfdDiagCodeForwardingPlaneReset,
    bfdDiagCodePathDown,
    bfdDiagCodeConcatPathDown,
    bfdDiagCodeAdminDown,
    bfdDiagCodeRevConcatPathDown,
    bfdDiagCodeMisConnectivityDefect,
    bfdDiagCodeMax
} adpl_bfdDiagCode_t;


typedef struct adplBfdStatus_s
{
  uint32_t			  refCount;
  
  /*协商参数*/
  uint32_t ulNegoTxInter;		 /*发送间隔*/
  uint32_t ulNegoRxInter;		 /*接收间隔*/
  uint32_t ulNegoEchoInter;		 /*echo发送间隔*/
  uint32_t ulNegoDetectMult; 	 /*检测倍数*/
  uint32_t remoteDisc;		 /*远端标识符*/
  
  adpl_bfdSessionState_t ulSessionState;		 /*会话状态*/  
  adpl_bfdDiagCode_t ulSessionDiag;		 /*会话诊断信息*/

	uint32_t ulCtrlPktIn;/*接收控制报文个数*/
	uint32_t ulCtrlPktOut;/*发送控制报文个数*/
	uint32_t ulCtrlPktDrop;/*丢弃控制报文个数*/

	uint32_t stSessionUpTime;/*会话最近一次UP时刻*/
	uint32_t stSessionDownTime;/*会话最近一次down时刻*/
	adpl_bfdDiagCode_t ulLastCommLostDiag;/*最近一次down时的诊断信息*/
	uint32_t ulSessionUpCount;/*会话up次数*/

	adpl_bfdSessionState_t ulRemoteState;		   /*会话状态*/  
	adpl_bfdDiagCode_t ulRemoteDiag;	   /*会话诊断信息*/


} adplBfdStatus_t;


/*yansh add 风扇添加 20160904*/
#define ADPL_MAX_FAN_SUBCARD_NUM           2
#define ADPL_MAX_FAN_NUM                   6
#define ADPL_MAX_FAN_SPD_LEV_NUM           4
typedef struct adpl_fanSpdLevCfg_s
{
    unsigned long         ulSpdLev;           /*档位*/
    unsigned long         ulTempScale;        /*升档温度*/
    unsigned long         ulSpeed;            /*转速*/
    unsigned long         ulAlarmSpdLow;      /*低速告警*/
    unsigned long         ulAlarmSpdHigh;     /*高速告警*/
}adpl_fanSpdLevCfg_t;
typedef enum
{
    ADPL_FAN_NORMAL = 1,
    ADPL_FAN_ABNORMAL,
}adpl_fanStatus_e;
#if 1
typedef enum
{
    ADPL_FAN_MODE_ENFORCE = 1,
    ADPL_FAN_MODE_AUTO,
}adpl_fanMoniMode_e;
typedef struct adpl_fanInfo_s
{
    adpl_fanStatus_e           eFanStatus;         /*当前风扇状态*/
    unsigned char*        pucFanName;         /*当前风扇名称*/
    unsigned long         ulCurSpeed;         /*当前风扇转速*/
}adpl_fanInfo_t;
typedef enum
{    
    ADPL_FAN_CARD_UP = 1,
    ADPL_FAN_CARD_DOWN,
}adpl_fanCardStatus_e;
typedef struct adpl_fanCurDataInfo_s
{
    unsigned long               ulFanCardNum;           /*当前监控风扇子卡数量*/
    unsigned long               ulFanNumOnCard;         /*子卡上风扇数量*/
    adpl_fanCardStatus_e             eFanCardStatus[ADPL_MAX_FAN_SUBCARD_NUM];/*当前风扇子卡状态*/
    unsigned long               ulFanNum;               /*当前监控风扇数量*/
    adpl_fanInfo_t                   stFanInfo[ADPL_MAX_FAN_NUM]; /*风扇信息*/
    unsigned long               ulSpdLevNum;            /*当前档位数*/
    adpl_fanSpdLevCfg_t              stFanSpdLevCfg[ADPL_MAX_FAN_SPD_LEV_NUM];      /*目前我们的风扇管理仅有一种控制策略*/
    adpl_fanMoniMode_e               eMoniMode;              /*当前风扇监控模式*/
    unsigned long               ulCurSpdLev;            /*当前风扇档位*/
    unsigned long               ulEnforceSpdLev;        /*当前风扇强制档位*/   
    unsigned long               ulTrapEnable;           /*trap发送开关*/
}adpl_fanCurDataInfo_t;

/**yanshi add: ddm**/
/*密码交验信息结构*/
typedef unsigned long   ulong_t;
typedef unsigned short  ushort_t;
typedef struct transceiverPwdInfo_s
{
	unsigned short  usPortId;                        /*端口号*/
	unsigned long   ulIfIndex;          /*全局索引*/
	unsigned char  ucRaisecomPN[16]; /* 我公司对应型号*/
	unsigned char  ucRaisecomSN[16]; /* 我公司对应的模块型号*/
	unsigned char  ucRaisecomCRC[4];   /*公司型号和模块型号的对应CRC交验码*/	    
} adpl_transceiverPwdInfo_t;
typedef struct
{
	ushort_t  	usHighAlarm;  
	ushort_t  	usLowAlarm; 
	ushort_t  	usHighWarning; 
	ushort_t  	usLowWarning;  
} adpl_digitalDiagnoticUnsignedAlarm_t;
typedef struct
{
	short  	sHighAlarm;  
	short  	sLowAlarm; 
	short  	sHighWarning; 
	short  	sLowWarning;  
} adpl_digitalDiagnoticSignedAlarm_t;
typedef struct
	{
		adpl_digitalDiagnoticSignedAlarm_t structTemperature;    /*模块温度域值*/       /*XFP_SFP*/
		adpl_digitalDiagnoticUnsignedAlarm_t structSupplyVoltage;/*供电电压域值*/       /*SFP*/    
		adpl_digitalDiagnoticUnsignedAlarm_t structBiasCurrent;  /*激光器偏置电流域值*/ /*XFP_SFP*/
		adpl_digitalDiagnoticUnsignedAlarm_t structTxOutputPower;/*发送光功率域值*/     /*XFP_SFP*/
		adpl_digitalDiagnoticUnsignedAlarm_t structReceivedPower;/*接收光功率域值*/     /*XFP_SFP*/   

		ulong_t   reserve_1[4];  /*SFP*/
	    float	  fRx_PWR4;      /*SFP*/
	    float	  fRx_PWR3;      /*SFP*/
		float	  fRx_PWR2;      /*SFP*/
		float  	  fRx_PWR1;      /*SFP*/
		float  	  fRx_PWR0;      /*SFP*/
		ushort_t  usTx_I_Slope;  /*SFP*/
		short 	  sTx_I_Offset;  /*SFP*/
		ushort_t  usTx_PWR_Slope;/*SFP*/
		short 	  sTx_PWR_Offset;/*SFP*/
		ushort_t  usT_Slope;     /*SFP*/
		short 	  sT_Offset;     /*SFP*/
		ushort_t  usV_Slope;     /*SFP*/
		short  	  sV_Offset;     /*SFP*/
		ulong_t   reserve_2;     /*SFP*/
		short 	  sTemperature;  /*参数值*//*XFP_SFP*/
		ushort_t  usSupplyVoltage;/*SFP*/
		ushort_t  usBiasCurrent;  /*XFP_SFP*/
		ushort_t  usTxOutputPower;/*XFP_SFP*/
		ushort_t  usReceivedPower;/*XFP_SFP*/
		ushort_t  reserve_3[11];  /*SFP*/
			
		short	  sAux1;          /*XFP*/
	    short	  sAux2;          /*XFP*/
	    adpl_digitalDiagnoticSignedAlarm_t structAux1; /*参数Aux1域值*//*XFP*/
	    adpl_digitalDiagnoticSignedAlarm_t structAux2; /*参数Aux2域值*//*XFP*/
	} adpl_digitalDiagnoticA2_t;	
typedef struct ddmUserWavelengthInfo_s
{
	unsigned short  usUserWavelength;          /**用户设置的波长 **/
							/**作为get操作的输出参数，用户获取的波长值**/
	short sWavelengthError; /**波长偏差**/
							/**作为get操作的输出参数，波长偏差**/
} ddmUserWaveLengthInfo_t;
typedef struct{
	ulong_t ulPort;
}ddmAbsentChangeCallBack;
typedef struct
{
	unsigned short  usPortId;          /*端口号*/                      /*XFP_SFP*/
	unsigned long   ulIfIndex;          /*全局索引*/
	unsigned char   ucIdentifier;      /*物理类型，比如sfp、xfp等等*/  /*XFP_SFP*/
	unsigned short  usCalibrationType; /*校准类型：外部校准，内部校准，未知*/ /*XFP_SFP*/
	unsigned char   ucAUX1Type;        /*性能参数AUX1类型*/            /*XFP*/
	unsigned char   ucAUX2Type;        /*性能参数AUX2类型*/            /*XFP*/
	unsigned char   ucOpticalPowerType;/*光功率类型：AVERAGE或OAM*/    /*XFP*/
	unsigned short  usDdmSupport;      /*1为支持数字诊断,2为不支持数字诊断*/
	adpl_digitalDiagnoticA2_t digitalDiagnotic;/*域值和当前值*/       /*XFP_SFP*/
} adpl_digitalDiagnotic_t;
typedef struct
{
  unsigned short usUserWavelength;	
  short sWavelengthError;
} ddmUserWaveLengthInfo;
typedef struct
{
	unsigned short usPortId;    /*端口号*/
	unsigned long   ulIfIndex;          /*全局索引*/
	unsigned char ucIdentifier;
	unsigned char ucReserve_8;  
	unsigned char ucConnector;  
	unsigned char uc10GEComplianceCodes;  /*for SFP+*/
	unsigned char ucReserve_6[2];  
	unsigned char ucGEComplianceCodes;  
	unsigned char ucReserve_7[2];  
	unsigned char ucMedia;  
	unsigned char ucReserve_9[2];  
	unsigned char ucBR;  /*速率*/
	unsigned char ucReserve_1;
	unsigned char ucLength9km; /*传输距离*/
	unsigned char ucLength9m;
	unsigned char ucLength50;
	unsigned char ucLength625;
	unsigned char ucReserve_2;    
    	unsigned char ucLengthOM3;      /*for SFP+*/
    	unsigned char ucVendorName[16]; /*SFP 厂家名字*/
	unsigned char ucReserve_3[4];
	unsigned char ucVendorPN[16]; /*产品型号*/
	unsigned char ucVendorRev[4]; /*版本号*/
	unsigned char ucWaveLength[2]; /*波长*/
	unsigned char ucReserve_4[6];
	unsigned char ucVendorSN[16]; /*流水号*/	
	unsigned char ucReserve_5[9];
	unsigned char ucEnhancedOption;                                  /*增强选项*/

	/*XFP新增*/
	unsigned char ucBRMin;/*最小速率*/    
	unsigned char ucBRMax;/*最大速率*/  
	unsigned char ucLengthSMFkm;  
	unsigned char ucLengthE50m;  
	unsigned char ucLengthCopper; 
	unsigned char ucWaveLengthTolerance[2] ; /*波长容限*/	
	unsigned char ucCompatility [7]; /*业务类型*/
	unsigned char ucPower;/*最大功耗*/
	unsigned char ucCDR;/*CDR功能*/
	unsigned char ucCLk;/*外部参考时钟*/
	unsigned char ucLaserType;/*激光器类型*/
	unsigned char ucDetectorType;/*探测器类型*/
	unsigned char ucLoopbackLine;/*线路环回支持*/
	unsigned char ucLoopbackXFI;/*XFI环回支持*/

} adpl_digitalDiagnoticAdditionalInfo_t;

/* BEGIN: Added by Hushouqiang, 2016/12/15   问题单号:UTN-TEST-20161215-1 */
typedef struct adpl_PlatformCustomOperations_s {
  int (*PlatCustomPortNameGet)( int port, char *pName ); /* Get port name */
  int (*PlatCustomPortMacGet)( int port,char *pMac ); /* Get the mac address of specified port */
}adpl_PlatformCustomOperations_t;


/* END:   Added by Hushouqiang, 2016/12/15 */





typedef struct adpl_DeviceOperations_s {
  
  int (*DevInfoDeviceSerialNumberGet)( char *pSN ); /*获取设备序列号*/
  int (*DevInfoDeviceModelGet)( char *pModel );
  int (*DevInfoHardwareVersionGet)( char *pHwVer );
  int (*DevInfoFirmwareVersionGet)( char *pFwVer );
  int (*DevInfoDeviceMfiVendorGet)( char *pVendor );
  int (*DevInfoDeviceMacAddressGet)( adplMacAddr_t *  mac  );
  int (*BpDevInfoDeviceSerialNumberGet)( char *pSN ); /*获取设备序列号*/
  int (*BpDevInfoDeviceModelGet)( char *pModel );
  int (*BpDevInfoHardwareVersionGet)( char *pHwVer );
  int (*BpDevInfoFirmwareVersionGet)( char *pFwVer );
  int (*BpDevInfoDeviceMfiVendorGet)( char *pVendor );
  int (*BpDevInfoDeviceMacAddressGet)( adplMacAddr_t *  mac  );
  int (*DevInfoDeviceTransmitDelayGet)( uint32_t *pDelay );
  int (*DevInfoSlotGetNext)(uint32_t SlotIndex , uint32_t *pSlotNext );
  int (*DevInfoSlotNameGet)(uint32_t SlotIndex , char *pSlotName );
  int (*DevInfoSlotTypeGet)(uint32_t SlotIndex , char *pSlotType );
  int (*DevInfoSlotStatusGet)(uint32_t SlotIndex , uint32_t *pSlotStatus );
  int (*DevInfoSlotSerialNumGet)(uint32_t SlotIndex , char *pSlotSerialNum );
  int (*DevInfoSlotHardwareRevisionGet)(uint32_t SlotIndex , char *pSlotHwRev );
  int (*DevInfoSlotFirmwareRevisionGet)(uint32_t SlotIndex , char *pSlotFwRev );
  int (*DevInfoSlotCapablityGet)(uint32_t SlotIndex , char *pSlotCapablity );
  void (*SystemReboot)(void);
  int (*RtcTimeSet)(uint32_t tv_sec);/*yanshi add:20160901 添加风扇支持 */
  int (*DevMonFanSpeedGet)(int fanIndex , int* pfanSpeed );
  int (*DevMonFanDataGet)(uint32_t fanIndex , adpl_fanCurDataInfo_t* pdata );
  int (*DevMonFanLedSet)(int fanIndex , int ledData );
  int (*DevMonFanSerialNumberGet)(int fanIndex ,  char *pSN );
  int (*DevMonFanSerialNumberSet)(int fanIndex ,  char *pSN );
  int (*DevMonFanSpeedLevelSet)(int fanIndex , int fanSpeedLvl );
  int (*DevMonFanCardStatusGet)(int fanIndex , int* pstatus );/**yanshi add:20160901 温度监控**/
  int (*DevMonTempGet)(int Index , int* ptemp );
  int (*DevMonTempThresholdHighGet)(int Index , int* ptemp );
  int (*DevMonTempThresholdHighSet)(int Index , int temp );
  int (*DevMonTempThresholdLowGet)(int Index , int* ptemp );
  int (*DevMonTempThresholdLowSet)(int Index , int temp );
  /**yanshi add:20160901 电压监控**/
  int (*DevMonVoltGet)(int Index , int* pVolt );
  int (*DevMonVoltThresholdHighGet)(int Index , int* pVolt ); 
  int (*DevMonVoltThresholdLowGet)(int Index , int* pVolt ); 
  int (*DevMonVoltThresholdHighSet)(int Index , int* pVolt );
  int (*DevMonVoltThresholdLowSet)(int Index , int* pVolt );
  /**yanshi add 20160927 ddm 测试**/
  int (*DevDDMGet)(int Index , adpl_digitalDiagnoticAdditionalInfo_t * pDdm );
  /**yanshi add 20161019  ddm 分接口实现**/
  int (*DevTransceiverPwdInfo)(int Index , adpl_transceiverPwdInfo_t *pwdInfo);
  int (*DevDigitalDiagnoticAddition)(ulong_t Index,  adpl_digitalDiagnoticAdditionalInfo_t  *addition);      
  int (*DevDigitalDiagnotic)(ulong_t ulIfIndex,  adpl_digitalDiagnotic_t  *digitalDiagnotic);
  int (*DevTransceiverConnectType)(ulong_t ulIfIndex,  ushort_t*  usConnectType);
  int (*DevPortTransceiverPowerDown)(ADPL_MODE mode, ulong_t ulIfIndex,  ushort_t*  usPowerDownStatus);
  int (*DevTransceiverLaser)(ADPL_MODE mode, ulong_t ulIfIndex,  ushort_t*  usLaserStatus);
  int (*DevPortTransceiverAbsent)(ulong_t ulIfIndex,  ushort_t*  usAbsStatus);
  int (*DevPortTransceiverReady)(ulong_t ulIfIndex,  ushort_t*  usNRStatus); /*2.8*/
  int (*DevTransceiverRXLos)(ulong_t ulIfIndex,  ushort_t*  usLaserStatus);
  int (*DevTransceiverWaveLength)(ADPL_MODE mode, ulong_t ulIfIndex,  ddmUserWaveLengthInfo*  UserWaveLengthInfo);/*2.10**/
  int (*DevTransceiverDataRate)(ADPL_MODE mode, ulong_t ulIfIndex,  ushort_t*  usDataRate);/**2.11**/
  int (*DevPortTransceiverLoopbackLine)(ADPL_MODE mode, ulong_t ulIfIndex,  ushort_t*  usLoopbackLineStatus);/**2.12**/
  int (*DevPortTransceiverLoopbackXfi)(ADPL_MODE mode, ulong_t ulIfIndex,  ushort_t*  usLoopbackXFIStatus);/**2.13**/
  int (*DevPortTransceiverABSChangeCallback)(ushort_t index, ddmAbsentChangeCallBack *absentChangeCallback);/*2.14*/
  int (*DevPvTransceiverTxFault)(ulong_t cmd, ulong_t ifindex, ulong_t value);/*2.15*/
  int (*DevHwPortValueGet)(ulong_t cmd, ulong_t ifindex, ulong_t value);/*2.16*/
  int (*DevSetPollSet)(ulong_t enable);/*2.17*/
  int (*DevSetSfpTx)(ulong_t ulSnmpPort, ulong_t enable);/*2.18*/
    /* 添加manufacturer mac地址  Added by yanshi, 2017/2/16 */
  int (*DevManufacture)(adplManuFunctureAll_t *pManufacture);
  int (*DevPortMacPlus1Get)( unsigned char *pucMac);
  int (*DevInfoDeviceMac3rdAddressGet)( unsigned char *pucMac);
  /* 添加接口管理  Added by jiaowb, 2017/3/11 */
  int (*DevPortEnableSet)( ulong_t ulIfindex,  ulong_t value);
  int (*DevPortPhySpeedSet)( ulong_t ulIfindex,  ulong_t value);
  int (*DevPortPhyDuplexSet)( ulong_t ulIfindex,  ulong_t value);
  int (*DevPortJumboFrameSet)( ulong_t ulIfindex,  ulong_t value);
}adpl_DeviceOperations_t;

typedef struct adpl_boot_cfg
{
	unsigned int	lFlag;		/* RC_BOOT_CFG_FLAG */
	unsigned int	boottype;	/*1: ptn system; 2: uboot*/
	unsigned char	ip[4];		/*example: 0xc0a804d7  --> 192.168.4.215 */
	unsigned char	ipMask[4];
	unsigned char	gateway[4];
}ADPL_BOOT_CFG;

struct cxn_state{
    unsigned int id;            /** controller id*/
    unsigned int state;         /** controller state*/
};

typedef int (*ADPL_cxn_status_change_f)(struct cxn_state *pstate);
typedef int (*ADPL_cxn_echo_timeout_preprocess_f)(struct cxn_state *pstate);

/* For qos*/
typedef enum
{
  ADPL_QOS_QUEUE_SCHED_MODE_NONE                  =     0x00,       /* Pass through. */
  ADPL_QOS_QUEUE_SCHED_MODE_SP                    =     0x01,       /* Weights are ignored. */
  ADPL_QOS_QUEUE_SCHED_MODE_RR                    =     0x02,       /* Weights are ignored. */
  ADPL_QOS_QUEUE_SCHED_MODE_WRR                   =     0x04,       
  ADPL_QOS_QUEUE_SCHED_MODE_WFQ 				  = 	0x08,		
  ADPL_QOS_QUEUE_SCHED_MODE_WDRR                  =     0x10,       
} ADPL_QOS_QUEUE_SCHED_MODE_t;

typedef enum
{
  ADPL_PORT_DISCARD_MODE_TD                       =     1,
  ADPL_PORT_DISCARD_MODE_WRED                     =     2,
}ADPL_PORT_DISCARD_MODE_t;

typedef enum
{
  ADPL_COSQ_DISCARD_COLOR_GREEN  = 1,
  ADPL_COSQ_DISCARD_COLOR_YELLOW  = 2,
  ADPL_COSQ_DISCARD_COLOR_RED    = 3
}ADPL_COSQ_DISCARD_COLOR_enum;


ADPL_ERROR_t  ADPL_CsfDisable(uint32_t lmepId);
ADPL_ERROR_t  ADPL_CsfEnable(uint32_t lmepId, adplOamCsfConfig_t *config);
ADPL_ERROR_t  ADPL_OamAisConfigGet(uint32_t lmepId, adplOamAisConfig_t *config);
ADPL_ERROR_t  ADPL_OamAisDisable(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamAisEnable(uint32_t lmepId, adplOamAisConfig_t *config);
ADPL_ERROR_t  ADPL_OamLbGet(  uint32_t lmepId,
									   adplOamLbConfig_t *config,
									   adplOamLbStatus_t *status );
ADPL_ERROR_t  ADPL_OamLbStop(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamLckConfigGet(uint32_t lmepId, adplOamLckConfig_t *config);
ADPL_ERROR_t  ADPL_OamLckDisable(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamLckEnable(uint32_t lmepId, adplOamLckConfig_t *config);
ADPL_ERROR_t  ADPL_OamLtGet(uint32_t lmepId, 
									   adplOamLtConfig_t *ltConfig, 
									   adplOamLtStatus_t *ltStatus);
ADPL_ERROR_t  ADPL_OamLtStart (uint32_t lmepId, adplOamLtConfig_t *config);
ADPL_ERROR_t  ADPL_OamMegCreate (uint32_t megIndex, adplOamMegConfig_t *config);
ADPL_ERROR_t  ADPL_OamMegModify (uint32_t megIndex, adplOamMegConfig_t *config);
ADPL_ERROR_t  ADPL_OamMegDelete(uint32_t megIndex);
ADPL_ERROR_t  ADPL_OamMegGet(uint32_t megIndex,
									 adplOamMegConfig_t *config,
									 adplOamMegStatus_t *status);
ADPL_ERROR_t  ADPL_OamMepCreate(uint32_t lmepId, adplOamMepConfig_t *config);
ADPL_ERROR_t  ADPL_OamMepModify(uint32_t lmepId, adplOamMepConfig_t *config);
ADPL_ERROR_t  ADPL_OamMepDelete(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamMepGet(uint32_t lmepId,
									 adplOamMepConfig_t *config,
									 adplOamMepStatus_t *status);
ADPL_ERROR_t   ADPL_OamMepProtectionAdd(uint32_t mlpGroupIndex, uint32_t lmepId,
                                        adplMlpheadendcfg_t *mlpcfg);
ADPL_ERROR_t  ADPL_OamMepProtectionGet(uint32_t mlpGroupIndex, uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamMepProtectionRemove(uint32_t mlpGroupIndex, uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamMipCreate(uint32_t localMpId, adplOamMipConfig_t *config);
ADPL_ERROR_t  ADPL_OamMipDelete(uint32_t localMpId);
ADPL_ERROR_t  ADPL_OamMipGet(uint32_t localMpId,
									 adplOamMipConfig_t *config,
									 adplOamMipStatus_t *status);
ADPL_ERROR_t ADPL_OamMepCCMDatabaseEntryNextGet(uint32_t lmepId,
                                                 uint32_t remoteMepId,
                                                 uint32_t *nextRemoteMepId);
ADPL_ERROR_t ADPL_OamMepCCMDatabaseEntryGet(uint32_t lmepId,
                                             uint32_t remoteMepId,
                                             adplOamCcmDatabaseEntry_t *dbEntry);
ADPL_ERROR_t  ADPL_OamMLPGroupCreate(uint32_t index, adplOamMLPGroupConfig_t *config);
ADPL_ERROR_t  ADPL_OamMLPGroupDelete(uint32_t index);
ADPL_ERROR_t  ADPL_OamMLPGroupGet(uint32_t index,
										  adplOamMLPGroupConfig_t *config,
										  adplOamMLPGroupStatus_t *status);
ADPL_ERROR_t  ADPL_OamMLPGroupRequest(uint32_t index, ADPL_PROTECTION_SWITCH_REQUEST_TYPE_t request);
ADPL_ERROR_t  ADPL_OamOamLbCreate(uint32_t lmepId, adplOamLbConfig_t *config);
ADPL_ERROR_t  ADPL_OamOdDmCreate(uint32_t lmepId, adplOamOdDmConfig_t *config);
ADPL_ERROR_t  ADPL_OamOdDmDelete(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamOdDmGet(uint32_t lmepId,
											   adplOamOdDmConfig_t *config, 
											   adplOamOdDmStatus_t *status);
ADPL_ERROR_t  ADPL_OamOdDmStart(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamOdDmStop(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamOdLmCountersGet(uint32_t lmepId, 
											   adplOamOdLmSnapshot_t *odLmCounters);
ADPL_ERROR_t  ADPL_OamOdLmCreate(uint32_t lmepId, adplOamOdLmConfig_t *config);
ADPL_ERROR_t  ADPL_OamOdLmDelete(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamOdLmGet(uint32_t lmepId,
											   adplOamOdLmConfig_t *config, 
											   adplOamOdLmStatus_t *status);
ADPL_ERROR_t  ADPL_OamOdLmStart(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamOdLmStop(uint32_t lmepId);
ADPL_ERROR_t  ADPL_OamRemoteMpCreate(uint32_t remoteMpId,
													   adplOamRemoteMpConfig_t *config);
ADPL_ERROR_t  ADPL_OamRemoteMpDelete(uint32_t remoteMpId);
ADPL_ERROR_t  ADPL_OamRemoteMpGet(uint32_t remoteMpId,
										   adplOamRemoteMpConfig_t *config);


ADPL_ERROR_t ADPL_PortSpeedSet(unsigned int ulIfindex , ADPL_PORT_SPEED_t speed);
ADPL_ERROR_t ADPL_PortDuplexSet(unsigned int ulIfindex , ADPL_PORT_DUPLEX_t duplex);
ADPL_ERROR_t ADPL_PortJumboFrameSet(unsigned int ulIfindex , unsigned int JumboFrame);
ADPL_ERROR_t  ADPL_PortAdminStatusGet ( unsigned int portNum , ADPL_PORT_ADMIN_t*  status );
ADPL_ERROR_t  ADPL_PortCurrSpeedGet ( unsigned int portNum , unsigned int *currSpeed);
ADPL_ERROR_t  ADPL_PortDescriptionGet ( unsigned int portNum , char* descp );
ADPL_ERROR_t  ADPL_PortDescriptionSet ( unsigned int portNum , char* descp );
ADPL_ERROR_t  ADPL_PortDisable ( unsigned int portNum );
ADPL_ERROR_t  ADPL_PortEnable ( unsigned int portNum );
ADPL_BOOL  ADPL_PortIsEnabled ( unsigned int portNum );
time_t	ADPL_PortLastChgTSGet ( unsigned int portNum , time_t*	ts );
ADPL_ERROR_t  ADPL_PortLinkUpDwnTrapDisable ( unsigned int portNum);
ADPL_ERROR_t  ADPL_PortLinkUpDwnTrapEnable ( unsigned int portNum  );
ADPL_ERROR_t  ADPL_PortMacGet ( unsigned int portNum , adplMacAddr_t *	mac );
int ADPL_PortName2Num ( char*  name);
ADPL_ERROR_t  ADPL_PortNameGet ( unsigned int portNum , char* name );
ADPL_ERROR_t  ADPL_PortNameSet( unsigned int portNum , char* name );
ADPL_ERROR_t  ADPL_PortOperStatusGet ( unsigned int portNum , ADPL_PORT_OP_STATUS_t*  status);
ADPL_ERROR_t  ADPL_PortStateGet ( unsigned int portNum , ADPL_PORT_STATE_t*  state );
ADPL_ERROR_t  ADPL_PortStatsClear ( unsigned int portNum);
ADPL_ERROR_t  ADPL_PortStatsGet ( unsigned int portNum , adplPortStats_t * stats);
ADPL_ERROR_t  ADPL_PortTypeGet ( unsigned int portNum , ADPL_PORT_TYPE_t* type );
ADPL_ERROR_t  ADPL_PortTypeSet ( unsigned int portNum , ADPL_PORT_TYPE_t  type );

ADPL_ERROR_t  ADPL_PortQueueMinMaxRateGet(uint32_t portNum, uint32_t queueId, uint32_t *minrate, uint32_t *maxrate);
ADPL_ERROR_t  ADPL_PortQueueIdIsValid(uint32_t portNum, uint32_t queueId);
ADPL_ERROR_t  ADPL_PortQueueMinMaxRateSet (uint32_t portNum, uint32_t queueId, uint32_t minrate, uint32_t maxrate);
ADPL_ERROR_t ADPL_PortNumQueuesGet(uint32_t portNum, uint32_t *numQueues);

ADPL_ERROR_t  ADPL_CosQueueMinMaxRateSet (uint32_t portNum, uint32_t queueId, uint32_t minrate, uint32_t maxrate);
ADPL_ERROR_t  ADPL_CosQueueMinMaxRateGet(uint32_t queueId, uint32_t *portNum, uint32_t *minrate, uint32_t *maxrate);

ADPL_ERROR_t  ADPL_PortQueueSchedSet(uint32_t port, int32_t mode);
ADPL_ERROR_t  ADPL_PortQueueSchedGet(uint32_t port, int32_t *mode);
ADPL_ERROR_t  ADPL_PortQueueWeightsSet(uint32_t port, int32_t *weights);
ADPL_ERROR_t  ADPL_PortQueueWeightModify(uint32_t port,uint32_t queueId, int32_t weight);

ADPL_ERROR_t  ADPL_PortQueueWeightsGet(uint32_t port, int32_t *weights);
ADPL_ERROR_t  ADPL_PortDiscardModeSet(uint32_t mode);
ADPL_ERROR_t  ADPL_PortDiscardModeGet(uint32_t *mode);
ADPL_ERROR_t  ADPL_PortDiscardConfig(uint32_t port, uint32_t cosq,uint32_t color,
  int drop_start, int drop_slope,int drop_end);
ADPL_ERROR_t  ADPL_PortDiscardConfigGet(uint32_t port, uint32_t cosq,uint32_t color,
  int *drop_start, int *drop_slope,int *drop_end);


ADPL_ERROR_t  ADPL_SysMachineVersionGet (char* version );
ADPL_ERROR_t  ADPL_SysOsVersionGet (char * version );

ADPL_ERROR_t ADPL_OfagentDpidSet(adplDpid_t* dpid);
ADPL_ERROR_t ADPL_OfagentDpidGet(adplDpid_t* dpid);
ADPL_ERROR_t ADPL_OfagentCxnAdd(adplAgentCxnParam_t * cfg ,uint32_t *of_cxn_id);
ADPL_ERROR_t ADPL_OfagentCxnRemove(uint32_t of_cxn_id);
ADPL_ERROR_t ADPL_OfagentCxnStatusGet(uint32_t cxn_id, adpl_ofCxnStatus_t * status);

ADPL_ERROR_t regControllerStateChangeInvokeAdd( ADPL_cxn_status_change_f *func );
ADPL_ERROR_t regControllerStateChangeInvokeDelete( ADPL_cxn_status_change_f *func );
ADPL_ERROR_t ADPL_PlatCustomOpsRegister(adpl_PlatformCustomOperations_t * ops);

#if 1

/*****************************************************************************
 函 数 名  : ADPL_DevInfoDeviceSerialNumberGet
 功能描述  : 获取设备序列号
 输入参数  : char *pSN  用于返回查询结果
 输出参数  : char *pSN
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoDeviceSerialNumberGet( char *pSN );

/*****************************************************************************
 函 数 名  : ADPL_DevInfoDeviceModelGet
 功能描述  : 获取设备的Model信息
 输入参数  : char *pModel 用于返回查询结果
 输出参数  : char *pModel
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoDeviceModelGet( char *pModel );

/*****************************************************************************
 函 数 名  : ADPL_DevInfoHardwareVersionGet
 功能描述  : 获取设备硬件版本
 输入参数  : char *pHwVer
 输出参数  : char *pHwVer
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoHardwareVersionGet( char *pHwVer );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoFirmwareVersionGet
 功能描述  : 获取设备固件版本
 输入参数  : char *pFwVer
 输出参数  : char *pFwVer
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoFirmwareVersionGet( char *pFwVer );



/*****************************************************************************
 函 数 名  : ADPL_DevInfoDeviceMfiVendorGet
 功能描述  : 获取设备制造商
 输入参数  : char *pVendor
 输出参数  : char *pVendor
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoDeviceMfiVendorGet( char *pVendor );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoDeviceMacAddressGet
 功能描述  : 获取设备MAC地址
 输入参数  : adplMacAddr_t *  mac 
 输出参数  : adplMacAddr_t *  mac
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoDeviceMacAddressGet( adplMacAddr_t *  mac  );

/*****************************************************************************/
ADPL_ERROR_t ADPL_BpDevInfoDeviceSerialNumberGet( char *pSN );

/*****************************************************************************
 函 数 名  : ADPL_DevInfoDeviceModelGet
 功能描述  : 获取设备的Model信息
 输入参数  : char *pModel 用于返回查询结果
 输出参数  : char *pModel
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_BpDevInfoDeviceModelGet( char *pModel );

/*****************************************************************************
 函 数 名  : ADPL_DevInfoHardwareVersionGet
 功能描述  : 获取设备硬件版本
 输入参数  : char *pHwVer
 输出参数  : char *pHwVer
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_BpDevInfoHardwareVersionGet( char *pHwVer );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoFirmwareVersionGet
 功能描述  : 获取设备固件版本
 输入参数  : char *pFwVer
 输出参数  : char *pFwVer
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_BpDevInfoFirmwareVersionGet( char *pFwVer );



/*****************************************************************************
 函 数 名  : ADPL_DevInfoDeviceMfiVendorGet
 功能描述  : 获取设备制造商
 输入参数  : char *pVendor
 输出参数  : char *pVendor
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_BpDevInfoDeviceMfiVendorGet( char *pVendor );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoDeviceMacAddressGet
 功能描述  : 获取设备MAC地址
 输入参数  : adplMacAddr_t *  mac 
 输出参数  : adplMacAddr_t *  mac
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_BpDevInfoDeviceMacAddressGet( adplMacAddr_t *  mac  );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoDeviceTransmitDelayGet
 功能描述  : 获取设备传输延迟，单位us
 输入参数  : uint8_t *pDelay
 输出参数  : uint8_t *pDelay
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoDeviceTransmitDelayGet( uint32_t *pDelay );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoSlotGetNext
 功能描述  : 获取设备的下一个槽位号
 输入参数  : uint32_t SlotIndex  当前槽位索引
 输出参数  : uint32_t *pSlotNext
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月27日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoSlotGetNext(uint32_t SlotIndex , uint32_t *pSlotNext );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoSlotNameGet
 功能描述  : 获取设备槽位板卡名称
 输入参数  : uint32_t SlotIndex  槽位索引
 输出参数  : char *pSlotName
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoSlotNameGet(uint32_t SlotIndex , char *pSlotName );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoSlotTypeGet
 功能描述  : 获取设备槽位板卡类型
 输入参数  : uint32_t SlotIndex  槽位索引
 输出参数  : char *pSlotType
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoSlotTypeGet(uint32_t SlotIndex , char *pSlotType );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoSlotStatusGet
 功能描述  : 获取设备槽位板卡状态
 输入参数  : uint32_t SlotIndex  槽位索引
 输出参数  : uint32_t *pSlotStatus
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoSlotStatusGet(uint32_t SlotIndex , uint32_t *pSlotStatus );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoSlotSerialNumGet
 功能描述  : 获取设备槽位板卡序列号
 输入参数  : uint32_t SlotIndex  槽位索引
 输出参数  : char *pSlotSerialNum
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoSlotSerialNumGet(uint32_t SlotIndex , char *pSlotSerialNum );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoSlotHardwareRevisionGet
 功能描述  : 获取设备槽位板卡硬件版本
 输入参数  : uint32_t SlotIndex  槽位索引
 输出参数  : char *pSlotHwRev
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoSlotHardwareRevisionGet(uint32_t SlotIndex , char *pSlotHwRev );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoSlotFirmwareRevisionGet
 功能描述  : 获取设备槽位板卡固件版本
 输入参数  : uint32_t SlotIndex  槽位索引
 输出参数  : char *pSlotFwRev
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoSlotFirmwareRevisionGet(uint32_t SlotIndex , char *pSlotFwRev );


/*****************************************************************************
 函 数 名  : ADPL_DevInfoSlotCapablityGet
 功能描述  : 获取设备槽位板卡支持的功能描述
 输入参数  : uint32_t SlotIndex  槽位索引
 输出参数  : char *pSlotCapablity
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DevInfoSlotCapablityGet(uint32_t SlotIndex , char *pSlotCapablity );


#endif


#if 1	/*leishenghua add 20160624 for inband manage*/

/*****************************************************************************
 函 数 名  : ADPL_InbandMngAdd
 功能描述  : 配置带内管理VLAN和接口
 输入参数  : int vlan 				管理VLAN
				unsiged char ip[4]		管理IP, for example: 0xc0,0xa8,0x1,0xfe --> 192.168.1.254
				unsigned char ipMask[4]   netmask;  for example: 0xff,0xff,0xff,0 -->255.255.255.0
 输出参数  : 			
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/

ADPL_ERROR_t ADPL_InbandMngAdd(int vlan, unsigned char ip[4], unsigned char ipMask[4]);


/*****************************************************************************
 函 数 名  : ADPL_InbandMngAdd
 功能描述  : 删除带内管理VLAN和接口
 输入参数  : int vlan 				管理VLAN
 输出参数  : unsiged char ip[4]		管理IP, for example: 0xc0,0xa8,0x1,0xfe --> 192.168.1.254
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_InbandMngDelete(int vlan, unsigned char ip[4]);


/*****************************************************************************
 函 数 名  : ADPL_OamOdDmCreate
 功能描述  : 创建OnDemand DM测试实例。
 输入参数  : uint32_t lmepId               
             adplOamOdDmConfig_t *config  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月19日
    作    者   : HuShouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t  ADPL_OamOdDmSet(uint32_t lmepId, adplOamOdDmConfig_t *config);


/*****************************************************************************
 函 数 名  : ADPL_OamOdDmCountersGet
 功能描述  : 获取OnDemand DM测试结果。
 输入参数  : uint32_t lmepId                       
             adplOamOdDmSnapshot_t *odDmCounters  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月19日
    作    者   : HuShouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t  ADPL_OamOdDmCountersGet(uint32_t lmepId, 
										adplOamOdDmSnapshot_t *odDmCounters);


/*****************************************************************************
 函 数 名  : ADPL_OamOdLmCreate
 功能描述  : 创建OnDemand LM测试实例。
 输入参数  : uint32_t lmepId               
             adplOamOdLmConfig_t *config  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月19日
    作    者   : HuShouqiang
    修改内容   : 新生成函数

*****************************************************************************/

#endif

ADPL_ERROR_t ADPL_PortNextGet(unsigned int portNum, unsigned int *nextPortNum);
ADPL_ERROR_t ADPL_ManagmentPortAdd(unsigned int portId,unsigned int ulIpAddr,unsigned int ulIpMask);
ADPL_ERROR_t ADPL_PortDelete(unsigned int portId);
ADPL_BOOL ADPL_IsManagmentPortCfg(void);


/*****************************************************************************
 函 数 名  : ADPL_PktSend
 功能描述  : ADPL层报文发送接口
 输入参数  : pkt
             flags
             outPortNum
             inPortNum (used in loopback mode)
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月12日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_PktSend(adpl_buffdesc *pkt, uint32_t flags,
                                   uint32_t outPortNum, uint32_t inPortNum);

ADPL_ERROR_t ADPL_PktReceive(struct timeval *timeout, adplPacket_t *pkt);
ADPL_ERROR_t ADPL_OfPktRecvRegister(ADPL_pkt_ready_callback_f f);
ADPL_ERROR_t ADPL_OfPktRecvUnRegister(void);


/*****************************************************************************
 函 数 名  : ADPL_reconnectPktCallbackRegister
 功能描述  : 重新连接确认报文处理函数注册
 输入参数  : cbFunction 回调函数
 输出参数  : 无
 返 回 值  : 
 修改历史      :
  1.日    期   : 2016年10月26日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_reconnectPktCallbackRegister(adplPacketCbFunction cbFunction);


/*****************************************************************************
 函 数 名  : ADPL_reconnectPktCallbackUnregister
 功能描述  : 重新连接确认报文处理函数解注册
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 修改历史      :
  1.日    期   : 2016年10月26日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_reconnectPktCallbackUnregister(void);




/*****************************************************************************
 函 数 名  : ADPL_RtcTimeSet
 功能描述  : Set RTC time
 输入参数  : uint32_t tv_sec  (自初始时间为00:00:00 01/01/1970以来的秒值)
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_RtcTimeSet(uint32_t tv_sec);

/*****************************************************************************
 函 数 名  : ADPL_SystemReboot
 功能描述  : 执行系统软复位
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
void ADPL_SystemReboot(void);

/*****************************************************************************
 函 数 名  : ADPL_EventCallbackRegister
 功能描述  : 注册驱动事件回调函数
 输入参数  : ADPL_EventCallback *func  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_EventCallbackRegister(ADPL_EventCallback *func );
/**yansh add 数字诊断告警 **/
ADPL_ERROR_t ADPL_SfpCallbackRegister(ADPL_EventCallback *func );

/*****************************************************************************
 函 数 名  : ADPL_OamEventCallbackRegister
 功能描述  : 注册OAM驱动事件回调函数
 输入参数  : ADPL_EventCallback *func  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年8月9日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_OamEventCallbackRegister(ADPL_EventCallback *func );

/*****************************************************************************
 函 数 名  : ADPL_ApsEventCallbackRegister
 功能描述  : 注册APS驱动事件回调函数
 输入参数  : ADPL_EventCallback *func  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月16日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_ApsEventCallbackRegister(ADPL_EventCallback *func );

/*****************************************************************************
 函 数 名  : ADPL_PowerDownEventCallbackRegister
 功能描述  : 注册powerdown 驱动事件回调函数
 输入参数  : ADPL_EventCallback *func  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年8月26日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_PowerDownEventCallbackRegister(ADPL_EventCallback *func );


/*****************************************************************************
 函 数 名  : ADPL_DhcpCpuEnable
 功能描述  :  DHCP报文上送CPU 配置
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpCpuEnable(void);


/*****************************************************************************
 函 数 名  : ADPL_DhcpCpuEnable
 功能描述  :  DHCP报文上送CPU 禁止
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpCpuDisable(void);


/*****************************************************************************
 函 数 名  : ADPL_DhcpCpuEnableByPort
 功能描述  :  DHCP报文上送CPU 配置
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpCpuEnableByPort(uint32_t inport);


/*****************************************************************************
 函 数 名  : ADPL_DhcpCpuDisableByPort
 功能描述  :  DHCP报文上送CPU 禁止
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpCpuDisableByPort(uint32_t inport);

/*****************************************************************************
 函 数 名  : ofdpaFlowDhcpReConnectAdd
 功能描述  : 重新连接报文上送CPU 配置
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpReconnectCpuEnable(void);


/*****************************************************************************
 函 数 名  : ADPL_DhcpCpuEnable
 功能描述  :   重新连接报文上送CPU 禁止
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpReconnectCpuDisable(void);


/*****************************************************************************
 函 数 名  : ADPL_DhcpVlanPortAdd
 功能描述  :  端口加入VLAN
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpVlanPortAdd(uint32_t port, uint32_t vlan);


/*****************************************************************************
 函 数 名  : ADPL_DhcpVlanPortDel
 功能描述  :  从VLAN中移除端口
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpVlanPortDel(uint32_t port, uint32_t vlan);


/*****************************************************************************
 函 数 名  : ADPL_DhcpVlanPortDelAll
 功能描述  :  从VLAN中移除所有端口
 返 回 值  : ADPL_ERROR_tg
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpVlanPortDelAll(uint32_t vlan);



/*****************************************************************************
 函 数 名  : ADPL_DhcpVlanPortGet
 功能描述  :  查询端口的管理VLAN
 参数：	
 				vlan 管理VLAN
 返 回 值  : ADPL_ERROR_t ： ADPL_E_NONE 说明查询成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpVlanPortGet(uint32_t port, uint32_t *vlan);



/*****************************************************************************
 函 数 名  : ADPL_DhcpVlanPortNextGet
 功能描述  :  配置了管理VLAN的下一个接口
 参数：
 			nextPort 下一个接口
 返 回 值  : ADPL_ERROR_t ： ADPL_E_NONE 说明查询成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :


*****************************************************************************/
ADPL_ERROR_t ADPL_DhcpVlanPortNextGet(uint32_t port, uint32_t *nextPort);



/*****************************************************************************
 函 数 名  : ADPL_BfdSessionCreate
 功能描述  : 创建BFD实例。
 输入参数  : uint32_t lmepId              
             adplBfdConfig_t *config  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月31日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t  ADPL_BfdSessionCreate(uint32_t lmepId, adplBfdConfig_t *config);


/*****************************************************************************
 函 数 名  : ADPL_BfdSessionDelete
 功能描述  : 删除BFD 实例。
 输入参数  : uint32_t lmepId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月31日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t  ADPL_BfdSessionDelete(uint32_t lmepId);


/*****************************************************************************
 函 数 名  : ADPL_BfdSessionGet
 功能描述  : 获取BFD 配置与状态。
 输入参数  : uint32_t lmepId                       
 输出参数  :  adplBfdConfig_t *config  
 				adplBfdStatus_t *status
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月31日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t  ADPL_BfdSessionGet(uint32_t lmepId,
                             adplBfdConfig_t *config,
                             adplBfdStatus_t *status);


/*****************************************************************************
 函 数 名  : ADPL_bfdEventCallbackRegister
 功能描述  : 注册BFD 驱动事件回调函数
 输入参数  : ADPL_EventCallback *func  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
ADPL_ERROR_t ADPL_bfdEventCallbackRegister(ADPL_EventCallback *func );


/*****************************************************************************
 函 数 名  : ADPL_PortNniUniTypeGet
 功能描述  : 获取端口uni/nni 类型信息
 输入参数  : unsigned int portNum                 			 
 输出参数  : ADPL_PORT_TYPE_t* nni : 1 上联口，0 下联口
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :

*****************************************************************************/
ADPL_ERROR_t  ADPL_PortNniUniTypeGet ( uint32_t portNum , uint32_t* nni );



/*****************************************************************************
 函 数 名  : ADPL_DeviceOpsRegister
 功能描述  : 不同产品注册产品自身特有的操作接口
 输入参数  : adpl_DeviceOperations_t *ops  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年8月22日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_DeviceOpsRegister(adpl_DeviceOperations_t *ops);
/*****************************************************************************
 函 数 名  : ADPL_SystemStartupTimeGet
 功能描述  : Get system startup time
 输入参数  : uint32_t time  (自初始时间为00:00:00 01/01/1970以来的秒值)
 输出参数  : uint32_t (自初始时间为00:00:00 01/01/1970以来的秒值)
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月1日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
uint32_t ADPL_SystemStartupTimeGet(uint32_t *time);

/*****************************************************************************
 函 数 名  : ADPL_SystemBootModeSet
 功能描述  : 设置系统下次启动的模式，传统模式或SDN模式
 输入参数  : int mode  1 = VxWorks , 2 = Linux
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月27日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_SystemBootModeSet(int mode);

/*****************************************************************************
 函 数 名  : ADPL_SystemBootModeSet
 功能描述  : 获取系统下次启动的模式
 输入参数  : int *mode  1 = VxWorks , 2 = Linux
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月27日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_SystemBootModeGet(int *mode);

/*****************************************************************************
 函 数 名  : ADPL_SystemBootParamConfig
 功能描述  : 系统启动参数配置
 输入参数  : ADPL_BOOT_CFG *cfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年8月1日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_SystemBootParamConfig(ADPL_BOOT_CFG *cfg);



/*****************************************************************************
 函 数 名  : ADPL_PwRedunCreate
 功能描述  : 创建PW redundancy保护组
 输入参数  : uint32_t index                    
             adplPwRedunConfig_t *config  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 

*****************************************************************************/
ADPL_ERROR_t  ADPL_PwRedunCreate(uint32_t index, adplPwRedunConfig_t *config);


/*****************************************************************************
 函 数 名  : ADPL_PwRedunDelete
 功能描述  : 删除指定的PW redundancy保护组。
 输入参数  : uint32_t index  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
ADPL_ERROR_t  ADPL_PwRedunDelete(uint32_t index);


/*****************************************************************************
 函 数 名  : ADPL_PwRedunGet
 功能描述  : 获取PW redundancy保护组配置。
 输入参数  : uint32_t index                    
             adplPwRedunConfig_t *config  
             adplPwRedunStatus_t *status  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 

*****************************************************************************/
ADPL_ERROR_t  ADPL_PwRedunGet(uint32_t index,
                                  adplPwRedunConfig_t *config,
                                  adplPwRedunStatus_t *status);




/*****************************************************************************
 函 数 名  : ADPL_BfdProtectionAdd
 功能描述  : 将MEP添加到指定的PW redundancy保护组中。
 输入参数  : uint32_t mlpGroupIndex  
             uint32_t lmepId         
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 


*****************************************************************************/
ADPL_ERROR_t	ADPL_BfdProtectionAdd(uint32_t mlpGroupIndex, uint32_t lmepId, 
                                         adplMlpheadendcfg_t *mlpcfg);

/*****************************************************************************
 函 数 名  : ADPL_BfdProtectionRemove
 功能描述  : 将MEP从指定的PW redundancy保护组上移除。
 输入参数  : uint32_t mlpGroupIndex  
             uint32_t lmepId         
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 

*****************************************************************************/
ADPL_ERROR_t  ADPL_BfdProtectionRemove(uint32_t mlpGroupIndex, uint32_t lmepId);


/*****************************************************************************
 函 数 名  : ADPL_BfdProtectionGet
 功能描述  : 检查MEP是否已添加到指定的PW redundancy中。
 输入参数  : uint32_t mlpGroupIndex  
             uint32_t lmepId         
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
ADPL_ERROR_t  ADPL_BfdProtectionGet(uint32_t mlpGroupIndex, uint32_t lmepId);


/*****************************************************************************
 函 数 名  : ADPL_pwRedunEventCallbackRegister
 功能描述  : 注册PW redundancy驱动事件回调函数
 输入参数  : ADPL_EventCallback *func  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 

*****************************************************************************/
ADPL_ERROR_t ADPL_pwRedunEventCallbackRegister(ADPL_EventCallback *func );


ADPL_ERROR_t ADPL_setPollSet(ulong_t enable);
ADPL_ERROR_t ADPL_digitalDiagnotic(ulong_t ulIfIndex,  adpl_digitalDiagnotic_t  *digitalDiagnotic);
ADPL_ERROR_t ADPL_portTransceiverAbsent(ulong_t ulIfIndex,  ulong_t*  usAbsStatus);
ADPL_ERROR_t ADPL_DevDDMGet(int  Index , adpl_digitalDiagnoticAdditionalInfo_t * pDdm);

ADPL_ERROR_t ADPL_DevMonVolGet(int Index , int * pVolt );
ADPL_ERROR_t ADPL_DevMonVoltThresholdHighGet(int Index , int* pVolt );
ADPL_ERROR_t ADPL_DevMonVoltThresholdLowGet(int Index , int* pVolt );
ADPL_ERROR_t ADPL_DevMonVoltThresholdHighSet(int Index , int* pVolt );
ADPL_ERROR_t ADPL_DevMonVoltThresholdLowSet(int Index , int* pVolt );

ADPL_ERROR_t ADPL_DevMonTempGet(int Index , int * ptemp );
ADPL_ERROR_t ADPL_DevMonTempThresholdHighGet(int Index , int* ptemp );
ADPL_ERROR_t ADPL_DevMonTempThresholdHighSet(int Index , int temp );
ADPL_ERROR_t ADPL_DevMonTempThresholdLowGet(int Index , int* ptemp );
ADPL_ERROR_t ADPL_DevMonTempThresholdLowSet(int Index , int temp );

ADPL_ERROR_t ADPL_DevMonFanDataGet(uint32_t fanIndex , adpl_fanCurDataInfo_t* pdata );
ADPL_ERROR_t ADPL_DevMonFanLedSet(int fanIndex , int ledData );
ADPL_ERROR_t ADPL_DevMonFanSerialNumberGet(int fanIndex ,  char *pSN );
ADPL_ERROR_t ADPL_DevMonFanSerialNumberSet(int fanIndex ,  char *pSN );
ADPL_ERROR_t ADPL_DevMonFanSpeedGet(int fanIndex , int* pfanSpeed );
ADPL_ERROR_t ADPL_DevMonFanSpeedLevelSet(int fanIndex , int fanSpeedLvl );
ADPL_ERROR_t ADPL_DevMonFanCardStatusGet(int fanIndex , int* pstatus );

/*****************************************************************************
 函 数 名  : ADPL_Y1564TstCreate
 功能描述  : 创建Y1564作业。
 输入参数  : uint32_t lmepId   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年3月2日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t  ADPL_Y1564TstCreate(uint32_t lmepId);

/*****************************************************************************
 函 数 名  : ADPL_Y1564TstDelete
 功能描述  : 删除Y1564 作业实例。
 输入参数  : uint32_t lmepId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年3月2日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t  ADPL_Y1564TstDelete(uint32_t lmepId);


/*****************************************************************************
 函 数 名  : ADPL_rcsam_lb_set
 功能描述  : 1564环回端配置。
 输入参数  : ulong_t ulIfindex , openflow id              
             			ulong_t enable  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/

int ADPL_rcsam_lb_set(ulong_t ulIfindex, ulong_t enable);

void ADPL_Init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*_OF_ADPL_API_H*/
#endif





