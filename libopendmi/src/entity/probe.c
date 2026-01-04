//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <limits.h>

#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/probe.h>

static short dmi_probe_value(dmi_word_t value);

const dmi_name_set_t dmi_probe_location_names =
{
    .code  = "probe-locations",
    .names = {
        DMI_NAME_UNSPEC(DMI_PROBE_LOCATION_UNSPEC),
        DMI_NAME_OTHER(DMI_PROBE_LOCATION_OTHER),
        DMI_NAME_UNKNOWN(DMI_PROBE_LOCATION_UNKNOWN),
        {
            .id   = DMI_PROBE_LOCATION_PROCESSOR,
            .code = "processor",
            .name = "Processor"
        },
        {
            .id   = DMI_PROBE_LOCATION_DISK,
            .code = "disk",
            .name = "Disk"
        },
        {
            .id   = DMI_PROBE_LOCATION_PERIPHERAL_BAY,
            .code = "peripheral-bay",
            .name = "Peripheral bay"
        },
        {
            .id   = DMI_PROBE_LOCATION_SYSTEM_MGMT_MODULE,
            .code = "system-mgmt-module",
            .name = "System management module"
        },
        {
            .id   = DMI_PROBE_LOCATION_MOTHERBOARD,
            .code = "motherboard",
            .name = "Motherboard"
        },
        {
            .id   = DMI_PROBE_LOCATION_MEMORY_MODULE,
            .code = "memory-module",
            .name = "Memory module"
        },
        {
            .id   = DMI_PROBE_LOCATION_PROCESSOR_MODULE,
            .code = "processor-module",
            .name = "Processor module"
        },
        {
            .id   = DMI_PROBE_LOCATION_POWER_UNIT,
            .code = "power-unit",
            .name = "Power unit"
        },
        {
            .id   = DMI_PROBE_LOCATION_ADDIN_CARD,
            .code = "addin-card",
            .name = "Add-in card"
        },
        {
            .id   = DMI_PROBE_LOCATION_FRONT_PANEL_BOARD,
            .code = "front-panel-board",
            .name = "Front panel board"
        },
        {
            .id   = DMI_PROBE_LOCATION_BACK_PANEL_BOARD,
            .code = "back-panel-board",
            .name = "Back panel board"
        },
        {
            .id   = DMI_PROBE_LOCATION_POWER_SYSTEM_BOARD,
            .code = "power-system-board",
            .name = "Power system board"
        },
        {
            .id   = DMI_PROBE_LOCATION_DRIVE_BACK_PLANE,
            .code = "drive-back-plane",
            .name = "Drive back plane"
        },
        DMI_NAME_NULL
    }
};

const char *dmi_probe_location_name(dmi_probe_location_t value)
{
    return dmi_name_lookup(&dmi_probe_location_names, value);
}

dmi_probe_t * dmi_probe_decode(dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_probe_t *info = nullptr;
    dmi_probe_data_t *data = dmi_cast(data, entity->data);

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->description = dmi_entity_string(entity, data->description);

    dmi_probe_details_t details = {
        .__value = dmi_decode(data->details)
    };

    info->location      = details.location;
    info->status        = details.status;
    info->maximum_value = dmi_probe_value(data->maximum_value);
    info->minimum_value = dmi_probe_value(data->minimum_value);
    info->resolution    = dmi_probe_value(data->resolution);
    info->tolerance     = dmi_probe_value(data->tolerance);
    info->accuracy      = dmi_probe_value(data->accuracy);
    info->oem_defined   = dmi_decode(data->oem_defined);

    if (entity->body_length > 0x14)
        info->nominal_value = dmi_probe_value(data->nominal_value);
    else
        info->nominal_value = SHRT_MIN;

    if (plevel != nullptr)
        *plevel = dmi_version(2, 2, 0);

    return info;
}

static short dmi_probe_value(dmi_word_t value)
{
    value = dmi_decode(value);

    if (value == DMI_PROBE_VALUE_UNKNOWN)
        return SHRT_MIN;

    return (int16_t)value;
}

void dmi_probe_free(dmi_probe_t *info)
{
    dmi_free(info);
}
