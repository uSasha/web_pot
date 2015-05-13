#include <DAVE.h>			//Declarations from DAVE Code Generation (includes SFR declaration)
#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void webServer (void const *argument);                             // thread function
osThreadId t_webServer;
osThreadDef (webServer, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread_WebServer (void) {
  t_webServer = osThreadCreate(osThread(webServer), 	NULL);  /* start task 'ledOff' */
	
  if(!t_webServer) return(-1);
  
  return(0);
}


/*----------------------------------------------------------------------------
  Task 2 'ledOff': switches the LED offs
 *---------------------------------------------------------------------------*/
void webServer (void const *argument) {
  for (;;) {
		net_main();
		osThreadYield();
		
  }
}
