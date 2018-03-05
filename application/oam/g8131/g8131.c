/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2005-2015
*
**********************************************************************
* @filename  g8131.c
*
* @purpose   G8131 module initilization
*
* @component G8131
*
* @comments
*
* @end
*
**********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "ofdpa_util.h"
//#include "driver_oam.h"
#include "msgq.h"
#include "ofdpa_porting.h"
#include "g8131_cfg.h"
#include "g8131_include.h"

g8131Cfg_t    *g8131Cfg = NULL;
ofdpaRWlock_t  g8131RWLock;
void          *g8131TaskSyncSem;
void          *g8131EventMsgqSem;

void           *g8131PduTxBufPoolId = NULL;

g8131_timer_list_t g8131TimerList;
timer_t            g8131TimerId;

/* Internal Event Message Queue */
void              *g8131EventQueue = NULL;

OFDPA_ERROR_t oamG8131Init(void)
{
	return 0;
}
