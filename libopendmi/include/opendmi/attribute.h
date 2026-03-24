//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_ATTRIBUTE_H
#define OPENDMI_ATTRIBUTE_H

#pragma once

#include <opendmi/utils/name.h>
#include <opendmi/utils/version.h>

typedef struct dmi_attribute        dmi_attribute_t;
typedef struct dmi_attribute_params dmi_attribute_params_t;
typedef struct dmi_attribute_ops    dmi_attribute_ops_t;

typedef char *dmi_attribute_format_fn(
        dmi_context_t         *context,
        const dmi_attribute_t *attribute,
        const void            *value,
        bool                   pretty);

typedef void dmi_attribute_parse_fn(const dmi_attribute_t *attr);

typedef enum dmi_attribute_type
{
    DMI_ATTRIBUTE_TYPE_NONE,
    DMI_ATTRIBUTE_TYPE_STRUCT,
    DMI_ATTRIBUTE_TYPE_HANDLE,
    DMI_ATTRIBUTE_TYPE_STRING,
    DMI_ATTRIBUTE_TYPE_BOOL,
    DMI_ATTRIBUTE_TYPE_INTEGER,
    DMI_ATTRIBUTE_TYPE_DECIMAL,
    DMI_ATTRIBUTE_TYPE_SIZE,
    DMI_ATTRIBUTE_TYPE_ADDRESS,
    DMI_ATTRIBUTE_TYPE_ENUM,
    DMI_ATTRIBUTE_TYPE_SET,
    DMI_ATTRIBUTE_TYPE_VERSION,
    DMI_ATTRIBUTE_TYPE_DATE,
    DMI_ATTRIBUTE_TYPE_UUID
} dmi_attribute_type_t;

typedef enum dmi_attribute_flag
{
    DMI_ATTRIBUTE_FLAG_SIGNED    = 0x01,
    DMI_ATTRIBUTE_FLAG_HEX       = 0x02,
    DMI_ATTRIBUTE_FLAG_BCD       = 0x04,
    DMI_ATTRIBUTE_FLAG_TRANSIENT = 0x08
} dmi_attribute_flag_t;

struct dmi_attribute_ops
{
    dmi_attribute_format_fn *format;
    dmi_attribute_parse_fn  *parse;
};

/**
 * @brief Attribute parameters.
 */
struct dmi_attribute_params
{
    /**
     * @brief Code name, used to identify the attribute during serialization.
     */
    char *code;

    /**
     * @brief Printable name, used to identify the attribute when printing.
     */
    char *name;

    /**
     * @brief Measurement units. Valid only for integer or decimal values,
     * should be set to `nullptr` in other cases.
     */
    const char *unit;

    /**
     * @brief Number of decimal digits after the decimal point. Valid only for
     * decimal attributes, should be set to zero in other cases.
     */
    unsigned int scale;

    /**
     * @brief Pointer to unspecified value, if applicable.
     */
    const void *unspec;

    /**
     * @brief Pointer to unknown value, if applicable.
     */
    const void *unknown;

    /**
     * @brief Flags.
     */
    unsigned int flags;

    /**
     * @brief Enumeration or boolean value names.
     */
    const dmi_name_set_t *values;

    /**
     * @brief Structure attribute descriptors.
     */
    const dmi_attribute_t *attrs;

    /**
     * @brief Minimum SMBIOS version supporting this attribute.
     */
    dmi_version_t level;
};

struct dmi_attribute
{
    dmi_member_ref_t value;
    dmi_member_ref_t counter;
    dmi_attribute_type_t type;
    dmi_attribute_params_t params;
};

#define DMI_ATTRIBUTE(__entity, __member, __type, ...) \
    {                                                  \
        .value   = dmi_member(__entity, __member),     \
        .counter = DMI_MEMBER_NULL,                    \
        .type    = DMI_ATTRIBUTE_TYPE_ ## __type,      \
        .params  = __VA_ARGS__                         \
    }

#define DMI_ATTRIBUTE_ARRAY(__entity, __member, __counter, __type, ...) \
    {                                                                   \
        .value   = dmi_member_array(__entity, __member),                \
        .counter = dmi_member(__entity, __counter),                     \
        .type    = DMI_ATTRIBUTE_TYPE_ ## __type,                       \
        .params  = __VA_ARGS__                                          \
    }

#define DMI_ATTRIBUTE_NULL                  \
    {                                       \
        .value   = DMI_MEMBER_NULL,         \
        .counter = DMI_MEMBER_NULL,         \
        .type    = DMI_ATTRIBUTE_TYPE_NONE, \
        .params  = {}                       \
    }

__BEGIN_DECLS

bool dmi_attribute_is_unspecified(const dmi_attribute_t *attr, const void *value);
bool dmi_attribute_is_unknown(const dmi_attribute_t *attr, const void *value);

bool dmi_attribute_get_bool(const dmi_attribute_t *attr, const void *value);
intmax_t dmi_attribute_get_int(const dmi_attribute_t *attr, const void *value);
uintmax_t dmi_attribute_get_uint(const dmi_attribute_t *attr, const void *value);

char *dmi_attribute_format(
        dmi_context_t         *context,
        const dmi_attribute_t *attribute,
        const void            *value,
        bool                  pretty);

__END_DECLS

#endif // !OPENDMI_ATTRIBUTE_H
