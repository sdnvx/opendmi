//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_UTILS_DECODE_H
#define OPENDMI_UTILS_DECODE_H

#pragma once

#include <opendmi/utils/endian.h>

__BEGIN_DECLS

uintmax_t __dmi_decode_bcd(const dmi_byte_t *value, size_t length);

__END_DECLS

static inline uint8_t dmi_decode_byte(dmi_byte_t value) { return value; }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static inline uint16_t dmi_decode_word(dmi_word_t value)   { return value; }
    static inline uint32_t dmi_decode_dword(dmi_dword_t value) { return value; }
    static inline uint64_t dmi_decode_qword(dmi_qword_t value) { return value; }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN
    static inline uint16_t dmi_decode_word(dmi_word_t value)   { return dmi_bswap16(value); }
    static inline uint32_t dmi_decode_dword(dmi_dword_t value) { return dmi_bswap32(value); }
    static inline uint64_t dmi_decode_qword(dmi_qword_t value) { return dmi_bswap64(value); }
#else
#   error "Unsupported endianness"
#endif

#define dmi_decode(value) (_Generic((value),                       \
                                    dmi_qword_t: dmi_decode_qword, \
                                    dmi_dword_t: dmi_decode_dword, \
                                    dmi_word_t:  dmi_decode_word,  \
                                    dmi_byte_t:  dmi_decode_byte   \
                                   )(value))

#define dmi_decode_bcd(value) \
        __dmi_decode_bcd((const dmi_byte_t *)&(value), sizeof(value))

#endif // !OPEDMI_UTILS_DECODE_H
