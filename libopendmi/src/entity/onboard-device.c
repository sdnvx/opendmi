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

#include <opendmi/entity/onboard-device.h>

const dmi_name_set_t dmi_onboard_device_type_names =
{
    .code  = "onboard-device-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_ONBOARD_DEVICE_TYPE_UNSPEC),
        DMI_NAME_OTHER(DMI_ONBOARD_DEVICE_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_ONBOARD_DEVICE_TYPE_UNKNOWN),
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_VIDEO,
            .code = "video",
            .name = "Video"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_SCSI_CONTROLLER,
            .code = "scsi-controller",
            .name = "SCSI controller"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_ETHERNET,
            .code = "ethernet",
            .name = "Ethernet"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_TOKEN_RING,
            .code = "token-ring",
            .name = "Token ring"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_SOUND,
            .code = "sound",
            .name = "Sound"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_PATA_CONTROLLER,
            .code = "pata-controller",
            .name = "PATA controller"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_SATA_CONTROLLER,
            .code = "sata-controller",
            .name = "SATA controller"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_SAS_CONTROLLER,
            .code = "sas-controller",
            .name = "SAS controller"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_WIRELESS_LAN,
            .code = "wireless lan",
            .name = "Wireless LAN"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_BLUETOOTH,
            .code = "bluetooth",
            .name = "Bluetooth"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_WIRELESS_WAN,
            .code = "wireless-wan",
            .name = "Wireless WAN"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_EMM_CONTROLLER,
            .code = "emm-controller",
            .name = "eMM controller"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_NVME_CONTROLLER,
            .code = "nvme-controller",
            .name = "NVMe controller"
        },
        {
            .id   = DMI_ONBOARD_DEVICE_TYPE_UFS_CONTROLLER,
            .code = "ufs-controller",
            .name = "UFS controller"
        },
        DMI_NAME_NULL
    }
};

static const dmi_attribute_t dmi_onboard_device_instance_attrs[] =
{
    DMI_ATTRIBUTE(dmi_onboard_device_instance_t, type, ENUM, {
        .code    = "type",
        .name    = "Type",
        .unspec  = dmi_value_ptr(DMI_ONBOARD_DEVICE_TYPE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_ONBOARD_DEVICE_TYPE_UNKNOWN),
        .values  = &dmi_onboard_device_type_names
    }),
    DMI_ATTRIBUTE(dmi_onboard_device_instance_t, is_enabled, BOOL, {
        .code    = "is-enabled",
        .name    = "Enabled"
    }),
    DMI_ATTRIBUTE(dmi_onboard_device_instance_t, description, STRING, {
        .code    = "description",
        .name    = "Description"
    }),
    DMI_ATTRIBUTE_NULL
};

static const dmi_attribute_t dmi_onboard_device_attrs[] =
{
    DMI_ATTRIBUTE(dmi_onboard_device_t, instance_count, INTEGER, {
        .code  = "instance-count",
        .name  = "Instance count"
    }),
    DMI_ATTRIBUTE_ARRAY(dmi_onboard_device_t, instances, instance_count, STRUCT, {
        .code  = "instances",
        .name  = "Instances",
        .attrs = dmi_onboard_device_instance_attrs
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_onboard_device_spec =
{
    .code       = "onboard-device",
    .name       = "Onboard devices information",
    .type       = DMI_TYPE_ONBOARD_DEVICE,
    .min_length = 0x06,
    .attributes = dmi_onboard_device_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_onboard_device_decode,
        .free   = (dmi_entity_free_fn_t)dmi_onboard_device_free
    }
};

const char *dmi_onboard_device_type_name(dmi_onboard_device_type_t value)
{
    return dmi_name_lookup(&dmi_onboard_device_type_names, value);
}

dmi_onboard_device_t *dmi_onboard_device_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_onboard_device_t *info;
    const dmi_onboard_device_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_ONBOARD_DEVICE));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->instance_count = (entity->body_length - sizeof(dmi_header_t)) /
                           sizeof(dmi_onboard_device_instance_data_t);

    info->instances = dmi_alloc_array(entity->context, sizeof(dmi_onboard_device_instance_t), info->instance_count);
    if (info->instances == nullptr) {
        dmi_free(info);
        return nullptr;
    }

    for (size_t i = 0; i < info->instance_count; i++) {
        dmi_onboard_device_instance_t *instance = &info->instances[i];
        const dmi_onboard_device_instance_data_t *instance_data = &data->instances[i];

        dmi_onboard_device_instance_details_t details = {
            .__value = dmi_decode(instance_data->details)
        };

        instance->type        = details.type;
        instance->is_enabled  = details.is_enabled;
        instance->description = dmi_entity_string(entity, instance_data->description);
    }

    if (plevel != nullptr)
        *plevel = dmi_version(2, 0, 0);

    return info;
}

void dmi_onboard_device_free(dmi_onboard_device_t *info)
{
    dmi_free(info->instances);
    dmi_free(info);
}
