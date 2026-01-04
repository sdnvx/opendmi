//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <stdbool.h>

#include <opendmi/utils/decode.h>

struct test_vector
{
    uint8_t   value[4];
    size_t    length;
    uintmax_t result;
};

struct test_vector test_data[] =
{
    { { 0x00, 0x00, 0x00, 0x00 }, 1, 0        },
    { { 0x00, 0x00, 0x00, 0x00 }, 2, 0        },
    { { 0x00, 0x00, 0x00, 0x00 }, 3, 0        },
    { { 0x00, 0x00, 0x00, 0x00 }, 4, 0        },

    { { 0x01, 0x00, 0x00, 0x00 }, 1, 1        },
    { { 0x01, 0x00, 0x00, 0x00 }, 2, 1        },
    { { 0x01, 0x00, 0x00, 0x00 }, 3, 1        },
    { { 0x01, 0x00, 0x00, 0x00 }, 4, 1        },

    { { 0x12, 0x00, 0x00, 0x00 }, 1, 12       },
    { { 0x12, 0x00, 0x00, 0x00 }, 2, 12       },
    { { 0x12, 0x00, 0x00, 0x00 }, 3, 12       },
    { { 0x12, 0x00, 0x00, 0x00 }, 4, 12       },

    { { 0x23, 0x01, 0x00, 0x00 }, 1, 23       },
    { { 0x23, 0x01, 0x00, 0x00 }, 2, 123      },
    { { 0x23, 0x01, 0x00, 0x00 }, 3, 123      },
    { { 0x23, 0x01, 0x00, 0x00 }, 4, 123      },

    { { 0x34, 0x12, 0x00, 0x00 }, 1, 34       },
    { { 0x34, 0x12, 0x00, 0x00 }, 2, 1234     },
    { { 0x34, 0x12, 0x00, 0x00 }, 3, 1234     },
    { { 0x34, 0x12, 0x00, 0x00 }, 4, 1234     },

    { { 0x45, 0x23, 0x01, 0x00 }, 1, 45       },
    { { 0x45, 0x23, 0x01, 0x00 }, 2, 2345     },
    { { 0x45, 0x23, 0x01, 0x00 }, 3, 12345    },
    { { 0x45, 0x23, 0x01, 0x00 }, 4, 12345    },

    { { 0x56, 0x34, 0x12, 0x00 }, 1, 56       },
    { { 0x56, 0x34, 0x12, 0x00 }, 2, 3456     },
    { { 0x56, 0x34, 0x12, 0x00 }, 3, 123456   },
    { { 0x56, 0x34, 0x12, 0x00 }, 4, 123456   },

    { { 0x67, 0x45, 0x23, 0x01 }, 1, 67       },
    { { 0x67, 0x45, 0x23, 0x01 }, 2, 4567     },
    { { 0x67, 0x45, 0x23, 0x01 }, 3, 234567   },
    { { 0x67, 0x45, 0x23, 0x01 }, 4, 1234567  },

    { { 0x78, 0x56, 0x34, 0x12 }, 1, 78       },
    { { 0x78, 0x56, 0x34, 0x12 }, 2, 5678     },
    { { 0x78, 0x56, 0x34, 0x12 }, 3, 345678   },
    { { 0x78, 0x56, 0x34, 0x12 }, 4, 12345678 }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        uint8_t   *value  = test_data[i].value;
        uint8_t    length = test_data[i].length;
        uintmax_t  result = test_data[i].result;

        if (__dmi_decode_bcd(value, length) != result)
            return EXIT_FAILURE;

        switch (length) {
        case sizeof(uint8_t):
            if (dmi_decode_bcd(*(uint8_t *)value) != result)
                return EXIT_FAILURE;
            break;

        case sizeof(uint16_t):
            if (dmi_decode_bcd(*(uint16_t *)value) != result)
                return EXIT_FAILURE;
            break;

        case sizeof(uint32_t):
            if (dmi_decode_bcd(*(uint32_t *)value) != result)
                return EXIT_FAILURE;
            break;

        case sizeof(uint64_t):
            if (dmi_decode_bcd(*(uint64_t *)value) != result)
                return EXIT_FAILURE;
            break;
        }
    }

    return EXIT_SUCCESS;
}
