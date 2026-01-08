//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_CONTEXT_H
#define OPENDMI_CONTEXT_H

#pragma once

#include <opendmi/types.h>
#include <opendmi/error.h>
#include <opendmi/log.h>
#include <opendmi/backend.h>
#include <opendmi/extension.h>
#include <opendmi/registry.h>
#include <opendmi/utils/version.h>

/**
 * @brief DMI context descriptor.
 */
struct dmi_context
{
    /**
     * @brief SMBIOS version number.
     */
    dmi_version_t smbios_version;

    /**
     * @brief SMBIOS entry point revision.
     */
    dmi_version_t entry_version;

    /**
     * @brief Pointer to SMBIOS entry point data.
     */
    void *entry_data;

    /**
     * @brief Actual size of entry point data.
     */
    size_t entry_size;

    /**
     * @brief Total number of SMBIOS structures.
     */
    size_t entity_count;

    /**
     * @brief Address of SMBIOS table area.
     */
    uint64_t table_area_addr;

    /**
     * @brief Actual size of SMBIOS table area.
     */
    size_t table_area_size;

    /**
     * @brief Maximum size of SMBIOS table area.
     */
    size_t table_area_max_size;

    /**
     * @brief Pointer to SMBIOS table area data.
     */
    dmi_data_t *table_data;

    /**
     * @brief Maximum size of SMBIOS structure.
     */
    size_t entity_max_size;

    /**
     * @brief Logger callback.
     */
    dmi_log_handler_t logger;

    /**
     * @brief Logging level.
     */
    dmi_log_level_t log_level;

    /**
     * @brief Backend handle.
     */
    dmi_backend_t *backend;

    /**
     * @brief Backend-specific data.
     */
    void *session;

    /**
     * @brief Entity specifications map.
     */
    const dmi_entity_spec_t **type_map;

    /**
     * @brief Entity registry.
     */
    dmi_registry_t *registry;

    /**
     * @brief Error state.
     */
    dmi_error_state_t error_state;
};

__BEGIN_DECLS

/**
 * @brief Create DMI context.
 */
dmi_context_t *dmi_create(void);

/**
 * @brief Open DMI context.
 *
 * @param[in] context DMI context handle.
 * @return The function returns `true` on success and `false` otherwise.
 */
bool dmi_open(dmi_context_t *context);

/**
 * @brief Add DMI extension.
 */
bool dmi_add_extension(dmi_context_t *context, const dmi_extension_t *extension);

/**
 * @brief Load dump file into DMI context.
 *
 * @param[in] context DMI context handle.
 * @param[in] path Path to dump file.
 *
 * @return The function returns `true` on success and `false` otherwise.
 */
bool dmi_dump_load(dmi_context_t *context, const char *path);

/**
 * @brief Save DMI context to dump file.
 *
 * @param[in] context   DMI context handle.
 * @param[in] path      Path to dump file.
 * @param[in] overwrite Overwrite existing files flag
 *
 * @return The function returns `true` on success and `false` otherwise.
 */
bool dmi_dump_save(dmi_context_t *context, const char *path, bool overwrite);

/**
 * @brief Find entity type identifier by its code.
 */
dmi_type_t dmi_type_find(dmi_context_t *context, const char *code);

/**
 * @brief Get entity type specification.
 */
const dmi_entity_spec_t *dmi_type_spec(dmi_context_t *context, dmi_type_t type);

/**
 * @brief Get entity type name.
 */
const char *dmi_type_name(dmi_context_t *context, dmi_type_t type);

/**
 * @brief Set logging handler.
 *
 * @param[in] context DMI context handle.
 * @param[in] logger Logging handler.
 *
 * @return The function returns `true` on success and `false` otherwise.
 */
bool dmi_set_logger(dmi_context_t *context, dmi_log_handler_t logger);

/**
 * @brief Set logging level.
 *
 * @param[in] context DMI context handle.
 * @param[in] level Logging level.
 *
 * @return The function returns `true` on success and `false` otherwise.
 */
bool dmi_set_log_level(dmi_context_t *context, dmi_log_level_t level);

/**
 * @brief Close DMI context.
 *
 * @param[in] context DMI context handle.
 * @return The function returns `true` on success and `false` otherwise.
 */
bool dmi_close(dmi_context_t *context);

/**
 * @brief Destroy DMI context.
 *
 * @param[in] context DMI context handle.
 */
void dmi_destroy(dmi_context_t *context);

__END_DECLS

#endif // !OPENDMI_CONTEXT_H
