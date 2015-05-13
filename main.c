/*
 * main.c
 *
 *  Created on: 2015 ��� 17 14:02:46
 *  Author: aleksandr.dmitriev
 */




#include <XMC4500.h>
#include <DAVE.h>												//Declarations from DAVE Code Generation (includes SFR declaration)
#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX          */
#include "rtc.h"                        // Infineon::DAVE:RTC
#include "Thread_sensors.h"
#include "Thread_pump.h"
#include "Thread_webserver.h"
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE


void dhcp_client_notify(uint32_t if_num, dhcpClientOption opt, const uint8_t *val, uint32_t len) {
if (opt == dhcpClientIPaddress) {
	DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_0);
 }
}

// RTC interrupt handler, fires every day
void IRQ_HourPassed(void)
{
	// check is it time to water flowers
	if(watering_isItTime())
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
	DAVE_Init();		/* Initialization of DAVE Apps  */
	Init_Thread_Pump();
	Init_Thread_Sensors();
	Init_Thread_WebServer();
	net_initialize();
	osKernelStart();
	
  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
	osDelay(osWaitForever);
}
