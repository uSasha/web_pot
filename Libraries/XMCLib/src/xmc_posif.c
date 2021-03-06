/*
 * Copyright (C) 2015 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * 
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,OR CONSEQUENTIAL DAMAGES, FOR ANY REASON
 * WHATSOEVER.
 */

/**
 * @file xmc_posif.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief POSIF low level driver API prototype definition for XMC family of microcontrollers <br>
 *
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of POSIF: <br>
 * -- Quadrature Decode (APIs prefixed with XMC_POSIF_QD_) <br>
 * -- Hall Sensor Control (APIs prefixed with XMC_POSIF_HSC_) <br>
 * -- MultiChannel Mode (APIs prefixed with XMC_POSIF_MCM_) <br>
 *
 * History
 *
 * Version 1.0.0 Initial version <br>
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_posif.h>

/* POSIF is not available on XMC1100 and XMC1200 */
#if defined(POSIF0)

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/
 
/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* API to retrieve driver version */
XMC_DRIVER_VERSION_t XMC_POSIF_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = XMC_POSIF_MAJOR_VERSION;
  version.minor = XMC_POSIF_MINOR_VERSION;
  version.patch = XMC_POSIF_PATCH_VERSION;

  return version;
}

/* API to enable the POSIF module */
void XMC_POSIF_Enable(XMC_POSIF_t *const peripheral)
{
  XMC_ASSERT("XMC_POSIF_Enable:Invalid module pointer", XMC_POSIF_CHECK_MODULE_PTR(peripheral));

  if (POSIF0 == peripheral)
  {
    #if (UC_FAMILY == XMC4)
      XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_POSIF0);
      XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_CCU);
      #if ((UC_SERIES == XMC44) || (UC_SERIES == XMC42) || (UC_SERIES == XMC41))
        XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF0);
      #endif
    #elif ((UC_SERIES == XMC13))
      XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF0);
    #endif
  }
  else
  {
    #if (UC_FAMILY == XMC4)
      #if ((UC_SERIES == XMC44))
        XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF1);
      #endif
      #if defined(XMC_SCU_PERIPHERAL_RESET_POSIF1)
        XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_POSIF1);
        XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_CCU);
      #endif
    #endif
  }
}

/* API to disable the POSIF module */
void XMC_POSIF_Disable(XMC_POSIF_t *const peripheral)
{
  XMC_ASSERT("XMC_POSIF_Disable:Invalid module pointer", XMC_POSIF_CHECK_MODULE_PTR(peripheral));

  if (POSIF0 == peripheral)
  {
    #if (UC_FAMILY == XMC4)
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_POSIF0);
    XMC_SCU_CLOCK_DisableClock(XMC_SCU_CLOCK_CCU);
      #if ((UC_SERIES == XMC44) || (UC_SERIES == XMC42) || (UC_SERIES == XMC41))
      XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF0);
      #endif
    #elif ((UC_SERIES == XMC13))
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF0);
    #endif
  }
  else
  {
    #if (UC_FAMILY == XMC4)
      #if ((UC_SERIES == XMC44))
        XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_POSIF1);
      #endif
      #if defined(XMC_SCU_PERIPHERAL_RESET_POSIF1)
        XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_POSIF1);
        XMC_SCU_CLOCK_DisableClock(XMC_SCU_CLOCK_CCU);
      #endif
    #endif
  }
}

/* API to initialize POSIF global resources */
void XMC_POSIF_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_POSIF_Init:Invalid module pointer", XMC_POSIF_CHECK_MODULE_PTR(peripheral));
  XMC_ASSERT("XMC_POSIF_Init:NULL Pointer", (config != (XMC_POSIF_CONFIG_t *)NULL) );

  /* Enable the POSIF module */
  XMC_POSIF_Enable(peripheral);

  /* Stop POSIF */
  XMC_POSIF_Stop(peripheral);

  /* Program the operational mode, input selectors and debounce filter */
  peripheral->PCONF = config->pconf;
}

/* API to initialize hall sensor interface */
XMC_POSIF_STATUS_t XMC_POSIF_HSC_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_HSC_CONFIG_t * const config)
{
  XMC_POSIF_STATUS_t retval;
  
  XMC_ASSERT("XMC_POSIF_HSC_Init:Invalid module pointer\n", XMC_POSIF_CHECK_MODULE_PTR(peripheral));
  XMC_ASSERT("XMC_POSIF_HSC_Init:NULL Pointer\n", (config != (XMC_POSIF_HSC_CONFIG_t *)NULL) );

  if (XMC_POSIF_MODE_HALL_SENSOR == (XMC_POSIF_MODE_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk) )
  {
    peripheral->PCONF |= config->hall_config;
    retval = XMC_POSIF_STATUS_OK;
  }
  else
  {
    retval = XMC_POSIF_STATUS_ERROR;
  }
  return retval;
}

/* API to initialize quadrature decoder interface */
XMC_POSIF_STATUS_t XMC_POSIF_QD_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_QD_CONFIG_t * const config)
{
  uint8_t reg;
  XMC_POSIF_STATUS_t retval;
  
  XMC_ASSERT("XMC_POSIF_QD_Init:Invalid module pointer", XMC_POSIF_CHECK_MODULE_PTR(peripheral));
  XMC_ASSERT("XMC_POSIF_QD_Init:NULL Pointer", (config != (XMC_POSIF_QD_CONFIG_t *)NULL) );

  reg = (uint8_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk);
  if (((uint32_t)XMC_POSIF_MODE_QD == reg) || ((uint32_t)XMC_POSIF_MODE_MCM_QD == reg))
  {
    /* Program the quadrature mode */
    peripheral->PCONF |= (uint32_t)(config->mode) << POSIF_PCONF_QDCM_Pos;
    peripheral->QDC = config->qdc;
    retval = XMC_POSIF_STATUS_OK;
  }
  else
  {
    retval = XMC_POSIF_STATUS_ERROR;
  }
  
  return retval;
}

/* API to initialize multi-channel mode.
 * This is used in Hall mode, standalone multi-channel mode and quadrature with multi-channel mode
 */
XMC_POSIF_STATUS_t XMC_POSIF_MCM_Init(XMC_POSIF_t *const peripheral, const XMC_POSIF_MCM_CONFIG_t * const config)
{
  XMC_POSIF_STATUS_t retval;

  XMC_ASSERT("XMC_POSIF_MCM_Init:Invalid module pointer", XMC_POSIF_CHECK_MODULE_PTR(peripheral));
  XMC_ASSERT("XMC_POSIF_MCM_Init:NULL Pointer", (config != (XMC_POSIF_MCM_CONFIG_t *)NULL) );

  if ((XMC_POSIF_MODE_t)((peripheral->PCONF) & (uint32_t)POSIF_PCONF_FSEL_Msk) != XMC_POSIF_MODE_QD)
  {
    peripheral->PCONF |= config->mcm_config;
    retval = XMC_POSIF_STATUS_OK;
  }
  else
  {
    retval = XMC_POSIF_STATUS_ERROR;
  }
  return retval;
}

/* API to configure input source */
void XMC_POSIF_SelectInputSource (XMC_POSIF_t *const peripheral, const XMC_POSIF_INPUT_PORT_t input0,
    const XMC_POSIF_INPUT_PORT_t input1, const XMC_POSIF_INPUT_PORT_t input2)
{
  uint32_t reg;
  XMC_ASSERT("XMC_POSIF_SelectInputSource:Invalid module pointer", XMC_POSIF_CHECK_MODULE_PTR(peripheral));
  XMC_ASSERT("XMC_POSIF_SelectInputSource:Wrong input port input0", (input0 < XMC_POSIF_INSEL_MAX));
  XMC_ASSERT("XMC_POSIF_SelectInputSource:Wrong input port input1", (input1 < XMC_POSIF_INSEL_MAX));
  XMC_ASSERT("XMC_POSIF_SelectInputSource:Wrong input port input2", (input2 < XMC_POSIF_INSEL_MAX));

  reg = (uint32_t)((((uint32_t)input0 << POSIF_PCONF_INSEL0_Pos) & (uint32_t)POSIF_PCONF_INSEL0_Msk) |
        (((uint32_t)input1 << POSIF_PCONF_INSEL1_Pos) & (uint32_t)POSIF_PCONF_INSEL1_Msk) |
        (((uint32_t)input2 << POSIF_PCONF_INSEL2_Pos) & (uint32_t)POSIF_PCONF_INSEL2_Msk));
  peripheral->PCONF = ((peripheral->PCONF & ~(uint32_t)XMC_POSIF_PCONF_INSEL_Msk) | reg);
}

/* API to select an interrupt node */
void XMC_POSIF_SetInterruptNode(XMC_POSIF_t *const peripheral, const XMC_POSIF_IRQ_EVENT_t event, const XMC_POSIF_SR_ID_t sr)
{
  uint32_t reg;
  
  XMC_ASSERT("XMC_POSIF_SetInterruptNode:Invalid module pointer", XMC_POSIF_CHECK_MODULE_PTR(peripheral));
  XMC_ASSERT("XMC_POSIF_SetInterruptNode:Wrong IRQ event", (event <= XMC_POSIF_IRQ_EVENT_PCLK) );
  XMC_ASSERT("XMC_POSIF_SetInterruptNode:Wrong SR ID", (sr <= XMC_POSIF_SR_ID_1) );
  
  reg = peripheral->PFLGE;
  reg &= ~((uint32_t)1 << ((uint32_t)event + (uint32_t)POSIF_PFLGE_CHESEL_Pos));
  reg |= (uint32_t)sr << ((uint32_t)event + (uint32_t)POSIF_PFLGE_CHESEL_Pos);
  peripheral->PFLGE = reg;
}
#endif /* #if defined(POSIF0) */
