//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include "config.h"

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif // HAVE_UNISTD_H

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include <opendmi/context.h>
#include <opendmi/entry.h>
#include <opendmi/entity.h>
#include <opendmi/utils.h>

#include <opendmi/entity/additional-info.h>
#include <opendmi/entity/baseboard.h>
#include <opendmi/entity/battery.h>
#include <opendmi/entity/bis-entry-point.h>
#include <opendmi/entity/cache.h>
#include <opendmi/entity/chassis.h>
#include <opendmi/entity/cooling-device.h>
#include <opendmi/entity/current-probe.h>
#include <opendmi/entity/firmware.h>
#include <opendmi/entity/firmware-inventory.h>
#include <opendmi/entity/firmware-language.h>
#include <opendmi/entity/group-assoc.h>
#include <opendmi/entity/hardware-security.h>
#include <opendmi/entity/ipmi-device.h>
#include <opendmi/entity/memory-array.h>
#include <opendmi/entity/memory-array-addr.h>
#include <opendmi/entity/memory-channel.h>
#include <opendmi/entity/memory-controller.h>
#include <opendmi/entity/memory-device.h>
#include <opendmi/entity/memory-device-addr.h>
#include <opendmi/entity/memory-error-32.h>
#include <opendmi/entity/memory-error-64.h>
#include <opendmi/entity/memory-module.h>
#include <opendmi/entity/mgmt-controller.h>
#include <opendmi/entity/mgmt-device.h>
#include <opendmi/entity/mgmt-device-component.h>
#include <opendmi/entity/mgmt-device-threshold.h>
#include <opendmi/entity/oem-strings.h>
#include <opendmi/entity/onboard-device.h>
#include <opendmi/entity/onboard-device-ex.h>
#include <opendmi/entity/oob-remote-access.h>
#include <opendmi/entity/pointing-device.h>
#include <opendmi/entity/port-connector.h>
#include <opendmi/entity/power-controls.h>
#include <opendmi/entity/power-supply.h>
#include <opendmi/entity/probe.h>
#include <opendmi/entity/processor.h>
#include <opendmi/entity/processor-ex.h>
#include <opendmi/entity/slot.h>
#include <opendmi/entity/string-property.h>
#include <opendmi/entity/system.h>
#include <opendmi/entity/system-boot.h>
#include <opendmi/entity/system-config.h>
#include <opendmi/entity/system-event-log.h>
#include <opendmi/entity/system-reset.h>
#include <opendmi/entity/system-config.h>
#include <opendmi/entity/temperature-probe.h>
#include <opendmi/entity/tpm-device.h>
#include <opendmi/entity/voltage-probe.h>

#include <opendmi/backend/dump.h>
#if defined(__linux__)
#   include <opendmi/backend/linux.h>
#   define DMI_BACKEND dmi_linux_backend
#elif defined(__APPLE__)
#   include <opendmi/backend/darwin.h>
#   define DMI_BACKEND dmi_darwin_backend
#elif defined(__FreeBSD__)
#   include <opendmi/backend/freebsd.h>
#   define DMI_BACKEND dmi_freebsd_backend
#elif defined(_WIN32)
#   include <opendmi/backend/windows.h>
#   define DMI_BACKEND dmi_windows_backend
#else
#   error "Unsupported OS type"
#endif

static bool dmi_open_ex(dmi_context_t *context, dmi_backend_t *backend, const void *arg);

/**
 * @internal
 * @brief Fixup DMI version number.
 */
static void dmi_version_fixup(dmi_context_t *context);

/**
 * @brief Backend handle.
 */
static dmi_backend_t *dmi_backend = &DMI_BACKEND;

static const dmi_entity_spec_t dmi_inactive_spec =
{
    .code = "inactive",
    .name = "Inactive",
    .type = DMI_TYPE_INACTIVE
};

static const dmi_entity_spec_t dmi_end_of_table_spec =
{
    .code = "end-of-table",
    .name = "End of table",
    .type = DMI_TYPE_END_OF_TABLE
};

/**
 * @brief Predefined entity specifications map.
 */
static const dmi_entity_spec_t *dmi_entity_specs[] =
{
    [DMI_TYPE_FIRMWARE]                = &dmi_firmware_spec,
    [DMI_TYPE_SYSTEM]                  = &dmi_system_spec,
    [DMI_TYPE_BASEBOARD]               = &dmi_baseboard_spec,
    [DMI_TYPE_CHASSIS]                 = &dmi_chassis_spec,
    [DMI_TYPE_PROCESSOR]               = &dmi_processor_spec,
    [DMI_TYPE_MEMORY_CONTROLLER]       = &dmi_memory_controller_spec,
    [DMI_TYPE_MEMORY_MODULE]           = &dmi_memory_module_spec,
    [DMI_TYPE_CACHE]                   = &dmi_cache_spec,
    [DMI_TYPE_PORT_CONNECTOR]          = &dmi_port_connector_spec,
    [DMI_TYPE_SYSTEM_SLOTS]            = &dmi_slot_spec,
    [DMI_TYPE_ONBOARD_DEVICE]          = &dmi_onboard_device_spec,
    [DMI_TYPE_OEM_STRINGS]             = &dmi_oem_strings_spec,
    [DMI_TYPE_SYSTEM_CONFIG_OPTIONS]   = &dmi_system_config_opts_spec,
    [DMI_TYPE_FIRMWARE_LANGUAGE]       = &dmi_firmware_language_spec,
    [DMI_TYPE_GROUP_ASSOC]             = &dmi_group_assoc_spec,
    [DMI_TYPE_SYSTEM_EVENT_LOG]        = &dmi_system_event_log_spec,
    [DMI_TYPE_MEMORY_ARRAY]            = &dmi_memory_array_spec,
    [DMI_TYPE_MEMORY_DEVICE]           = &dmi_memory_device_spec,
    [DMI_TYPE_MEMORY_ERROR_32]         = &dmi_memory_error_32_spec,
    [DMI_TYPE_MEMORY_ARRAY_ADDR]       = &dmi_memory_array_addr_spec,
    [DMI_TYPE_MEMORY_DEVICE_ADDR]      = &dmi_memory_device_addr_spec,
    [DMI_TYPE_POINTING_DEVICE]         = &dmi_pointing_device_spec,
    [DMI_TYPE_PORTABLE_BATTERY]        = &dmi_battery_spec,
    [DMI_TYPE_SYSTEM_RESET]            = &dmi_system_reset_spec,
    [DMI_TYPE_HARDWARE_SECURITY]       = &dmi_hardware_security_spec,
    [DMI_TYPE_POWER_CONTROLS]          = &dmi_power_controls_spec,
    [DMI_TYPE_VOLTAGE_PROBE]           = &dmi_voltage_probe_spec,
    [DMI_TYPE_COOLING_DEVICE]          = &dmi_cooling_device_spec,
    [DMI_TYPE_TEMPERATURE_PROBE]       = &dmi_temperature_probe_spec,
    [DMI_TYPE_CURRENT_PROBE]           = &dmi_current_probe_spec,
    [DMI_TYPE_OOB_REMOTE_ACCESS]       = &dmi_oob_remote_access_spec,
    [DMI_TYPE_BIS_ENTRY_POINT]         = &dmi_bis_entry_point_spec,
    [DMI_TYPE_SYSTEM_BOOT]             = &dmi_system_boot_spec,
    [DMI_TYPE_MEMORY_ERROR_64]         = &dmi_memory_error_64_spec,
    [DMI_TYPE_MGMT_DEVICE]             = &dmi_mgmt_device_spec,
    [DMI_TYPE_MGMT_DEVICE_COMPONENT]   = &dmi_mgmt_device_component_spec,
    [DMI_TYPE_MGMT_DEVICE_THRESHOLD]   = &dmi_mgmt_device_threshold_spec,
    [DMI_TYPE_MEMORY_CHANNEL]          = &dmi_memory_channel_spec,
    [DMI_TYPE_IPMI_DEVICE]             = &dmi_ipmi_device_spec,
    [DMI_TYPE_POWER_SUPPLY]            = &dmi_power_supply_spec,
    [DMI_TYPE_ADDITIONAL_INFO]         = &dmi_additional_info_spec,
    [DMI_TYPE_ONBOARD_DEVICE_EX]       = &dmi_onboard_device_ex_spec,
    [DMI_TYPE_MGMT_CONTROLLER_HOST_IF] = &dmi_mgmt_controller_host_if_spec,
    [DMI_TYPE_TPM_DEVICE]              = &dmi_tpm_device_spec,
    [DMI_TYPE_PROCESSOR_EX]            = &dmi_processor_ex_spec,
    [DMI_TYPE_FIRMWARE_INVENTORY]      = &dmi_firmware_inventory_spec,
    [DMI_TYPE_STRING_PROPERTY]         = &dmi_string_property_spec,
    [DMI_TYPE_INACTIVE]                = &dmi_inactive_spec,
    [DMI_TYPE_END_OF_TABLE]            = &dmi_end_of_table_spec
};

dmi_context_t *dmi_create(void)
{
    dmi_context_t *context = nullptr;

    // Allocate context descriptor
    context = dmi_alloc(nullptr, sizeof(dmi_context_t));
    if (context == nullptr)
        return nullptr;

    context->log_level = DMI_LOG_INFO;

    // Allocate type map
    context->type_map = dmi_alloc_array(context, sizeof(dmi_entity_spec_t *), 0x100);
    if (context->type_map == nullptr) {
        dmi_free(context);
        return nullptr;
    }

    return context;
}

bool dmi_open(dmi_context_t *context)
{
    if (context == nullptr)
        return false;

    return dmi_open_ex(context, dmi_backend, nullptr);
}

bool dmi_dump_load(dmi_context_t *context, const char *path)
{
    if (context == nullptr)
        return false;

    if (path == nullptr) {
        dmi_error_raise_ex(context, DMI_ERROR_NULL_ARGUMENT, "path");
        return false;
    }

    dmi_log_info(context, "Loading DMI dump: %s...", path);

    return dmi_open_ex(context, &dmi_dump_backend, path);
}

bool dmi_dump_save(dmi_context_t *context, const char *path, bool overwrite)
{
    int flags;
    int fd;
    bool success;
    ssize_t nwrite;

    if (context == nullptr)
        return false;

    if (path == nullptr) {
        dmi_error_raise_ex(context, DMI_ERROR_NULL_ARGUMENT, "path");
        return false;
    }
    if (context->entry_size > DMI_ENTRY_MAX_SIZE) {
        dmi_error_raise(context, DMI_ERROR_INVALID_EPS_LENGTH);
        return false;
    }

    flags = O_CREAT | O_WRONLY | O_TRUNC;
    if (not overwrite)
        flags |= O_EXCL;

    fd = open(path, flags, 0666);
    if (fd < 0) {
        dmi_error_raise_ex(context, DMI_ERROR_FILE_OPEN, "%s: %s", path, strerror(errno));
        return false;
    }

    success = false;
    do {
        dmi_byte_t entry[DMI_ENTRY_MAX_SIZE] = { 0 };
        memcpy(entry, context->entry_data, context->entry_size);

    write_entry:
        nwrite = write(fd, entry, sizeof(entry));
        if (nwrite < 0) {
            if (errno == EINTR)
                goto write_entry;

            dmi_error_raise_ex(context, DMI_ERROR_FILE_WRITE, "%s: %s", path, strerror(errno));
            break;
        }

    write_table:
        nwrite = write(fd, context->table_data, context->table_area_size);
        if (nwrite < 0) {
            if (errno == EINTR)
                goto write_table;

            dmi_error_raise_ex(context, DMI_ERROR_FILE_WRITE, "%s: %s", path, strerror(errno));
            break;
        }

        success = true;
    } while (false);

    close(fd);

    return success;
}

const dmi_entity_spec_t *dmi_type_spec(dmi_context_t *context, dmi_type_t type)
{
    const dmi_entity_spec_t *spec = nullptr;

    if ((type <= DMI_TYPE_INVALID) or (type > UINT8_MAX)) {
        dmi_error_raise_ex(context, DMI_ERROR_INVALID_ARGUMENT, "type");
        return nullptr;
    }

    if (context != nullptr)
        spec = context->type_map[type];

    if (spec == nullptr) {
        if ((size_t)type < countof(dmi_entity_specs))
            spec = dmi_entity_specs[type];
    }

    return spec;
}

const char *dmi_type_name(dmi_context_t *context, dmi_type_t type)
{
    const char *name;
    const dmi_entity_spec_t *spec = dmi_type_spec(context, type);

    if (spec != nullptr)
        name = spec->name;
    else
        name = type > 0x7F ? "OEM-specific" : "Unknown";

    return name;
}

bool dmi_set_logger(dmi_context_t *context, dmi_log_handler_t logger)
{
    if (context == nullptr)
        return false;

    context->logger = logger;

    return true;
}

bool dmi_set_log_level(dmi_context_t *context, dmi_log_level_t level)
{
    if (context == nullptr)
        return false;

    context->log_level = level;

    return true;
}

bool dmi_close(dmi_context_t *context)
{
    if (context == nullptr)
        return false;

    dmi_registry_destroy(context->registry);

    if (context->backend) {
        if (context->session != nullptr)
            context->backend->close(context);
    }

    memset(context, 0, sizeof(*context));

    return true;
}

void dmi_destroy(dmi_context_t *context)
{
    if (context == nullptr)
        return;

    // Close and free context
    dmi_close(context);

    dmi_free(context->type_map);
    dmi_free(context);
}

static bool dmi_open_ex(dmi_context_t *context, dmi_backend_t *backend, const void *arg)
{
    assert(context != nullptr);

    if ((context->backend != nullptr) or (context->session != nullptr)) {
        dmi_error_raise_ex(context,  DMI_ERROR_INVALID_STATE, "Context already initialized");
        return false;
    }

    dmi_log_info(context, "Opening DMI context...");
    dmi_log_info(context, "Using backend: %s", backend->name);

    // Initialize context
    bool success = false;
    do {
        context->backend = backend;

        // Initialize backend
        if (not context->backend->open(context, arg)) {
            dmi_log_error(context, "Unable to open backend: %s", backend->name);
            break;
        }

        // Read and decode entry point
        dmi_log_info(context, "Reading DMI entry point...");
        context->entry_data = context->backend->read_entry(context, &context->entry_size);
        if (context->entry_data == nullptr)
            break;
        if (not dmi_entry_decode(context, context->entry_data, context->entry_size))
            break;

        // Fixup SMBIOS version number
        dmi_version_fixup(context);
        dmi_log_info(context, "SMBIOS %u.%u.%u present",
                     dmi_version_major(context->smbios_version),
                     dmi_version_minor(context->smbios_version),
                     dmi_version_revision(context->smbios_version));

        // Read and decode SMBIOS structures
        // TODO: Use separate variable for size
        dmi_log_info(context, "Reading DMI structures...");
        context->table_data = context->backend->read_table(context, &context->table_area_size);
        if (context->table_data == nullptr)
            break;

        // Create registry
        context->registry = dmi_registry_create(context, 0);
        if (context->registry == nullptr)
            break;

        // Build and link registry
        if (not dmi_registry_build(context->registry))
            break;
        if (not dmi_registry_link(context->registry))
            break;

        success = true;
    } while (false);

    if (not success) {
        dmi_log_error(context, "Unable to open DMI context");
        dmi_close(context);
    }

    return success;
}

static void dmi_version_fixup(dmi_context_t *context)
{
    unsigned int major    = dmi_version_major(context->smbios_version);
    unsigned int minor    = dmi_version_minor(context->smbios_version);
    unsigned int revision = dmi_version_revision(context->smbios_version);

    if (major != 2)
        return;

    // Some BIOS report weird SMBIOS version, fix that up
    switch (minor) {
    case 0x1F:
    case 0x21:
        minor = 3;
        break;

    case 0x33:
        minor = 6;
        break;

    default:
        return;
    }

    context->smbios_version = dmi_version(major, minor, revision);
}
