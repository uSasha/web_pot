/*
 * main.c
 *
 *  Created on: 2015 ��� 17 14:02:46
 *  Author: aleksandr.dmitriev
 */




#include <XMC4500.h>
#include <DAVE.h>			//Declarations from DAVE Code Generation (includes SFR declaration)
#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX          */


extern void Init_Thread_Pump(void);
extern void Init_Thread_Sensors(void);

osThreadId t_ledOff;                    /* assigned task id of task: ledOff  */
extern osThreadId 	t_pump;


/*----------------------------------------------------------------------------
  Task 2 'ledOff': switches the LED off
 *---------------------------------------------------------------------------*/
 void ledOff (void const *argument) {
  for (;;) {
		DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_0);
    osDelay(4);                         /* delay 4000ms                     */
    DIGITAL_IO_SetOutputLow(&DIGITAL_IO_0);
		osSignalWait (0x0001, osWaitForever); /* wait for an event flag 0x0001   */
  }
}


osThreadDef(ledOff, osPriorityNormal, 1, 0);


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
  t_ledOff 		= osThreadCreate(osThread(ledOff), 	NULL);  /* start task 'ledOff' */

	osDelay(osWaitForever);
}
