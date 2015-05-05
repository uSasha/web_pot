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
void pump (void const *argument);                             // thread function

osThreadId 	t_pump;                                          // thread id
osThreadDef (pump, osPriorityNormal, 1, 0);                   // thread object

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
