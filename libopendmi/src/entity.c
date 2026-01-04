//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <opendmi/context.h>
#include <opendmi/entity.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

/**
 * @internal
 */
static bool dmi_entity_decode_length(dmi_entity_t *entity);

/**
 * @internal
 */
static bool dmi_entity_decode_strings(dmi_entity_t *entity);

dmi_entity_t *dmi_entity_decode(dmi_context_t *context, const void *data)
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

    dmi_log_debug(context, "%p: Handle 0x%04x, length %zu, type %d (%s)",
                  data, (unsigned)handle, length, (int)header->type,
                  dmi_type_name(context, type));

    // Check structure length
    if (length == 0) {
        type   = DMI_TYPE_END_OF_TABLE;
        length = sizeof(dmi_header_t);
    } else if (length < sizeof(dmi_header_t)) {
        dmi_error_raise_ex(context, DMI_ERROR_INVALID_ENTITY_LENGTH, "%zu", length);
        return nullptr;
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
    entity->spec        = dmi_type_spec(context, entity->type);
    entity->data        = data;

    // Decode structure data
    bool success = false;
    do {
        // Decode strings
        if (not dmi_entity_decode_length(entity))
            break;
        if (not dmi_entity_decode_strings(entity))
            break;

        // Decode information
        if ((entity->spec != nullptr) and (entity->spec->handlers.decode != nullptr)) {
            entity->info = entity->spec->handlers.decode(entity, &entity->level);
            if (entity->info == nullptr)
                break;
        }

        success = true;
    } while (false);

    if (not success) {
        dmi_entity_destroy(entity);
        return nullptr;
    }

    return entity;
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

const void *dmi_entity_info(const dmi_entity_t *entity, dmi_type_t type)
{
    if (entity == nullptr)
        return nullptr;

    if ((type != DMI_TYPE_INVALID) and (entity->type != type)) {
        dmi_error_raise(entity->context, DMI_ERROR_INVALID_ENTITY_TYPE);
        return nullptr;
    }

    return entity->info;
}

const char *dmi_entity_string(const dmi_entity_t *entity, dmi_string_t num)
{
    if (entity == nullptr)
        return nullptr;

    if (num > entity->string_count) {
        dmi_error_raise_ex(entity->context, DMI_ERROR_STRING_NOT_FOUND,
                           "Handle 0x%04x, index %u", entity->handle, num);
        return nullptr;
    }

    return num > 0 ? entity->strings[num - 1] : nullptr;
}

void dmi_entity_destroy(dmi_entity_t *entity)
{
    if (entity == nullptr)
        return;

    if ((entity->spec != nullptr) and (entity->info != nullptr)) {
        if (entity->spec->handlers.free)
            entity->spec->handlers.free(entity->info);
        else
            dmi_free(entity->info);
    }

    dmi_free(entity->strings);
    dmi_free(entity);
}

static bool dmi_entity_decode_length(dmi_entity_t *entity)
{
    assert(entity != nullptr);
    assert(entity->data != nullptr);

    const char *data  = dmi_cast(data, entity->data);
    const char *start = data + entity->body_length;
    const char *ptr   = start;
    size_t      count = 0;

    // Calculate structure length and count strings
    while (true) {
        if (*ptr != 0) {
            count++;

            while (*ptr != 0) {
                ptr++;
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

    const char *data  = dmi_cast(data, entity->data);
    const char *start = data + entity->body_length;
    const char *ptr   = start;
    size_t      index = 0;

    // Allocate strings index
    entity->strings = dmi_alloc_array(entity->context, sizeof(const char *), entity->string_count + 1);
    if (entity->strings == nullptr)
        return false;

    // Fetch string pointers
    if (entity->string_count > 0) {
        while (true) {
            if (*ptr != 0) {
                entity->strings[index++] = ptr;

                while (*ptr != 0) {
                    ptr++;
                }
            }

            ptr++;

            if (*ptr == 0) {
                ptr++;
                break;
            }
        }
    }

    // Terminate strings index
    entity->strings[index] = nullptr;

    return true;
}
