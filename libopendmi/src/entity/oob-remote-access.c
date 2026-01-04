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

#include <opendmi/entity/oob-remote-access.h>

static const dmi_name_set_t dmi_oob_connection_names =
{
    .code  = "oob-connections",
    .names = {
        {
            .id   = 0,
            .code = "is-inbound-enabled",
            .name = "Inbound connections enabled"
        },
        {
            .id   = 1,
            .code = "is-outbound-enabled",
            .name = "Outbound connections enabled"
        },
        DMI_NAME_NULL
    }
};

static const dmi_attribute_t dmi_oob_remote_access_attrs[] =
{
    DMI_ATTRIBUTE(dmi_oob_remote_access_t, vendor, STRING, {
        .code = "vendor",
        .name = "Vendor"
    }),
    DMI_ATTRIBUTE(dmi_oob_remote_access_t, connections, SET, {
        .code   = "connections",
        .name   = "Connections",
        .values = &dmi_oob_connection_names
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_oob_remote_access_spec =
{
    .code        = "oob-remote-access",
    .name        = "Out-of-band remote access",
    .type        = DMI_TYPE_OOB_REMOTE_ACCESS,
    .min_version = DMI_VERSION(2, 2, 0),
    .min_length  = 0x06,
    .attributes  = dmi_oob_remote_access_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_oob_remote_access_decode,
        .free   = (dmi_entity_free_fn_t)dmi_oob_remote_access_free
    }
};

dmi_oob_remote_access_t *dmi_oob_remote_access_decode(
        const dmi_entity_t *entity,
        dmi_version_t     *plevel)
{
    dmi_oob_remote_access_t *info;
    const dmi_oob_remote_access_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_OOB_REMOTE_ACCESS));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    dmi_oob_connections_t connections = {
        .__value = dmi_decode(data->connections)
    };

    info->vendor      = dmi_entity_string(entity, data->vendor);
    info->connections = connections;

    if (plevel != nullptr)
        *plevel = dmi_version(2, 2, 0);

    return info;
}

void dmi_oob_remote_access_free(dmi_oob_remote_access_t *info)
{
    dmi_free(info);
}
