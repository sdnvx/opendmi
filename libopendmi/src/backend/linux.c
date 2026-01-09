//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef __linux__
#   error "Unsupported OS type"
#endif // !__linux__

#include <sys/mman.h>

#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include <opendmi/context.h>
#include <opendmi/utils.h>

#include <opendmi/backend/linux.h>

typedef struct dmi_linux_session dmi_linux_session_t;

typedef struct dmi_linux_session
{
    dmi_data_t *entry;
    size_t entry_size;
    dmi_data_t *table;
    size_t table_size;
} dmi_linux_session_t;

static bool dmi_linux_open(dmi_context_t *context, const void *arg __attribute__((unused)));
static dmi_data_t *dmi_linux_read_entry(dmi_context_t *context, size_t *plength);
static dmi_data_t *dmi_linux_read_table(dmi_context_t *context, size_t *plength);
static bool dmi_linux_close(dmi_context_t *context);
static void dmi_linux_session_free(dmi_linux_session_t *session);

/**
 * @internal
 * @brief SMBIOS data directory path (SYSFS).
 */
static const char *dmi_linux_sysfs_path = "/sys/firmware/dmi/tables";

/**
 * @internal
 * @brief SMBIOS entry point file name (SYSFS).
 */
static const char *dmi_linux_entry_file = "smbios_entry_point";

/**
 * @brief SMBIOS table file name (SYSFS).
 */
static const char *dmi_linux_table_file = "DMI";

dmi_backend_t dmi_linux_backend =
{
    .name       = "Linux SysFS",
    .open       = dmi_linux_open,
    .read_entry = dmi_linux_read_entry,
    .read_table = dmi_linux_read_table,
    .close      = dmi_linux_close
};

static bool dmi_linux_open(dmi_context_t *context, const void *arg __attribute__((unused)))
{
    bool rv = false;
    dmi_linux_session_t *session = nullptr;

    assert(context != nullptr);
    assert(context->session == nullptr);

    session = dmi_alloc(context, sizeof(*session));
    if (session == nullptr)
        return false;

    do {
        char path[PATH_MAX];

        snprintf(path, sizeof(path), "%s/%s", dmi_linux_sysfs_path, dmi_linux_entry_file);
        session->entry = dmi_file_read(context, path, &session->entry_size);
        if (session->entry == nullptr)
            break;

        snprintf(path, sizeof(path), "%s/%s", dmi_linux_sysfs_path, dmi_linux_table_file);
        session->table = dmi_file_read(context, path, &session->table_size);
        if (session->table == nullptr)
            break;

        context->session = session;
        rv = true;
    } while (false);

    if (not rv)
        dmi_linux_session_free(session);

    return rv;
}

static dmi_data_t *dmi_linux_read_entry(dmi_context_t *context, size_t *plength)
{
    assert(context != nullptr);
    assert(context->session != nullptr);
    assert(plength != nullptr);

    dmi_linux_session_t *session = dmi_cast(session, context->session);

    *plength = session->entry_size;

    return session->entry;
}

static dmi_data_t *dmi_linux_read_table(dmi_context_t *context, size_t *plength)
{
    assert(context != nullptr);
    assert(context->session != nullptr);
    assert(plength != nullptr);

    dmi_linux_session_t *session = dmi_cast(session, context->session);

    *plength = session->table_size;

    return session->table;
}

static bool dmi_linux_close(dmi_context_t *context)
{
    assert(context != nullptr);
    assert(context->session != nullptr);

    dmi_linux_session_free(context->session);

    context->session = nullptr;
    context->backend = nullptr;

    return false;
}

static void dmi_linux_session_free(dmi_linux_session_t *session)
{
    if (session == nullptr)
        return;

    dmi_free(session->entry);
    dmi_free(session->table);

    dmi_free(session);
}
