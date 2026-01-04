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

#include <opendmi/entity/mgmt-device-threshold.h>

const dmi_attribute_t dmi_mgmt_device_threshold_attrs[] =
{
    DMI_ATTRIBUTE(dmi_mgmt_device_threshold_t, lower_non_critical, INTEGER, {
        .code    = "lower-non-critical",
        .name    = "Lower non-critical",
        .unknown = dmi_value_ptr((short)SHRT_MIN)
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_threshold_t, upper_non_critical, INTEGER, {
        .code    = "upper-non-critical",
        .name    = "Upper non-critical",
        .unknown = dmi_value_ptr((short)SHRT_MIN)
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_threshold_t, lower_critical, INTEGER, {
        .code    = "lower-critical",
        .name    = "Lower critical",
        .unknown = dmi_value_ptr((short)SHRT_MIN)
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_threshold_t, upper_critical, INTEGER, {
        .code    = "upper-critical",
        .name    = "Upper critical",
        .unknown = dmi_value_ptr((short)SHRT_MIN)
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_threshold_t, lower_non_recoverable, INTEGER, {
        .code    = "lower-non-recoverable",
        .name    = "Lower non-recoverable",
        .unknown = dmi_value_ptr((short)SHRT_MIN)
    }),
    DMI_ATTRIBUTE(dmi_mgmt_device_threshold_t, upper_non_recoverable, INTEGER, {
        .code    = "upper-non-recoverable",
        .name    = "Upper non-recoverable",
        .unknown = dmi_value_ptr((short)SHRT_MIN)
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_mgmt_device_threshold_spec =
{
    .code        = "mgmt-device-threshold",
    .name        = "Management device threshold data",
    .type        = DMI_TYPE_MGMT_DEVICE_THRESHOLD,
    .min_version = DMI_VERSION(2, 3, 0),
    .min_length  = 0x10,
    .attributes  = dmi_mgmt_device_threshold_attrs,
    .handlers    = {
        .decode = (dmi_entity_decode_fn_t)dmi_mgmt_device_threshold_decode,
        .free   = (dmi_entity_free_fn_t)dmi_mgmt_device_threshold_free
    }
};

dmi_mgmt_device_threshold_t *dmi_mgmt_device_threshold_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_mgmt_device_threshold_t *info;
    dmi_mgmt_device_threshold_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_MGMT_DEVICE_THRESHOLD));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->lower_non_critical    = dmi_decode(data->lower_non_critical);
    info->upper_non_critical    = dmi_decode(data->upper_non_critical);
    info->lower_critical        = dmi_decode(data->lower_critical);
    info->upper_critical        = dmi_decode(data->upper_critical);
    info->lower_non_recoverable = dmi_decode(data->lower_non_recoverable);
    info->upper_non_recoverable = dmi_decode(data->upper_non_recoverable);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 3, 0);

    return info;
}

void dmi_mgmt_device_threshold_free(dmi_mgmt_device_threshold_t *info)
{
    dmi_free(info);
}
