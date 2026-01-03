//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include <opendmi/version.h>
#include <opendmi/date.h>
#include <opendmi/attribute.h>
#include <opendmi/utils.h>

static char *dmi_attribute_format_handle(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_string(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_bool(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_integer(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_decimal(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_size(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_address(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_enum(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_set(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_version(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_date(const dmi_attribute_t *attr, const void *value, bool pretty);
static char *dmi_attribute_format_uuid(const dmi_attribute_t *attr, const void *value, bool pretty);

static const dmi_attribute_ops_t dmi_attribute_type_ops[] =
{
    [DMI_ATTRIBUTE_TYPE_HANDLE] = {
        .format = dmi_attribute_format_handle,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_STRING] = {
        .format = dmi_attribute_format_string,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_BOOL] = {
        .format = dmi_attribute_format_bool,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_INTEGER] = {
        .format = dmi_attribute_format_integer,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_DECIMAL] = {
        .format = dmi_attribute_format_decimal,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_SIZE] = {
        .format = dmi_attribute_format_size,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_ADDRESS] = {
        .format = dmi_attribute_format_address,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_ENUM] = {
        .format = dmi_attribute_format_enum,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_SET] = {
        .format = dmi_attribute_format_set,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_VERSION] = {
        .format = dmi_attribute_format_version,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_DATE] = {
        .format = dmi_attribute_format_date,
        .parse  = nullptr
    },
    [DMI_ATTRIBUTE_TYPE_UUID] = {
        .format = dmi_attribute_format_uuid,
        .parse  = nullptr
    }
};

bool dmi_attribute_is_unspecified(const dmi_attribute_t *attr, const void *value)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    if (attr->params.unspec) {
        if (memcmp(value, attr->params.unspec, attr->value.size) == 0)
            return true;
    } else if (attr->type == DMI_ATTRIBUTE_TYPE_STRING) {
        if (*(const char **)value == nullptr)
            return true;
    } else if (attr->type == DMI_ATTRIBUTE_TYPE_HANDLE) {
        if (*(dmi_handle_t *)value == DMI_HANDLE_INVALID)
            return true;
    }

    return false;
}

bool dmi_attribute_is_unknown(const dmi_attribute_t *attr, const void *value)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    if (attr->params.unknown) {
        if (memcmp(value, attr->params.unknown, attr->value.size) == 0)
            return true;
    }

    return false;
}

intmax_t dmi_attribute_get_int(const dmi_attribute_t *attr, const void *value)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    intmax_t rv;

    if (attr->value.size == sizeof(int8_t))
        rv = *(int8_t *)value;
    else if (attr->value.size == sizeof(int16_t))
        rv = *(int16_t *)value;
    else if (attr->value.size == sizeof(int32_t))
        rv = *(int32_t *)value;
    else if (attr->value.size == sizeof(int64_t))
        rv = *(int64_t *)value;
    else
        rv = INTMAX_MAX;

    return rv;
}

uintmax_t dmi_attribute_get_uint(const dmi_attribute_t *attr, const void *value)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    uintmax_t rv;

    if (attr->value.size == sizeof(uint8_t))
        rv = *(uint8_t *)value;
    else if (attr->value.size == sizeof(uint16_t))
        rv = *(uint16_t *)value;
    else if (attr->value.size == sizeof(uint32_t))
        rv = *(uint32_t *)value;
    else if (attr->value.size == sizeof(uint64_t))
        rv = *(uint64_t *)value;
    else
        rv = UINTMAX_MAX;

    return rv;
}

char *dmi_attribute_format(const dmi_attribute_t *attr, const void *value, bool pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    const dmi_attribute_ops_t *ops;

    if (attr->type >= countof(dmi_attribute_type_ops))
        return nullptr;

    ops = &dmi_attribute_type_ops[attr->type];
    if (ops->format == nullptr)
        return nullptr;

    return ops->format(attr, value, pretty);
}

static char *dmi_attribute_format_handle(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    dmi_unused(attr);
    dmi_unused(pretty);

    char *str = nullptr;

    if (dmi_asprintf(&str, "0x%04" PRIX16, *(dmi_handle_t *)value) < 0)
        return nullptr;

    return str;
}

static char *dmi_attribute_format_string(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    dmi_unused(attr);
    dmi_unused(pretty);

    const char *str = *(const char **)value;
    if (str == nullptr)
        return nullptr;

    return strdup(str);
}

static char *dmi_attribute_format_bool(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    const char *str = nullptr;

    if (pretty) {
        if (attr->params.values) {
            str = dmi_name_lookup(attr->params.values, *(bool *)value ? 1 : 0);
        } else {
            str = *(bool *)value ? "yes" : "no";
        }
    } else {
        str = *(bool *)value ? "true" : "false";
    }

    return strdup(str);
}

static char *dmi_attribute_format_integer(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    dmi_unused(pretty);

    int   rv        = -1;
    bool  is_signed = attr->params.flags & DMI_ATTRIBUTE_FLAG_SIGNED;
    bool  is_hex    = attr->params.flags & DMI_ATTRIBUTE_FLAG_HEX;
    char *str       = nullptr;

    // Note: as printf() arguments of type integer with sizes less than
    // sizeof(int) passed by compiler using int type, we need different casts
    // to make sign extension work properly.

    switch (attr->value.size) {
    case sizeof(int8_t):
        if (is_signed)
            rv = dmi_asprintf(&str, "%" PRId8, *(int8_t *)value);
        else if (is_hex)
            rv = dmi_asprintf(&str, "0x%" PRIX8, *(uint8_t *)value);
        else
            rv = dmi_asprintf(&str, "%" PRIu8, *(uint8_t *)value);
        break;

    case sizeof(int16_t):
        if (is_signed)
            rv = dmi_asprintf(&str, "%" PRId16, *(int16_t *)value);
        else if (is_hex)
            rv = dmi_asprintf(&str, "0x%" PRIX16, *(uint16_t *)value);
        else
            rv = dmi_asprintf(&str, "%" PRIu16, *(uint16_t *)value);
        break;

    case sizeof(int32_t):
        if (is_signed)
            rv = dmi_asprintf(&str, "%" PRId32, *(int32_t *)value);
        else if (is_hex)
            rv = dmi_asprintf(&str, "0x%" PRIX32, *(uint32_t *)value);
        else
            rv = dmi_asprintf(&str, "%" PRIu32, *(uint32_t *)value);
        break;

    case sizeof(int64_t):
        if (is_signed)
            rv = dmi_asprintf(&str, "%" PRId64, *(int64_t *)value);
        else if (is_hex)
            rv = dmi_asprintf(&str, "0x%" PRIX64, *(uint64_t *)value);
        else
            rv = dmi_asprintf(&str, "%" PRIu64, *(uint64_t *)value);
        break;

    default:
        return nullptr;
    }

    if (rv < 0)
        return nullptr;

    return str;
}

static char *dmi_attribute_format_decimal(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    dmi_unused(pretty);

    char  fmt[16];
    int   rv  = 0;
    char *str = nullptr;

    if (attr->params.scale == 0)
        return dmi_attribute_format_integer(attr, value, pretty);

    intmax_t src = dmi_attribute_get_int(attr, value);

    unsigned int scale = attr->params.scale;
    unsigned int factor = dmi_ipow32(10, scale);

    // Adjust scale and factor
    while (scale > 1) {
        if (src % 10 != 0)
            break;
        src /= 10, factor /= 10;
        scale--;
    }

    if (attr->params.flags & DMI_ATTRIBUTE_FLAG_SIGNED) {
        snprintf(fmt, sizeof(fmt), "%%lld.%%0%dllu", scale);
        rv = dmi_asprintf(&str, fmt, src / factor, llabs(src) % factor);
    } else {
        snprintf(fmt, sizeof(fmt), "%%llu.%%0%dllu", scale);
        rv = dmi_asprintf(&str, fmt, (uint64_t)src / factor, (uint64_t)src % factor);
    }

    if (rv < 0)
        return nullptr;

    return str;
}

static char *dmi_attribute_format_size(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    static const char *units[] = {
        "bytes", "KiB", "MiB", "GiB", "TiB", "PiB", nullptr
    };

    assert(attr != nullptr);
    assert(value != nullptr);

    int rv;
    unsigned int i;

    char *str = nullptr;
    uintmax_t size = dmi_attribute_get_uint(attr, value);

    if (pretty) {
        for (i = 0; units[i]; i++) {
            if ((size < 1024) or (size % 1024 != 0))
                break;
            size >>= 10;
        }

        rv = dmi_asprintf(&str, "%" PRIu64 " %s", size, units[i]);
    } else {
        rv = dmi_asprintf(&str, "%" PRIu64, size);
    }

    if (rv < 0)
        return nullptr;

    return str;
}

static char *dmi_attribute_format_address(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    // TODO: Implement address formatting
    return dmi_attribute_format_integer(attr, value, pretty);
}

static char *dmi_attribute_format_enum(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    const char *name;
    char *str = nullptr;

    assert(attr != nullptr);
    assert(value != nullptr);

    if (attr->params.values == nullptr)
        return nullptr;

    if (pretty)
        name = dmi_name_lookup(attr->params.values, *(int *)value);
    else
        name = dmi_code_lookup(attr->params.values, *(int *)value);

    if (name != nullptr)
        str = strdup(name);
    else if (pretty)
        dmi_asprintf(&str, "<invalid> (0x%x)", *(int *)value);
    else
        dmi_asprintf(&str, "0x%x", *(int *)value);

    return str;
}

static char *dmi_attribute_format_set(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    dmi_unused(pretty);

    char fmt[16];
    char *str = nullptr;
    uintmax_t src = dmi_attribute_get_uint(attr, value);

    snprintf(fmt, sizeof(fmt), "0x%%0%zu" PRIXMAX, attr->value.size * 2);

    int rv = dmi_asprintf(&str, fmt, src);
    if (rv < 0)
        return nullptr;

    return str;
}

static char *dmi_attribute_format_version(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    dmi_unused(pretty);

    int rv = 0;
    char *str = nullptr;

    dmi_version_t version = *(dmi_version_t *)value;

    unsigned major    = dmi_version_major(version);
    unsigned minor    = dmi_version_minor(version);
    unsigned revision = dmi_version_revision(version);

    unsigned scale = attr->params.scale;

    if (scale == 1)
        rv = dmi_asprintf(&str, "%u", major);
    else if (scale == 2)
        rv = dmi_asprintf(&str, "%u.%u", major, minor);
    else
        rv = dmi_asprintf(&str, "%u.%u.%u", major, minor, revision);

    if (rv < 0)
        return nullptr;

    return str;
}

static char *dmi_attribute_format_date(const dmi_attribute_t *attr, const void *value, bool pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    dmi_unused(attr);
    dmi_unused(pretty);

    return dmi_date_format(*(dmi_date_t *)value);
}

static char *dmi_attribute_format_uuid(
        const dmi_attribute_t *attr,
        const void            *value,
        bool                   pretty)
{
    assert(attr != nullptr);
    assert(value != nullptr);

    dmi_unused(attr);
    dmi_unused(pretty);

    int rv = 0;
    char *str = nullptr;

    dmi_uuid_t *uuid = (dmi_uuid_t *)value;

    rv = dmi_asprintf(&str, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                      dmi_ntoh(uuid->time_low),
                      dmi_ntoh(uuid->time_mid),
                      dmi_ntoh(uuid->time_hi_and_version),
                      uuid->clock_seq_hi_and_reserved,
                      uuid->clock_seq_low,
                      uuid->node[0], uuid->node[1], uuid->node[2],
                      uuid->node[3], uuid->node[4], uuid->node[5]);

    if (rv < 0)
        return nullptr;

    return str;
}
