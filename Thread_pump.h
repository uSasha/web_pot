#ifndef OS_HEADER
#define OS_HEADER

#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX          */
#include "DAVE.h"

#endif

int Init_Thread_Pump (void);
void watering_setTime(uint16_t watering_hour);
void watering_daysSet(uint8_t weekday);
void watering_daysReset(uint8_t weekday);
bool watering_isItTime(void);
void watering_setWeekAmount(uint16_t milliliters);

extern osThreadId 	t_pump;                                          // thread id
