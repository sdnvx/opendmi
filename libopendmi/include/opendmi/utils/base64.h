//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_UTILS_BASE64_H
#define OPENDMI_UTILS_BASE64_H

#pragma once

#include <opendmi/types.h>

__BEGIN_DECLS

char *dmi_base64_encode(
        const dmi_data_t *data,
        size_t            data_length,
        size_t           *poutput_length);

dmi_data_t *dmi_base64_decode(
        const char *data,
        size_t      data_length,
        size_t     *poutput_length);

__END_DECLS

#endif // !OPENDMI_UTILS_BASE64_H
