/*******************************************************************************
 *
 * Copyright (C) 2015 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon XMC4000 Series                                         **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR   : DAVE APP Developer                                              **
**                                                                            **
** MAY BE CHANGED BY USER [yes/no]: Yes                                       **
**                                                                            **
** MODIFICATION DATE :  16 Feb 2015                                           **
**                                                                            **
*******************************************************************************/
/**
 * @file   CPU_CTRL_XMC4.h
 *
 * @brief Header file for the CPU_CTRL_XMC4 APP.
 *
 * It provides enums,function prototypes and inline function definitions.
 *
 * VERSION HISTORY:
 * 16 Feb 2015	v4.0.0		Initial Version
 */
/**
 * @addtogroup CPU_CTRL_XMC4
 * @{
 */

#ifndef CPU_CTRL_XMC4_H
#define CPU_CTRL_XMC4_H

/*******************************************************************************
**                      Include Files                                         **
*******************************************************************************/
#include <xmc_common.h>
#include <DAVE_Common.h>
#include "cpu_ctrl_xmc4_conf.h"
/*******************************************************************************
 * MACROS
 *******************************************************************************/

/*******************************************************************************
**                             Enumerations                                   **
*******************************************************************************/
/**
 * @ingroup CPU_CTRL_XMC4_enumerations
 * @{
 */
/*
 * @brief enumeration for CPU_CTRL_XMC4 APP
 */
typedef enum CPU_CTRL_XMC4_STATUS
{
  CPU_CTRL_XMC4_STATUS_SUCCESS = 0U,       /**<APP initialization is success */
  CPU_CTRL_XMC4_STATUS_FAILURE = 1U        /**<APP initialization is failure */
} CPU_CTRL_XMC4_STATUS_t;

/**
 * @}
 */

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/
/**
 * @ingroup CPU_CTRL_XMC4_datastructures
 * @{
 */
/**
 * @brief configuration structure for CPU_CTRL_XMC4 APP
 */
typedef struct CPU_CTRL_XMC4_CONFIG
{
  uint8_t prigroup;	/**<This holds the priority group value. */
} CPU_CTRL_XMC4_CONFIG_t;

/*
 * @brief Init structure for CPU_CTRL_XMC4 APP
 */
typedef struct CPU_CTRL_XMC4
{
  const CPU_CTRL_XMC4_CONFIG_t *const config;  /**<APP configuration handle pointer */
  bool initialized;							   /**<APP is initialized or not. */
} CPU_CTRL_XMC4_t;

/**
 * @}
 */

/*******************************************************************************
 * FUNCTION PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup CPU_CTRL_XMC4_apidoc
 * @{
 */

/**
 * @brief Get CPU_CTRL_XMC4 APP version
 * @return DAVE_APP_VERSION_t APP version information (major, minor and patch number)
 *
 * \par<b>Description: </b><br>
 * The function can be used to check application software compatibility with a
 * specific version of the APP.
 *
 * Example Usage:
 *
 * @code
 * #include <DAVE.h>
 *
 * int main(void) {
 *   DAVE_APP_VERSION_t version;
 *   // CPU_CTRL_XMC4_Init() is called from within DAVE_Init().
 *   init_status = DAVE_Init();
 *
 *   version = CPU_CTRL_XMC4_GetAppVersion();
 *   if (version.major != 4U) {
 *   }
 *
 *   // More code here
 *   while(1) {
 *
 *   }
 *   return (0);
 * }
 * @endcode
 */
DAVE_APP_VERSION_t CPU_CTRL_XMC4_GetAppVersion(void);


/**
 * @brief Initializes the CPU_CTRL_XMC4 APP
 * @param handler Constant pointer for structure of type @ref CPU_CTRL_XMC4_t
 * @return @ref CPU_CTRL_XMC4_STATUS_t
 * @code
 *
 * #include <DAVE.h>
 * int main (void)
 * {
 *
 * DAVE_Init();  //CPU_CTRL_XMC4_Init is called from DAVE_Init()
 * while(1)
 * {}
 * return 0;
 * }
 * @endcode
 */
CPU_CTRL_XMC4_STATUS_t CPU_CTRL_XMC4_Init(CPU_CTRL_XMC4_t *const handler);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#include "cpu_ctrl_xmc4_extern.h"


#endif /* CPU_CTRL_XMC4_H */

/**
 * @} (end addtogroup CPU_CTRL_XMC4)
 */
