/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_nm_mem_user.c
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/4/28
  Last Modified :
  Description   : memory manager for netmap
  Function List :
  History       :
  1.Date        : 2018/4/28
    Author      : lanpinguo
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/
	
#include <errno.h>
#define _GNU_SOURCE	/* for CPU_SET() */
#include <stdio.h>
	
#include <ctype.h>	// isprint()
#include <unistd.h>	// sysconf()
#include <sys/poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <libgen.h>
#include <signal.h>
#include <linux/kernel.h>
#include <arpa/inet.h>
#include <argp.h>
#include <regex.h>
#include <pthread.h>
	
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include "ofdpa_datatypes.h"
#include "avl_api.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
#include "datapath.h"


struct dp_nm_obj_pool {
	void * base;
	/* ---------------------------------------------------*/
	/* these are only meaningful if the pool is finalized */
	/* (see 'finalized' field in netmap_mem_d)            */
	u_int objtotal;         /* actual total number of objects. */
	u_int memtotal;		/* actual total memory space */

	u_int objfree;          /* number of free objects. */

	uint32_t *bitmap;       /* one bit per buffer, 1 means free */
	uint32_t bitmap_slots;	/* number of uint32 entries in bitmap */
	/* ---------------------------------------------------*/
	uint32_t _objsize;

};


static struct dp_nm_obj_pool nm_pool;


OFDPA_ERROR_t dpNetmapMemPoolInit(void* base,uint32_t size, uint32_t objSize)
{
	uint32_t *bitmap;
	uint32_t bitmap_slots;



	if(base == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
			"base is null\r\n");
		return OFDPA_E_PARAM;
	}

	bitmap_slots = size/sizeof(uint32_t) + 1;
	bitmap = calloc(1,bitmap_slots);
	if(bitmap == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
			"malloc failed, size %d\r\n",size);
		return OFDPA_E_PARAM;
	}

	memset(&nm_pool,0,sizeof(nm_pool));
	nm_pool.base 					= base;
	nm_pool.objtotal 			= size;
	nm_pool.memtotal 			= size * objSize;
	nm_pool.bitmap 				= bitmap;
	nm_pool.bitmap_slots 	= bitmap_slots;
	nm_pool._objsize 			= objSize;


	return OFDPA_E_NONE;

}


/*
 * free by index, not by address.
 * XXX should we also cleanup the content ?
 */
static OFDPA_ERROR_t
dp_nm_obj_free(struct dp_nm_obj_pool *p, uint32_t j)
{
	uint32_t *ptr, mask;

	if (j >= p->objtotal) {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
			"invalid index %u, max %u\r\n", j, p->objtotal);
		return OFDPA_E_PARAM;
	}
	ptr = &p->bitmap[j / 32];
	mask = (1 << (j % 32));
	if (*ptr & mask) {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
			"ouch, double free on buffer %d\r\n", j);
		return OFDPA_E_EXISTS;
	} else {
		*ptr |= mask;
		p->objfree++;
		return OFDPA_E_NONE;
	}
}



/*
 * report the index, and use start position as a hint,
 * otherwise buffer allocation becomes terribly expensive.
 */
static void *
dp_nm_obj_malloc(struct dp_nm_obj_pool *p, u_int len, uint32_t *start, uint32_t *index)
{
	uint32_t i = 0;			/* index in the bitmap */
	uint32_t mask, j = 0;		/* slot counter */
	void *vaddr = NULL;

	if (len > p->_objsize) {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,"request size %d too large\r\n", len);
		return NULL;
	}

	if (p->objfree == 0) {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,"no more objects\r\n");
		return NULL;
	}
	if (start)
		i = *start;

	/* termination is guaranteed by p->free, but better check bounds on i */
	while (vaddr == NULL && i < p->bitmap_slots)  {
		uint32_t cur = p->bitmap[i];
		if (cur == 0) { /* bitmask is fully used */
			i++;
			continue;
		}
		/* locate a slot */
		for (j = 0, mask = 1; (cur & mask) == 0; j++, mask <<= 1)
			;

		p->bitmap[i] &= ~mask; /* mark object as in use */
		p->objfree--;

		vaddr = p->base + p->_objsize * (i * 32 + j);
		if (index)
			*index = i * 32 + j;
	}

	if (start)
		*start = i;
	return vaddr;
}


OFDPA_ERROR_t dpNetmapMemFree(uint32_t index)
{
	return dp_nm_obj_free(&nm_pool,index);
}


void * dpNetmapMemMalloc(u_int len, uint32_t *start, uint32_t *index)
{
	return dp_nm_obj_malloc(&nm_pool, len, start, index);
}








