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
 *Neither the name of the copyright holders nor the names of its              **
 contributors may be used to endorse or promote products derived from this    **
 software without specific prior written permission.                          **
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
** PLATFORM : Infineon XMC4000 Series                                         **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR   : APP Developer                                                   **
**                                                                            **
** NOTE     : Any Changes made to this file will be overwritten during Code   **
**            Generation                                                      **
**                                                                            **
*******************************************************************************/
/**
* @file clock_xmc4.h
* @date 16 Feb, 2015
* @version 4.0.0
*
* @brief Provides interface between low level drivers and APP<br>
*
* <b>Detailed description of file</b> <br>
* This file contains Enumerations, Structure definitions and function
* prototypes of APP.
*
* History
*
* 4.0.0       16-Feb-2015   Initial version for DAVEv4
*/
#ifndef CLOCK_XMC4_H
#define CLOCK_XMC4_H

/*******************************************************************************
**                           Include Files                                    **
*******************************************************************************/
#include <xmc_scu.h>
#include <xmc_gpio.h>
#include <DAVE_common.h>
#include "clock_xmc4_conf.h"

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/**
 * @ingroup App_publicparam
 * @{
 */
/**
 *  @brief Initialization data structure for CLOCK_XMC4 APP
 */

/**
 * @}
 */

/**
 * @ingroup CLOCK_XMC4_enumerations
 * @{
 */
/* No enums provided */
/**
 * @}
 */

/**
 * @ingroup CLOCK_XMC4_datastructures
 * @{
 */
/**
 * @}
 */

#ifdef __cplusplus
extern "C" {
#endif
/***********************************************************************************************************************
**                                                API prototypes                                                      **
***********************************************************************************************************************/
/**
 * @ingroup CLOCK_XMC4_apidoc
 * @{
 */
/**
 * @brief Get CLOCK_XMC4 APP version
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
 *   // Initialize CLOCK_XMC4 APP:
 *   // SystemCoreClockSetup() is called from SystemInit().
 *   init_status = DAVE_Init();
 *
 *   version = CLOCK_XMC4_GetAppVersion();
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
 * @endcode<BR>
 */
DAVE_APP_VERSION_t CLOCK_XMC4_GetDriverVersion(void);

/**
 * @brief This is a non-weak function, which retrieves high precision external oscillator frequency.
 * <b>Note: This function is used by xmc4_scu LLD for internal operations. Therefore the user do not required to call this API explicitly.</b>
 *
 * @return uint32_t
 *
 * \par<b>Description:</b><br>
 * This function to retrieves the external high precision oscillator frequency value, derived from either "External Crystal Mode" or
 * "External Clock Input Mode"
 * <BR>
 */
uint32_t OSCHP_GetFrequency(void);

/**
 * @}
 */

#include"clock_xmc4_extern.h"

#ifdef __cplusplus
}
#endif   

#endif /* End of CLOCK_XMC4_H */
