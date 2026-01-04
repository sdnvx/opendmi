//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <limits.h>

#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/memory-device-addr.h>

const dmi_attribute_t dmi_memory_device_addr_attrs[] =
{
    DMI_ATTRIBUTE(dmi_memory_device_addr_t, start_addr, ADDRESS, {
        .code    = "start-addr",
        .name    = "Starting address",
        .flags   = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_memory_device_addr_t, end_addr, ADDRESS, {
        .code    = "end-addr",
        .name    = "Ending address",
        .flags   = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_memory_device_addr_t, range_size, SIZE, {
        .code    = "range-size",
        .name    = "Range size"
    }),
    DMI_ATTRIBUTE(dmi_memory_device_addr_t, device_handle, HANDLE, {
        .code    = "device-handle",
        .name    = "Device handle"
    }),
    DMI_ATTRIBUTE(dmi_memory_device_addr_t, array_addr_handle, HANDLE, {
        .code    = "array-addr-handle",
        .name    = "Array mapped address handle"
    }),
    DMI_ATTRIBUTE(dmi_memory_device_addr_t, partition_pos, INTEGER, {
        .code    = "partition-pos",
        .name    = "Partition row position",
        .unknown = dmi_value_ptr((unsigned short)USHRT_MAX)
    }),
    DMI_ATTRIBUTE(dmi_memory_device_addr_t, interleave_pos, INTEGER, {
        .code    = "interleave-pos",
        .name    = "Interleave position",
        .unknown = dmi_value_ptr((unsigned short)USHRT_MAX)
    }),
    DMI_ATTRIBUTE(dmi_memory_device_addr_t, interleave_depth, INTEGER, {
        .code    = "interleave-depth",
        .name    = "Interleave data depth",
        .unknown = dmi_value_ptr((unsigned short)USHRT_MAX)
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_memory_device_addr_spec =
{
    .code        = "memory-device-address",
    .name        = "Memory device mapped address",
    .type        = DMI_TYPE_MEMORY_DEVICE_ADDR,
    .min_version = DMI_VERSION(2, 1, 0),
    .min_length  = 0x13,
    .attributes  = dmi_memory_device_addr_attrs,
    .handlers    = {
        .validate = (dmi_entity_validate_fn_t)dmi_memory_device_addr_validate,
        .decode   = (dmi_entity_decode_fn_t)dmi_memory_device_addr_decode,
        .link     = (dmi_entity_link_fn_t)dmi_memory_device_addr_link,
        .free     = (dmi_entity_free_fn_t)dmi_memory_device_addr_free
    }
};

bool dmi_memory_device_addr_validate(const dmi_entity_t *entity)
{
    dmi_memory_device_addr_data_t *data = dmi_cast(data, entity->data);

    if (data->start_addr == 0xFFFFFFFFu) {
        if (data->end_addr != 0xFFFFFFFFu)
            return false;

        if (entity->total_length < 0x13)
            return false;
        if (data->end_addr_ex <= data->start_addr_ex)
            return false;
    } else {
        if (data->end_addr == 0xFFFFFFFFu)
            return false;
        if (data->end_addr <= data->start_addr)
            return false;

        if (entity->total_length >= 0x13) {
            if (data->start_addr_ex != 0)
                return false;
            if (data->end_addr_ex != 0)
                return false;
        }
    }

    return true;
}

dmi_memory_device_addr_t *dmi_memory_device_addr_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_memory_device_addr_t *info;
    dmi_version_t level = dmi_version(2, 1, 0);
    const dmi_memory_device_addr_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MEMORY_DEVICE_ADDR));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    uint32_t start_addr = dmi_decode(data->start_addr);
    uint32_t end_addr   = dmi_decode(data->end_addr);

    info->start_addr        = start_addr << 10;
    info->end_addr          = end_addr << 10;
    info->device_handle     = dmi_decode(data->device_handle);
    info->array_addr_handle = dmi_decode(data->array_addr_handle);

    info->partition_pos    = data->partition_pos != 0xFFu ?
                             data->partition_pos : USHRT_MAX;
    info->interleave_pos   = data->interleave_pos != 0xFFu ?
                             data->interleave_pos : USHRT_MAX;
    info->interleave_depth = data->interleave_depth != 0xFFu ?
                             data->interleave_depth : USHRT_MAX;

    if (entity->body_length >= 0x13) {
        level = dmi_version(2, 7, 0);

        if (start_addr == 0xFFFFFFFFu)
            info->start_addr = dmi_decode(data->start_addr_ex);
        if (end_addr == 0xFFFFFFFFu)
            info->end_addr   = dmi_decode(data->end_addr_ex);
    }

    if (info->end_addr > info->start_addr)
        info->range_size = info->end_addr - info->start_addr;
    else
        info->range_size = info->start_addr - info->end_addr;

    if (plevel != nullptr)
        *plevel = level;

    return info;
}

bool dmi_memory_device_addr_link(dmi_entity_t *entity)
{
    dmi_memory_device_addr_t *info;
    dmi_registry_t *registry;

    info = dmi_cast(info, dmi_entity_info(entity, DMI_TYPE_MEMORY_DEVICE_ADDR));
    if (info == nullptr)
        return false;

    registry = entity->context->registry;

    info->device = dmi_registry_get(registry, info->device_handle, DMI_TYPE_MEMORY_DEVICE, false);
    if (info->device == nullptr)
        return false;

    info->array_addr = dmi_registry_get(registry, info->array_addr_handle, DMI_TYPE_MEMORY_ARRAY_ADDR, false);
    if (info->array_addr == nullptr) {
        dmi_log_warning(entity->context, "Memory array address not found: 0x%04x", info->array_addr_handle);
    }

    return true;
}

void dmi_memory_device_addr_free(dmi_memory_device_addr_t *info)
{
    dmi_free(info);
}
