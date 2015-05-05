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
** PLATFORM : Infineon XMC4000 Series       			                      **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR   : APP Developer                                                   **
**                                                                            **
** MAY BE CHANGED BY USER [yes/no]: Yes                                       **
**                                                                            **
** MODIFICATION DATE : 16 Feb, 2015                                           **
**                                                                            **
*******************************************************************************/
/**
* @file cpu_ctrl_xmc4.c
* @date 16 Feb, 2015
* @version 4.0.0
*
* @brief cpu_ctrl_xmc4.c provides implementation of APIs<br>
*
* <b>Detailed description of file</b> <br>
* This file contains public and private function implementations of CPU_CTRL_XMC4 APP.
*
* History
*
** 16 Feb 2015	v4.0.0		Initial Version
*
*/

/*********************************************************************************************************************
 * INCLUDE FILES
 ********************************************************************************************************************/
#include "cpu_ctrl_xmc4.h"

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
/*
 * API to retrieve the version of the CPU_CTRL_XMC4 APP
 */
DAVE_APP_VERSION_t CPU_CTRL_XMC4_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = CPU_CTRL_XMC4_MAJOR_VERSION;
  version.minor = CPU_CTRL_XMC4_MINOR_VERSION;
  version.patch = CPU_CTRL_XMC4_PATCH_VERSION;

  return version;
}
/*
 * API to initialize the CPU_CTRL_XMC4 APP
 */

CPU_CTRL_XMC4_STATUS_t CPU_CTRL_XMC4_Init(CPU_CTRL_XMC4_t *const handler)
{
  CPU_CTRL_XMC4_STATUS_t status;

  if(handler->initialized == false)
  {
	NVIC_SetPriorityGrouping(handler->config->prigroup);

	handler->initialized = true;

	status = CPU_CTRL_XMC4_STATUS_SUCCESS;
  }
  else
  {
    /* returning SUCCESS if APP is already initialized */
	status = CPU_CTRL_XMC4_STATUS_SUCCESS;
  }

  return (status);
}

