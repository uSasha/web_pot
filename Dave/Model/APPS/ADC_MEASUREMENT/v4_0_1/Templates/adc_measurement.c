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
 * @file   adc_measurement.c
 *
 * @brief Source file for ADC_MEASUREMENT APP
 */

/*CODE_BLOCK_BEGIN*/
/*******************************************************************************
 ** INCLUDE FILES                                                             **
 ******************************************************************************/
#include "adc_measurement.h"

/*******************************************************************************
**PUBLIC FUNCTION DEFINITION                                                  **
*******************************************************************************/

/*This function returns the version of the ADC_MEASUREMENT App*/
DAVE_APP_VERSION_t ADC_MEASUREMENT_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = (uint8_t) ADC_MEASUREMENT_MAJOR_VERSION;
  version.minor = (uint8_t) ADC_MEASUREMENT_MINOR_VERSION;
  version.patch = (uint8_t) ADC_MEASUREMENT_PATCH_VERSION;

  return version;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Initialization routine to call ADC LLD API's */
ADC_MEASUREMENT_STATUS_t ADC_MEASUREMENT_Init(ADC_MEASUREMENT_t *const handle_ptr)
{
  XMC_ASSERT("ADC_MEASUREMENT_Init:Invalid handle_ptr", (handle_ptr != NULL))

  const ADC_MEASUREMENT_channel_t *indexed;
  uint8_t j;
  ADC_MEASUREMENT_STATUS_t status;

  if (ADC_MEASUREMENT_STATUS_UNINITIALIZED == handle_ptr->init_state)
  {
    /* Call the function to initialise Clock and ADC global functional units*/
    status = (ADC_MEASUREMENT_STATUS_t) GLOBAL_ADC_Init(handle_ptr->global_handle);

#ifdef ADC_MEASUREMENT_CPU_1X
    status = (ADC_MEASUREMENT_STATUS_t)((uint32_t)status + (uint32_t)CPU_CTRL_XMC1_Init(handle_ptr->cpu_handle));
#endif
#ifdef ADC_MEASUREMENT_CPU_4X
    status = (ADC_MEASUREMENT_STATUS_t)((uint32_t)status + (uint32_t)CPU_CTRL_XMC4_Init(handle_ptr->cpu_handle));
#endif
  
    /*Initialize the Global Conversion class 0*/
    XMC_VADC_GLOBAL_InputClassInit(handle_ptr->global_handle->module_ptr,*handle_ptr->iclass_config_handle,
    		                              XMC_VADC_GROUP_CONV_STD,0U);
#if (UC_SERIES == XMC11)
    /*Initialize the Global Conversion class 1*/
    XMC_VADC_GLOBAL_InputClassInit(handle_ptr->global_handle->module_ptr,*handle_ptr->iclass_config_handle,
    		                              XMC_VADC_GROUP_CONV_STD,1U);
#endif
  
    /* Initialize the Background Scan hardware */
    XMC_VADC_GLOBAL_BackgroundInit(handle_ptr->global_handle->module_ptr, handle_ptr->backgnd_config_handle);
  
#if (XMC_VADC_GROUP_AVAILABLE == 0U)
    /* Initialize the global result register */
    XMC_VADC_GLOBAL_ResultInit(handle_ptr->global_handle->module_ptr,handle_ptr->array->res_handle);
#endif

    for(j = (uint8_t)0; j < ADC_MEASUREMENT_MAXCHANNELS; j++)
    {
      indexed = handle_ptr->array->channel_array[j];
#if (XMC_VADC_GROUP_AVAILABLE == 1U)
      /* Initialize for configured channels*/
      XMC_VADC_GROUP_ChannelInit(indexed->group_handle,(uint32_t)indexed->ch_num, indexed->ch_handle);
  
      /* Initialize for configured result registers */
      XMC_VADC_GROUP_ResultInit(indexed->group_handle,(uint32_t)indexed->ch_handle->result_reg_number,indexed->res_handle);
#endif
      /* Add all channels into the Background Request Source Channel Select Register */
      XMC_VADC_GLOBAL_BackgroundAddChannelToSequence(handle_ptr->global_handle->module_ptr, (uint32_t)indexed->group_index,
    		                           (uint32_t)indexed->ch_num);
    }
#if(UC_SERIES != XMC11)
    if ((handle_ptr->backgnd_config_handle->req_src_interrupt) && (handle_ptr->req_src_intr_handle != NULL ))
    {
#if (UC_FAMILY == XMC1)
      NVIC_SetPriority((IRQn_Type)handle_ptr->req_src_intr_handle->node_id,
                        handle_ptr->req_src_intr_handle->priority);
#else
      NVIC_SetPriority((IRQn_Type)handle_ptr->req_src_intr_handle->node_id,
                        NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                        handle_ptr->req_src_intr_handle->priority, handle_ptr->req_src_intr_handle->sub_priority));
#endif
      /* Connect background Request Source Event to NVIC node */
      XMC_VADC_GLOBAL_BackgroundSetReqSrcEventInterruptNode(handle_ptr->global_handle->module_ptr,
    		                                         (XMC_VADC_SR_t) handle_ptr->srv_req_node);
    
      /* Enable Background Scan Request source IRQ */
      NVIC_EnableIRQ((IRQn_Type)handle_ptr->req_src_intr_handle->node_id);
    }
#else /* Selected device is XMC11*/
#ifdef ADC_MEASUREMENT_CPU_1X /* End of single measurement is enabled*/
    NVIC_SetPriority((IRQn_Type)handle_ptr->result_intr_handle->node_id,
                            handle_ptr->result_intr_handle->priority);

    XMC_VADC_GLOBAL_SetResultEventInterruptNode(handle_ptr->global_handle->module_ptr, handle_ptr->srv_req_node );

    /* Enable Background Scan Request source IRQ */
    NVIC_EnableIRQ((IRQn_Type)handle_ptr->result_intr_handle->node_id);
#endif
#endif
    /* Mux Configuration is done*/
    if (handle_ptr->mux_config != NULL)
    {
    	(handle_ptr->mux_config)();
    }
  
    if (handle_ptr->start_conversion)
    {
      /* Start conversion manually using load event trigger*/
      XMC_VADC_GLOBAL_BackgroundTriggerConversion(handle_ptr->global_handle->module_ptr);
    }
    handle_ptr->init_state = status;
  }
  return (handle_ptr->init_state);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* This API will Software trigger ADC Background request source and starts conversion*/
void ADC_MEASUREMENT_StartConversion(ADC_MEASUREMENT_t *const handle_ptr)
{
  XMC_ASSERT("ADC_MEASUREMENT_Start:Invalid handle_ptr", (handle_ptr != NULL))

  /* Generate a load event to start background request source conversion*/
  XMC_VADC_GLOBAL_BackgroundTriggerConversion(handle_ptr->global_handle->module_ptr);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#if(XMC_VADC_GROUP_AVAILABLE == 1U)
/* This API will get the result of a conversion for a specific channel*/
XMC_VADC_RESULT_SIZE_t ADC_MEASUREMENT_GetResult(ADC_MEASUREMENT_channel_t *const handle_ptr)
{
  XMC_VADC_RESULT_SIZE_t result;

  XMC_ASSERT("ADC_MEASUREMENT_GetResult:Invalid handle_ptr", (handle_ptr != NULL))

  result = XMC_VADC_GROUP_GetResult(handle_ptr->group_handle,handle_ptr->ch_handle->result_reg_number);

  return (result);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* This API will get the result of a conversion for a specific channel. It will return the complete result register*/
uint32_t ADC_MEASUREMENT_GetDetailedResult(ADC_MEASUREMENT_channel_t *const handle_ptr)
{
	uint32_t result;

  XMC_ASSERT("ADC_MEASUREMENT_GetResult:Invalid handle_ptr", (handle_ptr != NULL))

  result = XMC_VADC_GROUP_GetDetailedResult(handle_ptr->group_handle,handle_ptr->ch_handle->result_reg_number);

  return (result);
}

#else /* Applicable for XMC1100 devices */
/* This API will get the result of the conversion from the global result*/
XMC_VADC_RESULT_SIZE_t ADC_MEASUREMENT_GetResult(ADC_MEASUREMENT_t *const handle_ptr)
{
  XMC_VADC_RESULT_SIZE_t result;

  XMC_ASSERT("ADC_MEASUREMENT_GetResult:Invalid handle_ptr", (handle_ptr != NULL))

  result = XMC_VADC_GLOBAL_GetResult(handle_ptr->global_handle->module_ptr);
  return (result);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* This API will get the result of a conversion for a specific channel. It will return the global result register*/
uint32_t ADC_MEASUREMENT_GetDetailedResult(ADC_MEASUREMENT_t *const handle_ptr)
{
	uint32_t result;

  XMC_ASSERT("ADC_MEASUREMENT_GetResult:Invalid handle_ptr", (handle_ptr != NULL))

  /* Needed only for XMC1100 devices to read global result register*/
  result = XMC_VADC_GLOBAL_GetDetailedResult(handle_ptr->global_handle->module_ptr);
  return (result);
}
#endif
