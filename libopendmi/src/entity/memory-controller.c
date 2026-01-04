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

#include <opendmi/entity/memory-controller.h>

static const dmi_name_set_t dmi_error_detect_method_names =
{
    .code  = "error-detect-methods",
    .names = {
        DMI_NAME_UNSPEC(DMI_ERROR_DETECT_METHOD_UNSPEC),
        DMI_NAME_OTHER(DMI_ERROR_DETECT_METHOD_OTHER),
        DMI_NAME_UNKNOWN(DMI_ERROR_DETECT_METHOD_UNKNOWN),
        DMI_NAME_NONE(DMI_ERROR_DETECT_METHOD_NONE),
        {
            .id   = DMI_ERROR_DETECT_METHOD_PARITY,
            .code = "parity",
            .name = "8-bit Parity"
        },
        {
            .id   = DMI_ERROR_DETECT_METHOD_ECC_32,
            .code = "ecc-32",
            .name = "32-bit ECC"
        },
        {
            .id   = DMI_ERROR_DETECT_METHOD_ECC_64,
            .code = "ecc-64",
            .name = "64-bit ECC"
        },
        {
            .id   = DMI_ERROR_DETECT_METHOD_ECC_128,
            .code = "ecc-128",
            .name = "128-bit ECC"
        },
        {
            .id   = DMI_ERROR_DETECT_METHOD_CRC,
            .code = "crc",
            .name = "CRC"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_error_correct_caps_names =
{
    .code  = "error-correct-caps",
    .names = {
        DMI_NAME_OTHER(0),
        DMI_NAME_UNKNOWN(1),
        DMI_NAME_NONE(2),
        {
            .id   = 3,
            .code = "single-bit",
            .name = "Single-bit error correcting"
        },
        {
            .id   = 4,
            .code = "double-bit",
            .name = "Double-bit error correcting"
        },
        {
            .id   = 5,
            .code = "scrubbing",
            .name = "Error scrubbing"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_memory_module_speed_names =
{
    .code  = "memory-module-speeds",
    .names = {
        DMI_NAME_OTHER(0),
        DMI_NAME_UNKNOWN(1),
        {
            .id   = 2,
            .code = "70ns",
            .name = "70 ns"
        },
        {
            .id   = 3,
            .code = "60ns",
            .name = "60 ns"
        },
        {
            .id   = 4,
            .code = "50ns",
            .name = "50 ns"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_memory_interleave_names =
{
    .code  = "memory-interleaves",
    .names = {
        DMI_NAME_UNSPEC(DMI_MEMORY_INTERLEAVE_UNSPEC),
        DMI_NAME_OTHER(DMI_MEMORY_INTERLEAVE_OTHER),
        DMI_NAME_UNKNOWN(DMI_MEMORY_INTERLEAVE_UNKNOWN),
        {
            .id   = DMI_MEMORY_INTERLEAVE_1WAY,
            .code = "1-way",
            .name = "One-Way Interleave"
        },
        {
            .id   = DMI_MEMORY_INTERLEAVE_2WAY,
            .code = "2-way",
            .name = "Two-Way Interleave"
        },
        {
            .id   = DMI_MEMORY_INTERLEAVE_4WAY,
            .code = "4-way",
            .name = "Four-Way Interleave"
        },
        {
            .id   = DMI_MEMORY_INTERLEAVE_8WAY,
            .code = "8-way",
            .name = "Eight-Way Interleave"
        },
        {
            .id   = DMI_MEMORY_INTERLEAVE_16WAY,
            .code = "16-way",
            .name = "Sixteen-Way Interleave"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_memory_module_voltage_names =
{
    .code  = "memory-module-voltages",
    .names = {
        {
            .id   = 0,
            .code = "5v",
            .name = "5V"
        },
        {
            .id   = 1,
            .code = "3v3",
            .name = "3.3V"
        },
        {
            .id   = 2,
            .code = "2v9",
            .name = "2.9V"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_memory_controller_attrs[] =
{
    DMI_ATTRIBUTE(dmi_memory_controller_t, error_detection, ENUM, {
        .code   = "error-detection",
        .name   = "Error detecting method",
        .values = &dmi_error_detect_method_names
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, error_correction, SET, {
        .code   = "error-correction",
        .name   = "Error correcting capabilities",
        .values = &dmi_error_correct_caps_names
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, supported_interleave, ENUM, {
        .code   = "supported-interleave",
        .name   = "Supported interleave",
        .values = &dmi_memory_interleave_names
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, current_interleave, ENUM, {
        .code   = "current-interleave",
        .name   = "Current interleave",
        .values = &dmi_memory_interleave_names
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, maximum_module_size, SIZE, {
        .code   = "maximum-module-size",
        .name   = "Maximum module size"
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, maximum_memory_size, SIZE, {
        .code   = "maximum-memory-size",
        .name   = "Maximum memory size"
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, supported_speeds, SET, {
        .code   = "supported-speeds",
        .name   = "Supported speeds",
        .values = &dmi_memory_module_speed_names
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, supported_types, SET, {
        .code   = "supported-types",
        .name   = "Supported types",
        .values = &dmi_memory_module_type_names
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, required_voltages, SET, {
        .code   = "required-voltages",
        .name   = "Required voltages",
        .values = &dmi_memory_module_voltage_names
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, slot_count, INTEGER, {
        .code   = "slot-count",
        .name   = "Slot count"
    }),
    DMI_ATTRIBUTE_ARRAY(dmi_memory_controller_t, module_handles, slot_count, HANDLE, {
        .code   = "module-handles",
        .name   = "Module handles"
    }),
    DMI_ATTRIBUTE(dmi_memory_controller_t, enabled_error_correction, SET, {
        .code   = "error-correction",
        .name   = "Error correcting capabilities",
        .values = &dmi_error_correct_caps_names,
        .level  = DMI_VERSION(2, 1, 0)
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_memory_controller_spec =
{
    .code       = "memory-controller",
    .name       = "Memory controller information",
    .type       = DMI_TYPE_MEMORY_CONTROLLER,
    .min_length = 0x08,
    .attributes = dmi_memory_controller_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_memory_controller_decode,
        .free   = (dmi_entity_free_fn_t)dmi_memory_controller_free
    }
};

const char *dmi_error_detect_method_name(dmi_error_detect_method_t value)
{
    return dmi_name_lookup(&dmi_error_detect_method_names, value);
}

const char *dmi_memory_interleave_name(dmi_memory_interleave_t value)
{
    return dmi_name_lookup(&dmi_memory_interleave_names, value);
}

dmi_memory_controller_t *dmi_memory_controller_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_memory_controller_t *info;
    dmi_version_t level = dmi_version(2, 0, 0);
    const dmi_memory_controller_data_t *data;
    const dmi_memory_controller_extra_t *extra;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MEMORY_CONTROLLER));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->error_detection  = dmi_decode(data->error_detection);
    info->error_correction = (dmi_error_correct_caps_t) {
        .__value = dmi_decode(data->error_correction)
    };

    info->supported_interleave = dmi_decode(data->supported_interleave);
    info->current_interleave   = dmi_decode(data->current_interleave);
    info->slot_count           = dmi_decode(data->slot_count);
    info->maximum_module_size  = 1 << dmi_decode(data->maximum_module_size);
    info->maximum_memory_size  = info->maximum_module_size * info->slot_count;

    info->supported_speeds = (dmi_memory_module_speed_t) {
        .__value = dmi_decode(data->supported_speeds)
    };

    info->supported_types = (dmi_memory_module_type_t) {
        .__value = dmi_decode(data->supported_types)
    };

    info->required_voltages = (dmi_memory_module_voltage_t) {
        .__value = dmi_decode(data->required_voltages)
    };

    info->module_handles = dmi_alloc_array(entity->context, sizeof(dmi_handle_t),
                                           info->slot_count);
    if (info->module_handles == nullptr) {
        dmi_free(info);
        return nullptr;
    }

    for (size_t i = 0; i < info->slot_count; i++) {
        info->module_handles[i] = dmi_decode(data->module_handles[i]);
    }

    const dmi_data_t *extra_start = entity->data + sizeof(*data) +
                                    sizeof(dmi_handle_t) * info->slot_count;

    if (entity->body_length > (size_t)(extra_start - entity->data)) {
        level = dmi_version(2, 1, 0);
        extra = dmi_cast(extra, extra_start);

        info->enabled_error_correction = (dmi_error_correct_caps_t) {
            .__value = dmi_decode(extra->enabled_error_correction)
        };
    }

    if (plevel != nullptr)
        *plevel = level;

    return info;
}

void dmi_memory_controller_free(dmi_memory_controller_t *info)
{
    dmi_free(info);
}
