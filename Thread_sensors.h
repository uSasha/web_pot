#ifndef OS_HEADER
#define OS_HEADER

#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX          */

#endif

int Init_Thread_Sensors (void);
void sensors (void const *argument);                             // thread function

extern osThreadId t_sensors;

