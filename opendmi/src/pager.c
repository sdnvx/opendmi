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

#include <wordexp.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <opendmi/context.h>
#include <opendmi/error.h>
#include <opendmi/pager.h>

bool dmi_pager_start(dmi_context_t *context)
{
    bool success = false;
    wordexp_t we = {};
    int rv;
	int fds[2];
	pid_t pid;

    const char *pager = getenv("PAGER");
    if (pager == nullptr)
        return true;

    rv = wordexp("${PAGER}", &we, WRDE_NOCMD);
    if (rv != 0) {
        switch (rv) {
        case WRDE_BADCHAR:
        case WRDE_BADVAL:
        case WRDE_SYNTAX:
            dmi_error_raise_ex(context, DMI_ERROR_SYSTEM, "Invalid $PAGER value: '%s'", pager);
            break;

        case WRDE_CMDSUB:
            dmi_error_raise_ex(context, DMI_ERROR_SYSTEM, "Command substitution is not allowed in $PAGER");
            break;

        case WRDE_NOSPACE:
            dmi_error_raise(context, DMI_ERROR_OUT_OF_MEMORY);
            break;
        }

        return false;
    }

    do {
        if (pipe(fds) < 0) {
            dmi_error_raise_ex(context, DMI_ERROR_SYSTEM, "Unable to create pipe: %s", strerror(errno));
            break;
        }

        pid = fork();
        if (pid < 0) {
            dmi_error_raise_ex(context, DMI_ERROR_SYSTEM, "Fork failed: %s", strerror(errno));
            break;
        }

        if (pid > 0) {
            close(STDIN_FILENO);

            if (dup2(fds[STDIN_FILENO], STDIN_FILENO) < 0) {
                dmi_error_raise_ex(context, DMI_ERROR_FILE_DUP, "%s", strerror(errno));
                break;
            }

            close(fds[STDIN_FILENO]);
            close(fds[STDOUT_FILENO]);

            if (execvp(we.we_wordv[0], we.we_wordv) < 0) {
                dmi_error_raise_ex(context, DMI_ERROR_SYSTEM, "Unable to exec pager: %s", strerror(errno));
                break;
            }
        } else {
            close(STDOUT_FILENO);

            if (dup2(fds[STDOUT_FILENO], STDOUT_FILENO) < 0) {
                dmi_error_raise_ex(context, DMI_ERROR_FILE_DUP, "%s", strerror(errno));
                break;
            }

            close(fds[STDIN_FILENO]);
            close(fds[STDOUT_FILENO]);
        }

        success = true;
    } while (false);

    wordfree(&we);

	return success;
}
