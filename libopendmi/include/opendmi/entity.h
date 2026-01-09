//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_ENTITY_H
#define OPENDMI_ENTITY_H

#pragma once

#include <opendmi/types.h>
#include <opendmi/attribute.h>
#include <opendmi/utils/version.h>

typedef struct dmi_entity       dmi_entity_t;
typedef struct dmi_entity_spec  dmi_entity_spec_t;
typedef struct dmi_entity_ops   dmi_entity_ops_t;
typedef struct dmi_header       dmi_header_t;
typedef struct dmi_string_entry dmi_string_entry_t;

typedef bool (*dmi_entity_validate_fn_t)(dmi_entity_t *entity);
typedef void *(*dmi_entity_decode_fn_t)(dmi_entity_t *entity, dmi_version_t *plevel);
typedef bool (*dmi_entity_link_fn_t)(dmi_entity_t *entity);
typedef void (*dmi_entity_free_fn_t)(void *info);

/**
 * @brief Entity operations.
 */
struct dmi_entity_ops
{
    /**
     * @brief Validation handler (optional).
     */
    dmi_entity_validate_fn_t validate;

    /**
     * @brief Decoding handler.
     */
    dmi_entity_decode_fn_t decode;

    /**
     * @brief Link handler.
     */
    dmi_entity_link_fn_t link;

    /**
     * @brief Destroy handler.
     */
    dmi_entity_free_fn_t free;
};

/**
 * @brief Entity specification.
 */
struct dmi_entity_spec
{
    /**
     * @brief Code name, used to identify the entity during serialization.
     */
    const char *code;

    /**
     * @brief Printable name, used to identify the entity when printing.
     */
    const char *name;

    /**
     * @brief DMI type.
     */
    dmi_type_t type;

    /**
     * @brief Minimum SMBIOS version. Zero means that the minimum version is
     * not specified.
     */
    dmi_version_t min_version;

    dmi_version_t required_from;

    dmi_version_t required_till;

    bool unique;

    /**
     * @brief Minimum length. Zero means that the minimum length is not
     * specified.
     *
     * Starting with SMBIOS 2.3, each SMBIOS structure type has a minimum
     * length - enabling the addition of new, but optional, fields to SMBIOS
     * structures. In no case shall a structure’s length result in a field
     * being less than fully populated. For example, a voltage probe structure
     * with length of 0x15 is invalid because the nominal value field would
     * not be fully specified.
     *
     * @since SMBIOS 2.3
     */
    size_t min_length;

    /**
     * @brief Attribute specifications.
     */
    const dmi_attribute_t *attributes;

    /**
     * @brief Operation handlers.
     */
    dmi_entity_ops_t handlers;
};

/**
 * @brief SMBIOS structure header.
 */
dmi_packed_struct(dmi_header)
{
    /**
     * @brief Specifies the type of structure. Types 0 through 127 (7Fh) are
     * reserved for and defined by this specification. Types 128 through 256
     * (0x80 to 0xFF) are available for system- and OEM-specific information.
     */
    dmi_byte_t type;

    /**
     * @brief Specifies the length of the formatted area of the structure,
     * starting at the Type field. The length of the structure’s string-set is
     * not included.
     */
    dmi_byte_t length;

    /**
     * @brief Specifies the structure’s handle.
     *
     * If the system configuration changes, a previously assigned handle might
     * no longer exist. However, after a handle has been assigned by the
     * platform firmware, the firmware cannot re-assign that handle number to
     * another structure.
     *
     * Unless otherwise specified, when referring to another structure’s
     * handle, the value 0x0FFFF is used to indicate that the referenced handle
     * is not applicable or does not exist.
     *
     * @see dmi_handle_t
     */
    dmi_handle_t handle;
};

/**
 * @brief String entry.
 */
struct dmi_string_entry
{
    /**
     * @brief Raw value.
     */
    const char *raw;

    /**
     * @brief Pretty value.
     */
    char *pretty;
};

/**
 * @brief Entity descriptor.
 */
struct dmi_entity
{
    /**
     * @brief Context handle.
     */
    dmi_context_t *context;

    /**
     * @brief Entity type.
     */
    dmi_type_t type;

    /**
     * @brief Pointer to specification.
     */
    const dmi_entity_spec_t *spec;

    /**
     * @brief DMI handle.
     */
    dmi_handle_t handle;

    /**
     * @brief Pointer to raw SMBIOS structure data, including header.
     */
    const dmi_data_t *data;

    /**
     * @brief Total length of the SMBIOS structure.
     */
    size_t total_length;

    /**
     * @brief Formatted section length.
     */
    size_t body_length;

    /**
     * @brief Unformed section length. The unformed section of the SMBIOS
     * structure is used for passing variable data such as text strings.
     */
    size_t extra_length;

    /**
     * @brief String data.
     */
    dmi_string_entry_t *strings;

    /**
     * @brief Number of strings in the SMBIOS structure.
     */
    size_t string_count;

    /**
     * @brief Decoded information.
     */
    void *info;

    /**
     * @brief Structure version.
     */
    dmi_version_t level;
};

__BEGIN_DECLS

/**
 * @brief Decode SMBIOS structure into entity.
 *
 * @param[in] context DMI context handle.
 * @param[in] data Pointer to SMBIOS structure data.
 */
dmi_entity_t *dmi_entity_decode(dmi_context_t *context, const void *data);

/**
 * @brief Get entity handle.
 *
 * @param[in] entity Entity descriptor.
 */
dmi_handle_t dmi_entity_handle(const dmi_entity_t *entity);

/**
 * @brief Get entity type.
 *
 * @param[in] entity Entity descriptor.
 */
dmi_type_t dmi_entity_type(const dmi_entity_t *entity);

/**
 * @brief Get entity type name.
 *
 * @param[in] entity Entity descriptor.
 */
const char *dmi_entity_name(const dmi_entity_t *entity);

/**
 * @brief Get pointer to raw SMBIOS data area of entity of the specified type.
 */
const void *dmi_entity_data(const dmi_entity_t *entity, dmi_type_t type);

/**
 * @brief Get pointer to decoded data of entity of the specified type.
 */
const void *dmi_entity_info(const dmi_entity_t *entity, dmi_type_t type);

/**
 * @brief Get entity string.
 *
 * @param[in] entity Entity descriptor.
 * @param[in] num    String number.
 * @param[in] raw    Raw value flag.
 */
const char *dmi_entity_string_ex(const dmi_entity_t *entity, dmi_string_t num, bool raw);

/**
 * @brief Destroy entity descriptor.
 *
 * @param[in] entity Entity descriptor.
 */
void dmi_entity_destroy(dmi_entity_t *entity);

__END_DECLS

static inline const char *dmi_entity_string(const dmi_entity_t *entity, dmi_string_t num)
{
    return dmi_entity_string_ex(entity, num, false);
}

#endif // !OPENDMI_ENTITY_H
