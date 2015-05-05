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
 * @file xmc_dac.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief DAC low level driver API prototype definition for XMC4 family of microcontrollers <br>
 *
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of DAC: <br>
 * -- Pattern generator (APIs suffixed with Pattern keyword) <br>
 * -- Noise generator (APIs suffixed with Noise keyword) <br>
 * -- Ramp generator(APIs suffixed with Ramp keyword) <br>
 * -- Single value mode(APIs suffixed with SingleValue keyword) <br>
 * -- Data processing mode(APIs suffixed with Data keyword) <br>
 *
 * History <br>
 *
 * Version 1.0.0  Initial version <br>
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include <xmc_dac.h>
#include <xmc_scu.h>

/* DAC peripheral is not available on XMC1X devices. */
#if defined(DAC)

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define XMC_DAC_MIN_FREQ_DIVIDER (16U)
#define XMC_DAC_MAX_FREQ_DIVIDER (1048576U)
#define XMC_DAC_DAC0PATL_PAT_BITSIZE (5U)

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

 /* API to retrieve driver version */
XMC_DRIVER_VERSION_t XMC_DAC_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = (uint8_t)XMC_DAC_MAJOR_VERSION;
  version.minor = (uint8_t)XMC_DAC_MINOR_VERSION;
  version.patch = (uint8_t)XMC_DAC_PATCH_VERSION;

  return version;
}

/* API to enable the DAC module */
void XMC_DAC_Enable(XMC_DAC_t *const dac)
{
  XMC_UNUSED_ARG(dac);
  
#if(UC_SERIES != XMC45)
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_DAC);
#endif
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_DAC);
}

/* API to disable the DAC module */
void XMC_DAC_Disable(XMC_DAC_t *const dac)
{
  XMC_UNUSED_ARG(dac);

  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_DAC);
#if(UC_SERIES != XMC45)
  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_DAC);
#endif
}

/* API to check whether DAC is enabled */
bool XMC_DAC_IsEnabled(const XMC_DAC_t *const dac)
{
  bool status;

  XMC_UNUSED_ARG(dac);

  status = XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_DAC);
  if(status == true)
  {
    status = false;
  }
  else
  {
    status = true;
  }
  return (status);
}

/* API to initialize DAC channel configuration */
void XMC_DAC_CH_Init(XMC_DAC_t *const dac, const uint8_t channel, const XMC_DAC_CH_CONFIG_t *const config)
{
  XMC_DAC_Enable(dac);

  dac->DACCFG[channel].low = config->cfg0;
  dac->DACCFG[channel].high = config->cfg1;
  if (channel < XMC_DAC_NO_CHANNELS)
  {
    XMC_DAC_CH_EnableOutput(dac, channel);
  }
}

/* API to set the waveform frequency except in Ramp and Pattern generation mode. */
XMC_DAC_CH_STATUS_t XMC_DAC_CH_SetFrequency(XMC_DAC_t *const dac,
                                            const uint8_t channel,
                                            const uint32_t frequency)
{
  uint32_t divider;
  XMC_DAC_CH_STATUS_t status;

  XMC_ASSERT("XMC_DAC_CH_SetFrequency: frequency must be greater than zero", frequency > 0U);

  divider = XMC_SCU_CLOCK_GetPeripheralClockFrequency() / frequency;

  if (divider < XMC_DAC_MIN_FREQ_DIVIDER)
  {
    status = XMC_DAC_CH_STATUS_ERROR_FREQ2HIGH;
  }
  else if (divider >= XMC_DAC_MAX_FREQ_DIVIDER)
  {
    status = XMC_DAC_CH_STATUS_ERROR_FREQ2LOW;
  }
  else {
    dac->DACCFG[channel].low = (dac->DACCFG[channel].low & (uint32_t)(~DAC_DAC0CFG0_FREQ_Msk)) |
                               (divider << DAC_DAC0CFG0_FREQ_Pos);
    status = XMC_DAC_CH_STATUS_OK;
  }

  return status;
}

/* API to set the waveform frequency in Ramp Mode. */
XMC_DAC_CH_STATUS_t XMC_DAC_CH_SetRampFrequency(XMC_DAC_t *const dac,
                                                const uint8_t channel,
                                                const uint32_t frequency)
{
  uint32_t stop;
  uint32_t start;

  start = dac->DACDATA[channel];
  stop = (dac->DAC01DATA >> (channel * DAC_DAC01DATA_DATA1_Pos)) & (uint32_t)DAC_DAC01DATA_DATA0_Msk;

  return XMC_DAC_CH_SetFrequency(dac, channel, frequency * ((stop - start) + 1U));
}

/* API to start the operation in Single Value Mode. */
XMC_DAC_CH_STATUS_t XMC_DAC_CH_StartSingleValueMode(XMC_DAC_t *const dac, const uint8_t channel)
{
  XMC_ASSERT("XMC_DAC_CH_StartSingleValueMode: dac parameter not valid\n", XMC_DAC_IS_DAC_VALID(dac));
  XMC_ASSERT("XMC_DAC_CH_StartSingleValueMode: channel parameter not valid\n", XMC_DAC_IS_CHANNEL_VALID(channel));
  XMC_ASSERT("XMC_DAC_CH_StartSingleValueMode: dac module not enabled\n", XMC_DAC_IsEnabled(dac));

  XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_SINGLE);

  return XMC_DAC_CH_STATUS_OK;
}

/* API to start the operation in Data Mode. */
XMC_DAC_CH_STATUS_t XMC_DAC_CH_StartDataMode(XMC_DAC_t *const dac,
                                             const uint8_t channel,
                                             const XMC_DAC_CH_TRIGGER_t trigger,
                                             const uint32_t frequency)
{
  XMC_DAC_CH_STATUS_t status = XMC_DAC_CH_STATUS_OK;

  XMC_ASSERT("XMC_DAC_CH_StartDataMode: dac parameter not valid\n", XMC_DAC_IS_DAC_VALID(dac));
  XMC_ASSERT("XMC_DAC_CH_StartDataMode: channel parameter not valid\n", XMC_DAC_IS_CHANNEL_VALID(channel));
  XMC_ASSERT("XMC_DAC_CH_StartDataMode: dac module not enabled\n", XMC_DAC_IsEnabled(dac));

  XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_IDLE);

  if (trigger == XMC_DAC_CH_TRIGGER_INTERNAL)
  {
    status = XMC_DAC_CH_SetFrequency(dac, channel, frequency);
  }

  if (status == XMC_DAC_CH_STATUS_OK)
  {
    XMC_DAC_CH_SetTrigger(dac, channel, trigger);
    XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_DATA);
  }

  return status;
}

/* API to start the operation in Ramp Mode. */
XMC_DAC_CH_STATUS_t XMC_DAC_CH_StartRampMode(XMC_DAC_t *const dac,
                                             const uint8_t channel,
                                             const uint16_t start,
                                             const uint16_t stop,
                                             const XMC_DAC_CH_TRIGGER_t trigger,
                                             const uint32_t frequency)
{
  XMC_DAC_CH_STATUS_t status = XMC_DAC_CH_STATUS_OK;

  XMC_ASSERT("XMC_DAC_CH_StartRampMode: dac parameter not valid\n", XMC_DAC_IS_DAC_VALID(dac));
  XMC_ASSERT("XMC_DAC_CH_StartRampMode: channel parameter not valid\n", XMC_DAC_IS_CHANNEL_VALID(channel));
  XMC_ASSERT("XMC_DAC_CH_StartRampMode: dac module not enabled\n", XMC_DAC_IsEnabled(dac));

  XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_IDLE);

  XMC_DAC_CH_SetRampStart(dac, channel, start);
  XMC_DAC_CH_SetRampStop(dac, channel, stop);
  XMC_DAC_CH_SetTrigger(dac, channel, trigger);

  if (trigger == XMC_DAC_CH_TRIGGER_INTERNAL)
  {
    status = XMC_DAC_CH_SetRampFrequency(dac, channel, frequency);
  }

  if (status == XMC_DAC_CH_STATUS_OK)
  {
    XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_RAMP);
  }

  return status;
}

/* API to start the operation in Pattern Mode. */
XMC_DAC_CH_STATUS_t XMC_DAC_CH_StartPatternMode(XMC_DAC_t *const dac,
                                                const uint8_t channel,
                                                const uint8_t *const pattern,
                                                const XMC_DAC_CH_PATTERN_SIGN_OUTPUT_t sign_output,
                                                const XMC_DAC_CH_TRIGGER_t trigger,
                                                const uint32_t frequency)
{
  XMC_DAC_CH_STATUS_t status = XMC_DAC_CH_STATUS_OK;

  XMC_ASSERT("XMC_DAC_CH_StartPatternMode: dac parameter not valid\n", XMC_DAC_IS_DAC_VALID(dac));
  XMC_ASSERT("XMC_DAC_CH_StartPatternMode: channel parameter not valid\n", XMC_DAC_IS_CHANNEL_VALID(channel));
  XMC_ASSERT("XMC_DAC_CH_StartPatternMode: dac module not enabled\n", XMC_DAC_IsEnabled(dac));

  XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_IDLE);

  if (trigger == XMC_DAC_CH_TRIGGER_INTERNAL)
  {
    status = XMC_DAC_CH_SetFrequency(dac, channel, frequency * XMC_DAC_SAMPLES_PER_PERIOD);
  }

  if (status == XMC_DAC_CH_STATUS_OK)
  {
    XMC_DAC_CH_SetPattern(dac, channel, pattern);
    if (XMC_DAC_CH_PATTERN_SIGN_OUTPUT_ENABLED == sign_output)
    {
      XMC_DAC_CH_EnablePatternSignOutput(dac, channel);
    }
    else
    {
      XMC_DAC_CH_DisablePatternSignOutput(dac, channel);
    }
    XMC_DAC_CH_SetTrigger(dac, channel, trigger);
    XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_PATTERN);
  }

  return status;
}

/* API to start the operation in Noise Mode. */
XMC_DAC_CH_STATUS_t XMC_DAC_CH_StartNoiseMode(XMC_DAC_t *const dac,
                                              const uint8_t channel,
                                              const XMC_DAC_CH_TRIGGER_t trigger,
                                              const uint32_t frequency)
{
  XMC_DAC_CH_STATUS_t status = XMC_DAC_CH_STATUS_OK;

  XMC_ASSERT("XMC_DAC_CH_StartNoiseMode: dac parameter not valid\n", XMC_DAC_IS_DAC_VALID(dac));
  XMC_ASSERT("XMC_DAC_CH_StartNoiseMode: channel parameter not valid\n", XMC_DAC_IS_CHANNEL_VALID(channel));
  XMC_ASSERT("XMC_DAC_CH_StartNoiseMode: dac module not enabled\n", XMC_DAC_IsEnabled(dac));

  XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_IDLE);

  if (trigger == XMC_DAC_CH_TRIGGER_INTERNAL)
  {
    status = XMC_DAC_CH_SetFrequency(dac, channel, frequency);
  }

  if (status == XMC_DAC_CH_STATUS_OK)
  {
    XMC_DAC_CH_SetTrigger(dac, channel, trigger);
    XMC_DAC_CH_SetMode(dac, channel, XMC_DAC_CH_MODE_NOISE);
  }

  return status;
}

/* API to write the pattern data table. */
void XMC_DAC_CH_SetPattern(XMC_DAC_t *const dac, uint8_t channel, const uint8_t *const data)
{
  uint32_t index;
  uint32_t temp;

  XMC_ASSERT("XMC_DAC_CH_SetPattern: dac parameter not valid\n", XMC_DAC_IS_DAC_VALID(dac));
  XMC_ASSERT("XMC_DAC_CH_SetPattern: channel parameter not valid\n", XMC_DAC_IS_CHANNEL_VALID(channel));
  XMC_ASSERT("XMC_DAC_CH_SetPattern: dac module not enabled\n", XMC_DAC_IsEnabled(dac));

  temp = data[0U];
  for(index = 1U; index < 6U; ++index)
  {
    temp |= (uint32_t)data[index] << (index * XMC_DAC_DAC0PATL_PAT_BITSIZE);
  }
  dac->DACPAT[channel].low = temp;

  temp = data[6U];
  for(index = 1U; index < 6U; ++index)
  {
    temp |= (uint32_t)data[index + 6U] << (index * XMC_DAC_DAC0PATL_PAT_BITSIZE);
  }
  dac->DACPAT[channel].high = temp;
}

#endif /* defined(DAC) */
