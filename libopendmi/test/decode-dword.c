//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <stdbool.h>

#include <opendmi/utils/decode.h>

struct test_vector
{
    uint8_t  value[4];
    uint32_t result;
};

struct test_vector test_data[] =
{
    { { 0x00, 0x00, 0x00, 0x00 }, 0x00000000U },
    { { 0xFF, 0xFF, 0xFF, 0xFF }, 0xFFFFFFFFU },
    { { 0x78, 0x56, 0x34, 0x12 }, 0x12345678U }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        dmi_dword_t value = *(dmi_dword_t *)test_data[i].value;
        uint32_t    result = test_data[i].result;

        if (dmi_decode_dword(value) != result)
            return EXIT_FAILURE;
        if (dmi_decode(value) != result)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
