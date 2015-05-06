/*----------------------------------------------------------------------------
 Water pump thread.
 Small water pump is based on DC motor, so it could be drived by simple PWM.
 
 Thread should wait for external signal and then turn on pump for desired time.
 *---------------------------------------------------------------------------*/

#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "rtc.h"                        // Infineon::DAVE:RTC
#include "pwm.h"                        // Infineon::DAVE:PWM
#define PWM_DUTY_CYCLE	40		// percent
#define PUMP_ON_TIME		2000	// microseconds
#define TICKS_PER_SECOND 1000000
#define MILLILITERS_PER_SECOND	100 // TODO find and update

uint32_t computePumpOnTicks(void);
void pump (void const *argument);                             // thread function

osThreadId 	t_pump;                                          // thread id
osThreadDef (pump, osPriorityNormal, 1, 0);                   // thread object
static uint16_t watering_hour = 12;	// hour 24 hour/day format
static bool watering_days[7] 	= {false, false, false, false, false, false, false};
static uint16_t watering_milliliters_per_week = 0;

int Init_Thread_Pump (void) {
  t_pump = osThreadCreate(osThread(pump), 		NULL);  /* start task 'pump' */
  if(!t_pump) return(-1);
  
  return(0);
}

void pump (void const *argument) {
  while (1) {
		PWM_SetDutyCycle(&PWM_0, PWM_DUTY_CYCLE);
		PWM_Start(&PWM_0);
		osDelay(PUMP_ON_TIME);
		PWM_Stop(&PWM_0);
		osSignalWait (0x0001, osWaitForever); 			/* wait for an event flag 0x0001   */
  }
}

void watering_setTime(int16_t hour_24hd)
{
	watering_hour = hour_24hd;
}

void watering_daysSet(uint8_t weekday)
{
	watering_days[weekday] = true;
}

void watering_daysReset(uint8_t weekday)
{
	watering_days[weekday] = false;
}

bool watering_isItTime(void)
{
	XMC_RTC_TIME_t current_time;
	RTC_GetTime(&current_time);

	return ((current_time.hours == watering_hour) 
			&& (watering_days[current_time.daysofweek]));
}

uint32_t computePumpOnTicks(void)
{
	uint8_t days_per_week = 0;
	for(uint8_t i = 0; i <= 6; i++)
	{
		if(watering_days[i] == true)
		{
			days_per_week++;
		}
	}
	return TICKS_PER_SECOND * watering_milliliters_per_week / days_per_week / MILLILITERS_PER_SECOND;
}

void watering_setWeekAmount(uint16_t milliliters)
{
	watering_milliliters_per_week = milliliters;
}
