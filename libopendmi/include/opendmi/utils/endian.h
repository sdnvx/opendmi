//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_UTILS_ENDIAN_H
#define OPENDMI_UTILS_ENDIAN_H

#pragma once

#include <opendmi/types.h>

static inline uint8_t dmi_bswap8(uint8_t value) { return value; };

__BEGIN_DECLS

/**
 * @internal
 * @brief Return a value in which the order of the bytes in their argument is
 * reversed. Used as fallback for dmi_bswap16() inline when no compiler builtin
 * is available.
 */
[[gnu::const]]
uint16_t __dmi_bswap16_compat(uint16_t value);

/**
 * @internal
 * @brief Return a value in which the order of the bytes in their argument is
 * reversed. Used as fallback for dmi_bswap32() inline when no compiler builtin
 * is available.
 */
[[gnu::const]]
uint32_t __dmi_bswap32_compat(uint32_t value);

/**
 * @internal
 * @brief Return a value in which the order of the bytes in their argument is
 * reversed. Used as fallback for dmi_bswap64() inline when no compiler builtin
 * is available.
 */
[[gnu::const]]
uint64_t __dmi_bswap64_compat(uint64_t value);

__END_DECLS

[[gnu::const]]
static inline uint16_t dmi_bswap16(uint16_t value)
{
#   if defined(_MSC_VER)
        return _byteswap_ushort(value);
#   elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap16(value);
#   else
        return __dmi_bswap16_compat(value);
#   endif
}

[[gnu::const]]
static inline uint32_t dmi_bswap32(uint32_t value)
{
#   if defined(_MSC_VER)
        return _byteswap_ulong(value);
#   elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap32(value);
#   else
        return __dmi_bswap32_compat(value);
#   endif
}

[[gnu::const]]
static inline uint64_t dmi_bswap64(uint64_t value)
{
#   if defined(_MSC_VER)
        return _byteswap_uint64(value);
#   elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap64(value);
#   else
        return __dmi_bswap64_compat(value);
#   endif
}

#define dmi_bswap(value) (_Generic((value),               \
                                   uint64_t: dmi_bswap64, \
                                   uint32_t: dmi_bswap32, \
                                   uint16_t: dmi_bswap16, \
                                   uint8_t:  dmi_bswap8   \
                                  )(value))

/**
 * @fn uint16_t dmi_hton16(uint16_t value)
 * @brief Convert unsigned 16-bit value from host to network byte order.
 */
[[gnu::const]]
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static inline uint16_t dmi_hton16(uint16_t value) { return dmi_bswap16(value); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN
    static inline uint16_t dmi_hton16(uint16_t value) { return value; }
#endif

/**
 * @fn uint32_t dmi_hton32(uint32_t value)
 * @brief Convert unsigned 32-bit value from host to network byte order.
 */
[[gnu::const]]
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static inline uint32_t dmi_hton32(uint32_t value) { return dmi_bswap32(value); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN
    static inline uint32_t dmi_hton32(uint32_t value) { return value; }
#endif

/**
 * @fn uint64_t dmi_hton64(uint64_t value)
 * @brief Convert unsigned 64-bit value from host to network byte order.
 */
[[gnu::const]]
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static inline uint64_t dmi_hton64(uint64_t value) { return dmi_bswap64(value); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN
    static inline uint64_t dmi_hton64(uint64_t value) { return value; }
#endif

/**
 * @fn uint16_t dmi_ntoh16(uint16_t value)
 * @brief Convert unsigned 16-bit value from network to host byte order.
 */
[[gnu::const]]
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static inline uint16_t dmi_ntoh16(uint16_t value) { return dmi_bswap16(value); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN
    static inline uint16_t dmi_ntoh16(uint16_t value) { return value; }
#endif

/**
 * @fn uint32_t dmi_ntoh32(uint32_t value)
 * @brief Convert unsigned 32-bit value from network to host byte order.
 */
[[gnu::const]]
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static inline uint32_t dmi_ntoh32(uint32_t value) { return dmi_bswap32(value); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN
    static inline uint32_t dmi_ntoh32(uint32_t value) { return value; }
#endif

/**
 * @fn uint64_t dmi_ntoh64(uint64_t value)
 * @brief Convert unsigned 64-bit value from network to host byte order.
 */
[[gnu::const]]
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static inline uint64_t dmi_ntoh64(uint64_t value) { return dmi_bswap64(value); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN
    static inline uint64_t dmi_ntoh64(uint64_t value) { return value; }
#endif

#define dmi_hton(value) (_Generic((value),              \
                                  uint64_t: dmi_hton64, \
                                  uint32_t: dmi_hton32, \
                                  uint16_t: dmi_hton16  \
                                 )(value))

#define dmi_ntoh(value) (_Generic((value),              \
                                  uint64_t: dmi_ntoh64, \
                                  uint32_t: dmi_ntoh32, \
                                  uint16_t: dmi_ntoh16  \
                                 )(value))

#endif // !OPENDMI_UTILS_ENDIAN_H
