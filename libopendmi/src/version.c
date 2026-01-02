//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdio.h>

#include <opendmi/version.h>
#include <opendmi/utils.h>

char *dmi_version_format(dmi_version_t version)
{
    int   rv  = -1;
    char *str = nullptr;

    unsigned major    = dmi_version_major(version);
    unsigned minor    = dmi_version_minor(version);
    unsigned revision = dmi_version_revision(version);

    if (revision != 0) {
        rv = dmi_asprintf(&str, "%d.%d.%d", major, minor, revision);
    } else {
        rv = dmi_asprintf(&str, "%d.%d", major, minor);
    }

    if (rv < 0)
        return nullptr;

    return str;
}
