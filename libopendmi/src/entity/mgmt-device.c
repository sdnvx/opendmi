//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/utils.h>

#include <opendmi/entity/mgmt-device.h>

static const dmi_name_set_t dmi_mgmt_device_type_names =
{
    .code  = "mgnt-device-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_MGMT_DEVICE_TYPE_UNSPEC),
        DMI_NAME_OTHER(DMI_MGMT_DEVICE_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_MGMT_DEVICE_TYPE_UNKNOWN),
        {
            .id   = DMI_MGMT_DEVICE_TYPE_NATIONAL_LM75,
            .code = "national-lm75",
            .name = "National Semiconductor LM75"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_NATIONAL_LM78,
            .code = "national-lm78",
            .name = "National Semiconductor LM78"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_NATIONAL_LM79,
            .code = "national-lm79",
            .name = "National Semiconductor LM79"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_NATIONAL_LM80,
            .code = "national-lm80",
            .name = "National Semiconductor LM80"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_NATIONAL_LM81,
            .code = "national-lm81",
            .name = "National Semiconductor LM81"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_ANALOG_ADM9240,
            .code = "analog-adm9240",
            .name = "Analog Devices ADM9240"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_DALLAS_DS1780,
            .code = "dallas-ds1780",
            .name = "Dallas Semiconductor DS1780"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_MAXIM_1617,
            .code = "maxim-1617",
            .name = "Maxim 1617"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_GENESYS_GL518SM,
            .code = "genesys-gl518sm",
            .name = "Genesys GL518SM"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_WINBOND_W83781D,
            .code = "winbond-w83781d",
            .name = "Winbond W83781D"
        },
        {
            .id   = DMI_MGMT_DEVICE_TYPE_HOLTEK_HT82H791,
            .code = "holtek-ht82h791",
            .name = "Holtek HT82H791"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_mgmt_device_addr_type_names =
{
    .code  = "mgmt-device-addr-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_MGMT_DEVICE_ADDR_TYPE_UNSPEC),
        DMI_NAME_OTHER(DMI_MGMT_DEVICE_ADDR_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_MGMT_DEVICE_ADDR_TYPE_UNKNOWN),
        {
            .id   = DMI_MGMT_DEVICE_ADDR_TYPE_PORT,
            .code = "port",
            .name = "I/O port"
        },
        {
            .id   = DMI_MGMT_DEVICE_ADDR_TYPE_MEMORY,
            .code = "memory",
            .name = "Memory"
        },
        {
            .id   = DMI_MGMT_DEVICE_ADDR_TYPE_SMBUS,
            .code = "smbus",
            .name = "SMBus"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_mgmt_device_attrs[] =
{
    DMI_ATTRIBUTE(dmi_mgmt_device_t, description, STRING, {
        .code    = "description",
        .name    = "Description"
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_t, type, ENUM, {
        .code    = "type",
        .name    = "Type",
        .unspec  = dmi_value_ptr(DMI_MGMT_DEVICE_TYPE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_MGMT_DEVICE_TYPE_UNKNOWN),
        .values  = &dmi_mgmt_device_type_names
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_t, addr, ADDRESS, {
        .code    = "address",
        .name    = "Address",
        .flags   = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_t, addr_type, ENUM, {
        .code    = "address-type",
        .name    = "Address type",
        .unspec  = dmi_value_ptr(DMI_MGMT_DEVICE_ADDR_TYPE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_MGMT_DEVICE_ADDR_TYPE_UNKNOWN),
        .values  = &dmi_mgmt_device_addr_type_names
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_mgmt_device_spec =
{
    .code        = "mgmt-device",
    .name        = "Management device",
    .type        = DMI_TYPE_MGMT_DEVICE,
    .min_version = DMI_VERSION(2, 3, 0),
    .min_length  = 0x0B,
    .attributes  = dmi_mgmt_device_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_mgmt_device_decode,
        .free   = (dmi_entity_free_fn_t)dmi_mgmt_device_free
    }
};

const char *dmi_mgmt_device_type_name(enum dmi_mgmt_device_type value)
{
    return dmi_name_lookup(&dmi_mgmt_device_type_names, value);
}

const char *dmi_mgmt_device_addr_type_name(enum dmi_mgmt_device_addr_type value)
{
    return dmi_name_lookup(&dmi_mgmt_device_addr_type_names, value);
}

dmi_mgmt_device_t *dmi_mgmt_device_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_mgmt_device_t *info;
    const dmi_mgmt_device_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MGMT_DEVICE));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->description = dmi_entity_string(entity, data->description);
    info->type        = dmi_value(data->type);
    info->addr        = dmi_value(data->addr);
    info->addr_type   = dmi_value(data->addr_type);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 3, 0);

    return info;
}

void dmi_mgmt_device_free(dmi_mgmt_device_t *info)
{
    dmi_free(info);
}
