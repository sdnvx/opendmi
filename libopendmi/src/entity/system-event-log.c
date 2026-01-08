//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/system-event-log.h>

static const dmi_name_set_t dmi_system_log_access_method_names =
{
    .code  = "access-methods",
    .names = {
        {
            .id   = DMI_SYSTEM_LOG_ACCESS_METHOD_INDEXED_IO_8BIT_1_1,
            .code = "indexed-io-8bit-1-1",
            .name = "Indexed I/O: 1 8-bit index port, 1 8-bit data port"
        },
        {
            .id   = DMI_SYSTEM_LOG_ACCESS_METHOD_INDEXED_IO_8BIT_2_1,
            .code = "indexed-io-8bit-2-1",
            .name = "Indexed I/O: 2 8-bit index ports, 1 8-bit data port"
        },
        {
            .id   = DMI_SYSTEM_LOG_ACCESS_METHOD_INDEXED_IO_16BIT,
            .code = "indexed-io-16bit",
            .name = "Indexed I/O: 1 16-bit index port, 1 8-bit data port"
        },
        {
            .id   = DMI_SYSTEM_LOG_ACCESS_METHOD_MMAP_32BIT_ADDR,
            .code = "memory-mapped-32bit-address",
            .name = "Memory-mapped physical 32-bit address"
        },
        {
            .id   = DMI_SYSTEM_LOG_ACCESS_METHOD_DATA_FUNCTIONS,
            .code = "data-functions",
            .name = "General-purpose non-volatile data functions"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_system_log_status_names =
{
    .code = "status",
    .names = {
        {
            .id   = 0,
            .code = "log-area-valid",
            .name = "Log area valid"
        },
        {
            .id   = 1,
            .code = "log-area-full",
            .name = "Log area full"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_system_event_log_attrs[] =
{
    DMI_ATTRIBUTE(dmi_system_event_log_t, access_method, ENUM, {
        .code   = "access-method",
        .name   = "Access Method",
        .values = &dmi_system_log_access_method_names
    }),
    DMI_ATTRIBUTE(dmi_system_event_log_t, status, SET, {
        .code = "status",
        .name = "Status",
        .values = &dmi_system_log_status_names,
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_system_event_log_spec =
{
    .code        = "system-event-log",
    .name        = "System event log",
    .type        = DMI_TYPE_SYSTEM_EVENT_LOG,
    .min_version = DMI_VERSION(2, 0, 0),
    .min_length  = 0x14,
    .attributes  = dmi_system_event_log_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_system_event_log_decode,
        .free   = (dmi_entity_free_fn_t)dmi_system_event_log_free
    }
};

dmi_system_event_log_t *
dmi_system_event_log_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_system_event_log_t *info;
    dmi_version_t level = dmi_version(2, 0, 0);
    const dmi_system_event_log_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_SYSTEM_EVENT_LOG));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->access_method  = dmi_decode(data->access_method);
    info->status.__value = dmi_decode(data->status);

    if (entity->body_length > 0x14u) {
        level = dmi_version(2, 1, 0);
    }

    if (plevel != nullptr)
        *plevel = level;

    return info;
}

void dmi_system_event_log_free(dmi_system_event_log_t *info)
{
    dmi_free(info);
}
