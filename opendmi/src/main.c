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
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <libgen.h>
#include <getopt.h>
#include <errno.h>
#include <assert.h>

#ifdef ENABLE_CURSES
#   if defined(CURSES_HAVE_NCURSES_NCURSES_H)
#       include <ncurses/ncurses.h>
#   elif defined(CURSES_HAVE_NCURSES_CURSES_H)
#       include <ncurses/curses.h>
#   elif defined(CURSES_HAVE_NCURSES_H)
#       include <ncurses.h>
#   elif defined(CURSES_HAVE_CURSES_H)
#       include <curses.h>
#   endif
#   include <term.h>
#endif // ENABLE_CURSES

#include <opendmi/context.h>
#include <opendmi/pager.h>
#include <opendmi/entity.h>
#include <opendmi/filter.h>
#include <opendmi/format.h>
#include <opendmi/format/text.h>

typedef enum dmi_command
{
    DMI_COMMAND_DUMP_TABLE,
    DMI_COMMAND_LIST_KEYWORDS,
    DMI_COMMAND_LIST_TYPES
} dmi_command_t;

typedef struct dmi_config
{
    dmi_command_t command;
    char *memory_device;
    bool quiet;
    bool debug;
    bool dump;
    char *input_path;
    char *output_path;
    const dmi_format_t *output_format;
    dmi_filter_t filter;
} dmi_config_t;

static bool parse_args(int argc, char *argv[], int *rv);
static dmi_handle_t parse_handle(const char *str);

static void show_version(void);
static void show_usage(const char *proc);

static bool list_keywords(dmi_context_t *context);
static bool list_types(dmi_context_t *context);

static void print_all(dmi_context_t *context, const dmi_format_t *format);
static void print_entity(const dmi_format_t *format, const dmi_entity_t *entity, void *session);

static void log_error(
        dmi_context_t   *context,
        dmi_log_level_t  level,
        const char      *format,
        va_list          args);

dmi_config_t config =
{
    .command       = DMI_COMMAND_DUMP_TABLE,
    .memory_device = nullptr,
    .quiet         = false,
    .debug         = false,
    .dump          = false,
    .input_path    = nullptr,
    .output_path   = nullptr,
    .output_format = &dmi_text_format,
    .filter        = { 0 }
};

static bool tty = false;

int main(int argc, char *argv[])
{
    dmi_context_t *context;
    bool status = false;

    int rv = EXIT_SUCCESS;
    if (not parse_args(argc, argv, &rv))
        return rv;

    argc -= optind;
    argv += optind;

#ifdef ENABLE_CURSES
    // Initialize terminal
    if (isatty(STDOUT_FILENO)) {
        if (setupterm(nullptr, STDOUT_FILENO, nullptr) == 0)
            tty = has_colors() and (start_color() != 0);
    }
#endif // ENABLE_CURSES

    // Create DMI context
    context = dmi_create();
    if (context == nullptr) {
        fprintf(stderr, "Unable to create DMI context\n");
        return EXIT_FAILURE;
    }

    // Initialize logging
    dmi_set_logger(context, log_error);
    if (config.quiet)
        dmi_set_log_level(context, DMI_LOG_WARNING);
    else if (config.debug)
        dmi_set_log_level(context, DMI_LOG_DEBUG);

    do {
        if (isatty(STDOUT_FILENO) && (config.output_path == nullptr)) {
            if (not dmi_pager_start(context))
                break;
        }

        if (config.command == DMI_COMMAND_LIST_KEYWORDS) {
            status = list_keywords(context);
            break;
        }

        if (config.command == DMI_COMMAND_LIST_TYPES) {
            status = list_types(context);
            break;
        }

        if (config.input_path != nullptr)
            status = dmi_dump_load(context, config.input_path);
        else
            status = dmi_open(context);

        if (not status)
            break;

        if (config.output_path != nullptr) {
            status = dmi_dump_save(context, config.output_path, false);
            if (not status)
                break;
        } else {
            print_all(context, config.output_format);
        }

        status = true;
    } while (false);

    // Close DMI context
    dmi_destroy(context);

    return status ? EXIT_SUCCESS : EXIT_FAILURE;
}

static bool parse_args(int argc, char *argv[], int *rv)
{
    static struct option long_options[] =
    {
        { "dev-mem",   required_argument, nullptr, 'd' },
        { "debug",     no_argument,       nullptr, 'g' },
        { "quiet",     no_argument,       nullptr, 'q' },
        { "string",    optional_argument, nullptr, 's' },
        { "type",      optional_argument, nullptr, 't' },
        { "handle",    required_argument, nullptr, 'H' },
        { "dump",      no_argument,       nullptr, 'u' },
        { "dump-bin",  required_argument, nullptr, 'o' },
        { "format",    required_argument, nullptr, 'f' },
        { "from-dump", required_argument, nullptr, 'i' },
        { "help",      no_argument,       nullptr, 'h' },
        { "version",   no_argument,       nullptr, 'v' }
    };

    const char *proc = basename(argv[0]);

    while (true) {
        int idx = 0;
        int opt = getopt_long(argc, argv, "d:qs?t?H:uo:i:hv", long_options, &idx);

        if (opt < 0)
            break;

        switch (opt) {
        case 'd':
            config.memory_device = optarg;
            break;

        case 'q':
            config.quiet = true;
            break;

        case 'g':
            config.debug = true;
            break;

        case 's':
            if (optarg == nullptr)
                config.command = DMI_COMMAND_LIST_KEYWORDS;
            break;

        case 't':
            if (optarg == nullptr)
                config.command = DMI_COMMAND_LIST_TYPES;
            break;

        case 'H':
            dmi_handle_t handle = parse_handle(optarg);
            if (handle == DMI_HANDLE_INVALID)
                return EXIT_FAILURE;
            if (not dmi_filter_add_handle(&config.filter, handle))
                return EXIT_FAILURE;
            break;

        case 'u':
            config.dump = true;
            break;

        case 'o':
            config.output_path = optarg;
            break;

        case 'i':
            config.input_path = optarg;
            break;

        case 'f':
            config.output_format = dmi_format_get(optarg);
            if (config.output_format == nullptr) {
                *rv = EXIT_FAILURE;
                fprintf(stderr, "%s: Invalid output format: %s", proc, optarg);
                return false;
            }
            break;

        case 'v':
            show_version();
            return false;

        case 'h':
            show_usage(proc);
            return false;

        default:
            *rv = EXIT_FAILURE;
            return false;
        }
    }

    return true;
}

static dmi_handle_t parse_handle(const char *str)
{
    char *ep;
    unsigned long value;

    errno = 0;
    value = strtoul(str, &ep, 16);

    if ((*str == 0) or (*ep != 0)) {
        fprintf(stderr, "Invalid handle value: %s\n", str);
        return DMI_HANDLE_INVALID;
    }
    if (((errno == ERANGE) and (value == ULONG_MAX)) or (value >= DMI_HANDLE_INVALID)) {
        fprintf(stderr, "Handle is out of range: %s\n", str);
        return DMI_HANDLE_INVALID;
    }

    return (dmi_handle_t)value;
}

static void show_version(void)
{
    printf("OpenDMI, version %s\n", OPENDMI_VERSION);
    printf("Copyright (c) 2025-2026, The OpenDMI contributors\n\n");
}

static void show_usage(const char *proc)
{
    show_version();

    printf("Usage:\n");
    printf("    %s [options]\n", proc);
}

static bool list_keywords(dmi_context_t *context __attribute__((unused)))
{
    return true;
}

static bool list_types(dmi_context_t *context)
{
    const char *format;

    if (isatty(STDIN_FILENO))
        format = "%3d %-24s %s\n";
    else
        format = "%d\t%s\t%s\n";

    for (size_t type = 0; type < 0x100; type++) {
        const dmi_entity_spec_t *spec = dmi_type_spec(context, (dmi_type_t)type);

        if (spec == nullptr)
            continue;

        printf(format, (int)spec->type, spec->code, spec->name);
    }

    return true;
}

static void print_all(dmi_context_t *context, const dmi_format_t *format)
{
    void *session;
    dmi_registry_iter_t iter;
    dmi_entity_t *entity;

    session = format->handlers.initialize(context, stdout);
    if (not session)
        return;

    if (format->handlers.dump_start != nullptr)
        format->handlers.dump_start(session);

    format->handlers.entry(session);

    if (format->handlers.table_start != nullptr)
        format->handlers.table_start(session);

    dmi_registry_iter_init(&iter, context->registry, &config.filter);
    while ((entity = dmi_registry_iter_next(&iter)) != nullptr) {
        print_entity(format, entity, session);
    }

    if (format->handlers.table_end != nullptr)
        format->handlers.table_end(session);

    if (format->handlers.dump_end != nullptr)
        format->handlers.dump_end(session);

    format->handlers.finalize(session);
}

static void print_entity(
        const dmi_format_t *format,
        const dmi_entity_t *entity,
        void               *session)
{
    assert(format != nullptr);
    assert(entity != nullptr);
    assert(session != nullptr);

    const dmi_entity_spec_t *spec = entity->spec;
    const dmi_attribute_t   *attr = nullptr;

    format->handlers.entity_start(session, entity);

    if (entity->info and not config.dump) {
        if (format->handlers.entity_attrs_start != nullptr)
            format->handlers.entity_attrs_start(session, entity);

        for (attr = spec->attributes; attr->params.name; attr++) {
            const dmi_data_t *value = dmi_member_ptr(entity->info, attr->value, dmi_data_t);

            // Check attribute level
            if (attr->params.level != DMI_VERSION_NONE) {
                if (entity->level < attr->params.level)
                    continue;
            }

            format->handlers.entity_attr(session, entity, attr, value);
        }

        if (format->handlers.entity_attrs_end != nullptr)
            format->handlers.entity_attrs_end(session, entity);
    } else {
        format->handlers.entity_data(session, entity);
        format->handlers.entity_strings(session, entity);
    }

    format->handlers.entity_end(session, entity);
}

static void log_error(
        dmi_context_t   *context __attribute__((unused)),
        dmi_log_level_t  level,
        const char      *format,
        va_list          args)
{
    FILE *out = stderr;

#ifdef ENABLE_CURSES
    if (tty) {
        out = stdout;

        switch (level) {
        case DMI_LOG_DEBUG:
            tputs(tparm(tigetstr("setaf"), 8), 1, putchar);
            break;

        case DMI_LOG_INFO:
            tputs(tparm(tigetstr("setaf"), COLOR_GREEN), 1, putchar);
            break;

        case DMI_LOG_NOTICE:
            tputs(tparm(tigetstr("setaf"), COLOR_CYAN), 1, putchar);
            break;

        case DMI_LOG_WARNING:
            tputs(tparm(tigetstr("setaf"), COLOR_YELLOW), 1, putchar);
            break;

        case DMI_LOG_ERROR:
            tputs(tparm(tigetstr("setaf"), COLOR_RED), 1, putchar);
            break;

        default:
            // fallthrough
        }
    }
#endif // ENABLE_CURSES

    fprintf(out, "[%s] ", dmi_log_level_name(level));

#ifdef ENABLE_CURSES
    if (tty)
        tputs(tparm(tigetstr("sgr0")), 1, putchar);
#endif // ENABLE_CURSES

    vfprintf(out, format, args);
    fprintf(out, "\n");
}
