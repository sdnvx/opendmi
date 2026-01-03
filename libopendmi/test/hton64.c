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
    uint64_t value;
    uint8_t  result[8];
};

struct test_vector test_data[] =
{
    { 0x0000000000000000LLu, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x0123456789ABCDEFLLu, { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF } },
    { 0xFFFFFFFFFFFFFFFFLLu, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        uint64_t result;

        result = dmi_hton64(test_data[i].value);
        if (memcmp(&result, test_data[i].result, sizeof(result)) != 0)
            return EXIT_FAILURE;

        result = dmi_hton(test_data[i].value);
        if (memcmp(&result, test_data[i].result, sizeof(result)) != 0)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
