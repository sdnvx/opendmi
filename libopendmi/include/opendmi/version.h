//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_VERSION_H
#define OPENDMI_VERSION_H

#pragma once

#include <opendmi/types.h>

/**
 * @brief SMBIOS version number.
 */
typedef uint32_t dmi_version_t;

#define DMI_VERSION(major, minor, revision) \
    ((dmi_version_t)((((major) & 0xFFU) << 16) | (((minor) & 0xFFU) << 8) | ((revision) & 0xFFU)))

#define DMI_VERSION_NONE DMI_VERSION(0, 0, 0)

/**
 * @brief Construct SMBIOS version number from its components.
 *
 * @param[in] major Major version number.
 * @param[in] minor Minor version number.
 * @param[in] revision Revision version number.
 *
 * @return Version number.
 */
static inline dmi_version_t dmi_version(unsigned int major,
                                        unsigned int minor,
                                        unsigned int revision)
{
    return DMI_VERSION(major, minor, revision);
}

/**
 * @brief Get major component of SMBIOS version number.
 *
 * @param[in] version Version number.
 * @return Major version number.
 */
static inline unsigned int dmi_version_major(dmi_version_t version)
{
    return (version & 0xFF0000U) >> 16;
}

/**
 * @brief Get minor component of SMBIOS version number.
 *
 * @param[in] version Version number.
 * @return Minor version number.
 */
static inline unsigned int dmi_version_minor(dmi_version_t version)
{
    return (version & 0x00FF00U) >> 8;
}

/**
 * @brief Get revision component of SMBIOS version number.
 *
 * @param[in] version Version number.
 * @return Revision version number.
 */
static inline unsigned int dmi_version_revision(dmi_version_t version)
{
    return version & 0x0000FFU;
}

__BEGIN_DECLS

char *dmi_version_format(dmi_version_t version);

__END_DECLS

#endif // !OPENDMI_VERSION_H
