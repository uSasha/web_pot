
#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "adc_measurement.h"            // Infineon::DAVE:ADC_MEASUREMENT

extern osThreadId t_ledOff;                    /* assigned task id of task: ledOff  */
extern osThreadId t_pump;    

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

void sensors (void const *argument);                             // thread function
osThreadId t_sensors;                     /* assigned task id of task: ledOn   */
osThreadDef (sensors, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread_Sensors (void) {

  t_sensors = osThreadCreate (osThread(sensors), NULL);
  if(!t_sensors) return(-1);
  
  return(0);
}

void sensors (void const *argument) {

  while (1) {
		ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
    osDelay(800);                       /* delay 800ms                       */
		uint16_t result = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A);
		if(result > 0x01FA)
		{
			osSignalSet(t_ledOff, 0x0001);      /* send event to task 'ledoff'       */
		}                                         // suspend thread
		osSignalSet(t_pump, 0x0001);
  }
}


