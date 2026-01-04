//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/firmware-inventory.h>

static const dmi_name_set_t dmi_version_format_names =
{
    .code  = "version-formats",
    .names = {
        {
            .id   = DMI_VERSION_FORMAT_FREE,
            .code = "free-form",
            .name = "Free form"
        },
        {
            .id   = DMI_VERSION_FORMAT_SEMANTIC,
            .code = "semantic",
            .name = "Semantic"
        },
        {
            .id   = DMI_VERSION_FORMAT_HEX_32,
            .code = "hexadecimal-32",
            .name = "Hexadecimal (32-bit)"
        },
        {
            .id   = DMI_VERSION_FORMAT_HEX_64,
            .code = "hexadecimal-64",
            .name = "Hexadecimal (64-bit)"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_firmware_ident_format_names =
{
    .code  = "firwmare-ident-formats",
    .names = {
        DMI_NAME_UNSPEC(DMI_FIRMWARE_IDENT_FORMAT_UNSPEC),
        {
            .id   = DMI_FIRMWARE_IDENT_FORMAT_FREE,
            .code = "free-form",
            .name = "Free form"
        },
        {
            .id   = DMI_FIRMWARE_IDENT_FORMAT_GUID,
            .code = "guid",
            .name = "GUID"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_firmware_inventory_feature_names =
{
    .code  = "firmware-inventory-features",
    .names = {
        {
            .id   = 0,
            .code = "is-updatable",
            .name = "Updatable"
        },
        {
            .id   = 1,
            .code = "is-write-protected",
            .name = "Write-protected"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_firmware_inventory_state_names =
{
    .code  = "firmware-inventory-states",
    .names = {
        DMI_NAME_UNSPEC(DMI_FIRMWARE_INVENTORY_STATE_UNSPEC),
        DMI_NAME_OTHER(DMI_FIRMWARE_INVENTORY_STATE_OTHER),
        DMI_NAME_UNKNOWN(DMI_FIRMWARE_INVENTORY_STATE_UNKNOWN),
        {
            .id   = DMI_FIRMWARE_INVENTORY_STATE_DISABLED,
            .code = "disabled",
            .name = "Disabled"
        },
        {
            .id   = DMI_FIRMWARE_INVENTORY_STATE_ENABLED,
            .code = "enabled",
            .name = "Enabled"
        },
        {
            .id   = DMI_FIRMWARE_INVENTORY_STATE_ABSENT,
            .code = "absent",
            .name = "Absent"
        },
        {
            .id   = DMI_FIRMWARE_INVENTORY_STATE_STANDBY_OFFLINE,
            .code = "standby-offline",
            .name = "Standby offline"
        },
        {
            .id   = DMI_FIRMWARE_INVENTORY_STATE_STANDBY_SPARE,
            .code = "standby-spare",
            .name = "Standby spare"
        },
        {
            .id   = DMI_FIRMWARE_INVENTORY_STATE_UNAVAIL_OFFLINE,
            .code = "unavail-offline",
            .name = "Unavailable offline"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_firmware_inventory_attrs[] =
{
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, name, STRING, {
        .code    = "name",
        .name    = "Name"
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, version, VERSION, {
        .code    = "version",
        .name    = "Version"
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, version_format, ENUM, {
        .code    = "version-format",
        .name    = "Version format",
        .values  = &dmi_version_format_names
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, ident, STRING, {
        .code    = "ident",
        .name    = "Identifier"
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, ident_format, ENUM, {
        .code    = "ident-format",
        .name    = "identifier format",
        .unspec  = dmi_value_ptr(DMI_FIRMWARE_IDENT_FORMAT_UNSPEC),
        .values  = &dmi_firmware_ident_format_names
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, release_date, STRING, {
        .code    = "release-date",
        .name    = "Release date"
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, vendor, STRING, {
        .code    = "vendor",
        .name    = "Vendor"
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, lowest_version, STRING, {
        .code    = "lowest-version",
        .name    = "Lowest version"
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, image_size, SIZE, {
        .code    = "image-size",
        .name    = "Image size"
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, features, SET, {
        .code    = "characteristics",
        .name    = "Characteristics",
        .values  = &dmi_firmware_inventory_feature_names
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, state, ENUM, {
        .code    = "state",
        .name    = "State",
        .unspec  = dmi_value_ptr(DMI_FIRMWARE_INVENTORY_STATE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_FIRMWARE_INVENTORY_STATE_UNSPEC),
        .values  = &dmi_firmware_inventory_state_names
    }),
    DMI_ATTRIBUTE(dmi_firmware_inventory_t, component_count, INTEGER, {
        .code    = "component-count",
        .name    = "Associated components count"
    }),
    DMI_ATTRIBUTE_ARRAY(dmi_firmware_inventory_t, component_handles, component_count, HANDLE, {
        .code    = "component-handles",
        .name    = "Associated component handles"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_firmware_inventory_spec =
{
    .code       = "firmware-inventory",
    .name       = "Firmware inventory information",
    .type       = DMI_TYPE_FIRMWARE_INVENTORY,
    .min_length = 0x18,
    .attributes = dmi_firmware_inventory_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_firmware_inventory_decode,
        .free   = (dmi_entity_free_fn_t)dmi_firmware_inventory_free
    }
};

const char *dmi_version_format_name(dmi_version_format_t value)
{
    return dmi_name_lookup(&dmi_version_format_names, value);
}

const char *dmi_firmware_ident_format_name(dmi_firmware_ident_format_t value)
{
    return dmi_name_lookup(&dmi_firmware_ident_format_names, value);
}

const char *dmi_firmware_inventory_state_name(dmi_firmware_inventory_state_t value)
{
    return dmi_name_lookup(&dmi_firmware_inventory_state_names, value);
}

dmi_firmware_inventory_t *dmi_firmware_inventory_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_firmware_inventory_t *info;
    const dmi_firmware_inventory_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_FIRMWARE_INVENTORY));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->name           = dmi_entity_string(entity, data->name);
    info->version        = dmi_entity_string(entity, data->version);
    info->version_format = dmi_decode(data->version_format);
    info->ident          = dmi_entity_string(entity, data->ident);
    info->ident_format   = dmi_decode(data->ident_format);
    info->release_date   = dmi_entity_string(entity, data->release_date);
    info->vendor         = dmi_entity_string(entity, data->vendor);
    info->lowest_version = dmi_entity_string(entity, data->lowest_version);
    info->image_size     = dmi_decode(data->image_size);

    dmi_firmware_inventory_features_t features = {
        .__value = dmi_decode(data->features)
    };
    info->features = features;

    info->state           = dmi_decode(data->state);
    info->component_count = dmi_decode(data->component_count);

    info->component_handles = dmi_alloc_array(entity->context, sizeof(dmi_handle_t), info->component_count);
    if (info->component_handles == nullptr) {
        dmi_free(info);
        return nullptr;
    }

    for (size_t i = 0; i < info->component_count; i++) {
        info->component_handles[i] = dmi_decode(data->component_handles[i]);
    }

    if (plevel != nullptr)
        *plevel = dmi_version(3, 5, 0);

    return info;
}

void dmi_firmware_inventory_free(dmi_firmware_inventory_t *info)
{
    dmi_free(info->component_handles);
    dmi_free(info->components);
    dmi_free(info);
}
