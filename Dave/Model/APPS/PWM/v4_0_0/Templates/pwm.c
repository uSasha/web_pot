/*******************************************************************************
 Copyright (c) 2015, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
 **
 Redistribution and use in source and binary forms, with or without           **
 modification,are permitted provided that the following conditions are met:   **
 **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
 **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
 **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
 **
 ********************************************************************************
 **                                                                            **
 **                                                                            **
 ** PLATFORM : Infineon XMC4000/XMC1000 Series                                 **
 **                                                                            **
 ** COMPILER : Compiler Independent                                            **
 **                                                                            **
 ** AUTHOR   : APP Developer                                                   **
 **                                                                            **
 ** MAY BE CHANGED BY USER [yes/no]: Yes                                       **
 **                                                                            **
 **                                                                            **
 *******************************************************************************/
/*************** Revision History **********************************************
Version        Date          Description

  4.0.0       16-Feb-2015   Initial version for DAVEv4
*******************************************************************************/

/**
 * @file  pwm.c
 *
 * @brief This file contains implementations of all Public and Private functions
 *        for generating single PWM.
 */

/*CODE_BLOCK_BEGIN[pwm.c]*/
/********************************************************************************
 **                      Include Files                                         **
 *******************************************************************************/
#include "pwm.h"

/*******************************************************************************
**  private macro Definition                                                    **
*******************************************************************************/
#define PWM_MAX_DUTY_CYCLE ((uint32_t)10000)
#define PWM_DUTY_CYCLE_SCALE ((uint32_t)100)

/********************************************************************************
 **                 CCU4 timer private function declarations                   **
 *******************************************************************************/
#ifdef PWM_SLICE_USED_CCU4
/*Initialize the App and XMC_CCU4_CC4 slice. */
PWM_STATUS_t PWM_lCCU4_Init(PWM_t *const handle_ptr);

/*Initialize interrupts*/
void PWM_lCCU4_ConfigInterrupts(PWM_t *const handle_ptr);

/*Starts the CCU4_CC4 slice. */
void PWM_lCCU4_Start(PWM_t *const handle_ptr);

/*Stops the CCU4_CC4 slice. */
void PWM_lCCU4_Stop(PWM_t *const handle_ptr);

/*Sets the duty cycle (uint32_t) for CCU4_CC4 slice. */
PWM_STATUS_t PWM_lCCU4_SetDutyCycle(PWM_t *const handle_ptr, uint32_t duty_cycle);

/*Sets the frequency for CCU4_CC4 slice. */
PWM_STATUS_t PWM_lCCU4_SetFreq(PWM_t *const handle_ptr, uint32_t pwm_freq_hz);

/*Sets the frequency and duty cycle for CCU4_CC4 slice. */
PWM_STATUS_t PWM_lCCU4_SetFreqAndDutyCycle(PWM_t *const handle_ptr, uint32_t pwm_freq_hz,uint32_t duty_cycle);

/*Gets the interrupt status of  CCU4_CC4 slice. */
bool PWM_lCCU4_GetInterruptStatus(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt);

/*Acknowledges the interrupt of  CCU4_CC4 slice. */
void PWM_lCCU4_AcknowledgeInterrupt(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt);

/*Sets the passive level of  CCU4_CC4 slice. */
void PWM_lCCU4_SetPassiveLevel(PWM_t *const handle_ptr, PWM_OutputPassiveLevelType_t pwm_output_passive_level);
#endif

/********************************************************************************
 **                 CCU8 timer private function declarations                   **
 *******************************************************************************/
#ifdef PWM_SLICE_USED_CCU8
/*Initialize the App and XMC_CCU8_CC8 slice. */
PWM_STATUS_t PWM_lCCU8_Init(PWM_t *const handle_ptr);

/*Initialize interrupts*/
void PWM_lCCU8_ConfigInterrupts(PWM_t *const handle_ptr);

/*Starts the CCU8_CC8 slice. */
void PWM_lCCU8_Start(PWM_t *const handle_ptr);

/*Stops the CCU8_CC8 slice. */
void PWM_lCCU8_Stop(PWM_t *const handle_ptr);

/*Sets the duty cycle (uint32_t) for CCU8_CC8 slice. */
PWM_STATUS_t PWM_lCCU8_SetDutyCycle(PWM_t *const handle_ptr, uint32_t duty_cycle);

/*Sets the frequency for CCU8_CC8 slice. */
PWM_STATUS_t PWM_lCCU8_SetFreq(PWM_t *const handle_ptr, uint32_t pwm_freq_hz);

/*Sets the frequency and duty cycle for CCU8_CC8 slice. */
PWM_STATUS_t PWM_lCCU8_SetFreqAndDutyCycle(PWM_t *const handle_ptr, uint32_t pwm_freq_hz, uint32_t duty_cycle);

/*Gets the interrupt status of  CCU8_CC8 slice. */
bool PWM_lCCU8_GetInterruptStatus(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt);

/*Acknowledges the interrupt of  CCU8_CC8 slice. */
void PWM_lCCU8_AcknowledgeInterrupt(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt);

/*Sets the passive level of  CCU8_CC8 slice. */
void PWM_lCCU8_SetPassiveLevel(PWM_t *const handle_ptr, PWM_OutputPassiveLevelType_t pwm_output_passive_level);
#endif

/********************************************************************************
 **                 CCU4 timer function definitions                            **
 *******************************************************************************/
#ifdef PWM_SLICE_USED_CCU4

/*Initialize the App and XMC_CCU4_CC4 slice. */
PWM_STATUS_t PWM_lCCU4_Init(PWM_t *const handle_ptr)
{
  PWM_STATUS_t status = PWM_STATUS_FAILURE;

  XMC_ASSERT("PWM_lCCU4_Init:Invalid handle_ptr" , (handle_ptr != NULL))

  if (PWM_STATUS_UNINITIALIZED == handle_ptr->state)
  {
    /* Initialize consumed Apps */
    status = (PWM_STATUS_t)GLOBAL_CCU4_Init(handle_ptr->GLOBAL_CCU4_handle);

    /*Initialize XMC_CCU4_CC4 slice */
    if (PWM_STATUS_SUCCESS == status)/*check GLOBAL_CCU4_Init status*/
    {
      XMC_DEBUG("PWM_lCCU4_Init:Initilizing slice A")
      XMC_CCU4_SLICE_CompareInit(handle_ptr->config_ptr->ccu4_slice_ptr, handle_ptr->config_ptr->ccu4_slice_config_ptr);

      /* Set the period and compare register values */
      XMC_CCU4_SLICE_SetTimerPeriodMatch(handle_ptr->config_ptr->ccu4_slice_ptr,
    		                             (uint16_t)handle_ptr->config_ptr->config_value->period_value);

      XMC_CCU4_SLICE_SetTimerCompareMatch(handle_ptr->config_ptr->ccu4_slice_ptr,
    		                              (uint16_t)handle_ptr->config_ptr->config_value->compare_value);

      /*Shifting the SLICE_0 mask according to the slice number*/
      XMC_CCU4_EnableShadowTransfer(handle_ptr->config_ptr->ccu4_kernel_ptr,
                              (uint32_t)(((uint32_t)XMC_CCU4_SHADOW_TRANSFER_SLICE_0 |
                            		      (uint32_t)XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0) <<
	                    	  (uint32_t)((uint32_t)handle_ptr->config_ptr->ccu4_slice_number << (uint32_t)2)));

      /* Initialize interrupts */
      PWM_lCCU4_ConfigInterrupts(handle_ptr);

      XMC_GPIO_Init(handle_ptr->config_ptr->gpio_out_port,handle_ptr->config_ptr->gpio_out_pin,
    		        handle_ptr->config_ptr->gpio_out_config);

      handle_ptr->state = PWM_STATUS_SUCCESS;

      /* Start the PWM generation if start at initialization is enabled */
      if ((bool) true == handle_ptr->config_ptr->start_control)
      {
        PWM_Start(handle_ptr);
      }
      status = PWM_STATUS_SUCCESS;
    }
    else
    {
      handle_ptr->state = PWM_STATUS_UNINITIALIZED;
    }

  }
  return (status);
} /* end of PWM_lCCU4_Init() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Initialize interrupts */
void PWM_lCCU4_ConfigInterrupts(PWM_t *const handle_ptr)
{
  XMC_ASSERT("PWM_lCCU4_ConfigInterrupts:Invalid handle_ptr" , (handle_ptr != NULL))

  if ((bool) true == handle_ptr->config_ptr->period_match_enable)
  {
    XMC_DEBUG("PWM_lCCU4_Init:period match enable slice A")
    XMC_CCU4_SLICE_EnableEvent(handle_ptr->config_ptr->ccu4_slice_ptr, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

    /* Bind event to Service Request Node If timer concatenation is not enabled, configure Slice A*/
    XMC_CCU4_SLICE_SetInterruptNode(handle_ptr->config_ptr->ccu4_slice_ptr, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH,
								  handle_ptr->config_ptr->ccu4_slice_period_match_node);
  }

  if ((bool) true == handle_ptr->config_ptr->compare_match_enable)
  {
    XMC_DEBUG("PWM_lCCU4_Init:compare match enable slice A")
    XMC_CCU4_SLICE_EnableEvent(handle_ptr->config_ptr->ccu4_slice_ptr, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP);

    /* Bind event to Service Request Node If timer concatenation is not enabled, configure Slice A*/
    XMC_CCU4_SLICE_SetInterruptNode(handle_ptr->config_ptr->ccu4_slice_ptr, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP,
								  handle_ptr->config_ptr->ccu4_slice_compare_match_node);
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Starts the CCU4_CC4 slice. */
void PWM_lCCU4_Start(PWM_t *const handle_ptr)
{
  XMC_ASSERT("PWM_lCCU4_Start:Invalid handle_ptr" , (handle_ptr != NULL))

  if ((PWM_STATUS_SUCCESS == handle_ptr->state) || (PWM_STATUS_STOPPED == handle_ptr->state))
  {
    /* clear IDLE mode for the slice */
    /* The MSB timer is started first since it does not
     * increment until lower period match happens.*/

    XMC_CCU4_EnableClock(handle_ptr->config_ptr->ccu4_kernel_ptr,handle_ptr->config_ptr->ccu4_slice_number);
    XMC_CCU4_SLICE_StartTimer(handle_ptr->config_ptr->ccu4_slice_ptr);

    handle_ptr->state = PWM_STATUS_RUNNING;
    XMC_DEBUG("PWM_lCCU4_Start:start PWM")
  }
} /* end of PWM_lCCU4_Start() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Stops the CCU4_CC4 slice. */
void PWM_lCCU4_Stop(PWM_t *const handle_ptr)
{
  XMC_ASSERT("PWM_lCCU4_Stop:Invalid handle_ptr" , (handle_ptr != NULL))

  if (PWM_STATUS_UNINITIALIZED != handle_ptr->state)
  {
    XMC_CCU4_SLICE_StopTimer(handle_ptr->config_ptr->ccu4_slice_ptr);
    XMC_CCU4_SLICE_ClearTimer(handle_ptr->config_ptr->ccu4_slice_ptr);
    XMC_CCU4_DisableClock(handle_ptr->config_ptr->ccu4_kernel_ptr,handle_ptr->config_ptr->ccu4_slice_number);

    handle_ptr->state = PWM_STATUS_STOPPED;
    XMC_DEBUG("PWM_lCCU4_Stop:stop PWM")
  }
} /* end of PWM_lCCU4_Stop() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Sets the duty cycle (uint32_t) for CCU4_CC4 slice. */
PWM_STATUS_t PWM_lCCU4_SetDutyCycle(PWM_t *const handle_ptr, uint32_t duty_cycle)
{
  uint32_t period;
  uint32_t compare;
  PWM_STATUS_t status;

  XMC_ASSERT("PWM_lCCU4_SetDutyCycle:Invalid handle_ptr" , (handle_ptr != NULL))
  XMC_ASSERT("PWM_lCCU4_SetDutyCycle:Invalid duty_cycle " , ((duty_cycle >= 0) && (duty_cycle <= PWM_MAX_DUTY_CYCLE)))

  status = PWM_STATUS_FAILURE;
  if (PWM_STATUS_UNINITIALIZED != handle_ptr->state)
  {
    /* duty cycle has to be in between 0 and 100 */
    if(duty_cycle <= PWM_MAX_DUTY_CYCLE)
    {
      /* period = (PR + 1) */
      period = (uint32_t)handle_ptr->config_ptr->config_value->period_value + 1U;

      /* Duty Cycle(symmetric) = (PR-CR1)+1 / period */
      compare = ((period * ((uint32_t) PWM_MAX_DUTY_CYCLE - duty_cycle)) / ((uint32_t) 100 * PWM_DUTY_CYCLE_SCALE));

      handle_ptr->config_ptr->config_value->compare_value = compare;
      handle_ptr->config_ptr->config_value->duty_cycle = duty_cycle;

      XMC_CCU4_SLICE_SetTimerCompareMatch(handle_ptr->config_ptr->ccu4_slice_ptr, (uint16_t)(compare & 0xffffU));
      XMC_CCU4_EnableShadowTransfer(handle_ptr->config_ptr->ccu4_kernel_ptr,
                                    (uint32_t)((uint32_t)XMC_CCU4_SHADOW_TRANSFER_SLICE_0 <<
    		    		            (uint32_t)((uint32_t)handle_ptr->config_ptr->ccu4_slice_number << (uint32_t)2)));
      status = PWM_STATUS_SUCCESS;
    }
  }
  return (status);
  XMC_DEBUG("PWM_lCCU4_SetDutyCycle:dutycycle set")
} /* end of PWM_lCCU4_SetDutyCycle() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Sets the frequency for CCU4_CC4 slice. */
PWM_STATUS_t PWM_lCCU4_SetFreq(PWM_t *const handle_ptr, uint32_t pwm_freq_hz)
{
  PWM_STATUS_t status;
  uint32_t module_freq;
  uint8_t prescaler;
  uint32_t period_value;
  uint32_t compare;

  XMC_ASSERT("PWM_lCCU4_SetFreq:Invalid handle_ptr " , (handle_ptr != NULL))
  XMC_ASSERT("PWM_lCCU4_SetFreq:Invalid pwm_freq_hz " , (pwm_freq_hz != 0U))

  status = handle_ptr->state;
  prescaler = 0U;
  period_value = 0U;
  if (status != PWM_STATUS_UNINITIALIZED)
  {
    status = PWM_STATUS_SUCCESS;
    /*Calculate the new period value*/
    module_freq = handle_ptr->GLOBAL_CCU4_handle->module_frequency;

    while(prescaler < (uint8_t)16)
    {
      period_value = (uint32_t)(module_freq / ((uint32_t)((uint32_t) 1 << (uint32_t)prescaler) * pwm_freq_hz));
      if (period_value < (PWM_MAX_TIMER_COUNT + 1U) )
      {
        break;
      }

      prescaler++;
    }

    if (prescaler == (uint8_t)16)
    {
      period_value = PWM_MAX_TIMER_COUNT;
      XMC_DEBUG("PWM_lCCU4_SetFreq:frequency could not be set")
      status = PWM_STATUS_FAILURE;
    }
    else
    {
      /*Calculate the new compare values using new period values in no-timer concatenation mode*/
      compare = (period_value * ((uint32_t)PWM_MAX_DUTY_CYCLE - handle_ptr->config_ptr->config_value->duty_cycle))
    		     / ((uint32_t) 100 * PWM_DUTY_CYCLE_SCALE);

      XMC_CCU4_SLICE_SetPrescaler(handle_ptr->config_ptr->ccu4_slice_ptr, prescaler);

      XMC_CCU4_SLICE_SetTimerPeriodMatch(handle_ptr->config_ptr->ccu4_slice_ptr, (uint16_t)(--period_value));

      XMC_CCU4_SLICE_SetTimerCompareMatch(handle_ptr->config_ptr->ccu4_slice_ptr, (uint16_t)(compare & 0xffffU));

      XMC_CCU4_EnableShadowTransfer(handle_ptr->config_ptr->ccu4_kernel_ptr,
                                   (uint32_t)(((uint32_t)XMC_CCU4_SHADOW_TRANSFER_SLICE_0 |
                        		   (uint32_t)XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0) <<
	                    	       (uint32_t)((uint32_t)handle_ptr->config_ptr->ccu4_slice_number << (uint32_t)2)));

      handle_ptr->config_ptr->config_value->compare_value = compare;
      handle_ptr->config_ptr->config_value->period_value = period_value;
    }
  }

  XMC_DEBUG("PWM_lCCU4_SetFreq:frequency set")
  return status;
} /* end of PWM_lCCU4_SetFreq() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Sets the frequency and duty cycle for CCU4_CC4 slice. */
PWM_STATUS_t PWM_lCCU4_SetFreqAndDutyCycle(PWM_t *const handle_ptr, uint32_t pwm_freq_hz, uint32_t duty_cycle)
{
  PWM_STATUS_t status;
  uint32_t module_freq;
  uint8_t prescaler;
  uint32_t period_value;
  uint32_t compare;

  XMC_ASSERT("PWM_lCCU4_SetFreqAndDutyCycle:Invalid handle_ptr " , (handle_ptr != NULL))
  XMC_ASSERT("PWM_lCCU4_SetFreqAndDutyCycle:Invalid pwm_freq_hz " , (pwm_freq_hz != 0U))

  status = handle_ptr->state;
  prescaler = 0U;
  period_value = 0U;
  if (status != PWM_STATUS_UNINITIALIZED)
  {
    status = PWM_STATUS_SUCCESS;
    /*Calculate the new period value*/
    module_freq = handle_ptr->GLOBAL_CCU4_handle->module_frequency;

    while(prescaler < (uint8_t)16)
    {
      period_value = (uint32_t)(module_freq / ((uint32_t)((uint32_t) 1 << (uint32_t)prescaler) * pwm_freq_hz));
      if (period_value < (PWM_MAX_TIMER_COUNT + 1U) )
      {
        break;
      }

      prescaler++;
    }

    if ((prescaler == (uint8_t)16) || (duty_cycle > PWM_MAX_DUTY_CYCLE))
    {
      XMC_DEBUG("PWM_lCCU4_SetFreqAndDutyCycle:Frequency or duty cycle could not be set")
      status = PWM_STATUS_FAILURE;
    }
    else
    {
      /*Calculate the new compare values using new period values in no-timer concatenation mode*/
      compare = (period_value * ((uint32_t)PWM_MAX_DUTY_CYCLE - duty_cycle)) / ((uint32_t) 100 * PWM_DUTY_CYCLE_SCALE);

      XMC_CCU4_SLICE_SetPrescaler(handle_ptr->config_ptr->ccu4_slice_ptr, prescaler);

      XMC_CCU4_SLICE_SetTimerPeriodMatch(handle_ptr->config_ptr->ccu4_slice_ptr, (uint16_t)(--period_value));

      XMC_CCU4_SLICE_SetTimerCompareMatch(handle_ptr->config_ptr->ccu4_slice_ptr, (uint16_t)(compare & 0xffffU));

      XMC_CCU4_EnableShadowTransfer(handle_ptr->config_ptr->ccu4_kernel_ptr,
                                    (uint32_t)(((uint32_t)XMC_CCU4_SHADOW_TRANSFER_SLICE_0 |
                      		        (uint32_t)XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0) <<
	                      	        (uint32_t)((uint32_t)handle_ptr->config_ptr->ccu4_slice_number << (uint32_t)2)));

      handle_ptr->config_ptr->config_value->compare_value = compare;
      handle_ptr->config_ptr->config_value->period_value = period_value;
      handle_ptr->config_ptr->config_value->duty_cycle = duty_cycle;
    }
  }

  XMC_DEBUG("PWM_lCCU4_SetFreqAndDutyCycle:frequency set")
  return status;
} /* end of PWM_lCCU4_SetFreqAndDutyCycle() api */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Gets the interrupt status of  CCU4_CC4 slice. */
bool PWM_lCCU4_GetInterruptStatus(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt)
{
  bool status;
  XMC_ASSERT("PWM_lCCU4_GetInterruptStatus:Invalid handle_ptr" , (handle_ptr != NULL))

  status = XMC_CCU4_SLICE_GetEvent(handle_ptr->config_ptr->ccu4_slice_ptr, (XMC_CCU4_SLICE_IRQ_ID_t) pwm_interrupt);

  return (status);
} /* end of PWM_lCCU4_GetInterruptStatus() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Acknowledges the interrupt of  CCU4_CC4 slice. */
void PWM_lCCU4_AcknowledgeInterrupt(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt)
{
  XMC_ASSERT("PWM_lCCU4_AcknowledgeInterrupt:Invalid handle_ptr" , (handle_ptr != NULL))

  XMC_CCU4_SLICE_ClearEvent(handle_ptr->config_ptr->ccu4_slice_ptr, (XMC_CCU4_SLICE_IRQ_ID_t) pwm_interrupt);

  XMC_DEBUG("PWM_lCCU4_AcknowledgeInterrupt:Acknowledge Interrupt")
} /* end of PWM_lCCU4_AcknowledgeInterrupt() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Sets the passive level of CCU4_CC4 slice. */
void PWM_lCCU4_SetPassiveLevel(PWM_t *const handle_ptr, PWM_OutputPassiveLevelType_t pwm_output_passive_level)
{
  XMC_ASSERT("PWM_lCCU4_SetPassiveLevel:Invalid handle_ptr" , (handle_ptr != NULL))

  XMC_ASSERT("PWM_lCCU4_SetPassiveLevel:Invalid pwm_output_passive_level " , 
             (pwm_output_passive_level < PWM_OUTPUT_PASSIVE_LEVEL_MAX));

  XMC_CCU4_SLICE_SetPassiveLevel(handle_ptr->config_ptr->ccu4_slice_ptr,
		                         (XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_t)pwm_output_passive_level);

  XMC_DEBUG("PWM_lCCU4_SetPassiveLevel:passive level changed")
} /* end of PWM_lCCU4_SetPassiveLevel() api */

#endif /* end of CCU4 function definitions */

/***********************************************************************************************************************
 **                 CCU8 timer function definitions                                                                   **
 **********************************************************************************************************************/
#ifdef PWM_SLICE_USED_CCU8

/*Initialize the App and CCU8_CC8 slice. */
PWM_STATUS_t PWM_lCCU8_Init(PWM_t *const handle_ptr)
{
  PWM_STATUS_t status = PWM_STATUS_FAILURE;

  XMC_ASSERT("PWM_lCCU8_Init:Invalid handle_ptr" , (handle_ptr != NULL))

  if (PWM_STATUS_UNINITIALIZED == handle_ptr->state)
  {
    /* Initialize consumed Apps */
    status = (PWM_STATUS_t)GLOBAL_CCU8_Init(handle_ptr->GLOBAL_CCU8_handle);

    /*Initialize XMC_CCU8_CC8 slice */
    if (PWM_STATUS_SUCCESS == status)
    {
      XMC_DEBUG("PWM_lCCU8_Init:Initilizing slice A")
      XMC_CCU8_SLICE_CompareInit(handle_ptr->config_ptr->ccu8_slice_ptr, handle_ptr->config_ptr->ccu8_slice_config_ptr);

      /* Set the period and compare register values */
      XMC_CCU8_SLICE_SetTimerPeriodMatch(handle_ptr->config_ptr->ccu8_slice_ptr,
    		                            (uint16_t)handle_ptr->config_ptr->config_value->period_value);

      XMC_CCU8_SLICE_SetTimerCompareMatch(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_COMPARE_CHANNEL_1,
    		                              (uint16_t)handle_ptr->config_ptr->config_value->compare_value);

      XMC_CCU8_EnableShadowTransfer(handle_ptr->config_ptr->ccu8_kernel_ptr,
    		                 (uint32_t)(((uint32_t)XMC_CCU8_SHADOW_TRANSFER_SLICE_0 |
    		                		     (uint32_t)XMC_CCU8_SHADOW_TRANSFER_PRESCALER_SLICE_0) <<
    				         (uint32_t)((uint32_t)handle_ptr->config_ptr->ccu8_slice_number << (uint32_t)2)));

      /* Initialize interrupts */
      PWM_lCCU8_ConfigInterrupts(handle_ptr);

      XMC_GPIO_Init(handle_ptr->config_ptr->gpio_out_port,handle_ptr->config_ptr->gpio_out_pin,
    		        handle_ptr->config_ptr->gpio_out_config);

      handle_ptr->state = PWM_STATUS_SUCCESS;

      /* Start the PWM generation if start at initialization is enabled */
      if ((bool) true == handle_ptr->config_ptr->start_control)
      {
        PWM_Start(handle_ptr);
      }
      status = PWM_STATUS_SUCCESS;
    }
    else
    {
      handle_ptr->state = PWM_STATUS_UNINITIALIZED;
    }

  }
  return(status);
} /* end of PWM_lCCU8_Init() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Initialize interrupts */
void PWM_lCCU8_ConfigInterrupts(PWM_t *const handle_ptr)
{
	/* Enable events. If timer concatenation is enabled, events will be enabled for Slice A */
	/* Bind event to Service Request Node If timer concatenation is not enabled, configure Slice A*/
	if ((bool) true == handle_ptr->config_ptr->period_match_enable)
	{
	  XMC_DEBUG("PWM_lCCU8_Init:period match enable slice A")

	  XMC_CCU8_SLICE_EnableEvent(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH);
	  XMC_CCU8_SLICE_SetInterruptNode(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH,
		  handle_ptr->config_ptr->ccu8_slice_period_match_node);
	}

	if ((bool) true == handle_ptr->config_ptr->compare_match_enable)
	{
	  XMC_DEBUG("PWM_lCCU8_Init:compare match enable slice A")

	  XMC_CCU8_SLICE_EnableEvent(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_1);
	  XMC_CCU8_SLICE_SetInterruptNode(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_1,
		  handle_ptr->config_ptr->ccu8_slice_compare_match_node);
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Starts the CCU8_CC8 slice. */
void PWM_lCCU8_Start(PWM_t *const handle_ptr)
{
  XMC_ASSERT("PWM_lCCU8_Start:Invalid handle_ptr" , (handle_ptr != NULL))

  if ((PWM_STATUS_SUCCESS == handle_ptr->state) || (PWM_STATUS_STOPPED == handle_ptr->state))
  {
    /* clear IDLE mode for the slice */
    /* The MSB timer is started first since it does not
     * increment until lower period match happens.*/
    XMC_CCU8_EnableClock(handle_ptr->config_ptr->ccu8_kernel_ptr,handle_ptr->config_ptr->ccu8_slice_number);
    XMC_CCU8_SLICE_StartTimer(handle_ptr->config_ptr->ccu8_slice_ptr);

    handle_ptr->state = PWM_STATUS_RUNNING;
    XMC_DEBUG("PWM_lCCU8_Start:start PWM")
  }
} /* end of PWM_lCCU8_Start() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Stops the CCU8_CC8 slice. */
void PWM_lCCU8_Stop(PWM_t *const handle_ptr)
{
  XMC_ASSERT("PWM_lCCU8_Stop:Invalid handle_ptr" , (handle_ptr != NULL))

  if (PWM_STATUS_UNINITIALIZED != handle_ptr->state)
  {
    XMC_CCU8_SLICE_StopTimer(handle_ptr->config_ptr->ccu8_slice_ptr);
    XMC_CCU8_SLICE_ClearTimer(handle_ptr->config_ptr->ccu8_slice_ptr);
    XMC_CCU8_DisableClock(handle_ptr->config_ptr->ccu8_kernel_ptr, handle_ptr->config_ptr->ccu8_slice_number);

    handle_ptr->state = PWM_STATUS_STOPPED;
    XMC_DEBUG("PWM_lCCU8_Stop:stop PWM")
  }
} /* end of PWM_lCCU8_Stop() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Sets the duty cycle (uint32_t) for CCU8_CC8 slice. */
PWM_STATUS_t PWM_lCCU8_SetDutyCycle(PWM_t *const handle_ptr, uint32_t duty_cycle)
{
  uint32_t period;
  uint32_t compare;
  PWM_STATUS_t status;

  XMC_ASSERT("PWM_lCCU8_SetDutyCycle:Invalid handle_ptr" , (handle_ptr != NULL))
  XMC_ASSERT("PWM_lCCU8_SetDutyCycle:Invalid duty_cycle " , ((duty_cycle >= 0) && (duty_cycle <= 100)))

  status = PWM_STATUS_FAILURE;
  if (handle_ptr->state != PWM_STATUS_UNINITIALIZED)
  {
    if(duty_cycle <= PWM_MAX_DUTY_CYCLE)
    {
	  period = (uint32_t)handle_ptr->config_ptr->config_value->period_value + 1U;

      /* Duty Cycle(symmetric) = (PR-CR1)+1 / period */
      compare = ((period * ((uint32_t) PWM_MAX_DUTY_CYCLE - duty_cycle)) / ((uint32_t) 100 * PWM_DUTY_CYCLE_SCALE));

      handle_ptr->config_ptr->config_value->compare_value = compare;
      handle_ptr->config_ptr->config_value->duty_cycle = duty_cycle;

      XMC_CCU8_SLICE_SetTimerCompareMatch(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_COMPARE_CHANNEL_1,
    		                             (uint16_t)(compare & 0xffffU));

      XMC_CCU8_EnableShadowTransfer(handle_ptr->config_ptr->ccu8_kernel_ptr,
    		                       (uint32_t)((uint32_t)XMC_CCU8_SHADOW_TRANSFER_SLICE_0 <<
    		    				   (uint32_t)((uint32_t)handle_ptr->config_ptr->ccu8_slice_number << (uint32_t)2)));
      status = PWM_STATUS_SUCCESS;
    }
  }

  return (status);
  XMC_DEBUG("PWM_lCCU8_SetDutyCycle:dutycycle set")
} /* end of PWM_lCCU8_SetDutyCycle() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Sets the frequency for CCU8_CC8 slice. */
PWM_STATUS_t PWM_lCCU8_SetFreq(PWM_t *const handle_ptr, uint32_t pwm_freq_hz)
{
  PWM_STATUS_t status;
  uint32_t module_freq;
  uint8_t prescaler;
  uint32_t period_value;
  uint32_t compare;

  XMC_ASSERT("PWM_lCCU8_SetFreq:Invalid handle_ptr" , (handle_ptr != NULL))
  XMC_ASSERT("PWM_lCCU8_SetFreq:Invalid pwm_freq_hz " , (pwm_freq_hz != 0U))

  status = handle_ptr->state;
  prescaler = 0U;
  period_value = 0U;
  if (status != PWM_STATUS_UNINITIALIZED)
  {
    status = PWM_STATUS_SUCCESS;
    module_freq = handle_ptr->GLOBAL_CCU8_handle->module_frequency;

    while(prescaler < (uint8_t)16)
    {
	  period_value = (uint32_t)(module_freq / ((uint32_t)((uint32_t) 1 << (uint32_t)prescaler) * pwm_freq_hz));
	  if (period_value < (PWM_MAX_TIMER_COUNT + 1U) )
	  {
		break;
	  }

	  prescaler++;
    }

    if (prescaler == (uint8_t)16)
    {
      XMC_DEBUG("PWM_lCCU8_SetFreq:frequency could not be set")
      status = PWM_STATUS_FAILURE;
    }
    else
    {
      /*Calculate the new compare values using new period values in no-timer concatenation mode*/
      compare = (period_value * (PWM_MAX_DUTY_CYCLE - handle_ptr->config_ptr->config_value->duty_cycle))
          		     / ((uint32_t) 100 * PWM_DUTY_CYCLE_SCALE);

      XMC_CCU8_SLICE_SetPrescaler(handle_ptr->config_ptr->ccu8_slice_ptr, prescaler);

      XMC_CCU8_SLICE_SetTimerPeriodMatch(handle_ptr->config_ptr->ccu8_slice_ptr, (uint16_t)(--period_value));

      XMC_CCU8_SLICE_SetTimerCompareMatch(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_COMPARE_CHANNEL_1,
    		                             (uint16_t)(compare & 0xffffU));

      XMC_CCU8_EnableShadowTransfer(handle_ptr->config_ptr->ccu8_kernel_ptr,
    	                            (uint32_t)(((uint32_t)XMC_CCU8_SHADOW_TRANSFER_SLICE_0 |
    		                		(uint32_t)XMC_CCU8_SHADOW_TRANSFER_PRESCALER_SLICE_0) <<
    		    			        (uint32_t)((uint32_t)handle_ptr->config_ptr->ccu8_slice_number << (uint32_t)2)));

      handle_ptr->config_ptr->config_value->compare_value = compare;
      handle_ptr->config_ptr->config_value->period_value = period_value;
	}
  }

  XMC_DEBUG("PWM_lCCU8_SetFreq:frequency set")
  return status;
} /* end of PWM_lCCU8_SetFreq() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Sets the frequency and duty cycle for CCU8_CC8 slice. */
PWM_STATUS_t PWM_lCCU8_SetFreqAndDutyCycle(PWM_t *const handle_ptr, uint32_t pwm_freq_hz, uint32_t duty_cycle)
{
  PWM_STATUS_t status;
  uint32_t module_freq;
  uint8_t prescaler;
  uint32_t period_value;
  uint32_t compare;

  XMC_ASSERT("PWM_lCCU8_SetFreqAndDutyCycle:Invalid handle_ptr" , (handle_ptr != NULL))
  XMC_ASSERT("PWM_lCCU8_SetFreqAndDutyCycle:Invalid pwm_freq_hz " , (pwm_freq_hz != 0U))
  XMC_ASSERT("PWM_lCCU8_SetDutyCycleFloat:Invalid duty_cycle " , ((duty_cycle >= 0) && (duty_cycle <= 100)))

  status = handle_ptr->state;
  prescaler = 0U;
  period_value = 0U;
  if (status != PWM_STATUS_UNINITIALIZED)
  {
    status = PWM_STATUS_SUCCESS;
    module_freq = handle_ptr->GLOBAL_CCU8_handle->module_frequency;

    while(prescaler < (uint8_t)16)
    {
	  period_value = (uint32_t)(module_freq / ((uint32_t)((uint32_t) 1 << (uint32_t)prescaler) * pwm_freq_hz));
	  if (period_value < (PWM_MAX_TIMER_COUNT + 1U) )
	  {
		break;
	  }

	  prescaler++;
    }

    if ((prescaler == (uint8_t)16) || (duty_cycle > PWM_MAX_DUTY_CYCLE))
    {
      XMC_DEBUG("PWM_lCCU8_SetFreqAndDutyCycle:frequency could not be set")
      status = PWM_STATUS_FAILURE;
    }
    else
    {
      /*Calculate the new compare values using new period values in no-timer concatenation mode*/

      compare = (period_value * ((uint32_t)PWM_MAX_DUTY_CYCLE - duty_cycle)) / ((uint32_t) 100 * PWM_DUTY_CYCLE_SCALE);

      XMC_CCU8_SLICE_SetPrescaler(handle_ptr->config_ptr->ccu8_slice_ptr, prescaler);

      XMC_CCU8_SLICE_SetTimerPeriodMatch(handle_ptr->config_ptr->ccu8_slice_ptr, (uint16_t)(--period_value));

      XMC_CCU8_SLICE_SetTimerCompareMatch(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_COMPARE_CHANNEL_1,
    		                           (uint16_t)(compare & 0xffffU));

      XMC_CCU8_EnableShadowTransfer(handle_ptr->config_ptr->ccu8_kernel_ptr,
    		                        (uint32_t)(((uint32_t)XMC_CCU8_SHADOW_TRANSFER_SLICE_0 |
    		                		(uint32_t)XMC_CCU8_SHADOW_TRANSFER_PRESCALER_SLICE_0) <<
    		    			        (uint32_t)((uint32_t)handle_ptr->config_ptr->ccu8_slice_number << (uint32_t)2)));

      handle_ptr->config_ptr->config_value->compare_value = compare;
      handle_ptr->config_ptr->config_value->period_value = period_value;
      handle_ptr->config_ptr->config_value->duty_cycle = duty_cycle;
    }
  }

  XMC_DEBUG("PWM_lCCU8_SetFreqAndDutyCycle:frequency set")
  return status;
} /* end of PWM_lCCU8_SetFreq() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Gets the interrupt status of  CCU8_CC8 slice. */
bool PWM_lCCU8_GetInterruptStatus(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt)
{
  bool status = (bool) false;
  XMC_ASSERT("PWM_lCCU8_GetInterruptStatus:Invalid handle_ptr" , (handle_ptr != NULL))

  status = XMC_CCU8_SLICE_GetEvent(handle_ptr->config_ptr->ccu8_slice_ptr, (XMC_CCU8_SLICE_IRQ_ID_t) pwm_interrupt);

  return (status);
} /* end of PWM_lCCU8_GetInterruptStatus() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Acknowledges the interrupt of  CCU8_CC8 slice. */
void PWM_lCCU8_AcknowledgeInterrupt(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt)
{
  XMC_ASSERT("PWM_lCCU8_AcknowledgeInterrupt:Invalid handle_ptr" , (handle_ptr != NULL))

  XMC_CCU8_SLICE_ClearEvent(handle_ptr->config_ptr->ccu8_slice_ptr, (XMC_CCU8_SLICE_IRQ_ID_t) pwm_interrupt);

  XMC_DEBUG("PWM_lCCU8_AcknowledgeInterrupt:Acknowledge Interrupt")
} /* end of PWM_lCCU8_AcknowledgeInterrupt() api */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Sets the passive level of CCU8_CC8 slice. */
void PWM_lCCU8_SetPassiveLevel(PWM_t *const handle_ptr, PWM_OutputPassiveLevelType_t pwm_output_passive_level)
{
  XMC_ASSERT("PWM_lCCU8_SetPassiveLevel:Invalid handle_ptr" , (handle_ptr != NULL))

  XMC_CCU8_SLICE_SetPassiveLevel(handle_ptr->config_ptr->ccu8_slice_ptr, XMC_CCU8_SLICE_OUTPUT_0,
		                        (XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t)pwm_output_passive_level);

  XMC_DEBUG("PWM_lCCU8_SetPassiveLevel:passive level changed")
} /* end of PWM_lCCU8_SetPassiveLevel() api */

#endif /* end of CCU8 function definitions */

/********************************************************************************
 **                 Function definitions                                       **
 *******************************************************************************/

/*This function returns the version of the PWM App*/
DAVE_APP_VERSION_t PWM_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = (uint8_t) PWM_MAJOR_VERSION;
  version.minor = (uint8_t) PWM_MINOR_VERSION;
  version.patch = (uint8_t) PWM_PATCH_VERSION;

  return version;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* This function initializes the app */
PWM_STATUS_t PWM_Init(PWM_t *const handle_ptr)
{
  PWM_STATUS_t status;
  status = PWM_STATUS_FAILURE;

#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
    status = PWM_lCCU4_Init(handle_ptr);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
    status = PWM_lCCU8_Init(handle_ptr);
  }
#endif

  return (status);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* This function starts the app. This needs to be called even if external start is configured.*/
void PWM_Start(PWM_t *const handle_ptr)
{
#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
    PWM_lCCU4_Start(handle_ptr);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
    PWM_lCCU8_Start(handle_ptr);
  }
#endif
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* This function stops the app */
void PWM_Stop(PWM_t *const handle_ptr)
{
#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
    PWM_lCCU4_Stop(handle_ptr);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
    PWM_lCCU8_Stop(handle_ptr);
  }
#endif
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*This function is used to set the duty cycle (uint32_t) of the PWM waveform  */
PWM_STATUS_t PWM_SetDutyCycle(PWM_t *const handle_ptr, uint32_t duty_cycle)
{
  PWM_STATUS_t status;
  status = PWM_STATUS_FAILURE;

#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
	  status = PWM_lCCU4_SetDutyCycle(handle_ptr, duty_cycle);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
	  status = PWM_lCCU8_SetDutyCycle(handle_ptr, duty_cycle);
  }
#endif
  return (status);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*This function changes the PWM frequency. Input parameter is the frequency value in Hz */
PWM_STATUS_t PWM_SetFreq(PWM_t *const handle_ptr, uint32_t pwm_freq_hz)
{
  PWM_STATUS_t status;
  status = PWM_STATUS_FAILURE;
#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
    status = PWM_lCCU4_SetFreq(handle_ptr, pwm_freq_hz);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
    status = PWM_lCCU8_SetFreq(handle_ptr, pwm_freq_hz);
  }
#endif
  return status;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*This function sets frequency and the duty cycle */
PWM_STATUS_t PWM_SetFreqAndDutyCycle(PWM_t *const handle_ptr, uint32_t pwm_freq_hz, uint32_t duty_cycle)
{
  PWM_STATUS_t status;
  status = PWM_STATUS_FAILURE;

#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
	  status = PWM_lCCU4_SetFreqAndDutyCycle(handle_ptr, pwm_freq_hz,duty_cycle);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
	  status = PWM_lCCU8_SetFreqAndDutyCycle(handle_ptr, pwm_freq_hz,duty_cycle);
  }
#endif
  return status;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*This function changes the PWM timer status_timer */
bool PWM_GetTimerStatus(PWM_t *const handle_ptr)
{
  bool status_timer;
  status_timer = (bool)false;
#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
    status_timer = XMC_CCU4_SLICE_IsTimerRunning(handle_ptr->config_ptr->ccu4_slice_ptr);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
    status_timer = XMC_CCU8_SLICE_IsTimerRunning(handle_ptr->config_ptr->ccu8_slice_ptr);
  }
#endif

  return (status_timer);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*This function returns the interrupt status_timer */
bool PWM_GetInterruptStatus(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt)
{
  bool status;
  status = (bool) false;

#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
    status = PWM_lCCU4_GetInterruptStatus(handle_ptr, pwm_interrupt);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
    status = PWM_lCCU8_GetInterruptStatus(handle_ptr, pwm_interrupt);
  }
#endif

  return status;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*This function Acknowledges the corresponding interrupt */
void PWM_AcknowledgeInterrupt(PWM_t *const handle_ptr, PWM_InterruptType_t pwm_interrupt)
{
#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
    PWM_lCCU4_AcknowledgeInterrupt(handle_ptr, pwm_interrupt);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
    PWM_lCCU8_AcknowledgeInterrupt(handle_ptr, pwm_interrupt);
  }
#endif
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*This function Acknowledges the corresponding interrupt */
void PWM_SetPassiveLevel(PWM_t *const handle_ptr, PWM_OutputPassiveLevelType_t pwm_output_passive_level)
{
#ifdef PWM_SLICE_USED_CCU4
  if (PWM_TIMER_SLICE_CCU4 == handle_ptr->timer_type)
  {
    PWM_lCCU4_SetPassiveLevel(handle_ptr, pwm_output_passive_level);
  }
#endif

#ifdef PWM_SLICE_USED_CCU8
  if (PWM_TIMER_SLICE_CCU8 == handle_ptr->timer_type)
  {
    PWM_lCCU8_SetPassiveLevel(handle_ptr, pwm_output_passive_level);
  }
#endif
}

/*CODE_BLOCK_END*/

