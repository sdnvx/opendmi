//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/utils/endian.h>

uint16_t __dmi_bswap16_compat(uint16_t value)
{
    // Flip bytes
    value = ((value & 0x00FFU) << 8) | ((value & 0xFF00U) >> 8);

    return value;
}

uint32_t __dmi_bswap32_compat(uint32_t value)
{
    // Flip bytes
    value = ((value & 0x00FF00FFU) << 8) | ((value & 0xFF00FF00U) >> 8);

    // Flip words
    value = ((value & 0x0000FFFFU) << 16) | ((value & 0xFFFF0000U) >> 16);

    return value;
}

uint64_t __dmi_bswap64_compat(uint64_t value)
{
    // Flip bytes
    value = ((value & 0x00FF00FF00FF00FFUL) << 8) | ((value & 0xFF00FF00FF00FF00UL) >> 8);

    // Flip words
    value = ((value & 0x0000FFFF0000FFFFUL) << 16) | ((value & 0xFFFF0000FFFF0000UL) >> 16);

    // Flip double words
    value = ((value & 0x00000000FFFFFFFFUL) << 32) | ((value & 0xFFFFFFFF00000000UL) >> 32);

    return value;
}
