//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include <opendmi/utils.h>
#include <opendmi/utils/vector.h>

static bool dmi_vector_resize(dmi_vector_t *vector, size_t capacity);

const size_t dmi_vector_delta_capacity = 16;

bool dmi_vector_push(dmi_vector_t *vector, uintptr_t value)
{
    assert(vector != nullptr);

    if (vector->length >= vector->capacity) {
        if (not dmi_vector_resize(vector, vector->capacity + dmi_vector_delta_capacity))
            return false;
    }

    vector->data[vector->length++] = value;

    return true;
}

bool dmi_vector_pop(dmi_vector_t *vector, uintptr_t *value)
{
    assert(vector != nullptr);
    assert(vector != nullptr);

    if (vector->length == 0)
        return false;

    *value = vector->data[--vector->length];

    return true;
}

void dmi_vector_clear(dmi_vector_t *vector)
{
    free(vector->data);
    memset(vector, 0, sizeof(*vector));
}

static bool dmi_vector_resize(dmi_vector_t *vector, size_t capacity)
{
    uintptr_t *data = realloc(vector->data, sizeof(vector->data[0]) * capacity);
    if (data == nullptr)
        return false;

    vector->data     = data;
    vector->capacity = capacity;

    return true;
}
