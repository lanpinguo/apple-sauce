#ifndef _ADPL_RCSAM_HWAPI_H
#define _ADPL_RCSAM_HWAPI_H

#ifdef __cplusplus
extern "C" {
#endif


/*MPLS����֧�ֻ����ӽӿڵ�8����,
UNIΪ�ӽӿ�ʱindex���Բ�ͬ,
��������ͬһ�����ӿ�*/
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
    ulong_t ulFDMean;	/*ƽ��������*/
    ulong_t ulFDVMin;	/*ʱ�Ӷ���*/
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
 �� �� ��      : ADPL_rcsam_uni_set
 ��������  : 1564 ����uni������
 �������  :  adpl_rcSamSvcUniHw_t *pRcSamUniInfo
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
int ADPL_rcsam_uni_set(adpl_rcSamSvcUniHw_t *pRcSamUniInfo);


/*****************************************************************************
 �� �� ��  : ADPL_rcsam_test_start_set
 ��������  : 1564 ������������Ҫ������uni��
 �������  :  ulong_t enable  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/

int ADPL_rcsam_test_start_set(ulong_t enable);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_l2_flow_setup
 ��������  : 1564 ����l2 ��������
 �������  :  adpl_rcSamSvcL2Hw_t *pRcSamFlowL2Info
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
int ADPL_rcsam_l2_flow_setup(adpl_rcSamSvcL2Hw_t *pRcSamFlowL2Info);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_l3_flow_setup
 ��������  : 1564 ����l3 ��������
 �������  :  adpl_rcSamSvcL3Hw_t *pRcSamFlowL3Info
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
int ADPL_rcsam_l3_flow_setup(adpl_rcSamSvcL3Hw_t *pRcSamFlowL3Info);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_path_set
 ��������  : 1564 ����·������
 �������  :  adpl_rcSamSvcPathHw_t *pRcSamPathInfo
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
int ADPL_rcsam_path_set(adpl_rcSamSvcPathHw_t *pRcSamPathInfo);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_do_test_set
 ��������  : 1564 ���Դ�������
 �������  :  adpl_rcSamSvcTstHw_t *pRcSamCfgTstInfo
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/

int ADPL_rcsam_do_test_set(adpl_rcSamSvcTstHw_t *pRcSamCfgTstInfo);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_test_enable
 ��������  : 1564 ���Կ�ʼ����
 �������  :  adpl_rcSamSvcTstEnaHw_t *pRcSamTstEna
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
int ADPL_rcsam_test_enable(adpl_rcSamSvcTstEnaHw_t *pRcSamTstEna);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_test_enable
 ��������  : 1564 ���Ի������
 �������  :  ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
int ADPL_rcsam_env_detect();

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_test_enable
 ��������  : 1564 ����5 ���������ݴ�����
 �������  :  ulong_t ulSvcIdList
 �������  :  ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
int ADPL_rcsam_pm_restore_set(ulong_t ulSvcIdList);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_test_enable
 ��������  : 1564 ���Դ���������
 �������  :  adpl_rcSamSvcBwHw_t *pRcSamBwInfo
 �������  :  ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
int ADPL_rcsam_bw_copy_set(adpl_rcSamSvcBwHw_t *pRcSamBwInfo);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_inservice_set
 ��������  : 1564 ��ϲ���
 �������  :  adpl_rcSamSvcInserviceHw_t *pRcSamInserviceInfo
 �������  :  ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/

int ADPL_rcsam_inservice_set(adpl_rcSamSvcInserviceHw_t *pRcSamInserviceInfo);


/*****************************************************************************
 �� �� ��      : ADPL_rcsam_test_result_clr
 ��������  : 1564 ͳ�ƽ����0
 �������  :  ulong_t ulSvcId
 �������  :  ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/

int ADPL_rcsam_test_result_clr(ulong_t ulSvcId);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_uni_pvid_get
 ��������  : 1564 ����uni��pvid��ȡ
 �������  :  
 �������  :  ulong_t *ulPvid
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/

int ADPL_rcsam_uni_pvid_get(ulong_t *ulPvid);


/*****************************************************************************
 �� �� ��      : ADPL_rcsam_uni_pvid_get
 ��������  : 1564 ���Խ����ȡ 
 �������  :  adpl_rcSamSvcRptHw_t *pRcSamTstRst
 �������  :  adpl_rcSamSvcRptHw_t *pRcSamTstRst
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/

int ADPL_rcsam_test_result_get(adpl_rcSamSvcRptHw_t *pRcSamTstRst);

/*****************************************************************************
 �� �� ��      : ADPL_rcsam_uni_pvid_get
 ��������  : 1564 ����ÿ5���Ӳ��Խ����ȡ 
 �������  :  adpl_rcSamSvcPer5MinRptHw_t *pRcSamPer5minRstGet
 �������  :  adpl_rcSamSvcPer5MinRptHw_t *pRcSamPer5minRstGet
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/

int ADPL_rcsam_per5min_result_get(adpl_rcSamSvcPer5MinRptHw_t *pRcSamPer5minRstGet);




#ifdef __cplusplus
}
#endif

#endif //_RCSAMHWAPI_H

