/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_HTTP_Server_CGI]
#include <stdio.h>
#include <string.h>
#include "rl_net.h"
#include <DAVE.h>												//Declarations from DAVE Code Generation (includes SFR declaration)
#include "logging_system.h"
#include "rtc.h"                        // Infineon::DAVE:RTC
#include "Thread_pump.h"
#include <stdlib.h>     /* atoi */

#include "rtc.h"

#define MIN_WEEK_AMOUNT		10
#define MAX_WEEK_AMOUNT		100


extern struct sensors_data data_buffer[BUFFER_SIZE]; 


// Process query string received by GET request.
void cgi_process_query (const char *qstr) {
  char var[40];

  do {
    // Loop through all the parameters
    qstr = http_get_env_var (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter
    if (var[0] != 0) {
      // First character is non-null, string exists

      /* Example of a parameter "ip=192.168.0.100"
      if (strncmp (var, "ip=", 3) == 0) {
        uint8_t ip_addr[IP4_ADDR_LEN];
        // Read parameter IP address submitted by the browser
        ip4_aton (&var[3], ip_addr);
         ...
      }
      */
    }
  } while (qstr);
}
 
// Process data received by POST request.
// Type code: - 0 = www-url-encoded form data.
//            - 1 = filename for file upload (null-terminated string).
//            - 2 = file upload raw data.
//            - 3 = end of file upload (file close requested).
//            - 4 = any XML encoded POST data (single or last stream).
//            - 5 = the same as 4, but with more XML data to follow.
void cgi_process_data (uint8_t code, const char *data, uint32_t len) {
	
  char var[40];
 
  switch (code) {
    case 0:
      // Url encoded form data received
      do {
        // Parse all parameters
        data = http_get_env_var (data, var, sizeof (var));
        if (var[0] != 0) {
          // First character is non-null, string exists
					XMC_RTC_TIME_t time;
					RTC_GetTime(&time);
					
          if (var[0] == 'h'){
						char temp_str[2];
						strncpy(temp_str, var+2, 2);
						uint8_t hour = atoi(temp_str);
						if((0 <= hour) && (hour <= 23))
						{
							time.hours = hour;
						}
          }else
					if (var[0] == 'm'){
						char temp_str[2];
						strncpy(temp_str, var+2, 2);
						uint8_t minute = atoi(temp_str);
						if((0 <= minute) && (minute <= 59))
						time.minutes = atoi(temp_str);
						
          }else
					if (var[0] == 'd'){
						char temp_str[1];
						strncpy(temp_str, var+2, 2);
						uint8_t day = atoi(temp_str);
						if((0 <= day) && (day <= 6))
						{						
							time.daysofweek = day;
						}
          }					
					RTC_SetTime(&time);
					
					if(var[0] == 'w')
					{
						if(var[2] == 'd')
						{
							char temp_str[1];
							strncpy(temp_str, var+4, 2);
							uint8_t day = atoi(temp_str);
							if((0 <= day) && (day <= 6))
							{
								// all days were cleared before in "amount" case
								// we only need to set active ones
								watering_daysSet(day);
							}
						}
						if(var[2] == 'a')
						{
							char temp_str[3];
							strncpy(temp_str, var+4, 3);
							uint16_t amount = atoi(temp_str);
							if((MIN_WEEK_AMOUNT <= amount) && (amount <= MAX_WEEK_AMOUNT))
							{
								watering_setWeekAmount(amount);
							}
							
							// we do it here because this case run before "days"
							// and we really want to clear watering days table only once
							watering_daysResetAll();		
						}
						if(var[2] == 'h')
						{
							char temp_str[2];
							strncpy(temp_str, var+4, 2);
							uint8_t hour = atoi(temp_str);
							if((0 <= hour) && (hour <= 23))
							{
								watering_setTime(atoi(temp_str));								
							}
						}
					}
        }
      } while (data);
      break;

    case 1:
      // Filename for file upload received
      /* Example
      if (data[0] != 0) {
        // Open a file for writing
        file = fopen (var, "w");
      }
      */
      break;

    case 2:
      // File content data received
      /* Example
      if (file != NULL) {
        // Write data to a file
        fwrite (data, 1, len, file);
      }
      */
      break;

    case 3:
      // File upload finished
      /* Example
      if (file != NULL) {
        // Close a file
        fclose (file);
      }
      */
      break;

    case 4:
      // XML encoded content type, last packet
      /* Example
      if (strcmp (http_get_content_type(), "text/xml; charset=utf-8" == 0) {
        // Content type xml, utf-8 encoding
         ...
      }
      */
      break;

    case 5:
      // XML encoded content type, more to follow
      // ... Process xml data
      break;

    default:
      // Ignore all other codes
      break;
  }
}
 
// Generate dynamic web data from a script line.
uint32_t cgi_script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  uint32_t len = 0;
	
	DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
	XMC_RTC_TIME_t time;
	RTC_GetTime(&time);
	uint16_t watering_hour = watering_getTime();
	uint16_t watering_amount = watering_getWeekAmount();
	
	switch (env[0]) { 
		case 'L' : 
			sprintf(buf, "{\"x\":\"%d\", \"y\":\"%d\", \"z\":\"%d\", \"watering_hour\":\"%d\", \"current_hour\":\"%d\", \"current_minutes\":\"%.02d\", \"watering_amount\":\"%d\", \"current_wday\":\"%d\", \"watering_mon\":\"%d\", \"watering_tue\":\"%d\", \"watering_wed\":\"%d\", \"watering_thu\":\"%d\", \"watering_fri\":\"%d\", \"watering_sat\":\"%d\", \"watering_sun\":\"%d\" }" \
		,data_buffer[0].moisture, data_buffer[0].temperature, data_buffer[0].lite, watering_hour, time.hours, time.minutes, watering_amount, time.daysofweek, watering_daysGet(0), watering_daysGet(1), watering_daysGet(2), watering_daysGet(3), watering_daysGet(4), watering_daysGet(5), watering_daysGet(6)); 

		len = strlen(buf); 
		break; 
	} 

  return (len);
}
 
// Override default Content-Type for CGX script files.
const char *cgx_content_type (void) {
  return ("application/json");
}
 
// Override default character encoding in html documents.
const char *http_encoding (void) {
  /* Example
  return ("utf-8");
  */
  return (NULL);
}
//! [code_HTTP_Server_CGI]
