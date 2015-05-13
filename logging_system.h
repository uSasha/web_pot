#ifndef DAVE_HEADER
#define DAVE_HEADER

#include <DAVE.h>	

#define BUFFER_SIZE				10		// number of entries

struct sensors_data
{
		time_t  timestamp;
		uint8_t temperature;
		uint8_t moisture;
		uint8_t lite;
};

#endif


bool data_Write(struct sensors_data * data, uint32_t size_words);
struct sensors_data data_Read(uint32_t size_words);
