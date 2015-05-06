
#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "adc_measurement.h"            // Infineon::DAVE:ADC_MEASUREMENT
#include <DAVE.h>  
#include "logging_system.h"

#define BUFFER_SIZE				10		// number of entries
#define TICKS_IN_MINUTE		60000000	// TODO check values

extern osThreadId t_ledOff;                    /* assigned task id of task: ledOff  */
extern osThreadId t_pump;    

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

void sensors (void const *argument);                             // thread function
osThreadId t_sensors;                     /* assigned task id of task: ledOn   */
osThreadDef (sensors, osPriorityNormal, 1, 0);                   // thread object
static uint32_t check_interval = 1000;


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
		osDelay(check_interval);                     
		// TODO convert to temperature
		// TODO convert to moisture
		data_buffer[i++] = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A);
		if(i >= BUFFER_SIZE)
		{
			if(!data_Write(data_buffer, BUFFER_SIZE))
			{
				// TODO handle data write error
			}
			i = 0;
			DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
		}
  }
}


// set sensor check interval input data is in minutes, saved data is in OS ticks
void sensors_setCheckInterval(uint32_t interval_input)
{
	check_interval = interval_input * TICKS_IN_MINUTE;
}
