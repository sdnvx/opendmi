//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <opendmi/error.h>
#include <opendmi/utils.h>

#include <opendmi/format/yaml/helpers.h>

bool dmi_yaml_emit(dmi_yaml_session_t *session, yaml_event_t *event)
{
    assert(session != nullptr);
    assert(event != nullptr);

    bool result = yaml_emitter_emit(session->emitter, event);
    if (not result) {
        dmi_error_raise_ex(session->context, DMI_ERROR_INTERNAL,
                           "Unable to emit YAML event: %s", session->emitter->problem);
    }

    return result;
}

bool dmi_yaml_label(dmi_yaml_session_t *session, const char *value)
{
    return dmi_yaml_scalar(session, value, nullptr, YAML_PLAIN_SCALAR_STYLE);
}

bool dmi_yaml_scalar(
        dmi_yaml_session_t  *session,
        const char          *value,
        const char          *tag,
        yaml_scalar_style_t  style)
{
    assert(session != nullptr);

    bool success = false;
    yaml_event_t event = {};
    char *escaped = nullptr;

    do {
        bool result = yaml_scalar_event_initialize(&event, nullptr,
                                                   (yaml_char_t *)tag,
                                                   (yaml_char_t *)value,
                                                   strlen(value),
                                                   true, false, style);

        if (not result) {
            dmi_error_raise_ex(session->context, DMI_ERROR_INTERNAL,
                               "Unable to initialize scalar event: %s",
                               session->emitter->problem);
            break;
        }
        if (not dmi_yaml_emit(session, &event))
            break;

        success = true;
    } while (false);

    dmi_free(escaped);

    return success;
}

bool dmi_yaml_sequence_start(dmi_yaml_session_t *session, yaml_sequence_style_t style)
{
    assert(session != nullptr);

    bool success = false;
    yaml_event_t event = {};

    do {
        bool result = yaml_sequence_start_event_initialize(&event, nullptr,
                                                           (yaml_char_t *)YAML_SEQ_TAG,
                                                           true, style);

        if (not result) {
            dmi_error_raise_ex(session->context, DMI_ERROR_INTERNAL,
                               "Unable to initialize sequence start event: %s",
                               session->emitter->problem);
            break;
        }
        if (not dmi_yaml_emit(session, &event))
            break;

        success = true;
    } while (false);

    return success;
}

bool dmi_yaml_sequence_end(dmi_yaml_session_t *session)
{
    assert(session != nullptr);

    bool success = false;
    yaml_event_t event = {};

    do {
        if (not yaml_sequence_end_event_initialize(&event))
            break;
        if (not dmi_yaml_emit(session, &event))
            break;

        success = true;
    } while (false);

    return success;
}

bool dmi_yaml_mapping_start(dmi_yaml_session_t *session, yaml_mapping_style_t style)
{
    assert(session != nullptr);

    bool success = false;
    yaml_event_t event = {};

    do {
        if (not yaml_mapping_start_event_initialize(&event, nullptr, (yaml_char_t *)YAML_MAP_TAG, true, style))
            break;
        if (not dmi_yaml_emit(session, &event))
            break;

        success = true;
    } while (false);

    return success;
}

bool dmi_yaml_mapping_end(dmi_yaml_session_t *session)
{
    assert(session != nullptr);

    bool success = false;
    yaml_event_t event = {};

    do {
        if (not yaml_mapping_end_event_initialize(&event))
            break;
        if (not dmi_yaml_emit(session, &event))
            break;

        success = true;
    } while (false);

    return success;
}
