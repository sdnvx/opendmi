//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_TEST_LOGGER_H
#define OPENDMI_TEST_LOGGER_H

#pragma once

#include <opendmi/log.h>

__BEGIN_DECLS

void dmi_test_log_handler(
        dmi_log_t       *target,
        dmi_log_level_t  level,
        const char      *format,
        va_list          args);

__END_DECLS

#endif // !OPENDMI_TEST_LOGGER_H
