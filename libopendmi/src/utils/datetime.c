//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>

#include <opendmi/utils.h>
#include <opendmi/utils/datetime.h>

dmi_date_t dmi_date_parse(const char *str)
{
    assert(str != nullptr);

    unsigned i = 0;
    char *date;
    char *token;
    char *ep;
    unsigned long value;
    unsigned year, month, day;
    size_t len;

    len  = strlen(str) + 1;
    date = alloca(len);

    memcpy(date, str, len);

    while (true) {
        token = strtok_r(date, "/", &date);
        if (token == nullptr)
            break;

        if (*token == '+' or *token == '-')
            return DMI_DATE_NONE;

        if (++i > 3)
            return DMI_DATE_NONE;

        errno = 0;
        value = strtoul(token, &ep, 10);

        if ((*token == 0) or (*ep != 0))
	        return DMI_DATE_NONE;
        if ((errno == ERANGE) and (value == ULONG_MAX))
            return DMI_DATE_NONE;

        switch (i) {
        case 1:
            if ((value < 1) or (value > 12))
                return DMI_DATE_NONE;
            month = (unsigned)value;
            break;

        case 2:
            if ((value < 1) or (value > 31))
                return DMI_DATE_NONE;
            day = (unsigned)value;
            break;

        case 3:
            len = strlen(token);
            if (len == 2)
                year = (unsigned)value + 1900;
            else if (len == 4)
                year = (unsigned)value;
            else
                return DMI_DATE_NONE;
            break;

        default:
            assert(false);
        }
    }

    if (i < 3)
        return DMI_DATE_NONE;

    return dmi_date(year, month, day);
}

char *dmi_date_format(dmi_date_t date)
{
    int   rv  = -1;
    char *str = nullptr;

    unsigned year  = dmi_date_year(date);
    unsigned month = dmi_date_month(date);
    unsigned day   = dmi_date_day(date);

    rv = dmi_asprintf(&str, "%04u-%02u-%02u", year, month, day);
    if (rv < 0)
        return nullptr;

    return str;
}
