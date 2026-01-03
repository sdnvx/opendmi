//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <stdbool.h>

#include <opendmi/utils.h>

struct test_vector
{
    uint32_t value;
    unsigned int factor;
    uint32_t result;
};

struct test_vector test_data[] =
{
    { 1,  0, 1       },
    { 1,  1, 1       },
    { 1,  2, 1       },
    { 1,  3, 1       },
    { 1,  4, 1       },
    { 1,  5, 1       },
    { 1,  6, 1       },
    { 2,  0, 1       },
    { 2,  1, 2       },
    { 2,  2, 4       },
    { 2,  3, 8       },
    { 2,  4, 16      },
    { 2,  5, 32      },
    { 2,  6, 64      },
    { 3,  0, 1       },
    { 3,  1, 3       },
    { 3,  2, 9       },
    { 3,  3, 27      },
    { 3,  4, 81      },
    { 3,  5, 243     },
    { 3,  6, 729     },
    { 5,  0, 1       },
    { 5,  1, 5       },
    { 5,  2, 25      },
    { 5,  3, 125     },
    { 5,  4, 625     },
    { 5,  5, 3125    },
    { 5,  6, 15625   },
    { 10, 0, 1       },
    { 10, 1, 10      },
    { 10, 2, 100     },
    { 10, 3, 1000    },
    { 10, 4, 10000   },
    { 10, 5, 100000  },
    { 10, 6, 1000000 }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        if (dmi_ipow32(test_data[i].value, test_data[i].factor) != test_data[i].result)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
