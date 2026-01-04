//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/group-assoc.h>

const dmi_attribute_t dmi_group_assoc_attrs[] =
{
    DMI_ATTRIBUTE(dmi_group_assoc_t, group_name, STRING, {
        .code = "group-name",
        .name = "Group name"
    }),
    DMI_ATTRIBUTE(dmi_group_assoc_t, item_type, INTEGER, {
        .code  = "item-type",
        .name  = "Item type",
        .flags = DMI_ATTRIBUTE_FLAG_HEX
    }),
    DMI_ATTRIBUTE(dmi_group_assoc_t, item_handle, HANDLE, {
        .code = "item-handle",
        .name = "Item handle"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_group_assoc_spec =
{
    .code       = "group-associations",
    .name       = "Group associations",
    .type       = DMI_TYPE_GROUP_ASSOC,
    .min_length = 0x08,
    .attributes = dmi_group_assoc_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_group_assoc_decode,
        .free   = (dmi_entity_free_fn_t)dmi_group_assoc_free
    }
};

dmi_group_assoc_t *dmi_group_assoc_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_group_assoc_t *info;
    const dmi_group_assoc_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_GROUP_ASSOC));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->group_name  = dmi_entity_string(entity, data->group_name);
    info->item_type   = dmi_decode(data->item_type);
    info->item_handle = dmi_decode(data->item_handle);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 0, 0);

    return info;
}

void dmi_group_assoc_free(dmi_group_assoc_t *info)
{
    dmi_free(info);
}
