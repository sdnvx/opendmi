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
    uint64_t value;
    uint64_t result;
};

struct test_vector test_data[] =
{
    { 0x0000000000000000UL, 0x0000000000000000UL },
    { 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL },
    { 0x0123456789ABCDEFUL, 0xEFCDAB8967452301UL }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        if (__dmi_bswap64_compat(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;

        if (dmi_bswap64(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;

        if (dmi_bswap(test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
