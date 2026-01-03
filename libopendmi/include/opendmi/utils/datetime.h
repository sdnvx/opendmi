//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_UTILS_DATETIME_H
#define OPENDMI_UTILS_DATETIME_H

#pragma once

#include <opendmi/types.h>

/**
 * @brief Date value.
 */
typedef uint32_t dmi_date_t;

#define DMI_DATE(year, month, day) \
    ((dmi_date_t)((((year) & 0xFFFFu) << 16) | (((month) & 0xFFu) << 8) | ((day) & 0xFFu)))

#define DMI_DATE_NONE DMI_DATE(0, 0, 0)

static inline dmi_date_t dmi_date(unsigned int year,
                                  unsigned int month,
                                  unsigned int day)
{
    return DMI_DATE(year, month, day);
}

static inline unsigned int dmi_date_year(dmi_date_t date)
{
    return (date & 0xFFFF0000U) >> 16;
}

static inline unsigned int dmi_date_month(dmi_date_t date)
{
    return (date & 0x00FF00U) >> 8;
}

static inline unsigned int dmi_date_day(dmi_date_t date)
{
    return date & 0x0000FFU;
}

__BEGIN_DECLS

dmi_date_t dmi_date_parse(const char *str);

char *dmi_date_format(dmi_date_t date);

__END_DECLS

#endif // !OPENDMI_UTILS_DATETIME_H
