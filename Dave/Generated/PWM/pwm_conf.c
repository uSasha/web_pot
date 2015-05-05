/*******************************************************************************
**  DAVE App Name : PWM       App Version: 4.0.0               
**  This file is generated by DAVE, User modification to this file will be    **
**  overwritten at the next code generation.                                  **
*******************************************************************************/


/*CODE_BLOCK_BEGIN[pwm_conf.c]*/
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
** PLATFORM : Infineon XMC4000/XMC1000 Series                                **
**                                                                           **
** COMPILER : Compiler Independent                                           **
**                                                                           **
** AUTHOR   : APP Developer                                                  **
**                                                                           **
** MAY BE CHANGED BY USER [yes/no]: yes                                      **
**                                                                           **
** NOTE     : Any Changes made to this file will be overwritten during Code  **
**            Generation                                                     **
**                                                                           **
******************************************************************************/
/*************** Revision History **********************************************
Version        Date          Description 

  4.0.0       16-Feb-2015   Initial version for DAVEv4
*******************************************************************************/

/**
 * @file pwm_conf.c
 * @brief This file contains the configuration parameters for all instances of
 * the PWM App
 */
/******************************************************************************
 ** INCLUDE FILES                                                            **
 *****************************************************************************/
#include "pwm.h"


const XMC_CCU4_SLICE_COMPARE_CONFIG_t PWM_0_Config_Handle =
{
  .timer_mode 		   = (uint32_t) XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
  .monoshot   		   = (uint32_t) false,
  .shadow_xfer_clear   = (uint32_t) 0,
  .dither_timer_period = (uint32_t) 0,	
  .dither_duty_cycle   = (uint32_t) 0,
  .prescaler_mode	   = (uint32_t) XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
  .mcm_enable		   = (uint32_t) 0,
  .prescaler_initval   = (uint32_t) 1,
  .float_limit		   = (uint32_t) 0,
  .dither_limit		   = (uint32_t) 0,
  .passive_level 	   = (uint32_t) XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
  .timer_concatenation = (uint32_t) 0
};

const XMC_GPIO_CONFIG_t  PWM_0_gpio_out_config	= 
{
  .mode                = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT3,
  .output_level        = XMC_GPIO_OUTPUT_LEVEL_LOW,
  .output_strength     = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
};

PWM_Dynamic_t PWM_0_config_value = 
{
  .compare_value = 20000U,
  .period_value  = 39999U,
  .duty_cycle    = 5000U
};
        
const PWM_ConfigType_t  PWM_0_ConfigHandle =
{
  .start_control                 = false,      
  .config_value                   = (PWM_Dynamic_t *)&PWM_0_config_value,
  .period_match_enable           = false,
  .compare_match_enable          = false,				
  .ccu4_kernel_ptr               = (XMC_CCU4_MODULE_t*) CCU40_BASE, 
  .ccu4_slice_ptr                = (CCU4_CC4_TypeDef*)  CCU40_CC42,
  .ccu4_kernel_number		     = 0U,
  .ccu4_slice_number             = 2U,
  .ccu4_slice_config_ptr         = &PWM_0_Config_Handle,
  .ccu4_slice_period_match_node  = XMC_CCU4_SLICE_SR_ID_0,
  .ccu4_slice_compare_match_node = XMC_CCU4_SLICE_SR_ID_0,
  .gpio_out_port                 = XMC_GPIO_PORT1,
  .gpio_out_pin                  = (uint8_t) 1,
  .gpio_out_config               = &PWM_0_gpio_out_config	
};

PWM_t PWM_0 =
{
  .timer_type             = PWM_TIMER_SLICE_CCU4,
  .state                  = PWM_STATUS_UNINITIALIZED,			
  .GLOBAL_CCU4_handle     = (GLOBAL_CCU4_t*)&GLOBAL_CCU4_0,
  .config_ptr             = &PWM_0_ConfigHandle
};

/********************************************************************************************************/

