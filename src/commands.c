#include <stdio.h>
#include "commands.h"
#include "result.h"
#include "build.h"
#include "file.h"

#define SBS_VERSION_MAJOR 0
#define SBS_VERSION_MINOR 1
#define SBS_VERSION_PATCH 0

struct SbsListArguments {
    const char *file;
    const char *resource;
};

static const char *build_flags[] = { 
    "--env", "-e",
    "--toolchain", "-tc",
    "--config", "-c",
    "--target", "-t",
    "--file", "-f" ,
    NULL
};

static const char *main_help = 
    "sbs build system (%d.%d.%d)\n"
    "                                                                               \n"
    "Usage: sbs [ <options> | <module> ]                                            \n"
    "                                                                               \n"
    "                                                                               \n"
    "options:                                                                       \n"
    "                                                                               \n"
    "   --help, -h      Prints this message                                         \n"
    "                                                                               \n"
    "                                                                               \n"
    "module:                                                                        \n"
    "                                                                               \n"
    "   init            Initializes the sbs system in the current directory         \n"
    "   list            Prints the list of user defined elements for a resource     \n"
    "   build           Starts the build process                                    \n"
    "                                                                               \n"
    "   For more information on the available resources, run:                       \n"
    "       sbs <module> --help                                                     \n"
    "                                                                               \n"
;

static const char *list_help =
    "sbs build system (%d.%d.%d)\n"
    "                                                                               \n"
    "Usage: sbs list <options>? <resource>                                          \n"
    "                                                                               \n"
    "                                                                               \n"
    "options:                                                                       \n"
    "                                                                               \n"
    "   --help, -h                  Prints this message                             \n"
    "   --file=<path>, -f=<path>    Build file's path. Default value is             \n"
    "                               .sbs/build.sbs                                  \n"
    "                                                                               \n"
    "                                                                               \n"
    "resource:                                                                      \n"
    "                                                                               \n"
    "   One of the following values that identify the different set of              \n"
    "   user-defined objects that can be declared within a build file:              \n"
    "                                                                               \n"
    "       envs            Shows the list of available environments                \n"
    "       toolchains      Shows the list of available toolchains                  \n"
    "       configs         Shows the list of the available configurations          \n"
    "       targets         Shows all the targets available to be built             \n"
    "       presets         Shows the list of available presets                     \n"
    "                                                                               \n"
;

static const char *build_help =
    "sbs build system (%d.%d.%d)\n"
    "                                                                               \n"
    "Usage: sbs build [ <preset> <options>? <resources>? | <options>? <resources> ] \n"
    "                                                                               \n"
    "                                                                               \n"
    "preset:                                                                        \n"
    "                                                                               \n"
    "   An identifier for a preset that contains a predefined set of resources to   \n"
    "   use in the build process. Each preset's resource can be overridden using    \n"
    "   the resources listed below.                                                 \n"
    "   To get a list of the available presets run \"sbs list presets\" .           \n"
    "                                                                               \n"
    "                                                                               \n"
    "options:                                                                       \n"
    "                                                                               \n"
    "   --help, -h                  Prints this message                             \n"
    "   --file=<path>, -f=<path>    Build file's path. Default value is             \n"
    "                               .sbs/build.sbs                                  \n"
    "                                                                               \n"
    "                                                                               \n"
    "resources:                                                                     \n"
    "                                                                               \n"
    "   Resources to be used in the build process. If the preset name is not        \n"
    "   provided, all these resources are required by the build process.            \n"
    "                                                                               \n"
    "       --env=<name>, -e=<name>             Environment to be used in the build \n"
    "                                           process                             \n"
    "                                                                               \n"
    "       --toolchain=<name>, -tc=<name>      Toolchain to build the target       \n"
    "                                                                               \n"
    "       --config=<name>, -c=<name>          Configuration used to build the     \n"
    "                                           target                              \n"
    "                                                                               \n"
    "       --target=<name>, -t=<name>          Target's name to be built           \n"
    "                                                                               \n"
    "   For more information on the available resources, run:                       \n"
    "       sbs list --help                                                         \n"
    "                                                                               \n"
;

static void print_main_help(int argc, char **argv)
{
    fprintf(stderr, main_help, SBS_VERSION_MAJOR, SBS_VERSION_MINOR, SBS_VERSION_PATCH);
}

static void print_build_help(int argc, char **argv)
{
    fprintf(stderr, build_help, SBS_VERSION_MAJOR, SBS_VERSION_MINOR, SBS_VERSION_PATCH);
}

static void print_list_help(int argc, char **argv)
{
    fprintf(stderr, list_help, SBS_VERSION_MAJOR, SBS_VERSION_MINOR, SBS_VERSION_PATCH);
}

static void print_error(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    fprintf(stderr, "\n");
    vfprintf(stderr, message, args);
    va_end(args);
}

static int compare_strings(const void* a, const void* b) 
{ 
    return strcmp(*(const char**)a, *(const char**)b); 
}

static inline bool is_valid_flag(const char *arg, const char *flags[])
{
    while (*flags)
    {
        const char *flag = *flags;
        flags++;
        if (flag && flm_cstring_equals(arg, flag))
            return true;
    }
    return false;
}

static inline bool is_flag_without_value(const char *longname, const char *shortname, const char *arg)
{
    if (!arg)
        return false;

    size_t lnsize = strlen(longname);
    if (flm_cstring_equals_n(longname, arg, lnsize))
        return strlen(arg) == lnsize;

    size_t snsize = strlen(shortname);
    if (flm_cstring_equals_n(shortname, arg, strlen(shortname)))
        return strlen(arg) == snsize;

    return false;
}

static inline bool is_flag_with_string_value(const char *longname, const char *shortname, const char *arg, const char **option)
{
    if (!arg)
        return false;

    if (!flm_cstring_equals_n(longname, arg, strlen(longname)) && !flm_cstring_equals_n(shortname, arg, strlen(shortname)))
        return false;

    *option = fl_cstring_find(arg, "=");

    if (!*option)
        return false;

    *option += 1;
    return true;
}

static inline bool is_flag_with_bool_value(const char *longname, const char *shortname, const char *arg, bool *option)
{
    if (!arg)
        return false;

    if (!flm_cstring_equals_n(longname, arg, strlen(longname)) && !flm_cstring_equals_n(shortname, arg, strlen(shortname)))
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


/*
 * Function: sbs_command_help
 *  Shows the help message 
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *
 * Returns:
 *  enum SbsResult - It always returns OK
 *
 */
enum SbsResult sbs_command_help(int argc, char **argv, char **env)
{
    print_main_help(argc, argv);
    return SBS_RES_OK;
}

/*
 * Function: sbs_command_init
 *  Creates an empty build file if it doesn't exist
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *
 * Returns:
 * enum SbsResult - OK if the files does not exist and it is created. Error otherwise.
 *
 */
enum SbsResult sbs_command_init(int argc, char **argv, char **env)
{
    if (!fl_io_file_exists(".sbs"))
        fl_io_dir_create(".sbs");

    // Default filename
    const char *build_file = ".sbs/build.sbs";

    // If it exists, we return an error
    if (fl_io_file_exists(build_file))
    {
        print_error("sbs has already been initialized\n");
        return SBS_RES_ERROR;
    }

    // Just a comment
    if (!fl_io_file_write_all_text(build_file, "# Empty"))
    {
        print_error("Could not initialize sbs.");
        return SBS_RES_ERROR;
    }

    // Notify everything it's ok
    fprintf(stdout, "sbs has been initialized. Start editing your %s file\n", build_file);

    return SBS_RES_OK;
}


/*
 * Function: sbs_command_list
 *  Prints a list of all the requested resources available in the build file
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *
 * Returns:
 *  enum SbsResult - Ok if the resources are listed. Error otherwise.
 *
 */
enum SbsResult sbs_command_list(int argc, char **argv, char **env)
{
    // If the are no arguments, print the help message
    if (argc == 2)
    {
        print_list_help(argc, argv);
        return SBS_RES_WRONG_ARGS;
    }

    struct SbsListArguments args = { 0 };
    
    // Skip program name and "list" argument
    char **argsptr = argv+2;
    char *arg = NULL;
    while ((arg = *argsptr++))
    {
        // The resource is a string
        if (arg[0] != '-' && args.resource == NULL)
        {
            args.resource = arg;
            continue;
        }

        if (!is_valid_flag(arg, (const char*[]){ "--help", "-h", "--file", "-f", NULL }))
        {
            print_error("Unknown option '%s'.\n", arg, arg);
            return SBS_RES_WRONG_ARGS;
        }

        // If the help flag is present, show the help message and leave
        if (is_flag_without_value("--help", "-h", arg))
        {
            print_list_help(argc, argv);
            return SBS_RES_OK;
        }

        // Check if the file flag is present in order to override the default file
        bool success = is_flag_with_string_value("--file", "-f", arg, &args.file);

        if (!success)
        {
            print_error("Unknown option '%s'. Did you mean '%s=...'?\n", arg, arg);
            return SBS_RES_WRONG_ARGS;
        }
    }

    // If the resource is not present, show the help message and return an error
    if (args.resource == NULL)
    {
        print_list_help(argc, argv);
        return SBS_RES_MISSING_RESOURCE_ARG;
    }

    // If present the file argument, make sure the filename is valid
    if (args.file && strlen(args.file) == 0)
    {
        print_error("File name cannot be empty\n");
        return SBS_RES_INVALID_FILE;
    }

    // If present the file argument, make sure the file exists
    if (args.file && !fl_io_file_exists(args.file))
    {
        const char *error = sbs_explain_result(SBS_RES_INVALID_FILE, args.file);
        print_error("%s", error);
        fl_cstring_delete(error);
        return SBS_RES_INVALID_FILE;
    }
    
    // Parse the build file to get all the resources
    struct SbsFile *file = sbs_file_parse(args.file ? args.file : ".sbs/build.sbs");

    // Get the requested resource. If the resource name is not valid, show an error.
    const char *resource = NULL;
    FlHashtable ht  = NULL;

    if (flm_cstring_equals("envs", args.resource))
    {
        ht = file->envs;
        resource = "environments";
    }
    else if (flm_cstring_equals("toolchains", args.resource))
    {
        ht = file->toolchains;
        resource = "toolchains";
    }
    else if (flm_cstring_equals("configs", args.resource))
    {
        ht = file->configurations;
        resource = "configurations";
    }
    else if (flm_cstring_equals("actions", args.resource))
    {
        ht = file->actions;
        resource = "actions";
    }
    else if (flm_cstring_equals("targets", args.resource))
    {
        ht = file->targets;
        resource = "targets";
    }
    else if (flm_cstring_equals("presets", args.resource))
    {
        ht = file->presets;
        resource = "presets";
    }
    else
    {
        sbs_file_delete(file);
        print_list_help(argc, argv);
        return SBS_RES_INVALID_RESOURCE;
    }

    // Get all the keys that are also the names
    char **keys = fl_hashtable_keys(ht);

    // If there are not keys there are not items of the requested resource type
    if (!keys)
    {
        fprintf(stdout, "There are no %s in the build file %s\n", resource, file->filename);
        sbs_file_delete(file);
        return SBS_RES_OK;
    }

    // Print the ordered list of requested resources
    qsort(keys, fl_array_length(keys), sizeof(char*), compare_strings); 

    fprintf(stdout, "List of %s in the build file %s\n", resource, file->filename);
    for (size_t i=0; i < fl_array_length(keys); i++)
        fprintf(stdout, "%s\n", keys[i]);
    
    sbs_file_delete(file);

    return SBS_RES_OK;
}

/*
 * Function: sbs_command_build
 *  Runs a build process using the provided arguments
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *
 * Returns:
 *  enum SbsResult - Ok if the build process succeed. Error otherwise
 *
 */
enum SbsResult sbs_command_build(int argc, char **argv, char **env)
{
    // If the are no arguments, print the help message
    if (argc == 2)
    {
        print_build_help(argc, argv);
        return SBS_RES_WRONG_ARGS;
    }

    struct SbsBuildArguments args = { 0 };
    
    // Skip program name and "build" argument
    char **argsptr = argv+2;
    char *arg = NULL;
    while ((arg = *argsptr++))
    {
        // If one of the arguments does not start with a dash, it is a preset name
        if (arg[0] != '-' && args.preset == NULL)
        {
            args.preset = arg;
            continue;
        }

        // If the help flag is present, show the help message and leave
        if (is_flag_without_value("--help", "-h", arg))
        {
            print_build_help(argc, argv);
            return SBS_RES_OK;
        }

        if (!is_valid_flag(arg, build_flags))
        {
            print_error("Unknown option '%s'.\n", arg, arg);
            return SBS_RES_WRONG_ARGS;
        }

        // Process the arguments
        bool success = is_flag_with_string_value("--env", "-e", arg, &args.env)
            || is_flag_with_string_value("--toolchain", "-tc", arg, &args.toolchain)
            || is_flag_with_string_value("--config", "-c", arg, &args.config)
            || is_flag_with_string_value("--target", "-t", arg, &args.target)
            || is_flag_with_string_value("--file", "-f", arg, &args.file);        

        // If the argument is not valid show an error
        if (!success)
        {
            print_error("Option '%s' has a syntax error. Did you mean '%s=...'?\n", arg, arg);
            return SBS_RES_WRONG_ARGS;
        }
    }

    // Check if the processed arguments are valid
    if (args.preset == NULL  && (args.env == NULL || args.toolchain == NULL || args.config == NULL || args.target == NULL))
    {
        print_build_help(argc, argv);
        return SBS_RES_WRONG_ARGS;
    }

    // If present the file argument, make sure the filename is valid
    if (args.file && strlen(args.file) == 0)
    {
        print_error("File name cannot be empty\n");
        return SBS_RES_INVALID_FILE;
    }

    // If the help flag is present, show the help message and leave
    if (args.file && !fl_io_file_exists(args.file))
    {
        const char *error = sbs_explain_result(SBS_RES_INVALID_FILE, args.file);
        print_error("%s", error);
        fl_cstring_delete(error);
        return SBS_RES_INVALID_FILE;
    }
    
    // Parse the build file to get all the resources
    struct SbsFile *file = sbs_file_parse(args.file ? args.file : ".sbs/build.sbs");

    // If the preset argument is present, make sure it exists
    if (args.preset != NULL && !fl_hashtable_has_key(file->presets, args.preset))
        return SBS_RES_INVALID_PRESET;
    
    // Run the build process and leave
    enum SbsResult result = sbs_build_run(file, args);

    sbs_file_delete(file);

    return result;
}

enum SbsResult sbs_command_action(int argc, char **argv, char **env)
{
    return SBS_RES_OK;
}


/*
 * Function: sbs_command_run
 *  Checks the requested command from the arguments array and calls the specific function
 *  to handle the incoming action request
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *
 * Returns:
 *  enum SbsResult - Error if the command is missing or not valid. Otherwise depends on the command's result
 *
 */
enum SbsResult sbs_command_run(int argc, char **argv, char **env)
{
    if (argc == 1)
    {
        // No arguments? Show the help message
        sbs_command_help(argc, argv, env);
        return SBS_RES_ERROR;
    }

    // By now, sbs just accepts 1 flag and it is the help message.
    if (is_flag_without_value("--help", "-h", argv[1]))
        return sbs_command_help(argc, argv, env);

    // If it is not a flag, it needs to be a module
    if (flm_cstring_equals("init", argv[1]))
        return sbs_command_init(argc, argv, env);
    
    if (flm_cstring_equals("list", argv[1]))
        return sbs_command_list(argc, argv, env);
    
    if (flm_cstring_equals("build", argv[1]))
        return sbs_command_build(argc, argv, env);    

    sbs_command_help(argc, argv, env);
    return SBS_RES_ERROR;
}
