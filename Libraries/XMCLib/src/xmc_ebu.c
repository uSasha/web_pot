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
 */
 
/**
 * @file xmc_ebu.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief EBU driver for XMC microcontroller family <br> 
 *
 * History <br>
 *
 * Version 1.0.0  Initial <br>
 */
 
 
/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
 
#include <xmc_ebu.h>

#if defined(EBU)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

/*
 * Retrieves the version of the EBU driver
 */
XMC_DRIVER_VERSION_t XMC_EBU_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = XMC_EBU_MAJOR_VERSION;
  version.minor = XMC_EBU_MINOR_VERSION;
  version.patch = XMC_EBU_PATCH_VERSION;

  return version;
}

/*
 * Initialize the EBU peripheral
 */
XMC_EBU_STATUS_t XMC_EBU_Init(XMC_EBU_t *const ebu,const XMC_EBU_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_EBU_Init: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  XMC_ASSERT("XMC_EBU_Init:Null Pointer", (config != (XMC_EBU_CONFIG_t *)NULL));
  
  /* Enable EBU */
  XMC_EBU_Enable(ebu);

  /* Clock configuration */
  ebu->CLC  =  config->ebu_clk_config.raw0;

  /*EBU Mode Configuration */
  ebu->MODCON =   config->ebu_mode_config.raw0;

  /* Address Bits available for GPIO function */
  ebu->USERCON = config->ebu_free_pins_to_gpio.raw0;

  return XMC_EBU_STATUS_OK;
}

/*
 * Configures the SDRAM with operating modes and refresh parameters
 */
void XMC_EBU_ConfigureSdram(XMC_EBU_t *const ebu,const XMC_EBU_SDRAM_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_EBU_Init: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  XMC_ASSERT("XMC_EBU_Init:Null Pointer", (config != (XMC_EBU_SDRAM_CONFIG_t *)NULL));
  
  /* EBU SDRAM Refresh Configuration Parameters */
  ebu->SDRMREF = config->raw2;
  /* EBU SDRAM General Configuration Parameters */
  ebu->SDRMCON = config->raw0;
  /* EBU SDRAM Operation Mode Configuration Parameters */
  ebu->SDRMOD = config->raw1;
}

/*
 * Configures the SDRAM region for read and write operation
 */
void XMC_EBU_ConfigureRegion(XMC_EBU_t *const ebu,const XMC_EBU_REGION_t *const region)
{
  
  XMC_ASSERT("XMC_EBU_Init: Invalid module pointer", XMC_EBU_CHECK_MODULE_PTR(ebu));
  XMC_ASSERT("XMC_EBU_Init:Null Pointer", (region != (XMC_EBU_REGION_t *)NULL));
  
  /* Read configuration of the region*/
  ebu->BUS[region->read_config.ebu_region_no].RDCON = region->read_config.ebu_bus_read_config.raw0;

  /* Read parameters of the region*/
  ebu->BUS[region->read_config.ebu_region_no].RDAPR = region->read_config.ebu_bus_read_config.raw1;

  /* Write configuration of the region*/
  ebu->BUS[region->write_config.ebu_region_no].WRCON = region->write_config.ebu_bus_write_config.raw0;

  /* Write parameters of the region*/
  ebu->BUS[region->write_config.ebu_region_no].WRAPR = region->write_config.ebu_bus_write_config.raw1;
}


#endif /* defined(EBU) */
