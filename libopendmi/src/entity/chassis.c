//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/name.h>
#include <opendmi/value.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/chassis.h>

static const dmi_name_set_t dmi_chassis_type_names =
{
    .code  = "chassis-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_CHASSIS_TYPE_UNSPEC),
        DMI_NAME_OTHER(DMI_CHASSIS_TYPE_OTHER),
        DMI_NAME_UNKNOWN(DMI_CHASSIS_TYPE_UNKNOWN),
        {
            .id   = DMI_CHASSIS_TYPE_DESKTOP,
            .code = "desktop",
            .name = "Desktop"
        },
        {
            .id   = DMI_CHASSIS_TYPE_LOW_PROFILE_DESKTOP,
            .code = "low-profile-desktop",
            .name = "Low-profile desktop"
        },
        {
            .id   = DMI_CHASSIS_TYPE_PIZZA_BOX,
            .code = "pizza-box",
            .name = "Pizza box"
        },
        {
            .id   = DMI_CHASSIS_TYPE_MINI_TOWER,
            .code = "mini-tower",
            .name = "Mini tower"
        },
        {
            .id   = DMI_CHASSIS_TYPE_TOWER,
            .code = "tower",
            .name = "Tower"
        },
        {
            .id   = DMI_CHASSIS_TYPE_PORTABLE,
            .code = "portable",
            .name = "Portable"
        },
        {
            .id   = DMI_CHASSIS_TYPE_LAPTOP,
            .code = "laptop",
            .name = "Laptop"
        },
        {
            .id   = DMI_CHASSIS_TYPE_NOTEBOOK,
            .code = "notebook",
            .name = "Notebook"
        },
        {
            .id   = DMI_CHASSIS_TYPE_HAND_HELD,
            .code = "hand-held",
            .name = "Hand held"
        },
        {
            .id   = DMI_CHASSIS_TYPE_DOCKING_STATION,
            .code = "docking-station",
            .name = "Docking station"
        },
        {
            .id   = DMI_CHASSIS_TYPE_ALL_IN_ONE,
            .code = "all-in-one",
            .name = "All-in-one"
        },
        {
            .id   = DMI_CHASSIS_TYPE_SUB_NOTEBOOK,
            .code = "sub-notebook",
            .name = "Sub-notebook"
        },
        {
            .id   = DMI_CHASSIS_TYPE_SPACE_SAVING,
            .code = "space-saving",
            .name = "Space-saving"
        },
        {
            .id   = DMI_CHASSIS_TYPE_LUNCH_BOX,
            .code = "lunch-box",
            .name = "Lunch box"
        },
        {
            .id   = DMI_CHASSIS_TYPE_MAIN_SERVER,
            .code = "main-server",
            .name = "Main server chassis"
        },
        {
            .id   = DMI_CHASSIS_TYPE_EXPANSION,
            .code = "expansion",
            .name = "Expansion chassis"
        },
        {
            .id   = DMI_CHASSIS_TYPE_SUB_CHASSIS,
            .code = "sub-chassis",
            .name = "Sub-chassis"
        },
        {
            .id   = DMI_CHASSIS_TYPE_BUS_EXPANSION,
            .code = "bus-expansion",
            .name = "Bus expansion chassis"
        },
        {
            .id   = DMI_CHASSIS_TYPE_PERIPHERAL,
            .code = "peripheral",
            .name = "Peripheral chassis"
        },
        {
            .id   = DMI_CHASSIS_TYPE_RAID,
            .code = "raid",
            .name = "RAID chassis"
        },
        {
            .id   = DMI_CHASSIS_TYPE_RACK_MOUNT,
            .code = "rack-mount",
            .name = "Rack-mount chassis"
        },
        {
            .id   = DMI_CHASSIS_TYPE_SEALED_CASE_PC,
            .code = "sealed-case-pc",
            .name = "Sealed-case PC"
        },
        {
            .id   = DMI_CHASSIS_TYPE_MULTI_SYSTEM,
            .code = "multy-system",
            .name = "Multi-system chassis"
        },
        {
            .id   = DMI_CHASSIS_TYPE_COMPACT_PCI,
            .code = "compact-pci",
            .name = "Compact PCI"
        },
        {
            .id   = DMI_CHASSIS_TYPE_ADVANCED_TCA,
            .code = "advanced-tca",
            .name = "Advanced TCA"
        },
        {
            .id   = DMI_CHASSIS_TYPE_BLADE,
            .code = "blade",
            .name = "Blade"
        },
        {
            .id   = DMI_CHASSIS_TYPE_BLADE_ENCLOSURE,
            .code = "blade-enclosure",
            .name = "Blade enclosure"
        },
        {
            .id   = DMI_CHASSIS_TYPE_TABLET,
            .code = "tablet",
            .name = "Tablet"
        },
        {
            .id   = DMI_CHASSIS_TYPE_CONVERTIBLE,
            .code = "convertible",
            .name = "Convertible"
        },
        {
            .id   = DMI_CHASSIS_TYPE_DETACHABLE,
            .code = "detachable",
            .name = "Detachable"
        },
        {
            .id   = DMI_CHASSIS_TYPE_IOT_GATEWAY,
            .code = "iot-gateway",
            .name = "IoT gateway"
        },
        {
            .id   = DMI_CHASSIS_TYPE_EMBEDDED_PC,
            .code = "embedded-pc",
            .name = "Embedded PC"
        },
        {
            .id   = DMI_CHASSIS_TYPE_MINI_PC,
            .code = "mini-pc",
            .name = "Mini PC"
        },
        {
            .id   = DMI_CHASSIS_TYPE_STICK_PC,
            .code = "stick-pc",
            .name = "Stick PC"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_chassis_security_status_names =
{
    .code  = "chassis-security-statuses",
    .names = {
        DMI_NAME_UNSPEC(DMI_CHASSIS_SECURITY_STATUS_UNSPEC),
        DMI_NAME_OTHER(DMI_CHASSIS_SECURITY_STATUS_OTHER),
        DMI_NAME_UNKNOWN(DMI_CHASSIS_SECURITY_STATUS_UNKNOWN),
        DMI_NAME_NONE(DMI_CHASSIS_SECURITY_STATUS_NONE),
        {
            .id   = DMI_CHASSIS_SECURITY_STATUS_EXT_IF_LOCKED,
            .code = "ext-if-locked",
            .name = "External interface locked"
        },
        {
            .id   = DMI_CHASSIS_SECURITY_STATUS_EXT_IF_ENABLED,
            .code = "ext-if-enabled",
            .name = "External interface enabled"
        },
        DMI_NAME_NULL
    }
};

static const dmi_name_set_t dmi_rack_type_names =
{
    .code  = "rack-types",
    .names = {
        DMI_NAME_UNSPEC(DMI_RACK_TYPE_UNSPEC),
        {
            .id   = DMI_RACK_TYPE_OPEN,
            .code = "open-rack",
            .name = "Open Rack"
        },
        DMI_NAME_NULL
    }
};

static const dmi_attribute_t dmi_chassis_element_attrs[] =
{
    // TODO: Add support for type attribute
    DMI_ATTRIBUTE(dmi_chassis_element_t, board_type, ENUM, {
        .code   = "board-type",
        .name   = "Board type",
        .values = &dmi_baseboard_type_names,
    }),
    DMI_ATTRIBUTE(dmi_chassis_element_t, minimum_count, INTEGER, {
        .code = "minimum-count",
        .name = "Minimum count"
    }),
    DMI_ATTRIBUTE(dmi_chassis_element_t, maximum_count, INTEGER, {
        .code = "maximum-count",
        .name = "Maximum count"
    }),
    DMI_ATTRIBUTE_NULL
};

static const dmi_attribute_t dmi_chassis_attrs[] =
{
    DMI_ATTRIBUTE(dmi_chassis_t, vendor, STRING, {
        .code    = "vendor",
        .name    = "Vendor"
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, type, ENUM, {
        .code    = "type",
        .name    = "Type",
        .unspec  = dmi_value_ptr(DMI_CHASSIS_TYPE_UNSPEC),
        .unknown = dmi_value_ptr(DMI_CHASSIS_TYPE_UNKNOWN),
        .values  = &dmi_chassis_type_names
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, is_lock_present, BOOL, {
        .code    = "is-lock-present",
        .name    = "Lock present"
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, version, STRING, {
        .code    = "version",
        .name    = "Version"
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, serial_number, STRING, {
        .code    = "serial-number",
        .name    = "Serial number"
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, asset_tag, STRING, {
        .code    = "asset-tag",
        .name    = "Asset tag"
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, bootup_state, ENUM, {
        .code    = "bootup-state",
        .name    = "Boot-up state",
        .unspec  = dmi_value_ptr(DMI_STATUS_UNSPEC),
        .unknown = dmi_value_ptr(DMI_STATUS_UNKNOWN),
        .values  = &dmi_status_names,
        .level   = DMI_VERSION(2, 1, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, power_supply_state, ENUM, {
        .code    = "power-supply-state",
        .name    = "Power supply state",
        .unspec  = dmi_value_ptr(DMI_STATUS_UNSPEC),
        .unknown = dmi_value_ptr(DMI_STATUS_UNKNOWN),
        .values  = &dmi_status_names,
        .level   = DMI_VERSION(2, 1, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, thermal_state, ENUM, {
        .code    = "thermal-state",
        .name    = "Thermal state",
        .unspec  = dmi_value_ptr(DMI_STATUS_UNSPEC),
        .unknown = dmi_value_ptr(DMI_STATUS_UNKNOWN),
        .values  = &dmi_status_names,
        .level   = DMI_VERSION(2, 1, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, security_status, ENUM, {
        .code    = "security-status",
        .name    = "Security status",
        .unspec  = dmi_value_ptr(DMI_CHASSIS_SECURITY_STATUS_UNSPEC),
        .unknown = dmi_value_ptr(DMI_CHASSIS_SECURITY_STATUS_UNKNOWN),
        .values  = &dmi_chassis_security_status_names,
        .level   = DMI_VERSION(2, 1, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, oem_defined, INTEGER, {
        .code    = "oem-defined",
        .name    = "OEM-defined",
        .flags   = DMI_ATTRIBUTE_FLAG_HEX,
        .level   = DMI_VERSION(2, 3, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, height, INTEGER, {
        .code    = "height",
        .name    = "Height",
        .unit    = DMI_UNIT_RACK,
        .unspec  = dmi_value_ptr((unsigned short)0),
        .level   = DMI_VERSION(2, 3, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, power_cord_count, INTEGER, {
        .code    = "power-cord-count",
        .name    = "Power cord count",
        .unspec  = dmi_value_ptr((unsigned short)0),
        .level   = DMI_VERSION(2, 3, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, element_count, INTEGER, {
        .code    = "element-count",
        .name    = "Contained element count",
        .level   = DMI_VERSION(2, 3, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, element_size, SIZE, {
        .code    = "element-size",
        .name    = "Contained element size",
        .level   = DMI_VERSION(2, 3, 0)
    }),
    DMI_ATTRIBUTE_ARRAY(dmi_chassis_t, elements, element_count, STRUCT, {
        .code    = "elements",
        .name    = "Contained elements",
        .attrs   = dmi_chassis_element_attrs,
        .level   = DMI_VERSION(2, 3, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, sku_number, STRING, {
        .code    = "sku-number",
        .name    = "SKU number",
        .level   = DMI_VERSION(2, 7, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, rack_type, ENUM, {
        .code    = "rack-type",
        .name    = "Rack type",
        .unspec  = dmi_value_ptr(DMI_RACK_TYPE_UNSPEC),
        .values  = &dmi_rack_type_names,
        .level   = DMI_VERSION(3, 9, 0)
    }),
    DMI_ATTRIBUTE(dmi_chassis_t, rack_height, INTEGER, {
        .code    = "rack-height",
        .name    = "Rack height",
        .level   = DMI_VERSION(3, 9, 0)
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_chassis_spec =
{
    .code          = "chassis",
    .name          = "System enclosure or chassis",
    .type          = DMI_TYPE_CHASSIS,
    .required_from = DMI_VERSION(2, 3, 0),
    .required_till = DMI_VERSION_NONE,
    .unique        = false,
    .min_length    = 0x09,
    .attributes    = dmi_chassis_attrs,
    .handlers      = {
        .decode = (dmi_entity_decode_fn_t)dmi_chassis_decode,
        .free   = (dmi_entity_free_fn_t)dmi_chassis_free
    }
};

const char *dmi_chassis_type_name(dmi_chassis_type_t value)
{
    return dmi_name_lookup(&dmi_chassis_type_names, value);
}

const char *dmi_chassis_security_status_name(dmi_chassis_security_status_t value)
{
     return dmi_name_lookup(&dmi_chassis_security_status_names, value);
}

dmi_chassis_t *dmi_chassis_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_chassis_t *info;
    dmi_version_t level = dmi_version(2, 0, 0);
    const dmi_chassis_data_t *data;
    const dmi_data_t *element_ptr;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_CHASSIS));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    dmi_chassis_type_data_t type = {
        .__value = dmi_decode(data->type)
    };

    info->vendor          = dmi_entity_string(entity,  data->vendor);
    info->type            = type.type;
    info->is_lock_present = type.is_lock_present;
    info->version         = dmi_entity_string(entity, data->version);
    info->serial_number   = dmi_entity_string(entity, data->serial_number);
    info->asset_tag       = dmi_entity_string(entity, data->asset_tag);

    //
    // SMBIOS 2.1 features
    //

    if (entity->body_length > 0x09) {
        level = dmi_version(2, 1, 0);
        info->bootup_state = dmi_decode(data->bootup_state);
    }
    if (entity->body_length > 0x0A)
        info->power_supply_state = dmi_decode(data->power_supply_state);
    if (entity->body_length > 0x0B)
        info->thermal_state = dmi_decode(data->thermal_state);
    if (entity->body_length > 0x0C)
        info->security_status = dmi_decode(data->security_status);

    //
    // SMBIOS 2.3 features
    //

    if (entity->body_length > 0x0D) {
        level = dmi_version(2, 3, 0);
        info->oem_defined = dmi_decode(data->oem_defined);
    }

    if (entity->body_length > 0x11)
        info->height = dmi_decode(data->height);
    if (entity->body_length > 0x12)
        info->power_cord_count = dmi_decode(data->power_cord_count);

    if (entity->body_length > 0x13) {
        info->element_count = dmi_decode(data->element_count);
        info->element_size  = dmi_decode(data->element_size);

        info->elements = dmi_alloc_array(entity->context, sizeof(dmi_chassis_element_t), info->element_count);
        if (info->elements == nullptr) {
            dmi_free(info);
            return nullptr;
        }

        element_ptr = entity->data + sizeof(dmi_chassis_data_t);
        for (size_t i = 0; i < info->element_count; i++) {
            dmi_chassis_element_t *element = &info->elements[i];
            const dmi_chassis_element_data_t *element_data = dmi_cast(element_data, element_ptr);

            uint8_t element_type = dmi_decode(element_data->type);
            if (element_type & 0x80u) {
                element->type = element_type & 0x7Fu;
            } else {
                element->type       = DMI_TYPE_INVALID;
                element->board_type = element_type;
            }

            element->minimum_count = dmi_decode(element_data->minimum_count);
            element->maximum_count = dmi_decode(element_data->maximum_count);

            element_ptr += info->element_size;
        }

        size_t base_len = (size_t)(element_ptr - entity->data);

        //
        // SMBIOS 2.7 features
        //
    
        if (entity->body_length > base_len) {
            level = dmi_version(2, 7, 0);

            size_t extra_len = entity->body_length - base_len;
            dmi_chassis_extra_t *extra = dmi_cast(extra, element_ptr);

            info->sku_number = dmi_entity_string(entity, extra->sku_number);

            //
            // SMBIOS 3.9 features
            //

            if (extra_len > 0x01) {
                level = dmi_version(3, 9, 0);
                info->rack_type   = dmi_decode(extra->rack_type);
            }
            if (extra_len > 0x02)
                info->rack_height = dmi_decode(extra->rack_height);
        }
    }

    if (plevel != nullptr)
        *plevel = level;

    return info;
}

void dmi_chassis_free(dmi_chassis_t *info)
{
    dmi_free(info->elements);
    dmi_free(info);
}
