#include <DAVE.h>	


struct sensors_data
{
   time_t  timestamp;
   uint8_t temperature;
   uint8_t moisture;
};

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
	return data;
}
