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

#include <opendmi/entity/pointing-device.h>

static const dmi_name_set_t dmi_pointing_device_type_names =
{
    .code  = "pointing-device-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_POINTING_DEVICE_TYPE_UNSPEC),
        DMI_NAME_OTHER(DMI_POINTING_DEVICE_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_POINTING_DEVICE_TYPE_UNKNOWN),
        {
            .id   = DMI_POINTING_DEVICE_TYPE_MOUSE,
            .code = "mouse",
            .name = "Mouse"
        },
        {
            .id   = DMI_POINTING_DEVICE_TYPE_TRACK_BALL,
            .code = "track-ball",
            .name = "Track ball"
        },
        {
            .id   = DMI_POINTING_DEVICE_TYPE_TRACK_POINT,
            .code = "track-point",
            .name = "Track point"
        },
        {
            .id   = DMI_POINTING_DEVICE_TYPE_GLIDE_POINT,
            .code = "glide-point",
            .name = "Glide point"
        },
        {
            .id   = DMI_POINTING_DEVICE_TYPE_TOUCH_PAD,
            .code = "touch-pad",
            .name = "Touch pad"
        },
        {
            .id   = DMI_POINTING_DEVICE_TYPE_TOUCH_SCREEN,
            .code = "touch-screen",
            .name = "Touch screen"
        },
        {
            .id   = DMI_POINTING_DEVICE_TYPE_OPTICAL_SENSOR,
            .code = "optical-sensor",
            .name = "Optical sensor"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_pointing_device_interface_names =
{
    .code  = "pointing-device-interfaces",
    .names = {
        DMI_NAME_UNSPEC(DMI_POINTING_DEVICE_INTERFACE_UNSPEC),
        DMI_NAME_OTHER(DMI_POINTING_DEVICE_INTERFACE_OTHER),
        DMI_NAME_UNKNOWN(DMI_POINTING_DEVICE_INTERFACE_UNKNOWN),
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_SERIAL,
            .code = "serial",
            .name = "Serial"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_PS2,
            .code = "ps2",
            .name = "PS/2"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_INFRARED,
            .code = "infrared",
            .name = "Infrared"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_HP_HIL,
            .code = "hp-hil",
            .name = "HP-HIL"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_BUS_MOUSE,
            .code = "bus-mouse",
            .name = "Bus mouse"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_ADB,
            .code = "adb",
            .name = "ADB (Apple Desktop Bus)"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_BUS_MOUSE_DB9,
            .code = "bus-mouse-db9",
            .name = "Bus mouse DB-9"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_BUS_MOUSE_DIN,
            .code = "bus-mouse-din",
            .name = "Bus mouse micro-DIN"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_USB,
            .code = "usb",
            .name = "USB"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_I2C,
            .code = "i2c",
            .name = "I2C"
        },
        {
            .id   = DMI_POINTING_DEVICE_INTERFACE_SPI,
            .code = "spi",
            .name = "SPI"
        },
        DMI_NAME_NULL
    }
};

const dmi_attribute_t dmi_pointing_device_attrs[] =
{
    DMI_ATTRIBUTE(dmi_pointing_device_t, type, ENUM, {
        .code    = "type",
        .name    = "Type",
        .unspec  = dmi_value_ptr(DMI_POINTING_DEVICE_TYPE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_POINTING_DEVICE_TYPE_UNKNOWN),
        .values  = &dmi_pointing_device_type_names
    }),
    DMI_ATTRIBUTE(dmi_pointing_device_t, interface, ENUM, {
        .code    = "interface",
        .name    = "Interface type",
        .unspec  = dmi_value_ptr(DMI_POINTING_DEVICE_INTERFACE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_POINTING_DEVICE_INTERFACE_UNKNOWN),
        .values  = &dmi_pointing_device_interface_names
    }),
    DMI_ATTRIBUTE(dmi_pointing_device_t, button_count, INTEGER, {
        .code    = "button-count",
        .name    = "Button count"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_pointing_device_spec =
{
    .code        = "pointing-device",
    .name        = "Built-in pointing device",
    .type        = DMI_TYPE_POINTING_DEVICE,
    .min_version = DMI_VERSION(2, 1, 0),
    .min_length  = 0x07,
    .attributes  = dmi_pointing_device_attrs,
    .handlers = {
        .decode = (dmi_entity_decode_fn_t)dmi_pointing_device_decode,
        .free   = (dmi_entity_free_fn_t)dmi_pointing_device_free
    }
};

const char *dmi_pointing_device_type_name(dmi_pointing_device_type_t value)
{
    return dmi_name_lookup(&dmi_pointing_device_type_names, value);
}

const char *dmi_pointing_device_interface_name(dmi_pointing_device_interface_t value)
{
    return dmi_name_lookup(&dmi_pointing_device_interface_names, value);
}

dmi_pointing_device_t *dmi_pointing_device_decode(dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_pointing_device_t *info;
    const dmi_pointing_device_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_POINTING_DEVICE));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->type         = dmi_decode(data->type);
    info->interface    = dmi_decode(data->interface);
    info->button_count = dmi_decode(data->button_count);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 1, 0);

    return info;
}

void dmi_pointing_device_free(dmi_pointing_device_t *info)
{
    dmi_free(info);
}
