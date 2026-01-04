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

#include <opendmi/entity/onboard-device-ex.h>

const dmi_attribute_t dmi_onboard_device_ex_attrs[] =
{
    DMI_ATTRIBUTE(dmi_onboard_device_ex_t, designator, STRING, {
        .code    = "designator",
        .name    = "Designator"
    }),
    DMI_ATTRIBUTE(dmi_onboard_device_ex_t, type, ENUM, {
        .code    = "type",
        .name    = "Type",
        .unspec  = dmi_value_ptr(DMI_ONBOARD_DEVICE_TYPE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_ONBOARD_DEVICE_TYPE_UNKNOWN),
        .values  = &dmi_onboard_device_type_names
    }),
    DMI_ATTRIBUTE(dmi_onboard_device_ex_t, is_enabled, BOOL, {
        .code    = "is-enabled",
        .name    = "Enabled"
    }),
    DMI_ATTRIBUTE(dmi_onboard_device_ex_t, instance, INTEGER, {
        .code    = "instance",
        .name    = "Instance"
    }),
    DMI_ATTRIBUTE(dmi_onboard_device_ex_t, address, STRUCT, {
        .code    = "address",
        .name    = "Address",
        .attrs   = dmi_pci_addr_attrs
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_onboard_device_ex_spec =
{
    .code        = "onboard-device-ex",
    .name        = "Onboard devices extended information",
    .type        = DMI_TYPE_ONBOARD_DEVICE_EX,
    .min_version = DMI_VERSION(2, 6, 0),
    .min_length  = 0x0B,
    .attributes  = dmi_onboard_device_ex_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_onboard_device_ex_decode,
        .free   = (dmi_entity_free_fn_t)dmi_onboard_device_ex_free
    }
};

dmi_onboard_device_ex_t *dmi_onboard_device_ex_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_onboard_device_ex_t *info;
    const dmi_onboard_device_ex_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_ONBOARD_DEVICE_EX));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->designator = dmi_entity_string(entity, data->designator);

    dmi_onboard_device_instance_details_t details = {
        .__value = dmi_decode(data->details)
    };

    info->type       = details.type;
    info->is_enabled = details.is_enabled;
    info->instance   = dmi_decode(data->instance);

    dmi_pci_addr_decode(&info->address, &data->address);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 6, 0);

    return info;
}

void dmi_onboard_device_ex_free(dmi_onboard_device_ex_t *info)
{
    dmi_free(info);
}
