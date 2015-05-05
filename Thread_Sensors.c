
#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "adc_measurement.h"            // Infineon::DAVE:ADC_MEASUREMENT
#include <DAVE.h>  

#define CHECK_INTERVAL		1000		// microseconds, should be less than ADC conversion time
#define BUFFER_SIZE				10		// number of entries

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
		static uint16_t data_buffer[BUFFER_SIZE];
		static uint16_t i = 0;
		
		ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
		osDelay(CHECK_INTERVAL);                     
		// convert to temperature
		// convert to moisture
		data_buffer[i++] = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A);
		if(i >= BUFFER_SIZE)
		{
			// send buffer to logging system
			i = 0;
			DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
		}
  }
}


