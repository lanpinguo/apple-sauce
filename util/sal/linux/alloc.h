/*
 * $Id: alloc.h,v 1.8 Broadcom SDK $
 * $Copyright: Copyright 2016 Broadcom Corporation.
 * This program is the proprietary software of Broadcom Corporation
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * File: 	alloc.h
 * Purpose: 	Memory allocation
 */

#ifndef _SAL_ALLOC_H
#define _SAL_ALLOC_H

/*
 * SAL Memory and Cache Support
 *
 *    NOTE: This driver was developed only on fully cache-coherent
 *    systems.  Therefore, we are certain to have missed a lot of places
 *    where we should be calling sal_dma_flush or sal_dma_inval before
 *    or after DMA operations.
 */

extern void *sal_alloc(unsigned int, char *);
extern void  sal_free(void *);
extern void sal_get_alloc_counters(unsigned long *alloc_bytes_count,unsigned long *free_bytes_count);

/*
 * DMA Memory allocation
 *
 */

extern void	*sal_dma_alloc(unsigned int, char *);
extern void	sal_dma_free(void *);

extern void     sal_dma_flush(void* addr, int len);
extern void     sal_dma_inval(void* addr, int len);
extern void	*sal_dma_vtop(void* addr);
extern void	*sal_dma_ptov(void* addr);

#define DISPLAY_MEM
#define DISPLAY_MEM_PRINTF(x)
#ifdef BROADCOM_DEBUG
/* { */
#ifdef INCLUDE_BCM_SAL_PROFILE
/* { */
extern void
sal_alloc_resource_usage_get(unsigned int *alloc_curr, unsigned int *alloc_max);

#ifndef __KERNEL__
/* { */
#if DISPLAY_MEM_USAGE
/* { */
#undef DISPLAY_MEM
/*
 * Display total memory allocated by sal_alloc() so far and maximum
 * allocated so far.
 */
#define DISPLAY_MEM \
    { \
        uint32 alloc_curr ; \
        uint32 alloc_max ; \
        sal_alloc_resource_usage_get(&alloc_curr, &alloc_max) ; \
        sal_printf( \
            "\r\n" \
            "%s(),%d: Heap memory consumers - curr/max %lu/%lu bytes\r\n\n", \
                            __FUNCTION__,__LINE__,(unsigned long)alloc_curr,(unsigned long)alloc_max) ; \
    }
#undef DISPLAY_MEM_PRINTF
#define DISPLAY_MEM_PRINTF(x) sal_printf x ;
/* } */
#endif
/* } */
#endif /* !__KERNEL__ */
/* } */
#endif
/* } */
#endif

#endif	/* !_SAL_ALLOC_H */
