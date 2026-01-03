//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <memory.h>

#include <opendmi/context.h>
#include <opendmi/utils.h>

#include <opendmi/entity/system-boot.h>

const dmi_attribute_t dmi_system_boot_attrs[] =
{
    // TODO: add string code representation (ranges, you know).
    DMI_ATTRIBUTE(dmi_system_boot_t, status, INTEGER, {
        .code = "status",
        .name = "Boot status"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_system_boot_spec =
{
    .code          = "system-boot",
    .name          = "System boot information",
    .type          = DMI_TYPE_SYSTEM_BOOT,
    .required_from = DMI_VERSION(2, 3, 0),
    .required_till = DMI_VERSION_NONE,
    .min_length    = 0x14,
    .attributes    = dmi_system_boot_attrs
};

dmi_system_boot_t *dmi_system_boot_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_system_boot_t *info;
    const dmi_system_boot_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_SYSTEM_BOOT));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->status = data->status[0];
    memcpy(info->status_data, data->status, countof(info->status_data));

    if (plevel != nullptr)
        *plevel = dmi_version(2, 3, 0);

    return info;
}

void dmi_system_boot_free(dmi_system_boot_t *info)
{
    dmi_free(info);
}
