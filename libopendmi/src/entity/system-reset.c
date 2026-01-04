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
#include <opendmi/utils/decode.h>

#include <opendmi/entity/system-reset.h>

static const dmi_name_set_t dmi_boot_option_names =
{
    .code  = "boot-options",
    .names = {
        DMI_NAME_RESERVED(DMI_BOOT_OPTION_RESERVED),
        {
            .id   = DMI_BOOT_OPTION_OPERATING_SYSTEM,
            .code = "operating-system",
            .name = "Operating system"
        },
        {
            .id   = DMI_BOOT_OPTION_SYSTEM_UTILS,
            .code = "system-utils",
            .name = "System utilities"
        },
        {
            .id   = DMI_BOOT_OPTION_AVOID_REBOOT,
            .code = "avoid-reboot",
            .name = "Do not reboot"
        },
        DMI_NAME_NULL
    }
};

static const dmi_attribute_t dmi_system_reset_attrs[] =
{
    DMI_ATTRIBUTE(dmi_system_reset_t, status, BOOL, {
        .code    = "status",
        .name    = "Reset enabled"
    }),
    DMI_ATTRIBUTE(dmi_system_reset_t, boot_on_watchdog, ENUM, {
        .code    = "boot-on-watchdog",
        .name    = "Boot option on watchdog",
        .values  = &dmi_boot_option_names
    }),
    DMI_ATTRIBUTE(dmi_system_reset_t, boot_on_limit, ENUM, {
        .code    = "boot-on-limit",
        .name    = "Boot option on reset limit",
        .values  = &dmi_boot_option_names
    }),
    DMI_ATTRIBUTE(dmi_system_reset_t, watchdog_present, BOOL, {
        .code    = "watchdog-present",
        .name    = "Watchdog present"
    }),
    DMI_ATTRIBUTE(dmi_system_reset_t, reset_count, INTEGER, {
        .code    = "reset-count",
        .name    = "Reset count",
        .unknown = dmi_value_ptr((unsigned short)USHRT_MAX)
    }),
    DMI_ATTRIBUTE(dmi_system_reset_t, reset_limit, INTEGER, {
        .code    = "reset-limit",
        .name    = "Reset limit",
        .unknown = dmi_value_ptr((unsigned short)USHRT_MAX)
    }),
    DMI_ATTRIBUTE(dmi_system_reset_t, timer_interval, INTEGER, {
        .code    = "timer-interval",
        .name    = "Timer interval",
        .unit    = DMI_UNIT_MINUTE,
        .unknown = dmi_value_ptr((unsigned short)USHRT_MAX)
    }),
    DMI_ATTRIBUTE(dmi_system_reset_t, timeout, INTEGER, {
        .code    = "timeout",
        .name    = "Timeout",
        .unit    = DMI_UNIT_MINUTE,
        .unknown = dmi_value_ptr((unsigned short)USHRT_MAX),
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_system_reset_spec =
{
    .code        = "system-reset",
    .name        = "System reset",
    .type        = DMI_TYPE_SYSTEM_RESET,
    .min_version = DMI_VERSION(2, 2, 0),
    .min_length  = 0x0D,
    .attributes  = dmi_system_reset_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_system_reset_decode,
        .free   = (dmi_entity_free_fn_t)dmi_system_reset_free
    }
};

const char *dmi_boot_option_name(dmi_boot_option_t value)
{
    return dmi_name_lookup(&dmi_boot_option_names, value);
}

dmi_system_reset_t *dmi_system_reset_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_system_reset_t *info;
    const dmi_system_reset_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_SYSTEM_RESET));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    dmi_system_reset_caps_t caps = {
        .__value = dmi_decode(data->capabilities)
    };

    info->status           = caps.status;
    info->boot_on_watchdog = caps.boot_on_watchdog;
    info->boot_on_limit    = caps.boot_on_limit;
    info->watchdog_present = caps.watchdog_present;

    info->reset_count    = dmi_decode(data->reset_count);
    info->reset_limit    = dmi_decode(data->reset_limit);
    info->timer_interval = dmi_decode(data->timer_inverval);
    info->timeout        = dmi_decode(data->timeout);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 2, 0);

    return info;
}

void dmi_system_reset_free(dmi_system_reset_t *info)
{
    dmi_free(info);
}
