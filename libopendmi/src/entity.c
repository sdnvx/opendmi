//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

#include <opendmi/context.h>
#include <opendmi/entity.h>
#include <opendmi/utils.h>
#include <opendmi/utils/codec.h>

/**
 * @internal
 */
static bool dmi_entity_decode_length(
        dmi_entity_t *entity,
        size_t        max_length);

/**
 * @internal
 */
static bool dmi_entity_decode_strings(dmi_entity_t *entity);

/**
 * @internal
 */
static char *dmi_entity_string_trim(dmi_context_t *context, const char *ptr);

dmi_entity_t *dmi_entity_create(
        dmi_context_t *context,
        const void    *data,
        size_t         max_length)
{
    dmi_entity_t *entity = nullptr;

    if (context == nullptr)
        return nullptr;

    if (data == nullptr) {
        dmi_error_raise_ex(context, DMI_ERROR_NULL_ARGUMENT, "data");
        return nullptr;
    }

    dmi_header_t *header = dmi_cast(header, data);
    dmi_type_t    type   = dmi_decode(header->type);
    size_t        length = dmi_decode(header->length);
    dmi_handle_t  handle = dmi_decode(header->handle);

    dmi_log_debug(context->logger,
                  "%p: Handle 0x%04x, length %zu, type %d (%s)",
                  data, handle, length, (int)header->type,
                  dmi_type_name(context, type));

    // Check structure length
    if (length == 0) {
        type   = DMI_TYPE(END_OF_TABLE);
        length = sizeof(dmi_header_t);
    } else if (length < sizeof(dmi_header_t)) {
        dmi_error_raise_ex(context, DMI_ERROR_INVALID_ENTITY_LENGTH,
                           "0x%04hx (%s): %zu bytes",
                           handle, dmi_type_name(context, type), length);
        return nullptr;
    } else if (length > max_length) {
        dmi_error_raise_ex(context, DMI_ERROR_ENTITY_TRUNCATED,
                           "0x%04hx (%s): %zu bytes long, only %zu available",
                           handle, dmi_type_name(context, type), length, max_length);
    }

    // Allocate new entity descriptor
    entity = dmi_alloc(context, sizeof(dmi_entity_t));
    if (entity == nullptr)
        return nullptr;

    // Decode structure header
    entity->context     = context;
    entity->type        = type;
    entity->body_length = length;
    entity->handle      = handle;
    entity->data        = data;

    // Decode structure data
    bool success = false;
    do {
        // Decode strings
        if (not dmi_entity_decode_length(entity, max_length)) {
            dmi_error_raise_ex(context, DMI_ERROR_ENTITY_TRUNCATED,
                               "0x%04hx (%s): Unformatted area exceeds limits",
                               handle, dmi_type_name(context, type));
            break;
        }
        if (not dmi_entity_decode_strings(entity))
            break;

        success = true;
    } while (false);

    if (not success) {
        dmi_entity_destroy(entity);
        return nullptr;
    }

    return entity;
}

bool dmi_entity_decode(dmi_entity_t *entity)
{
    if (entity == nullptr)
        return false;
    if (entity->state & DMI_ENTITY_STATE_DECODED)
        return true;

    dmi_context_t *context = entity->context;

    const dmi_entity_spec_t *spec = dmi_type_spec(entity->context, entity->type);
    if (spec == nullptr)
        return true;

    entity->spec  = spec;
    entity->level = spec->minimum_version;

    // Check minimum length constraint
    if ((spec->minimum_length != 0) and (entity->body_length < spec->minimum_length)) {
        dmi_error_raise_ex(context, DMI_ERROR_INVALID_ENTITY_LENGTH, "%zu", entity->body_length);
        return false;
    }

    if (spec->handlers.decode == nullptr)
        return true;

    // Allocate structure descriptor
    entity->info = dmi_alloc(context, spec->decoded_length);
    if (entity->info == nullptr)
        return false;

    // Initialize stream
    dmi_stream_initialize(&entity->stream, entity);
    dmi_stream_seek(&entity->stream, sizeof(dmi_header_t));

    // Execute decoder
    bool status = spec->handlers.decode(entity);

    if (status) {
        entity->state |= DMI_ENTITY_STATE_DECODED;
    } else {
        dmi_error_raise_ex(context, DMI_ERROR_ENTITY_DECODE,
                           "0x%04x (%s)", entity->handle, entity->spec->name);

        // Call cleanup handler on errors
        if (spec->handlers.cleanup != nullptr)
            spec->handlers.cleanup(entity);

        dmi_free(entity->info);

        entity->info  = nullptr;
        entity->level = DMI_VERSION_NONE;
    }

    // Reset stream after decoding
    dmi_stream_reset(&entity->stream);

    return status;
}

bool dmi_entity_link(dmi_entity_t *entity)
{
    if (entity == nullptr)
        return false;

    if (entity->spec == nullptr)
        return false;
    if (entity->spec->handlers.link == nullptr)
        return false;

    if (entity->state & DMI_ENTITY_STATE_LINKED)
        return true;

    if (not entity->spec->handlers.link(entity)) {
        dmi_context_t *context = entity->context;
        dmi_error_raise_ex(context, DMI_ERROR_ENTITY_LINK,
                           "0x%04x (%s)", entity->handle, entity->spec->name);
        return false;
    }

    entity->state |= DMI_ENTITY_STATE_LINKED;

    return true;
}

dmi_handle_t dmi_entity_handle(const dmi_entity_t *entity)
{
    if (entity == nullptr)
        return DMI_HANDLE_INVALID;

    return entity->handle;
}

dmi_type_t dmi_entity_type(const dmi_entity_t *entity)
{
    if (entity == nullptr)
        return DMI_TYPE_INVALID;

    return entity->type;
}

const char *dmi_entity_name(const dmi_entity_t *entity)
{
    if (entity == nullptr)
        return nullptr;

    return dmi_type_name(entity->context, entity->type);
}

const void *dmi_entity_data(const dmi_entity_t *entity, dmi_type_t type)
{
    if (entity == nullptr)
        return nullptr;

    if ((type != DMI_TYPE_INVALID) and (entity->type != type)) {
        dmi_error_raise(entity->context, DMI_ERROR_INVALID_ENTITY_TYPE);
        return nullptr;
    }

    return entity->data;
}

void *dmi_entity_info(const dmi_entity_t *entity, dmi_type_t type)
{
    if (entity == nullptr)
        return nullptr;

    if ((type != DMI_TYPE_INVALID) and (entity->type != type)) {
        dmi_error_raise(entity->context, DMI_ERROR_INVALID_ENTITY_TYPE);
        return nullptr;
    }

    return entity->info;
}

const char *dmi_entity_string_ex(const dmi_entity_t *entity, dmi_string_t num, bool raw)
{
    dmi_string_entry_t *entry;

    if ((entity == nullptr) or (num == 0))
        return nullptr;

    if (num > entity->string_count) {
        dmi_error_raise_ex(entity->context, DMI_ERROR_STRING_NOT_FOUND,
                           "Handle 0x%04x, index %u", entity->handle, num);
        return nullptr;
    }

    entry = &entity->strings[num - 1];

    if (entry->raw == nullptr)
        return nullptr;

    return raw ? entry->raw : entry->pretty;
}

void dmi_entity_destroy(dmi_entity_t *entity)
{
    if (entity == nullptr)
        return;

    if ((entity->spec != nullptr) and (entity->info != nullptr)) {
        if (entity->spec->handlers.cleanup != nullptr)
            entity->spec->handlers.cleanup(entity);

        dmi_free(entity->info);
    }

    if (entity->strings) {
        for (size_t i = 0; i < entity->string_count; i++)
            dmi_free(entity->strings[i].pretty);

        dmi_free(entity->strings);
    }

    dmi_free(entity);
}

static bool dmi_entity_decode_length(
        dmi_entity_t *entity,
        size_t        max_length)
{
    assert(entity != nullptr);
    assert(entity->data != nullptr);
    assert(max_length >= sizeof(dmi_header_t) + 2);

    const char *data      = dmi_cast(data, entity->data);
    const char *start     = data + entity->body_length;
    const char *ptr       = start;
    size_t      count     = 0;
    size_t      remaining = max_length - (ptr - data);

    if (--remaining == 0)
        return false;

    // Calculate structure length and count strings
    while (true) {
        if (*ptr != 0) {
            count++;

            if (--remaining == 0)
                return false;

            while (*ptr != 0) {
                ptr++;
                if (--remaining == 0)
                    return false;
            }
        }

        ptr++;
        if (*ptr == 0) {
            ptr++;
            break;
        }
    }

    entity->extra_length = ptr - start;
    entity->total_length = entity->body_length + entity->extra_length;
    entity->string_count = count;

    return true;
}

static bool dmi_entity_decode_strings(dmi_entity_t *entity)
{
    assert(entity != nullptr);
    assert(entity->data != nullptr);

    bool success = false;
    dmi_string_entry_t *strings = nullptr;

    // Allocate strings index
    strings = dmi_alloc_array(entity->context, sizeof(dmi_string_entry_t), entity->string_count + 1);
    if (strings == nullptr)
        return false;

    // Fetch string pointers
    if (entity->string_count > 0) {
        const char *data  = dmi_cast(data, entity->data);
        const char *start = data + entity->body_length;
        const char *ptr   = start;
        size_t      index = 0;

        while (true) {
            dmi_string_entry_t *entry = &strings[index++];

            if (*ptr != 0) {
                entry->raw    = ptr;
                entry->pretty = dmi_entity_string_trim(entity->context, ptr);

                if (!entry->pretty) {
                    dmi_error_raise(entity->context, DMI_ERROR_OUT_OF_MEMORY);
                    break;
                }

                while (*ptr != 0) {
                    ptr++;
                }
            }

            ptr++;

            if (*ptr == 0) {
                success = true;
                break;
            }
        }
    } else {
        success = true;
    }

    if (!success) {
        for (size_t i = 0; i < entity->string_count; i++)
            dmi_free(strings[i].pretty);
        dmi_free(strings);

        return false;
    }

    entity->strings = strings;

    return true;
}

static char *dmi_entity_string_trim(dmi_context_t *context, const char *ptr)
{
    assert(ptr != nullptr);

    size_t length;
    char  *str = nullptr;

    // Trim leading whitespaces
    ptr += strspn(ptr, "\t\n\v\f\r ");

    // Trim trailing whitespaces
    length = strlen(ptr);
    while (length > 0) {
        if (strchr("\t\n\v\f\r ", ptr[length - 1]) == nullptr)
            break;
        length--;
    }

    // Allocate string
    str = dmi_alloc(context, length + 1);
    if (str == nullptr)
        return nullptr;

    // Copy string value
    memcpy(str, ptr, length);

    return str;
}
