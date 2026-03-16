//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_FORMAT_H
#define OPENDMI_FORMAT_H

#pragma once

#include <stdio.h>

#include <opendmi/attribute.h>

typedef struct dmi_format     dmi_format_t;
typedef struct dmi_format_ops dmi_format_ops_t;

typedef enum dmi_format_flags
{
    DMI_FORMAT_FLAG_PRETTY = 1 << 0,
    DMI_FORMAT_FLAG_QUIET  = 1 << 1
} dmi_format_flags_t;

typedef void *dmi_format_initialize_fn(dmi_context_t *context, FILE *stream);

typedef bool dmi_format_dump_start_fn(void *session);
typedef bool dmi_format_entry_fn(void *session);
typedef bool dmi_format_table_start_fn(void *session);

typedef bool dmi_format_entity_start_fn(void *session, const dmi_entity_t *entity);
typedef bool dmi_format_entity_attrs_start_fn(void *session, const dmi_entity_t *entity);

typedef bool dmi_format_entity_attr_fn(
            void                  *session,
            const dmi_entity_t    *entity,
            const dmi_attribute_t *attr,
            const void            *value);

typedef bool dmi_format_entity_attrs_end_fn(void *session, const dmi_entity_t *entity);
typedef bool dmi_format_entity_data_fn(void *session, const dmi_entity_t *entity);
typedef bool dmi_format_entity_strings_fn(void *session, const dmi_entity_t *entity);
typedef bool dmi_format_entity_end_fn(void *session, const dmi_entity_t *entity);

typedef bool dmi_format_table_end_fn(void *session);
typedef bool dmi_format_dump_end_fn(void *session);

typedef void dmi_format_finalize_fn(void *session);

struct dmi_format_ops
{
    dmi_format_initialize_fn *initialize;

    dmi_format_dump_start_fn  *dump_start;
    dmi_format_entry_fn       *entry;
    dmi_format_table_start_fn *table_start;

    dmi_format_entity_start_fn *entity_start;

    dmi_format_entity_attrs_start_fn *entity_attrs_start;
    dmi_format_entity_attr_fn        *entity_attr;
    dmi_format_entity_attrs_end_fn   *entity_attrs_end;

    dmi_format_entity_data_fn    *entity_data;
    dmi_format_entity_strings_fn *entity_strings;

    dmi_format_entity_end_fn *entity_end;

    dmi_format_table_end_fn *table_end;
    dmi_format_dump_end_fn  *dump_end;

    dmi_format_finalize_fn *finalize;
};

struct dmi_format
{
    const char *code;
    const char *name;
    const dmi_format_ops_t handlers;
};

extern const dmi_format_t *dmi_formats[];
extern const dmi_format_t *dmi_format_default;

__BEGIN_DECLS

const dmi_format_t *dmi_format_get(const char *code);

__END_DECLS

#endif // !OPENDMI_FORMAT_H
