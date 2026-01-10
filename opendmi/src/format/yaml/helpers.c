//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <string.h>
#include <assert.h>

#include <opendmi/format/yaml/helpers.h>

bool dmi_yaml_label(dmi_yaml_session_t *session, const char *value)
{
    return dmi_yaml_scalar(session, value, YAML_PLAIN_SCALAR_STYLE);
}

bool dmi_yaml_scalar(dmi_yaml_session_t *session, const char *value, yaml_scalar_style_t style)
{
    assert(session != nullptr);

    bool success = false;
    yaml_event_t event = {};

    do {
        if (not yaml_scalar_event_initialize(
                    &event, nullptr, nullptr, (const yaml_char_t *)value, strlen(value), true, false, style))
            break;
        if (not yaml_emitter_emit(session->emitter, &event))
            break;

        success = true;
    } while (false);

    return success;
}

bool dmi_yaml_sequence_start(dmi_yaml_session_t *session, yaml_sequence_style_t style)
{
    assert(session != nullptr);

    bool success = false;
    yaml_event_t event = {};

    do {
        if (not yaml_sequence_start_event_initialize(&event, nullptr, nullptr, true, style))
            break;
        if (not yaml_emitter_emit(session->emitter, &event))
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
        if (not yaml_emitter_emit(session->emitter, &event))
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
        if (not yaml_mapping_start_event_initialize(&event, nullptr, nullptr, true, style))
            break;
        if (not yaml_emitter_emit(session->emitter, &event))
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
        if (not yaml_emitter_emit(session->emitter, &event))
            break;

        success = true;
    } while (false);

    return success;
}
