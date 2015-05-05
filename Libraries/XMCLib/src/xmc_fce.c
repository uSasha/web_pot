
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
 * @file xmc4_fce.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief FCE driver for XMC microcontroller family <br>
 *
 *
 * History <br>
 *
 * Version 1.0.0 Initial Version <br>
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/
 
#include <xmc_fce.h>

#if defined (FCE)

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

/* API to retrieve driver version */
XMC_DRIVER_VERSION_t XMC_FCE_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = XMC_FCE_MAJOR_VERSION;
  version.minor = XMC_FCE_MINOR_VERSION;
  version.patch = XMC_FCE_PATCH_VERSION;

  return version;
}

/*
 * Initialize the FCE peripheral:
 * Update FCE configuration and initialize seed value
 */
XMC_FCE_STATUS_t XMC_FCE_Init(const XMC_FCE_t *const engine)
{
  engine->kernel_ptr->CFG = engine->fce_cfg_update.regval;
  engine->kernel_ptr->CRC = engine->seedvalue;
  
  return XMC_FCE_STATUS_OK;
}

/* Disable FCE */
void XMC_FCE_Disable(void)
{
  FCE->CLC |= (uint32_t)FCE_CLC_DISR_Msk;
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_FCE);
}

/* Enable FCE */
void XMC_FCE_Enable(void)
{
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_FCE);
  FCE->CLC &= (uint32_t)~FCE_CLC_DISR_Msk;
}

/* Calculate and return the SAE J1850 CRC8 checksum */
XMC_FCE_STATUS_t XMC_FCE_CalculateCRC8(const XMC_FCE_t *const engine,
                                       const uint8_t *data,
									   uint32_t length,
									   uint8_t *result)
{
  XMC_FCE_STATUS_t status = XMC_FCE_STATUS_OK;
	
  XMC_ASSERT("XMC_FCE_CalculateCRC8: Wrong FCE kernel used", (engine -> kernel_ptr == XMC_FCE_CRC8));

  if (length == 0UL)
  {
    status = XMC_FCE_STATUS_ERROR;
  }
  else
  {
    while (0UL != length)
    {
      engine->kernel_ptr->IR = *data;
      data++;
      length -= 1U;
    }

    *result = (uint8_t)engine->kernel_ptr->CRC;
  }

  return status;
}

/* Calculate and return calculated CCITT CRC16 checksum */
XMC_FCE_STATUS_t XMC_FCE_CalculateCRC16(const XMC_FCE_t *const engine,
                                        const uint16_t *data,
										uint32_t length,
										uint16_t *result)
{
  XMC_FCE_STATUS_t status = XMC_FCE_STATUS_OK;
	
  XMC_ASSERT("XMC_FCE_CalculateCRC16: Wrong FCE kernel used", (engine -> kernel_ptr == XMC_FCE_CRC16));
  XMC_ASSERT("XMC_FCE_CalculateCRC16: Length field is empty", (length != 0));
  XMC_ASSERT("XMC_FCE_CalculateCRC16: Length is not aligned", ((length & 0x01) == 0));
  XMC_ASSERT("XMC_FCE_CalculateCRC16: Buffer is not aligned", (((uint32_t)data % 2U) == 0));

  /* Check if data and length are word aligned */
  if (((length & 0x01U) != 0U) || (((uint32_t)length % 2U) != 0U))
  {
    status = XMC_FCE_STATUS_ERROR;
  }
  else
  {
    while (0UL != length)
    {
      engine->kernel_ptr->IR = *data;
      data++;
      length -= 2U;
    }
	
    *result = (uint16_t)engine->kernel_ptr->CRC;
  }

  return status;
}

/* Calculate and return the IEEE 802.3 Ethernet CRC32 checksum */
XMC_FCE_STATUS_t XMC_FCE_CalculateCRC32(const XMC_FCE_t *const engine,
                                        const uint32_t *data,
										uint32_t length,
										uint32_t *result)
{
  XMC_FCE_STATUS_t status = XMC_FCE_STATUS_OK;
	
  XMC_ASSERT("XMC_FCE_CalculateCRC32: Wrong FCE kernel used", ((engine->kernel_ptr == XMC_FCE_CRC32_0) ||
                                                               (engine->kernel_ptr == XMC_FCE_CRC32_1)));
  XMC_ASSERT("XMC_FCE_CalculateCRC32: Length field is empty", (length != 0));
  XMC_ASSERT("XMC_FCE_CalculateCRC32: Length is not aligned", ((length & 0x03) == 0));
  XMC_ASSERT("XMC_FCE_CalculateCRC32: Buffer is not aligned", (((uint32_t)data % 4U) == 0));

  /* Check if data and length are word aligned */
  if (((length & 0x03U) != 0U) || (((uint32_t)length % 4U) != 0U))
  {
    status = XMC_FCE_STATUS_ERROR;
  }
  else
  {
    while (0UL != length)
    {
      engine->kernel_ptr->IR = *data;
      data++;
      length -= 4U;
    }

    *result = engine->kernel_ptr->CRC;
  }
  
  return status;
}

/* Trigger mismatch in the CRC registers */
void XMC_FCE_TriggerMismatch(const XMC_FCE_t *const engine, XMC_FCE_CTR_TEST_t test)
{
  /* Create a 0 to 1 transition and clear to 0 once it is done */
  engine->kernel_ptr->CTR &= ~((uint32_t)test);
  engine->kernel_ptr->CTR |= (uint32_t)test;
  engine->kernel_ptr->CTR &= ~((uint32_t)test);
}

/* Change endianness of 16-bit input buffer */
void CRC_LittleEndian16bit(uint8_t* inbuffer, uint16_t* outbuffer, uint16_t length)
{
  uint16_t counter = 0U;
  uint16_t bytecounter = 0U;

  if ((length & 0x01U)  == 0)
  {
    for (counter = 0U; counter < (length >> 1); counter++)
    {
      outbuffer[counter] = 0U;
    }

    outbuffer[counter] = 0U;
    counter = 0U;

    while (length)
    {
      outbuffer[counter] = ((uint16_t)((uint16_t)inbuffer[bytecounter] << 8U) |
                            (inbuffer[bytecounter + 1U]));
      counter += 1U;
      bytecounter += 2U;
      length -= 2U;
    }
  }
}

/* Change endianness of 32-bit input buffer */
void CRC_LittleEndian32bit(uint8_t* inbuffer, uint32_t* outbuffer, uint16_t length)
{
  uint16_t counter = 0U;
  uint16_t bytecounter = 0U;
 
  if ((length & 0x03U) == 0)
  {
    for (counter = 0U; counter < (length >> 2U); counter++)
    {
      outbuffer[counter] = 0U;
    }

    outbuffer[counter] = 0U;
    counter = 0U;

    while (length)
    {
      outbuffer[counter] = ((uint32_t)inbuffer[bytecounter]      << 24U) |
                           ((uint32_t)inbuffer[bytecounter + 1U] << 16U) |
                           ((uint32_t)inbuffer[bytecounter + 2U] <<  8U) |
                           (inbuffer[bytecounter + 3U]);
      counter += 1U;
      bytecounter += 4U;
      length -= 4U;
    }
  }
}

#endif
