#ifndef OS_HEADER
#define OS_HEADER

#include "cmsis_os.h"                   /* ARM::CMSIS:RTOS:Keil RTX          */
#include "DAVE.h"

#endif

int Init_Thread_Pump (void);
void watering_setTime(uint16_t watering_hour);
int16_t watering_getTime(void);
void watering_daysSet(uint8_t weekday);
void watering_daysReset(uint8_t weekday);
uint16_t watering_daysGet(uint8_t weekday);
bool watering_isItTime(void);
void watering_setWeekAmount(uint16_t milliliters);
uint16_t watering_getWeekAmount(void);
void watering_daysResetAll(void);


extern osThreadId 	t_pump;                                          // thread id
