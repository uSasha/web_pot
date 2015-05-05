/*
 * Copyright (C) 2015 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,OR CONSEQUENTIAL DAMAGES, FOR ANY REASON
 * WHATSOEVER.
 */

/**
 * @file xmc_hrpwm.c
 * @date 16 Feb, 2015
 * @version 1.0.2
 *
 * @brief HRPWM low level driver API prototype definition for XMC family of microcontrollers <br>
 *
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of HRPWM: <br>
 * -- HRPWM (APIs prefixed with XMC_HRPWM_HRC) <br>
 * -- CSG (APIS prefixed with XMC_HRPWM_CSG) <br>
 * History
 *
 * Version 1.0.0 Initial version<br>
 * Version 1.0.2 Variable g_hrpwm_char_data[] defined in startup file is used in place of trim data macro<br>            
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include <xmc_hrpwm.h>
#include <xmc_scu.h>

#if defined(HRPWM0)
/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
/* 60MHz is considered as the minimum range for 80MHz HRC operation */
#define XMC_HRPWM_MIN_VALUE_FOR_80MHZ_HRC_OPERATION  60000000U

/* 100MHz is considered as the maximum range for 80MHz HRC operation */
#define XMC_HRPWM_MAX_VALUE_FOR_80MHZ_HRC_OPERATION  100000000U

/* 150MHz is considered as the maximum range for 120MHz HRC operation */
#define XMC_HRPWM_MAX_VALUE_FOR_120MHZ_HRC_OPERATION 150000000U

/* 200MHz is considered as the maximum range for 180MHz HRC operation */
#define XMC_HRPWM_MAX_VALUE_FOR_180MHZ_HRC_OPERATION 200000000U

#if (UC_SERIES == XMC44)
#define XMC_HRPWM_DELAY_CNT (28800U)  /* Provides ~2.8 msec delay @ 220MHz frequency */

#elif (UC_SERIES == XMC42)
#define XMC_HRPWM_DELAY_CNT (19200U)  /* Provides ~2.8 msec delay @ 150MHz frequency */

#else
#define XMC_HRPWM_DELAY_CNT (36000U)  /* Provides ~5.3 msec delay @ 150MHz frequency */
#endif

/***********************************************************************************************************************
 * API IMPLEMENTATION - GENERAL
 **********************************************************************************************************************/
/* API to retrieve driver version info */
XMC_DRIVER_VERSION_t XMC_HRPWM_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = XMC_HRPWM_MAJOR_VERSION;
  version.minor = XMC_HRPWM_MINOR_VERSION;
  version.patch = XMC_HRPWM_PATCH_VERSION;

  return version;
}

/* Delay */
void XMC_HRPWM_lDelay(void)
{
  volatile uint32_t i;

  for (i = 0U; i <= XMC_HRPWM_DELAY_CNT; i++)  /* Wait more than 2 microseconds */
  {
    ; /* For MISRA */
  }
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM GLOBAL
 **********************************************************************************************************************/
/* Initializes HRPWM global registers */
XMC_HRPWM_STATUS_t XMC_HRPWM_Init(XMC_HRPWM_t *const hrpwm)
{
  uint32_t *csg_memory;
  uint32_t ccu_clock;
  uint32_t clkc;
  XMC_HRPWM_STATUS_t status;

  XMC_ASSERT("XMC_HRPWM_Init:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  status = XMC_HRPWM_STATUS_ERROR;

  /* Apply reset to HRPWM module */ 
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_HRPWM0);
 
  /* Release reset for HRPWM module */
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_HRPWM0);

  /* Ungate clock */
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_HRPWM0);

  hrpwm->GLBANA = 0x00004A4E;  /* Initialization sequence */

  hrpwm->HRBSC |= HRPWM0_HRBSC_HRBE_Msk;  /* Enable Bias Generator of HRPWM */
  
  /* Update CSG0 memory data */
  csg_memory = (uint32_t *)XMC_HRPWM_CSG0_MEMORY_ADDRESS;
  *csg_memory = g_hrpwm_char_data[0];
  /* write csg memory bits[14:11] with 0b1100 */
  *csg_memory &= ~(0xFFFF87FF);
  *csg_memory |= 0x00006000;

  /* Update CSG1 trimming data */
  csg_memory = (uint32_t *)XMC_HRPWM_CSG1_MEMORY_ADDRESS;
  *csg_memory = g_hrpwm_char_data[1];
  /* write csg memory bits[14:11] with 0b1100 */
  *csg_memory &= ~(0xFFFF87FF);
  *csg_memory |= 0x00006000;

  /* Update CSG2 trimming data */
  csg_memory = (uint32_t *)XMC_HRPWM_CSG2_MEMORY_ADDRESS;
  *csg_memory = g_hrpwm_char_data[2];
  /* write csg memory bits[14:11] with 0b1100 */
  *csg_memory &= ~(0xFFFF87FF);
  *csg_memory |= 0x00006000;
  
  /* Set CSG units to high speed mode */
  hrpwm->CSGCFG = 0x0000003F;
  
  /* Read CCU clock frequency */
  ccu_clock = XMC_SCU_CLOCK_GetCcuClockFrequency();

  if ((ccu_clock > XMC_HRPWM_MIN_VALUE_FOR_80MHZ_HRC_OPERATION) && (ccu_clock <= XMC_HRPWM_MAX_VALUE_FOR_80MHZ_HRC_OPERATION))
  {
    clkc = 3U; /* Clock frequency range 60MHz+ - 100MHz is considered as 80MHz HRC operation */
  }
  else if ((ccu_clock > XMC_HRPWM_MAX_VALUE_FOR_80MHZ_HRC_OPERATION) && (ccu_clock <= XMC_HRPWM_MAX_VALUE_FOR_120MHZ_HRC_OPERATION))
  {
    clkc = 2U; /* Clock frequency range 100MHz+ - 150MHz is considered as 120MHz HRC operation */
  }
  else if ((ccu_clock > XMC_HRPWM_MAX_VALUE_FOR_120MHZ_HRC_OPERATION) && (ccu_clock <= XMC_HRPWM_MAX_VALUE_FOR_180MHZ_HRC_OPERATION))
  {
    clkc = 1U; /* Clock frequency range 150MHz+ - 200MHz is considered as 180MHz HRC operation */
  }
  else
  {
    clkc = 0U; /* Invalid frequency for HRC operation: Clock frequency <= 60MHz & Clock frequency > 200MHz */
  }

  if (clkc != 0U) /* Enter the loop only if the clock frequency is valid */
  {
    /* Program HRC clock configuration with clock frequency information */
    hrpwm->HRCCFG |= (clkc << HRPWM0_HRCCFG_CLKC_Pos);
    
    hrpwm->HRCCFG |= HRPWM0_HRCCFG_HRCPM_Msk; /* Release HR generation from power down mode */
    
    XMC_HRPWM_lDelay(); /* As per Initialization sequence */
    
    /* Enable global high resolution generation / Force charge pump down */
    hrpwm->GLBANA |= HRPWM0_GLBANA_GHREN_Msk;
    
    XMC_HRPWM_lDelay();

    /* Check High resolution ready bit field */
    if ((hrpwm->HRGHRS & HRPWM0_HRGHRS_HRGR_Msk) == 1U)
    {
      /* High resolution logic unit is ready */
      status = XMC_HRPWM_STATUS_OK;
    }
  }
  else
  {
    status = XMC_HRPWM_STATUS_ERROR; /* Clock frequency is invalid */
  }
 
  return (status);
}

/* Enable global high resolution generation */
void XMC_HRPWM_EnableGlobalHR(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_EnableGlobalHR:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  /* Enable global high resolution generation / Force charge pump down */
  hrpwm->GLBANA |= HRPWM0_GLBANA_GHREN_Msk;

  XMC_HRPWM_lDelay(); /* Elapse startup time */
}

/* Disable global high resolution generation */
void XMC_HRPWM_DisableGlobalHR(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_DisableGlobalHR:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  /* Enable global high resolution generation / Force charge pump down */
  hrpwm->GLBANA &= ~(HRPWM0_GLBANA_GHREN_Msk);
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM HRC GLOBAL
 **********************************************************************************************************************/
/* Checks and returns high resolution generation working status */
XMC_HRPWM_HR_LOGIC_t XMC_HRPWM_GetHRGenReadyStatus(XMC_HRPWM_t *const hrpwm)
{
  XMC_HRPWM_HR_LOGIC_t status;

  XMC_ASSERT("XMC_HRPWM_GetHRGenReadyStatus:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  if (hrpwm->HRGHRS)
  {
    status = XMC_HRPWM_HR_LOGIC_WORKING;
  }
  else
  {
    status = XMC_HRPWM_HR_LOGIC_NOT_WORKING;
  }
  return status;
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM HRC CHANNEL
 **********************************************************************************************************************/
/* Initialize HRPWM HRC channel */
void XMC_HRPWM_HRC_Init(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_CONFIG_t *const config)
{ 
  XMC_ASSERT("XMC_HRPWM_HRC_Init:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  
  /* Setting of HRCy mode configuration */
  hrc->GC = config->gc;
  
  /* Passive level configuration */
  hrc->PL = config->psl;
}

/* Configure Source selector 0 */
void XMC_HRPWM_HRC_ConfigSourceSelect0(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_SRC_CONFIG_t *const config)
{
  uint32_t reg;
  reg = 0;

  XMC_ASSERT("XMC_HRPWM_HRC_ConfigSourceSelect0:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  
  /* HRC mode config for source selector 0 */
  hrc->GC &= ~(HRPWM0_HRC_GC_HRM0_Msk);
  hrc->GC |= config->high_res_mode << HRPWM0_HRC_GC_HRM0_Pos;

  /*****************************************************************************
  * HRCy global control selection (HRCyGSEL)
  ****************************************************************************/
  if(config->set_config == XMC_HRPWM_HRC_SRC_INPUT_CSG)
  {
    reg = config->cmp_set;
    reg |= XMC_HRPWM_HRC_SRC_INPUT_CSG << HRPWM0_HRC_GSEL_S0M_Pos;  /* comparator output controlled the set config */
  }
  else
  {
    reg &= ~(HRPWM0_HRC_GSEL_S0M_Msk);	/* CCU Timer controlled the set config */
  }

  if (config->clear_config == XMC_HRPWM_HRC_SRC_INPUT_CSG)
  {
    reg |= config->cmp_clear << HRPWM0_HRC_GSEL_C0CS_Pos;
    reg |= XMC_HRPWM_HRC_SRC_INPUT_CSG << HRPWM0_HRC_GSEL_C0M_Pos;  /* comparator output controlled the clear config */
  }
  else
  {
    reg &= ~(HRPWM0_HRC_GSEL_C0M_Msk);	/* CCU Timer controlled the clear config */
  }

  reg |= config->set_edge_config << HRPWM0_HRC_GSEL_S0ES_Pos;
  reg |= config->clear_edge_config << HRPWM0_HRC_GSEL_C0ES_Pos;
  hrc->GSEL &= 0xFFFF0000;
  hrc->GSEL |= reg;

  reg = 0;
  reg = config->timer_sel;
  reg |= config->src_trap_enable << HRPWM0_HRC_TSEL_TS0E_Pos;
  hrc->TSEL &= 0xFFFEFFF8;
  hrc->TSEL |= reg;
}

/* Configure Source selector 1 */
void XMC_HRPWM_HRC_ConfigSourceSelect1(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_SRC_CONFIG_t *const config)
{
  uint32_t reg;
  reg = 0;

  XMC_ASSERT("XMC_HRPWM_HRC_ConfigSourceSelect1:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  
  /* HRC mode config for source selector 1 */
  hrc->GC &= ~(HRPWM0_HRC_GC_HRM1_Msk);
  hrc->GC |= config->high_res_mode << HRPWM0_HRC_GC_HRM1_Pos;

  /*****************************************************************************
  * HRCy global control selection (HRCyGSEL)
  ****************************************************************************/

  if(config->set_config == XMC_HRPWM_HRC_SRC_INPUT_CSG)
  {
    reg = config->cmp_set << HRPWM0_HRC_GSEL_C1SS_Pos;
    reg |= XMC_HRPWM_HRC_SRC_INPUT_CSG << HRPWM0_HRC_GSEL_S1M_Pos;  /* comparator output controlled the set config*/
  }
  else
  {
    reg &= ~(HRPWM0_HRC_GSEL_S1M_Msk);	/* CCU Timer controlled the set config */
  }

  if (config->clear_config == XMC_HRPWM_HRC_SRC_INPUT_CSG)
  {
    reg |= config->cmp_clear << HRPWM0_HRC_GSEL_C1CS_Pos;
    reg |= XMC_HRPWM_HRC_SRC_INPUT_CSG << HRPWM0_HRC_GSEL_C1M_Pos;  /* comparator output controlled the clear config */
  }
  else
  {
    reg &= ~(HRPWM0_HRC_GSEL_C1M_Msk);	/* CCU Timer controlled the clear config */
  }


  reg |= config->set_edge_config << HRPWM0_HRC_GSEL_S1ES_Pos;
  reg |= config->clear_edge_config << HRPWM0_HRC_GSEL_C1ES_Pos;
  hrc->GSEL &= 0x0000FFFF;
  hrc->GSEL |= reg;

  /*****************************************************************************
   * HRCy timer selection (HRCyTSEL)
   ****************************************************************************/
  reg = 0;
  reg = config->timer_sel;
  reg |= config->src_trap_enable << HRPWM0_HRC_TSEL_TS1E_Pos;
  hrc->TSEL &= 0xFFFDFFC7;
  hrc->TSEL |= reg;
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM CSG GLOBAL
 **********************************************************************************************************************/
/* No api's for CSG GLOBAL in xmc_hrpwm.c file */
 
/***********************************************************************************************************************
 * API IMPLEMENTATION - HRPWM CSG SLICE
 **********************************************************************************************************************/
/* Initialization of CSG slice */
void XMC_HRPWM_CSG_Init(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CONFIG_t *const config)
{
  uint32_t reg;

  XMC_ASSERT("XMC_HRPWM_CSG_Init:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  /* Passive level configuration */
  csg->PLC = config->cmp_config.plc;
  
  /* DAC Reference values */
  csg->SDSV1 = config->dac_config.dac_dsv1;
  csg->DSV2  = config->dac_config.dac_dsv2;

  /* Pulse Swallow value */
  csg->SPC = config->sgen_config.pulse_swallow_val;
  
  /* Slope generation control (CSGySC) */
  if(config->sgen_config.ctrl_mode != (uint32_t) XMC_HRPWM_CSG_SLOPE_CTRL_MODE_STATIC)
  {
    /* Dynamic Slope Generation */
	  csg->SC = config->sgen_config.sc;
  }
  else
  {
    /* Static Mode */
    csg->SC = config->sgen_config.static_mode_ist_enable << HRPWM0_CSG_SC_IST_Pos;
  }
  reg = config->dac_config.start_mode << HRPWM0_CSG_SC_SWSM_Pos;
  csg->SC |= reg;

  /* Comparator Initialization */
  csg->CC = config->cmp_config.cc;

  /* Blanking value */
  csg->BLV = config->cmp_config.blanking_val;
}

/* Set either CINA or CINB as inverting input of the comparator */
void XMC_HRPWM_CSG_SetCMPInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CMP_INPUT_t input)
{
  XMC_ASSERT("XMC_HRPWM_CSG_SetCMPInput:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  if (input == XMC_HRPWM_CSG_CMP_INPUT_CINA)
  {
    /* Inverting comparator input connected to CINA */
    csg->CC &= ~(HRPWM0_CSG_CC_IMCS_Msk);
  }
  else
  {
    /* Inverting comparator input connected to CINB */    
    csg->CC |= HRPWM0_CSG_CC_IMCS_Msk;
  }
}

/* Configure input selection for Blanking function */
void XMC_HRPWM_CSG_SelBlankingInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  uint32_t reg;
  
  XMC_ASSERT("XMC_HRPWM_CSG_SelBlankingInput:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  reg = csg->CC;
  
  if ((reg & HRPWM0_CSG_CC_EBE_Msk) != 0 )  /* external blanking trigger enabled? */
  {
    reg &= ~HRPWM0_CSG_CC_IBS_Msk;
    reg |= (uint32_t) config->mapped_input;
  }

  reg &= ~HRPWM0_CSG_CC_BLMC_Msk;
  reg |= ((uint32_t) config->edge) << HRPWM0_CSG_CC_BLMC_Pos;

  csg->CC = reg;
}

/* Configure input selection to start slope generation function */
void XMC_HRPWM_CSG_StartSlopeGenConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_StartSlopeGenConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~HRPWM0_CSG0_DCI_STRIS_Msk;
  csg->DCI |= config->mapped_input << HRPWM0_CSG0_DCI_STRIS_Pos;
  csg->IES &= ~HRPWM0_CSG_IES_STRES_Msk;
  csg->IES |= (config->edge) << HRPWM0_CSG_IES_STRES_Pos;
}

/* Configure input selection to stop slope generation function */
void XMC_HRPWM_CSG_StopSlopeGenConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_StopSlopeGenConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~HRPWM0_CSG0_DCI_STPIS_Msk;
  csg->DCI |= config->mapped_input << HRPWM0_CSG0_DCI_STPIS_Pos;
  csg->IES &= ~HRPWM0_CSG_IES_STPES_Msk;
  csg->IES |= (config->edge) << HRPWM0_CSG_IES_STPES_Pos;
}

/* Configure input selection for triggering DAC conversion */
void XMC_HRPWM_CSG_TriggerDACConvConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_TriggerDACConvConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~HRPWM0_CSG0_DCI_TRGIS_Msk;
  csg->DCI |= config->mapped_input << HRPWM0_CSG0_DCI_TRGIS_Pos;
  csg->IES &= ~HRPWM0_CSG_IES_TRGES_Msk;
  csg->IES |= (config->edge) << HRPWM0_CSG_IES_TRGES_Pos;
}

/* Configure input selection for triggering shadow transfer */
void XMC_HRPWM_CSG_TriggerShadowXferConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_TriggerShadowXferConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~HRPWM0_CSG0_DCI_STIS_Msk;
  csg->DCI |= config->mapped_input << HRPWM0_CSG0_DCI_STIS_Pos;
  csg->IES &= ~HRPWM0_CSG_IES_STES_Msk;
  csg->IES |= (config->edge) << HRPWM0_CSG_IES_STES_Pos;
}

/* Configure input selection to trigger a switch in DAC reference value. This is only applicable to DAC in static mode */
void XMC_HRPWM_CSG_DACRefValSwitchingConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_HRPWM_CSG_DACRefValSwitchingConfig:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~HRPWM0_CSG0_DCI_SVIS_Msk;
  csg->DCI |= config->mapped_input << HRPWM0_CSG0_DCI_SVIS_Pos;
  csg->IES &= ~HRPWM0_CSG_IES_SVLS_Msk;
  csg->IES |= config->level << HRPWM0_CSG_IES_SVLS_Pos;
}

/* Configure input selection for clock selection used in slope generation */
void XMC_HRPWM_CSG_SelSlopeGenClkInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CLK_INPUT_t input_clk)
{
  XMC_ASSERT("XMC_HRPWM_CSG_SelSlopeGenClkInput:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  csg->DCI &= ~HRPWM0_CSG0_DCI_SCS_Msk;
  csg->DCI |= input_clk << HRPWM0_CSG0_DCI_SCS_Pos;
}

/* Set the service request interrupt node */
void XMC_HRPWM_CSG_SetSRNode(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_IRQ_ID_t event,
                                   const XMC_HRPWM_CSG_IRQ_SR_LINE_t sr)
{
  XMC_ASSERT("XMC_HRPWM_CSG_SetSRNode:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  
  switch (event)
  {
    case (XMC_HRPWM_CSG_IRQ_ID_VLS1):
      csg->SRS &= ~HRPWM0_CSG_SRS_VLS1S_Msk;
      csg->SRS |= sr;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_VLS2):
      csg->SRS &= ~HRPWM0_CSG_SRS_VLS2S_Msk;
      csg->SRS |= sr << HRPWM0_CSG_SRS_VLS2S_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_TRGS):
      csg->SRS &= ~HRPWM0_CSG_SRS_TRLS_Msk;
      csg->SRS |= sr << HRPWM0_CSG_SRS_TRLS_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_STRS):
    case (XMC_HRPWM_CSG_IRQ_ID_STPS):
      csg->SRS &= ~HRPWM0_CSG_SRS_SSLS_Msk;
      csg->SRS |= sr << HRPWM0_CSG_SRS_SSLS_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_STD):
      csg->SRS &= ~HRPWM0_CSG_SRS_STLS_Msk;
      csg->SRS |= sr << HRPWM0_CSG_SRS_STLS_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_CRSE):
    case (XMC_HRPWM_CSG_IRQ_ID_CFSE):
      csg->SRS &= ~HRPWM0_CSG_SRS_CRFLS_Msk;
      csg->SRS |= sr << HRPWM0_CSG_SRS_CRFLS_Pos;
      break;

    case (XMC_HRPWM_CSG_IRQ_ID_CSEE):
      csg->SRS &= ~HRPWM0_CSG_SRS_CSLS_Msk;
      csg->SRS |= sr << HRPWM0_CSG_SRS_CSLS_Pos;
      break;

    default:
      break;
  }
}

#endif  /* #if defined(HRPWM0) */
