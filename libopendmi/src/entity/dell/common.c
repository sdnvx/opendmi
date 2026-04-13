//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/utils/name.h>
#include <opendmi/entity/dell/common.h>

const dmi_name_set_t dmi_dell_enable_state_names =
{
    .code  = "dell-enable-states",
    .names = (const dmi_name_t[]){
        DMI_NAME_UNSPEC(DMI_DELL_ENABLE_STATE_UNSPEC),
        DMI_NAME_OTHER(DMI_DELL_ENABLE_STATE_OTHER),
        DMI_NAME_UNKNOWN(DMI_DELL_ENABLE_STATE_UNKNOWN),
        {
            .id   = DMI_DELL_ENABLE_STATE_ENABLED,
            .code = "enabled",
            .name = "Enabled"
        },
        {
            .id   = DMI_DELL_ENABLE_STATE_DISABLED,
            .code = "disabled",
            .name = "Disabled"
        },
        DMI_NAME_NULL
    }
};

const dmi_name_set_t dmi_dell_port_security_names =
{
    .code  = "dell-port-security-settings",
    .names = (const dmi_name_t[]){
        DMI_NAME_UNSPEC(DMI_DELL_PORT_SECURITY_UNSPEC),
        DMI_NAME_OTHER(DMI_DELL_PORT_SECURITY_OTHER),
        DMI_NAME_UNKNOWN(DMI_DELL_PORT_SECURITY_UNKNOWN),
        DMI_NAME_NONE(DMI_DELL_PORT_SECURITY_NONE),
        {
            .id   = DMI_DELL_PORT_SECURITY_EXT_IFACE_LOCKED,
            .code = "external-iface-locked",
            .name = "External interface locked out"
        },
        {
            .id   = DMI_DELL_PORT_SECURITY_EXT_IFACE_ENABLED,
            .code = "external-iface-enabled",
            .name = "External interface enabled"
        },
        {
            .id   = DMI_DELL_PORT_SECURITY_BOOT_BYPASS,
            .code = "boot-bypass",
            .name = "Boot-bypass"
        },
        DMI_NAME_NULL
    }
};

const char *dmi_dell_enable_state_name(dmi_dell_enable_state_t value)
{
    return dmi_name_lookup(&dmi_dell_enable_state_names, (int)value);
}

const char *dmi_dell_port_security_name(dmi_dell_port_security_t value)
{
    return dmi_name_lookup(&dmi_dell_port_security_names, (int)value);
}
