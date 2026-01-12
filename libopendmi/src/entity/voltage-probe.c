//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/value.h>
#include <opendmi/entity/voltage-probe.h>

/**
 * @brief Voltage probe attributes.
 */
const dmi_attribute_t dmi_voltage_probe_attrs[] =
{
    DMI_ATTRIBUTE(dmi_voltage_probe_t, description, STRING, {
        .code    = "description",
        .name    = "Description"
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, location, ENUM, {
        .code    = "location",
        .name    = "Location",
        .unspec  = dmi_value_ptr(DMI_PROBE_LOCATION_UNSPEC),
        .unknown = dmi_value_ptr(DMI_PROBE_LOCATION_UNKNOWN),
        .values  = &dmi_probe_location_names
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, status, ENUM, {
        .code    = "status",
        .name    = "Status",
        .unspec  = dmi_value_ptr(DMI_STATUS_UNSPEC),
        .unknown = dmi_value_ptr(DMI_STATUS_UNKNOWN),
        .values  = &dmi_status_names
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, maximum_value, DECIMAL, {
        .code    = "maximum-value",
        .name    = "Maximum value",
        .scale   = 1,
        .unit    = DMI_UNIT_MILLIVOLT,
        .unknown = dmi_value_ptr(DMI_PROBE_VALUE_UNKNOWN),
        .flags   = DMI_ATTRIBUTE_FLAG_SIGNED
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, minimum_value, DECIMAL, {
        .code    = "minimum-value",
        .name    = "Minimum value",
        .scale   = 1,
        .unit    = DMI_UNIT_MILLIVOLT,
        .unknown = dmi_value_ptr(DMI_PROBE_VALUE_UNKNOWN),
        .flags   = DMI_ATTRIBUTE_FLAG_SIGNED
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, resolution, DECIMAL, {
        .code    = "resolution",
        .name    = "Resolution",
        .scale   = 3,
        .unit    = DMI_UNIT_MILLIVOLT,
        .unknown = dmi_value_ptr(DMI_PROBE_VALUE_UNKNOWN),
        .flags   = DMI_ATTRIBUTE_FLAG_SIGNED
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, tolerance, DECIMAL, {
        .code    = "tolerance",
        .name    = "Tolerance",
        .scale   = 1,
        .unit    = DMI_UNIT_MILLIVOLT,
        .unknown = dmi_value_ptr(DMI_PROBE_VALUE_UNKNOWN),
        .flags   = DMI_ATTRIBUTE_FLAG_SIGNED
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, accuracy, DECIMAL, {
        .code    = "accuracy",
        .name    = "Accuracy",
        .scale   = 2,
        .unit    = DMI_UNIT_MILLIVOLT,
        .unknown = dmi_value_ptr(DMI_PROBE_VALUE_UNKNOWN),
        .flags   = DMI_ATTRIBUTE_FLAG_SIGNED
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, oem_defined, INTEGER, {
        .code    = "oem-defined",
        .name    = "OEM-defined",
        .flags   = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_voltage_probe_t, nominal_value, DECIMAL, {
        .code    = "nominal-value",
        .name    = "Nominal value",
        .scale   = 1,
        .unit    = DMI_UNIT_MILLIVOLT,
        .unknown = dmi_value_ptr(DMI_PROBE_VALUE_UNKNOWN),
        .flags   = DMI_ATTRIBUTE_FLAG_SIGNED
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_voltage_probe_spec =
{
    .code       = "voltage-probe",
    .name       = "Voltage probe",
    .type       = DMI_TYPE_VOLTAGE_PROBE,
    .min_length = 0x14,
    .attributes = dmi_voltage_probe_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_probe_decode,
        .free   = (dmi_entity_free_fn_t)dmi_probe_free
    }
};
