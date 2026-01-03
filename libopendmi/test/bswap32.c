//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <stdbool.h>

#include <opendmi/utils/endian.h>

struct test_vector
{
    uint32_t value;
    uint32_t result;
};

struct test_vector test_data[] =
{
    { 0x00000000U, 0x00000000U },
    { 0xFFFFFFFFU, 0xFFFFFFFFU },
    { 0x01234567U, 0x67452301U }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        if (__dmi_bswap32_compat(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;

        if (dmi_bswap32(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;

        if (dmi_bswap(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
