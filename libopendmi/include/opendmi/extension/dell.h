//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_ENTITY_DELL_H
#define OPENDMI_ENTITY_DELL_H

#pragma once

#include <opendmi/entity.h>
#include <opendmi/extension.h>

typedef struct dmi_dell_calling_interface_data dmi_dell_calling_interface_data_t;
typedef struct dmi_dell_calling_interface_token dmi_dell_calling_interface_token_t;

typedef enum dmi_dell_type
{
    DMI_TYPE_DELL_INDEXED_IO        = 212, ///< Dell: Indexed IO
	DMI_TYPE_DELL_PROTECTED_AREA_1  = 214, ///< Dell: Protected Area Type 1
	DMI_TYPE_DELL_PROTECTED_AREA_2  = 215, ///< Dell: Protected Area Type 2
	DMI_TYPE_DELL_CALLING_INTERFACE = 218  ///< Dell: Calling interface
} dmi_dell_type_t;

/**
 * @brief Dell SMI calling interface token.
 */
dmi_packed_struct(dmi_dell_calling_interface_token)
{
    /**
     * @brief Token identifier.
     */
    uint16_t id;

    /**
     * @brief Location.
     */
    uint16_t location;

    /**
     * @brief Token value or string length.
     */
    uint16_t length;
};

/**
 * @brief Dell SMI calling interface structure (type 218).
 */
dmi_packed_struct(dmi_dell_calling_interface_data)
{
    /**
     * @brief SMBIOS structure header.
     */
    dmi_header_t header;

    /**
     * @brief Command address.
     */
    uint16_t command_addr;

    /**
     * @brief Command code.
     */
	uint8_t command_code;

    /**
     * @brief Supported commands.
     */
	uint32_t supported_commands;

    /**
     * @brief Tokens.
     */
	dmi_dell_calling_interface_token_t tokens[];
};

/**
 * @brief Dell DMI extension.
 */
extern const dmi_extension_t dmi_dell_ext;

#endif // !OPENDMI_ENTITY_DELL_H
