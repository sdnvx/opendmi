//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/string-property.h>

static const dmi_attribute_t dmi_string_property_attrs[] =
{
    DMI_ATTRIBUTE(dmi_string_property_t, ident, INTEGER, {
        .code = "ident",
        .name = "Identifier"
    }),
    DMI_ATTRIBUTE(dmi_string_property_t, value, STRING, {
        .code = "value",
        .name = "Identifier"
    }),
    DMI_ATTRIBUTE(dmi_string_property_t, parent_handle, HANDLE, {
        .code = "parent-handle",
        .name = "Parent handle"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_string_property_spec =
{
    .code        = "string-property",
    .name        = "String property",
    .type        = DMI_TYPE_STRING_PROPERTY,
    .min_version = DMI_VERSION(3, 5, 0),
    .min_length  = 0x09,
    .attributes  = dmi_string_property_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_string_property_decode,
        .link   = (dmi_entity_link_fn_t)dmi_string_property_link,
        .free   = (dmi_entity_free_fn_t)dmi_string_property_free,
    }
};

dmi_string_property_t *dmi_string_property_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_string_property_t *info;
    const dmi_string_property_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_STRING_PROPERTY));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->ident         = dmi_decode(data->ident);
    info->value         = dmi_entity_string(entity, data->value);
    info->parent_handle = dmi_decode(data->parent_handle);

    if (plevel != nullptr)
        *plevel = dmi_version(3, 5, 0);

    return info;
}

bool dmi_string_property_link(dmi_entity_t *entity)
{
    dmi_string_property_t *info;
    dmi_registry_t *registry;

    info = dmi_cast(info, dmi_entity_info(entity, DMI_TYPE_STRING_PROPERTY));
    if (info == nullptr)
        return false;

    registry = entity->context->registry;

    if (info->parent_handle != DMI_HANDLE_INVALID) {
        info->parent = dmi_registry_get(registry, info->parent_handle, DMI_TYPE_INVALID, false);
    }

    return true;
}

void dmi_string_property_free(dmi_string_property_t *info)
{
    dmi_free(info);
}
