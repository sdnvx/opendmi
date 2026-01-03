//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <memory.h>

#include <opendmi/attribute.h>
#include <opendmi/utils.h>
#include <opendmi/utils/uuid.h>

struct test_vector
{
    uint8_t value[16];
    bool pretty;
    const char *result;
};

struct test_vector test_data[] =
{
    {
        .value = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        },
        .pretty = false,
        .result = "00000000-0000-0000-0000-000000000000"
    },
    {
        .value = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        },
        .pretty = true,
        .result = "00000000-0000-0000-0000-000000000000"
    },
    {
        .value = {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
        },
        .pretty = false,
        .result = "00112233-4455-6677-8899-AABBCCDDEEFF"
    },
    {
        .value = {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
        },
        .pretty = true,
        .result = "00112233-4455-6677-8899-AABBCCDDEEFF"
    },
    {
        .value = {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        },
        .pretty = false,
        .result = "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF"
    },
    {
        .value = {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        },
        .pretty = true,
        .result = "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF"
    }
};

int main(void)
{
    const dmi_attribute_t attr = {
        .value   = {
            .size   = sizeof(dmi_uuid_t),
            .offset = 0
        },
        .counter = DMI_MEMBER_NULL,
        .type    = DMI_ATTRIBUTE_TYPE_UUID,
        .params  = { 0 }
    };

    for (size_t i = 0; i < countof(test_data); i++) {
        char *result;

        result = dmi_attribute_format(&attr, test_data[i].value, test_data[i].pretty);
        if (result == nullptr)
            return EXIT_FAILURE;

        if (strcmp(result, test_data[i].result) != 0) {
            dmi_free(result);
            return EXIT_FAILURE;
        }

        dmi_free(result);
    }

    return EXIT_SUCCESS;
}
