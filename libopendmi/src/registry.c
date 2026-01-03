//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <assert.h>

#include <opendmi/registry.h>
#include <opendmi/context.h>
#include <opendmi/entity.h>
#include <opendmi/log.h>
#include <opendmi/utils.h>

/**
 * @internal
 */
static bool dmi_registry_put(dmi_registry_t *registry, dmi_entity_t *entity);

dmi_registry_t *dmi_registry_create(dmi_context_t *context, size_t capacity)
{
    bool success = false;
    dmi_registry_t *registry = nullptr;

    dmi_log_debug(context, "Creating registry...");

    if (capacity == 0)
        capacity = DMI_REGISTRY_CAPACITY;

    registry = dmi_alloc(context, sizeof(*registry));
    if (registry == nullptr)
        return nullptr;

    registry->context  = context;
    registry->capacity = capacity;

    do {
        registry->index = dmi_alloc_array(context, sizeof(dmi_registry_entry_t *), capacity);
        if (registry->index == nullptr)
            break;

        success = true;
    } while (false);

    if (not success) {
        dmi_registry_destroy(registry);
        return nullptr;
    }

    return registry;
}

dmi_entity_t *dmi_registry_get(dmi_registry_t *registry, dmi_handle_t handle, dmi_type_t type)
{
    dmi_registry_entry_t *entry = nullptr;

    if (registry == nullptr)
        return nullptr;

    if ((handle == DMI_HANDLE_INVALID) or (handle == DMI_HANDLE_UNSUPPORTED)) {
        dmi_error_raise_ex(registry->context, DMI_ERROR_INVALID_ARGUMENT, "handle: 0x%04x", handle);
        return nullptr;
    }

    entry = registry->index[(size_t)handle % registry->capacity];

    while (entry != nullptr) {
        if (entry->entity->handle == handle)
            break;
        entry = entry->next;
    }

    if (entry == nullptr) {
        dmi_error_raise_ex(registry->context, DMI_ERROR_ENTITY_NOT_FOUND, "0x%04x", handle);
        return nullptr;
    }

    if ((type != DMI_TYPE_INVALID) and (entry->entity->type != type)) {
        dmi_error_raise_ex(registry->context, DMI_ERROR_INVALID_ENTITY_TYPE, "0x%04x", handle);
        return nullptr;
    }

    return entry->entity;
}

dmi_entity_t *dmi_registry_get_any(
        dmi_registry_t *registry,
        dmi_handle_t    handle,
        dmi_type_t     *type)
{
    dmi_entity_t *entity;

    if (registry == nullptr)
        return nullptr;

    entity = dmi_registry_get(registry, handle, DMI_TYPE_INVALID);
    if (entity == nullptr)
        return nullptr;

    if (type != nullptr) {
        while (*type != DMI_TYPE_INVALID) {
            if (entity->type == *type)
                break;
            type++;
        }

        if (*type == DMI_TYPE_INVALID) {
            dmi_error_raise_ex(registry->context, DMI_ERROR_INVALID_ENTITY_TYPE, "0x%04x", handle);
            return nullptr;
        }
    }

    return entity;
}

void dmi_registry_destroy(dmi_registry_t *registry)
{
    if (registry == nullptr)
        return;

    dmi_log_debug(registry->context, "Destroying registry...");

    if (registry->index) {
        dmi_registry_entry_t *entry, *next;

        for (size_t i = 0; i < registry->capacity; i++) {
            entry = registry->index[i];

            while (entry != nullptr) {
                next = entry->next;

                dmi_entity_destroy(entry->entity);
                dmi_free(entry);

                entry = next;
            }
        }

        dmi_free(registry->index);
    }

    dmi_free(registry);
}

bool dmi_registry_build(dmi_registry_t *registry)
{
    bool success = false;
    size_t index = 0;
    dmi_context_t *context = registry->context;
    const dmi_data_t *ptr = context->table_data;

    dmi_log_debug(context, "Scanning SMBIOS structures...");

    // Scan table area
    while ((context->entity_count == 0) or (index < context->entity_count)) {
        dmi_entity_t *entity = nullptr;

        // Check structure address
        if ((size_t)(ptr - context->table_data) > context->table_area_size) {
            dmi_error_raise(context, DMI_ERROR_INVALID_ENTITY_ADDR);
            goto exit;
        }

        // Decode structure into entity
        entity = dmi_entity_decode(context, ptr);
        if (entity == nullptr) {
            dmi_error_raise_ex(context, DMI_ERROR_ENTITY_DECODE,
                               "0x%04x (%s)", entity->handle, entity->spec->name);
            goto exit;
        }

        if (entity->type == DMI_TYPE_END_OF_TABLE) {
            dmi_entity_destroy(entity);
            break;
        }

        // Add entity to registry
        if (not dmi_registry_put(registry, entity)) {
            dmi_error_raise_ex(context, DMI_ERROR_ENTITY_REGISTER,
                               "0x%04x (%s)", entity->handle, entity->spec->name);
            dmi_entity_destroy(entity);
            goto exit;
        }

        // Update structure pointer and index
        ptr += entity->total_length;
        index++;
    }

    // Set entity count
    registry->count = index + 1;
    dmi_log_debug(context, "Found %zu structures", registry->count);

    success = true;

exit:
    return success;
}

bool dmi_registry_link(dmi_registry_t *registry)
{
    dmi_registry_iter_t iter;
    dmi_entity_t *entity;

    dmi_log_debug(registry->context, "Linking SMBIOS structures...");

    dmi_registry_iter_init(&iter, registry, nullptr);
    while ((entity = dmi_registry_iter_next(&iter)) != nullptr) {
        if ((entity->spec == nullptr) or (entity->spec->handlers.link == nullptr))
            continue;

        dmi_log_debug(registry->context, "%p: Handle 0x%04hx, length %zu, type %d (%s)",
                      entity->data,
                      entity->handle,
                      entity->body_length,
                      entity->type,
                      dmi_type_name(registry->context, entity->type));

        if (not entity->spec->handlers.link(entity)) {
            dmi_error_raise_ex(entity->context, DMI_ERROR_ENTITY_LINK,
                               "0x%04x (%s)", entity->handle, entity->spec->name);
            return false;
        }
    }

    return true;
}

static bool dmi_registry_put(dmi_registry_t *registry, dmi_entity_t *entity)
{
    size_t hash;
    dmi_registry_entry_t *entry;
    dmi_registry_entry_t *last;

    // Allocate new entry
    entry = dmi_alloc(registry->context, sizeof(dmi_registry_entry_t));
    if (entry == nullptr)
        return false;

    // Initialize entry
    memset(entry, 0, sizeof(dmi_registry_entry_t));
    entry->entity = entity;

    hash = (size_t)entity->handle % registry->capacity;
    last = registry->index[hash];

    // Add entry to index
    if (last == nullptr) {
        registry->index[hash] = entry;
    } else {
        while (true) {
            if (last->entity->data == entity->data) {
                dmi_free(entry);
                dmi_error_raise_ex(registry->context, DMI_ERROR_DUPLICATE_ENTRY,
                                   "%p (0x%04x)", entity->data, entity->handle);
                return false;
            }

            if (last->entity->handle == entity->handle) {
                dmi_error_raise_ex(registry->context, DMI_ERROR_DUPLICATE_HANDLE,
                                   "0x%04x", entity->handle);
            }

            if (last->next == nullptr)
                break;

            last = last->next;
        }

        last->next = entry;
    }

    entry->seq_prev = registry->tail;

    if (registry->tail != nullptr)
        registry->tail->seq_next = entry;
    registry->tail = entry;

    if (registry->head == nullptr)
        registry->head = entry;

    return true;
}

bool dmi_registry_iter_init(
        dmi_registry_iter_t *iter,
        dmi_registry_t *registry,
        dmi_filter_t *filter)
{
    assert(iter != nullptr);
    assert(registry != nullptr);

    if ((iter == nullptr) or (registry == nullptr))
        return false;

    iter->registry = registry;
    iter->position = nullptr;
    iter->filter   = filter;
    iter->is_done  = false;

    return false;
}

bool dmi_registry_iter_has_next(dmi_registry_iter_t *iter)
{
    dmi_registry_entry_t *next;

    if ((iter == nullptr) or iter->is_done)
        return false;

    if (iter->position == nullptr)
        next = iter->registry->head;
    else
        next = iter->position->seq_next;

    if (iter->filter != nullptr) {
        while (not dmi_filter_match(iter->filter, next->entity))
            next = next->seq_next;
    }

    return next != nullptr;
}

dmi_entity_t *dmi_registry_iter_next(dmi_registry_iter_t *iter)
{
    if ((iter == nullptr) or iter->is_done)
        return nullptr;

    if (iter->position == nullptr)
        iter->position = iter->registry->head;
    else
        iter->position = iter->position->seq_next;

    if (iter->filter != nullptr) {
        while (not dmi_filter_match(iter->filter, iter->position->entity))
            iter->position = iter->position->seq_next;
    }

    if (iter->position == nullptr) {
        iter->is_done = true;
        return nullptr;
    }

    return iter->position->entity;
}
