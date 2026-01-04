//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <opendmi/context.h>
#include <opendmi/utils.h>
#include <opendmi/utils/decode.h>

#include <opendmi/entity/firmware-language.h>

const dmi_attribute_t dmi_firmware_language_attrs[] =
{
    DMI_ATTRIBUTE(dmi_firmware_language_t, language_count, INTEGER, {
        .code = "language-count",
        .name = "Language count"
    }),
    DMI_ATTRIBUTE_ARRAY(dmi_firmware_language_t, languages, language_count, STRING, {
        .code = "languages",
        .name = "Languages"
    }),
    DMI_ATTRIBUTE(dmi_firmware_language_t, is_abbreviated, BOOL, {
        .code = "is-abbreviated",
        .name = "Abbreviated"
    }),
    DMI_ATTRIBUTE(dmi_firmware_language_t, current_language, STRING, {
        .code = "current-language",
        .name = "Current language"
    }),
    DMI_ATTRIBUTE_NULL
};

const dmi_entity_spec_t dmi_firmware_language_spec =
{
    .code       = "firmware-language",
    .name       = "Firmware language information",
    .type       = DMI_TYPE_FIRMWARE_LANGUAGE,
    .min_length = 0x05,
    .attributes = dmi_firmware_language_attrs,
    .handlers   = {
        .decode = (dmi_entity_decode_fn_t)dmi_firmware_language_decode,
        .free   = (dmi_entity_free_fn_t)dmi_firmware_language_free
    }
};

dmi_firmware_language_t *dmi_firmware_language_decode(const dmi_entity_t *entity, dmi_version_t *plevel)
{
    dmi_firmware_language_t *info;
    const dmi_firmware_language_data_t *data;

    data = dmi_cast(data, dmi_entity_data(entity, DMI_TYPE_FIRMWARE_LANGUAGE));
    if (data == nullptr)
        return nullptr;

    info = dmi_alloc(entity->context, sizeof(*info));
    if (info == nullptr)
        return nullptr;

    info->language_count = dmi_decode(data->language_count);

    info->languages = dmi_alloc_array(entity->context, sizeof(const char *), info->language_count);
    if (info->languages == nullptr) {
        dmi_free(info);
        return nullptr;
    }

    for (size_t i = 0; i < info->language_count; i++) {
        info->languages[i] = dmi_entity_string(entity, (dmi_string_t)(i + 1));
    }

    dmi_firmware_language_flags_t flags = {
        .__value = dmi_decode(data->flags)
    };

    info->is_abbreviated   = flags.is_abbreviated;
    info->current_language = dmi_entity_string(entity, data->current_language);

    if (plevel != nullptr)
        *plevel = dmi_version(2, 0, 0);

    return info;
}

void dmi_firmware_language_free(dmi_firmware_language_t *info)
{
    dmi_free(info->languages);
    dmi_free(info);
}
