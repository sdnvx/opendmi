//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <opendmi/utils.h>

struct test_vector
{
    uint8_t    value[16];
    dmi_uuid_t result;
};

struct test_vector test_data[] =
{
    {
        .value = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        },
        .result = {
            .time_low                  = 0x00000000U,
            .time_mid                  = 0x0000U,
            .time_hi_and_version       = 0x0000U,
            .clock_seq_hi_and_reserved = 0x00U,
            .clock_seq_low             = 0x00U,
            .node                      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        }
    },
    {
        .value = {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        },
        .result = {
            .time_low                  = 0xFFFFFFFFU,
            .time_mid                  = 0xFFFFU,
            .time_hi_and_version       = 0xFFFFU,
            .clock_seq_hi_and_reserved = 0xFFU,
            .clock_seq_low             = 0xFFU,
            .node                      = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
        }
    },
    {
        .value = {
            0x33, 0x22, 0x11, 0x00, 0x55, 0x44, 0x77, 0x66,
            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
        },
        .result = {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            .time_low                  = 0x33221100U,
            .time_mid                  = 0x5544U,
            .time_hi_and_version       = 0x7766U,
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN
            .time_low                  = 0x00112233U,
            .time_mid                  = 0x4455U,
            .time_hi_and_version       = 0x6677U,
#else
#   error "Unsupported endianness"
#endif
            .clock_seq_hi_and_reserved = 0x88U,
            .clock_seq_low             = 0x99U,
            .node                      = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }
        }
    }
};

int main(void)
{
    for (size_t i = 0; i < countof(test_data); i++) {
        dmi_uuid_t uuid = dmi_uuid_decode(test_data[i].value);
        if (memcmp(uuid.__value, test_data[i].result.__value, sizeof(uuid)) != 0)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
