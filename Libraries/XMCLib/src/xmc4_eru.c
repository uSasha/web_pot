/*
 * Copyright (C) 2015 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY
 * REASON WHATSOEVER.
 *
 */

/**
 * @file xmc4_eru.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief ERU driver for XMC microcontroller family.
 *
 * History <br>
 *
 * Version 1.0.0 Initial version <br>
 */

#include "xmc_eru.h"
#include "xmc_scu.h"

#if UC_FAMILY == XMC4

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
/* Enable the clock and De-assert the ERU module from the reset state. */
void XMC_ERU_Enable(XMC_ERU_t *const eru)
{
#if defined(XMC_ERU1)
  if (eru == XMC_ERU1)
  {
    #if (UC_SERIES != XMC45) 
      XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_ERU1);
    #endif
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_ERU1);
  }
#else
  XMC_UNUSED_ARG(eru);
#endif
}

/* Disable the clock and Reset the ERU module. */
void XMC_ERU_Disable(XMC_ERU_t *const eru)
{
#if defined(XMC_ERU1)
  if (eru == XMC_ERU1)
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_ERU1);
    #if (UC_SERIES != XMC45) 
      XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_ERU1);
    #endif
  }
#else
  XMC_UNUSED_ARG(eru);
#endif
}

#endif /* if( UC_FAMILY == XMC1 ) */
