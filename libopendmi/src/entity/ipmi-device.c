//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <limits.h>

#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/utils.h>

#include <opendmi/entity/ipmi-device.h>

static const dmi_name_set_t dmi_ipmi_interface_names =
{
    .code  = "ipmi-interfaces",
    .names = {
        DMI_NAME_UNKNOWN(DMI_IPMI_INTERFACE_UNKNOWN),
        {
            .id   = DMI_IPMI_INTERFACE_KCS,
            .code = "kcs",
            .name = "KCS: Keyboard Controller Style"
        },
        {
            .id   = DMI_IPMI_INTERFACE_SMIC,
            .code = "smic",
            .name = "SMIC: Server Management Interface Chip"
        },
        {
            .id   = DMI_IPMI_INTERFACE_BT,
            .code = "bt",
            .name = "BT: Block Transfer"
        },
        {
            .id   = DMI_IPMI_INTERFACE_SSIF,
            .code = "ssif",
            .name = "SSIF: SMBus System Interface"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_ipmi_addr_type_names =
{
    .code   = "ipmi-addr-types",
    .names = {
        {
            .id   = DMI_IPMI_ADDR_TYPE_MEMORY,
            .code = "memory",
            .name = "Memory"

        },
        {
            .id   = DMI_IPMI_ADDR_TYPE_IO,
            .code = "io",
            .name = "I/O"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_ipmi_intr_trigger_names =
{
    .code   = "ipmi-intr-triggers",
    .names = {
        DMI_NAME_UNSPEC(DMI_IPMI_INTR_TRIGGER_UNSPEC),
        {
            .id   = DMI_IPMI_INTR_TRIGGER_EDGE,
            .code = "edge",
            .name = "Edge"
        },
        {
            .id   = DMI_IPMI_INTR_TRIGGER_LEVEL,
            .code = "level",
            .name = "Level"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_ipmi_intr_polarity_names =
{
    .code   = "ipmi-intr-polarities",
    .names = {
        DMI_NAME_UNSPEC(DMI_IPMI_INTR_POLARITY_UNSPEC),
        {
            .id   = DMI_IPMI_INTR_POLARITY_LOW,
            .code = "active-low",
            .name = "Active low"
        },
        {
            .id   = DMI_IPMI_INTR_POLARITY_HIGH,
            .code = "active-high",
            .name = "Active high"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_ipmi_device_attrs[] =
{
    DMI_ATTRIBUTE(dmi_ipmi_device_t, interface_type, ENUM, {
        .code   = "interface-type",
        .name   = "Interface type",
        .values = &dmi_ipmi_interface_names
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, spec_version, VERSION, {
        .code   = "specification-version",
        .name   = "Specification version",
        .scale  = 2
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, i2c_target_addr, INTEGER, {
        .code   = "i2c-target-address",
        .name   = "I2C target address",
        .flags  = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, nv_storage_addr, INTEGER, {
        .code   = "nv-storage-address",
        .name   = "NV storage address",
        .flags  = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, base_addr, ADDRESS, {
        .code   = "base-address",
        .name   = "Base address"
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, base_addr_lsb, INTEGER, {
        .code   = "base-address-lsb",
        .name   = "Base address LSB"
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, base_addr_type, ENUM, {
        .code   = "base-address-type",
        .name   = "Base address type",
        .values = &dmi_ipmi_addr_type_names
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, intr_trigger, ENUM, {
        .code   = "interrupt-trigger-mode",
        .name   = "Interrupt trigger mode",
        .unspec = dmi_value_ptr(DMI_IPMI_INTR_TRIGGER_UNSPEC),
        .values = &dmi_ipmi_intr_trigger_names
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, intr_polarity, ENUM, {
        .code   = "interrupt-polarity",
        .name   = "Interrupt polarity",
        .unspec = dmi_value_ptr(DMI_IPMI_INTR_POLARITY_UNSPEC),
        .values = &dmi_ipmi_intr_polarity_names
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, intr_number, INTEGER, {
        .code   = "interrupt-number",
        .name   = "Interrupt number",
        .unspec = dmi_value_ptr((unsigned short)0)
    }),
    DMI_ATTRIBUTE(dmi_ipmi_device_t, register_spacing, INTEGER, {
        .code   = "register-spacing",
        .name   = "Register spacing",
        .unit   = "bytes",
        .unspec = dmi_value_ptr((unsigned short)0)
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_ipmi_device_spec =
{
    .code        = "ipmi-device",
    .name        = "IPMI device information",
    .type        = DMI_TYPE_IPMI_DEVICE,
    .min_length  = 0x12,
    .attributes  = dmi_ipmi_device_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_ipmi_device_decode,
        .free   = (dmi_entity_free_fn_t)dmi_ipmi_device_free
    }
};

const char *dmi_ipmi_interface_name(dmi_ipmi_interface_t value)
{
    return dmi_name_lookup(&dmi_ipmi_interface_names, value);
}

const char *dmi_ipmi_addr_type_name(dmi_ipmi_addr_type_t value)
{
    return dmi_name_lookup(&dmi_ipmi_addr_type_names, value);
}

const char *dmi_ipmi_intr_trigger_name(dmi_ipmi_intr_trigger_t value)
{
    return dmi_name_lookup(&dmi_ipmi_intr_trigger_names, value);
}

const char *dmi_ipmi_intr_polarity_name(dmi_ipmi_intr_polarity_t value)
{
    return dmi_name_lookup(&dmi_ipmi_intr_polarity_names, value);
}

dmi_ipmi_device_t *dmi_ipmi_device_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_ipmi_device_t *info;
    const dmi_ipmi_device_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_IPMI_DEVICE));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->interface_type = dmi_value(data->interface_type);
    info->spec_version   = dmi_version((data->spec_version & 0xF0) >> 4, data->spec_version & 0x0F, 0);

    info->i2c_target_addr = dmi_value(data->i2c_target_addr);
    if (data->nv_storage_addr != 0xFFu)
        info->nv_storage_addr = dmi_value(data->nv_storage_addr);
    else
        info->nv_storage_addr = USHRT_MAX;

    uint64_t base_addr = dmi_value(data->base_addr);
    info->base_addr = base_addr & 0x7FFFFFFFFFFFFFFFu;
    if (info->base_addr & 0x8000000000000000u)
        info->base_addr_type = DMI_IPMI_ADDR_TYPE_IO;
    else
        info->base_addr_type = DMI_IPMI_ADDR_TYPE_MEMORY;

    dmi_ipmi_device_details_t details = {
        .__value = dmi_value(data->details)
    };

    if (details.is_intr_info_specified) {
        if (details.is_intr_level_triggered)
            info->intr_trigger = DMI_IPMI_INTR_TRIGGER_LEVEL;
        else
            info->intr_trigger = DMI_IPMI_INTR_TRIGGER_EDGE;

        if (details.is_intr_active_high)
            info->intr_polarity = DMI_IPMI_INTR_POLARITY_HIGH;
        else
            info->intr_polarity = DMI_IPMI_INTR_POLARITY_LOW;
    } else {
        info->intr_trigger  = DMI_IPMI_INTR_TRIGGER_UNSPEC;
        info->intr_polarity = DMI_IPMI_INTR_POLARITY_UNSPEC;
    }

    switch (details.register_spacing) {
    case DMI_IPMI_REGISTER_SPACING_1:
        info->register_spacing = 1;
        break;

    case DMI_IPMI_REGISTER_SPACING_4:
        info->register_spacing = 4;
        break;

    case DMI_IPMI_REGISTER_SPACING_16:
        info->register_spacing = 16;
        break;

    default:
        // fallthrough
    }

    info->base_addr_lsb = details.base_addr_lsb;
    info->intr_number   = dmi_value(data->intr_number);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 0, 0);

    return info;
}

void dmi_ipmi_device_free(dmi_ipmi_device_t *info)
{
    dmi_free(info);
}
