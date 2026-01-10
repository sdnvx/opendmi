//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <stdlib.h>

#include <opendmi/attribute.h>
#include <opendmi/utils.h>

struct test_vector
{
    const void *value;
    bool pretty;
    const char *result;
};

struct test_vector test_data[] =
{
    { dmi_value_ptr((dmi_handle_t)0x0000u), false, "0x0000" },
    { dmi_value_ptr((dmi_handle_t)0x0000u), true,  "0x0000" },
    { dmi_value_ptr((dmi_handle_t)0x1234u), false, "0x1234" },
    { dmi_value_ptr((dmi_handle_t)0x1234u), true,  "0x1234" },
    { dmi_value_ptr((dmi_handle_t)0xABCDu), false, "0xABCD" },
    { dmi_value_ptr((dmi_handle_t)0xABCDu), true,  "0xABCD" },
    { dmi_value_ptr((dmi_handle_t)0xFFFFu), false, "0xFFFF" },
    { dmi_value_ptr((dmi_handle_t)0xFFFFu), true , "0xFFFF" }
};

int main(void)
{
    const dmi_attribute_t attr = {
        .value   = {
            .size   = sizeof(dmi_handle_t),
            .offset = 0
        },
        .counter = DMI_MEMBER_NULL,
        .type    = DMI_ATTRIBUTE_TYPE_HANDLE,
        .params  = {}
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
