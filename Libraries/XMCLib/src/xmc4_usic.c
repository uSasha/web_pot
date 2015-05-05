/*
 * Copyright (C) 2015 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

/**
 * @file xmc4_usic.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief Template driver for XMC microcontroller family.
 *
 * History <br>
 *
 * Version 1.0.0  Initial <br>
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/
#include <xmc_usic.h>

#if UC_FAMILY == XMC4
#include <xmc_scu.h>

/*******************************************************************************
 * API Implementation
 *******************************************************************************/
/**
 * @brief API to enable the USIC driver
 * @retval void
 *
 */
void XMC_USIC_Enable(XMC_USIC_t *const usic)
{
  if (usic == USIC0)
  {
#if(UC_SERIES != XMC45)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC0);
#endif
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC0);
  }
#if defined(USIC1)  
  else if (usic == USIC1)
  {
#if(UC_SERIES != XMC45)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC1);
#endif	
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC1);
  }
#endif  
#if defined(USIC2)  
  else if (usic == USIC2)
  {
#if(UC_SERIES != XMC45)  
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_CLOCK_USIC2);
#endif
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC2);
  }
#endif  
  else
  {
    XMC_ASSERT("USIC module not available", 0/*Always*/);
  }
}
/**
 * @brief API to Disable the USIC driver
 * @retval void
 *
 */
void XMC_USIC_Disable(XMC_USIC_t *const usic)
{
  if (usic == (XMC_USIC_t *)USIC0)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC0);
#if(UC_SERIES != XMC45)
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC0);
#endif
  }
#if defined(USIC1)  
  else if (usic == (XMC_USIC_t *)USIC1)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC1);
#if(UC_SERIES != XMC45)
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_USIC1);
#endif
  }
#endif  
#if defined(USIC2)  
  else if (usic == (XMC_USIC_t *)USIC2)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USIC2);
#if(UC_SERIES != XMC45)
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_CLOCK_USIC2);
#endif
  }
#endif  
  else
  {
	  XMC_ASSERT("USIC module not available", 0/*Always*/);
  }
  
}

#endif /* UC_FAMILY == XMC4 */
