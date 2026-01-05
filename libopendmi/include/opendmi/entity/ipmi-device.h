//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_ENTITY_IPMI_DEVICE_H
#define OPENDMI_ENTITY_IPMI_DEVICE_H

#pragma once

#include <opendmi/entity.h>

typedef struct dmi_ipmi_device         dmi_ipmi_device_t;
typedef struct dmi_ipmi_device_data    dmi_ipmi_device_data_t;
typedef union  dmi_ipmi_device_details dmi_ipmi_device_details_t;

typedef enum dmi_ipmi_interface
{
    DMI_IPMI_INTERFACE_UNKNOWN = 0x00, ///< Unknown
    DMI_IPMI_INTERFACE_KCS     = 0x01, ///< KCS: Keyboard Controller Style
    DMI_IPMI_INTERFACE_SMIC    = 0x02, ///< SMIC: Server Management Interface Chip
    DMI_IPMI_INTERFACE_BT      = 0x03, ///< BT: Block Transfer
    DMI_IPMI_INTERFACE_SSIF    = 0x04, ///< SSIF: SMBus System Interface
} dmi_ipmi_interface_t;

typedef enum dmi_ipmi_addr_type
{
    DMI_IPMI_ADDR_TYPE_MEMORY = 0x0,
    DMI_IPMI_ADDR_TYPE_IO     = 0x1
} dmi_ipmi_addr_type_t;

typedef enum dmi_ipmi_intr_trigger
{
    DMI_IPMI_INTR_TRIGGER_UNSPEC = 0x0, ///< Unspecified
    DMI_IPMI_INTR_TRIGGER_EDGE   = 0x1, ///< Edge
    DMI_IPMI_INTR_TRIGGER_LEVEL  = 0x2  ///< Level
} dmi_ipmi_intr_trigger_t;

typedef enum dmi_ipmi_intr_polarity
{
    DMI_IPMI_INTR_POLARITY_UNSPEC = 0x0, ///< Unspecified
    DMI_IPMI_INTR_POLARITY_LOW    = 0x1, ///< Active low
    DMI_IPMI_INTR_POLARITY_HIGH   = 0x2  ///< Active high
} dmi_ipmi_intr_polarity_t;

typedef enum dmi_ipmi_register_spacing
{
    DMI_IPMI_REGISTER_SPACING_1        = 0x0, ///< 1 byte
    DMI_IPMI_REGISTER_SPACING_4        = 0x1, ///< 4 bytes
    DMI_IPMI_REGISTER_SPACING_16       = 0x2, ///< 16 bytes
    DMI_IPMI_REGISTER_SPACING_RESERVED = 0x3, ///< Reserved
} dmi_ipmi_register_spacing_t;

dmi_packed_union(dmi_ipmi_device_details)
{
    /**
     * @brief Raw value.
     */
    uint8_t __value;

    dmi_packed_struct()
    {
        bool       is_intr_level_triggered : 1;
        bool       is_intr_active_high     : 1;
        dmi_byte_t __reserved_1            : 1; ///< Reserved for future use, set to 0.
        bool       is_intr_info_specified  : 1;
        bool       base_addr_lsb           : 1;
        dmi_byte_t __reserved_2            : 1; ///< Reserved for future use, set to 0.
        dmi_byte_t register_spacing        : 2;
    };
};

/**
 * @brief IPMI device information structure (type 38).
 *
 * The information in this structure defines the attributes of an Intelligent
 * Platform Management Interface (IPMI) Baseboard Management Controller (BMC).
 * See the Intelligent Platform Management Interface (IPMI) Interface
 * Specification for full documentation of IPMI and additional information on
 * the use of this structure.
 *
 * The type 42 structure can also be used to describe a physical management
 * controller host interface and one or more protocols that share that
 * interface. If IPMI is not shared with other protocols, either the type 38
 * or the type 42 structures can be used. Providing Type 38 is recommended for
 * backward compatibility.
 */
dmi_packed_struct(dmi_ipmi_device_data)
{
    /**
     * @brief SMBIOS structure header.
     */
    dmi_header_t header;

    /**
     * @brief Baseboard Management Controller (BMC) interface type.
     */
    dmi_byte_t interface_type;

    /**
     * @brief IPMI specification revision, in BCD format, to which the BMC was
     * designed. Bits 7:4 hold the most significant digit of the revision,
     * while bits 3:0 hold the least significant bits. Example: A value of 0x10
     * indicates revision 1.0.
     */
    dmi_byte_t spec_version;

    /**
     * @brief Target address on the I2C bus of this BMC.
     */
    dmi_byte_t i2c_target_addr;

    /**
     * @brief Bus ID of the NV storage device If no storage device exists for
     * this BMC, the field is set to 0xFF.
     */
    dmi_byte_t nv_storage_addr;

    /**
     * @brief Base address (either memory-mapped or I/O) of the BMC. If the
     * least-significant bit of the field is a 1, the address is in I/O space.
     * Otherwise, the address is memory-mapped. See the IPMI Interface
     * Specification for usage details.
     */
    dmi_qword_t base_addr;

    /**
     * @brief Base address modifier and interrupt info.
     */
    dmi_byte_t details;

    /**
     * @brief Interrupt number for IPMI System Interface. Zero means that IPMI
     * interrupt is unspecified or unsupported.
     */
    dmi_byte_t intr_number;
};

struct dmi_ipmi_device
{
    /**
     * @brief Baseboard Management Controller (BMC) interface type.
     */
    dmi_ipmi_interface_t interface_type;

    /**
     * @brief IPMI specification revision.
     */
    dmi_version_t spec_version;

    /**
     * @brief Target address on the I2C bus of this BMC.
     */
    dmi_i2c_addr_t i2c_target_addr;

    /**
     * @brief Bus ID of the NV storage device. If no storage device exists for
     * this BMC, the field is set to `UINT8_MAX`.
     */
    uint8_t nv_storage_addr;

    /**
     * @brief Base address (either memory-mapped or I/O) of the BMC. If the
     * least-significant bit of the field is a 1, the address is in I/O space.
     * Otherwise, the address is memory-mapped. See the IPMI Interface
     * Specification for usage details.
     */
    dmi_size_t base_addr;

    /**
     * @brief Base address type.
     */
    dmi_ipmi_addr_type_t base_addr_type;

    /**
     * @brief Base address LSB state.
     */
    bool base_addr_lsb;

    /**
     * @brief Interrupt trigger mode.
     */
    dmi_ipmi_intr_trigger_t intr_trigger;

    /**
     * @brief Interrupt polarity.
     */
    dmi_ipmi_intr_polarity_t intr_polarity;

    /**
     * @brief Register spacing in bytes.
     */
    unsigned short register_spacing;

    /**
     * @brief Interrupt number for IPMI System Interface. Zero means that IPMI
     * interrupt is unspecified or unsupported.
     */
    unsigned short intr_number;
};

/**
 * @brief IPMI device information entity specification.
 */
extern const dmi_entity_spec_t dmi_ipmi_device_spec;

__BEGIN_DECLS

const char *dmi_ipmi_interface_name(dmi_ipmi_interface_t value);
const char *dmi_ipmi_addr_type_name(dmi_ipmi_addr_type_t value);
const char *dmi_ipmi_intr_trigger_name(dmi_ipmi_intr_trigger_t value);
const char *dmi_ipmi_intr_polarity_name(dmi_ipmi_intr_polarity_t value);

dmi_ipmi_device_t *dmi_ipmi_device_decode(const dmi_entity_t *entity, dmi_version_t *plevel);

/**
 * @internal
 */
void dmi_ipmi_device_free(dmi_ipmi_device_t *info);

__END_DECLS

#endif // !OPENDMI_ENTITY_IPMI_DEVICE_H
