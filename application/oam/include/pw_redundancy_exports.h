
#ifndef _PW_REDUN_EXPORTS_
#define _PW_REDUN_EXPORTS_


#define OFDPA_PW_REDUN_MIN_ID          1
#define OFDPA_PW_REDUN_MAX_ID          128
#define OFDPA_PW_REDUN_MAX             (OFDPA_PW_REDUN_MAX_ID - OFDPA_PW_REDUN_MIN_ID + 1)


typedef struct pwRedunEventInfo_s
{
  uint32_t                  lmepId;
  uint32_t 					eventType;
  OFDPA_BOOL                faultOn;
} pwRedunEventInfo_t;



#endif

