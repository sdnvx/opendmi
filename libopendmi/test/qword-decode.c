//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <stdbool.h>

#include <opendmi/utils.h>

struct test_vector
{
    uint8_t  value[8];
    uint64_t result;
};

struct test_vector test_data[] =
{
    { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, 0x0000000000000000UL },
    { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, 0xFFFFFFFFFFFFFFFFUL },
    { { 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 }, 0x0123456789ABCDEFUL }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        if (dmi_qword_decode(*(dmi_qword_t *)test_data[i].value) != test_data[i].result)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
