//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/memory-array.h>

static const dmi_name_set_t dmi_memory_array_location_names =
{
    .code  = "memory-array-locations",
    .names = {
        DMI_NAME_UNSPEC(DMI_MEMORY_ARRAY_LOCATION_UNSPEC),
        DMI_NAME_OTHER(DMI_MEMORY_ARRAY_LOCATION_OTHER),
        DMI_NAME_UNKNOWN(DMI_MEMORY_ARRAY_LOCATION_UNKNOWN),
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_MOTHERBOARD,
            .code = "motherboard",
            .name = "System board or motherboard"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_ISA,
            .code = "isa",
            .name = "ISA add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_EISA,
            .code = "eisa",
            .name = "EISA add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_PCI,
            .code = "pci",
            .name = "PCI add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_MCA,
            .code = "mca",
            .name = "MCA add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_PCMCIA,
            .code = "pcmcia",
            .name = "PCMCIA add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_PROPRIETARY,
            .code = "proprietary",
            .name = "Proprietary add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_NUBUS,
            .code = "nubus",
            .name = "NuBus"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_PC_98_C20,
            .code = "pc-98-c20",
            .name = "PC-98/C20 add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_PC_98_C24,
            .code = "pc-98-c24",
            .name = "PC-98/C24 add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_PC_98_E,
            .code = "pc-98-e",
            .name = "PC-98/E add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_PC_98_LOCAL_BUS,
            .code = "pc-98-local-bus",
            .name = "PC-98/Local bus add-on card"
        },
        {
            .id   = DMI_MEMORY_ARRAY_LOCATION_CXL,
            .code = "cxl",
            .name = "CXL add-on card"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_memory_array_usage_names =
{
    .code  = "memoery-array-usages",
    .names = {
        DMI_NAME_UNSPEC(DMI_MEMORY_ARRAY_USAGE_UNSPEC),
        DMI_NAME_OTHER(DMI_MEMORY_ARRAY_USAGE_OTHER),
        DMI_NAME_UNKNOWN(DMI_MEMORY_ARRAY_USAGE_UNKNOWN),
        {
            .id   = DMI_MEMORY_ARRAY_USAGE_SYSTEM,
            .code = "system",
            .name = "System memory"
        },
        {
            .id   = DMI_MEMORY_ARRAY_USAGE_VIDEO,
            .code = "video",
            .name = "Video memory"
        },
        {
            .id   = DMI_MEMORY_ARRAY_USAGE_FLASH,
            .code = "flash",
            .name = "Flash memory"
        },
        {
            .id   = DMI_MEMORY_ARRAY_USAGE_NVRAM,
            .code = "nvram",
            .name = "Non-volatile RAM"
        },
        {
            .id   = DMI_MEMORY_ARRAY_USAGE_CACHE,
            .code = "cache",
            .name = "Cache memory"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_memory_array_attrs[] =
{
    DMI_ATTRIBUTE(dmi_memory_array_t, location, ENUM, {
        .code    = "location",
        .name    = "Location",
        .unspec  = dmi_value_ptr(DMI_MEMORY_ARRAY_LOCATION_UNSPEC),
        .unknown = dmi_value_ptr(DMI_MEMORY_ARRAY_LOCATION_UNKNOWN),
        .values  = &dmi_memory_array_location_names
    }),
    DMI_ATTRIBUTE(dmi_memory_array_t, usage, ENUM, {
        .code    = "use",
        .name    = "Use",
        .unspec  = dmi_value_ptr(DMI_MEMORY_ARRAY_USAGE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_MEMORY_ARRAY_USAGE_UNKNOWN),
        .values  = &dmi_memory_array_usage_names
    }),
    DMI_ATTRIBUTE(dmi_memory_array_t, error_correction, ENUM, {
        .code    = "error-correction",
        .name    = "Memory error correction",
        .unspec  = dmi_value_ptr(DMI_ERROR_CORRECT_TYPE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_ERROR_CORRECT_TYPE_UNSPEC),
        .values  = &dmi_error_correct_type_names
    }),
    DMI_ATTRIBUTE(dmi_memory_array_t, maximum_capacity, SIZE, {
        .code    = "maximum-capacity",
        .name    = "Maximum capacity"
    }),
    DMI_ATTRIBUTE(dmi_memory_array_t, error_handle, HANDLE, {
        .code    = "error-handle",
        .name    = "Memory error information handle"
    }),
    DMI_ATTRIBUTE(dmi_memory_array_t, device_count, INTEGER, {
        .code    = "device-count",
        .name    = "Number of memory devices"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_memory_array_spec =
{
    .code        = "memory-array",
    .name        = "Physical memory array",
    .type        = DMI_TYPE_MEMORY_ARRAY,
    .min_version = DMI_VERSION(2, 1, 0),
    .min_length  = 0x0F,
    .attributes  = dmi_memory_array_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_memory_array_decode,
        .free   = (dmi_entity_free_fn_t)dmi_memory_array_free
    }
};

const char *dmi_memory_array_location_name(dmi_memory_array_location_t value)
{
    return dmi_name_lookup(&dmi_memory_array_location_names, value);
}

const char *dmi_memory_array_usage_name(dmi_memory_array_usage_t value)
{
    return dmi_name_lookup(&dmi_memory_array_usage_names, value);
}

dmi_memory_array_t *dmi_memory_array_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_memory_array_t *info;
    dmi_version_t level = dmi_version(2, 1, 0);
    const dmi_memory_array_data_t *data;
    dmi_dword_t maximum_capacity;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MEMORY_ARRAY));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    maximum_capacity = dmi_decode(data->maximum_capacity);

    info->location         = dmi_decode(data->location);
    info->usage            = dmi_decode(data->usage);
    info->error_correction = dmi_decode(data->error_correction);
    info->maximum_capacity = (dmi_size_t)(maximum_capacity & 0x7FFFFFFFU) << 10;
    info->error_handle     = dmi_decode(data->error_handle);
    info->device_count     = dmi_decode(data->device_count);

    if (entity->body_length > 0x0Fu) {
        level = dmi_version(2, 7, 0);

        if (maximum_capacity & 0x80000000)
            info->maximum_capacity = dmi_decode(data->maximum_capacity_ex);
    }

    if (plevel != nullptr)
        *plevel = level;

    return info;
}

void dmi_memory_array_free(dmi_memory_array_t *info)
{
    dmi_free(info);
}
