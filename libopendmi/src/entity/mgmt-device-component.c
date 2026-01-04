//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/mgmt-device-component.h>

const dmi_attribute_t dmi_mgmt_device_component_attrs[] =
{
    DMI_ATTRIBUTE(dmi_mgmt_device_component_t, description, STRING, {
        .code = "description",
        .name = "Description"
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_component_t, device_handle, HANDLE, {
        .code = "device-handle",
        .name = "Device handle"
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_component_t, component_handle, HANDLE, {
        .code = "component-handle",
        .name = "Component handle"
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_component_t, threshold_handle, HANDLE, {
        .code = "threshold-handle",
        .name = "Threshold handle"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_mgmt_device_component_spec =
{
    .code        = "mgmt-device-component",
    .name        = "Management device component",
    .type        = DMI_TYPE_MGMT_DEVICE_COMPONENT,
    .min_version = DMI_VERSION(2, 3, 0),
    .min_length  = 0x0B,
    .attributes  = dmi_mgmt_device_component_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_mgmt_device_component_decode,
        .free   = (dmi_entity_free_fn_t)dmi_mgmt_device_component_free
    }
};

dmi_mgmt_device_component_t *dmi_mgmt_device_component_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_mgmt_device_component_t *info;
    const dmi_mgmt_device_component_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MGMT_DEVICE_COMPONENT));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->description      = dmi_entity_string(entity, data->description);
    info->device_handle    = dmi_decode(data->device_handle);
    info->component_handle = dmi_decode(data->component_handle);
    info->threshold_handle = dmi_decode(data->threshold_handle);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 3, 0);

    return info;
}

bool dmi_mgmt_device_component_link(dmi_entity_t *entity)
{
    static dmi_type_t dmi_component_types[] = {
        DMI_TYPE_COOLING_DEVICE,
        DMI_TYPE_TEMPERATURE_PROBE,
        DMI_TYPE_VOLTAGE_PROBE,
        DMI_TYPE_CURRENT_PROBE,
        DMI_TYPE_INVALID
    };

    dmi_mgmt_device_component_t *info;
    dmi_registry_t *registry;

    info = dmi_cast(info, dmi_entity_info(entity, DMI_TYPE_MGMT_DEVICE_COMPONENT));
    if (info == nullptr)
        return false;

    registry = entity->context->registry;

    info->device = dmi_registry_get(registry, info->device_handle, DMI_TYPE_MGMT_DEVICE, false);
    if (info->device == nullptr)
        return false;

    info->component = dmi_registry_get_any(registry, info->component_handle, dmi_component_types, false);
    if (info->component == nullptr)
        return false;

    if (info->threshold_handle != DMI_HANDLE_INVALID) {
        info->threshold = dmi_registry_get(registry, info->threshold_handle, DMI_TYPE_MGMT_DEVICE_THRESHOLD, false);
    }

    return true;
}

void dmi_mgmt_device_component_free(dmi_mgmt_device_component_t *info)
{
    dmi_free(info);
}
