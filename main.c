/*
 * main.c
 *
 *  Created on: 2015 ��� 17 14:02:46
 *  Author: aleksandr.dmitriev
 */




#include <XMC4500.h>
#include <DAVE.h>			//Declarations from DAVE Code Generation (includes SFR declaration)
#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX          */
#include "rtc.h"                        // Infineon::DAVE:RTC


extern void Init_Thread_Pump(void);
extern void Init_Thread_Sensors(void);
extern int Init_Thread_WebServer (void);

bool watering_days[7] = {false, false, false, false, false, false, false};
uint32_t watering_time = 12;						// hour 24 hour/day format

extern osThreadId 	t_webServer;                    /* assigned task id of task: ledOff  */
extern osThreadId 	t_pump;
extern osThreadId t_webServer;


// RTC interrupt handler, fires every day
void IRQ_HourPassed(void)
{
	// check is it time to water flowers
	XMC_RTC_TIME_t current_time;
	RTC_GetTime(&current_time);

	if((current_time.hours == watering_time) && (watering_days[current_time.daysofweek]))
	{
		osSignalSet(t_pump, 0x0001);	// water wlowers
	}
}

/**
 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the App initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */
int main(void)
{
	osKernelInitialize();
	
  DAVE_STATUS_t status;
  status = DAVE_Init();		/* Initialization of DAVE Apps  */

	Init_Thread_Pump();
	Init_Thread_Sensors();
	Init_Thread_WebServer();
	osKernelStart();
	
  if(status == DAVE_STATUS_FAILURE)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler */
    XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
    while(1U)
    {
    }
  }

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
	osDelay(osWaitForever);
}
