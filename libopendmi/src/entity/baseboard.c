//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/utils.h>

#include <opendmi/entity/baseboard.h>

const dmi_name_set_t dmi_baseboard_type_names =
{
    .code  = "baseboard-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_BASEBOARD_TYPE_UNSPEC),
        DMI_NAME_UNKNOWN(DMI_BASEBOARD_TYPE_UNKNOWN),
        DMI_NAME_OTHER(DMI_BASEBOARD_TYPE_OTHER),
        {
            .id   = DMI_BASEBOARD_TYPE_SERVER_BLADE,
            .code = "server-blade",
            .name = "Server blade"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_CONNECTIVITY_SWITCH,
            .code = "connectivity-switch",
            .name = "Connectivity switch"
        },
        {
            .id   =  DMI_BASEBOARD_TYPE_SYSTEM_MANAGEMENT_MODULE,
            .code = "system-management-module",
            .name = "System management module"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_PROCESSOR_MODULE,
            .code = "processor-module",
            .name = "Processor module"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_IO_MODULE,
            .code = "io-module",
            .name = "IO module"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_MEMORY_MODULE,
            .code = "memory-module",
            .name = "Memory module"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_DAUGHTERBOARD,
            .code = "daughterboard",
            .name = "Daughterboard"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_MOTHERBOARD,
            .code = "motherboard",
            .name = "Motherboard"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_PROCESSOR_MEMORY_MODULE,
            .code = "processor-memory-module",
            .name = "Processor/memory module"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_PROCESSOR_IO_MODULE,
            .code = "processor-io-module",
            .name = "Processor/IO module"
        },
        {
            .id   = DMI_BASEBOARD_TYPE_INTERCONNECT_BOARD,
            .code = "interconnect-board",
            .name = "Interconnect board"
        },
        DMI_NAME_NULL
    }
};

const dmi_name_set_t dmi_baseboard_feature_names =
{
    .code  = "baseboard-features",
    .names = {
        {
            .id   = 0,
            .code = "is-hosting-board",
            .name = "Hosting board"
        },
        {
            .id   = 1,
            .code = "require-daugther-board",
            .name = "Require daugther board"
        },
        {
            .id   = 2,
            .code = "is-removable",
            .name = "Removable"
        },
        {
            .id   = 3,
            .code = "is-replaceable",
            .name = "Replaceable"
        },
        {
            .id   = 4,
            .code = "is-hot-swappable",
            .name = "Hot-swappable"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_baseboard_attrs[] =
{
    DMI_ATTRIBUTE(dmi_baseboard_t, vendor, STRING, {
        .code    = "vendor",
        .name    = "Vendor"
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, product, STRING, {
        .code    = "product",
        .name    = "Product"
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, version, STRING, {
        .code    = "version",
        .name    = "Version"
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, serial_number, STRING, {
        .code    = "serial-number",
        .name    = "Serial number"
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, asset_tag, STRING, {
        .code    = "asset-tag",
        .name    = "Asset tag"
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, features, SET, {
        .code    = "features",
        .name    = "Features",
        .values  = &dmi_baseboard_feature_names
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, location, STRING, {
        .code    = "location",
        .name    = "Location"
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, chassis_handle, HANDLE, {
        .code    = "chassis-handle",
        .name    = "Chassis handle",
        .unspec  = dmi_value_ptr(DMI_HANDLE_INVALID)
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, type, ENUM, {
        .code    = "type",
        .name    = "Type",
        .unspec  = dmi_value_ptr(DMI_BASEBOARD_TYPE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_BASEBOARD_TYPE_UNKNOWN),
        .values  = &dmi_baseboard_type_names
    }),
    DMI_ATTRIBUTE(dmi_baseboard_t, children_count, INTEGER, {
        .code    = "children-count",
        .name    = "Children count"
    }),
    DMI_ATTRIBUTE_ARRAY(dmi_baseboard_t, children_handles, children_count, HANDLE, {
        .code    = "children",
        .name    = "Children"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_baseboard_spec =
{
    .code       = "baseboard",
    .name       = "Baseboard or module information",
    .type       = DMI_TYPE_BASEBOARD,
    .min_length = 0x0F,
    .attributes = dmi_baseboard_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_baseboard_decode,
        .free   = (dmi_entity_free_fn_t)dmi_baseboard_free
    }
};

const char *dmi_baseboard_type_name(dmi_baseboard_type_t value)
{
    return dmi_name_lookup(&dmi_baseboard_type_names, value);
}

dmi_baseboard_t *
dmi_baseboard_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_baseboard_t *info;
    const dmi_baseboard_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_BASEBOARD));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->vendor        = dmi_entity_string(entity, data->vendor);
    info->product       = dmi_entity_string(entity, data->product);
    info->version       = dmi_entity_string(entity, data->version);
    info->serial_number = dmi_entity_string(entity, data->serial_number);
    info->asset_tag     = dmi_entity_string(entity, data->asset_tag);

    dmi_baseboard_features_t features = {
        .__value = dmi_value(data->features)
    };

    info->features       = features;
    info->location       = dmi_entity_string(entity, data->location);
    info->chassis_handle = dmi_value(data->chassis_handle);
    info->type           = dmi_value(data->type);
    info->children_count = dmi_value(data->children_count);

    info->children_handles = dmi_alloc_array(entity->context, sizeof(dmi_handle_t), info->children_count);
    if (info->children_handles == nullptr) {
        dmi_free(info);
        return nullptr;
    }

    for (size_t i = 0; i < info->children_count; i++)
        info->children_handles[i] = dmi_value(data->children_handles[i]);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 0, 0);

    return info;
}

void dmi_baseboard_free(dmi_baseboard_t *info)
{
    dmi_free(info->children_handles);
    dmi_free(info);
}
