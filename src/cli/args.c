#include <stdbool.h>
#include <fllib/Cstring.h>
#include "args.h"

bool sbs_args_is_flag(const char *longname, const char *shortname, const char *arg)
{
    // If the first condition is true, it means arg is at least "longname_size" too, so it is safe to access
    // arg[longname_size] IF strlen(arg) != longname_size (it can only be greather in that case)
    size_t longname_size = strlen(longname);
    if (flm_cstring_equals_n(longname, arg, longname_size) && (strlen(arg) == longname_size || arg[longname_size] == '='))
        return true;

    // Same as above, but for shortname
    size_t shortname_size = strlen(shortname);
    if (flm_cstring_equals_n(shortname, arg, shortname_size) && (strlen(arg) == shortname_size || arg[shortname_size] == '='))
        return true;

    return false;
}

bool sbs_args_is_string(const char *arg, char **option)
{
    if (!arg)
        return false;

    *option = fl_cstring_find(arg, "=");

    if (!*option)
        return false;

    *option += 1;
    return true;
}

bool sbs_args_is_bool(const char *arg, bool *option)
{
    if (!arg)
        return false;

    char *value = fl_cstring_find(arg, "=");

    if (!value)
        return false;

    bool result = false;

    if (flm_cstring_equals(value, "true") || flm_cstring_equals(value, "yes") || flm_cstring_equals(value, "y"))
    {
        result = true;
    }
    else if (flm_cstring_equals(value, "false") || flm_cstring_equals(value, "no") || flm_cstring_equals(value, "n"))
    {
        result = false;
    }
    else
    {
        return false;
    }

    if (option)
        *option = result;

    return true;
}
