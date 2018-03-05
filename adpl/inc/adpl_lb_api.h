#ifndef _ADPL_LB_API_H_
#define _ADPL_LB_API_H_


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

#endif

