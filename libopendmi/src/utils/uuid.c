//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <memory.h>

#include <opendmi/utils/endian.h>
#include <opendmi/utils/uuid.h>

dmi_uuid_t dmi_uuid_decode(const dmi_byte_t value[16])
{
    dmi_uuid_t uuid;

    memcpy(uuid.__value, value, sizeof(uuid.__value));

    uuid.time_low            = dmi_bswap(uuid.time_low);
    uuid.time_mid            = dmi_bswap(uuid.time_mid);
    uuid.time_hi_and_version = dmi_bswap(uuid.time_hi_and_version);

    return uuid;
}

void dmi_uuid_encode(dmi_uuid_t value, uint8_t out[16])
{
    value.time_low            = dmi_bswap(value.time_low);
    value.time_mid            = dmi_bswap(value.time_mid);
    value.time_hi_and_version = dmi_bswap(value.time_hi_and_version);

    memcpy(out, value.__value, sizeof(value.__value));
}
