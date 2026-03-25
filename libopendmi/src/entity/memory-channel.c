//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/name.h>
#include <opendmi/utils/codec.h>

#include <opendmi/entity/memory-channel.h>
#include <opendmi/entity/memory-device.h>

static bool dmi_memory_channel_decode(dmi_entity_t *entity);
static bool dmi_memory_channel_link(dmi_entity_t *entity);
static void dmi_memory_channel_cleanup(dmi_entity_t *entity);

static const dmi_name_set_t dmi_memory_channel_type_names =
{
    .code  = "memory-channel-types",
    .names = (dmi_name_t[]){
        DMI_NAME_UNSPEC(DMI_MEMORY_CHANNEL_TYPE_UNSPEC),
        DMI_NAME_OTHER(DMI_MEMORY_CHANNEL_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_MEMORY_CHANNEL_TYPE_UNKNOWN),
        {
            .id   = DMI_MEMORY_CHANNEL_TYPE_RAMBUS,
            .code = "rambus",
            .name = "RamBus"
        },
        {
            .id   = DMI_MEMORY_CHANNEL_TYPE_SYNCLINK,
            .code = "synclink",
            .name = "SyncLink"
        },
        DMI_NAME_NULL
    }
};

const dmi_entity_spec_t dmi_memory_channel_spec =
{
    .code            = "memory-channel",
    .name            = "Memory channel",
    .type            = DMI_TYPE(MEMORY_CHANNEL),
    .minimum_version = DMI_VERSION(2, 3, 0),
    .minimum_length  = 0x0A,
    .decoded_length  = sizeof(dmi_memory_channel_t),
    .attributes      = (dmi_attribute_t[]){
        DMI_ATTRIBUTE(dmi_memory_channel_t, type, ENUM, {
            .code    = "type",
            .name    = "Type",
            .unspec  = dmi_value_ptr(DMI_MEMORY_CHANNEL_TYPE_UNSPEC),
            .unknown = dmi_value_ptr(DMI_MEMORY_CHANNEL_TYPE_UNKNOWN),
            .values  = &dmi_memory_channel_type_names,
        }),
        DMI_ATTRIBUTE(dmi_memory_channel_t, maximum_load, INTEGER, {
            .code    = "maximum-load",
            .name    = "Maximum load"
        }),
        DMI_ATTRIBUTE(dmi_memory_channel_t, device_count, INTEGER, {
            .code    = "device-count",
            .name    = "Device count"
        }),
        DMI_ATTRIBUTE_ARRAY(dmi_memory_channel_t, devices, device_count, STRUCT, {
            .code    = "devices",
            .name    = "Devices",
            .attrs   = (dmi_attribute_t[]){
                DMI_ATTRIBUTE(dmi_memory_channel_device_t, load, INTEGER, {
                    .code = "load",
                    .name = "Load"
                }),
                DMI_ATTRIBUTE(dmi_memory_channel_device_t, handle, HANDLE, {
                    .code = "handle",
                    .name = "Handle"
                }),
                DMI_ATTRIBUTE_NULL
            }
        }),
        DMI_ATTRIBUTE_NULL
    },
    .handlers = {
        .decode  = dmi_memory_channel_decode,
        .link    = dmi_memory_channel_link,
        .cleanup = dmi_memory_channel_cleanup
    }
};

const char *dmi_memory_channel_type_name(dmi_memory_channel_type_t value)
{
    return dmi_name_lookup(&dmi_memory_channel_type_names, value);
}

static bool dmi_memory_channel_decode(dmi_entity_t *entity)
{
    dmi_memory_channel_t *info;
    const dmi_memory_channel_data_t *data;

    data = dmi_entity_data(entity, DMI_TYPE_MEMORY_CHANNEL);
    if (data == nullptr)
        return false;

    info = dmi_entity_info(entity, DMI_TYPE_MEMORY_CHANNEL);
    if (info == nullptr)
        return false;

    info->type         = dmi_decode(data->type);
    info->maximum_load = dmi_decode(data->maximum_load);
    info->device_count = dmi_decode(data->device_count);

    info->devices = dmi_alloc_array(entity->context, sizeof(dmi_memory_channel_device_t),
                                    info->device_count);
    if (info->devices == nullptr)
        return false;

    for (size_t i = 0; i < info->device_count; i++) {
        dmi_memory_channel_device_t *device = &info->devices[i];
        const dmi_memory_channel_device_data_t *device_data = &data->devices[i];

        device->load   = dmi_decode(device_data->load);
        device->handle = dmi_decode(device_data->handle);
    }

    return true;
}

static bool dmi_memory_channel_link(dmi_entity_t *entity)
{
    dmi_memory_channel_t *info;
    dmi_registry_t *registry;
    dmi_entity_t *device;

    info = dmi_entity_info(entity, DMI_TYPE_MEMORY_CHANNEL);
    if (info == nullptr)
        return false;

    registry = entity->context->registry;

    for (size_t i = 0; i < info->device_count; i++) {
        device = dmi_registry_get(registry, info->devices[i].handle, DMI_TYPE_MEMORY_DEVICE, false);
        if (device == nullptr)
            continue;

        dmi_memory_device_t *device_info = dmi_cast(device_info, device->info);

        info->devices[i].device = device;
        device_info->channel    = entity;
    }

    return true;
}

static void dmi_memory_channel_cleanup(dmi_entity_t *entity)
{
    dmi_memory_channel_t *info;

    info = dmi_entity_info(entity, DMI_TYPE_MEMORY_CHANNEL);
    if (info == nullptr)
        return;

    dmi_free(info->devices);
}
