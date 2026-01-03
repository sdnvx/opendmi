//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>

#include <opendmi/entity/memory-array-addr.h>

const dmi_attribute_t dmi_memory_array_addr_attrs[] =
{
    DMI_ATTRIBUTE(dmi_memory_array_addr_t, start_addr, ADDRESS, {
        .code   = "start-addr",
        .name   = "Starting address",
        .flags  = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_memory_array_addr_t, end_addr, ADDRESS, {
        .code  = "end-addr",
        .name  = "Ending address",
        .flags = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_memory_array_addr_t, range_size, SIZE, {
        .code   = "range-size",
        .name   = "Range size"
    }),
    DMI_ATTRIBUTE(dmi_memory_array_addr_t, array_handle, HANDLE, {
        .code   = "array-handle",
        .name   = "Memory array handle"
    }),
    DMI_ATTRIBUTE(dmi_memory_array_addr_t, partition_width, INTEGER, {
        .code   = "partition-width",
        .name   = "Partition width"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_memory_array_addr_spec =
{
    .code        = "memory-array-address",
    .name        = "Memory array mapped address",
    .type        = DMI_TYPE_MEMORY_ARRAY_ADDR,
    .min_version = DMI_VERSION(2, 1, 0),
    .min_length  = 0x0F,
    .attributes  = dmi_memory_array_addr_attrs,
    .handlers    = {
        .validate = (dmi_entity_validate_fn_t)dmi_memory_array_addr_validate,
        .decode   = (dmi_entity_decode_fn_t)dmi_memory_array_addr_decode,
        .link     = (dmi_entity_link_fn_t)dmi_memory_array_addr_link,
        .free     = (dmi_entity_free_fn_t)dmi_memory_array_addr_free
    }
};

bool dmi_memory_array_addr_validate(const dmi_entity_t *entity)
{
    dmi_memory_array_addr_data_t *data = dmi_cast(data, entity->data);

    if (data->start_addr == 0xFFFFFFFFU) {
        if (data->end_addr != 0xFFFFFFFFU)
            return false;

        if (entity->total_length < 0x1F)
            return false;
        if (data->end_addr_ex <= data->start_addr_ex)
            return false;
    } else {
        if (data->end_addr == 0xFFFFFFFFU)
            return false;
        if (data->end_addr <= data->start_addr)
            return false;

        if (entity->total_length >= 0x1F) {
            if (data->start_addr_ex != 0)
                return false;
            if (data->end_addr_ex != 0)
                return false;
        }
    }

    return true;
}

dmi_memory_array_addr_t *dmi_memory_array_addr_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_memory_array_addr_t *info;
    dmi_version_t level = dmi_version(2, 1, 0);
    const dmi_memory_array_addr_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MEMORY_ARRAY_ADDR));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->start_addr      = dmi_value(data->start_addr) << 10;
    info->end_addr        = dmi_value(data->end_addr) << 10;
    info->array_handle    = dmi_value(data->array_handle);
    info->partition_width = dmi_value(data->partition_width);

    if (entity->body_length >= 0x1F) {
        level = dmi_version(2, 7, 0);

        if (data->start_addr == 0xFFFFFFFFu) {
            info->start_addr = dmi_value(data->start_addr_ex);
            info->end_addr   = dmi_value(data->end_addr_ex);
        }
    }

    if (info->end_addr > info->start_addr)
        info->range_size = info->end_addr - info->start_addr;
    else
        info->range_size = info->start_addr - info->end_addr;

    if (plevel != nullptr)
        *plevel = level;

    return info;
}

bool dmi_memory_array_addr_link(dmi_entity_t *entity)
{
    dmi_memory_array_addr_t *info;
    dmi_registry_t *registry;

    info = dmi_cast(info, dmi_entity_info(entity, DMI_TYPE_MEMORY_ARRAY_ADDR));
    if (info == nullptr)
        return false;

    registry = entity->context->registry;

    info->array = dmi_registry_get(registry, info->array_handle, DMI_TYPE_MEMORY_ARRAY, false);

    return true;
}

void dmi_memory_array_addr_free(dmi_memory_array_addr_t *info)
{
    dmi_free(info);
}
