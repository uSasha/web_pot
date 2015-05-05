/*
 * Copyright (C) 2015 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

/**
 * @file xmc4_rtc.h
 * @date 20 Feb, 2015
 * @version 1.0.2
 *
 * History <br>
 *
 * Version 1.0.0 Initial <br>
 * Version 1.0.2  Documentation updates <br>
 */

#ifndef XMC4_RTC_H
#define XMC4_RTC_H

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup RTC
 * @{
 */

/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * Wakeup events for RTC from hibernate domain
 */
typedef enum XMC_RTC_WAKEUP_EVENT
{
  XMC_RTC_WAKEUP_EVENT_ON_ALARM   = RTC_CTR_TAE_Msk,  /**< Wakeup from alarm event */
  XMC_RTC_WAKEUP_EVENT_ON_SECONDS = RTC_CTR_ESEC_Msk, /**< Wakeup from seconds event */
  XMC_RTC_WAKEUP_EVENT_ON_MINUTES = RTC_CTR_EMIC_Msk, /**< Wakeup from minutes event */
  XMC_RTC_WAKEUP_EVENT_ON_HOURS   = RTC_CTR_EHOC_Msk, /**< Wakeup from hours event */
  XMC_RTC_WAKEUP_EVENT_ON_DAYS    = RTC_CTR_EDAC_Msk, /**< Wakeup from days event */
  XMC_RTC_WAKEUP_EVENT_ON_MONTHS  = RTC_CTR_EMOC_Msk, /**< Wakeup from months event */
  XMC_RTC_WAKEUP_EVENT_ON_YEARS   = RTC_CTR_EYEC_Msk  /**< Wakeup from years event */
} XMC_RTC_WAKEUP_EVENT_t;

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param event A valid RTC Wakeup event (::XMC_RTC_WAKEUP_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable Wakeup from hibernate mode <br>
 *
 * \par
 * The function sets the bitfields of CTR register to enable wakeup from hibernate mode.  
 * Setting the masking value for the RTC wakeup events containing in the ::XMC_RTC_WAKEUP_EVENT_t 
 * leads to a wakeup from hibernate mode.
 * 
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_DisableHibernationWakeUp()
 */
__STATIC_INLINE void XMC_RTC_EnableHibernationWakeUp(const uint32_t event)
{
  RTC->CTR |= event;
}

/**
 * @param event A valid RTC Wakeup event (::XMC_RTC_WAKEUP_EVENT_t) or a valid combination of
 *              logically OR'd events          
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable Wakeup from hibernate mode <br>
 *
 * \par
 * The function resets the bitfields of CTR register to disable wakeup from hibernate mode.  
 * Resetting the masking value for the RTC wakeup events containing in the ::XMC_RTC_WAKEUP_EVENT_t  
 * disables wakeup from hibernate mode.
 * 
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_EnableHibernationWakeUp()
 */
__STATIC_INLINE void XMC_RTC_DisableHibernationWakeUp(const uint32_t event)
{
  RTC->CTR &= ~event;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* XMC4_RTC_H */
