//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/system-config.h>

const dmi_attribute_t dmi_system_config_opts_attrs[] =
{
    DMI_ATTRIBUTE(dmi_system_config_opts_t, option_count, INTEGER, {
        .code = "option-count",
        .name = "Option count"
    }),
    DMI_ATTRIBUTE_ARRAY(dmi_system_config_opts_t, options, option_count, STRING, {
        .code    = "options",
        .name    = "Options"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_system_config_opts_spec =
{
    .code       = "system-config-options",
    .name       = "System configuration options",
    .type       = DMI_TYPE_SYSTEM_CONFIG_OPTIONS,
    .min_length = 0x05,
    .attributes = dmi_system_config_opts_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_system_config_opts_decode,
        .free   = (dmi_entity_free_fn_t)dmi_system_config_opts_free
    }
};

dmi_system_config_opts_t *dmi_system_config_opts_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_system_config_opts_t *info;
    const dmi_system_config_opts_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_SYSTEM_CONFIG_OPTIONS));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->option_count = dmi_decode(data->count);

    info->options = dmi_alloc_array(entity->context, sizeof(const char *), info->option_count);
    if (info->options == nullptr) {
        dmi_free(info);
        return nullptr;
    }

    for (size_t i = 0; i < info->option_count; i++) {
        info->options[i] = dmi_entity_string(entity, (dmi_string_t)(i + 1));
    }

    if (plevel != nullptr)
        *plevel = dmi_version(2, 0, 0);

    return info;
}

void dmi_system_config_opts_free(dmi_system_config_opts_t *info)
{
    dmi_free(info->options);
    dmi_free(info);
}
