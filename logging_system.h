#ifndef DAVE_HEADER
#define DAVE_HEADER

#include <DAVE.h>	

#endif

struct sensors_data
{
   time_t  timestamp;
   uint8_t temperature;
   uint8_t moisture;
};

bool data_Write(struct sensors_data * data, uint32_t size_words);

uint16_t * data_Read(uint32_t size_words);
