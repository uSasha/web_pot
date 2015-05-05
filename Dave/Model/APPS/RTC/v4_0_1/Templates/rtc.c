/*******************************************************************************
 Copyright (c) 2015, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
                                                                              **
 Redistribution and use in source and binary forms, with or without           **
 modification,are permitted provided that the following conditions are met:   **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
                                                                              **
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon XMC4000/XMC1000 Series   			                  **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR   : APP Developer                                                   **
**                                                                            **
** MAY BE CHANGED BY USER [yes/no]: Yes                                       **
**                                                                            **
** MODIFICATION DATE : Feb 16, 2015                                           **
**                                                                            **
*******************************************************************************/
/**
* @file rtc.c
* @date 16 Feb, 2015
* @version 4.0.0
********************************************************************************
	** Version History:                                                       **
********************************************************************************
	** v4.0.0 16-Feb-15 Initial version
*******************************************************************************/ 

/**
	* @file rtc.c
	*
	*
	* @brief  Real Time Clock APP Implementation Source File.
	*
	*
*/

/*******************************************************************************
	** Include Files:                                                         **
******************************************************************************/
#include "rtc.h"
/*******************************************************************************
	** IMPLEMENTATION                                                         **
*******************************************************************************/
uint8_t RTC_lleapyear(uint16_t year);

bool RTC_lConfigureInterrupts(RTC_t *const handler);

XMC_RTC_STATUS_t RTC_lRegister_Callbacks(RTC_t *const handler);

/* Constant array used to store the number of days in each month */
const int32_t RTC_DAYS_IN_MONTH[13] =
{
	/* Index from 1, hence skip 0*/
	0,
	/*Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec*/
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* Constant used for the number of days in an year */
#define RTC_DAYS_IN_AN_YEAR     (365U)
/* Constant used for the seconds in a day */
#define RTC_SECONDS_IN_A_DAY    (24U * 60U * 60U)
/* Constant used for the seconds in an hour */
#define RTC_SECONDS_IN_AN_HOUR  (60U * 60U)
/* Constant used for the seconds in a minute */
#define RTC_SECONDS_IN_A_MINUTE (60U)
/* Constant used for the epoch year */
#define RTC_EPOCH_YEAR          (1970U)


/*
 * API to retrieve the version of the RTC APP
 */
DAVE_APP_VERSION_t RTC_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = RTC_MAJOR_VERSION;
  version.minor = RTC_MINOR_VERSION;
  version.patch = RTC_PATCH_VERSION;

  return version;
}

/*
	*  Initialization function for the APP. Configures the registers
	*  based on options selected in UI.
*/
RTC_STATUS_t RTC_Init(RTC_t *const handler)
{
	XMC_RTC_STATUS_t status;
	RTC_STATUS_t rtc_initstatus;
	bool interrupt_configured;

	status = XMC_RTC_STATUS_OK;
	rtc_initstatus = RTC_STATUS_FAILURE;

#if (RTC_INTERRUPT_ENABLED == 1)
#if (UC_FAMILY == XMC4)
	rtc_initstatus = (RTC_STATUS_t)GLOBAL_SCU_XMC4_Init(GLOBAL_SCU_HANDLE);
	if(rtc_initstatus == RTC_STATUS_SUCCESS)
	{
#else
	rtc_initstatus = (RTC_STATUS_t)GLOBAL_SCU_XMC1_Init(GLOBAL_SCU_HANDLE);
	if(rtc_initstatus == RTC_STATUS_SUCCESS)
	{
#endif
#endif
				if(handler->initialized == false)
				{
					/* Initialize the clock source and pre-scalar */
					status = XMC_RTC_Init((XMC_RTC_CONFIG_t *)(handler->time_alarm_config));

					if(status == XMC_RTC_STATUS_OK)
					{
						/* Configure periodic, alarm and hibernate periodic interrupts */
						interrupt_configured = RTC_lConfigureInterrupts(handler);

						if(interrupt_configured == true)
						{
							status = RTC_lRegister_Callbacks(handler);
						}

						if(status == XMC_RTC_STATUS_OK)
						{
							/* Check RTC start during init is set or not in UI */
							if(handler->config->start == RTC_START_ENABLE)
							{
								RTC_Start();
							}
							handler->initialized = true;
							rtc_initstatus = RTC_STATUS_SUCCESS;
						}
					}
					else
					{
						rtc_initstatus = RTC_STATUS_FAILURE;
					}
				}
				else
				{
					rtc_initstatus = RTC_STATUS_SUCCESS;
				}
#if (RTC_INTERRUPT_ENABLED == 1)
	} /* end of if(rtc_initstatus == GLOBAL_SCU_XMC4_STATUS_OK) */
#endif

	return (rtc_initstatus);
}
	/*
		*  This function configures periodic and alarm interrupts
	*/
	bool RTC_lConfigureInterrupts(RTC_t *const handler)
	{
		uint32_t regval;
		bool interrupt_configured = false;

		/* Enable periodic seconds, minutes, hours days, months and years interrupts */
		regval = (((uint32_t)handler->config->periodic_sec_intr << RTC_MSKSR_MPSE_Pos)|
		((uint32_t)handler->config->periodic_min_intr << RTC_MSKSR_MPMI_Pos) |
		((uint32_t)handler->config->periodic_hour_intr << RTC_MSKSR_MPHO_Pos) |
		((uint32_t)handler->config->periodic_day_intr << RTC_MSKSR_MPDA_Pos) |
		((uint32_t)handler->config->periodic_month_intr << RTC_MSKSR_MPMO_Pos)|
		((uint32_t)handler->config->periodic_year_intr << RTC_MSKSR_MPYE_Pos));

		/* Enable RTC periodic interrupt in SCU when any of the periodic interrupts are enabled */
		if(regval != 0U)
		{
			XMC_RTC_EnableEvent(regval);
			#if(UC_FAMILY == XMC4)
				XMC_SCU_INTERRUPT_EnableEvent(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC);
			#endif
			interrupt_configured = true;
		}

		#ifdef RTC_EVENTTRIGTOSCU
			if(handler->config->alarm_intr == RTC_ALARMINT_ENABLE)
			{
				XMC_RTC_EnableEvent(XMC_RTC_EVENT_ALARM);
				#if(UC_FAMILY == XMC4)
					XMC_SCU_INTERRUPT_EnableEvent(XMC_SCU_INTERRUPT_EVENT_RTC_ALARM);
				#endif
				interrupt_configured = true;
			}
		#endif

		return (interrupt_configured);
	}

	/*
		*  Interface to register the RTC call backs
	*/
	XMC_RTC_STATUS_t RTC_lRegister_Callbacks(RTC_t *const handler)
	{
		XMC_RTC_STATUS_t pi_status;
		XMC_RTC_STATUS_t ai_status;

		pi_status = XMC_RTC_STATUS_OK;
		ai_status = XMC_RTC_STATUS_OK;

#if (RTC_INTERRUPT_ENABLED == 1)
		#if(UC_FAMILY == XMC4)
			pi_status = (XMC_RTC_STATUS_t)GLOBAL_SCU_XMC4_RegisterCallback(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC,
																	  	   handler->config->pi_listener);

			if(handler->config->alarm_intr == RTC_ALARMINT_ENABLE)
			{
				ai_status = (XMC_RTC_STATUS_t)GLOBAL_SCU_XMC4_RegisterCallback(XMC_SCU_INTERRUPT_EVENT_RTC_ALARM,
																		       handler->config->ai_listener);
			}
		#else
			pi_status = (XMC_RTC_STATUS_t)GLOBAL_SCU_XMC1_RegisterCallback(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC,
																	  	   handler->config->pi_listener);

			if(handler->config->alarm_intr == RTC_ALARMINT_ENABLE)
			{
				ai_status = (XMC_RTC_STATUS_t)GLOBAL_SCU_XMC1_RegisterCallback(XMC_SCU_INTERRUPT_EVENT_RTC_ALARM,
																		       handler->config->ai_listener);
			}

		#endif
#endif

			return (pi_status & ai_status);
	}

	/*
 	 *  This function is used to set RTC time.
	 */
	RTC_STATUS_t RTC_SetTime(XMC_RTC_TIME_t *time)
	{
		RTC_STATUS_t status = RTC_STATUS_SUCCESS;

		if(time->days != 0 && time->month != 0)
		{
			time->days = time->days - 1;
			time->month = time->month - 1;

			XMC_RTC_SetTime(time);
		}
		else
		{
			status = RTC_STATUS_FAILURE;
		}

		return (status);

	}

	/*
 	 *  This function is used to get RTC time.
	 */
	void RTC_GetTime(XMC_RTC_TIME_t *time)
	{
		XMC_RTC_GetTime(time);

		time->days = time->days + 1;
		time->month = time->month + 1;
	}

	/*
 	 *  This function is used to set Alarm time.
	 */
	RTC_STATUS_t RTC_SetAlarmTime(XMC_RTC_ALARM_t *alarm)
	{
		RTC_STATUS_t status = RTC_STATUS_SUCCESS;

		if(alarm->days != 0 && alarm->month != 0)
		{
			alarm->days = alarm->days - 1;
			alarm->month = alarm->month - 1;

			XMC_RTC_SetAlarm(alarm);
		}
		else
		{
			status = RTC_STATUS_FAILURE;
		}

		return (status);
	}

	/*
 	 *  This function is used to get Alarm time from XMC.
 	 *  And returns in standard time format.
	 */
	void RTC_GetAlarmTime(XMC_RTC_ALARM_t *alarm)
	{
		XMC_RTC_GetAlarm(alarm);

		alarm->days = alarm->days + 1;
		alarm->month = alarm->month + 1;
	}

	/*
 	 *  This function is used to get event status.
	 */
	uint32_t RTC_GetFlagStatus(void)
	{
		uint32_t eventstatus;

		eventstatus = XMC_RTC_GetEventStatus();

		return eventstatus;
	}

	/*
		*  This function is to get the time in seconds calculated from Epoch time
		*  (01/01/1970).
	*/
	RTC_STATUS_t RTC_Time(time_t* time_value)
	{
		int32_t elapsedyear;
	    int32_t elapsedmonth;
		int32_t elapseddays;
		int32_t elapsedseconds;

		RTC_STATUS_t status;
		XMC_RTC_TIME_t curr_time;

		/*Check if RTC module is enabled and no NULL pointer*/
		if(((true == XMC_RTC_IsRunning()) && (time_value != NULL)))
		{
			/* Read values from TIM0 and TIM1 registers */
			XMC_RTC_GetTime(&curr_time);

			/*Count number of Days for Elapsed Years since Epoch*/
			elapseddays = (curr_time.year - RTC_EPOCH_YEAR) * RTC_DAYS_IN_AN_YEAR;

			/* Add the number of days to be adjusted for leap years,
			start from previous year and check backward */
			for(elapsedyear = (curr_time.year - 1); elapsedyear>= 1970; elapsedyear--)
			{
				if(RTC_lleapyear((uint16_t)elapsedyear))
				{
					elapseddays++;
				}
			}
			/*If current year is leap year add 1 only if March or later*/
			if(RTC_lleapyear((uint16_t)curr_time.year))
			{
				if(curr_time.month > 2)
				{
					elapseddays++;
				}
			}

			/*Add number of Days from Elapsed months from current year*/
			for(elapsedmonth = (curr_time.month); elapsedmonth != 0; elapsedmonth--)
			{
				elapseddays += RTC_DAYS_IN_MONTH[elapsedmonth];
			}

			/*Add Elapsed days from current month*/
			elapseddays += curr_time.days;

			/*Accumulate the total seconds for ElapsedDays*/
			elapsedseconds = (elapseddays * RTC_SECONDS_IN_A_DAY);

			/*Add seconds for current hour, minute and seconds*/
			elapsedseconds += (curr_time.hours * RTC_SECONDS_IN_AN_HOUR);
			elapsedseconds += (curr_time.minutes * RTC_SECONDS_IN_A_MINUTE);
			elapsedseconds += curr_time.seconds;

			*time_value = (time_t)elapsedseconds;
			status = RTC_STATUS_SUCCESS;
		}
		else
		{
		    status = RTC_STATUS_FAILURE;
		}
		return (status);
	}
	
	/* This function returns 1 if it is leap year otherwise 0.*/
	uint8_t RTC_lleapyear(uint16_t year)
	{
		uint8_t valid = 0U;

		if ((((year) % 400U) == 0U) || ((((year) % 100U) != 0U) &&
		(((year) %4U) == 0U)))
		{
			valid = 1U;
		}
		return valid;
	}
