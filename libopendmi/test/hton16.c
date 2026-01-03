//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <memory.h>

#include <opendmi/endian.h>

struct test_vector
{
    uint16_t value;
    uint8_t  result[2];
};

struct test_vector test_data[] =
{
    { 0x0000u, { 0x00, 0x00 } },
    { 0x0123u, { 0x01, 0x23 } },
    { 0xFFFFu, { 0xFF, 0xFF } }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        uint16_t result;

        result = dmi_hton16(test_data[i].value);
        if (memcmp(&result, test_data[i].result, sizeof(result)) != 0)
            return EXIT_FAILURE;

        result = dmi_hton(test_data[i].value);
        if (memcmp(&result, test_data[i].result, sizeof(result)) != 0)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
