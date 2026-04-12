//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/value.h>
#include <opendmi/utils.h>
#include <opendmi/utils/name.h>
#include <opendmi/utils/codec.h>

#include <opendmi/entity/battery.h>

static bool dmi_battery_decode(dmi_entity_t *entity);

static const dmi_name_set_t dmi_battery_chemistry_names =
{
    .code  = "battery-chemistries",
    .names = (dmi_name_t[]){
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

const dmi_entity_spec_t dmi_battery_spec =
{
    .code            = "portable-battery",
    .name            = "Portable battery",
    .description     = (const char *[]){
        "This structure describes the attributes of the portable battery or "
        "batteries for the system. The structure contains the static "
        "attributes for the group. Each structure describes attributes for a "
        "single battery pack.",
        //
        nullptr
    },
    .type            = DMI_TYPE(PORTABLE_BATTERY),
    .minimum_version = DMI_VERSION(2, 1, 0),
    .minimum_length  = 0x05,
    .decoded_length  = sizeof(dmi_battery_t),
    .attributes      = (const dmi_attribute_t[]){
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
    },
    .handlers = {
        .decode = dmi_battery_decode
    }
};

const char *dmi_battery_chemistry_name(dmi_battery_chemistry_t value)
{
    return dmi_name_lookup(&dmi_battery_chemistry_names, value);
}

static bool dmi_battery_decode(dmi_entity_t *entity)
{
    const dmi_battery_data_t *data;
    dmi_battery_t *info;

    data = dmi_entity_data(entity, DMI_TYPE(PORTABLE_BATTERY));
    if (data == nullptr)
        return false;

    info = dmi_entity_info(entity, DMI_TYPE(PORTABLE_BATTERY));
    if (info == nullptr)
        return false;

    info->location = dmi_entity_string(entity, data->location);
    info->vendor   = dmi_entity_string(entity, data->vendor);

    const char *manufacture_date = dmi_entity_string(entity, data->manufacture_date);
    if (manufacture_date != nullptr) {
        info->manufacture_date = dmi_date_parse(manufacture_date);
        if (info->manufacture_date == DMI_DATE_NONE) {
            dmi_log_warning(entity->context->logger,
                            "Invalid battery manufacture date format: '%s'", manufacture_date);
        }
    }

    info->serial_number    = dmi_entity_string(entity, data->serial_number);
    info->name             = dmi_entity_string(entity, data->name);
    info->chemistry        = dmi_decode(data->chemistry);
    info->capacity         = dmi_decode(data->capacity);
    info->voltage          = dmi_decode(data->voltage);
    info->sbds_version     = dmi_entity_string(entity, data->sbds_version);

    uint8_t maximum_error = dmi_decode(data->maximum_error);
    if (maximum_error != 0xFFu)
        info->maximum_error = maximum_error;
    else
        info->maximum_error = USHRT_MAX;

    // SMBIOS 2.2 features
    if (entity->body_length > 0x10u) {
        entity->level = dmi_version(2, 2, 0);

        info->sbds_serial_number = dmi_decode(data->sbds_serial_number);

        if (manufacture_date == nullptr) {
            uint16_t sbds_manufacture_date = dmi_decode(data->sbds_manufacture_date);

            if (sbds_manufacture_date != 0) {
                info->manufacture_date = dmi_date(
                    ((sbds_manufacture_date >> 9) & 0x7Fu) + 1980,
                    (sbds_manufacture_date >> 5) & 0x0Fu,
                    sbds_manufacture_date & 0x1Fu
                );
            }
        }

        info->sbds_chemistry = dmi_entity_string(entity, data->sbds_chemistry);
        info->oem_defined    = dmi_decode(data->oem_defined);

        info->capacity *= dmi_decode(data->capacity_factor);
    }

    return true;
}
