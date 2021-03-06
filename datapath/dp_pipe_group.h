/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_group.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/3/8
  Last Modified :
  Description   : group header
  Function List :
  History       :
  1.Date        : 2018/3/8
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



#ifndef INCLUDE_DP_PIPE_GROUP_H
#define INCLUDE_DP_PIPE_GROUP_H

#include "ofdpa_list.h"



/*  Group Table Status. This information is aggregated for all Groups.
*/
typedef struct ofdpaGroupTableStatus_s
{
  uint32_t group_database_size; /* Maximum number of group records in the database */
  uint32_t num_groups_in_database; /* Current number of group records in the database */

  /* Maximum groups in each group table  */
  uint32_t groupTableMaxCount[OFDPA_GROUP_ENTRY_TYPE_LAST];

  /* Current number of groups in each group table  */
  uint32_t groupTableEntryCount[OFDPA_GROUP_ENTRY_TYPE_LAST];

} ofdpaGroupTableStatus_t;


/*  Group Bucket Table Status. This information is aggregated for all Groups.
*/
typedef struct ofdpaGroupBucketTableStatus_s
{
  uint32_t group_bucket_ref_database_size; /* Maximum number of group bucket records in the database */
  uint32_t group_bucket_data_database_size; /* Maximum number of group bucket records in the database */

  /* Maximum buckets of each type */
  uint32_t groupBucketTableMaxCount[OFDPA_GROUP_ENTRY_TYPE_LAST];

} ofdpaGroupBucketTableStatus_t;



typedef struct ofdpaGrpPipeNodeConfig_s
{
	void 												*tpool;
	uint32_t										max_subThread;
	int 												nodeSock;
	pthread_t 									nodeTid ;


	ofdpaGroupTableStatus_t			*grpStatus;
	ofdpaGroupBucketTableStatus_t	 *grpBuktStatus;
  avlTree_t 									ofdbGroupTable_tree;

}ofdpaGrpPipeNodeConfig_t;



#define DP_ADD_ACTION_TO_BUCKET(pBukt,pAct) \
{ \
	rv = dpAddAct2Bukt(pBukt,pAct); \
	if(rv != OFDPA_E_NONE){ \
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC, \
											 "Add action failed, rv = %d!\r\n", rv); \
		return rv; \
	} \
}while(0)


typedef struct dpGrpWork_s
{
		ofdpaPktCb_t *pPkt;
		ofdpaGrpPipeNode_t * pGrp;
}dpGrpWork_t;


/********************************** EXPPORT API **********************************************/
extern ofdpaGrpPipeNodeConfig_t grp_pipe_config;

OFDPA_ERROR_t dpGrpSubWorkDispatch(dpGrpWork_t	*work);



#endif /* INCLUDE_DP_PIPE_GROUP_H */




