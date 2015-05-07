#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "adc_measurement.h"            // Infineon::DAVE:ADC_MEASUREMENT
#include <DAVE.h>  
#include "logging_system.h"

#define TICKS_IN_MINUTE		60000000	// TODO check values
#define ADC_FACTOR_TEMP		100		// TODO check values
#define ADC_FACTOR_MOISTURE 200 // TODO check values


void sensors (void const *argument);    // thread function
void sensors_measureData(struct sensors_data * data_struct);

extern osThreadId t_ledOff;                    /* assigned task id of task: ledOff  */
extern osThreadId t_pump;    
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
		struct sensors_data  current_measurement;
		sensors_measureData(&current_measurement);		
		data_Write(&current_measurement);
		DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
  }
}


// set sensor check interval input data is in minutes, saved data is in OS ticks
void sensors_setCheckInterval(uint32_t interval_input)
{
	check_interval = interval_input * TICKS_IN_MINUTE;
}


void sensors_measureData(struct sensors_data * data_struct)
{
	ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
	osDelay(check_interval);   

	uint16_t result = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A);
	result *= ADC_FACTOR_TEMP;
	data_struct->temperature = result;

	result = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_B);
	result *= ADC_FACTOR_MOISTURE;
	data_struct->moisture = result;	

	result = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_C);
	result *= ADC_FACTOR_MOISTURE;
	data_struct->lite = result;		
}
