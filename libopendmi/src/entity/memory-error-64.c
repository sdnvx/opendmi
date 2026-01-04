//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/memory-error-64.h>

const dmi_entity_spec_t dmi_memory_error_64_spec =
{
    .code        = "memory-error-64",
    .name        = "64-bit memory error information",
    .type        = DMI_TYPE_MEMORY_ERROR_64,
    .min_version = DMI_VERSION(2, 3, 0),
    .min_length  = 0x1F,
    .attributes  = dmi_memory_error_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_memory_error_64_decode,
        .free   = (dmi_entity_free_fn_t)dmi_memory_error_free
    }
};

dmi_memory_error_t *dmi_memory_error_64_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_memory_error_t *info;
    const dmi_memory_error_64_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MEMORY_ERROR_64));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->type            = dmi_decode(data->type);
    info->granularity     = dmi_decode(data->granularity);
    info->operation       = dmi_decode(data->operation);
    info->vendor_syndrome = dmi_decode(data->vendor_syndrome);
    info->array_addr      = dmi_decode(data->array_addr);
    info->device_addr     = dmi_decode(data->device_addr);
    info->resolution      = dmi_decode(data->resolution);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 3, 0);

    return info;
}
