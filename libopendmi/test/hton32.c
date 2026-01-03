//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <memory.h>

#include <opendmi/utils/endian.h>

struct test_vector
{
    uint32_t value;
    uint8_t  result[4];
};

struct test_vector test_data[] =
{
    { 0x00000000Lu, { 0x00, 0x00, 0x00, 0x00 } },
    { 0x01234567Lu, { 0x01, 0x23, 0x45, 0x67 } },
    { 0xFFFFFFFFLu, { 0xFF, 0xFF, 0xFF, 0xFF } }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        uint32_t result;

        result = dmi_hton32(test_data[i].value);
        if (memcmp(&result, test_data[i].result, sizeof(result)) != 0)
            return EXIT_FAILURE;

        result = dmi_hton(test_data[i].value);
        if (memcmp(&result, test_data[i].result, sizeof(result)) != 0)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
