#include <DAVE.h>	
#include "logging_system.h"

bool data_Write(struct sensors_data * data, uint32_t size_words)
{
	// TODO implement data reading from Flash/SD
	return true;
}


struct sensors_data data_Read(uint32_t size_words)
{
	// TODO implement data reading from Flash/SD
	struct sensors_data data;
	data.timestamp = 0;
	data.moisture = 0;
	data.temperature = 0;
	data.lite = 0;
	return data;
}
