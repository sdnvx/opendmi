//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/filter.h>

dmi_filter_t *dmi_filter_create(dmi_context_t *context)
{
    dmi_unused(context);

    return nullptr;
}

bool dmi_filter_add_handle(dmi_filter_t *filter, dmi_handle_t handle)
{
    dmi_unused(filter);
    dmi_unused(handle);

    return false;
}

bool dmi_filter_add_type(dmi_filter_t *filter, dmi_type_t type)
{
    dmi_unused(filter);
    dmi_unused(type);

    return false;
}

bool dmi_filter_match(dmi_filter_t *filter, dmi_entity_t *entity)
{
    dmi_unused(filter);
    dmi_unused(entity);

    return false;
}

void dmi_filter_destroy(dmi_filter_t *filter)
{
    dmi_unused(filter);
}
