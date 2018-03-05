/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2005-2015
*
**********************************************************************
* @filename  g8131_util.h
*
* @purpose   g8131 file
*
* @component g8131
*
* @comments
*
* @create    7/11/2013
*
* @end
*
**********************************************************************/
#ifndef INCLUDE_G8131_UTIL_H
#define INCLUDE_G8131_UTIL_H

#include "g8131_include.h"
#include "ofdpa_datatypes.h"

#define _G8131_RETURN_CODE_CHECK(status) \
  if (OFDPA_E_NONE != status) \
  { \
    G8131_TRACE(G8131_DEBUG_ERROR, "[#ERROR#] return error %d", status); \
    return status; \
  } \

#endif
