/*******************************************************************************
**  DAVE App Name : RTC       App Version: 4.0.1               
**  This file is generated by DAVE, User modification to this file will be    **
**  overwritten at the next code generation.                                  **
*******************************************************************************/


/*******************************************************************************
**                                                                            **
** Copyright (C) 2015 Infineon Technologies AG. All rights reserved.          **
**                                                                            **
** Infineon Technologies AG (Infineon) is supplying this software for use     **
** with Infineon's microcontrollers.                                          **
** This file can be freely distributed within development tools that are      **
** supporting such microcontrollers.                                          **
**                                                                            **
** THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,IMPLIED  **
** OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF         **
** MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS         **
** SOFTWARE. INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR          **
** SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.  **
**                                                                            **
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon XMC4000/XMC1000 Series                                 **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR   : APP Developer                                                   **
**                                                                            **
** NOTE     : Any Changes made to this file will be overwritten during Code   **
**            Generation                                                      **
**                                                                            **
**                                                                            **
*******************************************************************************/


/*******************************************************************************
** Version History:                                                           **
********************************************************************************
** v4.0.0    16 Feb 2015  Initial version                                     **
*******************************************************************************/
/**
 *
 * @file rtc_conf.h
 *
 *
 *
 * @brief Real Time Clock APP implementation configuration file.
 *        This file contains other dependent fle inclusions and
 *        macros.
 *
*/

#ifndef RTC_CONF_H
#define RTC_CONF_H

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************
 ** @Project Includes:                                                      **
 *****************************************************************************/

#define RTC_MAJOR_VERSION 4U
#define RTC_MINOR_VERSION 0U
#define RTC_PATCH_VERSION 1U



#if (UC_FAMILY == XMC4)
#include "GLOBAL_SCU_XMC4/global_scu_xmc4.h"
#else
#include "GLOBAL_SCU_XMC1/global_scu_xmc1.h"
#endif
#define RTC_INTERRUPT_ENABLED  1

#define RTC_EVENTTRIGTOSCU

#define DEFAULT_DIVIDERVALUE 0x7FFFU

#ifdef __cplusplus
}
#endif

#endif  /* ifndef RTC_CONF_H */
