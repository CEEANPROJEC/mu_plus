/**@file

Library provides access to the memory protection setting which may exist
in the platform-specific early store due to a memory related exception being triggered.

Copyright (c) Microsoft Corporation.
SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/MemoryProtectionLib.h>
#include <Library/MemoryProtectionExceptionLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>

#include "MemoryProtectionExceptionCommon.h"

/**
  Gets a memory protections setting from CMOS (if it's valid).

  @param[in]     VarToken   MEMORY_PROTECTION_VAR_TOKEN representing variable.
  @param[in,out] Setting    UINT32 populated with bitmask for current memory protection setting.

  @retval EFI_SUCCESS       Setting now contains bitmask for String memory setting.
  @retval EFI_NOT_FOUND     Memory protections variable region in CMOS is invalid.
  @retval EFI_ABORTED       Setting address was NULL

**/
EFI_STATUS
GetMemoryProtectionCmosSetting (
  IN    MEMORY_PROTECTION_VAR_TOKEN      VarToken,
  OUT   UINT32                          *Setting
  )
{
  UINT8 CmosVal;

  CmosVal = CmosReadMemoryProtectionByte();

  if (CmosVal & CMOS_MEM_PROT_VALID_BIT_MASK) {
    if(VarToken == MEM_PROT_GLOBAL_TOGGLE_SETTING) {
      *Setting = (CmosVal & CMOS_MEM_PROT_TOG_BIT_MASK) >> 1;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Gets a memory protection setting from the platform-specific early store. This setting value is only intended
  to exist in early store if an exception was hit potentially related to memory protections.

  @param[in]     VarToken   MEMORY_PROTECTION_VAR_TOKEN representing variable.
  @param[in,out] Setting    UINT32 populated with bitmask for current memory protection setting.

  @retval EFI_SUCCESS             Setting now contains bitmask for String memory setting.
  @retval EFI_NOT_FOUND           Memory protections variable region in CMOS is invalid.
  @retval EFI_INVALID_PARAMETER   Setting was NULL

**/
EFI_STATUS
EFIAPI
MemoryProtectionExceptionOverrideCheck (
  IN    MEMORY_PROTECTION_VAR_TOKEN      VarToken,
  OUT   UINT32                          *Setting
  )
{
  if (Setting == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return GetMemoryProtectionCmosSetting(VarToken, Setting);
}

/**
  Clears the memory protection setting from the platform-specific early store.

  @retval EFI_SUCCESS       Always return success

**/
VOID
EFIAPI
ClearMemoryProtectionExceptionOverride (
  VOID
  ) 
{
  CmosWriteMemoryProtectionByte((UINT8) 0);
}