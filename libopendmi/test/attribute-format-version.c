//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <stdlib.h>

#include <opendmi/version.h>
#include <opendmi/attribute.h>
#include <opendmi/utils.h>

struct test_vector
{
    const void *value;
    int scale;
    bool pretty;
    const char *result;
};

struct test_vector test_data[] =
{
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 0, false, "1.2.3" },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 0, true,  "1.2.3" },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 1, false, "1"     },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 1, true,  "1"     },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 2, false, "1.2"   },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 2, true,  "1.2"   },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 3, false, "1.2.3" },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 3, true,  "1.2.3" },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 4, false, "1.2.3" },
    { dmi_value_ptr(DMI_VERSION(1, 2, 3)), 4, true,  "1.2.3" }
};

int main(void)
{
    for (size_t i = 0; i < dmi_array_size(test_data); i++) {
        char *result;

        const dmi_attribute_t attr = {
            .value   = {
                .size   = sizeof(dmi_version_t),
                .offset = 0
            },
            .counter = DMI_MEMBER_NULL,
            .type    = DMI_ATTRIBUTE_TYPE_VERSION,
            .params  = {
                .scale = test_data[i].scale
            }
        };

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
