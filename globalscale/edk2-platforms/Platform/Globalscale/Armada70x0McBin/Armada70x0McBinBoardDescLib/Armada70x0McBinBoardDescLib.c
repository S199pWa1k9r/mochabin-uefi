/**
*
*  Copyright (C) 2018, Marvell International Ltd. and its affiliates.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>

#include <Library/ArmadaBoardDescLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MvGpioLib.h>
#include <Library/UefiBootServicesTableLib.h>

//
// GPIO Expander
//
STATIC MV_GPIO_EXPANDER mGpioExpander = {
  PCA9555_ID,
  0x21,
  0x0,
};


EFI_STATUS
EFIAPI
ArmadaBoardGpioExpanderGet (
  IN OUT MV_GPIO_EXPANDER **GpioExpanders,
  IN OUT UINTN             *GpioExpanderCount
  )
{
  *GpioExpanderCount = 1;
  *GpioExpanders = &mGpioExpander;

  return EFI_SUCCESS;
}

//
// PCIE
//
STATIC
MV_PCIE_CONTROLLER mPcieController[] = {
  { /* PCIE2 @0xF2640000 */
    .PcieDbiAddress        = 0xF2640000,
    .ConfigSpaceAddress    = 0xE0000000,
    /* serg */
#if 1
    .HaveResetGpio         = FALSE,
    .PcieResetGpio         = { 0 },
#endif
/* mini-pcie#1 (J5)
&cp0_pcie2 {
        status = "okay";
        marvell,reset-gpio = <&cp0_gpio0 9 GPIO_ACTIVE_HIGH>;
};
*/
#if 0
    .HaveResetGpio         = TRUE,
    .PcieResetGpio         =
     {
       MV_GPIO_DRIVER_TYPE_SOC_CONTROLLER,
       MV_GPIO_CP0_CONTROLLER0,
       9,
       FALSE
     },
#endif
    .PcieBusMin            = 0,
    .PcieBusMax            = 0xFE,
    .PcieIoTranslation     = 0xEFF00000,
    .PcieIoWinBase         = 0x0,
    .PcieIoWinSize         = 0x10000,
    .PcieMmio32Translation = 0,
    .PcieMmio32WinBase     = 0xC0000000,
    .PcieMmio32WinSize     = 0x20000000,
    .PcieMmio64Translation = 0,
    .PcieMmio64WinBase     = 0x800000000,
    .PcieMmio64WinSize     = 0x100000000,
  }
};

/**
  Return the number and description of PCIE controllers used on the platform.

  @param[in out] **PcieControllers      Array containing PCIE controllers'
                                        description.
  @param[in out]  *PcieControllerCount  Amount of used PCIE controllers.

  @retval EFI_SUCCESS                   The data were obtained successfully.
  @retval other                         Return error status.

**/
EFI_STATUS
EFIAPI
ArmadaBoardPcieControllerGet (
  IN OUT MV_PCIE_CONTROLLER CONST **PcieControllers,
  IN OUT UINTN                     *PcieControllerCount
  )
{
  *PcieControllers = mPcieController;
  *PcieControllerCount = ARRAY_SIZE (mPcieController);

  return EFI_SUCCESS;
}

//
// Order of devices in SdMmcDescTemplate has to be in par with ArmadaSoCDescLib
//
STATIC
MV_BOARD_SDMMC_DESC mSdMmcDescTemplate[] = {
  { /* eMMC 0xF06E0000 */
    0,     /* SOC will be filled by MvBoardDescDxe */
    0,     /* SdMmcDevCount will be filled by MvBoardDescDxe */
    FALSE, /* Xenon1v8Enabled */
    FALSE, /* Xenon8BitBusEnabled */
    TRUE,  /* XenonSlowModeEnabled */
    0x40,  /* XenonTuningStepDivisor */
    EmbeddedSlot /* SlotType */
#if 1 
  },
  { /* SD/MMC 0xF2780000 */
    0,     /* SOC will be filled by MvBoardDescDxe */
    0,     /* SdMmcDevCount will be filled by MvBoardDescDxe */
    TRUE,  /* Xenon1v8Enabled */
    FALSE, /* Xenon8BitBusEnabled */
    FALSE, /* XenonSlowModeEnabled */
    0x19,  /* XenonTuningStepDivisor */
    EmbeddedSlot /* SlotType */
#endif
  }
};

EFI_STATUS
EFIAPI
ArmadaBoardDescSdMmcGet (
  OUT UINTN               *SdMmcDevCount,
  OUT MV_BOARD_SDMMC_DESC **SdMmcDesc
  )
{
  *SdMmcDesc = mSdMmcDescTemplate;
  *SdMmcDevCount = ARRAY_SIZE (mSdMmcDescTemplate);

  return EFI_SUCCESS;
}
