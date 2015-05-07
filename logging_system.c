#include <DAVE.h>	
#include "logging_system.h"

#define BUFFER_SIZE				10		// number of entries


bool logging_writeToStorage(struct sensors_data * data_buffer);


void data_Write(struct sensors_data * input_data)
{
	// TODO implement data reading from Flash/SD
  static struct sensors_data  data_buffer[BUFFER_SIZE];
  static uint16_t i = 0;
  data_buffer[i] = * input_data;
  time_t time_sec;
  RTC_Time(&time_sec); // get UNIX time
  data_buffer[i].timestamp = time_sec;
  i++;
  if(i >= BUFFER_SIZE)
  {
    i = 0;
    logging_writeToStorage(data_buffer);
  }
}


struct sensors_data data_Read(uint32_t size_words)
{
  // TODO implement data reading from Flash/SD
  struct sensors_data data;
  data.timestamp = 0;
  data.moisture = 0;
  data.lite = 0;
  data.temperature = 0;
  return data;
}


bool logging_writeToStorage(struct sensors_data * data_buffer)
{
  // TODO write buffer to Flash
  return true;
}
