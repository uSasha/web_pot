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
* @file global_scu.h
* @date 16 Feb, 2015
* @version 4.0.0
*
* @brief global_scu.h provides interface between low level drivers <br>
*
* <b>Detailed description of file</b> <br>
* This file contains enumeration, structure definitions and function
* prototypes of GLOBAL_SCU_XMC4 APP for XMC4000 devices.
*
* VERSION HISTORY:
* 16 Feb 2015	v4.0.0		Initial Version
*
*/  
#ifndef GLOBAL_SCU_XMC4_H
#define GLOBAL_SCU_XMC4_H

/******************************************************************************
**                      INCLUDE FILES                                        **
******************************************************************************/
#include <xmc_scu.h>
#include <DAVE_Common.h>
#include "CPU_CTRL_XMC4\cpu_ctrl_xmc4.h"

#include "global_scu_xmc4_conf.h"

/**
 * @addtogroup GLOBAL_SCU_XMC4
 * @{
 */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

/*********************************************************************************
 * ENUMS
 *********************************************************************************/
/**
 * @ingroup GLOBAL_SCU_XMC4_enumerations
 * @{
 */
/**
 * Initialization status.
 */
typedef enum GLOBAL_SCU_XMC4_STATUS
{
  GLOBAL_SCU_XMC4_STATUS_SUCCESS = 0U,  /**< Initialization status success */
  GLOBAL_SCU_XMC4_STATUS_FAILURE = 1U	/**< Initialization status failure */
} GLOBAL_SCU_XMC4_STATUS_t;

/**
 * enum structure which holds the callback status type for service requests
 */
typedef enum GLOBAL_SCU_XMC4_RegisterCbStatusType
{
  GLOBAL_SCU_XMC4_REGISTER_SUCCESS = 0, /**< Callback status success */
  GLOBAL_SCU_XMC4_REGISTER_BUSY = 1,    /**< Callback status busy */
  GLOBAL_SCU_XMC4_REGISTER_FAILURE = 2  /**< Callback status failure */
} GLOBAL_SCU_XMC4_RegisterCbStatusType;

typedef XMC_SCU_INTERRUPT_EVENT_t GLOBAL_SCU_INTERRUPT_EVENT_t;
typedef XMC_SCU_INTERRUPT_EVENT_HANDLER_t GLOBAL_SCU_EVENT_HANDLER_t;

/**
 * @}
 */

/******************************************************************************
**                      DATA STRUCTURES                                      **
******************************************************************************/
/**
 * @ingroup GLOBAL_SCU_XMC4_datastructures
 * @{
 */

/**
 * @brief Structure which holds the interrupt node configuration.
 *  Note: Sub-priority is not available for XMC1000 Devices
 */
typedef struct GLOBAL_SCU_XMC4_CONFIG
{
  const IRQn_Type node; /**< Mapped NVIC Node */
  uint8_t priority;    /**<  Node Interrupt Priority    */    
  uint8_t subpriority; /**<  Node Interrupt Subpriority */
  bool enable_at_init; /**<  Interrupt enable for Node  */
} GLOBAL_SCU_XMC4_CONFIG_t;

/**
 * Structure which holds the init status configuration
 *
 */
typedef struct GLOBAL_SCU_XMC4
{
  const GLOBAL_SCU_XMC4_CONFIG_t* config;  /**< Config Handle */
  bool initialized;                        /**< Initialized status */
} GLOBAL_SCU_XMC4_t;
/**
 * @}
 */

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
**                      API DEFINITIONS                                      **
******************************************************************************/
/**
 * @ingroup GLOBAL_SCU_XMC4_apidoc
 * @{
 */
/**
 * @brief Get GLOBAL_SCU_XMC4 APP version.
 * @return \a DAVE_APP_VERSION_t APP version information (major, minor and patch number)
 *
 * \par<b>Description: </b><br>
 * The function can be used to check application software compatibility with a
 * specific version of the APP.
 * @code
 * #include <DAVE.h>
 *  int main(void)
 *  {
 *    DAVE_APP_VERSION_t version;
 *    DAVE_Init();
 *    version = GLOBAL_SCU_XMC4_GetAppVersion();
 *    if(version.major != 4U)
 *    {
 *    }
 *    return 0;
 *  }
 * @endcode<BR> </p>
 */
DAVE_APP_VERSION_t GLOBAL_SCU_XMC4_GetAppVersion(void);

/**
 * @brief Initializes GLOBAL_SCU_XMC4 APP.
 * @param handle Constant pointer to structure of type @ref GLOBAL_SCU_XMC4_t
 * @return @ref GLOBAL_SCU_XMC4_STATUS_t APP Status
 *
 * @code
 *  #include <DAVE.h>
 *
 *  int main(void)
 *  {
 *    DAVE_Init(); // GLOBAL_SCU_XMC4_Init(&GLOBAL_SCU_XMC4_0) is called within DAVE_Init()
 *    // .....
 *    return 0;
 *  }
 *
 * @endcode<BR> </p>
 */
GLOBAL_SCU_XMC4_STATUS_t GLOBAL_SCU_XMC4_Init(GLOBAL_SCU_XMC4_t *const handle);

/**
 * @brief Registers event handler functions for a specific SCU Event.
 *
 * @param  event Event ID
 * @param  handler  userFunction User defined Event Handler
 * @return @ref GLOBAL_SCU_XMC4_RegisterCbStatusType
 *
 *\par<b>Description: </b><br>
 * The user registers the call back function and if the event is unmasked,the interrupt
 * handler will service the Event as well as clear the event status bit.
 *
 * @code
 *  #include <DAVE.h>
 *
 *  void CbListener(void)
 *  {// Call back function implementation
 *  };
 *  int main(void)
 *  {
 *    DAVE_Init(); // GLOBAL_SCU_XMC4_Init(&GLOBAL_SCU_XMC4_0) is called within DAVE_Init()
 *    // Example usage of API
 *    GLOBAL_SCU_XMC4_RegisterCallback(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC,CbListener);
 *    // .....
 *    return 0;
 *  }
 *
 * @endcode<BR> </p>
 */
GLOBAL_SCU_XMC4_RegisterCbStatusType GLOBAL_SCU_XMC4_RegisterCallback
(
  const GLOBAL_SCU_INTERRUPT_EVENT_t event,
  const GLOBAL_SCU_EVENT_HANDLER_t handler
);

/**
 *@}
 */
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#include "global_scu_xmc4_extern.h"

#endif 
