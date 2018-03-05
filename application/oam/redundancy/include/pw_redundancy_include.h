/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2005-2015
*
**********************************************************************
* @filename  g8131_include.h
*
* @purpose   g8131_include file
*
* @component g8131
*
* @comments
*
* @create    1-NOV-2013
*
* @end
*
**********************************************************************/

#ifndef INCLUDE_PW_REDUNDANCY_INCLUDE_H
#define INCLUDE_PW_REDUNDANCY_INCLUDE_H

/*
 **********************************************************************
 *                            HEADER FILES
 **********************************************************************
 */

#include "ofdpa_util.h"
#include "pw_redundancy_exports.h"




#define PW_REDUN_PG_CHECK(id) \
do \
{ \
  if ((id < OFDPA_PW_REDUN_MIN_ID) || (id > OFDPA_PW_REDUN_MAX_ID)) \
  { \
  	return OFDPA_E_PARAM; \
  } \
} while(0)


#define _PW_REDUN_RETURN_CODE_CHECK(status) \
	if (OFDPA_E_NONE != status) \
	{ \
	  return status; \
	} \


/*
 **********************************************************************
 *                            EXTERNAL DATA STRUCTURES
 **********************************************************************
*/

#endif
