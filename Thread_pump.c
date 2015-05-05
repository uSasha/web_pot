
#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "rtc.h"                        // Infineon::DAVE:RTC
#include "pwm.h"                        // Infineon::DAVE:PWM

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void pump (void const *argument);                             // thread function
osThreadId 	t_pump;                                          // thread id
osThreadDef (pump, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread_Pump (void) {

  t_pump 			= osThreadCreate(osThread(pump), 		NULL);  /* start task 'pump' */
  if(!t_pump) return(-1);
  
  return(0);
}

void pump (void const *argument) {
  while (1) {
		XMC_RTC_TIME_t time;
		RTC_GetTime(&time);
		PWM_SetDutyCycle(&PWM_0, time.seconds);
		PWM_Start(&PWM_0);
		osDelay(2000);
		PWM_Stop(&PWM_0);
//		osDelay(2000);               // suspend thread
		osSignalWait (0x0001, osWaitForever); /* wait for an event flag 0x0001   */
  }
}
