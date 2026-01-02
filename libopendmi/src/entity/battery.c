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

#include <opendmi/entity/battery.h>

static const dmi_name_set_t dmi_battery_chemistry_names =
{
    .code  = "battery-chemistries",
    .names = {
        DMI_NAME_UNSPEC(DMI_BATTERY_CHEMISTRY_UNSPEC),
        DMI_NAME_OTHER(DMI_BATTERY_CHEMISTRY_OTHER),
        DMI_NAME_UNKNOWN(DMI_BATTERY_CHEMISTRY_UNKNOWN),
        {
            .id   = DMI_BATTERY_CHEMISTRY_LEAD_ACID,
            .code = "lead-acid",
            .name = "Lead acid"
        },
        {
            .id   = DMI_BATTERY_CHEMISTRY_NI_CD,
            .code = "ni-cd",
            .name = "Nickel cadmium"
        },
        {
            .id   = DMI_BATTERY_CHEMISTRY_NI_MH,
            .code = "ni-mh",
            .name = "Nickel metal hyrdide"
        },
        {
            .id   = DMI_BATTERY_CHEMISTRY_LI_ION,
            .code = "li-ion",
            .name = "Lithium-ion"
        },
        {
            .id   = DMI_BATTERY_CHEMISTRY_ZN_AIR,
            .code = "zn-air",
            .name = "Zinc air"
        },
        {
            .id   = DMI_BATTERY_CHEMISTRY_LI_PO,
            .code = "li-po",
            .name = "Lithium polymer"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_battery_attrs[] =
{
    DMI_ATTRIBUTE(dmi_battery_t, location, STRING, {
        .code    = "location",
        .name    = "Location"
    }),
    DMI_ATTRIBUTE(dmi_battery_t, vendor, STRING, {
        .code    = "vendor",
        .name    = "Vendor"
    }),
    DMI_ATTRIBUTE(dmi_battery_t, manufacture_date, DATE, {
        .code    = "manufacture-date",
        .name    = "Manufacture date",
        .unspec  = dmi_value_ptr(DMI_DATE_NONE)
    }),
    DMI_ATTRIBUTE(dmi_battery_t, serial_number, STRING, {
        .code    = "serial-number",
        .name    = "Serial number"
    }),
    DMI_ATTRIBUTE(dmi_battery_t, name, STRING, {
        .code    = "name",
        .name    = "Name"
    }),
    DMI_ATTRIBUTE(dmi_battery_t, chemistry, ENUM, {
        .code    = "chemistry",
        .name    = "Chemistry",
        .unspec  = dmi_value_ptr(DMI_BATTERY_CHEMISTRY_UNSPEC),
        .unknown = dmi_value_ptr(DMI_BATTERY_CHEMISTRY_UNKNOWN),
        .values  = &dmi_battery_chemistry_names,
    }),
    DMI_ATTRIBUTE(dmi_battery_t, capacity, DECIMAL, {
        .code    = "capacity",
        .name    = "Design capacity",
        .unit    = DMI_UNIT_WATT_HOUR,
        .scale   = 3,
        .unknown = dmi_value_ptr((unsigned int)0)
    }),
    DMI_ATTRIBUTE(dmi_battery_t, voltage, DECIMAL, {
        .code    = "voltage",
        .name    = "Design voltage",
        .unit    = DMI_UNIT_VOLT,
        .scale   = 3,
        .unknown = dmi_value_ptr((unsigned short)0)
    }),
    DMI_ATTRIBUTE(dmi_battery_t, sbds_version, STRING, {
        .code    = "sbds-version",
        .name    = "SBDS version"
    }),
    DMI_ATTRIBUTE(dmi_battery_t, maximum_error, INTEGER, {
        .code    = "maximum-error",
        .name    = "Maximum error",
        .unit    = DMI_UNIT_PERCENT,
        .unknown = dmi_value_ptr((unsigned short)USHRT_MAX)
    }),
    DMI_ATTRIBUTE(dmi_battery_t, sbds_serial_number, INTEGER, {
        .code    = "sbds-serial-number",
        .name    = "SBDS serial number",
        .level   = DMI_VERSION(2, 2, 0)
    }),
    DMI_ATTRIBUTE(dmi_battery_t, sbds_chemistry, STRING, {
        .code    = "sbds-chemistry",
        .name    = "SBDS chemistry",
        .level   = DMI_VERSION(2, 2, 0)
    }),
    DMI_ATTRIBUTE(dmi_battery_t, oem_defined, INTEGER, {
        .code    = "oem-defined",
        .name    = "OEM-defined",
        .flags   = DMI_ATTRIBUTE_FLAG_HEX,
        .level   = DMI_VERSION(2, 2, 0)
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_battery_spec =
{
    .code        = "portable-battery",
    .name        = "Portable battery",
    .type        = DMI_TYPE_PORTABLE_BATTERY,
    .min_version = DMI_VERSION(2, 1, 0),
    .min_length  = 0x05,
    .attributes  = dmi_battery_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_battery_decode,
        .free   = (dmi_entity_free_fn_t)dmi_battery_free
    }
};

const char *dmi_battery_chemistry_name(dmi_battery_chemistry_t value)
{
    return dmi_name_lookup(&dmi_battery_chemistry_names, value);
}

dmi_battery_t *
dmi_battery_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_battery_t *info;
    dmi_version_t level = dmi_version(2, 1, 0);
    const dmi_battery_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_PORTABLE_BATTERY));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->location = dmi_entity_string(entity, data->location);
    info->vendor   = dmi_entity_string(entity, data->vendor);

    const char *manufacture_date = dmi_entity_string(entity, data->manufacture_date);
    if (manufacture_date != nullptr) {
        info->manufacture_date = dmi_date_parse(manufacture_date);
        if (info->manufacture_date == DMI_DATE_NONE)
            dmi_log_warning(entity->context, "Invalid battery manufacture date format: '%s'", manufacture_date);
    }

    info->serial_number    = dmi_entity_string(entity, data->serial_number);
    info->name             = dmi_entity_string(entity, data->name);
    info->chemistry        = dmi_value(data->chemistry);
    info->capacity         = dmi_value(data->capacity);
    info->voltage          = dmi_value(data->voltage);
    info->sbds_version     = dmi_entity_string(entity, data->sbds_version);

    uint8_t maximum_error = dmi_value(data->maximum_error);
    if (maximum_error != 0xFFu)
        info->maximum_error = maximum_error;
    else
        info->maximum_error = USHRT_MAX;

    // SMBIOS 2.2 features
    if (entity->body_length >= 0x10u) {
        level = dmi_version(2, 2, 0);

        info->sbds_serial_number    = dmi_value(data->sbds_serial_number);

        if (manufacture_date == nullptr) {
            info->manufacture_date = dmi_date(
                ((data->sbds_manufacture_date & 0xFF00u) >> 8) + 1980,
                (data->sbds_manufacture_date & 0x00F0u) >> 4,
                data->sbds_manufacture_date & 0x000Fu
            );
        }

        info->sbds_chemistry        = dmi_entity_string(entity, data->sbds_chemistry);
        info->oem_defined           = dmi_value(data->oem_defined);

        info->capacity *= dmi_value(data->capacity_factor);
    }

    if (plevel != nullptr)
        *plevel = level;

    return info;
}

void dmi_battery_free(dmi_battery_t *info)
{
    dmi_free(info);
}
