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
 * @file xmc_dma.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief DMA driver for XMC4000 microcontroller family.
 *
 * History <br>
 *
 * Version 1.0.0  Initial <br>
 */

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "xmc_dma.h"
#include "xmc_scu.h"

#if defined(GPDMA0)

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define DLR_SRSEL_RS_MSK (0xfUL)
#define DLR_SRSEL_RS_BITSIZE (4UL)
#define DMA_EVENT_MAX (5UL)
#define GPDMA_CH_CFGH_DEST_PER_Pos GPDMA0_CH_CFGH_DEST_PER_Pos
#define GPDMA_CH_CFGH_SRC_PER_Pos GPDMA0_CH_CFGH_SRC_PER_Pos
#define GPDMA0_CH_CFGH_PER_Msk (0x7U)
#define GPDMA1_CH_CFGH_PER_Msk (0x3U)
#define GPDMA_CH_CFGH_PER_BITSIZE (4U)
#define GPDMA_CH_CTLL_INT_EN_Msk GPDMA0_CH_CTLL_INT_EN_Msk

/*******************************************************************************
 * LOCAL DATA
 *******************************************************************************/

#if defined (GPDMA0)
XMC_DMA_CH_EVENT_HANDLER_t dma0_event_handlers[XMC_DMA0_NUM_CHANNELS];
XMC_PRIOARRAY_DEF(dma0_ordered_channels, XMC_DMA0_NUM_CHANNELS);
#endif

#if defined (GPDMA1)
XMC_DMA_CH_EVENT_HANDLER_t dma1_event_handlers[XMC_DMA1_NUM_CHANNELS];
XMC_PRIOARRAY_DEF(dma1_ordered_channels, XMC_DMA1_NUM_CHANNELS);
#endif
 
/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

 /* Get DMA low level driver version */
XMC_DRIVER_VERSION_t XMC_DMA_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = (uint8_t)XMC_DMA_MAJOR_VERSION;
  version.minor = (uint8_t)XMC_DMA_MINOR_VERSION;
  version.patch = (uint8_t)XMC_DMA_PATCH_VERSION;

  return version;
}

/* Initialize GPDMA */
void XMC_DMA_Init(XMC_DMA_t *const dma)
{
#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
    XMC_PRIOARRAY_Init(XMC_PRIOARRAY(dma0_ordered_channels));
#if UC_SERIES == XMC45
  }
  else
  {
    XMC_PRIOARRAY_Init(XMC_PRIOARRAY(dma1_ordered_channels));
  }
#endif

  XMC_DMA_Enable(dma);
}

/* Enable GPDMA module */
void XMC_DMA_Enable(XMC_DMA_t *const dma)
{
#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
#if (UC_SERIES != XMC45)
    XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_GPDMA0);
#endif
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_GPDMA0);
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_GPDMA0);
#if UC_SERIES == XMC45
  }
  else
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_GPDMA1);
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_GPDMA1);
  }
#endif

  dma->DMACFGREG = 0x1U;
}

/* Disable GPDMA module */
void XMC_DMA_Disable(XMC_DMA_t *const dma)
{
  dma->DMACFGREG = 0x0U;

#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_GPDMA0);
#if (UC_SERIES != XMC45)
    XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_GPDMA0);
#endif
#if UC_SERIES == XMC45
  }
  else
  {
    XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_GPDMA1);
  }
#endif
}

/* Check is the GPDMA peripheral is enabled */
bool XMC_DMA_IsEnabled(const XMC_DMA_t *const dma)
{
  bool status;

#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
    status = !XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_GPDMA0);
#if (UC_SERIES != XMC45)
    status = status && !XMC_SCU_CLOCK_IsPeripheralClockGated(XMC_SCU_PERIPHERAL_CLOCK_GPDMA0);
#endif
#if UC_SERIES == XMC45
  }
  else
  {
    status = !XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_GPDMA1);
  }
#endif

  /* DMA reset is not asserted and peripheral clock is not gated */
  if (status == true)
  {
    status = status && (dma->DMACFGREG != 0U);
  }

  return status;
}

/* Enable request line */
void XMC_DMA_EnableRequestLine(XMC_DMA_t *const dma, uint8_t line, uint8_t peripheral)
{
#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
    DLR->SRSEL0 = ((DLR->SRSEL0 & (uint32_t)~(DLR_SRSEL_RS_MSK << (line * DLR_SRSEL_RS_BITSIZE))) |
                   ((uint32_t)peripheral << (line * DLR_SRSEL_RS_BITSIZE)));
    DLR->LNEN |= (0x1UL << line);
#if UC_SERIES == XMC45
  }
  else
  {
    DLR->SRSEL1 = ((DLR->SRSEL1 & (uint32_t)~(DLR_SRSEL_RS_MSK << (line * DLR_SRSEL_RS_BITSIZE))) |
                   ((uint32_t)peripheral << (line * DLR_SRSEL_RS_BITSIZE)));
    DLR->LNEN |= (0x100UL << line);
  }
#endif
}

/* Get DMA DLR overrun status */
bool XMC_DMA_GetOverrunStatus(XMC_DMA_t *const dma, uint8_t line)
{
  bool status;

#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
    status = (bool)(DLR->OVRSTAT & (0x1UL << line));
#if UC_SERIES == XMC45
  }
  else
  {
    status = (bool)(DLR->OVRSTAT & (0x100UL << line));
  }
#endif

  return status;
}

/* Clear DMA DLR overrun status */
void XMC_DMA_ClearOverrunStatus(XMC_DMA_t *const dma, const uint8_t line)
{
#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
    DLR->OVRCLR |= (uint32_t)(0x1UL << line);
#if UC_SERIES == XMC45
  }
  else
  {
    DLR->OVRCLR |= (uint32_t)(0x100UL << line);
  }
#endif
}

/* Enable DMA channel */
void XMC_DMA_CH_Enable(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CHENREG = (uint32_t)(0x101UL << channel);
}

/* Disable DMA channel */
void XMC_DMA_CH_Disable(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CFGL |= (uint32_t)GPDMA0_CH_CFGL_CH_SUSP_Msk;
  while((dma->CH[channel].CFGL & GPDMA0_CH_CFGL_FIFO_EMPTY_Msk) == 0U)
  {
    /* wait until no data is left in the channel FIFO */
  }
  dma->CHENREG = (uint32_t)(0x100UL << channel);
}

/* Check if a DMA channel is enabled */
bool XMC_DMA_CH_IsEnabled(XMC_DMA_t *const dma, const uint8_t channel)
{
  return (bool)(dma->CHENREG & ((uint32_t)1U << channel));
}

/* Initialize DMA channel */
XMC_DMA_CH_STATUS_t XMC_DMA_CH_Init(XMC_DMA_t *const dma, const uint8_t channel, const XMC_DMA_CH_CONFIG_t *const config)
{
  XMC_DMA_CH_STATUS_t status;
  uint8_t interface;
  uint8_t peripheral;

  if (XMC_DMA_IsEnabled(dma) == true)
  {
    if (XMC_DMA_CH_IsEnabled(dma, channel) == false)
    {
      dma->CH[channel].SAR = config->src_addr;
      dma->CH[channel].DAR = config->dst_addr;
      dma->CH[channel].LLP = (uint32_t)config->linked_list_pointer;
      dma->CH[channel].CTLH = (uint32_t)config->block_size;
      dma->CH[channel].CTLL = config->control;

      dma->CH[channel].CFGL = (uint32_t)((uint32_t)config->priority |
                                         (uint32_t)GPDMA0_CH_CFGL_HS_SEL_SRC_Msk |
                                         (uint32_t)GPDMA0_CH_CFGL_HS_SEL_DST_Msk);

#if UC_SERIES == XMC45
      if (dma == XMC_DMA0)
      {
#endif
        XMC_PRIOARRAY_Add(XMC_PRIOARRAY(dma0_ordered_channels), (int32_t)channel, (int32_t)config->priority);

        if (channel < (uint8_t)2)
        {
          /* Configure scatter and gather */
          dma->CH[channel].SGR = config->src_gather_control;
          dma->CH[channel].DSR = config->dst_scatter_control;
        }
#if UC_SERIES == XMC45
      }
      else
      {
        XMC_PRIOARRAY_Add(XMC_PRIOARRAY(dma1_ordered_channels), (int32_t)channel, (int32_t)config->priority);
      }
#endif

      if (config->dst_handshaking == XMC_DMA_CH_DST_HANDSHAKING_HARDWARE)
      {
        /* Hardware handshaking interface configuration */
        if ((config->transfer_flow == (uint32_t)XMC_DMA_CH_TRANSFER_FLOW_M2P_DMA) ||
            (config->transfer_flow == (uint32_t)XMC_DMA_CH_TRANSFER_FLOW_P2P_DMA))
        {
#if UC_SERIES == XMC45
          if (dma == XMC_DMA0)
          {
#endif
            interface = config->dst_peripheral_request & GPDMA0_CH_CFGH_PER_Msk;
#if UC_SERIES == XMC45
          }
          else
          {
            interface = config->dst_peripheral_request & GPDMA1_CH_CFGH_PER_Msk;
          }
#endif
          peripheral = config->dst_peripheral_request >> GPDMA_CH_CFGH_PER_BITSIZE;

          dma->CH[channel].CFGH |= (uint32_t)((uint32_t)interface << GPDMA0_CH_CFGH_DEST_PER_Pos);
          XMC_DMA_EnableRequestLine(dma, interface, peripheral);
          dma->CH[channel].CFGL &= (uint32_t)~GPDMA0_CH_CFGL_HS_SEL_DST_Msk;
        }
      }


      if (config->src_handshaking == XMC_DMA_CH_SRC_HANDSHAKING_HARDWARE)
      {
        if ((config->transfer_flow == (uint32_t)XMC_DMA_CH_TRANSFER_FLOW_P2M_DMA) ||
            (config->transfer_flow == (uint32_t)XMC_DMA_CH_TRANSFER_FLOW_P2P_DMA))
        {
#if UC_SERIES == XMC45
          if (dma == XMC_DMA0)
          {
#endif
            interface = config->src_peripheral_request & GPDMA0_CH_CFGH_PER_Msk;
#if UC_SERIES == XMC45
          }
          else
          {
            interface = config->src_peripheral_request & GPDMA1_CH_CFGH_PER_Msk;
          }
#endif
          peripheral = config->src_peripheral_request >> GPDMA_CH_CFGH_PER_BITSIZE;

          dma->CH[channel].CFGH |= (uint32_t)((uint32_t)interface << GPDMA0_CH_CFGH_SRC_PER_Pos);
          XMC_DMA_EnableRequestLine(dma, interface, peripheral);
          dma->CH[channel].CFGL &= (uint32_t)~GPDMA0_CH_CFGL_HS_SEL_SRC_Msk;
        }
      }
      
      XMC_DMA_CH_ClearEventStatus(dma, channel, (uint32_t)((uint32_t)XMC_DMA_CH_EVENT_TRANSFER_COMPLETE |
                                                           (uint32_t)XMC_DMA_CH_EVENT_BLOCK_TRANSFER_COMPLETE |
                                                           (uint32_t)XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE |
                                                           (uint32_t)XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE |
                                                           (uint32_t)XMC_DMA_CH_EVENT_ERROR));

      switch (config->transfer_type)
      {
        case XMC_DMA_CH_TRANSFER_TYPE_SINGLE_BLOCK:
          break;

        case XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_CONTIGUOUS_DSTADR_RELOAD:
          dma->CH[channel].CFGL |= (uint32_t)GPDMA0_CH_CFGL_RELOAD_DST_Msk;
          break;

        case XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_RELOAD_DSTADR_CONTIGUOUS:
          dma->CH[channel].CFGL |= (uint32_t)GPDMA0_CH_CFGL_RELOAD_SRC_Msk;
          break;

        case XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_RELOAD_DSTADR_RELOAD:
          dma->CH[channel].CFGL |= (uint32_t)((uint32_t)GPDMA0_CH_CFGL_RELOAD_DST_Msk |
                                              (uint32_t)GPDMA0_CH_CFGL_RELOAD_SRC_Msk);
          break;

        case XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_CONTIGUOUS_DSTADR_LINKED:
          dma->CH[channel].CTLL |= (uint32_t)GPDMA0_CH_CTLL_LLP_DST_EN_Msk;
          break;

        case XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_RELOAD_DSTADR_LINKED:
          dma->CH[channel].CFGL |= (uint32_t)GPDMA0_CH_CFGL_RELOAD_SRC_Msk;
          dma->CH[channel].CTLL |= (uint32_t)GPDMA0_CH_CTLL_LLP_DST_EN_Msk;
          break;

        case XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_LINKED_DSTADR_CONTIGUOUS:
          dma->CH[channel].CTLL |= (uint32_t)GPDMA0_CH_CTLL_LLP_SRC_EN_Msk;
          break;

        case XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_LINKED_DSTADR_RELOAD:
          dma->CH[channel].CFGL |= (uint32_t)GPDMA0_CH_CFGL_RELOAD_DST_Msk;
          dma->CH[channel].CTLL |= (uint32_t)GPDMA0_CH_CTLL_LLP_SRC_EN_Msk;
          break;

        case XMC_DMA_CH_TRANSFER_TYPE_MULTI_BLOCK_SRCADR_LINKED_DSTADR_LINKED:
          dma->CH[channel].CTLL |= (uint32_t)((uint32_t)GPDMA0_CH_CTLL_LLP_SRC_EN_Msk |
                                              (uint32_t)GPDMA0_CH_CTLL_LLP_DST_EN_Msk);
          break;

        default:
          break;
      }

      status = XMC_DMA_CH_STATUS_OK;

    }
    else
    {
      status = XMC_DMA_CH_STATUS_BUSY;
    }
  }
  else 
  {
    status = XMC_DMA_CH_STATUS_ERROR;
  }
  
  return status;
}

/* Suspend DMA channel transfer */
void XMC_DMA_CH_Suspend(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CFGL |= (uint32_t)GPDMA0_CH_CFGL_CH_SUSP_Msk;
}

/* Resume DMA channel transfer */
void XMC_DMA_CH_Resume(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CFGL &= (uint32_t)~GPDMA0_CH_CFGL_CH_SUSP_Msk;
}

/* Check if a DMA channel is suspended */
bool XMC_DMA_CH_IsSuspended(XMC_DMA_t *const dma, const uint8_t channel)
{
  return (bool)(dma->CH[channel].CFGL & (uint32_t)GPDMA0_CH_CFGL_CH_SUSP_Msk);
}

/* Enable GPDMA event */
void XMC_DMA_CH_EnableEvent(XMC_DMA_t *const dma, const uint8_t channel, const uint32_t event)
{
  uint32_t event_idx;

  for(event_idx = 0UL; event_idx < DMA_EVENT_MAX; ++event_idx)
  {
    if (event & ((uint32_t)0x1UL << event_idx))
    {
      dma->MASKCHEV[event_idx * 2UL] |= ((uint32_t)0x101UL << channel);
    }
  }
}

/* Disable GPDMA event */
void XMC_DMA_CH_DisableEvent(XMC_DMA_t *const dma, const uint8_t channel, const uint32_t event)
{
  uint32_t event_idx;

  for(event_idx = 0UL; event_idx < DMA_EVENT_MAX; ++event_idx)
  {
    if (event & ((uint32_t)0x1UL << event_idx))
    {
      dma->MASKCHEV[event_idx * 2UL] |= ((uint32_t)0x100UL << channel);
    }
  }
}

/* Clear GPDMA event */
void XMC_DMA_CH_ClearEventStatus(XMC_DMA_t *const dma, const uint8_t channel, const uint32_t event)
{
  uint32_t event_idx;

  for(event_idx = 0UL; event_idx < DMA_EVENT_MAX; ++event_idx)
  {
    if (event & (uint32_t)((uint32_t)0x1UL << event_idx))
    {
      dma->CLEARCHEV[event_idx * 2UL] = ((uint32_t)0x1UL << channel);
    }
  }

}

/* Get GPDMA event status */
uint32_t XMC_DMA_CH_GetEventStatus(XMC_DMA_t *const dma, const uint8_t channel)
{
  uint32_t event_idx;
  uint32_t status = 0UL;

  for(event_idx = 0UL; event_idx < DMA_EVENT_MAX; ++event_idx)
  {
    status |= (uint32_t)((dma->STATUSCHEV[event_idx * 2UL] & (uint32_t)((uint32_t)0x1UL << (uint32_t)channel)) ?
                         ((uint32_t)0x1UL << event_idx) : (uint32_t)0UL);
  }

  return status;
}

/* Enable source gather */
void XMC_DMA_CH_EnableSourceGather(XMC_DMA_t *const dma, const uint8_t channel, uint32_t interval, uint16_t count)
{
  dma->CH[channel].CTLL |= (uint32_t)GPDMA0_CH_CTLL_SRC_GATHER_EN_Msk;
  dma->CH[channel].SGR = ((uint32_t)interval << GPDMA0_CH_SGR_SGI_Pos) | ((uint32_t)count << GPDMA0_CH_SGR_SGC_Pos);
}

/* Disable source gather */
void XMC_DMA_CH_DisableSourceGather(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CTLL &= (uint32_t)~GPDMA0_CH_CTLL_SRC_GATHER_EN_Msk;
}

/* Enable destination scatter */
void XMC_DMA_CH_EnableDestinationScatter(XMC_DMA_t *const dma, const uint8_t channel, uint32_t interval, uint16_t count)
{
  dma->CH[channel].CTLL |= (uint32_t)GPDMA0_CH_CTLL_DST_SCATTER_EN_Msk;
  dma->CH[channel].DSR = ((uint32_t)interval << GPDMA0_CH_DSR_DSI_Pos) | ((uint32_t)count << GPDMA0_CH_DSR_DSC_Pos);
}

/* Disable destination scatter */
void XMC_DMA_CH_DisableDestinationScatter(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CTLL &= (uint32_t)~GPDMA0_CH_CTLL_DST_SCATTER_EN_Msk;
}

/* Trigger source request */
void XMC_DMA_CH_TriggerSourceRequest(XMC_DMA_t *const dma, const uint8_t channel, const XMC_DMA_CH_TRANSACTION_TYPE_t type, bool last)
{
  if ((uint32_t)type == (uint32_t)XMC_DMA_CH_TRANSACTION_TYPE_SINGLE)
  {  
    dma->SGLREQSRCREG = ((uint32_t)0x101UL << channel);
  }

  if (last == true)
  {
  dma->LSTSRCREG = (uint32_t)0x101UL << channel;
  }
  
  dma->REQSRCREG = (uint32_t)0x101UL << channel;
}

/* Trigger destination request */
void XMC_DMA_CH_TriggerDestinationRequest(XMC_DMA_t *const dma, const uint8_t channel, const XMC_DMA_CH_TRANSACTION_TYPE_t type, bool last)
{
  if(type == XMC_DMA_CH_TRANSACTION_TYPE_SINGLE)
  {  
    dma->SGLREQDSTREG = (uint32_t)0x101UL << channel;
  }

  if (last == true)
  {
    dma->LSTDSTREG = (uint32_t)0x101UL << channel;
  }

  dma->REQDSTREG = (uint32_t)0x101UL << channel;
}

/* Enable source address reload */
void XMC_DMA_CH_EnableSourceAddressReload(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CFGL |= (uint32_t)GPDMA0_CH_CFGL_RELOAD_SRC_Msk;
}

/* Disable source address reload */
void XMC_DMA_CH_DisableSourceAddressReload(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CFGL &= (uint32_t)~GPDMA0_CH_CFGL_RELOAD_SRC_Msk;
}

/* Enable destination address reload */
void XMC_DMA_CH_EnableDestinationAddressReload(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CFGL |= (uint32_t)GPDMA0_CH_CFGL_RELOAD_DST_Msk;
}

/* Disable destination address reload */
void XMC_DMA_CH_DisableDestinationAddressReload(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CFGL &= (uint32_t)~GPDMA0_CH_CFGL_RELOAD_DST_Msk;
}

/* Request last multi-block transfer */
void XMC_DMA_CH_RequestLastMultiblockTransfer(XMC_DMA_t *const dma, const uint8_t channel)
{
  dma->CH[channel].CFGL &= (uint32_t)~(GPDMA0_CH_CFGL_RELOAD_SRC_Msk | GPDMA0_CH_CFGL_RELOAD_DST_Msk);
}

/* Set event handler */
void XMC_DMA_CH_SetEventHandler(XMC_DMA_t *const dma, const uint8_t channel, XMC_DMA_CH_EVENT_HANDLER_t event_handler)
{
#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
    dma0_event_handlers[channel] = event_handler;
#if UC_SERIES == XMC45
  }
  else
  {
    dma1_event_handlers[channel] = event_handler;
  }
#endif
}

/* Default DMA IRQ handler */
void XMC_DMA_IRQHandler(XMC_DMA_t *const dma)
{
  uint32_t event;
  int32_t channel;
  uint32_t mask;
  XMC_DMA_CH_EVENT_HANDLER_t *dma_event_handlers;
  XMC_DMA_CH_EVENT_HANDLER_t event_handler;
  XMC_PRIOARRAY_t *prio_array;

#if UC_SERIES == XMC45
  if (dma == XMC_DMA0)
  {
#endif
    prio_array = XMC_PRIOARRAY(dma0_ordered_channels);
    dma_event_handlers = dma0_event_handlers;
#if UC_SERIES == XMC45
  }
  else
  {
    prio_array = XMC_PRIOARRAY(dma1_ordered_channels);
    dma_event_handlers = dma1_event_handlers;
  }
#endif
  
  event = XMC_DMA_GetEventStatus(dma);

  if ((event & (uint32_t)XMC_DMA_CH_EVENT_ERROR) != (uint32_t)0UL)
  {
    event = XMC_DMA_GetChannelsErrorStatus(dma);

    channel = XMC_PRIOARRAY_GetHead(prio_array);
    while (channel != -1)
    {
      mask = (uint32_t)1U << channel;
      if (event & mask)
      {
        event_handler = dma_event_handlers[channel];

        /* Call user callback to handle event */
        if (event_handler != NULL)
        {
          event_handler(XMC_DMA_CH_EVENT_ERROR);
        }

        XMC_DMA_CH_ClearEventStatus(dma, (uint8_t)channel, (uint32_t)XMC_DMA_CH_EVENT_ERROR);
        
        break;
      }
      channel = XMC_PRIOARRAY_GetItemNext(prio_array, channel);
    }
  }
  else if ((event & (uint32_t)XMC_DMA_CH_EVENT_TRANSFER_COMPLETE) != (uint32_t)0UL)
  {
    event = XMC_DMA_GetChannelsTransferCompleteStatus(dma);

    channel = XMC_PRIOARRAY_GetHead(prio_array);
    while (channel != -1)
    {
      mask = (uint32_t)1U << channel;
      if (event & mask)
      {
        event_handler = dma_event_handlers[channel];

        /* Call user callback to handle event */
        if (event_handler != NULL)
        {
          event_handler(XMC_DMA_CH_EVENT_TRANSFER_COMPLETE);
        }

        XMC_DMA_CH_ClearEventStatus(dma, (uint8_t)channel, (uint32_t)((uint32_t)XMC_DMA_CH_EVENT_TRANSFER_COMPLETE | 
                                                                      (uint32_t)XMC_DMA_CH_EVENT_BLOCK_TRANSFER_COMPLETE | 
                                                                      (uint32_t)XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE | 
                                                                      (uint32_t)XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE));
																	  
        XMC_PRIOARRAY_Remove(prio_array, channel);
                                                           
        break;
      }
      channel = XMC_PRIOARRAY_GetItemNext(prio_array, channel);
    }
  }
  else if ((event & (uint32_t)XMC_DMA_CH_EVENT_BLOCK_TRANSFER_COMPLETE) != (uint32_t)0UL)
  {
    event = XMC_DMA_GetChannelsBlockCompleteStatus(dma);

    channel = XMC_PRIOARRAY_GetHead(prio_array);
    while (channel != -1)
    {
      mask = (uint32_t)1U << channel;
      if (event & mask)
      {
        event_handler = dma_event_handlers[channel];

        /* Call user callback to handle event */
        if (event_handler != NULL)
        {
          event_handler(XMC_DMA_CH_EVENT_BLOCK_TRANSFER_COMPLETE);
        }

        XMC_DMA_CH_ClearEventStatus(dma, (uint8_t)channel, (uint32_t)((uint32_t)XMC_DMA_CH_EVENT_BLOCK_TRANSFER_COMPLETE |
                                                                      (uint32_t)XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE | 
                                                                      (uint32_t)XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE));
        break;
      }
      channel = XMC_PRIOARRAY_GetItemNext(prio_array, channel);
    }
  }
  else if ((event & (uint32_t)XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE) != (uint32_t)0UL)
  {
    event = XMC_DMA_GetChannelsSourceTransactionCompleteStatus(dma);

    channel = XMC_PRIOARRAY_GetHead(prio_array);
    while (channel != -1)
    {
      mask = (uint32_t)1U << channel;
      if (event & mask)
      {
        event_handler = dma_event_handlers[channel];

        /* Call user callback to handle event */
        if (event_handler != NULL)
        {
          event_handler(XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE);
        }

        XMC_DMA_CH_ClearEventStatus(dma, (uint8_t)channel, (uint32_t)XMC_DMA_CH_EVENT_SRC_TRANSACTION_COMPLETE);
        
        break;
      }
      channel = XMC_PRIOARRAY_GetItemNext(prio_array, channel);
    }
  }
  else if ((event & (uint32_t)XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE) != (uint32_t)0UL)
  {
    event = XMC_DMA_GetChannelsDestinationTransactionCompleteStatus(dma);

    channel = XMC_PRIOARRAY_GetHead(prio_array);
    while (channel != -1)
    {
      mask = (uint32_t)1U << channel;
      if (event & mask)
      {
        event_handler = dma_event_handlers[channel];

        /* Call user callback to handle event */
        if (event_handler != NULL)
        {
          event_handler(XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE);
        }
        
        XMC_DMA_CH_ClearEventStatus(dma, (uint8_t)channel, (uint32_t)XMC_DMA_CH_EVENT_DST_TRANSACTION_COMPLETE);

        break;
      }
      channel = XMC_PRIOARRAY_GetItemNext(prio_array, channel);
    }
  }
  else
  {
    /* no active interrupt was found? */
  }

}

#endif /* GPDMA0 */
