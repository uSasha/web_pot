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
** PLATFORM : Infineon XMC4000 Series                                         **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR : APP Developer                                                     **
**                                                                            **
** MAY BE CHANGED BY USER [yes/no]: Yes                                       **
**                                                                            **
** MODIFICATION DATE : Feb 16, 2015                                           **
**                                                                            **
*******************************************************************************/

/**
* @file global_scu_xmc4.c
* @date 16 Feb, 2015
* @version 4.0.0
*
* @brief global_scu_xmc4.c provides implementation of APIs<br>
*
* <b>Detailed description of file</b> <br>
* This file contains public and private function implementations of 
* global_scu_xmc4 App for XMC4000 devices
*
* History
*
* Version 4.0.0  Initial version
*
*/
/*******************************************************************************
**                      Author(s) Identity                                    **
********************************************************************************
**                                                                            **
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** SNR        APP Developer                                                   **
*******************************************************************************/

/*******************************************************************************
 ** INCLUDE FILES                                                             **
 ******************************************************************************/
#include "global_scu_xmc4.h"
/*******************************************************************************
 * MACROS
 *******************************************************************************/
void SCU_0_IRQHandler(void);
/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

/*******************************************************************************/

/*
 * API to retrieve the version of the GLOBAL_SCU_XMC4 APP
 */
DAVE_APP_VERSION_t GLOBAL_SCU_XMC4_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = GLOBAL_SCU_XMC4_MAJOR_VERSION;
  version.minor = GLOBAL_SCU_XMC4_MINOR_VERSION;
  version.patch = GLOBAL_SCU_XMC4_PATCH_VERSION;

  return version;
}

/*  Function to configure SCU Interrupts based on user configuration.
 * 
 */
GLOBAL_SCU_XMC4_STATUS_t GLOBAL_SCU_XMC4_Init(GLOBAL_SCU_XMC4_t*const handle)
{
  GLOBAL_SCU_XMC4_STATUS_t initstatus;
  CPU_CTRL_XMC4_STATUS_t CPU_initstatus;

  XMC_ASSERT("GLOBAL_SCU_XMC4_Init:HandlePtr NULL", (handle != NULL));

  initstatus = GLOBAL_SCU_XMC4_STATUS_SUCCESS;

  CPU_initstatus = CPU_CTRL_XMC4_Init(CPU_CTRL_HANDLE);

  if(CPU_initstatus == CPU_CTRL_XMC4_STATUS_SUCCESS)
  {
	 if(handle->initialized == false &&  (handle->config->enable_at_init == true))
	 {
       NVIC_SetPriority((IRQn_Type)0,
	                    NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
	                    (uint32_t)(handle->config->priority),
	                    (uint32_t)(handle->config->subpriority)));

	   NVIC_EnableIRQ((IRQn_Type)0);

	   handle->initialized = true;
	 }
  }
  else
  {
    initstatus = GLOBAL_SCU_XMC4_STATUS_FAILURE;
  }

  return (initstatus);
}


/*  Utility function to register Event Handler for a given event.
 * 
 */
GLOBAL_SCU_XMC4_RegisterCbStatusType GLOBAL_SCU_XMC4_RegisterCallback
(
  const GLOBAL_SCU_INTERRUPT_EVENT_t event,
  const GLOBAL_SCU_EVENT_HANDLER_t handler
)
{
  GLOBAL_SCU_XMC4_RegisterCbStatusType status;

  status = (GLOBAL_SCU_XMC4_RegisterCbStatusType)XMC_SCU_INTERRUPT_SetEventHandler(event, handler);

  return (status);
}

/*  SCU Interrupt Handler.
 * 
 */
void SCU_0_IRQHandler(void)
{
	XMC_SCU_IRQHandler(0);
}     

