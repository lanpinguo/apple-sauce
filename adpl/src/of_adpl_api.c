

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
extern void * ofagentapp_pkt_in_handle(void *arg);
extern void wsSoftRest();

extern void __ind_ofdpa_driver_module_init__(void);
extern void __ofstatemanager_module_init__(void);
extern void __biglist_module_init__(void);
extern void __configuration_module_init__(void);
extern void __ofconnectionmanager_module_init__(void);
extern void __socketmanager_module_init__(void);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*include*/
//#include <rospublic.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
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
#include <time.h>



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
#include <indigo/of_connection_manager.h>

#define AIM_LOG_MODULE_NAME adpl_ofagent
#include <AIM/aim_log.h>


#include "of_adpl_util.h"
#include "of_adpl_api.h"
#include "sal_thread.h"


static int sighup_eventfd;
static int sigterm_eventfd;
time_t startTime;
unsigned int g_inband_vlan = 0xffff;
unsigned int g_inband_vlan_last = 0xffff;

timer_t    dhcpLostTimerId;
int dhcpLostTimerRun = 0;
int vlan_Port_openflow =1;

int g_dhcp_lost_handle = 1;
	
/*******************************************************************************/
/* Internal prototyp declaration                                               */
/*******************************************************************************/

AIM_LOG_STRUCT_DEFINE(
                      AIM_LOG_OPTIONS_DEFAULT,
                      AIM_LOG_BITS_DEFAULT,
                      NULL,     /* Custom Log Map */
                      0
                      );

void
regControllerStateChangeInvoke(indigo_cxn_id_t cxn_id,
		  indigo_cxn_protocol_params_t *cxn_proto_params,
                  indigo_cxn_state_t state,
                  void *cookie);

struct ofdpaSocketHandle_s ofdpaSocketHandle =
{
  .eventSockFd = -1,
  .pktSockFd = -1,
};


ADPL_EventCallback gPortEventCallbackFunc = NULL;
ADPL_EventCallback gOamEventCallbackFunc = NULL;
ADPL_EventCallback gBfdEventCallbackFunc = NULL;

ADPL_EventCallback gPowerDownEventCallbackFunc = NULL;
ADPL_EventCallback gApsEventCallbackFunc = NULL;
adpl_DeviceOperations_t gDeviceOps = { NULL };

pthread_t ofagent_ntid = 0;
pthread_t ofagent_pkt_rcv_ntid = 0;
pthread_t ofagent_event_rcv_ntid = 0;

typedef struct adplSocketHandle_s
{
  int     eventSockFd;
  int     pktSockFd;
} adplSocketHandle_t;


#if 1

/* Maximum number of packets queued in a UNIX datagram socket. */
#define DRIVER_SOCKET_Q_MAX  1024

/* UNIX datagram socket "address" for client and server. */
#define OFDPA_CLIENT_ADDR   "/tmp/ofdpa_pkt_client"
#define OFDPA_SERVER_ADDR   "/tmp/ofdpa_pkt_server"

/* Minimum and maximum ethernet frame lengths for packet transmit API.
 * Lengths include Ethernet header and payload, and do not include the
 * 4-byte CRC trailer. We assume the hardware automatically pads Ethernet
 * frames to the minimum length required on the wire. */
#define OFDPA_MIN_PKT_LEN 14
#define OFDPA_MAX_PKT_LEN 12288

/* UNIX datagram socket "address" for client and server. */
#define OFDPA_EVENT_CLIENT_ADDR   "/tmp/ofdpa_event_client"
#define OFDPA_EVENT_SERVER_ADDR   "/tmp/ofdpa_event_server"

#define OFDPA_DATAPATH_MAX_FLOWS_AGE   10000
#define OFDPA_DATAPATH_FLOWS_AGE_INTERVAL_USEC   5000000




int ofdpaClientEventSockFdGet(void)
{
  return ofdpaSocketHandle.eventSockFd;
}

int ofdpaClientPktSockFdGet(void)
{
  return ofdpaSocketHandle.pktSockFd;
}


/* socket is created as a blocking socket */
static
int dpaClientEventSocketCreate(void)
{
  struct sockaddr_un clsockaddr;
  int addrlen;
  int sockfd;
  int retval;
  unsigned int rcvSize = (1024 * 1024); /* bytes */

  
  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    return -1;
  }

  memset(&clsockaddr, 0, sizeof(clsockaddr));
  clsockaddr.sun_family = AF_UNIX;
  snprintf(clsockaddr.sun_path, sizeof(clsockaddr.sun_path) - 1, OFDPA_EVENT_CLIENT_ADDR);
  addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(clsockaddr.sun_path);
  unlink((const char *)clsockaddr.sun_path);     /* remove old socket file if it exists */

  retval = bind(sockfd, (const struct sockaddr *)&clsockaddr, addrlen);
  if (retval < 0)
  {
    close(sockfd);
    return -1;
  }


  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvSize, sizeof(rcvSize)) == -1)
  {
    close(sockfd);
    return -1;
  }


  return sockfd;
}

/* socket is created as a blocking socket */
static
int dpaClientPktSocketCreate(void)
{
  struct sockaddr_un clsockaddr;
  int addrlen;
  int sockfd;
  int retval;
  unsigned int rcvSize = (50 * 1024);   /* bytes */

  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    return -1;
  }

  memset(&clsockaddr, 0, sizeof(clsockaddr));
  clsockaddr.sun_family = AF_UNIX;
  snprintf(clsockaddr.sun_path, sizeof(clsockaddr.sun_path) - 1, "%s", OFDPA_CLIENT_ADDR);
  addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(clsockaddr.sun_path);
  unlink((const char *)clsockaddr.sun_path);     /* remove old socket file if it exists */

  retval = bind(sockfd, (const struct sockaddr *)&clsockaddr, addrlen);
  if (retval < 0)
  {
    close(sockfd);
    return -1;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvSize, sizeof(rcvSize)) == -1)
  {
    close(sockfd);
    return -1;
  }

  return sockfd;
}

OFDPA_ERROR_t ofdpaEventReceive(struct timeval *timeout)
{
  int ofdpaEventClientFd;
  //dpaEventMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

  ofdpaEventClientFd = ofdpaClientEventSockFdGet();
  if (ofdpaEventClientFd < 0)
  {
    return OFDPA_E_FAIL;
  }

  if (timeout)
  {
    if ((timeout->tv_sec == 0) && (timeout->tv_usec == 0))
    {
      /* set socket to non-blocking for this read */
      flags |= MSG_DONTWAIT;
    }
    else
    {
      /* blocking socket with a timeout */
      rv = setsockopt(ofdpaEventClientFd, SOL_SOCKET, SO_RCVTIMEO, (char *)timeout,
                      sizeof(struct timeval));
      if (rv < 0)
      {
		AIM_LOG_ERROR("Failed to set packet receive timeout. Error %d.\r\n", rv);

        return OFDPA_E_FAIL;
      }
    }
  }
  else
  {
    /* blocking socket with no timeout. Make sure there is no timeout configured
     * on the socket from previous call. */
    rv = setsockopt(ofdpaEventClientFd, SOL_SOCKET, SO_RCVTIMEO, NULL, 0);
  }

  //recvBytes = recvfrom(ofdpaEventClientFd, &msg, sizeof(msg), flags, 0, 0);

  if (recvBytes < 0)
  {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
    {
      /* Normal if no packets waiting to be received and caller didn't block. */
      return OFDPA_E_TIMEOUT;
    }
  	AIM_LOG_ERROR("Failed to receive packet. recvfrom() returned %d. errno %s.\r\n",
  							  recvBytes, strerror(errno));
    return OFDPA_E_FAIL;
  }

  /* drain any additional event messages from socket */
  flags = MSG_DONTWAIT;
  do
  {

    //memset(&msg, 0, sizeof(msg));
    //recvBytes = recvfrom(ofdpaEventClientFd, &msg, sizeof(msg), flags, 0, 0);
	
  } while(recvBytes > 0);

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaPktReceive(struct timeval *timeout, ofdpaPacket_t *pkt)
{
  int ofdpaClientFd;
  //pktMetaData_t *md;
  int recvBytes;
  int rv;
  int flags = 0;
  static unsigned char *buf = NULL;

  if (!pkt || !pkt->pktData.pstart)
  {
    return OFDPA_E_PARAM;
  }

  ofdpaClientFd = ofdpaClientPktSockFdGet();
  if (ofdpaClientFd < 0)
  {
    return OFDPA_E_FAIL;
  }

  /* Allocate one max size buffer the first time this function is called */
  if (buf == NULL)
  {
    //buf = malloc(OFDPA_MAX_PKT_LEN + sizeof(pktMetaData_t));
    if (buf == NULL)
    {
      AIM_LOG_ERROR("Failed to allocate packet receive buffer.\r\n");
      return OFDPA_E_FAIL;
    }
  }

  if (timeout)
  {
    if ((timeout->tv_sec == 0) && (timeout->tv_usec == 0))
    {
      /* set socket to non-blocking for this read */
      flags |= MSG_DONTWAIT;
    }
    else
    {
      /* blocking socket with a timeout */
      rv = setsockopt(ofdpaClientFd, SOL_SOCKET, SO_RCVTIMEO, (char *)timeout,
                      sizeof(struct timeval));
      if (rv < 0)
      {
        AIM_LOG_ERROR("Failed to set packet receive timeout. Error %d.\r\n", rv);
        return OFDPA_E_FAIL;
      }
    }
  }
  else
  {
    /* blocking socket with no timeout. Make sure there is no timeout configured
     * on the socket from previous call. */
    rv = setsockopt(ofdpaClientFd, SOL_SOCKET, SO_RCVTIMEO, NULL, 0);
  }

  /* Now retrieve the entire packet */
  //recvBytes = recvfrom(ofdpaClientFd, buf, OFDPA_MAX_PKT_LEN + sizeof(pktMetaData_t), flags, 0, 0);
#if 0
  if (recvBytes < (int) sizeof(pktMetaData_t))
  {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
    {
      /* Normal if no packets waiting to be received and caller didn't block. */
      return OFDPA_E_TIMEOUT;
    }
    AIM_LOG_ERROR("Failed to receive packet. recvfrom() returned %d. errno %s.\r\n",
                      recvBytes, strerror(errno));
    return OFDPA_E_FAIL;
  }
#endif

  return OFDPA_E_NONE;
}


OFDPA_ERROR_t ofdpaClientEventSockBind(void)
{
  ofdpaSocketHandle.eventSockFd = dpaClientEventSocketCreate();

  if (ofdpaSocketHandle.eventSockFd < 0)
  {
    ofdpaSocketHandle.eventSockFd = -1;
    return OFDPA_E_FAIL;
  }
  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaClientPktSockBind(void)
{
  ofdpaSocketHandle.pktSockFd = dpaClientPktSocketCreate();

  if (ofdpaSocketHandle.pktSockFd < 0)
  {
    ofdpaSocketHandle.pktSockFd = -1;
    return OFDPA_E_FAIL;
  }
  return OFDPA_E_NONE;
}



void * ofagentapp_event_handle(void *arg)
{  

	if(	OFDPA_E_NONE != ofdpaClientEventSockBind())
	{
		printf("\r\nofdpaClientEventSockBind Failed");
		return ((void *)0);
	}
    
	while(1)
	{
		if((ofdpaEventReceive(NULL)) == OFDPA_E_NONE)
		{
			ind_ofdpa_flow_event_receive();
			ind_ofdpa_port_event_receive();
		}
		sleep(1);
	}

	return ((void *)0);
}




static void
ind_ofdpa_event_socket_ready(int socket_id, void *cookie, int read_ready,
                            int write_ready, int error_seen)
{
  struct timeval timeout;

  if (!read_ready)
  {
    AIM_LOG_ERROR("Error: read not ready for connection");
    return;
  }

  if (error_seen)
  {
    AIM_LOG_ERROR("Error seen on socket");
    return;
  }

  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  while ((ofdpaEventReceive(&timeout)) == OFDPA_E_NONE)
  {
    ind_ofdpa_flow_event_receive();
    ind_ofdpa_port_event_receive();
  }
  return;
}

static void
ind_ofdpa_pkt_socket_ready(int socket_id, void *cookie, int read_ready,
                            int write_ready, int error_seen)
{
  if (!read_ready)
  {
    AIM_LOG_ERROR("Error: read not ready for connection");
    return;
  }

  if (error_seen)
  {
    AIM_LOG_ERROR("Error seen on socket");
    return;
  }

  ind_ofdpa_pkt_receive();

  return;
}

static void
sighup_callback(int socket_id, void *cookie,
                int read_ready, int write_ready, int error_seen)
{
    uint64_t x;
    if (read(sighup_eventfd, &x, sizeof(x)) < 0) {
        /* silence warn_unused_result */
    }
    AIM_LOG_MSG("Received SIGHUP");
}

static void
sighup(int signum)
{
    uint64_t x = 1;
    if (write(sighup_eventfd, &x, sizeof(x)) < 0) {
        /* silence warn_unused_result */
    }
}

static void
sigterm_callback(int socket_id, void *cookie,
                 int read_ready, int write_ready, int error_seen)
{
    uint64_t x;
    if (read(sigterm_eventfd, &x, sizeof(x)) < 0) {
        /* silence warn_unused_result */
    }

    ind_soc_run_status_set(IND_SOC_RUN_STATUS_EXIT);
}

static void
sigterm(int signum)
{
    uint64_t x = 1;
    if (write(sigterm_eventfd, &x, sizeof(x)) < 0) {
        /* silence warn_unused_result */
    }
}



ADPL_ERROR_t ADPL_OfagentThreadCore(void * cfg)
{
	ind_soc_config_t soc_cfg;
	ind_cxn_config_t cxn_cfg;
	ind_core_config_t core_cfg = {
		.disconnected_mode = 0,
		.max_flowtable_entries = 0
	};
	
	of_desc_str_t dp_desc = "";
	char hostname[256] = "orangePi";
	char domainname[256];
	of_desc_str_t mfr_desc = "";
	of_desc_str_t sw_desc = "SDN_1.3.0";
	of_desc_str_t hw_desc = "orangePiPC2";
	of_serial_num_t serial_num = "";
	uint64_t dpid = OFSTATEMANAGER_CONFIG_DPID_DEFAULT;
	OFDPA_ERROR_t	  rc;



	AIM_LOG_STRUCT_REGISTER();

	__ind_ofdpa_driver_module_init__();
	__ofstatemanager_module_init__();
	__biglist_module_init__();
	__configuration_module_init__();
	__ofconnectionmanager_module_init__();
	__socketmanager_module_init__();

#if 0
	aim_log_fid_set_all(AIM_LOG_FLAG_FATAL, 1);
	aim_log_fid_set_all(AIM_LOG_FLAG_ERROR, 1);
	aim_log_fid_set_all(AIM_LOG_FLAG_WARN, 1);
	aim_log_fid_set_all(AIM_LOG_FLAG_VERBOSE, 1);
	aim_log_fid_set_all(AIM_LOG_FLAG_TRACE, 1);
#endif

	aim_log_fid_set_all(AIM_LOG_FLAG_FATAL, 1);
	aim_log_fid_set_all(AIM_LOG_FLAG_ERROR, 1);
	aim_log_fid_set_all(AIM_LOG_FLAG_WARN, 1);


	/* Setup Indigo DPID */
	printf("OF Datapath ID: 0x%016llX\n", dpid);
	(void)indigo_core_dpid_set(dpid);

	rc = ofdpaClientEventSockBind();
	if (rc != OFDPA_E_NONE)
	{
	  return rc;
	}
	rc = ofdpaClientPktSockBind();
	if (rc != OFDPA_E_NONE)
	{
	  return rc;
	}

	if (ind_soc_init(&soc_cfg) < 0)
	{
		AIM_LOG_FATAL("Failed to initialize Indigo socket manager");
		return 1;
	}

	if (ind_cxn_init(&cxn_cfg) < 0)
	{
		AIM_LOG_FATAL("Failed to initialize Indigo connection manager");
		return 1;
	}

	if (ind_core_init(&core_cfg) < 0)
	{
		AIM_LOG_FATAL("Failed to initialize Indigo core module");
		return 1;
	}

	/* Enable all modules */

	if (ind_soc_enable_set(1) < 0) 
	{
		AIM_LOG_FATAL("Failed to enable Indigo socket manager");
		return 1;
	}

	if (ind_cxn_enable_set(1) < 0) 
	{
		AIM_LOG_FATAL("Failed to enable Indigo connection manager");
		return 1;
	}

	if (ind_core_enable_set(1) < 0) 
	{
		AIM_LOG_FATAL("Failed to enable Indigo core module");
		return 1;
	}


	ind_core_mfr_desc_set(mfr_desc);

	ind_core_sw_desc_set(sw_desc);

	ind_core_hw_desc_set(hw_desc);

	if (gethostname(hostname, sizeof(hostname))) 
	{
		sprintf(hostname, "(unknown)");
	}
	if (getdomainname(domainname, sizeof(domainname))) 
	{
		sprintf(domainname, "(unknown)");
	}
	snprintf(dp_desc, sizeof(dp_desc), "%s.%s pid %d",
				hostname, domainname, getpid());

	ind_core_dp_desc_set(dp_desc);
  
  ind_core_serial_num_set(serial_num);
	/* The SIGHUP handler triggers sighup_callback to run in the main loop. */
	if ((sighup_eventfd = eventfd(0, 0)) < 0) {
		AIM_LOG_FATAL("Failed to allocate eventfd");
		abort();
	}
	signal(SIGHUP, sighup);
	if (ind_soc_socket_register(sighup_eventfd, sighup_callback, NULL) < 0) {
		abort();
	}
	
	/* The SIGTERM handler triggers sigterm_callback to run in the main loop. */
	if ((sigterm_eventfd = eventfd(0, 0)) < 0) {
		AIM_LOG_FATAL("Failed to allocate eventfd");
		abort();
	}
	signal(SIGTERM, sigterm);
	if (ind_soc_socket_register(sigterm_eventfd, sigterm_callback, NULL) < 0) {
		abort();
	}

	if (ind_soc_socket_register(ofdpaClientEventSockFdGet(), ind_ofdpa_event_socket_ready, NULL) < 0)
	{
	  return 1;
	}
	
	if (ind_soc_socket_register(ofdpaClientPktSockFdGet(), ind_ofdpa_pkt_socket_ready, NULL) < 0)
	{
	  return 1;
	}

  //indigo_cxn_status_change_register(regControllerStateChangeInvoke,NULL);
	/* Start Indigo State Machine */
	ind_soc_select_and_run(-1);

	/* Go here, indicate that a exception happened*/
	AIM_LOG_MSG("Stopping %s", "Indigo");



	ind_core_finish();
	ind_cxn_finish();
	ind_soc_finish();

	return ADPL_E_NONE;
}




ADPL_ERROR_t ADPL_OfagentStart(void * cfg)
{

	ADPL_PRINT_DEBUG("\r\nOfagent init start...");

	if(ofagent_ntid != 0)
	{
		ADPL_PRINT_DEBUG("\r\n%s@%d",__FILE__,__LINE__);
		return ADPL_E_EXISTS;
	}

  ofagent_ntid = (pthread_t)sal_thread_create("Ofagent", SAL_THREAD_STKSZ,90, ADPL_OfagentThreadCore, NULL);


	ADPL_PRINT_DEBUG("\r\nOfagent init done");
	return ADPL_E_NONE;
}

ADPL_ERROR_t ADPL_OfagentCxnAdd_check(adplAgentCxnParam_t * cfg, int *p_cnx_id)
{

	int cnx_id = 0;
	indigo_cxn_protocol_params_t protocol;
    char controller_ip[INDIGO_CXN_IPV4_STRING_LEN] = {0};

	snprintf(controller_ip, INDIGO_CXN_IPV4_STRING_LEN,
			"%d.%d.%d.%d",
			cfg->controler_ip[0],
			cfg->controler_ip[1],
			cfg->controler_ip[2],
			cfg->controler_ip[3]);
	
	for(cnx_id=0; cnx_id<32; cnx_id++)
	{
		if(indigo_cxn_connection_protocol_get(cnx_id, &protocol) == INDIGO_ERROR_NONE)
		{
			/*ip都不能重复*/
			if(memcmp(protocol.tcp_over_ipv4.controller_ip, controller_ip, INDIGO_CXN_IPV4_STRING_LEN) == 0)
			{
			

				if(memcmp(protocol.tcp_over_ipv4.controller_ip, controller_ip, INDIGO_CXN_IPV4_STRING_LEN) == 0
					&& protocol.tcp_over_ipv4.controller_port == cfg->l4_port)
				{
					/*端口和IP完全相同*/
					return ADPL_E_EXISTS;
				}
				else
				{
					if(p_cnx_id)
					{
						*p_cnx_id = cnx_id;
					}					
					return ADPL_E_FULL;
				}	
			}
		}
	}

	return ADPL_E_NONE;
}



/*****************************************************************************
 函 数 名  : ADPL_OfagentCxnRemove
 功能描述  : 删除一条与控制器的连接
 输入参数  : uint32_t of_cxn_id  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_OfagentCxnRemove(uint32_t of_cxn_id)
{

	if (indigo_cxn_connection_remove(of_cxn_id) < 0) 
	{
		AIM_LOG_FATAL("Failed to remove cxn %d", of_cxn_id);
		return ADPL_E_FAIL;
	}

	return ADPL_E_NONE;
}


/*****************************************************************************
 函 数 名  : ADPL_OfagentCxnAdd
 功能描述  : 添加一个控制器的连接
 输入参数  : adplAgentCxnParam_t * cfg  ，ip地址采用大端模式存于数组中。
                    
 输出参数  : uint32_t *of_cxn_id ,创建成功则用于返回当前连接的索引值。
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_OfagentCxnAdd(adplAgentCxnParam_t * cfg ,uint32_t *of_cxn_id)
{
	indigo_cxn_protocol_params_t proto;
	indigo_cxn_config_params_t config = {
		.version = OF_VERSION_1_3,
		.cxn_priority = 0,
		.local = 0,
		.listen = 0,
		.periodic_echo_ms = 10000,
		.reset_echo_count = 3,
		.pre_reset_echo_count = 0,
	};
	indigo_cxn_id_t cxn_id;
	int old_cnx_id = 0;
	ADPL_ERROR_t rc = 0;
	
	if ( cfg == NULL )
	{
	    return ADPL_E_PARAM;
	}

	if ( ofagent_ntid == 0)
	{
	    ADPL_OfagentStart(NULL);
		sleep(1);
	}


	memset(&proto, 0, sizeof(proto));
	snprintf(proto.tcp_over_ipv4.controller_ip,INDIGO_CXN_IPV4_STRING_LEN,
			"%d.%d.%d.%d",
			cfg->controler_ip[0],
			cfg->controler_ip[1],
			cfg->controler_ip[2],
			cfg->controler_ip[3]);

	if((rc = ADPL_OfagentCxnAdd_check(cfg, &old_cnx_id)) != ADPL_E_NONE)
	{
		if(rc == ADPL_E_EXISTS)
		{
			AIM_LOG_FATAL("Failed to check controller %s %d", proto.tcp_over_ipv4.controller_ip, cfg->l4_port);
			return ADPL_E_FAIL;		
		}
		else if(rc == ADPL_E_FULL)
		{
			
			ADPL_OfagentCxnRemove(old_cnx_id);

		}	

	}

	proto.header.protocol = INDIGO_CXN_PROTO_TCP_OVER_IPV4;

	proto.tcp_over_ipv4.controller_port = cfg->l4_port;

	proto.tcp_over_ipv4.protocol = INDIGO_CXN_PROTO_TCP_OVER_IPV4;

	if ( cfg->role == ADPL_OFAGENT_ROLE_SERVER)
	{
		config.listen = 1;
	}

    if ( cfg->periodic_echo_ms != 0 )
    {
        config.periodic_echo_ms = cfg->periodic_echo_ms;
    }
    if ( cfg->reset_echo_count != 0 )
    {
        config.reset_echo_count = cfg->reset_echo_count;
    }
    if (cfg->pre_reset_echo_count != 0)
    {
        config.pre_reset_echo_count = cfg->pre_reset_echo_count;
    }

	/*leishenghua modify 20170724, 快速倒换周期大于openflow中断周期时，
	快速倒换没有意义*/
	if(config.pre_reset_echo_count >= config.reset_echo_count)
	{
		config.pre_reset_echo_count = 0;
	}
	
	AIM_LOG_MSG("Adding controller %s", proto.tcp_over_ipv4.controller_ip);
	if (indigo_cxn_connection_add(&proto, &config, &cxn_id) < 0) 
	{
		AIM_LOG_FATAL("Failed to add controller %s", proto.tcp_over_ipv4.controller_ip);
		return ADPL_E_FAIL;
	}

	if ( of_cxn_id != NULL )
	{
	    *of_cxn_id = cxn_id;
	}
	
	return ADPL_E_NONE;
}


/*****************************************************************************
 函 数 名  : ADPL_OfagentDpidSet
 功能描述  : 设置Ofagent的DPID
 输入参数  : adplDpid_t* dpid ，采用大端模式存于数组中。 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_OfagentDpidSet(adplDpid_t* dpid)
{
	indigo_error_t rc ;
	adplDpidUnion_t dpid_new;
	adplDpidUnion_t dpid_old;
	int i = 0 ;
	int len = 0;



	if ( dpid == NULL )
	{
	    return ADPL_E_PARAM;
	}

	/* Adjust to little endian */
	for ( i = 0, len = sizeof(adplDpid_t) ; i < len ; i++ )
	{
	    dpid_new.addr[len - 1 - i] = dpid->addr[i];
	}
	
	
	rc = indigo_core_dpid_get(&dpid_old.dpid);
	if ( rc !=  0)
	{
	    return ADPL_E_FAIL;
	}

	if(dpid_old.dpid == dpid_new.dpid)
	{
		return ADPL_E_NONE;
	}
	

	/*printf("\r\nModify OF Datapath ID to  0x%016llX\n", dpid_new.dpid);*/

	rc = indigo_core_dpid_set(dpid_new.dpid);
	if ( rc !=  0)
	{
	    return ADPL_E_FAIL;
	}

	return ADPL_E_NONE;
}



/*****************************************************************************
 函 数 名  : ADPL_OfagentDpidGet
 功能描述  : 获取Ofagent的DPID
 输入参数  :  
 输出参数  : adplDpid_t* dpid ，用于返回当前DPID值
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_OfagentDpidGet(adplDpid_t* dpid)
{
	indigo_error_t rc ;
	adplDpidUnion_t tmp;
	int i = 0 ;
	int len = 0;
	of_dpid_t rdpid;


	if ( dpid == NULL )
	{
	    return ADPL_E_PARAM;
	}


	rc = indigo_core_dpid_get(&rdpid);
	if ( rc !=  0)
	{
	    return ADPL_E_FAIL;
	}

	tmp.dpid = rdpid;

	for ( i = 0, len = sizeof(adplDpid_t) ; i < len ; i++ )
	{
	    dpid->addr[i] = tmp.addr[len - 1 - i];
	}

	
	printf("Get OF Datapath ID   0x%016llX\n", tmp.dpid);

	
	return ADPL_E_NONE;
}

/*****************************************************************************
 函 数 名  : ADPL_OfagentCxnStatusGet
 功能描述  : 获取Ofagent的状态
 输入参数  :  
 输出参数  : adpl_ofCxnStatus_t* status ，用于返回当前status值
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_OfagentCxnStatusGet(uint32_t cxn_id, adpl_ofCxnStatus_t * status)
{
    indigo_error_t rc;
    
    indigo_cxn_status_t cxn_status;

    char  *indigo_cxn_state[] = {
                                    "Disconnected",
                                    "Connecting",
                                    "Handshake_done",
                                    "Closing"};
    char  *indigo_cxn_role[] = {
                                    "UNKNOWN",
                                    "Master",
                                    "Slave",
                                    "Equal"};
    char  *of_version[] = {
                                    "unknown",
                                    "V_1.0",
                                    "V_1.1",
                                    "V_1.2",
                                    "V_1.3"};

    rc = indigo_cxn_connection_status_get(cxn_id,&cxn_status);

    if(rc == INDIGO_ERROR_NONE)
    {
        memcpy(status->verson, of_version[cxn_status.negotiated_version],strlen(of_version[cxn_status.negotiated_version]));
        memcpy(status->role,indigo_cxn_role[cxn_status.role],strlen(indigo_cxn_role[cxn_status.role]));
        memcpy(status->state, indigo_cxn_state[cxn_status.state],strlen(indigo_cxn_state[cxn_status.state]));
        status->MsgIn = cxn_status.messages_in;
        status->MsgOut = cxn_status.messages_out;
    }

    return rc;
}


/*****************************************************************************
 函 数 名  : ADPL_OfagentCxnStatusGet
 功能描述  : 获取Ofagent的状态
 输入参数  :  
 输出参数  : adpl_ofCxnStatus_t* status ，用于返回当前status值
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月23日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
ADPL_ERROR_t ADPL_OfagentCxnEchoPerioGet(uint32_t *periodic_echo_ms)
{
    indigo_error_t rc;
    uint32_t cxn_id = 0;
    indigo_cxn_config_params_t cxn_cfg;

  
	for(cxn_id=0; cxn_id<32; cxn_id++)
	{
    	rc = indigo_cxn_connection_config_get(cxn_id,&cxn_cfg);
		if(rc == INDIGO_ERROR_NONE)
	    {
	       *periodic_echo_ms = cxn_cfg.periodic_echo_ms;
		   
	       break; 
	    }
	}
   

    return rc;
}
/*****************************************************************************
 函 数 名  : ADPL_OfagentCxnCfgParmChange
 功能描述  :修改cxnoffagent的配置信息
 输入参数  :  
 输出参数  : 
 返 回 值  : ADPL_ERROR_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月23日
    作    者   : 
    修改内容   : 新生成函数

*****************************************************************************/

ADPL_ERROR_t ADPL_OfagentCxnCfgParmChange(uint32_t cxn_id, adplAgentCxnParam_t * cfg)
{
	indigo_cxn_config_params_t config = {
		.version = OF_VERSION_1_3,
		.cxn_priority = 0,
		.local = 0,
		.listen = 0,
		.periodic_echo_ms = 10000,
		.reset_echo_count = 3,
		.pre_reset_echo_count = 3,
	};

    if ( cfg->periodic_echo_ms != 0 )
    {
        config.periodic_echo_ms = cfg->periodic_echo_ms;
    }
    if ( cfg->reset_echo_count != 0 )
    {
        config.reset_echo_count = cfg->reset_echo_count;
    }
    if (cfg->pre_reset_echo_count != 0)
    {
        config.pre_reset_echo_count = cfg->pre_reset_echo_count;
    }

	/*leishenghua modify 20170724, 快速倒换周期大于openflow中断周期时，
	快速倒换没有意义*/
	if(config.pre_reset_echo_count >= config.reset_echo_count)
	{
		config.pre_reset_echo_count = 0;
	}
	
    if( indigo_cxn_connection_config_chg(cxn_id, &config) < 0)
    {
		AIM_LOG_FATAL("Failed to change controller %d param", cxn_id);
		return ADPL_E_FAIL;
    }
    
    return ADPL_E_NONE;
    
}

ADPL_ERROR_t ADPL_OfPktRecvRegister(ADPL_pkt_ready_callback_f f)
{
	if ( f == NULL )
	{
	    return ADPL_E_PARAM;
	}

	if ( ofagent_ntid == 0)
	{
	    ADPL_OfagentStart(NULL);
		sleep(1);
	}
	
    ind_soc_socket_unregister(ofdpaClientPktSockFdGet());

	if (ind_soc_socket_register(ofdpaClientPktSockFdGet(), (ind_soc_socket_ready_callback_f)f, NULL) < 0)
	{
	  return ADPL_E_FAIL;
	}

    return ADPL_E_NONE;
}

ADPL_ERROR_t ADPL_OfPktRecvUnRegister(void)
{
    ind_soc_socket_unregister(ofdpaClientPktSockFdGet());

	if (ind_soc_socket_register(ofdpaClientPktSockFdGet(), ind_ofdpa_pkt_socket_ready, NULL) < 0)
	{
	  return ADPL_E_FAIL;
	}

    return ADPL_E_NONE;
}

ADPL_ERROR_t ADPL_PktReceive(struct timeval *timeout, adplPacket_t *pkt)
{
    return ofdpaPktReceive(timeout,(ofdpaPacket_t *)pkt);
}


/*****************************************************************************
 函 数 名  : ADPL_reconnectPktCallbackRegister
 功能描述  : 重新连接确认报文处理函数注册
 输入参数  : cbFunction 回调函数
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月26日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/

ADPL_ERROR_t ADPL_reconnectPktCallbackRegister(adplPacketCbFunction cbFunction)
{
	return ADPL_E_NONE;
}


/*****************************************************************************
 函 数 名  : ADPL_reconnectPktCallbackUnregister
 功能描述  : 重新连接确认报文处理函数解注册
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月26日
    作    者   : leishenghua
    修改内容   : 新生成函数

*****************************************************************************/

ADPL_ERROR_t ADPL_reconnectPktCallbackUnregister(void)
{
	return ADPL_E_NONE;
}

/*****************************************************************************
 函 数 名  : ADPL_Init
 功能描述  : ADPL层代码初始化函数，由设备启动时调用，完成ofagent的启动
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年3月24日
    作    者   : Hushouqiang
    修改内容   : 新生成函数

*****************************************************************************/
void ADPL_Init(void)
{
/*  ADPL_EventCallbackRegister(ADPL_EventCallbackTst);*/

	/*Save system startup time*/
	time(&startTime);

	ADPL_OfagentStart(NULL);

}

#endif
