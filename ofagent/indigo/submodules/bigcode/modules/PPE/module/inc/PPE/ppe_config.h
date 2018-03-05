/****************************************************************
 *
 *        Copyright 2013, Big Switch Networks, Inc.
 *
 * Licensed under the Eclipse Public License, Version 1.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *        http://www.eclipse.org/legal/epl-v10.html
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the
 * License.
 *
 ***************************************************************/

/**
 * @file
 * @brief PPE Configuration Definitions.
 *
 *
 * @addtogroup ppe-config
 * @{
 *
 */

#ifndef __PPE_CONFIG_H__
#define __PPE_CONFIG_H__

#include <stdint.h>

#ifdef GLOBAL_INCLUDE_CUSTOM_CONFIG
#include <global_custom_config.h>
#endif
#ifdef PPE_INCLUDE_CUSTOM_CONFIG
#include <ppe_custom_config.h>
#endif




/* <auto.start.cdefs(PPE_CONFIG_HEADER).header> */
#include <AIM/aim.h>
/**
 * PPE_CONFIG_INCLUDE_LOGGING
 *
 * Include or exclude logging. */


#ifndef PPE_CONFIG_INCLUDE_LOGGING
#define PPE_CONFIG_INCLUDE_LOGGING 1
#endif

/**
 * PPE_CONFIG_LOG_OPTIONS_DEFAULT
 *
 * Default enabled log options. */


#ifndef PPE_CONFIG_LOG_OPTIONS_DEFAULT
#define PPE_CONFIG_LOG_OPTIONS_DEFAULT AIM_LOG_OPTIONS_DEFAULT
#endif

/**
 * PPE_CONFIG_LOG_BITS_DEFAULT
 *
 * Default enabled log options. */


#ifndef PPE_CONFIG_LOG_BITS_DEFAULT
#define PPE_CONFIG_LOG_BITS_DEFAULT AIM_LOG_BITS_DEFAULT
#endif

/**
 * PPE_CONFIG_LOG_CUSTOM_BITS_DEFAULT
 *
 * Default enabled log options. */


#ifndef PPE_CONFIG_LOG_CUSTOM_BITS_DEFAULT
#define PPE_CONFIG_LOG_CUSTOM_BITS_DEFAULT 0
#endif

/**
 * PPE_CONFIG_PORTING_STDLIB
 *
 * Default all porting macros to use the C standard libraries. */


#ifndef PPE_CONFIG_PORTING_STDLIB
#define PPE_CONFIG_PORTING_STDLIB 1
#endif

/**
 * PPE_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
 *
 * Include standard library headers for stdlib porting macros. */


#ifndef PPE_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
#define PPE_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS PPE_CONFIG_PORTING_STDLIB
#endif

/**
 * PPE_CONFIG_INCLUDE_CUSTOM_META_FIELDS
 *
 * Include custom metafield definitions for your own application. */


#ifndef PPE_CONFIG_INCLUDE_CUSTOM_META_FIELDS
#define PPE_CONFIG_INCLUDE_CUSTOM_META_FIELDS 0
#endif

/**
 * PPE_CONFIG_INCLUDE_UCLI
 *
 * Include Generic UCLI support. */


#ifndef PPE_CONFIG_INCLUDE_UCLI
#define PPE_CONFIG_INCLUDE_UCLI 0
#endif

/**
 * PPE_CONFIG_INCLUDE_UTM
 *
 * Include the PPE unit test module. */


#ifndef PPE_CONFIG_INCLUDE_UTM
#define PPE_CONFIG_INCLUDE_UTM 0
#endif



/**
 * All compile time options can be queried or displayed
 */

/** Configuration settings structure. */
typedef struct ppe_config_settings_s {
    /** name */
    const char* name;
    /** value */
    const char* value;
} ppe_config_settings_t;

/** Configuration settings table. */
/** ppe_config_settings table. */
extern ppe_config_settings_t ppe_config_settings[];

/**
 * @brief Lookup a configuration setting.
 * @param setting The name of the configuration option to lookup.
 */
const char* ppe_config_lookup(const char* setting);

/**
 * @brief Show the compile-time configuration.
 * @param pvs The output stream.
 */
int ppe_config_show(struct aim_pvs_s* pvs);

/* <auto.end.cdefs(PPE_CONFIG_HEADER).header> */



#include "ppe_porting.h"
#endif /* __PPE_CONFIG_H__ */
/*@}*/

