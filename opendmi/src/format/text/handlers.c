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

    fprintf(session->stream, "SMBIOS %s present.\n", version);
    fprintf(session->stream, "%zu structures occupying %zu bytes.\n",
            entity_count, context->table_area_size);
    fprintf(session->stream, "Table at 0x%" PRIx64 ".\n", context->table_area_addr);
    fprintf(session->stream, "\n");

    dmi_free(version);

    return true;
}

bool dmi_text_entity_start(dmi_text_session_t *session, const dmi_entity_t *entity)
{
    assert(session != nullptr);
    assert(entity != nullptr);

#ifdef ENABLE_CURSES
    if (session->is_tty)
        tputs(tparm(tigetstr("setaf"), COLOR_YELLOW), 1, putchar);
#endif // ENABLE_CURSES

    fprintf(session->stream, "Handle 0x%04hx, DMI type %d, %zu bytes\n",
           dmi_entity_handle(entity),
           dmi_entity_type(entity),
           entity->total_length);
    fprintf(session->stream, "%s\n", dmi_entity_name(entity));

#ifdef ENABLE_CURSES
    if (session->is_tty)
        tputs(tparm(tigetstr("sgr0")), 1, putchar);
#endif // ENABLE_CURSES

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
    fprintf(session->stream, "\t%s: ", attr->params.name);

    // Print attribute value
    if (not dmi_member_is_present(attr->counter)) {
        if (attr->type == DMI_ATTRIBUTE_TYPE_STRUCT)
            dmi_text_entity_attr_struct(session, attr, value);
        else
            dmi_text_entity_attr_value(session, attr, value);
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

    fprintf(session->stream, "%zu items\n", count);

    for (size_t i = 0; i < count; i++, ptr += attr->value.size) {
        fprintf(session->stream, "\t\t%zu: ", i);

        if (attr->type == DMI_ATTRIBUTE_TYPE_STRUCT)
            dmi_text_entity_attr_struct(session, attr, ptr);
        else
            dmi_text_entity_attr_value(session, attr, ptr);
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

    fprintf(session->stream, "\n");
    for (child_attr = attr->params.attrs; child_attr->params.name; child_attr++) {
        const dmi_data_t *ptr = dmi_member_ptr(value, child_attr->value, dmi_data_t);

        fprintf(session->stream, "\t\t\t%s: ", child_attr->params.name);
        dmi_text_entity_attr_value(session, child_attr, ptr);
    }
}

void dmi_text_entity_attr_value(
        dmi_text_session_t    *session,
        const dmi_attribute_t *attr,
        const void            *value)
{
    assert(session != nullptr);
    assert(attr != nullptr);
    assert(value != nullptr);

    char *text;

    if (dmi_attribute_is_unspecified(attr, value)) {
        fprintf(session->stream, "<unspecified>\n");
        return;
    }
    if (dmi_attribute_is_unknown(attr, value)) {
        fprintf(session->stream, "<unknown>\n");
        return;
    }

    text = dmi_attribute_format(attr, value, true);
    if (text == nullptr) {
        fprintf(session->stream, "<error>\n");
        return;
    }

    if (attr->params.unit)
        fprintf(session->stream, "%s %s\n", text, attr->params.unit);
    else
        fprintf(session->stream, "%s\n", text);

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
        fprintf(session->stream, "\t\t%s: %s\n", name, flag ? "yes" : "no");
    }
}

bool dmi_text_entity_data(dmi_text_session_t *session, const dmi_entity_t *entity)
{
    assert(session != nullptr);
    assert(entity != nullptr);

    fprintf(session->stream, "\tHeader and data:\n");
    dmi_text_hex_data(session, entity->data, entity->body_length);

    return true;
}

bool dmi_text_entity_strings(dmi_text_session_t *session, const dmi_entity_t *entity)
{
    assert(session != nullptr);
    assert(entity != nullptr);

    if (entity->string_count == 0)
        return true;

    fprintf(session->stream, "\tStrings:\n");

    for (dmi_string_t i = 1; i <= entity->string_count; i++) {
        const char *str = dmi_entity_string(entity, i);

        dmi_text_hex_data(session, str, strlen(str) + 1);
        fprintf(session->stream, "\t\t\"%s\"\n", str);
    }

    return true;
}

bool dmi_text_entity_end(dmi_text_session_t *session, const dmi_entity_t *entity)
{
    assert(session != nullptr);
    assert(entity != nullptr);

    dmi_unused(entity);

    fprintf(session->stream, "\n");

    return true;
}

void dmi_text_finalize(dmi_text_session_t *session)
{
    assert(session != nullptr);

    dmi_free(session);
}
