//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <limits.h>

#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/value.h>
#include <opendmi/utils.h>

#include <opendmi/entity/power-supply.h>

static const dmi_name_set_t dmi_power_supply_type_names =
{
    .code  = "power-supply-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_POWER_SUPPLY_TYPE_UNSPEC),
        DMI_NAME_OTHER(DMI_POWER_SUPPLY_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_POWER_SUPPLY_TYPE_UNKNOWN),
        {
            .id   = DMI_POWER_SUPPLY_TYPE_LINEAR,
            .code = "linear",
            .name = "Linear"
        },
        {
            .id   = DMI_POWER_SUPPLY_TYPE_SWITCHING,
            .code = "switching",
            .name = "Switching"
        },
        {
            .id   = DMI_POWER_SUPPLY_TYPE_BATTERY,
            .code = "battery",
            .name = "Battery"
        },
        {
            .id   = DMI_POWER_SUPPLY_TYPE_UPS,
            .code = "ups",
            .name = "UPS"
        },
        {
            .id   = DMI_POWER_SUPPLY_TYPE_CONVERTER,
            .code = "converter",
            .name = "Converter"
        },
        {
            .id   = DMI_POWER_SUPPLY_TYPE_REGULATOR,
            .code = "regulator",
            .name = "Regulator"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_range_switching_type_names =
{
    .code  = "range-switching-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_RANGE_SWITCHING_TYPE_UNSPEC),
        DMI_NAME_OTHER(DMI_RANGE_SWITCHING_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_RANGE_SWITCHING_TYPE_UNKNOWN),
        {
            .id   = DMI_RANGE_SWITCHING_TYPE_MANUAL,
            .code = "manual",
            .name = "Manual"
        },
        {
            .id   = DMI_RANGE_SWITCHING_TYPE_AUTO,
            .code = "auto",
            .name = "Auto-switch"
        },
        {
            .id   = DMI_RANGE_SWITCHING_TYPE_WIDE,
            .code = "wide-range",
            .name = "Wide range"
        },
        {
            .id   = DMI_RANGE_SWITCHING_TYPE_NOT_APPLICABLE,
            .code = "not-applicable",
            .name = "Not applicable"
        },
        DMI_NAME_NULL
    }
};

static const dmi_attribute_t dmi_power_supply_attrs[] =
{
    DMI_ATTRIBUTE(dmi_power_supply_t, group, INTEGER, {
        .code    = "group",
        .name    = "Power unit group"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, location, STRING, {
        .code    = "location",
        .name    = "Location"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, name, STRING, {
        .code    = "name",
        .name    = "Name"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, vendor, STRING, {
        .code    = "vendor",
        .name    = "Manufacturer"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, serial_number, STRING, {
        .code    = "serial-number",
        .name    = "Serial number"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, asset_tag, STRING, {
        .code    = "asset-tag",
        .name    = "Asset tag"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, part_number, STRING, {
        .code    = "part-number",
        .name    = "Part number"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, revision, STRING, {
        .code    = "revision",
        .name    = "Revision"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, maximum_capacity, INTEGER, {
        .code    = "maximum-capacity",
        .name    = "Maximum capacity",
        .unit    = DMI_UNIT_WATT,
        .unknown = dmi_value_ptr((short)SHRT_MIN),
        .flags   = DMI_ATTRIBUTE_FLAG_SIGNED
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, hot_swappable, BOOL, {
        .code    = "hot-swappable",
        .name    = "Hot-swappable"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, present, BOOL, {
        .code    = "present",
        .name    = "Present"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, unplugged, BOOL, {
        .code    = "unplugged",
        .name    = "Unplugged"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, range_switching, ENUM, {
        .code    = "range-switching",
        .name    = "Input voltage range switching",
        .values  = &dmi_range_switching_type_names
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, status, ENUM, {
        .code    = "status",
        .name    = "Status",
        .values  = &dmi_status_names
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, type, ENUM, {
        .code    = "type",
        .name    = "Type",
        .values  = &dmi_power_supply_type_names
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, voltage_probe_handle, HANDLE, {
        .code    = "voltage-probe-handle",
        .name    = "Input voltage probe handle"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, cooling_device_handle, HANDLE, {
        .code    = "cooling-device-handle",
        .name    = "Cooling device handle"
    }),
    DMI_ATTRIBUTE(dmi_power_supply_t, current_probe_handle, HANDLE, {
        .code    = "current-probe-handle",
        .name    = "Input current probe handle"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_power_supply_spec =
{
    .code        = "power-supply",
    .name        = "System power supply",
    .type        = DMI_TYPE_POWER_SUPPLY,
    .min_version = DMI_VERSION(2, 3, 1),
    .min_length  = 0x16,
    .attributes  = dmi_power_supply_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_power_supply_decode,
        .free   = (dmi_entity_free_fn_t)dmi_power_supply_free
    }
};

const char *dmi_power_supply_type_name(dmi_power_supply_type_t value)
{
    return dmi_name_lookup(&dmi_power_supply_type_names, value);
}

const char *dmi_range_switching_type_name(dmi_range_switching_type_t value)
{
    return dmi_name_lookup(&dmi_range_switching_type_names, value);
}

dmi_power_supply_t *dmi_power_supply_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_power_supply_t *info;
    const dmi_power_supply_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_POWER_SUPPLY));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->group            = data->group;
    info->location         = dmi_entity_string(entity, data->location);
    info->name             = dmi_entity_string(entity, data->name);
    info->vendor           = dmi_entity_string(entity, data->vendor);
    info->serial_number    = dmi_entity_string(entity, data->serial_number);
    info->asset_tag        = dmi_entity_string(entity, data->asset_tag);
    info->part_number      = dmi_entity_string(entity, data->part_number);
    info->revision         = dmi_entity_string(entity, data->revision);
    info->maximum_capacity = dmi_value(data->maximum_capacity);

    dmi_power_supply_details_t details = {
        .__value = dmi_value(data->characteristics)
    };

    info->hot_swappable   = details.hot_swappable;
    info->present         = details.present;
    info->unplugged       = details.unplugged;
    info->range_switching = details.range_switching;
    info->status          = details.status;
    info->type            = details.type;

    info->voltage_probe_handle  = dmi_value(data->voltage_probe_handle);
    info->cooling_device_handle = dmi_value(data->cooling_device_handle);
    info->current_probe_handle  = dmi_value(data->current_probe_handle);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 3, 1);

    return info;
}

void dmi_power_supply_free(dmi_power_supply_t *info)
{
    dmi_free(info);
}
