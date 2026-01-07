//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <stdbool.h>

#include <opendmi/utils/datetime.h>

struct test_vector
{
    const char *value;
    dmi_date_t result;
};

struct test_vector test_data[] =
{
    { "",            DMI_DATE_NONE          },
    { "01",          DMI_DATE_NONE          },
    { "01/01",       DMI_DATE_NONE          },
    { "01/01/00",    DMI_DATE(1900, 01, 01) },
    { "12/31/99",    DMI_DATE(1999, 12, 31) },
    { "00/01/00",    DMI_DATE_NONE          },
    { "13/01/00",    DMI_DATE_NONE          },
    { "01/00/00",    DMI_DATE_NONE          },
    { "01/32/00",    DMI_DATE_NONE          },
    { "01/01/00/01", DMI_DATE_NONE          },
    { "01/01/2000",  DMI_DATE(2000, 01, 01) },
    { "01/01/0000",  DMI_DATE(0000, 01, 01) },
    { "01/01/9999",  DMI_DATE(9999, 01, 01) },
    { "01/01/1",     DMI_DATE_NONE          },
    { "01/01/100",   DMI_DATE_NONE          },
    { "01/01/10000", DMI_DATE_NONE          },
    { "+01/01/00",   DMI_DATE_NONE          },
    { "01/+01/00",   DMI_DATE_NONE          },
    { "01/01/+00",   DMI_DATE_NONE          },
    { "-01/01/00",   DMI_DATE_NONE          },
    { "01/-01/00",   DMI_DATE_NONE          },
    { "01/01/-00",   DMI_DATE_NONE          }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        dmi_date_t result;

        result = dmi_date_parse(test_data[i].value);
        if (result != test_data[i].result)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
