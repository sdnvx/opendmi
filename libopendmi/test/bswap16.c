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
    uint16_t value;
    uint16_t result;
};

struct test_vector test_data[] =
{
    { 0x0000U, 0x0000U },
    { 0xFFFFU, 0xFFFFU },
    { 0x0123U, 0x2301U }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        if (__dmi_bswap16_compat(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;

        if (dmi_bswap16(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;

        if (dmi_bswap(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
