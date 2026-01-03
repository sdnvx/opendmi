//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_ENTITY_BATTERY_H
#define OPENDMI_ENTITY_BATTERY_H

#pragma once

#include <opendmi/entity.h>
#include <opendmi/utils/datetime.h>

typedef struct dmi_battery_data dmi_battery_data_t;
typedef struct dmi_battery      dmi_battery_t;

/**
 * @brief Battery chemistry types.
 */
typedef enum dmi_battery_chemistry
{
    DMI_BATTERY_CHEMISTRY_UNSPEC    = 0x00, ///< Unspecified
    DMI_BATTERY_CHEMISTRY_OTHER     = 0x01, ///< Other
    DMI_BATTERY_CHEMISTRY_UNKNOWN   = 0x02, ///< Unknown
    DMI_BATTERY_CHEMISTRY_LEAD_ACID = 0x03, ///< Lead acid
    DMI_BATTERY_CHEMISTRY_NI_CD     = 0x04, ///< Nickel cadmium
    DMI_BATTERY_CHEMISTRY_NI_MH     = 0x05, ///< Nickel metal hyrdide
    DMI_BATTERY_CHEMISTRY_LI_ION    = 0x06, ///< Lithium-ion
    DMI_BATTERY_CHEMISTRY_ZN_AIR    = 0x07, ///< Zinc air
    DMI_BATTERY_CHEMISTRY_LI_PO     = 0x08, ///< Lithium polymer
    __DMI_BATTERY_CHEMISTRY_COUNT
} dmi_battery_chemistry_t;

/**
 * @brief Portable battery structure.
 *
 * This structure describes the attributes of the portable battery or batteries
 * for the system. The structure contains the static attributes for the group.
 * Each structure describes attributes for a single battery pack.
 *
 * @since SMBIOS 2.1
 */
dmi_packed_struct(dmi_battery_data)
{
    /**
     * @brief SMBIOS structure header.
     */
    dmi_header_t header;

    /**
     * @brief Number of the string that identifies the location of the battery.
     * Example: "in the back, on the left-hand side".
     *
     * @since SMBIOS 2.1
     */
    dmi_string_t location;

    /**
     * @brief Number of the string that names the company that manufactured the
     * battery.
     *
     * @since SMBIOS 2.1
     */
    dmi_string_t vendor;

    /**
     * @brief Number of the string that identifies the date on which the
     * battery was manufactured.
     *
     * @note
     * SMBIOS 2.2+ implementations that use a smart battery set this field to
     * `0` (no string) to indicate that the SBDS manufacture date field
     * contains the information.
     *
     * @since SMBIOS 2.1
     */
    dmi_string_t manufacture_date;

    /**
     * @brief Number of the string that contains the serial number for the
     * battery.
     *
     * @note
     * SMBIOS 2.2+ implementations that use a smart battery set this field to
     * `0` (no string) to indicate that the SBDS serial number field contains
     * the information.
     *
     * @since SMBIOS 2.1
     */
    dmi_string_t serial_number;

    /**
     * @brief Number of the string that names the battery device. Example:
     * "DR-36".
     *
     * @since SMBIOS 2.1
     */
    dmi_string_t name;

    /**
     * @brief The battery chemistry.
     *
     * @note
     * SMBIOS 2.2+ implementations that use a smart battery set this field to
     * `0x02` (Unknown) to indicate that the SBDS Device Chemistry field
     * contains the information.
     *
     * @since SMBIOS 2.1
     */
    dmi_byte_t chemistry;

    /**
     * @brief Design capacity of the battery in mWatt-hours. If the value is
     * unknown, the field contains `0`.
     *
     * @note
     * For SMBIOS 2.2+ implementations, this value is multiplied by the design
     * capacity multiplier to produce the actual value.
     *
     * @since SMBIOS 2.1
     */
    dmi_word_t capacity;

    /**
     * @brief Design voltage of the battery in mVolts. If the value is unknown,
     * the field contains `0`.
     *
     * @since SMBIOS 2.1
     */
    dmi_word_t voltage;

    /**
     * @brief Number of the string that contains the Smart Battery Data
     * Specification version number supported by this battery. If the battery
     * does not support the function, no string is supplied.
     *
     * @since SMBIOS 2.1
     */
    dmi_string_t sbds_version;

    /**
     * @brief Maximum error (as a percentage in the range 0 to 100) in the
     * Watt-hour data reported by the battery, indicating an upper bound on how
     * much additional energy the battery might have above the energy it
     * reports having. If the value is unknown, the field contains `0xFF`.
     *
     * @since SMBIOS 2.1
     */
    dmi_byte_t maximum_error;

    /**
     * @brief 16-bit value that identifies the battery’s serial number.
     *
     * This value, when combined with the manufacturer, device name, and
     * manufacture date, uniquely identifies the battery. The serial number
     * field must be set to `0` (no string) for this field to be valid.
     *
     * @since SMBIOS 2.2
     */
    dmi_word_t sbds_serial_number;

    /**
     * @brief Date the cell pack was manufactured, in packed format.
     *
     * @since SMBIOS 2.2
     */
    dmi_word_t sbds_manufacture_date;

    /**
     * @brief Number of the string that identifies the battery chemistry (for
     * example, "PbAc"). The device chemistry field must be set to `0x02`
     * (Unknown) for this field to be valid.
     *
     * @since SMBIOS 2.2
     */
    dmi_string_t sbds_chemistry;

    /**
     * @brief Multiplication factor of the Design Capacity value, which
     * assures that the mWatt hours value does not overflow for SBDS
     * implementations.
     *
     * The multiplier default is `1`, SBDS implementations use the value
     * `10` to correspond to the data as returned from the SBDS Function
     * `0x18`.
     *
     * @since SMBIOS 2.2
     */
    dmi_byte_t capacity_factor;

    /**
     * @brief Contains OEM- or firmware vendor-specific information.
     *
     * @since SMBIOS 2.2
     */
    dmi_dword_t oem_defined;
};

struct dmi_battery
{
    /**
     * @brief String that identifies the location of the battery. Example: "in
     * the back, on the left-hand side".
     */
    const char *location;

    /**
     * @brief String that names the company that manufactured the battery.
     */
    const char *vendor;

    /**
     * @brief The date on which the battery was manufactured.
     */
    dmi_date_t manufacture_date;

    /**
     * @brief Number of the string that contains the serial number for the
     * battery.
     */
    const char *serial_number;

    /**
     * @brief String that names the battery device. Example: "DR-36".
     */
    const char *name;

    /**
     * @brief The battery chemistry.
     */
    dmi_battery_chemistry_t chemistry;

    /**
     * @brief Design capacity of the battery in mWatt-hours. If the value is
     * unknown, the field contains `0`.
     */
    unsigned int capacity;

    /**
     * @brief Design voltage of the battery in mVolts. If the value is unknown,
     * the field contains `0`.
     */
    unsigned short voltage;

    /**
     * @brief String that contains the Smart Battery Data Specification version
     * number supported by this battery. If the battery does not support the
     * function, no string is supplied.
     */
    const char *sbds_version;

    /**
     * @brief Maximum error (as a percentage in the range 0 to 100) in the
     * Watt-hour data reported by the battery, indicating an upper bound on how
     * much additional energy the battery might have above the energy it
     * reports having. If the value is unknown, the field contains `USHRT_MAX`.
     */
    unsigned short maximum_error;

    /**
     * @brief 16-bit value that identifies the battery’s serial number.
     *
     * This value, when combined with the manufacturer, device name, and
     * manufacture date, uniquely identifies the battery. The serial number
     * field must be set to `0` (no string) for this field to be valid.
     */
    uint16_t sbds_serial_number;

    /**
     * @brief String that identifies the battery chemistry (for example,
     * "PbAc"). The device chemistry field must be set to `0x02` (Unknown) for
     * this field to be valid.
     */
    const char *sbds_chemistry;

    /**
     * @brief Contains OEM- or firmware vendor-specific information.
     */
    uint32_t oem_defined;
};

/**
 * @brief Portable battery entity specification.
 */
extern const dmi_entity_spec_t dmi_battery_spec;

__BEGIN_DECLS

const char *dmi_battery_chemistry_name(dmi_battery_chemistry_t value);

/**
 * @internal
 */
dmi_battery_t *dmi_battery_decode(const dmi_entity_t *entity, dmi_version_t *plevel);

/**
 * @internal
 */
void dmi_battery_free(dmi_battery_t *info);

__END_DECLS

#endif // !OPENDMI_ENTITY_BATTERY_H
