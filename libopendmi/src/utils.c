//
// OpenDMI: Cross-platform DMI/SMBIOS framework
// Copyright (c) 2025-2026, The OpenDMI contributors
//
// SPDX-License-Identifier: BSD-3-Clause
//
#include "config.h"

#include <sys/stat.h>

#ifdef HAVE_SYS_MMAN_H
#   include <sys/mman.h>
#endif // HAVE_SYS_MMAN_H

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_SHARE_H
#   include <share.h>
#endif // !HAVE_SHARE_H

#ifdef HAVE_IO_H
#   include <io.h>
#endif // HAVE_IO_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include <opendmi/context.h>
#include <opendmi/utils.h>

#if defined(_WIN32)
typedef struct _stat stat_t;

#define fstat(fd, buf)      _fstat(fd, buf)
#define read(fd, buf, size) _read(fd, buf, size)
#define close(fd)           _close(fd)
#else // !defined(_WIN32)
typedef struct stat stat_t;
#endif // !defined(_WIN32)

static ssize_t dmi_file_read_data(int fd, dmi_data_t *data, size_t avail);

void *dmi_alloc(dmi_context_t *context, size_t size)
{
    void *ptr;

    ptr = calloc(1, size);
    if ((ptr == nullptr) and (context != nullptr)) {
        dmi_log_error(context, dmi_error_message(DMI_ERROR_OUT_OF_MEMORY));
        dmi_error_raise(context, DMI_ERROR_OUT_OF_MEMORY);
    }

    return ptr;
}

void dmi_free(void *ptr)
{
    if (ptr == nullptr)
        return;

    free(ptr);
}

bool dmi_checksum(const void *data, size_t length)
{
    if (data == nullptr) {
        errno = EINVAL;
        return false;
    }

    uint8_t sum   = 0;
    size_t  index = 0;

    while (index < length) {
        sum += ((const uint8_t *)data)[index++];
    }

    return (sum == 0) ? true : false;
}

uint32_t dmi_ipow32(uint32_t value, unsigned int factor)
{
    uint32_t result = 1;

    if (factor == 0)
        return result;

    while (factor > 1) {
        if (factor % 2)
            result *= value, factor--;

        value *= value, factor /= 2;
    }

    return value * result;
}

uint64_t dmi_ipow64(uint64_t value, unsigned int factor)
{
    uint64_t result = 1;

    if (factor == 0)
        return result;

    while (factor > 1) {
        if (factor % 2)
            result *= value, factor--;

        value *= value, factor /= 2;
    }

    return value * result;
}

int dmi_asprintf(char **strp, const char *fmt, ...)
{
    int rv;
    va_list ap;

    va_start(ap, fmt);
    rv = dmi_vasprintf(strp, fmt, ap);
    va_end(ap);

    return rv;
}

int dmi_vasprintf(char **strp, const char *fmt, va_list ap)
{
    int rv;

#if defined(_WIN32)
    // _vscprintf tells you how big the buffer needs to be
    int len = _vscprintf(fmt, ap);
    if (len < 0)
        return -1;

    size_t size = (size_t)len + 1;
    char *str = malloc(size);
    if (str == nullptr) {
        *strp = nullptr;
        return -1;
    }

    // vsprintf_s is the "secure" version of vsprintf
    rv = vsprintf_s(str, size, fmt, ap);
    if (rv < 0) {
        free(str);
        *strp = nullptr;
        return -1;
    }

    *strp = str;
#else // !defined(_WIN32)
    rv = vasprintf(strp, fmt, ap);
#endif // !defined(_WIN32)

    return rv;
}

dmi_data_t *dmi_file_read(dmi_context_t *context, const char *path, size_t *plength)
{
    if (context == nullptr)
        return nullptr;

    if (path == nullptr) {
        dmi_error_raise_ex(context, DMI_ERROR_NULL_ARGUMENT, "path");
        return nullptr;
    }
    if (plength == nullptr) {
        dmi_error_raise_ex(context, DMI_ERROR_NULL_ARGUMENT, "plength");
        return nullptr;
    }

    int         fd     = -1;
    dmi_data_t *data   = nullptr;
    ssize_t     length = 0;

    bool success = false;
    do {
        stat_t st;

        dmi_log_debug(context, "Reading %s ...", path);

#if defined(_WIN32)
        if (_sopen_s(&fd, path, _O_RDONLY, _SH_DENYNO, _S_IREAD) != 0)
            break;
#else
        if ((fd = open(path, O_RDONLY)) < 0) {
            dmi_error_raise_ex(context, DMI_ERROR_FILE_OPEN, "%s: %s", path, strerror(errno));
            break;
        }
#endif

        if (fstat(fd, &st) < 0) {
            dmi_error_raise_ex(context, DMI_ERROR_FILE_STAT, "%s: %s", path, strerror(errno));
            break;
        }

        if ((data = malloc(st.st_size)) == nullptr) {
            dmi_error_raise(context, DMI_ERROR_OUT_OF_MEMORY);
            break;
        }

        length = dmi_file_read_data(fd, data, st.st_size);
        if (length < 0) {
            dmi_error_raise_ex(context, DMI_ERROR_FILE_READ, "%s: %s", path, strerror(errno));
            break;
        }

        dmi_log_debug(context, "Read %zu bytes", length);

        success = true;
    } while (false);

    if (fd >= 0)
        close(fd);

    if (not success) {
        dmi_free(data);
        return nullptr;
    }

    *plength = (size_t)length;

    return data;
}

static ssize_t dmi_file_read_data(int fd, dmi_data_t *data, size_t avail)
{
    assert(fd >= 0);
    assert(data != nullptr);

    size_t length = 0;

    while (avail > 0) {
        ssize_t nread = read(fd, data + length, avail);

        if (nread < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }

        if (nread == 0)
            break;

        length += nread;
        avail  -= nread;
    }

    return length;
}

#if !defined(_WIN32)
dmi_data_t *dmi_file_map(dmi_context_t *context, const char *path, size_t *plength)
{
    int         fd   = -1;
    dmi_data_t *data = nullptr;
    stat_t      st;

    if (context == nullptr)
        return nullptr;

    if (path == nullptr) {
        dmi_error_raise_ex(context, DMI_ERROR_NULL_ARGUMENT, "path");
        return nullptr;
    }
    if (plength == nullptr) {
        dmi_error_raise_ex(context, DMI_ERROR_NULL_ARGUMENT, "plength");
        return nullptr;
    }

    bool success = false;
    do {
        fd = open(path, O_RDONLY);
        if (fd < 0) {
            dmi_error_raise_ex(context, DMI_ERROR_FILE_OPEN, "%s: %s", path, strerror(errno));
            break;
        }

        if (fstat(fd, &st) < 0) {
            dmi_error_raise_ex(context, DMI_ERROR_FILE_STAT, "%s: %s", path, strerror(errno));
            break;
        }

        if ((data = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
            dmi_error_raise_ex(context, DMI_ERROR_FILE_MAP, "%s: %s", path, strerror(errno));
            break;
        }

        success = true;
    } while (false);

    if (fd >= 0)
        close(fd);

    if (not success)
        return nullptr;

    *plength = st.st_size;

    return data;
}
#endif // !defined(_WIN32)
