#ifndef _ADPL_RCSAM_HWAPI_H
#define _ADPL_RCSAM_HWAPI_H

#ifdef __cplusplus
extern "C" {
#endif


/*MPLS测量支持基于子接口的8条流,
UNI为子接口时index可以不同,
但必须是同一个父接口*/
typedef struct adpl_rcSamSvcUniHw_s
{    
    ulong_t ulSvcId;
    ulong_t ulUniIfidx;
}adpl_rcSamSvcUniHw_t;


typedef struct adpl_rcSamSvcBwHw_s
{    
    ulong_t ulSvcId;
    ulong_t ulCir;
    ulong_t ulCbs;
    ulong_t ulEir;
    ulong_t ulEbs;
    ulong_t ulSvid;
    ulong_t ulCvid;
	ulong_t ulScos;
	ulong_t ulCcos;
	ulong_t ulDscp;
	ulong_t ulIdentifyType;
}adpl_rcSamSvcBwHw_t;

typedef struct adpl_rcSamSvcL2Hw_s
{   
	ulong_t ulSvcId;
	ulong_t enumFlowType;
	ulong_t enumFlowMode;
	ulong_t ulFlowSize;
	uchar_t auchDmac[6];
	uchar_t auchSmac[6];
	BOOL bStag;
	BOOL bCtag;
	ulong_t ulSTpid;
	ulong_t ulSvid;
	ulong_t ulCTpid;
	ulong_t ulCvid;
	uchar_t uchScos;
	uchar_t uchSCfi;
	uchar_t uchCcos;
	uchar_t uchCCfi;
	ushort_t ulEthType;
	ulong_t enumFlowPattern;
	ulong_t isMplsTest;
	ulong_t isRsomSet;
	ulong_t isRsomNoPreUntag;
}adpl_rcSamSvcL2Hw_t;

typedef struct adpl_rcSamSvcL3Hw_s
{ 
	adpl_rcSamSvcL2Hw_t stL2;
	ulong_t ulDip;
	ulong_t ulSip;
	ushort_t ulDstUdpPort;
	ushort_t ulSrcUdpPort;
	BOOL bDscp;
	ulong_t ulIpPri;
	ulong_t ulL3ChkSum;
	uchar_t uchIpTtl;
	ulong_t UNIPort;
	ulong_t isRsomSet;
	ulong_t isRsomNoPreUntag;
}adpl_rcSamSvcL3Hw_t;

#if 0
typedef struct rcSamSvcFlowMplsHw_s
{
	adpl_rcSamSvcL2Hw_t stL2;
	ulong_t ulLspLabel;
	ulong_t ulLspExp;
	ulong_t ulPwLabel;
	ulong_t ulPwExp;
	uchar_t aucLspName[32 + 4];   
	ulong_t ulPeer;
	ulong_t ulVcid;
}adpl_rcSamSvcMplsHw_t;
#endif
typedef struct adpl_rcSamSvcInserviceHw_s
{    
    ulong_t ulSvcId;
	ulong_t ulUniPort;
    ulong_t ulVlanId;
	ulong_t ulCos;
	ulong_t ulIpDscp;
	ulong_t ulLspLabel;
  	ulong_t ulPwLabel;
	ulong_t ulIdentifyType;
}adpl_rcSamSvcInserviceHw_t;

typedef struct adpl_rcSamSvcPathHw_s
{    
    ulong_t ulSvcId;
    ulong_t ulPktNum;
}adpl_rcSamSvcPathHw_t;

typedef struct adpl_rcSamSvcTstHw_s
{    
    ulong_t ulSvcId;
    ulong_t ulRate;  /*bps*/
	ulong_t ulBrust;
}adpl_rcSamSvcTstHw_t;

typedef struct adpl_rcSamSvcArpRepHw_s
{    
    ulong_t ulSvcId;
    uchar_t auchDmac[6];
	ulong_t ulEthType;
	ulong_t ulDip;
	BOOL 	bEnable;
}adpl_rcSamSvcArpRepHw_t;

typedef struct adpl_rcSamSvcTstEnaHw_s
{    
    ulong_t ulSvcIdList;
    ulong_t ulSvcTi;
	ulong_t ulEnable;
}adpl_rcSamSvcTstEnaHw_t;

typedef struct adpl_rcSamSvcRptHw_s
{
    ulong_t ulSvcId;
    ulong_t ulTstRet;
    ulong_t ulIr;
    ulong_t ulFlr;
    ulong_t ulFDMin;
    ulong_t ulFDMax;
    ulong_t ulFDCurrent;
    ulong_t ulFDMean;	/*平均抖动？*/
    ulong_t ulFDVMin;	/*时延抖动*/
    ulong_t ulFDVMax;
    ulong_t ulFDVCurrent;
    ulong_t ulFDVMean;
    ulong_t ulIrMin;
    ulong_t ulIrMax;
    ulong_t ulFlrMin;
    ulong_t ulFlrMax;
    ulong_t ulFDR;
    ulong_t ulBER;
    ulong_t ulAVail;
    ulong_t ulPadding;
}adpl_rcSamSvcRptHw_t;

typedef struct adpl_rcSamSvcPer5MinRptHw_s
{
	ulong_t ulSvcId;
	ulong_t ulTstNum;
	ulong_t ulTstStartTime;
	ulong_t ulIr;
	ulong_t ulFlr;
	ulong_t ulFDMin;
	ulong_t ulFDMax;
	ulong_t ulFDMean;
	ulong_t ulFDVMin;
	ulong_t ulFDVMax;
	ulong_t ulFDVMean;
	ulong_t ulFDR;
	ulong_t ulBER;
	ulong_t ulAvailStatus;
}adpl_rcSamSvcPer5MinRptHw_t;

typedef struct adpl_rcSamArpInfo_s
{
	uchar_t uchDmac[6];
	uchar_t uchSmac[6];
	ulong_t ulSip;
	ulong_t ulNextHopIp;
	ulong_t ulPort;
	ushort_t uTpid;
	ushort_t uSvid;
}adpl_rcSamArpInfo_t;



/*****************************************************************************
 函 数 名      : ADPL_rcsam_uni_set
 功能描述  : 1564 测试uni口配置
 输入参数  :  adpl_rcSamSvcUniHw_t *pRcSamUniInfo
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
int ADPL_rcsam_uni_set(adpl_rcSamSvcUniHw_t *pRcSamUniInfo);


/*****************************************************************************
 函 数 名  : ADPL_rcsam_test_start_set
 功能描述  : 1564 测试启动，需要先配置uni口
 输入参数  :  ulong_t enable  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/

int ADPL_rcsam_test_start_set(ulong_t enable);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_l2_flow_setup
 功能描述  : 1564 测试l2 报文配置
 输入参数  :  adpl_rcSamSvcL2Hw_t *pRcSamFlowL2Info
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
int ADPL_rcsam_l2_flow_setup(adpl_rcSamSvcL2Hw_t *pRcSamFlowL2Info);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_l3_flow_setup
 功能描述  : 1564 测试l3 报文配置
 输入参数  :  adpl_rcSamSvcL3Hw_t *pRcSamFlowL3Info
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
int ADPL_rcsam_l3_flow_setup(adpl_rcSamSvcL3Hw_t *pRcSamFlowL3Info);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_path_set
 功能描述  : 1564 测试路径建立
 输入参数  :  adpl_rcSamSvcPathHw_t *pRcSamPathInfo
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
int ADPL_rcsam_path_set(adpl_rcSamSvcPathHw_t *pRcSamPathInfo);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_do_test_set
 功能描述  : 1564 测试带宽配置
 输入参数  :  adpl_rcSamSvcTstHw_t *pRcSamCfgTstInfo
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/

int ADPL_rcsam_do_test_set(adpl_rcSamSvcTstHw_t *pRcSamCfgTstInfo);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_test_enable
 功能描述  : 1564 测试开始配置
 输入参数  :  adpl_rcSamSvcTstEnaHw_t *pRcSamTstEna
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
int ADPL_rcsam_test_enable(adpl_rcSamSvcTstEnaHw_t *pRcSamTstEna);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_test_enable
 功能描述  : 1564 测试环境检测
 输入参数  :  无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
int ADPL_rcsam_env_detect();

/*****************************************************************************
 函 数 名      : ADPL_rcsam_test_enable
 功能描述  : 1564 测试5 分钟性能暂存配置
 输入参数  :  ulong_t ulSvcIdList
 输出参数  :  无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
int ADPL_rcsam_pm_restore_set(ulong_t ulSvcIdList);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_test_enable
 功能描述  : 1564 测试带宽拷贝配置
 输入参数  :  adpl_rcSamSvcBwHw_t *pRcSamBwInfo
 输出参数  :  无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
int ADPL_rcsam_bw_copy_set(adpl_rcSamSvcBwHw_t *pRcSamBwInfo);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_inservice_set
 功能描述  : 1564 拟合测试
 输入参数  :  adpl_rcSamSvcInserviceHw_t *pRcSamInserviceInfo
 输出参数  :  无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/

int ADPL_rcsam_inservice_set(adpl_rcSamSvcInserviceHw_t *pRcSamInserviceInfo);


/*****************************************************************************
 函 数 名      : ADPL_rcsam_test_result_clr
 功能描述  : 1564 统计结果清0
 输入参数  :  ulong_t ulSvcId
 输出参数  :  无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/

int ADPL_rcsam_test_result_clr(ulong_t ulSvcId);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_uni_pvid_get
 功能描述  : 1564 测试uni口pvid获取
 输入参数  :  
 输出参数  :  ulong_t *ulPvid
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/

int ADPL_rcsam_uni_pvid_get(ulong_t *ulPvid);


/*****************************************************************************
 函 数 名      : ADPL_rcsam_uni_pvid_get
 功能描述  : 1564 测试结果获取 
 输入参数  :  adpl_rcSamSvcRptHw_t *pRcSamTstRst
 输出参数  :  adpl_rcSamSvcRptHw_t *pRcSamTstRst
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/

int ADPL_rcsam_test_result_get(adpl_rcSamSvcRptHw_t *pRcSamTstRst);

/*****************************************************************************
 函 数 名      : ADPL_rcsam_uni_pvid_get
 功能描述  : 1564 性能每5分钟测试结果获取 
 输入参数  :  adpl_rcSamSvcPer5MinRptHw_t *pRcSamPer5minRstGet
 输出参数  :  adpl_rcSamSvcPer5MinRptHw_t *pRcSamPer5minRstGet
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/

int ADPL_rcsam_per5min_result_get(adpl_rcSamSvcPer5MinRptHw_t *pRcSamPer5minRstGet);




#ifdef __cplusplus
}
#endif

#endif //_RCSAMHWAPI_H

