//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/utils.h>

#include <opendmi/entity/system.h>

const dmi_name_set_t dmi_system_wakeup_type_names =
{
    .code  = "system-wakeup-types",
    .names = {
        DMI_NAME_RESERVED(DMI_SYSTEM_WAKEUP_TYPE_RESERVED),
        DMI_NAME_OTHER(DMI_SYSTEM_WAKEUP_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_SYSTEM_WAKEUP_TYPE_UNKNOWN),
        {
            .id   = DMI_SYSTEM_WAKEUP_TYPE_APM_TIMER,
            .code = "apm-timer",
            .name = "APM Timer"
        },
        {
            .id   = DMI_SYSTEM_WAKEUP_TYPE_MODEM_RING,
            .code = "modem-ring",
            .name = "Modem Ring"
        },
        {
            .id   = DMI_SYSTEM_WAKEUP_TYPE_LAN_REMOTE,
            .code = "lan-remote",
            .name = "LAN Remote"
        },
        {
            .id = DMI_SYSTEM_WAKEUP_TYPE_POWER_SWITCH,
            .code = "power-switch",
            .name = "Power Switch"
        },
        {
            .id   = DMI_SYSTEM_WAKEUP_TYPE_PCI_PME,
            .code = "pci-pme",
            .name = "PCI PME#"
        },
        {
            .id   = DMI_SYSTEM_WAKEUP_TYPE_POWER_RESTORE,
            .code = "power-restore",
            .name = "AC Power Restored"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_system_attrs[] =
{
    DMI_ATTRIBUTE(dmi_system_t, vendor, STRING, {
        .code    = "vendor",
        .name    = "Manufacturer"
    }),
    DMI_ATTRIBUTE(dmi_system_t, product, STRING, {
        .code    = "product",
        .name    = "Product"
    }),
    DMI_ATTRIBUTE(dmi_system_t, version, STRING, {
        .code    = "version",
        .name    = "Version"
    }),
    DMI_ATTRIBUTE(dmi_system_t, serial_number, STRING, {
        .code    = "serial-number",
        .name    = "Serial number"
    }),
    DMI_ATTRIBUTE(dmi_system_t, uuid, UUID, {
        .code    = "uuid",
        .name    = "UUID",
        .level   = DMI_VERSION(2, 1, 0)
    }),
    DMI_ATTRIBUTE(dmi_system_t, wakeup_type, ENUM, {
        .code    = "wakeup-type",
        .name    = "Wakeup type",
        .values  = &dmi_system_wakeup_type_names,
        .unknown = dmi_value_ptr(DMI_SYSTEM_WAKEUP_TYPE_UNKNOWN),
        .level   = DMI_VERSION(2, 1, 0)
    }),
    DMI_ATTRIBUTE(dmi_system_t, sku_number, STRING, {
        .code    = "sku-number",
        .name    = "SKU number",
        .level   = DMI_VERSION(2, 4, 0)
    }),
    DMI_ATTRIBUTE(dmi_system_t, family, STRING, {
        .code    = "family",
        .name    = "Family",
        .level   = DMI_VERSION(2, 4, 0)
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_system_spec =
{
    .code          = "system",
    .name          = "System information",
    .type          = DMI_TYPE_SYSTEM,
    .required_from = DMI_VERSION(2, 3, 0),
    .required_till = DMI_VERSION_NONE,
    .unique        = true,
    .min_length    = 0x8,
    .attributes    = dmi_system_attrs,
    .handlers      = {
        .decode = (dmi_entity_decode_fn_t)dmi_system_decode,
        .free   = (dmi_entity_free_fn_t)dmi_system_free
    }
};

const char *dmi_system_wakeup_type_name(dmi_system_wakeup_type_t value)
{
    return dmi_name_lookup(&dmi_system_wakeup_type_names, value);
}

dmi_system_t *dmi_system_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_system_t *info;
    dmi_version_t level = dmi_version(2, 0, 0);
    const dmi_system_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_SYSTEM));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->vendor        = dmi_entity_string(entity, data->vendor);
    info->product       = dmi_entity_string(entity, data->product);
    info->version       = dmi_entity_string(entity, data->version);
    info->serial_number = dmi_entity_string(entity, data->serial_number);

    //
    // SMBIOS 2.1 features
    //

    if (entity->body_length > 0x08u) {
        level = dmi_version(2, 1, 0);
        info->uuid = dmi_uuid_decode(data->uuid);
    }
    if (entity->body_length > 0x18u)
        info->wakeup_type = data->wakeup_type;

    //
    // SMBIOS 2.4 features
    //

    if (entity->body_length > 0x19u) {
        level = dmi_version(2, 4, 0);
        info->sku_number = dmi_entity_string(entity, data->sku_number);
    }
    if (entity->body_length > 0x1Au)
        info->family = dmi_entity_string(entity, data->family);

    if (plevel != nullptr)
        *plevel = level;

    return info;
}

void dmi_system_free(dmi_system_t *info)
{
    dmi_free(info);
}
