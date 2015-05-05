/*******************************************************************************
**																			  **
**                                                                            **
** Copyright (C) 2015 Infineon Technologies AG. All rights reserved.          **
**                                                                            **
** Infineon Technologies AG (Infineon) is supplying this software for use     **
** with Infineon's microcontrollers.                                          **
** This file can be freely distributed within development tools that are      ** 
** supporting such microcontrollers.                                          **
**                                                                            **
** THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED **
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

/**
 * @file rtc.h
 * 
 * @brief  Real Time Clock APP implementation header file.
 *
 * VERSION HISTORY:
 * 16 Feb 2015	v4.0.0		Initial Version
 */

#ifndef RTC_H
#define RTC_H


/*******************************************************************************
** Include Files:                                                             **
*******************************************************************************/
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include "rtc_conf.h"
#include <DAVE_Common.h>
/*******************************************************************************
** Global Macro Definitions:                                                  **
*******************************************************************************/

/*******************************************************************************
** Global Type Definitions:                                                   **
*******************************************************************************/
/**
 * @ingroup RTC_enumerations
 * @{
 */

typedef enum RTC_STATUS
{
  RTC_STATUS_SUCCESS = 0U,	    /**< APP initialization status success */
  RTC_STATUS_FAILURE = 1U		/**< APP initialization status failure */
} RTC_STATUS_t;

/**
 * This enumeration defines RTC Module Enable and Disable values.
 */

typedef enum RTC_EnableType
{
  RTC_START_DISABLE = 0x0U,	/**< Disable RTC start */
  RTC_START_ENABLE = 0x1U	/**< Enable RTC start */
} RTC_EnableType;


/**
 * This enumeration defines RTC Module Interrupt Related Events Enable and
 * Disable values.
 */

typedef enum RTC_InterruptEnableType
{
  RTC_EVENTTRIGSCU_TIMER_ENABLE = 0x0U,		   /**< Event Trigger to SCU for Timer Enable */
  RTC_EVENTTRIGNMI_TIMER_ENABLE = 0x1U,		   /**< Event Trigger to NMI for Timer Enable */
  RTC_EVENTTRIGSCU_ALARM_ENABLE = 0x0U,		   /**< Event Trigger to SCU for Alarm Enable */
  RTC_EVENTTRIGNMI_ALARM_ENABLE = 0x1U,		   /**< Event Trigger to NMI for Alarm Enable */
  RTC_PERIODICSECINT_DISABLE = 0x0U,		   /**< Timer Periodic Seconds Interrupt Disable */
  RTC_PERIODICSECINT_ENABLE = 0x1U,		       /**< Timer Periodic Seconds Interrupt Enable */
  RTC_PERIODICMININT_DISABLE = 0x0U,		   /**< Timer Periodic Minutes Interrupt Disable */
  RTC_PERIODICMININT_ENABLE = 0x1U,			   /**< Timer Periodic Minutes Interrupt Enable */
  RTC_PERIODICHOURINT_DISABLE = 0x0U,		   /**< Timer Periodic Hours Interrupt Disable */
  RTC_PERIODICHOURINT_ENABLE = 0x1U,		   /**< Timer Periodic Hours Interrupt Enable */
  RTC_PERIODICDAYINT_DISABLE = 0x0U,		   /**< Timer Periodic Days Interrupt Disable */
  RTC_PERIODICDAYINT_ENABLE = 0x1U,			   /**< Timer Periodic Days Interrupt Enable */
  RTC_PERIODICMONTHINT_DISABLE = 0x0U,		   /**< Timer Periodic Month Interrupt Disable */
  RTC_PERIODICMONTHINT_ENABLE = 0x1U,		   /**< Timer Periodic Month Interrupt Enable */
  RTC_PERIODICYEARINT_DISABLE = 0x0U,		   /**< Timer Periodic Years Interrupt Disable */
  RTC_PERIODICYEARINT_ENABLE = 0x1U,		   /**< Timer Periodic Years Interrupt Enable */
  RTC_ALARMINT_DISABLE = 0x0U,				   /**< Alarm Interrupt Disable */
  RTC_ALARMINT_ENABLE = 0x1U				   /**< Alarm Interrupt Enable */
} RTC_InterruptEnableType;


/**
 * Type of Call back function for Alarm Event.
 * A void argument can be optionally used.
 */
typedef void (*RTC_AlarmCallBackPtr)(void);

/**
 * Type of Call back function for Periodic Timer Event.
 * A void argument can be optionally used.
 *
 */
typedef void (*RTC_TimerCallBackPtr)(void);

/**
  * @ingroup RTC_datastructures
  * @{
  */

/**
 * RTC APP Handle structure. Stores configured parameters in APP UI.
 */

typedef struct RTC_CONFIG
{
 RTC_EnableType start;										  /**< rtc start */

 RTC_InterruptEnableType eventtrig_timer;					  /**< event trigger for timer */
 RTC_InterruptEnableType periodic_sec_intr;					  /**< periodic seconds interrupt */
 RTC_InterruptEnableType periodic_min_intr;					  /**< periodic minutes interrupt */
 RTC_InterruptEnableType periodic_hour_intr;				  /**< periodic hours interrupt */
 RTC_InterruptEnableType periodic_day_intr;					  /**< periodic days interrupt */
 RTC_InterruptEnableType periodic_month_intr;				  /**< periodic months interrupt */
 RTC_InterruptEnableType periodic_year_intr;				  /**< periodic years interrupt */

 RTC_InterruptEnableType alarm_intr;						  /**< alarm interrupt */

 RTC_InterruptEnableType eventtrig_alarm;					  /**< event trigger for alarm */

 RTC_TimerCallBackPtr pi_listener;							  /**< periodic interrupt listener */

 RTC_AlarmCallBackPtr ai_listener;							  /**< alarm interrupt listener */

} RTC_CONFIG_t;

/**
 * Runtime RTC handler.
 */
typedef struct RealTimeClock
{
  const XMC_RTC_CONFIG_t *const time_alarm_config; /** Pointer to RTC LLD configuration data */
  const RTC_CONFIG_t *const config;                /** Pointer to configuration data */
  bool initialized;							       /** status for initialized or not */
} RTC_t;
/**
 *@}
 */

#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
** Function ProtoTypes:                                                       **
*******************************************************************************/
/**
 * @ingroup RTC_apidoc
 * @{
 */

/**
 * @brief Get RTC APP version.
 * @return @ref DAVE_APP_VERSION_t APP version information (major, minor and patch number)
 *
 * \par<b>Description: </b><br>
 * The function can be used to check application software compatibility with a
 * specific version of the APP.
 *
 * @code
 * #include <DAVE.h>
 * #include <DAVE_common.h>
 *  int main(void)
 *  {
 *    DAVE_APP_VERSION_t version;
 *    DAVE_Init();
 *    version = RTC_GetAppVersion();
 *    if(version.major != 4U)
 *    {
 *    }
 *    return 0;
 *  }
 * @endcode<BR> </p>
 */
DAVE_APP_VERSION_t RTC_GetAppVersion(void);


 /**
 * @brief Initializes RTC module.
 * @param handle Constant pointer to RTC structure of type @ref RTC_t
 * @return @ref RTC_STATUS_t
 *
 * \par<b>Description:</b><br>
 * Configures the RTC module registers as per settings updated in
 * UI and enables the RTC module.
 *
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite: Instantiate RTC APP and generate code before copying below code snippet.
 * @code
 *  #include <DAVE.h>
 *  int main(void)
 *  {
 *    DAVE_Init(); // RTC_Init(&RTC_0) will be called within DAVE_Init()
 *    // ... infinite loop ...
 *    while(1)
 *    {}
 *  }
 * @endcode<BR> </p> 
 */
RTC_STATUS_t RTC_Init(RTC_t *const handle);

/**
 * @brief Returns the time in seconds since the epoch time(01/01/1970).
 * @param time_value Pointer to structure of type @ref time_t
 * @return @ref RTC_STATUS_t
 *
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite: Instantiate RTC APP and generate code before copying below code snippet.
 * @code 
 * #include <DAVE.h>
 * int main(void)
 * {
 *   RTC_STATUS_t status = RTC_STATUS_FAILURE;
 *   time_t Time_Sec;
 *   DAVE_Init();  // RTC_Init(&RTC_0) will be called within DAVE_Init()
 *   status = RTC_Time(&Time_Sec);
 *   if(status != RTC_STATUS_SUCCESS)
 *   {
 *	   //error
 *   }
 *   // ... infinite loop ...
 *   while(1)
 *   {}
 * }
 * @endcode<BR> </p> 
 */
RTC_STATUS_t RTC_Time(time_t* time_value) ;

/**
 * @brief Starts the RTC running
 * @return None
 *
 *\par<b>Description: </b><br>
 * Before verifying this API it is required to deselect "Start after initialization" checbox in UI.
 * Thereby this API shall be used to start the RTC module.
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite:
 * 1. Instantiate RTC APP <br>
 * 2. Unselect "Start after initialization checkbox in UI" <br>
 * 3. Generate code before copying below code snippet. <br>
 * @code
 * #include <DAVE.h>
 * int main(void)
 * {
 *   DAVE_Init();// RTC_Init(&RTC_0) will be called within DAVE_Init()
 *   RTC_Stop();
 *   if (XMC_RTC_IsRunning() == false)
 *   {
 *     RTC_Start();
 *   }
 *   // ... infinite loop ...
 *   while(1)
 *   {}
 * }
 * @endcode<BR> </p>
 */
__STATIC_INLINE void RTC_Start(void)
{
	XMC_RTC_Start();
}

/**
 * @brief Stops the RTC running
 * @return None
 *
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite: Instantiate RTC APP <br>
 * Enable periodic seconds event in "Interrupt Settings" tab. <br>
 * Generate code before copying below code snippet.
 *
 * @code
 * #include <DAVE.h>
 *
 * void Time_Handler(void)
 * {
 *    static uint32_t seconds = 0;
 *    seconds++;
 *
 *    if(seconds == 30)
 *    {
 *      RTC_Stop();
 *    }
 * }
 * int main(void)
 * {
 *   DAVE_Init(); // RTC_Init(&RTC_0) will be called within DAVE_Init()
 *   // ... infinite loop ...
 *   while(1)
 *   {}
 * }
 * @endcode<BR> </p>
 */
__STATIC_INLINE void RTC_Stop(void)
{
	XMC_RTC_Stop();
}

/**
 * @brief Sets the time
 * @param time Pointer to structure of type @ref XMC_RTC_TIME_t
 * @return @ref RTC_STATUS_t
 *
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite: Instantiate RTC APP and generate code before copying below code snippet.
 * @code
 * #include <DAVE.h>
 * XMC_RTC_TIME_t timeval =
 * {
 *    .seconds = 0U,
 *    .minutes = 0U,
 *    .hours = 0U,
 *    .days = 1U,
 *    .month = 1,
 *    .year = 1970U
 * };
 * int main(void)
 * {
 *   RTC_STATUS_t status = RTC_STATUS_FAILURE;
 *   DAVE_Init();
 *   RTC_Stop();
 *   status = RTC_SetTime(&timeval);
 *   if(status != RTC_STATUS_SUCCESS)
 *   {
 *	   //error
 *   }
 *   // ... infinite loop ...
 *   while(1)
 *   {}
 * }
 * @endcode<BR> </p>
 */
RTC_STATUS_t RTC_SetTime(XMC_RTC_TIME_t *time);
/**
 * @brief Sets the alarm time
 * @param alarm Pointer to structure of type @ref XMC_RTC_ALARM_t
 * @return @ref RTC_STATUS_t
 *
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite: Instantiate RTC APP and generate code before copying below code snippet.
 * @code
 * #include <DAVE.h>
 * XMC_RTC_ALARM_t alarm =
 * {
 *    .seconds = 0U,
 *    .minutes = 1U,
 *    .hours = 0U,
 *    .days = 1U,
 *    .month = 1,
 *    .year = 1970U
 * };
 * int main(void)
 * {
 *   RTC_STATUS_t status = RTC_STATUS_FAILURE;
 *   DAVE_Init();
 *   RTC_Stop();
 *   status = RTC_SetAlarmTime(&alarm);
 *   if(status != RTC_STATUS_SUCCESS)
 *   {
 *	   //error
 *   }
 *   // ... infinite loop ...
 *   while(1)
 *   {}
 * }
 * @endcode<BR> </p>
 */
RTC_STATUS_t RTC_SetAlarmTime(XMC_RTC_ALARM_t *alarm);
/**
 * @brief Gets the time
 * @param time Pointer to structure of type @ref XMC_RTC_TIME_t
 * @return None
 *
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite: Instantiate RTC APP and generate code before copying below code snippet.
 * @code
 *
 * #include <DAVE.h>
 * XMC_RTC_TIME_t timeval =
 * {
 *   .seconds = 0U,
 *   .minutes = 0U,
 *   .hours = 0U,
 *   .days = 1U,
 *   .month = 1,
 *   .year = 1970U
 * };
 * int main(void)
 * {
 *  RTC_STATUS_t status = RTC_STATUS_FAILURE;
 *  DAVE_Init();
 *  RTC_Stop();
 *  status = RTC_SetTime(&timeval);
 *  if(status == RTC_STATUS_SUCCESS)
 *  {
 *    RTC_GetTime(&timeval);
 *  }
   // ... infinite loop ...
   while(1)
   {}
 }

 * @endcode<BR> </p>
 */
void RTC_GetTime(XMC_RTC_TIME_t *time);
/**
 * @brief Gets the alarm time
 * @param time Pointer to structure of type @ref XMC_RTC_ALARM_t
 * @return None
 *
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite: Instantiate RTC APP and generate code before copying below code snippet.
 * @code
  * #include <DAVE.h>
 * XMC_RTC_ALARM_t alarm =
 * {
 *   .seconds = 0U,
 *   .minutes = 1U,
 *   .hours = 0U,
 *   .days = 1U,
 *   .month = 1,
 *   .year = 1970U
 * };
 * int main(void)
 * {
 *  RTC_STATUS_t status = RTC_STATUS_FAILURE;
 *  DAVE_Init();
 *  RTC_Stop();
 *  status = RTC_SetAlarmTime(&alarm);
 *  if(status == RTC_STATUS_SUCCESS)
 *  {
 *    RTC_GetAlarmTime(&alarm);
 *  }
 *   // ... infinite loop ...
 *   while(1)
 *   {}
 * }
 * @endcode<BR> </p>
 */
void RTC_GetAlarmTime(XMC_RTC_ALARM_t *alarm);
/**
 * @brief Gets the flag status
 * @param None
 * @return uint32_t flag status
 *
 * <P ALIGN="LEFT"><B>Example:</B>
 * Pre-requisite: Instantiate RTC APP and generate code before copying below code snippet.
 * @code
 * #include <DAVE.h>
 * int main(void)
 * {
 *   DAVE_Init();	// RTC_Init(&RTC_0); is called inside DAVE_Init()
 *   if(RTC_GetFlagStatus()& RTC_STSSR_SPSE_Msk) // check if the periodic seconds event has occurred.
 *   {
 *   }
 *   while(1)
 *   {}
 * }
 * @endcode<BR> </p>
 */
uint32_t RTC_GetFlagStatus(void);

/**
 *@}
 */

#include "rtc_extern.h"

#ifdef __cplusplus
}
#endif

#endif


