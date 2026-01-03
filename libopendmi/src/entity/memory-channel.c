//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/utils.h>

#include <opendmi/entity/memory-channel.h>

static const dmi_name_set_t dmi_memory_channel_type_names =
{
    .code  = "memory-channel-types",
    .names = {
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

static const dmi_attribute_t dmi_memory_device_channel_attrs[] =
{
    DMI_ATTRIBUTE(dmi_memory_channel_device_t, load, INTEGER, {
        .code = "load",
        .name = "Load"
    }),
    DMI_ATTRIBUTE(dmi_memory_channel_device_t, handle, HANDLE, {
        .code = "handle",
        .name = "Handle"
    }),
    DMI_ATTRIBUTE_NULL
};

static const dmi_attribute_t dmi_memory_channel_attrs[] =
{
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
        .attrs   = dmi_memory_device_channel_attrs
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_memory_channel_spec =
{
    .code        = "memory-channel",
    .name        = "Memory channel",
    .type        = DMI_TYPE_MEMORY_CHANNEL,
    .min_version = DMI_VERSION(2, 3, 0),
    .min_length  = 0x0A,
    .attributes  = dmi_memory_channel_attrs,
    .handlers = {
        .decode = (dmi_entity_decode_fn_t)dmi_memory_channel_decode,
        .link   = (dmi_entity_link_fn_t)dmi_memory_channel_link,
        .free   = (dmi_entity_free_fn_t)dmi_memory_channel_free
    }
};

const char *dmi_memory_channel_type_name(dmi_memory_channel_type_t value)
{
    return dmi_name_lookup(&dmi_memory_channel_type_names, value);
}

dmi_memory_channel_t *dmi_memory_channel_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_memory_channel_t *info;
    const dmi_memory_channel_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MEMORY_CHANNEL));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->type         = dmi_value(data->type);
    info->maximum_load = dmi_value(data->maximum_load);
    info->device_count = dmi_value(data->device_count);

    info->devices = dmi_alloc_array(entity->context, sizeof(dmi_memory_channel_device_t),
                                    info->device_count);
    if (info->devices == nullptr) {
        dmi_free(info);
        return nullptr;
    }

    for (size_t i = 0; i < info->device_count; i++) {
        dmi_memory_channel_device_t *device = &info->devices[i];
        const dmi_memory_channel_device_data_t *device_data = &data->devices[i];

        device->load   = dmi_value(device_data->load);
        device->handle = dmi_value(device_data->handle);
    }

    if (plevel != nullptr)
        *plevel = dmi_version(2, 3, 0);

    return info;
}

bool dmi_memory_channel_link(dmi_entity_t *entity)
{
    dmi_memory_channel_t *info;
    dmi_registry_t *registry;
    dmi_entity_t *device;

    info = dmi_cast(info, dmi_entity_info(entity, DMI_TYPE_MEMORY_CHANNEL));
    if (info == nullptr)
        return false;

    registry = entity->context->registry;

    for (size_t i = 0; i < info->device_count; i++) {
        device = dmi_registry_get(registry, info->devices[i].handle, DMI_TYPE_MEMORY_DEVICE, false);
        if (device == nullptr)
            continue;

        info->devices[i].device = device;
    }

    return true;
}

void dmi_memory_channel_free(dmi_memory_channel_t *info)
{
    dmi_free(info);
}
