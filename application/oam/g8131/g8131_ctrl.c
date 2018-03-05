/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
**********************************************************************
*
* @filename   g8131_ctrl.c
*
* @purpose    Contains definitions to execution flow control procedures
*             for dot1ag component
*
* @component  G8131
*
* @comments   none
*
* @create     5-Nov-2013
*
**********************************************************************/
#include <stdlib.h>
#include <string.h>
#include "g8131_include.h"
#include "dot1ag_api.h"
#include "msgq.h"
#include "ofdb_api.h"

extern ofdpaRWlock_t g8131RWLock;
extern void *g8131EventMsgqSem;

/* Internal Event Message Queue */
extern void *g8131EventQueue;

extern g8131_timer_list_t g8131TimerList;

/*********************************************************************
* @purpose  Save the data in a message to a shared memory
*
* @param    event        @b{(input)} event type
* @param    *data        @b{(input)} pointer to data
*
* @returns
*
* @comments Once the message is serviced, this variable size data will
*           be retrieved
*
* @end
*********************************************************************/
static void g8131FillMsg(void *data, g8131MgmtMsg_t *msg)
{
  switch(msg->event)
  {
    case G8131_EVENT_PG_CREATE:
    case G8131_EVENT_PG_DELETE:
    case G8131_EVENT_REQUEST:
    case G8131_EVENT_ALARM_DFOP_CM:
    case G8131_EVENT_ALARM_DFOP_PM:
    case G8131_EVENT_ALARM_DFOP_NR:
    case G8131_EVENT_ALARM_DFOP_TO:
      memcpy(&(msg->data.reqMsg), data, sizeof(msg->data.reqMsg));
      break;

    case G8131_EVENT_PDU_RECEIVE:
      memcpy(&(msg->data.pduMsg), data, sizeof(msg->data.pduMsg));
      break;

    default:
      break;
  }
}

/*********************************************************************
* @purpose  g8131 task which serves the request queue
*
* @param    none
*
* @returns  void
*
* @comments
*
* @end
**********************************************************************/
void g8131Task(void *arg)
{
  int status;
  g8131MgmtMsg_t  eventMsg;

  dot1agApsRcvCallBackRegister(g8131PduRcvFromDot1ag);
  do
  {
    dpaSemTake(g8131EventMsgqSem, DPA_SEM_FOREVER);
    status = msgqRcv(g8131EventQueue, &eventMsg);
	G8131_TRACE(G8131_DEBUG_EVENT,
                  "msgqRcv event %x grpid = %d\r\n", eventMsg.event,  eventMsg.data.reqMsg.grpId);
    if (0 == status)
    {
      dpaRWlockWritelock(&g8131RWLock);
      (void)g8131DispatchCmd(&eventMsg);
      dpaRWlockUnlock(&g8131RWLock);
    }
  }while(1);
}

/*********************************************************************
* @purpose  This function is used to send timer events
*
* @param    timerCtrlBlk @b{(input)}   Timer Control Block
* @param    ptrData      @b{(input)}   Ptr to passed data
*
* @returns  None
*
* @notes    None
* @end
*********************************************************************/
void g8131TimerExpiryHdlr(timer_t timerCtrlBlk, void* ptrData)
{
  if (OFDPA_E_NONE != g8131IssueCmd(G8131_EVENT_TIMER_TICK, 0, NULL))
  {
    G8131_TRACE(G8131_DEBUG_ERROR, "[#ERROR#] send timer tick failed.");
  }
}

/*********************************************************************
* @purpose  This function is used to create timer.
*
* @param    tmType       @b{(input)}   For APS and Hold-off timers
* @param    tmValue      @b{(input)}   timer value
* @param    pFunc        @b{(input)}   registered callback function
* @param    para         @b{(input)}   registered callback function's
*                                          parameter
*
* @returns   pointer to structure g8131_timer_list_t
*
* @notes     None
* @end
*********************************************************************/
void *g8131TimerCreate(g8131_timer_type_t tmType,
                       uint32_t tmValue, void *pFunc, void *para)
{
  g8131_timer_list_t *p = g8131TimerList.next;
  g8131_timer_list_t *pNode = NULL;

  /*if timer list is empty, create the first one.*/
  if (!p)
  {
    pNode = malloc(sizeof(*pNode));
    if (!pNode)
    {
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "[#ERROR#] malloc memory for timer failed.");
      return NULL;
    }

    memset(pNode, 0, sizeof(*pNode));
    pNode->node.para = para;
    pNode->node.func_p = pFunc;
    pNode->node.timeLeft = tmValue;
    pNode->node.timeType = tmType;
    g8131TimerList.next = pNode;
    return pNode;
  }

  /*If timerList is not empty, seach the link list to update or create*/
  for(; p; p=p->next)
  {
     /*add the same node */
    if (p->node.para == para && p->node.func_p == pFunc)
    {
      p->node.timeLeft = tmValue;
      return p;
    }
    else if (!p->next)  /*the list last node*/
    {
      pNode = malloc(sizeof(*pNode));
      if (!pNode)
      {
        G8131_TRACE(G8131_DEBUG_ERROR,
                    "[#ERROR#] malloc memory for timer failed.");
        return NULL;
      }

      memset(pNode,0,sizeof(*pNode));
      pNode->node.para = para;
      pNode->node.func_p = pFunc;
      pNode->node.timeLeft = tmValue;
      pNode->node.timeType = tmType;
      p->next = pNode;
      return pNode;
    }
  }
  return pNode;
}

/*********************************************************************
* @purpose  This function is used to delete timer node.
*
* @param    pNode        @b{(input)}   timer's Node in timer list
*
* @returns
*
* @notes    None
* @end
*********************************************************************/
void g8131TimerDelete(void *pNode)
{
  g8131_timer_list_t *pDel = (g8131_timer_list_t *)pNode;
  g8131_timer_list_t *cur = &g8131TimerList;

  if (!pNode)
    return;

  if (g8131TimerList.next == pDel)
  {
    g8131TimerList.next = pDel->next;
    free( pDel);
    return;
  }

  for(; cur; cur = cur->next)
  {
    if (cur->next == pDel)
    {
      cur->next = pDel->next;
      free( pDel);
      return;
    }
  }


  G8131_TRACE(G8131_DEBUG_ERROR,
	        "pNode %x no found, timeType %d", pDel, pDel->node.timeType);


}

/*********************************************************************
* @purpose  This function is used to get timer node.
*
* @param    grpId        @b{(input)}   protection  group ID
* @param    tmType       @b{(input)}   APS/Hold-off
*
* @returns  pointer to structure g8131_timer_list_t
*
* @notes    None
* @end
*********************************************************************/
g8131_timer_list_t *g8131TimerGet(uint32_t grpId, g8131_timer_type_t tmType)
{
  g8131_timer_list_t *cur = &g8131TimerList;
  pgOperData_t *pgNode = NULL;

  for(; cur; cur=cur->next)
  {
    pgNode = (pgOperData_t *)(cur->node.para);
    if ((cur->node.timeType == tmType) || (pgNode->grpId == grpId))
    {
      return cur;
    }
  }

  return NULL;
}

/*********************************************************************
* @purpose  This function is used to get timer node.
*
* @param    pNode        @b{(input)}   timer node
* @param    para         @b{(input)}   registered callback function's parameter
* @param    tmValue      @b{(input)}   timer expired value
* @param    pFunc        @b{(input)}   timer expired callback function
*
* @returns  OFDPA_E_NONE or other for failure
*
* @notes    None
* @end
*********************************************************************/
OFDPA_ERROR_t g8131TimerUpdate(void *pNode, void *para,
                         uint32_t tmValue, void *pFunc)
{
  g8131_timer_list_t *p = (g8131_timer_list_t *)pNode;
  g8131_timer_list_t *cur = &g8131TimerList;

  if (!pNode || !para)
  {
  	G8131_TRACE(G8131_DEBUG_ERROR,
                "pNode %x para %x", pNode, para);
    return OFDPA_E_FAIL;
  }
  
  for(; cur; cur=cur->next)
  {
    if (cur->next == pNode)
    {
      p->node.func_p = pFunc;
      p->node.para = para;
      p->node.timeLeft = tmValue;
      return OFDPA_E_NONE;
    }
  }

  return OFDPA_E_FAIL;
}

/*********************************************************************
* @purpose  Process timer event
*
* @returns  void
*
* @notes    This function is called every timer interval
* @end
*********************************************************************/
void g8131TimerProcess(void)
{
  g8131_timer_list_t *p = g8131TimerList.next;

  if (!p)
    return;

  for(; p; p=p->next)
  {
    if (p->node.timeLeft <= 0)
      continue;

    p->node.timeLeft = p->node.timeLeft - 1;

    if (p->node.timeLeft <= 0)
      p->node.func_p(p->node.para);
  }
}

/*********************************************************************
* @purpose  Send a command to g8131 mgmtevent queue
*
* @param    event        @b{(input)} event type
* @param    intIfNum     @b{(input)} internal interface number
* @param    *data        @b{(input)} pointer to data
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments Command is queued for service
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131IssueCmd(uint32_t event, uint32_t intIfNum, void *data)
{
  OFDPA_ERROR_t status;
  g8131MgmtMsg_t eventmsg;

  /* copy event, intIfNum and data ptr to msg struct */
  memset(&eventmsg, 0, sizeof(eventmsg));
  eventmsg.event = event;
  eventmsg.intf  = intIfNum;

  if (NULL != data)
  {
    g8131FillMsg(data, &eventmsg);
  }

  
  G8131_TRACE(G8131_DEBUG_EVENT,
				"msgqRcv event %x intIfNum %d grpId = %d\r\n", event, intIfNum, eventmsg.data.reqMsg.grpId);
  /* send message */
  status = msgqSnd(g8131EventQueue, (char *) &eventmsg);

  dpaSemGive(g8131EventMsgqSem);

  return status;
}


/*********************************************************************
* @purpose  Send a command to g8131 state machine directly
*
* @param    event        @b{(input)} event type
* @param    intIfNum     @b{(input)} internal interface number
* @param    *data        @b{(input)} pointer to data
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments Command is queued for service
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131IssueCmdSync(uint32_t event, uint32_t intIfNum, void *data)
{
  OFDPA_ERROR_t status;
  g8131MgmtMsg_t eventmsg;

  /* copy event, intIfNum and data ptr to msg struct */
  memset(&eventmsg, 0, sizeof(eventmsg));
  eventmsg.event = event;
  eventmsg.intf  = intIfNum;

  if (NULL != data)
  {
    g8131FillMsg(data, &eventmsg);
  }

	/*leishenghua modify 20170116, 保护倒换时需要获取ofdb 信号量，
	提前获取，避免死锁*/
	/*OFDB_READ_LOCK_TAKE;*/
	dpaRWlockWritelock(&g8131RWLock);
  status = g8131DispatchCmd(&eventmsg);
	dpaRWlockUnlock(&g8131RWLock);
	/*OFDB_LOCK_GIVE;*/

  return status;
}


/*********************************************************************
* @purpose  Route the event to a handling function and grab the parms
*
* @param    pMsg         @b{(input)} message containing event and
*                                    interface number
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
 **********************************************************************/
OFDPA_ERROR_t g8131DispatchCmd(g8131MgmtMsg_t *pMsg)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  switch(pMsg->event)
  {
    case G8131_EVENT_PG_CREATE:
      G8131_TRACE(G8131_DEBUG_CONFIG,
                  "Receive PG_CREATE grpId %d", pMsg->data.reqMsg.grpId);
      status = g8131PgCreateHandler(pMsg->data.reqMsg.grpId);
      break;

    case G8131_EVENT_PG_DELETE:
      G8131_TRACE(G8131_DEBUG_CONFIG,
                  "Receive PG_DELETE grpId %d", pMsg->data.reqMsg.grpId);
      status = g8131PgDeleteHandler(pMsg->data.reqMsg.grpId);
      break;

    case G8131_EVENT_REQUEST:
      G8131_TRACE(G8131_DEBUG_EVENT,
                  "Receive REQUEST %d grpId %d interface: %d",
                  pMsg->data.reqMsg.request, pMsg->data.reqMsg.grpId, pMsg->intf);
      status = g8131StateMachineClassfier(pMsg->data.reqMsg.grpId,
                                          pMsg->data.reqMsg.request,
                                          pMsg->data.reqMsg.signal);
      break;

    case G8131_EVENT_PDU_RECEIVE:
      status = g8131PduRcvHandler(pMsg->intf,
                                  pMsg->data.pduMsg.vlanId,
                                  pMsg->data.pduMsg.innerVlanId,
                                  pMsg->data.pduMsg.g8131Pkt,
                                  pMsg->data.pduMsg.dataLength,
                                  pMsg->data.pduMsg.cfmPduOffset);
      break;

    case G8131_EVENT_ALARM_DFOP_CM:
      g8131AlarmCMHandler(pMsg->data.reqMsg.grpId);
      break;

    case G8131_EVENT_ALARM_DFOP_PM:
      g8131AlarmPMHandler(pMsg->data.reqMsg.grpId);
      break;

    case G8131_EVENT_ALARM_DFOP_NR:
      /*do nothing*/
      break;

    case G8131_EVENT_TIMER_TICK:
      g8131TimerProcess();
      break;

    default:
      status = OFDPA_E_UNAVAIL;
      break;
  }

  _G8131_RETURN_CODE_CHECK(status);

  return status;
}

/*********************************************************************
* @purpose  Debug function to generate local event or simulate remote APS
*           packets receiving for G8131 test.
*
* @param    grpId        @b{(input)} protection group ID
* @param    req          @b{(input)} local request or remote request value
*
* @returns  void
*
* @comments
*
* @end
 **********************************************************************/
void g8131Debugcmd(uint32_t grpId, g8131SMRequest_t req)
{
  uint8_t fReq, fType, fReqSig, fBrgSig;
  uint8_t debugPkt[32] =
  {
    0x01, 0x80, 0xc2, 0x00, 0x00, 0x31, 0x00, 0x10,
    0x18, 0x7f, 0xf9, 0x8d, 0x81, 0x00, 0x00, 0x64,
    0x89, 0x02, 0x20, 0x27, 0x00, 0x04, 0xbf, 0x01,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  if ((req >= G8131_LR_START) && (req <= G8131_LR_END))
  {
    if (OFDPA_E_NONE != g8131PgRequest(grpId, req))
    {
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "g8131PgRequest grpId %d req %d failed",
                  grpId, req);
    }
    return;
  }

  if ((req >= G8131_RR_START) && (req <= G8131_RR_END))
  {
    if (OFDPA_E_NONE != g8131ConvRequest2Field(req,
                                               &fReq, &fType, &fReqSig, &fBrgSig))
    {
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "g8131ConvRequest2Field %d failed",
                  req);
      return;
    }

    debugPkt[22] = ((fReq << 4) | fType);
    debugPkt[23] = fReqSig;
    debugPkt[24] = fBrgSig;

    if (OFDPA_E_NONE != g8131PduRcvFromDot1ag(1, 100, 0, debugPkt, sizeof(debugPkt), 0))
    {
      G8131_TRACE(G8131_DEBUG_ERROR,
                  "g8131PduRcvFromDot1ag %d failed",
                  req);
    }

    return;
  }

  G8131_TRACE(G8131_DEBUG_ERROR,
              "request %d is out of range (%d-%d)",
              req, G8131_LR_START, G8131_RR_END);

}



/*********************************************************************
* @purpose  This function is used to get timer node.
*
* @param    pNode        @b{(input)}   timer node
* @param    para         @b{(input)}   registered callback function's parameter
* @param    tmValue      @b{(input)}   timer expired value
* @param    pFunc        @b{(input)}   timer expired callback function
*
* @returns  OFDPA_E_NONE or other for failure
*
* @notes    None
* @end
*********************************************************************/
OFDPA_ERROR_t g8131TimerDebugShow(void)
{
  int i = 0;
  g8131_timer_list_t *cur = &g8131TimerList;

  for(; cur; cur=cur->next)
  {
  	if(i%8 == 0)
	{
		printf("\r\n%04d: ", i);
	}
    printf("0x%x ", cur);		
	i++;

  }
  printf("\r\n");

  return OFDPA_E_FAIL;
}


