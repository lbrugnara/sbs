#ifndef SBS_ARGS_H

#include <fllib/Std.h>
#include <fllib/Cstring.h>

typedef enum SbsArgsResult {
    SBS_ARGS_OK,
    SBS_ARGS_HELP,
    SBS_ARGS_ERROR
} SbsArgsResult;

#define sbs_parse_args(args, body) \
do { \
    bool sbs_break = false; \
    SbsArgsResult *sbs_resultptr = NULL; \
    char *sbs_arg = NULL; \
    char **sbs_argsptr = (args); \
    body \
} while (0)

#define use_retval(retvalptr) sbs_resultptr = (retvalptr); *sbs_resultptr = SBS_ARGS_OK
    
#define use_error_func(perr) void (*sbs_print_err)(const char*, ...) = (perr)

#define with_help(lname, sname) \
do { \
    char **sbs_argsptr_bu = sbs_argsptr; \
    while ((sbs_arg = *sbs_argsptr++)) \
    { \
        if (sbs_args_is_flag((lname), (sname), sbs_arg)) { \
            sbs_break = true; \
            if (sbs_resultptr) *(sbs_resultptr) = SBS_ARGS_HELP; \
        } \
    } \
    sbs_argsptr = sbs_argsptr_bu; \
    sbs_arg = NULL; \
} while (0)

#define with_help_func(lname, sname, func) \
do { \
    char **sbs_argsptr_bu = sbs_argsptr; \
    while ((sbs_arg = *sbs_argsptr++)) \
    { \
        if (sbs_args_is_flag((lname), (sname), sbs_arg)) { \
            sbs_break = true; \
            func \
        } \
    } \
    sbs_argsptr = sbs_argsptr_bu; \
    sbs_arg = NULL; \
} while (0)

#define with_options(...) \
do { \
    if (sbs_break) break; \
    sbs_arg = *sbs_argsptr++; \
    if (!sbs_arg) \
        break; \
    if (false){} \
    __VA_ARGS__ \
    else { \
        if (sbs_print_err) { \
            size_t l = strlen(sbs_arg); \
            if ((l == 1 && sbs_arg[0] == '-') || (l == 2 && sbs_arg[0] == '-' && sbs_arg[1] == '-')) \
                sbs_print_err("Missing option name for %s flag.", l == 1 ? "short name" : "long name"); \
            else \
                sbs_print_err("Unknown option '%s'.", sbs_arg, sbs_arg); \
        } \
        if (sbs_resultptr) *sbs_resultptr = SBS_ARGS_ERROR; \
    } \
} while (1)

#define command_any(dest) \
    else if (sbs_arg[0] != '-' && (dest) == NULL) \
    { \
        (dest) = sbs_arg; \
    } \

#define flag_string(lname, sname, strptr) \
    else if (sbs_args_is_flag((lname), (sname), sbs_arg)) \
    { \
        if (!sbs_args_is_string(sbs_arg, (strptr))) \
        { \
            if (sbs_print_err) \
                sbs_print_err("Flag '%s' expects a string value, use '%s=...'?", sbs_arg, sbs_arg); \
            if (sbs_resultptr)  \
                *sbs_resultptr = SBS_ARGS_ERROR; \
        } \
    }

#define flag_bool(lname, sname, boolptr) \
    else if (sbs_args_is_flag((lname), (sname), sbs_arg)) \
    { \
        if (!sbs_args_is_bool(sbs_arg, (boolptr))) \
        { \
            if (sbs_print_err) \
                sbs_print_err("Flag '%s' expects a string value, use '%s=...'?", sbs_arg, sbs_arg); \
            if (sbs_resultptr)  \
                *sbs_resultptr = SBS_ARGS_ERROR; \
        } \
    }

#define flag_without_value(lname, sname, boolptr) \
    else if (sbs_args_is_flag((lname), (sname), sbs_arg)) \
    { \
        *(boolptr) = true; \
    }

#define sbs_args_is_command(module_name, str) flm_cstring_equals((module_name), (str))

bool sbs_args_is_flag(const char *longname, const char *shortname, const char *arg);
bool sbs_args_is_string(const char *arg, char **option);
bool sbs_args_is_bool(const char *arg, bool *option);

#endif /* SBS_ARGS_H */
