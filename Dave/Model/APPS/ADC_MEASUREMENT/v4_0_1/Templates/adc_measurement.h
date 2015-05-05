/******************************************************************************
 Copyright (c) 2015, Infineon Technologies AG                                **
 All rights reserved.                                                        **
																			 **
 Redistribution and use in source and binary forms, with or without          **
 modification,are permitted provided that the following conditions are met:  **
																			 **
 *Redistributions of source code must retain the above copyright notice,     **
 this list of conditions and the following disclaimer.                       **
 *Redistributions in binary form must reproduce the above copyright notice,  **
 this list of conditions and the following disclaimer in the documentation   **
 and/or other materials provided with the distribution.                      **
 *Neither the name of the copyright holders nor the names of its             **
 contributors may be used to endorse or promote products derived from this   **
 software without specific prior written permission.                         **
																			 **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE  **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR        **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS   **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)      **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  **
 POSSIBILITY OF SUCH DAMAGE.                                                 **
																			 **
 To improve the quality of the software, users are encouraged to share       **
 modifications, enhancements or bug fixes with Infineon Technologies AG      **
 dave@infineon.com).                                                         **
																			 **
*******************************************************************************
**                                                                           **
**                                                                           **
** PLATFORM : Infineon XMC4000/ XMC1000 Series                               **
**                                                                           **
** COMPILER : Compiler Independent                                           **
**                                                                           **
** AUTHOR   : APP Developer                                                  **
**                                                                           **
** MAY BE CHANGED BY USER [yes/no]: Yes                                      **
**                                                                           **
**                                                                           **
******************************************************************************/
/*************** Revision History **********************************************
Version        Date          Description

  4.0.0       16-Feb-2015   Initial version for DAVEv4
  4.0.1       19-Feb-2015   Added GetResult APIs for XMC1100 devices.
*******************************************************************************/

/**
 * @file   adc_measurement.h
 *
 * @brief Header file for the ADC_MEASUREMENT APP
 * It provides enums,function prototypes and inline function definitions.
 */

/*CODE_BLOCK_BEGIN*/
#ifndef _ADC_MEASUREMENT_H_
#define _ADC_MEASUREMENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
**                      INCLUDE FILES                                         **
*******************************************************************************/

#include "GLOBAL_ADC/global_adc.h"
#include "adc_measurement_conf.h"

/*******************************************************************************
**                      GLOBAL MACRO DEFINITION                               **
*******************************************************************************/

/*******************************************************************************
**                      ENUMERATIONS                                          **
*******************************************************************************/
/**
 * @ingroup ADC_MEASUREMENT_enumerations
 * @{
 */
/**@brief Return value of an API  */

typedef enum ADC_MEASUREMENT_STATUS
{
  ADC_MEASUREMENT_STATUS_SUCCESS = 0, /**< App is Initialized */
  ADC_MEASUREMENT_STATUS_FAILURE, /**< App Initialization failed*/
  ADC_MEASUREMENT_STATUS_UNINITIALIZED /**<  App has not been Initialized */
} ADC_MEASUREMENT_STATUS_t;
/**
 * @}
 */

/*******************************************************************************
**                      GLOBAL TYPE DEFINITION                                **
*******************************************************************************/
/**
 * @ingroup ADC_MEASUREMENT_datastructures
 * @{
 */

typedef void (*ADC_MEASUREMENT_MUX_CONFIG_t)(void); /**< Function pointer to the mux configuration*/

/**
 *  Structure to initialize  Request Source Interrupt's NVIC Node
 */
typedef struct ADC_MEASUREMENT_ISRHandle
{
  uint32_t node_id; 		/**< This holds the Node ID of the NVIC.*/

  uint32_t priority; 		/**< This holds the NVIC priority.*/
#if(UC_FAMILY == XMC4)
  uint32_t sub_priority; 	/**< This holds the SubPriority of the NVIC. for Only XMC4x Devices*/
#endif
}ADC_MEASUREMENT_ISR_t;

/**
 * Structure to initialize ADC channels.
 */
typedef struct ADC_MEASUREMENT_channel
{
#if( XMC_VADC_GROUP_AVAILABLE == 1U)
  XMC_VADC_CHANNEL_CONFIG_t *ch_handle; /**< This holds the VADC Channel LLD struct*/

  XMC_VADC_RESULT_CONFIG_t *res_handle; /**< This hold the VADC LLD Result handler*/
#endif

#if( XMC_VADC_GROUP_AVAILABLE == 1U)
  XMC_VADC_GROUP_t *group_handle; 		/**< This holds the group to which the channel belongs*/
#endif

  uint8_t group_index; 					/**< This holds the group index*/

  uint8_t ch_num; 						/**< This Holds the Channel Number*/

} ADC_MEASUREMENT_channel_t;

/**
 * Structure to hold channels handles that are configured
 */
typedef struct ADC_MEASUREMENT_channel_array
{
  const ADC_MEASUREMENT_channel_t *const channel_array[ADC_MEASUREMENT_MAXCHANNELS]; /**< Array which consists
                                                                                        of Apps Channel Handles*/
#if( XMC_VADC_GROUP_AVAILABLE == 0U)
  XMC_VADC_RESULT_CONFIG_t *res_handle; /**< This hold the VADC LLD Result handler*/
#endif
} ADC_MEASUREMENT_channel_array_t;

/**
 * Structure to configure ADC_MEASUREMENT APP.
 */
typedef struct ADC_MEASUREMENT_Handle
{
  const ADC_MEASUREMENT_channel_array_t *const array; 		 /**< This holds ADC_MEASUREMENT_Channel_HandleArray*/

  const XMC_VADC_BACKGROUND_CONFIG_t *const backgnd_config_handle; /**< This holds the LLD Background Scan Init Structure*/

  const XMC_VADC_GLOBAL_CLASS_t *const iclass_config_handle;  /**< This holds the adc global ICLASS 0 configuration*/

  GLOBAL_ADC_t *const global_handle; 						 /**< This hold the ADC Global App handle*/

#ifdef ADC_MEASUREMENT_CPU_1X
  CPU_CTRL_XMC1_t *const cpu_handle; 					 	 /**< This is the pointer to the CPU App handle.*/
#endif
#ifdef ADC_MEASUREMENT_CPU_4X
  CPU_CTRL_XMC4_t *const cpu_handle; 					 	 /**< This is the pointer to the CPU App handle.*/
#endif

#if (UC_SERIES != XMC11)
  const ADC_MEASUREMENT_ISR_t *const req_src_intr_handle; 	 /**< This has the NVIC configuration structure*/
#else
  const ADC_MEASUREMENT_ISR_t *const result_intr_handle; 	 /**< This has the NVIC configuration structure*/
#endif

  ADC_MEASUREMENT_MUX_CONFIG_t mux_config; /**< This hold the pointer to the function that does mux configuration.*/

  ADC_MEASUREMENT_STATUS_t init_state; 	  /**< This structure will give information about the status of the App*/

  const XMC_VADC_SR_t srv_req_node; 	  /**< Service Request Line selected*/

  const bool start_conversion; 			  /**< This indicates whether to start at initialization of the App*/
} ADC_MEASUREMENT_t;

/**
 * @}
 */

/*******************************************************************************
**                     FUNCTION PROTOTYPES                                    **
*******************************************************************************/
/**
 * @ingroup ADC_MEASUREMENT_apidoc
 * @{
 */

/**
 * @brief Get ADC_MEASUREMENT APP version
 * @return DAVE_APP_VERSION_t APP version information (major, minor and patch number)
 *
 * \par<b>Description: </b><br>
 * The function can be used to check application software compatibility with a
 * specific version of the APP.
 *
 * Example Usage:
 *
 * @code
 * #include <DAVE.h>
 *
 * int main(void) {
 *   DAVE_STATUS_t init_status;
 *   DAVE_APP_VERSION_t version;
 *
 *   // Initialize ADC_MEASUREMENT APP:
 *   // ADC_MEASUREMENT_Init() is called from within DAVE_Init().
 *   init_status = DAVE_Init();
 *
 *   version = ADC_MEASUREMENT_GetAppVersion();
 *   if (version.major != 1U) {
 *     // Probably, not the right version.
 *   }
 *
 *   // More code here
 *   while(1) {
 *
 *   }
 *   return (0);
 * }
 * @endcode
 */
DAVE_APP_VERSION_t ADC_MEASUREMENT_GetAppVersion(void);

/**
 * @brief Initializes the ADC Background Scan with the configured
 * parameters. <BR>
 * @param handle_ptr pointer to the Instance variable<BR>
 * @return Initialization succeeded if ADC_MEASUREMENT_STATUS_t::ADC_MEASUREMENT_STATUS_SUCCESS is returned.<BR>
 *
 * \par<b>Description: </b><br>
 * Initializes the adc measurements. Invoking this API would result in configuration of background request source
 * of the VADC. All the required channel configurations are pushed. At the end of ADC_MEASUREMENT_Init() all
 * the channels are ready to start conversion.
 *
 * @code
 * #include <DAVE.h>
 * int main(void)
 * {
 *    DAVE_Init(); //ADC_MEASUREMENT_Init is called within DAVE_Init
 *    return 0;
 * }
 @endcode
 */ 
ADC_MEASUREMENT_STATUS_t ADC_MEASUREMENT_Init(ADC_MEASUREMENT_t *const handle_ptr);

/**
 * @brief Starts the conversion of the required measurements. <BR>
 * @param handle_ptr pointer to the Instance variable<BR>
 * @return None <BR>
 *
 * \par<b>Description: </b><br>
 *  Starts the ADC Background Scan if "start at initialization" has not been selected in the UI editor.
 *  This causes a load event to occur and all the channels in the background sequence will be converted once.
 *
 * @code
  // Ensure that end of measurements interrupt has been enabled
  #include <DAVE.h>

  XMC_VADC_RESULT_SIZE_t result;
  void Adc_Measurement_Handler()
  {
    result = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A);
  }

  int main(void)
  {
     DAVE_Init();
     ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
     while(1);
     return 0;
  }
 @endcode
 */
void ADC_MEASUREMENT_StartConversion(ADC_MEASUREMENT_t *const handle_ptr);

#if(XMC_VADC_GROUP_AVAILABLE == 1U)
/**
 * @brief Returns the converted value for a specific channel.<BR>
 * @param handle_ptr pointer to the Instance variable. (Can pass the macro defined in conf.h)<BR>
 * @return XMC_VADC_RESULT_SIZE_t conversion result.<BR>
 *                                Range: [ 0x0 to 0x3FF] if accumulation of results is switched off.
 *
 * \par<b>Description: </b><br>
 * Result of conversion would be returned by invoking this API. This returns the value that is stored in the result
 * register.
 *
 * \par<b>Note: </b><br>
 * Not Applicable for XMC1100 devices.
 *
 * @code
  // Ensure that end of measurements interrupt has been enabled
  #include <DAVE.h>

  XMC_VADC_RESULT_SIZE_t result;
  void Adc_Measurement_Handler()
  {
    result = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A);
  }

  int main(void)
  {
     DAVE_Init();
     ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
     while(1);
     return 0;
  }
 @endcode
 */
XMC_VADC_RESULT_SIZE_t ADC_MEASUREMENT_GetResult(ADC_MEASUREMENT_channel_t *const handle_ptr);

/**
 * @brief Returns a detailed conversion result.<BR>
 * @param handle_ptr pointer to the Instance variable. (Can pass the macro defined in ADC_MEASUREMENT_conf.h)<BR>
 * @return uint32_t The complete Result register.<BR>
 *
 * \par<b>Description: </b><br>
 * Result of conversion would be returned by invoking this API. This returns the entire result register.
 * This would contain various other information regarding the converted value.
 *
 * \par<b>Note: </b><br>
 * Not Applicable for XMC1100 devices.
 *
 * @code
  #include <DAVE.h>

  typedef struct detailed_result_struct
  {
	uint8_t channel_num;
	uint8_t group_num;
	uint16_t conversion_result;
  }detailed_result_struct_t;

  uint32_t result;
  bool valid_result;
  detailed_result_struct_t detailed_result;

  void Adc_Measurement_Handler()
  {
	uint32_t result;
    valid_result = (bool)false;

    result = ADC_MEASUREMENT_GetDetailedResult(&ADC_MEASUREMENT_Channel_A);
    if((bool)(result >> VADC_G_RES_VF_Pos))
    {
       valid_result = (bool)true;
       detailed_result.channel_num = (result & VADC_G_RES_CHNR_Msk) >> VADC_G_RES_CHNR_Pos;
       detailed_result.group_num = ADC_MEASUREMENT_Channel_A.group_index;
       detailed_result.conversion_result = result & VADC_G_RES_RESULT_Msk;
    }
  }

  int main(void)
  {
     DAVE_Init();
     ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
     while(1);
     return 0;
  }
 @endcode
 */
uint32_t ADC_MEASUREMENT_GetDetailedResult(ADC_MEASUREMENT_channel_t *const handle_ptr);

#else /* Applicable for XMC1100 devices*/
/**
 * @brief Returns the converted value from the global result register.<BR>
 * @param handle_ptr pointer to the handle variable.<BR>
 * @return XMC_VADC_RESULT_SIZE_t conversion result.<BR>
 *                                Range: [ 0x0 to 0x3FF] if accumulation of results is switched off.
 *
 * \par<b>Description: </b><br>
 * Result of conversion would be returned by invoking this API. This returns the value that is stored in the  global
 * result register.
 *
 * \par<b>Note: </b><br>
 * Applicable only for XMC1100 Devices.<BR>\n
 * XMC1100 devices a call to this API will always read from the global result register. In the XMC1100
 * devices all the conversion results would be stored in the global result register.
 * The wait for read mode will be enabled for the global result register and hence the next conversion results
 * will only be present if the previous values were read.
 *
 * @code
  // Ensure that end of measurements interrupt has been enabled
  #include <DAVE.h>

  XMC_VADC_RESULT_SIZE_t result;
  void Adc_Measurement_Handler()
  {
    result = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_0);
  }

  int main(void)
  {
     DAVE_Init();
     ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
     while(1);
     return 0;
  }
 @endcode
 */
XMC_VADC_RESULT_SIZE_t ADC_MEASUREMENT_GetResult(ADC_MEASUREMENT_t *const handle_ptr);

/**
 * @brief Returns a detailed conversion result.<BR>
 * @param handle_ptr pointer to the Instance variable. (Can pass the macro defined in ADC_MEASUREMENT_conf.h)<BR>
 * @return uint32_t The complete Result register.<BR>
 *
 * \par<b>Description: </b><br>
 * Result of conversion would be returned by invoking this API. This returns the entire result register.
 * This would contain various other information regarding the converted value.
 *
 * \par<b>Note: </b><br>
 * Applicable only for XMC1100 Devices.<BR>\n
 * XMC1100 devices a call to this API will always read from the global result register. In the XMC1100
 * devices all the conversion results would be stored in the global result register.
 * The wait for read mode will be enabled for the global result register and hence the next conversion results
 * will only be present if the previous values were read.
 *
 * @code
  #include <DAVE.h>

  typedef struct detailed_result_struct
  {
	uint8_t channel_num;
	uint8_t group_num;
	uint16_t conversion_result;
  }detailed_result_struct_t;

  uint32_t result;
  bool valid_result;
  detailed_result_struct_t detailed_result[10];

  void Adc_Measurement_Handler()
  {
	static uint8_t index;
	uint32_t result;
    valid_result = (bool)false;

    result = ADC_MEASUREMENT_GetDetailedResult(&ADC_MEASUREMENT_0);

    if((bool)(result >> VADC_GLOBRES_VF_Pos))
    {
       valid_result = (bool)true;
       detailed_result[index].channel_num = (result & VADC_GLOBRES_CHNR_Msk) >> VADC_GLOBRES_CHNR_Pos;
       detailed_result[index].group_num = ADC_MEASUREMENT_Channel_A.group_index;
       detailed_result[index].conversion_result = result & VADC_GLOBRES_RESULT_Msk;
    }
    index++;
  }

  int main(void)
  {
     DAVE_Init();
     ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
     while(1);
     return 0;
  }
 @endcode
 */
uint32_t ADC_MEASUREMENT_GetDetailedResult(ADC_MEASUREMENT_t *const handle_ptr);
#endif

/**
 * @}
 */

#include "ADC_MEASUREMENT_Extern.h"
#ifdef __cplusplus
}
#endif

#endif /* ADC_MEASUREMENT_H_ */
