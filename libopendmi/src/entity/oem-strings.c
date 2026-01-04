//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/oem-strings.h>

const dmi_attribute_t dmi_oem_strings_attrs[] =
{
    DMI_ATTRIBUTE(dmi_oem_strings_t, string_count, INTEGER, {
        .code = "string-count",
        .name = "String count"
    }),
    DMI_ATTRIBUTE_ARRAY(dmi_oem_strings_t, strings, string_count, STRING, {
        .code    = "strings",
        .name    = "Strings"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_oem_strings_spec =
{
    .code       = "oem-strings",
    .name       = "OEM strings",
    .type       = DMI_TYPE_OEM_STRINGS,
    .min_length = 0x05,
    .attributes = dmi_oem_strings_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_oem_strings_decode,
        .free   = (dmi_entity_free_fn_t)dmi_oem_strings_free
    }
};

dmi_oem_strings_t *dmi_oem_strings_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_oem_strings_t *info;
    const dmi_oem_strings_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_OEM_STRINGS));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->string_count = dmi_decode(data->count);

    info->strings = dmi_alloc_array(entity->context, sizeof(const char *), info->string_count);
    if (info->strings == nullptr) {
        dmi_free(info);
        return nullptr;
    }

    for (size_t i = 0; i < info->string_count; i++) {
        info->strings[i] = dmi_entity_string(entity, (dmi_string_t)(i + 1));
    }

    if (plevel != nullptr)
        *plevel = dmi_version(2, 0, 0);

    return info;
}

void dmi_oem_strings_free(dmi_oem_strings_t *info)
{
    dmi_free(info->strings);
    dmi_free(info);
}
