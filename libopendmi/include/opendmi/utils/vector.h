//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#ifndef OPENDMI_UTILS_VECTOR_H
#define OPENDMI_UTILS_VECTOR_H

#pragma once

#include <opendmi/types.h>

typedef struct dmi_vector dmi_vector_t;

struct dmi_vector
{
    uintptr_t *data;
    size_t capacity;
    size_t length;
};

__BEGIN_DECLS

bool dmi_vector_push(dmi_vector_t *vector, uintptr_t value);
bool dmi_vector_pop(dmi_vector_t *vector, uintptr_t *value);
void dmi_vector_clear(dmi_vector_t *vector);

__END_DECLS

#endif // !OPENDMI_UTILS_VECTOR_H
