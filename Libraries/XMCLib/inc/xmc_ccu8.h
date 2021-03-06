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
 * @file xmc_ccu8.h
 * @date 20 Feb, 2015
 * @version 1.0.2
 *
 * History
 *
 * Version 1.0.0  Initial version <br>
 * Version 1.0.2  Documentation updates <br>
*/

#ifndef CCU8_H
#define CCU8_H

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_common.h"
#include "xmc_scu.h"

#if defined(CCU80)

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup CCU8
 *
 * @brief Capture Compare Unit 8 (CCU8) low level driver for XMC family of microcontrollers<br>
 *
 * The CCU8 peripheral functions play a major role in applications that need complex Pulse Width Modulation (PWM) signal
 * generation, with complementary high side and low side switches, multi phase control. These functions in conjunction 
 * with a very flexible and programmable signal conditioning scheme, make the CCU8 the must have peripheral for state 
 * of the art motor control, multi phase and multi level power electronics systems.\n
 * Each CCU8 module is comprised of four identical 16 bit Capture/Compare Timer slices, CC8y (where y = [0..4]). Each 
 * timer slice can work in compare mode or in capture mode.
 *
 * APIs provided in this file cover the following functional blocks of CCU8:\n
 * -- Timer configuration, Capture configuration, Function/Event configuration, Interrupt configuration\n
 * \par Note:
 * 1. SLICE (APIs prefixed with e.g. XMC_CCU8_SLICE_)\n
 * 2. Module (APIs are not having any prefix e.g. XMC_CCU8_) \n
 *
 * \par Timer(Compare mode) configuration:\n
 * This section of the LLD provides the configuration structure XMC_CCU8_SLICE_COMPARE_CONFIG_t, 
 * XMC_CCU8_SLICE_DEAD_TIME_CONFIG_t and the initialization functions XMC_CCU8_SLICE_CompareInit(), XMC_CCU8_SLICE_DeadTimeInit().\n
 *
 * It can be used to:
 * -# Start and Stop the timer. (XMC_CCU8_SLICE_StartTimer(), XMC_CCU8_SLICE_StopTimer())
 * -# Update the period, compare, Dither, Prescaler  and Passive values. (XMC_CCU8_SLICE_SetTimerPeriodMatch(), 
 *    XMC_CCU8_SLICE_SetTimerCompareMatch(), XMC_CCU8_SLICE_SetPrescaler(), XMC_CCU8_SLICE_SetDitherCompareValue(), 
 *    XMC_CCU8_SLICE_SetPassiveLevel())
 * -# Configure the dead time.(XMC_CCU8_SLICE_SetDeadTimeValue(), XMC_CCU8_SLICE_SetDeadTimePrescaler())
 * -# Enable the slices to support multichannel mode. (XMC_CCU8_SLICE_EnableMultiChannelMode())
 * 
 * \par Capture configuration:\n
 * This section of the LLD provides the configuration structure XMC_CCU8_SLICE_CAPTURE_CONFIG_t and the initialization
 * function XMC_CCU8_SLICE_CaptureInit().\n
 *
 * It can be used to:
 * -# Configure the capture functionality. (XMC_CCU8_SLICE_Capture0Config(), XMC_CCU8_SLICE_Capture1Config())
 * -# Read the captured values along with the status, which indicate the value is latest or not.
 *    (XMC_CCU8_SLICE_GetCaptureRegisterValue())
 * 
 * \par Function/Event configuration:\n
 * This section of the LLD provides the configuration structure XMC_CCU8_SLICE_EVENT_CONFIG_t.\n
 * 
 * It can be used to:
 * -# Enable and Disable the events. (XMC_CCU8_SLICE_EnableEvent(), XMC_CCU8_SLICE_DisableEvent())
 * -# Configure to start and stop the timer on external events.(XMC_CCU8_SLICE_StartConfig(), XMC_CCU8_SLICE_StopConfig())
 * -# Modulation, external load and Gating of the timer output.(XMC_CCU8_SLICE_ModulationConfig(), 
 *    XMC_CCU8_SLICE_LoadConfig(), XMC_CCU8_SLICE_GateConfig())
 * -# Control the count direction of the timer based on the external event. (XMC_CCU8_SLICE_DirectionConfig())
 * -# Count the external events.(XMC_CCU8_SLICE_CountConfig())
 * -# External Trap. Which can be used as protective feature.(XMC_CCU8_SLICE_EnableTrap(), XMC_CCU8_SLICE_DisableTrap(),
 *    XMC_CCU8_SLICE_TrapConfig())
 *
 * \par Interrupt configuration:\n
 * This section of the LLD provides the function to configure the interrupt node to each event (XMC_CCU8_SLICE_SetInterruptNode())
 *
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define XMC_CCU8_MAJOR_VERSION (1U) /**< Major number of the driver version. Format \<major\>.\<minor\>.\<patch\> */
#define XMC_CCU8_MINOR_VERSION (0U) /**< Minor number of the driver version. Format \<major\>.\<minor\>.\<patch\> */
#define XMC_CCU8_PATCH_VERSION (2U) /**< Patch number of the driver version. Format \<major\>.\<minor\>.\<patch\> */

#if ((UC_SERIES == XMC45) || (UC_SERIES == XMC44))
#define XMC_CCU8_NUM_MODULES           (2U) /* Number of XMC_CCU8 modules on the device */
#elif ((UC_SERIES == XMC42) || (UC_SERIES == XMC41) || (UC_SERIES == XMC13))
#define XMC_CCU8_NUM_MODULES           (1U) /* Number of XMC_CCU8 modules on the device */
#endif

#define XMC_CCU8_NUM_SLICES_PER_MODULE (4U) /**< Number of timer slices per module */
#define XMC_CCU8_SLICE_TIMER_LENGTH    (4U) /* Timer length - 16 bits */

#if ((UC_SERIES == XMC45) || (UC_SERIES == XMC44))
/* Macro to check if the slice ptr passed is valid */
#define XMC_CCU8_CHECK_MODULE_PTR(PTR)  ( ((PTR)== CCU80) || ((PTR)== CCU81) )
#elif ((UC_SERIES == XMC42) || (UC_SERIES == XMC41) || (UC_SERIES == XMC13))
/* Macro to check if the slice ptr passed is valid */
#define XMC_CCU8_CHECK_MODULE_PTR(PTR)  ( ((PTR)== CCU80) )
#endif

#if ((UC_SERIES == XMC45) || (UC_SERIES == XMC44))
/* Macro to check if the slice ptr passed is valid */
#define XMC_CCU8_CHECK_SLICE_PTR(PTR)   ( ((PTR)== CCU80_CC80) || ((PTR)== CCU80_CC81) || ((PTR)== CCU80_CC82) || \
                                          ((PTR)== CCU80_CC83) || ((PTR)== CCU81_CC80) || ((PTR)== CCU81_CC81) || \
                                          ((PTR)== CCU81_CC82) || ((PTR)== CCU81_CC83) )
#elif ((UC_SERIES == XMC42) || (UC_SERIES == XMC41) || (UC_SERIES == XMC13))
/* Macro to check if the slice ptr passed is valid */
#define XMC_CCU8_CHECK_SLICE_PTR(PTR)   ( ((PTR)== CCU80_CC80) || ((PTR)== CCU80_CC81) || ((PTR)== CCU80_CC82) || \
                                          ((PTR)== CCU80_CC83) )
#endif

#define XMC_CCU8_SLICE_CHECK_INTERRUPT(interrupt) \
    ((interrupt == XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH)           || \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_ONE_MATCH)              || \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_1)  || \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_DOWN_CH_1)|| \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_2)  || \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_2)  || \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_EVENT0)                 || \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_EVENT1)                 || \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_EVENT2)                 || \
     (interrupt == XMC_CCU8_SLICE_IRQ_ID_TRAP))
     
/* Macro to check if the slice ptr passed is valid */
#define XMC_CCU8_SLICE_CHECK_COMP_CHANNEL(cmp_channel) \
    ((cmp_channel == XMC_CCU8_SLICE_COMPARE_CHANNEL_1) || \
     (cmp_channel == XMC_CCU8_SLICE_COMPARE_CHANNEL_2))

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/**
 * Typedef for CCU8 Global data structure
 */
typedef CCU8_GLOBAL_TypeDef XMC_CCU8_MODULE_t;

/**
 * Typedef for CCU8 Slice data structure
 */
typedef CCU8_CC8_TypeDef XMC_CCU8_SLICE_t;

/**
 *  Return Value of an API
 */
typedef enum XMC_CCU8_STATUS
{
  XMC_CCU8_STATUS_OK      = 0U, /**< API fulfils request */
  XMC_CCU8_STATUS_ERROR       , /**< API cannot fulfil request */
  XMC_CCU8_STATUS_RUNNING     , /**< The timer slice is currently running */
  XMC_CCU8_STATUS_IDLE          /**< The timer slice is currently idle */
} XMC_CCU8_STATUS_t;

/**
 * CCU8 module clock
 */
typedef enum XMC_CCU8_CLOCK
{
  XMC_CCU8_CLOCK_SCU        = 0U, /**< Select the fCCU as the clock */
  XMC_CCU8_CLOCK_EXTERNAL_A     , /**< External clock-A */
  XMC_CCU8_CLOCK_EXTERNAL_B     , /**< External clock-B */
  XMC_CCU8_CLOCK_EXTERNAL_C     , /**< External clock-C */
} XMC_CCU8_CLOCK_t;

/**
 * CCU8 set the shadow transfer type for multichannel mode
 */
typedef enum XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER
{
  XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_SW_SLICE0       = (uint32_t)0x4000000, /**< Shadow transfer through software 
                                                                                     only for slice 0*/
  XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_SW_MCSS_SLICE0  = (uint32_t)0x4000400, /**< Shadow transfer through software 
                                                                                     and hardware for slice 0 */
  XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_SW_SLICE1       = (uint32_t)0x8000000, /**< Shadow transfer through software 
                                                                                     only for slice 1*/
  XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_SW_MCSS_SLICE1  = (uint32_t)0x8000800, /**< Shadow transfer through software 
                                                                                     and hardware for slice 1 */
  XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_SW_SLICE2       = (uint32_t)0x10000000, /**< Shadow transfer through software
                                                                                      only for slice 2 */
  XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_SW_MCSS_SLICE2  = (uint32_t)0x10001000, /**< Shadow transfer through software
                                                                                      and hardware for slice 2 */
  XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_SW_SLICE3       = (uint32_t)0x20000000, /**< Shadow transfer through software
                                                                                      only for slice 3*/
  XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_SW_MCSS_SLICE3  = (uint32_t)0x20002000, /**< Shadow transfer through software
                                                                                      and hardware for slice 3 */
} XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_t;

/**
 *  Operational modes of CCU8 slice
 */
typedef enum XMC_CCU8_SLICE_MODE
{
  XMC_CCU8_SLICE_MODE_COMPARE  = 0U, /**< slice(CC8y) operates in Compare Mode */
  XMC_CCU8_SLICE_MODE_CAPTURE      , /**< slice(CC8y) operates in Capture Mode */
} XMC_CCU8_SLICE_MODE_t;

/**
 * Slice Output selection
 */
typedef enum XMC_CCU8_SLICE_OUTPUT
{
  XMC_CCU8_SLICE_OUTPUT_0  = 1U, /**< Slice Output-0 */
  XMC_CCU8_SLICE_OUTPUT_1  = 2U, /**< Slice Output-1 */
  XMC_CCU8_SLICE_OUTPUT_2  = 4U, /**< Slice Output-2 */
  XMC_CCU8_SLICE_OUTPUT_3  = 8U  /**< Slice Output-3 */
} XMC_CCU8_SLICE_OUTPUT_t;

/**
 *  Timer counting modes for the slice
 */
typedef enum XMC_CCU8_SLICE_TIMER_COUNT_MODE
{
  XMC_CCU8_SLICE_TIMER_COUNT_MODE_EA  = 0U, /**< Edge Aligned Mode */
  XMC_CCU8_SLICE_TIMER_COUNT_MODE_CA      , /**< Centre Aligned Mode */
} XMC_CCU8_SLICE_TIMER_COUNT_MODE_t;

/**
 *  Timer repetition mode for the slice
 */
typedef enum XMC_CCU8_SLICE_TIMER_REPEAT_MODE
{
  XMC_CCU8_SLICE_TIMER_REPEAT_MODE_REPEAT = 0U, /**< Repetitive mode: continuous mode of operation */
  XMC_CCU8_SLICE_TIMER_REPEAT_MODE_SINGLE     , /**< Single shot mode: Once a Period match/One match
                                                     occurs timer goes to idle state */
} XMC_CCU8_SLICE_TIMER_REPEAT_MODE_t;

/**
 * Timer counting direction for the CCU8 slice
 */
typedef enum XMC_CCU8_SLICE_TIMER_COUNT_DIR
{
  XMC_CCU8_SLICE_TIMER_COUNT_DIR_UP   = 0U, /**< Counting up */
  XMC_CCU8_SLICE_TIMER_COUNT_DIR_DOWN     , /**< Counting down */
} XMC_CCU8_SLICE_TIMER_COUNT_DIR_t;

/**
 *  Capture mode register sets
 */
typedef enum XMC_CCU8_SLICE_CAP_REG_SET
{
  XMC_CCU8_SLICE_CAP_REG_SET_LOW  = 0U, /**< Capture register-0, Capture register-1 used */
  XMC_CCU8_SLICE_CAP_REG_SET_HIGH     , /**< Capture register-0, Capture register-1 used */
} XMC_CCU8_SLICE_CAP_REG_SET_t;

/**
 *  Prescaler mode
 */
typedef enum XMC_CCU8_SLICE_PRESCALER_MODE
{
  XMC_CCU8_SLICE_PRESCALER_MODE_NORMAL = 0U, /**< Fixed division of module clock */
  XMC_CCU8_SLICE_PRESCALER_MODE_FLOAT      , /**< Floating divider */
} XMC_CCU8_SLICE_PRESCALER_MODE_t;

/**
 *  Timer output passive level
 */
typedef enum XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL
{
  XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_LOW  = 0U, /**< Passive level = Low */
  XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH     , /**< Passive level = High */
} XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t;

/**
 *  Compare Channel selection
 */
typedef enum XMC_CCU8_SLICE_COMPARE_CHANNEL
{
  XMC_CCU8_SLICE_COMPARE_CHANNEL_1  = 0U, /**< Compare Channel-1 */
  XMC_CCU8_SLICE_COMPARE_CHANNEL_2      , /**< Compare Channel-2 */
} XMC_CCU8_SLICE_COMPARE_CHANNEL_t;

/**
 * Dead Time Generator Clock Divider
 */
typedef enum XMC_CCU8_SLICE_DTC_DIV
{
  XMC_CCU8_SLICE_DTC_DIV_1             = 0U, /**< DTC clock = Slice Clock */
  XMC_CCU8_SLICE_DTC_DIV_2                 , /**< DTC clock = Slice Clock/2 */
  XMC_CCU8_SLICE_DTC_DIV_4                 , /**< DTC clock = Slice Clock/4 */
  XMC_CCU8_SLICE_DTC_DIV_8                 , /**< DTC clock = Slice Clock/8 */
} XMC_CCU8_SLICE_DTC_DIV_t;

/**
 *  The compare channel output which is routed to the slice output signal(STy).
 */
typedef enum XMC_CCU8_SLICE_STATUS
{
  XMC_CCU8_SLICE_STATUS_CHANNEL_1        = 0U, /**< Channel-1 status connected to Slice Status */
  XMC_CCU8_SLICE_STATUS_CHANNEL_2            , /**< Channel-2 status connected to Slice Status */
  XMC_CCU8_SLICE_STATUS_CHANNEL_1_AND_2      , /**< <b>Wired And<\b> of Channel-1 and Channel-2 status connected to
                                                    Slice status */
#if(UC_SERIES == XMC13)
  XMC_CCU8_SLICE_STATUS_CHANNEL_1_OR_2         /**< <b>Wired OR<\b> of Channel-1 and Channel-2 status connected to Slice
                                                    status */                                         
#endif
} XMC_CCU8_SLICE_STATUS_t;

/**
 *  Compare channel for which modulation has to be applied
 */
typedef enum XMC_CCU8_SLICE_MODULATION_CHANNEL
{
  XMC_CCU8_SLICE_MODULATION_CHANNEL_NONE  = 0U, /**< No modulation */
  XMC_CCU8_SLICE_MODULATION_CHANNEL_1         , /**< Modulation for Compare Channel-1 */
  XMC_CCU8_SLICE_MODULATION_CHANNEL_2         , /**< Modulation for Compare Channel-2 */
  XMC_CCU8_SLICE_MODULATION_CHANNEL_1_AND_2     /**< Modulation for Compare Channel-1 and Compare Channel-2 */
} XMC_CCU8_SLICE_MODULATION_CHANNEL_t;

/**
 *  External Function list
 */
typedef enum XMC_CCU8_SLICE_FUNCTION
{
  XMC_CCU8_SLICE_FUNCTION_START           = 0U, /**< Start function */
  XMC_CCU8_SLICE_FUNCTION_STOP                , /**< Stop function */
  XMC_CCU8_SLICE_FUNCTION_CAPTURE_EVENT0      , /**< Capture Event-0 function, CCycapt0 signal is used for event 
                                                     generation */
  XMC_CCU8_SLICE_FUNCTION_CAPTURE_EVENT1      , /**< Capture Event-1 function, CCycapt1 signal is used for event 
                                                     generation */
  XMC_CCU8_SLICE_FUNCTION_GATING              , /**< Gating function */
  XMC_CCU8_SLICE_FUNCTION_DIRECTION           , /**< Direction function */
  XMC_CCU8_SLICE_FUNCTION_LOAD                , /**< Load function */
  XMC_CCU8_SLICE_FUNCTION_COUNT               , /**< Counting function */
  XMC_CCU8_SLICE_FUNCTION_OVERRIDE            , /**< Override function */
  XMC_CCU8_SLICE_FUNCTION_MODULATION          , /**< Modulation function */
  XMC_CCU8_SLICE_FUNCTION_TRAP                , /**< Trap function */
} XMC_CCU8_SLICE_FUNCTION_t;

/**
 *  External Event list
 */
typedef enum XMC_CCU8_SLICE_EVENT
{
  XMC_CCU8_SLICE_EVENT_NONE  = 0U, /**< None */
  XMC_CCU8_SLICE_EVENT_0         , /**< Event-0 */
  XMC_CCU8_SLICE_EVENT_1         , /**< Event-1 */
  XMC_CCU8_SLICE_EVENT_2         , /**< Event-2 */
} XMC_CCU8_SLICE_EVENT_t;

/**
 *  External Event trigger criteria  - Edge sensitivity
 */
typedef enum XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY
{
  XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_NONE          = 0U, /**< None */
  XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE       , /**< Rising Edge of the input signal generates
                                                                 event trigger */
  XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_FALLING_EDGE      , /**< Falling Edge of the input signal generates event
                                                                 trigger */
  XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_DUAL_EDGE         , /**< Both Rising and Falling edges cause an event trigger */
} XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_t;

/**
 *  External Event trigger criteria  - Level sensitivity
 */
typedef enum XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY
{
  XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH = 0U, /**< Level sensitive functions react to a high signal level*/
  XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_LOW      , /**< Level sensitive functions react to a low signal level */
} XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_t;

/**
 *  Low pass filter Configuration. The External Event input should be stable for a selected number of clock cycles.
 */
typedef enum XMC_CCU8_SLICE_EVENT_FILTER
{
  XMC_CCU8_SLICE_EVENT_FILTER_DISABLED  = 0U, /**< No Low Pass Filtering is applied */
  XMC_CCU8_SLICE_EVENT_FILTER_3_CYCLES      , /**< Input should be stable for 3 clock cycles */
  XMC_CCU8_SLICE_EVENT_FILTER_5_CYCLES      , /**< Input should be stable for 5 clock cycles */
  XMC_CCU8_SLICE_EVENT_FILTER_7_CYCLES      , /**< Input should be stable for 7 clock cycles */
} XMC_CCU8_SLICE_EVENT_FILTER_t;

/**
 *  External Event Input list. This list depicts the possible input connections to the CCU8 slice.
 *  Interconnects are specific to each device.
 */
typedef enum XMC_CCU8_SLICE_INPUT
{
  XMC_CCU8_SLICE_INPUT_A  = 0U, /**< Input-A */
  XMC_CCU8_SLICE_INPUT_B      , /**< Input-B */
  XMC_CCU8_SLICE_INPUT_C      , /**< Input-C */
  XMC_CCU8_SLICE_INPUT_D      , /**< Input-D */
  XMC_CCU8_SLICE_INPUT_E      , /**< Input-E */
  XMC_CCU8_SLICE_INPUT_F      , /**< Input-F */
  XMC_CCU8_SLICE_INPUT_G      , /**< Input-G */
  XMC_CCU8_SLICE_INPUT_H      , /**< Input-H */
  XMC_CCU8_SLICE_INPUT_I      , /**< Input-I */
  XMC_CCU8_SLICE_INPUT_J      , /**< Input-J */
  XMC_CCU8_SLICE_INPUT_K      , /**< Input-K */
  XMC_CCU8_SLICE_INPUT_L      , /**< Input-L */
  XMC_CCU8_SLICE_INPUT_M      , /**< Input-M */
  XMC_CCU8_SLICE_INPUT_N      , /**< Input-N */
  XMC_CCU8_SLICE_INPUT_O      , /**< Input-O */
  XMC_CCU8_SLICE_INPUT_P      , /**< Input-P */
} XMC_CCU8_SLICE_INPUT_t;

/**
 * Actions that can be performed upon detection of an external Timer STOP event
 */
typedef enum XMC_CCU8_SLICE_END_MODE
{
  XMC_CCU8_SLICE_END_MODE_TIMER_STOP       = 0U, /**< Stops the timer, without clearing TIMER register */
  XMC_CCU8_SLICE_END_MODE_TIMER_CLEAR          , /**< Without stopping timer, clears the TIMER register */
  XMC_CCU8_SLICE_END_MODE_TIMER_STOP_CLEAR     , /**< Stops the timer and clears the TIMER register */
} XMC_CCU8_SLICE_END_MODE_t;

/**
 *  Actions that can be performed upon detection of an external Timer START event
 */
typedef enum XMC_CCU8_SLICE_START_MODE
{
  XMC_CCU8_SLICE_START_MODE_TIMER_START       = 0U, /**< Start the timer from the current count of TIMER register */
  XMC_CCU8_SLICE_START_MODE_TIMER_START_CLEAR     , /**< Clears the TIMER register and start the timer */
} XMC_CCU8_SLICE_START_MODE_t;

/**
 *  Modulation of timer output signals
 */
typedef enum XMC_CCU8_SLICE_MODULATION_MODE
{
  XMC_CCU8_SLICE_MODULATION_MODE_CLEAR_ST_OUT  = 0U, /**< Clear ST and OUT signals */
  XMC_CCU8_SLICE_MODULATION_MODE_CLEAR_OUT         , /**< Clear only the OUT signal */
} XMC_CCU8_SLICE_MODULATION_MODE_t;

/**
 *  Trap exit mode
 */
typedef enum XMC_CCU8_SLICE_TRAP_EXIT_MODE
{
  XMC_CCU8_SLICE_TRAP_EXIT_MODE_AUTOMATIC = 0U, /**< Clear trap state as soon as the trap signal is de-asserted */
  XMC_CCU8_SLICE_TRAP_EXIT_MODE_SW            , /**< Clear only when acknowledged by software */
} XMC_CCU8_SLICE_TRAP_EXIT_MODE_t;

/**
 *  Timer clear on capture
 */
typedef enum XMC_CCU8_SLICE_TIMER_CLEAR_MODE
{
  XMC_CCU8_SLICE_TIMER_CLEAR_MODE_NEVER    = 0U, /**< Never clear the timer on any capture event */
  XMC_CCU8_SLICE_TIMER_CLEAR_MODE_CAP_HIGH     , /**< Clear only when timer value has been captured in C3V and C2V */
  XMC_CCU8_SLICE_TIMER_CLEAR_MODE_CAP_LOW      , /**< Clear only when timer value has been captured in C1V and C0V */
  XMC_CCU8_SLICE_TIMER_CLEAR_MODE_ALWAYS       , /**< Always clear the timer upon detection of any capture event */
} XMC_CCU8_SLICE_TIMER_CLEAR_MODE_t;

/**
 *  Multi Channel Shadow transfer request configuration options
 */
typedef enum XMC_CCU8_SLICE_MCMS_ACTION
{
  XMC_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR          = 0U, /**< Transfer Compare and Period Shadow register values to
                                                                the actual registers upon MCS xfer request */
  XMC_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP     = 1U, /**< Transfer Compare, Period and Prescaler Compare Shadow
                                                                register values to the actual registers upon MCS xfer 
                                                                request */
  XMC_CCU8_SLICE_MCMS_ACTION_TRANSFER_PR_CR_PCMP_DIT = 3U, /**< Transfer Compare, Period ,Prescaler Compare and Dither 
                                                                Compare register values to the actual registers upon 
                                                                MCS xfer request */
} XMC_CCU8_SLICE_MCMS_ACTION_t;

/**
 * Available Interrupt Event Ids
 */
typedef enum XMC_CCU8_SLICE_IRQ_ID
{
  XMC_CCU8_SLICE_IRQ_ID_PERIOD_MATCH            = 0U , /**< Period match counting up */
  XMC_CCU8_SLICE_IRQ_ID_ONE_MATCH               = 1U , /**< One match counting down */
  XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_1   = 2U , /**< Compare match counting up for channel 1 */
  XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_DOWN_CH_1 = 3U , /**< Compare match counting down for channel 1 */
  XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_UP_CH_2   = 4U , /**< Compare match counting up for channel 2 */
  XMC_CCU8_SLICE_IRQ_ID_COMPARE_MATCH_DOWN_CH_2 = 5U , /**< Compare match counting down for channel 2 */
  XMC_CCU8_SLICE_IRQ_ID_EVENT0                  = 8U , /**< Event-0 occurrence */
  XMC_CCU8_SLICE_IRQ_ID_EVENT1                  = 9U , /**< Event-1 occurrence */
  XMC_CCU8_SLICE_IRQ_ID_EVENT2                  = 10U, /**< Event-2 occurrence */
  XMC_CCU8_SLICE_IRQ_ID_TRAP                    = 11U, /**< Trap occurrence */
} XMC_CCU8_SLICE_IRQ_ID_t;

/**
 * Service Request Lines for CCU8. Event are mapped to these SR lines and these are used to generate the interrupt.
 */
typedef enum XMC_CCU8_SLICE_SR_ID
{
  XMC_CCU8_SLICE_SR_ID_0    = 0U, /**< Service Request Line-0 selected  */
  XMC_CCU8_SLICE_SR_ID_1        , /**< Service Request Line-1 selected  */
  XMC_CCU8_SLICE_SR_ID_2        , /**< Service Request Line-2 selected  */
  XMC_CCU8_SLICE_SR_ID_3        , /**< Service Request Line-3 selected  */
} XMC_CCU8_SLICE_SR_ID_t;


/**
 *  Slice shadow transfer options.
 */
typedef enum XMC_CCU8_SHADOW_TRANSFER
{
  XMC_CCU8_SHADOW_TRANSFER_SLICE_0            = CCU8_GCSS_S0SE_Msk,  /**< Transfer Period, Compare and Passive Level 
                                                                          shadow register values to actual registers for
                                                                          SLICE-0 */
  XMC_CCU8_SHADOW_TRANSFER_DITHER_SLICE_0     = CCU8_GCSS_S0DSE_Msk, /**< Transfer Dither compare shadow register value
                                                                          to actual register for SLICE-0 */
  XMC_CCU8_SHADOW_TRANSFER_PRESCALER_SLICE_0  = CCU8_GCSS_S0PSE_Msk, /**< Transfer Prescaler shadow register value to
                                                                          actual register for SLICE-0 */
  XMC_CCU8_SHADOW_TRANSFER_SLICE_1            = CCU8_GCSS_S1SE_Msk,  /**< Transfer Period, Compare and Passive Level 
                                                                          shadow register values to actual registers for
                                                                          SLICE-1 */
  XMC_CCU8_SHADOW_TRANSFER_DITHER_SLICE_1     = CCU8_GCSS_S1DSE_Msk, /**< Transfer Dither compare shadow register value
                                                                          to actual registers for SLICE-1 */
  XMC_CCU8_SHADOW_TRANSFER_PRESCALER_SLICE_1  = CCU8_GCSS_S1PSE_Msk, /**< Transfer Prescaler shadow register value to
                                                                          actual register for SLICE-1 */
  XMC_CCU8_SHADOW_TRANSFER_SLICE_2            = CCU8_GCSS_S2SE_Msk,  /**< Transfer Period, Compare and Passive Level 
                                                                          shadow register values to actual registers for
                                                                          SLICE-2 */
  XMC_CCU8_SHADOW_TRANSFER_DITHER_SLICE_2     = CCU8_GCSS_S2DSE_Msk, /**< Transfer Dither compare shadow register value
                                                                          to actual register for SLICE-2 */
  XMC_CCU8_SHADOW_TRANSFER_PRESCALER_SLICE_2  = CCU8_GCSS_S2PSE_Msk, /**< Transfer Prescaler shadow register value to
                                                                          actual register for SLICE-2 */
  XMC_CCU8_SHADOW_TRANSFER_SLICE_3            = CCU8_GCSS_S3SE_Msk,  /**< Transfer Period, Compare and Passive Level 
                                                                          shadow register values to actual registers for
                                                                          SLICE-3 */
  XMC_CCU8_SHADOW_TRANSFER_DITHER_SLICE_3     = CCU8_GCSS_S3DSE_Msk, /**< Transfer Dither compare shadow register value
                                                                          to actual register for SLICE-3 */
  XMC_CCU8_SHADOW_TRANSFER_PRESCALER_SLICE_3  = CCU8_GCSS_S3PSE_Msk  /**< Transfer Prescaler shadow register value to
                                                                          actual register for SLICE-3 */
} XMC_CCU8_SHADOW_TRANSFER_t;

/**
 *  Output selector for CCU8x.OUT0-3
 */
typedef enum XMC_CCU8_OUT_PATH
{
  XMC_CCU8_OUT_PATH_OUT0_ST1      = (uint32_t)0x20000, /**< CCU8yST1 signal path is connected the CCU8x.OUT0 */
  XMC_CCU8_OUT_PATH_OUT0_INV_ST1  = (uint32_t)0x20002, /**< Inverted CCU8yST1 signal path is connected the CCU8x.OUT0 */
  XMC_CCU8_OUT_PATH_OUT1_ST1      = (uint32_t)0x40000, /**< CCU8yST1 signal path is connected the CCU8x.OUT1 */
  XMC_CCU8_OUT_PATH_OUT1_INV_ST1  = (uint32_t)0x40004, /**< Inverted CCU8yST1 signal path is connected the CCU8x.OUT1 */
  XMC_CCU8_OUT_PATH_OUT2_ST2      = (uint32_t)0x80000, /**< CCU8yST2 signal path is connected the CCU8x.OUT2 */
  XMC_CCU8_OUT_PATH_OUT2_INV_ST2  = (uint32_t)0x80008, /**< Inverted CCU8yST2 signal path is connected the CCU8x.OUT2 */
  XMC_CCU8_OUT_PATH_OUT3_ST2      = (uint32_t)0x100000,/**< CCU8yST2 signal path is connected the CCU8x.OUT2 */
  XMC_CCU8_OUT_PATH_OUT3_INV_ST1  = (uint32_t)0x100010 /**< Inverted CCU8yST2 signal path is connected the CCU8x.OUT3 */
} XMC_CCU8_OUT_PATH_t;

/**
 *  CCU8 slice mask which can be used for the APIs as input where multi slice support is available
 */
typedef enum XMC_CCU8_SLICE_MASK
{
  XMC_CCU8_SLICE_MASK_SLICE_0  = 1U , /**< SLICE-0 */
  XMC_CCU8_SLICE_MASK_SLICE_1  = 2U , /**< SLICE-1 */
  XMC_CCU8_SLICE_MASK_SLICE_2  = 4U , /**< SLICE-2 */
  XMC_CCU8_SLICE_MASK_SLICE_3  = 8U   /**< SLICE-3 */
} XMC_CCU8_SLICE_MASK_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/**
 *  Configuration data structure of an External Event(Event-0/1/2).
 *  Needed to configure the various aspects of an External Event.
 *  This structure will not connect the external event with an external function.
 */
typedef struct XMC_CCU8_SLICE_EVENT_CONFIG
{
  XMC_CCU8_SLICE_INPUT_t                   mapped_input; /**< Required input signal for the Event. */
  XMC_CCU8_SLICE_EVENT_EDGE_SENSITIVITY_t  edge;         /**< Select the event edge of the input signal.
                                                              This is needed for an edge sensitive External function.*/
  XMC_CCU8_SLICE_EVENT_LEVEL_SENSITIVITY_t level;        /**< Select the event level of the input signal.
                                                              This is needed for an level sensitive External function.*/
  XMC_CCU8_SLICE_EVENT_FILTER_t            duration;     /**< Low Pass filter duration in terms of fCCU clock cycles. */
} XMC_CCU8_SLICE_EVENT_CONFIG_t;

/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__TASKING__)
  #pragma warning 586
#endif

/**
 * Dead Time configuration
 */
typedef struct XMC_CCU8_SLICE_DEAD_TIME_CONFIG
{
  union
  {
    struct
    {
      uint32_t enable_dead_time_channel1 : 1; /**< Enable dead time for Compare Channel-1 */
      uint32_t enable_dead_time_channel2 : 1; /**< Enable dead time for Compare Channel-2 */
      uint32_t channel1_st_path : 1;          /**< Should dead time be applied to ST output of Compare Channel-1? */
      uint32_t channel1_inv_st_path : 1;      /**< Should dead time be applied to inverse ST output of 
                                                   Compare Channel-1? */
      uint32_t channel2_st_path : 1;          /**< Should dead time be applied to ST output of Compare Channel-2? */
      uint32_t channel2_inv_st_path : 1;      /**< Should dead time be applied to inverse ST output of 
                                                   Compare Channel-2? */
      uint32_t div : 2;                       /**< Dead time prescaler divider value.
                                                   Accepts enum ::XMC_CCU8_SLICE_DTC_DIV_t*/
      uint32_t : 24;
    };
    uint32_t dtc;
  };
  union
  {
    struct
    {
      uint32_t channel1_st_rising_edge_counter : 8; /**< Contains the delay value that is applied to the rising edge
	                                                       for compare channel-1. Range: [0x0 to 0xFF] */
      uint32_t channel1_st_falling_edge_counter : 8; /**< Contains the delay value that is applied to the falling edge
	                                                        for compare channel-1. Range: [0x0 to 0xFF] */
      uint32_t : 16;
    };
    uint32_t dc1r;
  };
  union
  {
    struct
    {
      uint32_t channel2_st_rising_edge_counter : 8; /**< Contains the delay value that is applied to the rising edge
	                                                     for compare channel-2. Range: [0x0 to 0xFF]*/
      uint32_t channel2_st_falling_edge_counter : 8; /**< Contains the delay value that is applied to the falling edge
	                                                     for compare channel-2. Range: [0x0 to 0xFF]*/
      uint32_t : 16;
    };
    uint32_t dc2r;
  };
} XMC_CCU8_SLICE_DEAD_TIME_CONFIG_t;

/**
 *  Configuration data structure for CCU8 slice. Specifically configures the CCU8 slice to compare mode operation.
 *  This excludes event and function configuration.
 */
typedef struct XMC_CCU8_SLICE_COMPARE_CONFIG
{
  union
  {
    struct
    {
      uint32_t timer_mode : 1;         /**< Edge aligned or Centre Aligned.
                                            Accepts enum :: XMC_CCU8_SLICE_TIMER_COUNT_MODE_t */
      uint32_t monoshot : 1;           /**< Single shot or Continuous mode .
                                            Accepts enum :: XMC_CCU8_SLICE_TIMER_REPEAT_MODE_t */
      uint32_t shadow_xfer_clear : 1; /**< Should PR and CR shadow xfer happen when timer is cleared? */
      uint32_t : 10;
      uint32_t dither_timer_period: 1; /**< Can the period of the timer dither? */
      uint32_t dither_duty_cycle : 1;  /**< Can the compare match of the timer dither? */
      uint32_t : 1;
      uint32_t prescaler_mode: 1;      /**< Normal or floating prescaler mode.
                                            Accepts enum :: XMC_CCU8_SLICE_PRESCALER_MODE_t */
      uint32_t : 8;
      uint32_t mcm_ch1_enable : 1;     /**< Multi-Channel mode for compare channel 1 enable? */
      uint32_t mcm_ch2_enable : 1;     /**< Multi-Channel mode for compare channel 2 enable? */
      uint32_t : 2;
      uint32_t slice_status : 2;       /**< Which of the two channels drives the slice status output.
	                                          Accepts enum :: XMC_CCU8_SLICE_STATUS_t*/
      uint32_t : 1;
    };
    uint32_t tc;
  };
  union
  {
    struct
    {
      uint32_t passive_level_out0 : 1; /**< ST and OUT passive levels Configuration for OUT0. 
	                                          Accepts enum :: XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t */
      uint32_t passive_level_out1 : 1; /**< ST and OUT passive levels Configuration for OUT1. 
	                                          Accepts enum :: XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t */
      uint32_t passive_level_out2 : 1; /**< ST and OUT passive levels Configuration for OUT2. 
	                                          Accepts enum :: XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t */
      uint32_t passive_level_out3 : 1; /**< ST and OUT passive levels Configuration for OUT3. 
	                                          Accepts enum :: XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t */
      uint32_t : 28;
    };
    uint32_t psl;
  };
  union
  {
    struct
    {
      uint32_t asymmetric_pwm : 1;     /**< Should the PWM be a function of the 2 compare channels
                                            rather than period value? */
      uint32_t invert_out0 : 1;        /**< Should inverted ST of Channel-1 be connected to OUT0? */
      uint32_t invert_out1 : 1;        /**< Should inverted ST of Channel-1 be connected to OUT1? */
      uint32_t invert_out2 : 1;        /**< Should inverted ST of Channel-2 be connected to OUT2? */
      uint32_t invert_out3 : 1;        /**< Should inverted ST of Channel-2 be connected to OUT3? */
      uint32_t : 27;
    };
    uint32_t chc;
  };
  uint32_t prescaler_initval : 4;      /**< Initial prescaler divider value */
  uint32_t float_limit : 4;            /**< The max value which the prescaler divider can increment to.
                                            Range : [0 to 15] */
  uint32_t dither_limit : 4;           /**< The value that determines the spreading of dithering 
                                            Range : [0 to 15] */
  uint32_t timer_concatenation : 1;    /**< Enables the concatenation of the timer if true*/
} XMC_CCU8_SLICE_COMPARE_CONFIG_t;

/**
 *  Configuration data structure for CCU8 slice. Specifically configures the CCU8 slice to capture mode operation.
 *  This excludes event and function configuration.
 */
typedef struct XMC_CCU8_SLICE_CAPTURE_CONFIG
{
  union
  {
    struct
    {
      uint32_t : 4;
      uint32_t fifo_enable : 1;      /**< Should the capture registers be setup as a FIFO?(Extended capture mode) */
      uint32_t timer_clear_mode : 2; /**< How should the timer register be cleared upon detection of capture event?
                                          Accepts enum ::XMC_CCU8_SLICE_TIMER_CLEAR_MODE_t */
      uint32_t : 4;
      uint32_t same_event : 1;       /**< Should the capture event for C1V/C0V and C3V/C2V be same capture edge? */
      uint32_t ignore_full_flag : 1; /**< Should updates to capture registers follow full flag rules? */
      uint32_t : 3;
      uint32_t prescaler_mode: 1;    /**< Normal or floating prescaler Accepts enum :: XMC_CCU8_SLICE_PRESCALER_MODE_t*/
      uint32_t : 15;
    };
    uint32_t tc;
  };
  uint32_t prescaler_initval : 4;    /**< Prescaler divider value */
  uint32_t float_limit : 4;          /**< The max value which the prescaler divider can increment to */
  uint32_t timer_concatenation : 1;    /**< Enables the concatenation of the timer */
} XMC_CCU8_SLICE_CAPTURE_CONFIG_t;
/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif
/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param
 *   None <BR>
 * @return <BR>
 *   DRIVER_VERSION_t Driver version information.
 *
 * \par<b>Description:</b><br>
 *  Return version (major, minor and patch number) of the driver.\n\n
 *  The function is commonly used to check for user software compatibility with a specific version of the low level
 *  driver.
 * \par<b>Related APIs:</b><br>
 * None
 */
XMC_DRIVER_VERSION_t XMC_CCU8_GetDriverVersion(void);

/**
 * @param module Constant pointer to CCU8 module
 * @param mcs_action multi-channel shadow transfer request configuration
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Initialization of global register GCTRL.\n\n
 *  As part of module initialization, behaviour of the module upon detection
 *  Multi-Channel Mode trigger is configured. Will also invoke the XMC_CCU8_EnableModule().
 *  The API call would bring up the required CCU8 module and also initialize the module for
 *  the required multi-channel shadow transfer.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_SLICE_CompareInit()<BR> XMC_CCU8_SLICE_CaptureInit().
 */
void XMC_CCU8_Init(XMC_CCU8_MODULE_t *const module, const XMC_CCU8_SLICE_MCMS_ACTION_t mcs_action);

/**
 * @param module Constant pointer to CCU8 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the CCU8 module and brings it to active state.\n\n
 * Also disables the gating of the clock signal (if applicable depending on the device being selected).
 * Invoke this API before any operations are done on the CCU8 module. Invoked from  XMC_CCU8_Init().
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_SetModuleClock()<BR>  XMC_CCU8_DisableModule()<BR>  XMC_CCU8_StartPrescaler().
 */
void XMC_CCU8_EnableModule(XMC_CCU8_MODULE_t *const module);

/**
 * @param module Constant pointer to CCU8 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Brings the CCU8 module to reset state and enables gating of the clock signal(if applicable depending
 * on the device being selected).\n\n
 * Invoke this API when a CCU8 module needs to be disabled completely.
 * Any operation on the CCU8 module will have no effect after this API is called.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_EnableModule()<BR> XMC_CCU8_DisableModule().
 */
void XMC_CCU8_DisableModule(XMC_CCU8_MODULE_t *const module);

/**
 * @param module Constant pointer to CCU8 module
 * @param clock Choice of input clock to the module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Selects the Module Clock by configuring GCTRL.PCIS bits.\n\n
 *  There are 3 potential clock sources. This API helps to select the required clock source.
 *  Call to this API is valid after the XMC_CCU8_Init().
 *
 * \par<b>Related APIs:</b><br>
 * None.<BR>
 */
void XMC_CCU8_SetModuleClock(XMC_CCU8_MODULE_t *const module, const XMC_CCU8_CLOCK_t clock);


/**
 * @param module Constant pointer to CCU8 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Starts the prescaler and restores clocks to the timer slices, by setting GIDLC.SPRB bit.<br>\n
 *  Once the input to the prescaler has been chosen and the prescaler divider of all slices programmed,
 *  the prescaler itself may be started. Invoke this  API after XMC_CCU8_Init()
 *  (Mandatory to fully initialize the module).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_Init()<BR>  XMC_CCU8_EnableClock()<BR>  XMC_CCU8_DisableClock()<BR>  XMC_CCU8_StartPrescaler()<BR> 
 *  XMC_CCU8_StopPrescaler().
 */
__STATIC_INLINE void XMC_CCU8_StartPrescaler(XMC_CCU8_MODULE_t *const module)
{
  XMC_ASSERT("XMC_CCU8_StartPrescaler:Invalid Module Pointer", XMC_CCU8_CHECK_MODULE_PTR(module));
  module->GIDLC |= (uint32_t) CCU8_GIDLC_SPRB_Msk;
}

/**
 * @param module Constant pointer to CCU8 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Stops the prescaler and blocks clocks to the timer slices, by setting GIDLS.CPRB bit.\n\n
 * Clears the run bit of the prescaler. Ensures that the module clock is not supplied to
 * the slices of the module.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_EnableClock()<BR>  XMC_CCU8_DisableClock()<BR>  XMC_CCU8_StartPrescaler()<BR>  XMC_CCU8_StopPrescaler().
 */
__STATIC_INLINE void XMC_CCU8_StopPrescaler(XMC_CCU8_MODULE_t *const module)
{
  XMC_ASSERT("XMC_CCU8_StopPrescaler:Invalid Module Pointer", XMC_CCU8_CHECK_MODULE_PTR(module));
  module->GIDLS |= (uint32_t) CCU8_GIDLS_CPRB_Msk;
}

/**
 * @param module Constant pointer to CCU8 module
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Returns the state of the prescaler, by reading GSTAT.PRB bit.\n\n
 * If clock is being supplied to the slices of the  module then returns as true. 
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_StartPrescaler()<BR>  XMC_CCU8_StopPrescaler()<BR> XMC_CCU8_EnableClock()<BR>  XMC_CCU8_DisableClock().
 */
__STATIC_INLINE bool XMC_CCU8_IsPrescalerRunning(XMC_CCU8_MODULE_t *const module)
{
  XMC_ASSERT("XMC_CCU8_IsPrescalerRunning:Invalid Module Pointer", XMC_CCU8_CHECK_MODULE_PTR(module));
  return((bool)((module->GSTAT & (uint32_t) CCU8_GSTAT_PRB_Msk) == CCU8_GSTAT_PRB_Msk));
}

/**
 * @param module Constant pointer to CCU8 module
 * @param clock_mask Slices whose clocks are to be enabled simultaneously.
 *                   Bit location 0/1/2/3 represents slice-0/1/2/3 respectively. 
 *                   Range: [0x1 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables clocks of multiple slices at a time, by configuring GIDLC.CS0I, GIDLC.CS1I, GIDLC.CS2I, 
 * GIDLC.CS3I bits.\n\n
 * Takes an input clock_mask, which determines the slices that would receive the clock. Bring them out 
 * of the idle state simultaneously.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_EnableClock()<BR>  XMC_CCU8_DisableClock().
 */
__STATIC_INLINE void XMC_CCU8_EnableMultipleClocks(XMC_CCU8_MODULE_t *const module, const uint8_t clock_mask)
{
  XMC_ASSERT("XMC_CCU8_EnableMultipleClocks:Invalid Module Pointer", XMC_CCU8_CHECK_MODULE_PTR(module));
  XMC_ASSERT("XMC_CCU8_EnableMultipleClocks:Invalid clock mask", (clock_mask < 16U));

  module->GIDLC |= (uint32_t) clock_mask;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param compare_init Pointer to slice configuration structure
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Initialization of a CC8 slice to compare mode, by configuring CC8yTC, CC8yCMC, CC8yPSC, CC8yDITH, CC8yPSL, 
 * CC8yFPCS, CC8yCHC registers.\n\n
 * CC8 slice is configured with Timer configurations in this routine. Timer is stopped before initialization 
 * by calling XMC_CCU8_SLICE_StopTimer(). After initialization user has to explicitly enable
 * the shadow transfer for the required values by calling XMC_CCU8_EnableShadowTransfer() with appropriate mask.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
void XMC_CCU8_SLICE_CompareInit(XMC_CCU8_SLICE_t *const slice,
                                const XMC_CCU8_SLICE_COMPARE_CONFIG_t *const compare_init);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param capture_init Pointer to slice configuration structure
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Initialization of a CC8 slice to capture mode, by configuring CC8yTC, CC8yCMC, CC8yPSC,CC8yFPCS registers.\n\n
 * CC8 slice is configured with Capture configurations in  this routine. Timer is stopped before initialization
 * by calling XMC_CCU8_SLICE_StopTimer(). After initialization user has to explicitly enable the shadow transfer
 * for the required  values by calling XMC_CCU8_EnableShadowTransfer() with appropriate mask. 
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_SLICE_Capture0Config()<BR>  XMC_CCU8_SLICE_Capture1Config().
 */
void XMC_CCU8_SLICE_CaptureInit(XMC_CCU8_SLICE_t *const slice,
                                const XMC_CCU8_SLICE_CAPTURE_CONFIG_t *const capture_init);

/**
 * @param module Constant pointer to CCU8 module
 * @param slice_number Slice for which the clock should be Enabled.
 *                   Range: [0x0 to 0x3]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Enables the slice timer clock, by configuring GIDLC.CS0I, GIDLC.CS1I, GIDLC.CS2I, 
 *  GIDLC.CS3I bits according to the selected \a slice_number.\n\n
 *  It is possible to enable/disable clock at slice level. This uses the \b slice_number to indicate the
 *  slice whose clock needs to be enabled. Directly accessed register is GIDLC.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_DisableClock()<BR>  XMC_CCU8_EnableMultipleClocks()<BR>  XMC_CCU8_StartPrescaler()<BR>  XMC_CCU8_StopPrescaler().
 */
__STATIC_INLINE void XMC_CCU8_EnableClock(XMC_CCU8_MODULE_t *const module, const uint8_t slice_number)
{
  XMC_ASSERT("XMC_CCU8_EnableClock:Invalid Module Pointer", XMC_CCU8_CHECK_MODULE_PTR(module));
  XMC_ASSERT("XMC_CCU8_EnableClock:Invalid Slice Number", (slice_number < 4U));

  module->GIDLC |= ((uint32_t) 1 << slice_number);
}

/**
 * @param module Constant pointer to CCU8 module
 * @param slice_number Slice for which the clock should be disabled.
 *                   Range: [0x0 to 0x3]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the slice timer clock, by configuring GIDLS.SS0I, GIDLS.SSS1I, GIDLS.SSS2I, 
 * GIDLS.SSS3I bits according to the selected \a slice_number .\n\n
 * It is possible to disable clock at slice level using the module pointer.
 * \b slice_number is used to disable the clock to a given slice of the module.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_EnableClock()<BR>  XMC_CCU8_EnableMultipleClocks()<BR>  XMC_CCU8_StartPrescaler()<BR>  XMC_CCU8_StopPrescaler().
 */
__STATIC_INLINE void XMC_CCU8_DisableClock(XMC_CCU8_MODULE_t *const module, const uint8_t slice_number)
{
  XMC_ASSERT("XMC_CCU8_DisableClock:Invalid Module Pointer", XMC_CCU8_CHECK_MODULE_PTR(module));
  XMC_ASSERT("XMC_CCU8_DisableClock:Invalid Slice Number", (slice_number < 4U));

  module->GIDLS |= ((uint32_t) 1 << slice_number);
}

/**
 * @param module Constant pointer to CCU8 module
 * @param out_path_msk configuration for output path selection.
 *        combination of XMC_CCU8_OUT_PATH_t enum items can be used to create a mask.
 * 
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configure the out the path of the two compare channels with specified ST signal, by configuring the 
 ^ CC8yCHC register.\n\n
 * 
 * For the two compare channels it is possible to select either direct ST signal or inverted ST signal.
 * \b out_path_msk is used to set the required out put path.
 *
 * \par<b>Related APIs:</b><br>
 * None
*/
void XMC_CCU8_SLICE_SetOutPath(XMC_CCU8_SLICE_t *const slice, const uint32_t out_path_msk);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Map an External event to the External Start Function
 * @param start_mode Behaviour of slice when the start function is activated
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Configures the Start Function of the slice, by configuring CC8yCMC.ENDS and CC8yTC.ENDM bits.\n\n
 *  Start function is mapped with one of the 3 events. An external signal can control when a CC8 timer should start.
 *  Additionally, the behaviour of the slice upon activation of the start function is configured as well.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_SLICE_StopConfig()<BR>  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_StartConfig(XMC_CCU8_SLICE_t *const slice,
                                const XMC_CCU8_SLICE_EVENT_t event,
                                const XMC_CCU8_SLICE_START_MODE_t start_mode);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Map an External event to the External Stop Function
 * @param end_mode Behaviour of slice when the stop function is activated
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Stop function for the slice, by configuring CC8yCMC.STRTS and CC8yTC.STRM bits.\n\n
 * Stop function is mapped with one of the 3 events. An external signal can control when a CCU8 timer should stop.
 * Additionally, the behaviour of the slice upon activation of the stop function is configured as well.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_CCU8_SLICE_StartConfig()<BR>  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_StopConfig(XMC_CCU8_SLICE_t *const slice,
                               const XMC_CCU8_SLICE_EVENT_t event,
                               const XMC_CCU8_SLICE_END_MODE_t end_mode);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Map an External event to the External load Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Configures the Load Function for the slice, by configuring CC8yCMC.LDS bit.\n\n
 *  Load function is mapped with one of the 3 events. Up on occurrence of the event,\n
 *  if CC8yTCST.CDIR set to 0,CC8yTIMER register is reloaded with the value from compare channel 1 or 
 *                            compare channel 2\n
 *  if CC8yTCST.CDIR set to 1,CC8yTIMER register is reloaded with the value from period register\n
 *  
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_LoadConfig(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Map an External event to the External Modulation Function
 * @param mod_mode Desired Modulation mode
 * @param channel Specify the channel(s) on which the modulation should be applied.
 * @param synch_with_pwm Option to synchronize modulation with PWM start
 *                       Pass \b true if the modulation needs to be synchronized with PWM signal.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Output Modulation Function of the slice, by configuring CC8yCMC.MOS, CC8yTC.EMT and 
 * CC8yTC.EMS bits.\n\n
 * Modulation function is mapped with one of the 3 events. The output signal of the CCU can
 * be modulated according to a external input.  Additionally, the behaviour of the slice upon activation
 * of the modulation function is configured as well.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_ModulationConfig(XMC_CCU8_SLICE_t *const slice,
                                     const XMC_CCU8_SLICE_EVENT_t event,
                                     const XMC_CCU8_SLICE_MODULATION_MODE_t mod_mode,
                                     const XMC_CCU8_SLICE_MODULATION_CHANNEL_t channel, 
                                     const bool synch_with_pwm
                                     );

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Map an External event to the External Count Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Count Function of the slice, by configuring CC8yCMC.CNTS bit.\n\n
 * Count function is mapped with one of the 3 events. CCU8 slice can take an external
 * signal to act as the counting event. The CCU8 slice would count the
 * edges present on the \b event selected. 
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_CountConfig(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event  Map an External event to the External Gating Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Gating Function of the slice, by configuring CC8yCMC.GATES bit.\n\n
 * Gating function is mapped with one of the 3 events.  A CCU8 slice can use an input signal that would
 * operate as counter gating. If the configured Active level is detected the counter will gate all the pulses.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_GateConfig(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Map an External event to the Capture-0 Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Capture-0 Function of the slice, by configuring CC8yCMC.CAP0S bit.\n\n
 * Capture function is mapped with one of the 3 events.  A CCU8 slice can be configured into capture-0 mode
 * with the selected \b event. In this mode the CCU8 will capture the timer value into CC8yC0V and CC8yC1V.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_Capture0Config(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Map an External event to the Capture-1 Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Capture-1 Function of the slice, by configuring CC8yCMC.CAP1S bit.\n\n
 * Capture function is mapped with one of the 3 events. A CCU8 slice can be configured into capture-1
 * mode with the selected \b event. In this mode the CCU8 will capture the timer value into CC8yC2V and CC8yC3V.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_Capture1Config(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    bool would return true if the extended capture read back mode is enabled<BR>
 *
 * \par<b>Description:</b><br>
 * Checks if Extended capture mode read is enabled for particular slice or not, by reading CC8yTC.ECM bit.\n\n
 * In this mode the there is only one associated read address for all the capture registers.
 * Individual capture registers can still be accessed in this mode.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_GetCapturedValueFromFifo().
 */
__STATIC_INLINE bool XMC_CCU8_SLICE_IsExtendedCapReadEnabled(const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_IsPrescalerRunning:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return((bool)((slice->TC & (uint32_t) CCU8_CC8_TC_ECM_Msk) == (uint32_t)CCU8_CC8_TC_ECM_Msk));
}

#if UC_FAMILY == XMC4
/**
 * @param module Constant pointer to CCU8 module
 * @param slice_number to check whether read value belongs to required slice or not
 * @return <BR>
 *   int32_t   Returns -1 if the FIFO value being retrieved is not from the \b slice_number.
 *             Returns the value captured in the \b slice_number, if captured value is from the correct slice.
 *                     Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Read captured value from FIFO(ECRD register).\n\n
 * This is applicable only in the Capture mode of operation. The signal whose timing characteristics are to be measured 
 * must be mapped to an event which in turn must be mapped to the capture function. Based on the capture criteria, the 
 * instant timer values are captured into capture registers. Timing characteristics of the input signal may then be 
 * derived/inferred from the captured values.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_IsExtendedCapReadEnabled().
 */
int32_t XMC_CCU8_GetCapturedValueFromFifo(const XMC_CCU8_MODULE_t *const module, const uint8_t slice_number);
#else
/**
 * @param slice Constant pointer to CC8 Slice
 * @param set The capture register set from which the captured value is to be retrieved
 * @return <BR>
 *    uint32_t Returns the value captured in the \b slice_number
 *             Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Read captured value from FIFO(CC8yECRD0 and CC8yECRD1).\n\n
 * This is applicable only in the Capture mode of operation. The signal whose timing characteristics are to be measured 
 * must be mapped to an event which in turn must be mapped to the capture function. Based on the capture criteria, the 
 * instant timer values are captured into capture registers. Timing characteristics of the input signal may then be 
 * derived/inferred from the captured values.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_IsExtendedCapReadEnabled().
 */
uint32_t XMC_CCU8_SLICE_GetCapturedValueFromFifo(const XMC_CCU8_SLICE_t *const slice,
		                                             const XMC_CCU8_SLICE_CAP_REG_SET_t set);
#endif

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Map an External event to the External Count Direction Function
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Count Direction of the slice, by configuring CC8yCMC.UDS bit.\n\n
 * Count direction function is mapped with one of the 3 events. A slice can be configured to change the 
 * CC8yTIMER count direction depending on an external signal.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_DirectionConfig(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_EVENT_t event);

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the status bit override Function of the slice, by configuring CC8yCMC.OFS bit.\n\n
 * Status bit override function is mapped with one of the 3 events. A slice can be configured to change the
 * output of the timer's CC8yST1 signal depending on an external signal.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent().
 */
void XMC_CCU8_SLICE_StatusBitOverrideConfig(XMC_CCU8_SLICE_t *const slice);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param exit_mode How should a previously logged trap state be exited?
 * @param synch_with_pwm Should exit of trap state be synchronized with PWM cycle start?
 * @return <BR>
 *    None<BR>
 *
 *
 * \par<b>Description:</b><br>
 * Configures the Trap Function of the slice, by configuring CC8yCMC.TS, CC8yTC.TRPSE, and CC8yTC.TRPSW bits.\n\n
 * Trap function is mapped with Event-2. Criteria for exiting the trap state is configured.
 * This trap function allows PWM outputs to react on the state of an input pin.
 * Thus PWM output can be forced to inactive state upon detection of a trap.
 * It is also possible to synchronize the trap function with the PWM signal using the \b synch_with_pwm.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureEvent()<BR>  XMC_CCU8_SLICE_SetInput().
 */
void XMC_CCU8_SLICE_TrapConfig(XMC_CCU8_SLICE_t *const slice,
                               const XMC_CCU8_SLICE_TRAP_EXIT_MODE_t exit_mode,
                               bool synch_with_pwm);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param ev1_config Pointer to event 1 configuration data
 * @param ev2_config Pointer to event 2 configuration data
 * @return <BR>
 *    None<BR>
 *
 *
 * \par<b>Description:</b><br>
 * Map Status bit override function with an Event1 & Event 2 of the slice and configure CC8yINS register.\n\n
 * Details such as the input mapped to the event, event detection criteria and Low Pass filter options are programmed
 * by this routine for the events 1 & 2.  Event-1 input would be the trigger to override the value.
 * Event-2 input would be the override value.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_StatusBitOverrideConfig().
 */
void XMC_CCU8_SLICE_ConfigureStatusBitOverrideEvent(XMC_CCU8_SLICE_t *const slice,
                                                    const XMC_CCU8_SLICE_EVENT_CONFIG_t *const ev1_config,
                                                    const XMC_CCU8_SLICE_EVENT_CONFIG_t *const ev2_config);
/**
 * @param slice Constant pointer to CC8 Slice
 * @param event The External Event which needs to be configured.
 * @param config Pointer to event configuration data.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures an External Event of the slice, by updating CC8yINS register .\n\n
 * Details such as the input mapped to the event, event detection criteria and low pass filter
 * options are programmed by this routine. The Event \b config  will configure the input selection,
 * the edge selection, the level selection and the Low pass filter for the  event.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_StartConfig()<BR>  XMC_CCU8_SLICE_StopConfig()<BR>  XMC_CCU8_SLICE_LoadConfig()<BR> 
 *  XMC_CCU8_SLICE_ModulationConfig()<BR>  XMC_CCU8_SLICE_CountConfig()<BR>  XMC_CCU8_SLICE_GateConfig()<BR> 
 *  XMC_CCU8_SLICE_Capture0Config()<BR> XMC_CCU8_SLICE_Capture1Config()<BR>  XMC_CCU8_SLICE_DirectionConfig()<BR> 
 *  XMC_CCU8_SLICE_StatusBitOverrideConfig()<BR>  XMC_CCU8_SLICE_TrapConfig().
 */
void XMC_CCU8_SLICE_ConfigureEvent(XMC_CCU8_SLICE_t *const slice,
                                   const XMC_CCU8_SLICE_EVENT_t event,
                                   const XMC_CCU8_SLICE_EVENT_CONFIG_t *config);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event The External Event which needs to be configured.
 * @param input One of the 16 inputs meant to be mapped to the desired event
 * @return <BR>
 *    None<BR>
 *
 *
 * \par<b>Description:</b><br>
 * Selects an input for an external event, by configuring CC8yINS register.\n\n
 * It is possible to select one of the possible 16 input signals for a given Event.
 * This configures the CC8yINS.EVxIS for the selected event.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_StartConfig()<BR>  XMC_CCU8_SLICE_StopConfig()<BR>  XMC_CCU8_SLICE_LoadConfig()<BR> 
 *  XMC_CCU8_SLICE_ModulationConfig()<BR> XMC_CCU8_SLICE_CountConfig()<BR>  XMC_CCU8_SLICE_GateConfig()<BR> 
 *  XMC_CCU8_SLICE_Capture0Config()<BR>  XMC_CCU8_SLICE_Capture1Config()<BR> XMC_CCU8_SLICE_DirectionConfig()<BR> 
 *  XMC_CCU8_SLICE_StatusBitOverrideConfig()<BR>  XMC_CCU8_SLICE_TrapConfig().
 */
void XMC_CCU8_SLICE_SetInput(XMC_CCU8_SLICE_t *const slice,
                             const XMC_CCU8_SLICE_EVENT_t event,
                             const XMC_CCU8_SLICE_INPUT_t input);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param out_mask  Output signals for which the Trap function needs to be activated.
 *                   Use ::XMC_CCU8_SLICE_OUTPUT_t enum items to create a mask of choice,
 *                   using a bit wise OR operation
 *                   Range: [0x1 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the trap feature, by setting CC8yTC.TRAPE0, CC8yTC.TRAPE1, CC8yTC.TRAPE2 and CC8yTC.TRAPE3 bit based on the
 * \a out_mask.\n\n
 * A particularly useful feature where the PWM output can be forced inactive upon detection of a trap. The trap signal
 * can be the output of a sensing element which has just detected an abnormal electrical condition.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_TrapConfig()<BR>  XMC_CCU8_SLICE_DisableTrap()<BR>  XMC_CCU8_SLICE_ConfigureEvent()<BR> 
 *  XMC_CCU8_SLICE_SetInput().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_EnableTrap(XMC_CCU8_SLICE_t *const slice, const uint32_t out_mask)
{
  XMC_ASSERT("XMC_CCU8_SLICE_EnableTrap:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->TC |= (uint32_t)out_mask << CCU8_CC8_TC_TRAPE0_Pos;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param out_mask  Output signals for which the Trap function needs to be deactivated.
 *                   Use ::XMC_CCU8_SLICE_OUTPUT_t enum items to create a mask of choice,
 *                   using a bit wise OR operation.
 *                   Range: [0x1 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the trap feature, by clearing CC8yTC.TRAPE0, CC8yTC.TRAPE1, CC8yTC.TRAPE2 and CC8yTC.TRAPE3 bit based on the
 * \a out_mask.\n\n.\n\n
 * This API will revert the changes done by XMC_CCU8_SLICE_EnableTrap().
 * This Ensures that the TRAP function has no effect on the output of the CCU8 slice.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableTrap().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_DisableTrap(XMC_CCU8_SLICE_t *const slice, const uint32_t out_mask)
{
  XMC_ASSERT("XMC_CCU8_SLICE_DisableTrap:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  
  slice->TC &= ~((uint32_t)out_mask << CCU8_CC8_TC_TRAPE0_Pos);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    bool returns \b true if the Timer is running else it returns \b false.
 *
 * \par<b>Description:</b><br>
 * Returns the state of the timer (Either Running or stopped(idle)), by reading CC8yTCST.TRB bit.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_StartTimer()<BR>  XMC_CCU8_SLICE_StopTimer().
 */
__STATIC_INLINE bool XMC_CCU8_SLICE_IsTimerRunning(const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_GetTimerStatus:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return(bool)(((slice->TCST) & CCU8_CC8_TCST_TRB_Msk) == (uint32_t)CCU8_CC8_TCST_TRB_Msk);
}

/**
 * @param slice Pointer to an instance of CC8 slice
 * @return <BR>
 *    bool returns \b true if the dead time counter of Compare channel-1 is running else it returns \b false.
 *
 * \par<b>Description:</b><br>
 * Returns the state of the  Dead time counter 1 (Either Running or stopped(idle)), by reading CC8yTCST.DTR1 bit.
 * This returns the state of the dead time counter which is linked to Compare channel-1.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_IsDeadTimeCntr2Running().
 */
__STATIC_INLINE bool XMC_CCU8_SLICE_IsDeadTimeCntr1Running(const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_IsDeadTimeCntr1Running:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return(bool)(((slice->TCST) & CCU8_CC8_TCST_DTR1_Msk) == (uint32_t)CCU8_CC8_TCST_DTR1_Msk);
}

/**
 * @param slice Pointer to an instance of CC8 slice
 * @return <BR>
 *    bool returns \b true if the dead time counter of Compare channel-2 is running else it returns \b false.
 *
 * \par<b>Description:</b><br>
 * Returns the state of the  Dead time counter 2 (Either Running or stopped(idle)), by reading CC8yTCST.DTR2 bit.
 * This returns the state of the dead time counter which is linked to Compare channel-2.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_IsDeadTimeCntr1Running().
 */
__STATIC_INLINE bool XMC_CCU8_SLICE_IsDeadTimeCntr2Running(const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_IsDeadTimeCntr2Running:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return(bool)(((slice->TCST) & CCU8_CC8_TCST_DTR2_Msk) == (uint32_t)CCU8_CC8_TCST_DTR2_Msk);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *   ::XMC_CCU8_SLICE_TIMER_COUNT_DIR returns the direction in which the timer is counting.
 *
 * \par<b>Description:</b><br>
 * Returns the timer counting direction, by reading CC8yTCST.CDIR bit.\n\n
 * This API will return the direction in which the timer is currently
 * incrementing(XMC_CCU8_SLICE_TIMER_COUNT_DIR_UP) or decrementing (XMC_CCU8_SLICE_TIMER_COUNT_DIR_DOWN).
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE XMC_CCU8_SLICE_TIMER_COUNT_DIR_t XMC_CCU8_SLICE_GetCountingDir(const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_GetCountingDir:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return((XMC_CCU8_SLICE_TIMER_COUNT_DIR_t)(((slice->TCST) & CCU8_CC8_TCST_CDIR_Msk) >> CCU8_CC8_TCST_CDIR_Pos));
}
/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Starts the timer counting operation, by setting CC8yTCSET.TRBS bit.\n\n
 * It is necessary to have configured the CC8 slice before starting its timer.
 * Before the Timer is started ensure that the clock is provided to the slice.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_StopTimer().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_StartTimer(XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_StartTimer:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->TCSET = CCU8_CC8_TCSET_TRBS_Msk;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Stops the Timer.\n\n
 * Timer counting operation can be stopped by invoking this API, by setting CC8yTCCLR.TRBC bit.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_StartTimer().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_StopTimer(XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_StopTimer:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->TCCLR |= (uint32_t) CCU8_CC8_TCCLR_TRBC_Msk;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Resets the timer count to zero, by setting CC8yTCCLR.TCC bit.\n\n
 * A timer which has been stopped can still retain the last counted value.
 * After invoking this API the timer value will be cleared.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_StartTimer().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_ClearTimer(XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_ClearTimer:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->TCCLR |= (uint32_t) CCU8_CC8_TCCLR_TCC_Msk;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    ::XMC_CCU8_SLICE_MODE returns XMC_CCU8_SLICE_MODE_COMPARE if the slice is operating in compare mode
 *                          returns XMC_CCU8_SLICE_MODE_CAPTURE if the slice is operating in capture mode
 *
 * \par<b>Description:</b><br>
 * Retrieves the current mode of operation in the slice (either Capture mode or Compare mode), by reading 
 * CC8yTC.CMOD bit.\n\n
 * Ensure that before invoking this API the CCU8 slice should be configured otherwise the output of this API is
 * invalid.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
__STATIC_INLINE XMC_CCU8_SLICE_MODE_t XMC_CCU8_SLICE_GetSliceMode(const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_GetSliceMode:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return((XMC_CCU8_SLICE_MODE_t)(((slice->TC) & CCU8_CC8_TC_CMOD_Msk) >> CCU8_CC8_TC_CMOD_Pos));
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param mode Desired repetition mode (Either single shot or Continuous)
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the Timer to either Single shot mode or continuous mode, by configuring CC8yTC.TSSM bit.\n\n
 * The timer will count up to the terminal count as specified in the period register and stops immediately if the repeat
 * mode has been set to single shot. In the continuous mode of operation, the timer starts counting all over again after
 * reaching the terminal count.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_GetTimerRepeatMode().
 */
void XMC_CCU8_SLICE_SetTimerRepeatMode(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_TIMER_REPEAT_MODE_t mode);

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <br>
 *  ::XMC_CCU8_SLICE_TIMER_REPEAT_MODE returns XMC_CCU8_SLICE_TIMER_REPEAT_MODE_REPEAT if continuous mode is selected
 *                                     returns XMC_CCU8_SLICE_TIMER_REPEAT_MODE_SINGLE if single shot mode is selected
 *
 * \par<b>Description:</b><br>
 * Retrieves the Timer repeat mode, either Single shot mode or continuous mode, by reading CC8yTC.TSSM bit.\n\n
 * The timer will count upto the terminal count as specified in the period register and stops immediately if the repeat
 * mode has been set to single shot mode. In the continuous mode of operation, the timer starts counting
 * all over again after reaching the terminal count.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetTimerRepeatMode().
 */
__STATIC_INLINE XMC_CCU8_SLICE_TIMER_REPEAT_MODE_t XMC_CCU8_SLICE_GetTimerRepeatMode(
                                                                                    const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_GetTimerRepeatMode:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return((XMC_CCU8_SLICE_TIMER_REPEAT_MODE_t)(((slice->TC) & CCU8_CC8_TC_TSSM_Msk) >> CCU8_CC8_TC_TSSM_Pos));
}
/**
 * @param slice Constant pointer to CC8 Slice
 * @param mode Desired counting mode (Either Edge Aligned or Center Aligned)
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the timer counting mode either Edge Aligned or Center Aligned, by configuring CC8yTC.TCM bit.\n\n
 * In the edge aligned mode, the timer counts from 0 to the terminal count. Once the timer count has reached a preset
 * compare value, the timer status output asserts itself. It will now deassert only after the timer count reaches the
 * terminal count.\n In the center aligned mode, the timer first counts from 0 to the terminal count and then back to 0.
 * During this upward and downward counting, the timer status output stays asserted as long as the timer value is 
 * greater than the compare value.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_GetTimerCountingMode().
 */
void XMC_CCU8_SLICE_SetTimerCountingMode(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_TIMER_COUNT_MODE_t mode);

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <br>
 *  ::XMC_CCU8_SLICE_TIMER_COUNT_MODE returns XMC_CCU8_SLICE_TIMER_COUNT_MODE_EA if edge aligned mode is selected
 *                                     returns XMC_CCU8_SLICE_TIMER_COUNT_MODE_CA if center aligned mode is selected
 *
 * \par<b>Description:</b><br>
 * Retrieves timer counting mode either Edge aligned or Center Aligned, by reading CC8yTC.TCM bit.\n\n
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetTimerCountingMode().
 */
__STATIC_INLINE XMC_CCU8_SLICE_TIMER_COUNT_MODE_t XMC_CCU8_SLICE_GetTimerCountingMode(
		                                                                            const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_GetTimerCountingMode:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return((XMC_CCU8_SLICE_TIMER_COUNT_MODE_t)(((slice->TC) & CCU8_CC8_TC_TCM_Msk) >> CCU8_CC8_TC_TCM_Pos));
}
/**
 * @param slice Constant pointer to CC8 Slice
 * @param period_val Timer period value
 *        Range: [0x0 to 0xFFFF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Programs the timer period, by writing CC8yPRS register.\n\n
 * The frequency of counting/ PWM frequency is determined by this value. The period value is written to a shadow 
 * register. Explicitly enable the shadow transfer for the the period value by calling
 * XMC_CCU8_EnableShadowTransfer() with appropriate mask. If shadow transfer is enabled and the timer is running,
 * a period match transfers the value from the shadow register to the actual period register.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_GetTimerPeriodMatch().
 */
void XMC_CCU8_SLICE_SetTimerPeriodMatch(XMC_CCU8_SLICE_t *const slice, const uint16_t period_val);

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *   uint16_t returns the current timer period value
 *            Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves the timer period value currently effective, by reading CC8yPR register.\n\n
 * If the timer is active then the value being returned is currently being used for the PWM period.
 *
 * \par<b>Note:</b><br>
 * The XMC_CCU8_SLICE_SetTimerPeriodMatch() would set the new period value to a shadow register.
 * This would only transfer the new values into the actual period register if the shadow transfer request
 * is enabled and if a period match occurs. Hence a consecutive call to XMC_CCU8_SLICE_GetTimerPeriodMatch()
 * would not reflect the new values until the shadow transfer completes.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetTimerPeriodMatch().
 */
__STATIC_INLINE uint16_t XMC_CCU8_SLICE_GetTimerPeriodMatch(const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_GetTimerPeriodMatch:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return((uint16_t) slice->PR);
}
/**
 * @param slice Constant pointer to CC8 Slice
 * @param channel Select the compare channel to which the \b compare_val has to programmed.
 * @param compare_val Timer compare value
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Programs the timer compare value, by writing CC8yCR1S and CC8yCR2S registers.\n\n
 *  The PWM duty cycle is determined by this value.\n\n
 * The compare value is written to a shadow register. Explicitly enable the shadow transfer for
 * the the period/compare value by calling XMC_CCU8_EnableShadowTransfer() with
 * appropriate mask.If shadow transfer is enabled and the timer is running,
 * a period match transfers the value from the shadow register to the actual compare register.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_GetTimerPeriodMatch().
 */
void XMC_CCU8_SLICE_SetTimerCompareMatch(XMC_CCU8_SLICE_t *const slice,
                                         const XMC_CCU8_SLICE_COMPARE_CHANNEL_t channel,
                                         const uint16_t compare_val);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param channel Select the compare channel to retrieve from.
 * @return <BR>
 *   uint16_t returns the current timer compare value
 *            Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves the timer compare value currently effective, by reading CC8yCR1S and CC8yCR2S registers.\n\n
 * If the timer is active then the value being returned is currently being for the PWM duty cycle( timer compare value).
 *
 * \par<b>Note:</b><br>
 * The XMC_CCU8_SLICE_SetTimerCompareMatch() would set the new compare value to a shadow register.
 * This would only transfer the new values into the actual compare register if the shadow transfer request
 * is enabled and if a period match occurs. Hence a consecutive call to XMC_CCU8_SLICE_GetTimerCompareMatch()
 * would not reflect the new values until the shadow transfer completes.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetTimerCompareMatch().
 */
uint16_t XMC_CCU8_SLICE_GetTimerCompareMatch(const XMC_CCU8_SLICE_t *const slice,
                                             const XMC_CCU8_SLICE_COMPARE_CHANNEL_t channel);

/**
 * @param module Constant pointer to CCU8 module
 * @param shadow_transfer_msk Shadow transfer request mask for various transfers.
 *                            Use ::XMC_CCU8_SHADOW_TRANSFER_t enum items to create a mask of choice,
 *                            using a bit wise OR operation.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Requests of shadow transfer for Period, Compare, Passive level, dither and prescaler, by configuring 
 * the GCSS register.\n\n
 * The transfer from the shadow registers to the actual registers is done in the immediate next occurrence of the
 * shadow transfer trigger after the API is called.
 *
 * Any call to XMC_CCU8_SLICE_SetTimerPeriodMatch()<BR>  XMC_CCU8_SLICE_SetTimerCompareMatch()<BR> 
 * XMC_XMC_CCU8_SLICE_SetPrescaler()<BR> XMC_CCU8_SLICE_CompareInit()<BR> XMC_CCU8_SLICE_CaptureInit().
 * must be succeeded by this API.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void XMC_CCU8_EnableShadowTransfer(XMC_CCU8_MODULE_t *const module, const uint32_t shadow_transfer_msk);

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *   uint16_t returns the current timer value
 *            Range: [0x0 to 0xFFFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves the latest timer value, from CC8yTIMER register.\n\n
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetTimerValue().
 */
__STATIC_INLINE uint16_t XMC_CCU8_SLICE_GetTimerValue(const XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_GetTimerValue:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  return((uint16_t) slice->TIMER);
}
/**
 * @param slice Constant pointer to CC8 Slice
 * @param timer_val The new timer value that has to be loaded into the TIMER register.
 *                  Range: [0x0 to 0xFFFF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Loads a new timer value, by setting CC8yTIMER register.\n\n
 * 
 * \par<b>Note:</b><br>
 *  Request to load is ignored if the timer is running.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_GetTimerValue().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_SetTimerValue(XMC_CCU8_SLICE_t *const slice, const uint16_t timer_val)
{
  XMC_ASSERT("XMC_CCU8_SLICE_SetTimerValue:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->TIMER = (uint32_t) timer_val;
}
/**
 * @param slice Constant pointer to CC8 Slice
 * @param period_dither Boolean instruction on dithering of period match
 * @param duty_dither Boolean instruction on dithering of compare match
 * @param spread Dither compare value
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables dithering of PWM frequency and duty cycle, by configuring CC8yTC.DITHE and CC8yDITS bits.\n\n
 * Some control loops are slow in updating PWM frequency and duty cycle. In such a case, a Bresenham style dithering
 * can help reduce long term errors. Dithering can be applied to period and duty individually,
 * this can be selected using the parameter \b period_dither and \b duty_dither.
 * The \b spread would provide the dither compare value. If the dither counter value is less than this \b spread then 
 * the period/compare values would be dithered according to the dither mode selected. This API would invoke 
 * XMC_CCU8_SLICE_SetDitherCompareValue().
 *
 * \par<b>Note:</b><br>
 * After this API call, XMC_CCU8_EnableShadowTransfer() has to be called with appropriate mask
 * to transfer the dither value.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_DisableDithering().
 */
void XMC_CCU8_SLICE_EnableDithering(XMC_CCU8_SLICE_t *const slice,
                                    const bool period_dither,
                                    const bool duty_dither,
                                    const uint8_t spread);

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables dithering of PWM frequency and duty cycle, by clearing CC8yTC.DITHE bits.\n\n
 * This disables the Dither mode that was set in XMC_CCU8_SLICE_EnableDithering().
 * This API will not clear the dither compare value.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableDithering().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_DisableDithering(XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_DisableDithering:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->TC &= ~((uint32_t) CCU8_CC8_TC_DITHE_Msk);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the floating prescaler, by setting CC8yTC.FPE bit.\n\n
 * The prescaler divider starts with an initial value and increments upon every period match. It keeps incrementing
 * until a ceiling (prescaler compare value) is hit and thereafter rolls back to the original prescaler divider value.\n
 * It is necessary to have programmed an initial divider value and a compare value before the feature is enabled.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetFloatingPrescalerCompareValue()<BR>  XMC_CCU8_SLICE_DisableFloatingPrescaler()<BR> 
 *  XMC_XMC_CCU8_SLICE_SetPrescaler().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_EnableFloatingPrescaler(XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_EnableFloatingPrescaler:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->TC |= (uint32_t) CCU8_CC8_TC_FPE_Msk;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the floating prescaler, by clearing CC8yTC.FPE bit.\n\n
 * This would return the prescaler to the normal mode.
 * The prescaler that would be applied is the value present in CC8yPSC.
 *
  * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableFloatingPrescaler().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_DisableFloatingPrescaler(XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_DisableFloatingPrescaler:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->TC &= ~((uint32_t) CCU8_CC8_TC_FPE_Msk);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param comp_val Dither compare value
 *                 Range: [0x0 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Sets the dither spread/compare value, by setting CC8yDITS.DCVS bits.\n\n
 * This value is the cornerstone of PWM dithering feature. Dithering is applied/done when the value in the
 * dithering counter is less than this compare/spread value. For all dithering counter values greater than
 * the spread value, there is no dithering. After setting the value XMC_CCU8_EnableShadowTransfer() has to be
 * called with appropriate mask.
 *
  * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableDithering().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_SetDitherCompareValue(XMC_CCU8_SLICE_t *const slice, const uint8_t comp_val)
{
  XMC_ASSERT("XMC_CCU8_SLICE_SetDitherCompareValue:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetDitherCompareValue:Invalid Dither compare value", (comp_val <= 15U));

  slice->DITS = comp_val;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param div_val Prescaler divider value
 *                 Range: [0x0 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Programs the slice specific prescaler divider, by configuring the CC8yPSC and CC8yFPC registers.\n\n
 * The prescaler divider may only be programmed after the prescaler run bit has been cleared
 * by calling XMC_CCU8_StopPrescaler().
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetFloatingPrescalerCompareValue().
 */
void XMC_CCU8_SLICE_SetPrescaler(XMC_CCU8_SLICE_t *const slice, const uint8_t div_val);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param cmp_val Prescaler divider compare value
 *                 Range: [0x0 to 0xF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Programs the slice specific prescaler divider compare value, by configuring CC8yFPCS register.\n\n
 * The compare value is applicable only in floating mode of operation. The prescaler divider starts with an initial
 * value and increments to the compare value steadily upon every period match. Once prescaler divider
 * equals the prescaler divider compare value, the value in the former resets back to the PVAL (from FPC). After setting 
 * the value, XMC_CCU8_EnableShadowTransfer() has to be called with appropriate mask.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetPrescaler().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_SetFloatingPrescalerCompareValue(XMC_CCU8_SLICE_t *const slice,
                                                                     const uint8_t cmp_val)
{
  XMC_ASSERT("XMC_CCU8_SLICE_SetFloatingPrescalerCompareValue:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  /* First, write to the shadow register */
  slice->FPCS = (uint32_t) cmp_val;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param ch_num Compare channel for which the multi-channel mode is needed.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the multichannel mode, by setting CC8yTC.MCME1 or CC8yTC.MCME1 bits based on the \a ch_num.\n\n
 * The output state of the Timer slices can be controlled in parallel by a single input signal.
 * A particularly useful feature in motor control applications where the PWM output of multiple slices of a module can
 * be gated and ungated by multi-channel gating inputs connected to the slices. A peripheral like POSIF connected to the
 * motor knows exactly which of the power drive switches are to be turned on and off at any instant. It can thus through
 * a gating bus (known as multi-channel inputs) control which of the slices output stays gated/ungated.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_DisableMultiChannelMode()<BR>  XMC_CCU8_SetMultiChannelShadowTransferMode().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_EnableMultiChannelMode(XMC_CCU8_SLICE_t *const slice,
                                                           const XMC_CCU8_SLICE_COMPARE_CHANNEL_t ch_num)
{
  XMC_ASSERT("XMC_CCU8_SLICE_EnableMultiChannelMode:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_EnableMultiChannelMode:Invalid Channel", XMC_CCU8_SLICE_CHECK_COMP_CHANNEL(ch_num));

  slice->TC |= (uint32_t)CCU8_CC8_TC_MCME1_Msk << ch_num;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param ch_num Compare channel for which the multi-channel mode needs to be disabled.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the multichannel mode, by clearing CC8yTC.MCME1 or CC8yTC.MCME1 bits based on the \a ch_num.\n\n
 * Returns the slices to the normal operation mode. This takes the slice number as input and
 * configures the multi channel mode for it.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableMultiChannelMode().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_DisableMultiChannelMode(XMC_CCU8_SLICE_t *const slice,
                                                            const XMC_CCU8_SLICE_COMPARE_CHANNEL_t ch_num)
{
  XMC_ASSERT("XMC_CCU8_SLICE_DisableMultiChannelMode:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_DisableMultiChannelMode:Invalid Channel", XMC_CCU8_SLICE_CHECK_COMP_CHANNEL(ch_num));

  slice->TC &= ~((uint32_t)CCU8_CC8_TC_MCME1_Msk << ch_num);
}

/**
 * @param module Constant pointer to CCU8 module
 * @param slice_mode_msk Slices for which the configuration has to be applied.
 *                       Use ::XMC_CCU8_MULTI_CHANNEL_SHADOW_TRANSFER_t enum items to create a mask of choice,
 *                             using a bit wise OR operation.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the Multi-channel shadow transfer request trigger signal either by software or hardware by configuring 
 * GCTRL.MSE0, GCTRL.MSE1, GCTRL.MSE2, and GCTRL.MSE3 based on the mask.\n\n
 * The shadow transfer would take place either if it was requested by software or by the CCU8x.MCSS input.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
*/
void XMC_CCU8_SetMultiChannelShadowTransferMode(XMC_CCU8_MODULE_t *const module, const uint32_t slice_mode_msk);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param reg_num The capture register from which the captured value is to be retrieved
 *                Range: [0,3]
 * @return <BR>
 *    uint32_t  Returns the Capture register value.
 *              Range: [0 to 0x1FFFFF]
 *
 * \par<b>Description:</b><br>
 * Retrieves timer value which has been captured in the Capture registers, by reading CC8yCV[\b reg_num] register.\n\n
 * The signal whose timing characteristics are to be measured must be mapped to an event which in turn must be mapped 
 * to the capture function. Based on the capture criteria, the timer values are captured into capture registers. Timing
 * characteristics of the input signal may then be derived/inferred from the captured values. The full flag will help 
 * to find out if there is a new captured value present.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_GetLastCapturedTimerValue().
 */
uint32_t XMC_CCU8_SLICE_GetCaptureRegisterValue(const XMC_CCU8_SLICE_t *const slice, const uint8_t reg_num);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param set  The capture register set, which must be evaluated
 * @param val_ptr Out Parameter of the API.Stores the captured timer value into this out parameter.
 * @return <BR>
 *  ::XMC_CCU8_STATUS Returns XMC_CCU8_STATUS_OK if there was new value present in the capture registers.
 *                   returns XMC_CCU8_STATUS_ERROR if there was no new value present in the capture registers.
 *
 * \par<b>Description:</b><br>
 * Retrieves the latest captured timer value, by reading CC8yCV registers.\n\n
 * Retrieve the timer value last stored by the slice. When separate capture events are used,
 * users must specify the capture set to evaluate. If single capture event mode is used, all 8 capture registers are
 * evaluated.\n
 * The lowest register is evaluated first followed by the next higher ordered register and this continues until all
 * capture registers have been evaluated.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_GetCaptureregisterValue().
 */
XMC_CCU8_STATUS_t XMC_CCU8_SLICE_GetLastCapturedTimerValue(const XMC_CCU8_SLICE_t *const slice,
                                                           const XMC_CCU8_SLICE_CAP_REG_SET_t set,
                                                           uint32_t *val_ptr);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Event whose assertion can potentially lead to an interrupt
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the generation of an interrupt pulse for the event, by configuring CC8yINTE register.\n\n
 * For an event to lead to an interrupt, it must first be enabled and bound to a service request line. The corresponding
 * NVIC node must be enabled as well. This API merely enables the event. Binding with SR is performed by another API.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetInterruptNode()<BR>  XMC_CCU8_SLICE_EnableMultipleEvents()<BR>  XMC_CCU8_SLICE_DisableEvent()<BR> 
 *  XMC_CCU8_SLICE_DisableMultipleEvents().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_EnableEvent(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_IRQ_ID_t event)
{
  XMC_ASSERT("XMC_CCU8_SLICE_EnableEvent:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_EnableEvent:Invalid SR event", XMC_CCU8_SLICE_CHECK_INTERRUPT(event));

  slice->INTE |= ((uint32_t) 1) << ((uint32_t) event);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param mask Event mask such that multiple events can be enabled.
 *                  Use ::XMC_CCU8_SLICE_IRQ_ID_t enum items to create a mask of choice,
 *                  using a bit wise OR operation.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the generation of an interrupt pulse for the required events, by configuring CC8yINTE register.\n\n
 * For an event to lead to an interrupt, it must first be enabled and bound to a service request line. The corresponding
 * NVIC node must be enabled as well. This API merely enables the events. Binding with SR is performed by another API.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetInterruptNode()<BR>  XMC_CCU8_SLICE_EnableEvent()<BR>  XMC_CCU8_SLICE_DisableEvent()<BR> 
 *  XMC_CCU8_SLICE_DisableMultipleEvents().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_EnableMultipleEvents(XMC_CCU8_SLICE_t *const slice, const uint16_t mask)
{
  XMC_ASSERT("XMC_CCU8_SLICE_EnableMultipleEvents:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->INTE = (uint32_t) mask;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Event whose assertion can potentially lead to an interrupt
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the generation of an interrupt pulse for the event, by clearing CC8yINTE register.\n\n
 * Prevents the event from being asserted.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetInterruptNode()<BR>  XMC_CCU8_SLICE_EnableEvent()<BR>  XMC_CCU8_SLICE_EnableMultipleEvents()<BR> 
 *  XMC_CCU8_SLICE_DisableMultipleEvents().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_DisableEvent(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_IRQ_ID_t event)
{
  XMC_ASSERT("XMC_CCU8_SLICE_DisableEvent:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_DisableEvent:Invalid SR event", XMC_CCU8_SLICE_CHECK_INTERRUPT(event));

  slice->INTE &= ~(((uint32_t) 1) << ((uint32_t) event));
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param mask Event mask such that multiple events can be enabled.
 *             Use ::XMC_CCU8_SLICE_IRQ_ID_t enum items to create a mask of choice,
 *             using a bit wise OR operation.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Disables the generation of an interrupt pulse for the required events, by clearing CC8yINTE register.\n\n
 *  Prevents selected events of the slice from being asserted.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetInterruptNode()<BR>  XMC_CCU8_SLICE_EnableEvent()<BR>  XMC_CCU8_SLICE_EnableMultipleEvents()<BR> 
 *  XMC_CCU8_SLICE_DisableEvent().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_DisableMultipleEvents(XMC_CCU8_SLICE_t *const slice, const uint16_t mask)
{
  XMC_ASSERT("XMC_CCU8_SLICE_DisableMultipleEvents:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->INTE &= ~((uint32_t) mask);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Event whose assertion can potentially lead to an interrupt
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Manually asserts the requested event, by setting CC8ySWS register.\n\n
 * For an event to lead to an interrupt, it must first be enabled and bound to a service request line. The corresponding
 * NVIC node must be enabled as well. This API manually asserts the requested event.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_SetInterruptNode()<BR>  XMC_CCU8_SLICE_EnableEvent()<BR>  XMC_CCU8_SLICE_EnableMultipleEvents().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_SetEvent(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_IRQ_ID_t event)
{
  XMC_ASSERT("XMC_CCU8_SLICE_SetEvent:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_SetEvent:Invalid SR event", XMC_CCU8_SLICE_CHECK_INTERRUPT(event));

  slice->SWS |= ((uint32_t) 1) << ((uint32_t) event);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Asserted event which must be acknowledged.
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Acknowledges an asserted event, by setting CC8ySWR with respective event flag.\n\n
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableEvent()<BR>  XMC_CCU8_SLICE_SetEvent()<BR>  XMC_CCU8_SLICE_GetEvent().
 *
 */
__STATIC_INLINE void XMC_CCU8_SLICE_ClearEvent(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_IRQ_ID_t event)
{
  XMC_ASSERT("XMC_CCU8_SLICE_ClearEvent:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  XMC_ASSERT("XMC_CCU8_SLICE_ClearEvent:Invalid SR event", XMC_CCU8_SLICE_CHECK_INTERRUPT(event));
  slice->SWR |= ((uint32_t) 1) << ((uint32_t) event);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Event to be evaluated for assertion
 * @return <br>
 *         bool  Returns true if event is set else false is returned.
 *
 * \par<b>Description:</b><br>
 * Evaluates if a given event is asserted or not, by reading CC8yINTS register.\n\n
 * Return true if the event is asserted. For a event to be asserted it has to be
 * first enabled. Only if that event is enabled the call to this API is valid.
 * If the Event is enabled and has not yet occurred then a false is returned.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableEvent()<BR>  XMC_CCU8_SLICE_SetEvent().
 */
bool XMC_CCU8_SLICE_GetEvent(const XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_IRQ_ID_t event);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param event Event which must be bound to a service request line
 * @param sr The Service request line which is bound to the \b event
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Binds requested event to a service request line, by configuring CC8ySRS register with respective event.\n\n
 * For an event to lead to an interrupt, it must first be enabled and bound to a service request line. The corresponding
 * NVIC node must be enabled as well. This API binds the requested event with the requested service request line(\b sr).
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableEvent()<BR>  XMC_CCU8_SLICE_SetEvent().
 *
 */
void XMC_CCU8_SLICE_SetInterruptNode(XMC_CCU8_SLICE_t *const slice,
                                     const XMC_CCU8_SLICE_IRQ_ID_t event,
                                     const XMC_CCU8_SLICE_SR_ID_t sr);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param out Output signal for which the passive level needs to be set.
 * @param level Output passive level for the \b out signal
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the passive level for the slice output, by setting CC8yPSL register.\n\n
 * Defines the passive level for the timer slice output pin. Selects either level high is passive
 * or level low is passive. This is the level of the output before the compare match is value changes it.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableEvent()<BR>  XMC_CCU8_SLICE_SetEvent().
 */
void XMC_CCU8_SLICE_SetPassiveLevel(XMC_CCU8_SLICE_t *const slice,
                                    const XMC_CCU8_SLICE_OUTPUT_t out,
                                    const XMC_CCU8_SLICE_OUTPUT_PASSIVE_LEVEL_t level);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param config Pointer to dead time configuration data
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Initializes Dead time configuration for the slice outputs, by configuring CC8yDC1R, CC8yDC2R, CC8yDTC registers.\n\n
 * This routine programs dead time delays (rising & falling) and dead time clock prescaler.
 * Details such as the choice of dead time for channel1, channel2, ST1, Inverted ST1, ST2, Inverted ST2, are also
 * programmed by this routine.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_ConfigureDeadTime()<BR>  XMC_CCU8_SLICE_SetDeadTimeValue()<BR>  XMC_CCU8_SLICE_SetDeadTimePrescaler()<BR> 
 *  XMC_CCU8_SLICE_IsDeadTimeCntr1Running()<BR>  XMC_CCU8_SLICE_IsDeadTimeCntr2Running().
 */
void XMC_CCU8_SLICE_DeadTimeInit(XMC_CCU8_SLICE_t *const slice,
                                 const XMC_CCU8_SLICE_DEAD_TIME_CONFIG_t *const config);

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the slice to generate PWM in asymmetric compare mode, by setting CC8yCHC.ASE bit.\n\n
 * In asymmetric compare mode, the compare channels 1 & 2 are grouped to generate the PWM.This would
 * generate an inverted PWM at OUT0 & OUT1.
 * In Edge Aligned mode (counting up), the Status bit is set when a compare match of
 * Compare channel-1 occurs and cleared when a compare match event of Compare channel-2 occurs.\n
 * In Center Aligned mode, the status bit is set when a compare match event of Compare channel-1 occurs while
 * counting up and cleared when a compare match event of Compare channel-2 occurs while counting down.
 *
 * \par<b>Note:</b><br>
 * External count direction function is enabled then the asymmetric mode of operation is not possible.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableSymmetricCompareMode()<BR> 
 */
__STATIC_INLINE void XMC_CCU8_SLICE_EnableAsymmetricCompareMode(XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_EnableAsymmetricCompareMode:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->CHC |= (uint32_t) CCU8_CC8_CHC_ASE_Msk;
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the slice to generate PWM in symmetric(standard) compare mode, by clearing CC8yCHC.ASE bit.\n\n
 * In symmetric compare mode, the compare channels 1 & 2 are independent of each other & each channel generates the
 * PWM & inverted PWM at OUT0, OUT1, OUT2 & OUT3.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_EnableAsymmetricCompareMode().
 */
__STATIC_INLINE void XMC_CCU8_SLICE_EnableSymmetricCompareMode(XMC_CCU8_SLICE_t *const slice)
{
  XMC_ASSERT("XMC_CCU8_SLICE_EnableSymmetricCompareMode:Invalid Slice Pointer", XMC_CCU8_CHECK_SLICE_PTR(slice));
  slice->CHC &= ~((uint32_t) CCU8_CC8_CHC_ASE_Msk);
}

/**
 * @param slice Constant pointer to CC8 Slice
 * @param mask The Dead Time configuration mask.
 *             Do a bitwise OR operation on the following values depending on the need.
 *             Value 0x1:  Dead Time Enable for Compare Channel 1
 *             Value 0x2:  Dead Time Enable for Compare Channel 2
 *             Value 0x4:  Dead Time Enable for CC8yST1 path is enabled.
 *             Value 0x8:  Dead Time Enable for Inverted CC8yST1 path is enabled.
 *             Value 0x10: Dead Time Enable for CC8yST2 path is enabled.
 *             Value 0x20: Dead Time Enable for Inverted CC8yST2 path is enabled.
 *             Range: [0x0 to 0x3F]
 *
 * \par<b>Description:</b><br>
 * Activates or deactivates dead time for compare channel and ST path, by configuring CC8y.DC1R, CC8y.DC1R and 
 * CC8y.DTC registers.\n\n
 * Use the provided masks to enable/disable the dead time for the compare channels and the ST signals. It is possible
 * to deactivate the dead time for all the options by passing a 0x0 as the mask.
 * Details such as the choice of dead time for channel1, channel2, ST1, Inverted ST1, ST2, Inverted ST2, are
 * programmed by this routine.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_DeadTimeInit()<BR> XMC_CCU8_SLICE_SetDeadTimeValue()<BR>  XMC_CCU8_SLICE_SetDeadTimePrescaler()<BR> 
 *  XMC_CCU8_SLICE_IsDeadTimeCntr1Running()<BR>  XMC_CCU8_SLICE_IsDeadTimeCntr2Running().
 */
void XMC_CCU8_SLICE_ConfigureDeadTime(XMC_CCU8_SLICE_t *const slice, const uint8_t mask);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param channel Compare channel number
 * @param rise_value Programs rising edge delay
 *                   Range: [0x0 to 0xFF]
 * @param fall_value Programs falling edge delay
 *                   Range: [0x0 to 0xFF]
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures the dead time for rising and falling edges, by updating CC8y.DC1R, CC8y.DC1R registers.\n\n
 * This API will Configure the delay that is need either when the value changes from 0 to 1 (rising edge) or
 * value changes from 1 to 0(falling edge). Directly accessed registers are CC8yDC1R, CC8yDC2R.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_DeadTimeInit()<BR> XMC_CCU8_SLICE_SetDeadTimeValue()<BR>  XMC_CCU8_SLICE_ConfigureDeadTime()<BR> 
 *  XMC_CCU8_SLICE_IsDeadTimeCntr1Running()<BR>  XMC_CCU8_SLICE_IsDeadTimeCntr2Running().
 */
void XMC_CCU8_SLICE_SetDeadTimeValue(XMC_CCU8_SLICE_t *const slice,
                                     const XMC_CCU8_SLICE_COMPARE_CHANNEL_t channel,
                                     const uint8_t rise_value,
                                     const uint8_t fall_value);

/**
 * @param slice Pointer to an instance of CC8 slice
 * @param div_val    Prescaler divider value
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures clock division factor for dead time generator, by configuring CC8yDTC.DTCC bit. 
 * The Clock divider works on the timer clock. It is possible to scale the timer clock for the dead time 
 * generator by a factor of 1/2/4/8. This selection is passed as an argument to the API.
 *
 * \par<b>Related APIs:</b><br>
 *  XMC_CCU8_SLICE_DeadTimeInit()<BR> XMC_CCU8_SLICE_SetDeadTimeValue()<BR>  XMC_CCU8_SLICE_ConfigureDeadTime()<BR> 
 *  XMC_CCU8_SLICE_IsDeadTimeCntr1Running()<BR>  XMC_CCU8_SLICE_IsDeadTimeCntr2Running().
 */
void XMC_CCU8_SLICE_SetDeadTimePrescaler(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_DTC_DIV_t div_val);

/**
 * @param slice Constant pointer to CC8 Slice
 * @param channel which channel status has to be give as out
 * @return <BR>
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Configures status ST1, ST2 mapping to STy, by configuring CC8yTC.STOS bits.\n\n
 * This routine defines the output STy as a function of ST1 or ST2 or both ST1 & ST2.
 * It is possible to make the CCU8x.STy signal to reflect the CC8y.ST1/CC8y.ST2 or a function of the 2 signals.
 *
  * \par<b>Related APIs:</b><br>
 *  None.
 */
void XMC_CCU8_SLICE_ConfigureStatusBitOutput(XMC_CCU8_SLICE_t *const slice, const XMC_CCU8_SLICE_STATUS_t channel);

#ifdef __cplusplus
}
#endif
                                   
/**
 * @}
 */

/**
 * @}
 */

#endif /* defined(CCU80) */

#endif /* CCU8_H */


