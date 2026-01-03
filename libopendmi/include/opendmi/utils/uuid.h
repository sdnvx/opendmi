//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_UTILS_UUID_H
#define OPENDMI_UTILS_UUID_H

#pragma once

#include <opendmi/types.h>

/**
 * @brief UUID value.
 */
typedef union dmi_uuid dmi_uuid_t;

/**
 * @brief UUID type.
 */
dmi_packed_union(dmi_uuid)
{
    /**
     * @brief Raw value.
     */
    dmi_byte_t __value[16];

    dmi_packed_struct()
    {
        /**
         * @brief Low field of the timestamp.
         */
        uint32_t time_low;

        /**
         * @brief Middle field of the timestamp.
         */
        uint16_t time_mid;

        /**
         * @brief High field of the timestamp multiplexed with the version number.
         */
        uint16_t time_hi_and_version;

        /**
         * @brief High field of the clock sequence multiplexed with the variant.
         */
        uint8_t clock_seq_hi_and_reserved;

        /**
         * @brief Low field of the clock sequence.
         */
        uint8_t clock_seq_low;

        /**
         * @brief Spatially unique node identifier.
         */
        uint8_t node[6];
    };
};

__BEGIN_DECLS

/**
 * @brief Decode UUID from SMBIOS format into RFC 4122 format.
 */
dmi_uuid_t dmi_uuid_decode(const dmi_byte_t value[16]);

/**
 * @brief Encode UUID from RFC 4122 format into SMBIOS format.
 */
void dmi_uuid_encode(dmi_uuid_t value, uint8_t out[16]);

__END_DECLS

#endif // !OPENDMI_UTILS_UUID_H
