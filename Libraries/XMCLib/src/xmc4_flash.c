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
 * @file xmc_flash.c
 * @date 16 Feb, 2015
 * @version 1.0.0
 *
 * @brief FLASH driver for XMC microcontroller family.
 *
 * History <br>
 *
 * Version 1.0.0 Initial version <br> 
 */

#include "xmc_flash.h"

#if UC_FAMILY == XMC4

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define XMC_FLASH_PROTECTION_CONFIGURATION_WORDS (8UL) /* Used to upadte the assembly buffer during protection 
                                                          configuration */
#define XMC_FLASH_PROTECTION_CONFIRMATION_OFFSET (512UL) /* Offset address for UCB page */
#define XMC_FLASH_PROTECTION_CONFIRMATION_WORDS  (4UL)
#define XMC_FLASH_PROTECTION_CONFIRMATION_CODE   (0x8AFE15C3UL)

/*******************************************************************************
 * LOCAL FUNCTIONS
 *******************************************************************************/
/*
 * Command to reset the status of the PFLASH
 */
void XMC_FLASH_lResetCommand(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xf0U;
}

/*
 * Command to program the PFLASH in to page mode, so that assembly buffer is used 
 */
void XMC_FLASH_lEnterPageModeCommand(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x50;
}

/*
 * Command to load the data into the page assembly buffer 
 */
void XMC_FLASH_lLoadPageCommand(uint32_t low_word, uint32_t high_word)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x55f0U);
  *address = low_word;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x55f4U);
  *address = high_word;
}

/*
 * Command to start the programming of one page with data from the assembly buffer
 */
void XMC_FLASH_lWritePageCommand(uint32_t *page_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xa0U;
  address = page_start_address;
  *address = 0xaaU;
}

/*
 * Command to start the programming of UCB page with data from the assembly buffer
 */
void XMC_FLASH_lWriteUCBPageCommand(uint32_t *page_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xc0U;
  address = page_start_address;
  *address = 0xaaU;
}

/*
 * Command to erase sector which is starting with the specified address
 */
void XMC_FLASH_lEraseSectorCommand(uint32_t *sector_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x80U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = sector_start_address;
  *address = 0x30U;
}

/*
 * Command to erase physical sector which is starting with the specified address
 */
void XMC_FLASH_lErasePhysicalSectorCommand(uint32_t *sector_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x80U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = sector_start_address;
  *address = 0x40U;
}

/*
 * Command to erase UCB sector which is starting with the specified address
 */
void XMC_FLASH_lEraseUCBCommand(uint32_t *ucb_sector_start_address)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x80U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = ucb_sector_start_address;
  *address = 0xc0U;
}

/*
 * Command to temporarily disables the write protection belonging to the the USER specified, when passwords match with their
 * configured values
 */
void XMC_FLASH_lDisableSectorWriteProtectionCommand(uint32_t user, uint32_t password_0, uint32_t password_1)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x553cU);
  *address = user;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = password_0;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = password_1;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5558U);
  *address = 0x05U;
}

/*
 * Command to temporarily disables the read protection along with write protection, when passwords match with their
 * configured values
 */
void XMC_FLASH_lDisableReadProtectionCommand(uint32_t password_0, uint32_t password_1)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xaaU;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = 0x55U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x553cU);
  *address = 0x00U;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = password_0;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0xaaa8U);
  *address = password_1;
  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5558U);
  *address = 0x08U;
}

/*
 * Command to enables the protection as it was configured
 */
void XMC_FLASH_lResumeProtectionCommand(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0x5eU;
}

/*
 * Command to clear FSR.PROG and FSR.ERASE and the error flags in FSR such as PFOPER, SQER, PROER, PFDBER, ORIER, VER
 */
void XMC_FLASH_lClearStatusCommand(void)
{
  volatile uint32_t *address;

  address = (uint32_t *)(XMC_FLASH_UNCACHED_BASE + 0x5554U);
  *address = 0xf5U;
}

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

 /* Returns the Driver major minor patch versionsEach of the XMC low level drivers export a version number.
 * Applications may then audit such a version number to determine compatibility.
 */
XMC_DRIVER_VERSION_t XMC_FLASH_GetDriverVersion(void)
{
  XMC_DRIVER_VERSION_t version;

  version.major = (uint8_t)XMC_FLASH_MAJOR_VERSION;
  version.minor = (uint8_t)XMC_FLASH_MINOR_VERSION;
  version.patch = (uint8_t)XMC_FLASH_PATCH_VERSION;

  return version;
}

/*
 * This API shall clear Program, erase and error flags(PFOPER, SQER, PROER, PFDBER, ORIER, VER) of FSR register.
 */
void XMC_FLASH_ClearStatus(void)
{
  XMC_FLASH_lClearStatusCommand();
}

/*
 * This API returns the FSR register value
 */
uint32_t XMC_FLASH_GetStatus(void)
{
  return FLASH0->FSR;
}

/*
 * This API enables the events which required to trigger the ISR 
 */
void XMC_FLASH_EnableEvent(const uint32_t event_msk)
{
  FLASH0->FCON |= event_msk;
}

/*
 * This API disables the event generation 
 */
void XMC_FLASH_DisableEvent(const uint32_t event_msk)
{
  FLASH0->FCON &= ~event_msk;
}

/*
 * This API write the PFLASH page
 */
void XMC_FLASH_ProgramPage(uint32_t *address, const uint32_t *data)
{
  uint32_t index;

  XMC_FLASH_lClearStatusCommand();  
  XMC_FLASH_lEnterPageModeCommand();

  for (index = 0; index < XMC_FLASH_WORDS_PER_PAGE; index += 2)
  {
    XMC_FLASH_lLoadPageCommand(data[index], data[index + 1]);
  }

  XMC_FLASH_lWritePageCommand(address);    
}

/*
 * This API erase the sector
 */
void XMC_FLASH_EraseSector(uint32_t *address)
{
  XMC_FLASH_lClearStatusCommand();
  XMC_FLASH_lEraseSectorCommand(address);
}

/*
 * This API install the global read and sector write protection for the specified user
 */
void XMC_FLASH_InstallProtection(uint8_t user,
                                 uint32_t protection_mask,
	                               uint32_t password_0,
                                 uint32_t password_1)
{
  uint32_t index; 
  
  XMC_ASSERT(" XMC_FLASH_ConfigureProtection: User level out of range", (user < 3U));

  XMC_FLASH_lEnterPageModeCommand();

  XMC_FLASH_lLoadPageCommand(protection_mask, 0UL);
  XMC_FLASH_lLoadPageCommand(protection_mask, 0UL);
  XMC_FLASH_lLoadPageCommand(password_0, password_1);
  XMC_FLASH_lLoadPageCommand(password_0, password_1);
  
  for (index = 0; index < (XMC_FLASH_WORDS_PER_PAGE - XMC_FLASH_PROTECTION_CONFIGURATION_WORDS); index += 2)
  {
    XMC_FLASH_lLoadPageCommand(0UL, 0UL);
  }

  XMC_FLASH_lWriteUCBPageCommand((uint32_t *)((uint32_t)XMC_FLASH_UCB0 + (user * XMC_FLASH_BYTES_PER_UCB)));
}

/*
 * This API confirm the protection. So that This sectors are locked with the specified protection.
 */
void XMC_FLASH_ConfirmProtection(uint8_t user)
{
  uint32_t index;

  XMC_ASSERT(" XMC_FLASH_ConfirmProtection: User level out of range", (user < 3U));

  XMC_FLASH_lEnterPageModeCommand();

  XMC_FLASH_lLoadPageCommand(XMC_FLASH_PROTECTION_CONFIRMATION_CODE, 0U);
  XMC_FLASH_lLoadPageCommand(XMC_FLASH_PROTECTION_CONFIRMATION_CODE, 0U);

  /* Fill the rest of page buffer with zeros*/
  for (index = 0UL; index < (XMC_FLASH_WORDS_PER_PAGE - XMC_FLASH_PROTECTION_CONFIRMATION_WORDS); index += 2)
  {
    XMC_FLASH_lLoadPageCommand(0UL, 0UL);
  }

  XMC_FLASH_lWriteUCBPageCommand((uint32_t *)((uint32_t)XMC_FLASH_UCB0 + 
                                 (user * XMC_FLASH_BYTES_PER_UCB) + XMC_FLASH_PROTECTION_CONFIRMATION_OFFSET));
}

/*
 * This API verify read protection configuration. And returns true if passwords are matching.
 */
bool XMC_FLASH_VerifyReadProtection(uint32_t password_0, uint32_t password_1)
{
  XMC_FLASH_lClearStatusCommand();
  XMC_FLASH_lDisableReadProtectionCommand(password_0, password_1);

  return (bool)((XMC_FLASH_GetStatus() & XMC_FLASH_STATUS_READ_PROTECTION_INSTALLED) && 
                (FLASH0->PROCON0 & XMC_FLASH_PROTECTION_READ_GLOBAL));
}

/*
 * This API verify sector write protection configuration. And returns true if passwords are matching for the 
 * specified user.
 */
bool XMC_FLASH_VerifyWriteProtection(uint32_t user,
                                     uint32_t protection_mask, 
									                   uint32_t password_0, 
                                     uint32_t password_1)
{
  uint32_t *flash_procon_ptr;

  XMC_ASSERT(" XMC_FLASH_VerifyWriteProtection: User level out of range", (user < 3U));

  flash_procon_ptr = (uint32_t *)(&(FLASH0->PROCON0) + user);

  XMC_FLASH_lClearStatusCommand();
  XMC_FLASH_lDisableSectorWriteProtectionCommand(user, password_0, password_1);

  return (bool)((XMC_FLASH_GetStatus() & (1U << (FLASH_FSR_WPRODIS0_Pos + user))) && 
                 (*flash_procon_ptr == (protection_mask & ~XMC_FLASH_PROTECTION_READ_GLOBAL)));
}

#endif
