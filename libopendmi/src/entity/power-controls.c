//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/power-controls.h>

const dmi_attribute_t dmi_power_controls_attrs[] =
{
    DMI_ATTRIBUTE(dmi_power_controls_t, poweron_month, INTEGER, {
        .code = "poweron-month",
        .name = "Next power-on month"
    }),
    DMI_ATTRIBUTE(dmi_power_controls_t, poweron_day, INTEGER, {
        .code = "poweron-day",
        .name = "Next power-on day of month"
    }),
    DMI_ATTRIBUTE(dmi_power_controls_t, poweron_hour, INTEGER, {
        .code = "poweron-hour",
        .name = "Next power-on hour"
    }),
    DMI_ATTRIBUTE(dmi_power_controls_t, poweron_minute, INTEGER, {
        .code = "poweron-minute",
        .name = "Next power-on minute"
    }),
    DMI_ATTRIBUTE(dmi_power_controls_t, poweron_second, INTEGER, {
        .code = "poweron-second",
        .name = "Next power-on second"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_power_controls_spec =
{
    .code        = "power-controls",
    .name        = "System power controls",
    .type        = DMI_TYPE_POWER_CONTROLS,
    .min_version = DMI_VERSION(2, 2, 0),
    .min_length  = 0x09,
    .attributes  = dmi_power_controls_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_power_controls_decode,
        .free   = (dmi_entity_free_fn_t)dmi_power_controls_free
    }
};

dmi_power_controls_t *dmi_power_controls_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_power_controls_t *info;
    const dmi_power_controls_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_POWER_CONTROLS));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->poweron_month  = dmi_cast(info->poweron_month,
                                    dmi_decode_bcd(data->poweron_month));
    info->poweron_day    = dmi_cast(info->poweron_day,
                                    dmi_decode_bcd(data->poweron_day));
    info->poweron_hour   = dmi_cast(info->poweron_hour,
                                    dmi_decode_bcd(data->poweron_hour));
    info->poweron_minute = dmi_cast(info->poweron_minute,
                                    dmi_decode_bcd(data->poweron_minute));
    info->poweron_second = dmi_cast(info->poweron_second,
                                    dmi_decode_bcd(data->poweron_second));

    if (plevel != nullptr)
        *plevel = dmi_version(2, 2, 0);

    return info;
}

void dmi_power_controls_free(dmi_power_controls_t *info)
{
    dmi_free(info);
}
