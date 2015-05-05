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
 * @file xmc4_gpio.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief GPIO low level driver API prototype definition for XMC4 family of microcontrollers. 
 *
 * History <br>
 *
 * Version 1.0.0  Initial version <br>
 */

#include "xmc_gpio.h"

#if UC_FAMILY == XMC4

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define PORT_PDR_Msk              PORT0_PDR0_PD0_Msk
#define PORT_PDR_Size             (4U)
#define PORT_HWSEL_Msk            PORT0_HWSEL_HW0_Msk

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

void XMC_GPIO_Init(XMC_GPIO_PORT_t *const port, const uint8_t pin, const XMC_GPIO_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_GPIO_Init: Invalid port", XMC_GPIO_CHECK_PORT(port));
  XMC_ASSERT("XMC_GPIO_Init: Invalid mode", XMC_GPIO_CHECK_MODE(config->mode));
  XMC_ASSERT("XMC_GPIO_Init: Invalid output level", XMC_GPIO_CHECK_OUTPUT_LEVEL(config->output_level));
  XMC_ASSERT("XMC_GPIO_Init: Invalid output strength", XMC_GPIO_CHECK_OUTPUT_STRENGTH(config->output_strength));

  /* Switch to input */
  port->IOCR[pin >> 2U] &= (uint32_t)~(PORT_IOCR_PC_Msk << (PORT_IOCR_PC_Size * (pin & 0x3U)));

  /* HW port control is disabled */
  port->HWSEL &= ~(uint32_t)((uint32_t)PORT_HWSEL_Msk << ((uint32_t)pin << 1U));


  /* Enable digital input */
  if (XMC_GPIO_CHECK_ANALOG_PORT(port))
  {
    port->PDISC &= ~(uint32_t)((uint32_t)0x1U << pin);
  }
  else
  {
    /* Set output level */
    port->OMR = (uint32_t)config->output_level << pin;

    /* Set output driver strength */
    port->PDR[pin >> 3U] &= (uint32_t)~(PORT_PDR_Msk << ((uint32_t)PORT_PDR_Size * ((uint32_t)pin & 0x7U)));
    port->PDR[pin >> 3U] |= (uint32_t)config->output_strength << ((uint32_t)PORT_PDR_Size * ((uint32_t)pin & 0x7U));
  }

  /* Set mode */
  port->IOCR[pin >> 2U] |= (uint32_t)config->mode << ((uint32_t)PORT_IOCR_PC_Size * ((uint32_t)pin & 0x3U));
}

void XMC_GPIO_SetOutputStrength(XMC_GPIO_PORT_t *const port, const uint8_t pin, XMC_GPIO_OUTPUT_STRENGTH_t strength)
{
  XMC_ASSERT("XMC_GPIO_Init: Invalid port", XMC_GPIO_CHECK_OUTPUT_PORT(port));
  XMC_ASSERT("XMC_GPIO_Init: Invalid output strength", XMC_GPIO_CHECK_OUTPUT_STRENGTH(strength));

  port->PDR[pin >> 3U] &= (uint32_t)~((uint32_t)PORT_PDR_Msk << ((uint32_t)PORT_PDR_Size * ((uint32_t)pin & 0x7U)));
  port->PDR[pin >> 3U] |= (uint32_t)strength << ((uint32_t)PORT_PDR_Size * ((uint32_t)pin & 0x7U));
}

#endif /* UC_FAMILY == XMC4 */
