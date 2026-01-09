//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include "../../config.h"

#include <string.h>
#include <limits.h>
#include <inttypes.h>
#include <assert.h>

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef ENABLE_CURSES
#   if defined(CURSES_HAVE_NCURSES_NCURSES_H)
#       include <ncurses/ncurses.h>
#   elif defined(CURSES_HAVE_NCURSES_CURSES_H)
#       include <ncurses/curses.h>
#   elif defined(CURSES_HAVE_NCURSES_H)
#       include <ncurses.h>
#   elif defined(CURSES_HAVE_CURSES_H)
#       include <curses.h>
#   endif
#   include <term.h>
#endif // ENABLE_CURSES

#include <opendmi/context.h>
#include <opendmi/error.h>
#include <opendmi/utils.h>

#include <opendmi/format/text/handlers.h>
#include <opendmi/format/text/helpers.h>

#define COLOR_NONE (-1)
#define COLOR_GREY 8

static void dmi_tprintf(
        dmi_text_session_t *session,
        int color,
        const char *format,
        ...);

void *dmi_text_initialize(dmi_context_t *context, FILE *stream)
{
    assert(context != nullptr);
    assert(stream != nullptr);

    dmi_text_session_t *session;

    session = dmi_alloc(context, sizeof(*session));
    if (session == nullptr) {
        dmi_error_raise(context, DMI_ERROR_OUT_OF_MEMORY);
        return nullptr;
    }

    session->context = context;
    session->stream  = stream;
    session->is_tty  = isatty(fileno(session->stream));

    return session;
}

bool dmi_text_entry(dmi_text_session_t *session)
{
    assert(session != nullptr);

    dmi_context_t *context = session->context;

    char *version = dmi_version_format(context->smbios_version);
    if (version == nullptr) {
        dmi_error_raise(context, DMI_ERROR_OUT_OF_MEMORY);
        return false;
    }

    size_t entity_count = context->entity_count;
    if (entity_count == 0)
        entity_count = context->registry->count;

    dmi_tprintf(session, COLOR_NONE, "SMBIOS %s present.\n", version);
    dmi_tprintf(session, COLOR_NONE, "%zu structures occupying %zu bytes.\n",
                entity_count, context->table_area_size);
    dmi_tprintf(session, COLOR_NONE, "Table at 0x%" PRIx64 ".\n", context->table_area_addr);
    dmi_tprintf(session, COLOR_NONE, "\n");

    dmi_free(version);

    return true;
}

bool dmi_text_entity_start(dmi_text_session_t *session, const dmi_entity_t *entity)
{
    assert(session != nullptr);
    assert(entity != nullptr);

    dmi_tprintf(session, COLOR_YELLOW, "Handle 0x%04hx, DMI type %d, %zu bytes\n",
                dmi_entity_handle(entity),
                dmi_entity_type(entity),
                entity->total_length);
    dmi_tprintf(session, COLOR_YELLOW, "%s\n", dmi_entity_name(entity));

    return true;
}

bool dmi_text_entity_attr(
        dmi_text_session_t    *session,
        const dmi_entity_t    *entity,
        const dmi_attribute_t *attr,
        const void            *value)
{
    assert(session != nullptr);
    assert(entity != nullptr);
    assert(attr != nullptr);
    assert(value != nullptr);

    // Print attribute name
    dmi_tprintf(session, COLOR_NONE, "\t%s: ", attr->params.name);

    // Print attribute value
    if (not dmi_member_is_present(attr->counter)) {
        if (attr->type == DMI_ATTRIBUTE_TYPE_STRUCT)
            dmi_text_entity_attr_struct(session, attr, value);
        else
            dmi_text_entity_attr_value(session, attr, value, nullptr);
    } else {
        dmi_text_entity_attr_array(session, attr, entity->info, value);
    }

    return true;
}

void dmi_text_entity_attr_array(
        dmi_text_session_t    *session,
        const dmi_attribute_t *attr,
        const dmi_data_t      *info,
        const void            *value)
{
    assert(session != nullptr);
    assert(attr != nullptr);
    assert(info != nullptr);
    assert(value != nullptr);

    // TODO: Support counters of different sizes
    size_t count = dmi_member_value(info, attr->counter, size_t);
    const dmi_data_t *ptr = *(const dmi_data_t **)value;

    dmi_tprintf(session, COLOR_NONE, "%zu items\n", count);

    for (size_t i = 0; i < count; i++, ptr += attr->value.size) {
        dmi_tprintf(session, COLOR_NONE, "\t\t%zu: ", i);

        if (attr->type == DMI_ATTRIBUTE_TYPE_STRUCT) {
            dmi_text_entity_attr_struct(session, attr, ptr);
        } else {
            const char *descr = nullptr;

            if (attr->type == DMI_ATTRIBUTE_TYPE_HANDLE) {
                dmi_handle_t  handle = *(dmi_handle_t *)ptr;
                dmi_entity_t *entity = dmi_registry_get(session->context->registry, handle, DMI_TYPE_INVALID, true);

                descr = dmi_entity_name(entity);
            }

            dmi_text_entity_attr_value(session, attr, ptr, descr);
        }
    }
}

void dmi_text_entity_attr_struct(
        dmi_text_session_t    *session,
        const dmi_attribute_t *attr,
        const void            *value)
{
    assert(session != nullptr);
    assert(attr != nullptr);
    assert(value != nullptr);

    const dmi_attribute_t *child_attr = nullptr;

    dmi_tprintf(session, COLOR_NONE, "\n");
    for (child_attr = attr->params.attrs; child_attr->params.name; child_attr++) {
        const dmi_data_t *ptr = dmi_member_ptr(value, child_attr->value, dmi_data_t);

        dmi_tprintf(session, COLOR_NONE, "\t\t\t%s: ", child_attr->params.name);
        dmi_text_entity_attr_value(session, child_attr, ptr, nullptr);
    }
}

void dmi_text_entity_attr_value(
        dmi_text_session_t    *session,
        const dmi_attribute_t *attr,
        const void            *value,
        const char            *descr)
{
    assert(session != nullptr);
    assert(attr != nullptr);
    assert(value != nullptr);

    char *text;

    if (dmi_attribute_is_unspecified(attr, value)) {
        dmi_tprintf(session, COLOR_GREY, "<unspecified>\n");
        return;
    }
    if (dmi_attribute_is_unknown(attr, value)) {
        dmi_tprintf(session, COLOR_CYAN, "<unknown>\n");
        return;
    }

    text = dmi_attribute_format(attr, value, true);
    if (text == nullptr) {
        dmi_tprintf(session, COLOR_RED, "<error>\n");
        return;
    }

    if (attr->params.unit)
        dmi_tprintf(session, COLOR_NONE, "%s %s", text, attr->params.unit);
    else
        dmi_tprintf(session, COLOR_NONE, "%s", text);

    if (descr != nullptr)
        dmi_tprintf(session, COLOR_NONE, " - %s", descr);

    fputc('\n', session->stream);

    dmi_free(text);

    if (attr->type == DMI_ATTRIBUTE_TYPE_SET)
        dmi_text_entity_attr_set(session, attr, value);
}

void dmi_text_entity_attr_set(
        dmi_text_session_t    *session,
        const dmi_attribute_t *attr,
        const void            *value)
{
    assert(session != nullptr);
    assert(attr != nullptr);
    assert(value != nullptr);

    uintmax_t mask = dmi_attribute_get_uint(attr, value);

    for (size_t i = 0; i < attr->value.size * CHAR_BIT; i++) {
        const char *name = dmi_name_lookup(attr->params.values, i);
        if (name == nullptr)
            continue;

        bool flag = mask & (1 << i);
        dmi_tprintf(session, COLOR_NONE, "\t\t%s: %s\n", name, flag ? "yes" : "no");
    }
}

bool dmi_text_entity_data(dmi_text_session_t *session, const dmi_entity_t *entity)
{
    assert(session != nullptr);
    assert(entity != nullptr);

    dmi_tprintf(session, COLOR_NONE, "\tHeader and data:\n");
    dmi_text_hex_data(session, entity->data, entity->body_length);

    return true;
}

bool dmi_text_entity_strings(dmi_text_session_t *session, const dmi_entity_t *entity)
{
    assert(session != nullptr);
    assert(entity != nullptr);

    if (entity->string_count == 0)
        return true;

    dmi_tprintf(session, COLOR_NONE, "\tStrings:\n");

    for (dmi_string_t i = 1; i <= entity->string_count; i++) {
        const char *str = dmi_entity_string_ex(entity, i, true);

        dmi_text_hex_data(session, str, strlen(str) + 1);
        dmi_tprintf(session, COLOR_NONE, "\t\t\"%s\"\n", str);
    }

    return true;
}

bool dmi_text_entity_end(dmi_text_session_t *session, const dmi_entity_t *entity)
{
    assert(session != nullptr);
    assert(entity != nullptr);

    dmi_unused(entity);

    dmi_tprintf(session, COLOR_NONE, "\n");

    return true;
}

void dmi_text_finalize(dmi_text_session_t *session)
{
    assert(session != nullptr);

    dmi_free(session);
}

static void dmi_tprintf(
        dmi_text_session_t *session,
        int color,
        const char *format,
        ...)
{
    va_list args;

#ifdef ENABLE_CURSES
    if (session->is_tty and (color >= 0))
        tputs(tparm(tigetstr("setaf"), color), 1, putchar);
#endif // ENABLE_CURSES

    va_start(args, format);
    vfprintf(session->stream, format, args);
    va_end(args);

#ifdef ENABLE_CURSES
    if (session->is_tty and (color >= 0))
        tputs(tparm(tigetstr("sgr0")), 1, putchar);
#endif // ENABLE_CURSES
}
