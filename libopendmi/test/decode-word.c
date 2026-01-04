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
    uint8_t  value[2];
    uint16_t result;
};

struct test_vector test_data[] =
{
    { { 0x00, 0x00 }, 0x0000U },
    { { 0xFF, 0xFF }, 0xFFFFU },
    { { 0x34, 0x12 }, 0x1234U }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        dmi_word_t value  = *(dmi_word_t *)test_data[i].value;
        uint16_t   result = test_data[i].result;

        if (dmi_decode_word(value) != result)
            return EXIT_FAILURE;
        if (dmi_decode(value) != result)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
