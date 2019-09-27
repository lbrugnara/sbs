#define _XOPEN_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <fllib.h>
#include "executor.h"

extern char** environ;

enum SbsExecutorType {
    SBS_EXEC_SYSTEM,
    SBS_EXEC_CUSTOM
};

struct SbsExecutor {
    enum SbsExecutorType type;
    struct SbsEnv *env;
};

struct SbsCustomExecutor {
    struct SbsExecutor base;
    FlProcess process;
};

struct SbsCommandDriver {
    // Command's format string where the %s specifier is the actual command to be executed and the %lu is a temporal name
    char *driver_command;
    // Sentinel to find the "start" of the driver's echo in order to get the exit code
    char *sentinel_driver_echo;
    // Sentinel to find the start of the driver's output
    char *sentinel_driver_result;
    // Format string to retrieve the exit code from the sentinel_driver_result using the sscanf function
    char *exit_code_format;
} 
    CmdCommandDriver = {
        .driver_command =           "%s\necho __tmp__%lu__=%%errorlevel%%\n",
        .sentinel_driver_echo =     "echo __tmp__%lu__=%%errorlevel%%",
        .sentinel_driver_result =   "\n__tmp__%lu__=",
        .exit_code_format =         "\n__tmp__%lu__=%%d"
    },
    BashCommandDriver = {
        .driver_command =           "%s\n\necho __tmp__%lu__=$?\n",
        .sentinel_driver_echo =     "echo __tmp__%lu__=$?",
        #ifdef _WIN32
        .sentinel_driver_result =   "\n__tmp__%lu__=",
        #else
        .sentinel_driver_result =   "__tmp__%lu__=",
        #endif
        .exit_code_format =         "\n__tmp__%lu__=%%d"
    },
    PowershellCommandDriver = {
        .driver_command =           "%s\necho __tmp__%lu__=$(If ($lastexitcode) {$lastexitcode} ElseIf ($?) {0} Else {-1})\n",
        .sentinel_driver_echo =     "echo __tmp__%lu__=$(If",
        .sentinel_driver_result =   "\n__tmp__%lu__=",
        .exit_code_format =         "\n__tmp__%lu__=%%d"
    }
;

SbsExecutor sbs_executor_new(struct SbsEnv *env)
{
    flm_assert(env, "Environment cannot be null");

    enum SbsExecutorType type = env->type && !flm_cstring_equals("system", env->type) ? SBS_EXEC_CUSTOM : SBS_EXEC_SYSTEM;

    struct SbsExecutor *executor = NULL;

    if (type == SBS_EXEC_CUSTOM)
    {
        struct SbsCustomExecutor *cexec = fl_malloc(sizeof(struct SbsCustomExecutor));
        
        char **envp = NULL;
        if (env->variables)
        {
            // Make room for the current environment variables
            size_t env_req_length = 0;
            char **curenv = environ;
            while (*curenv++)
                env_req_length++;            

            // Make room for the custom environment variables
            env_req_length += fl_array_length(env->variables);

            // Make room for the last NULL
            env_req_length++;

            envp = fl_malloc(sizeof(char*) * env_req_length);

            size_t i=0;
            curenv = environ;
            while(*curenv)
                envp[i++] = *curenv++;

            for (size_t j=0; j < fl_array_length(env->variables); j++)
                envp[i++] = env->variables[j];

            envp[i] = NULL;
        }

        bool is_bash = flm_cstring_equals_n(env->type, "bash", 4);
        char **argv = NULL;
        if (env->args || is_bash)
        {

            size_t env_args_length = env->args ? fl_array_length(env->args) : 0;
            size_t argv_req_length = env_args_length;

            if (is_bash)
                argv_req_length++;

            // Make room for the last NULL
            argv_req_length++;

            argv = fl_malloc(sizeof(char*) * argv_req_length);

            size_t i=0;
            if (is_bash)
                argv[i++] = "--verbose";

            if (env_args_length > 0)
                for (size_t j=0; j < env_args_length; j++)
                    argv[i++] = env->args[j];

            argv[i] = NULL;
        }

        cexec->process = fl_process_create(env->terminal ? env->terminal : env->type, argv, envp, fl_process_pipe_new(), fl_process_pipe_new(), NULL);

        executor = (struct SbsExecutor*)cexec;
    }
    else
    {
        executor = fl_malloc(sizeof(struct SbsExecutor));

        if (env->variables)
        {
            for (size_t i=0; i < fl_array_length(env->variables); i++)
            {
                #ifdef _WIN32
                _putenv(env->variables[i]);
                #else
                putenv(env->variables[i]);
                #endif
            }
        }
    }
    
    executor->env = env;
    executor->type = type;

    return executor;
}

static unsigned long djb2_hash(const char *key, size_t size)
{
    unsigned long hash = 5381;
    FlByte c;

    for (size_t i=0; i < size; i++)
        hash = ((hash << 5) + hash) + key[i];
    return hash;
}

bool sbs_executor_run_command(SbsExecutor executor, const char *command)
{
    if (executor->type == SBS_EXEC_SYSTEM)
    {
        printf("%s\n", command);
        return system(command) == 0;
    }

    // SBS_EXEC_CUSTOM
    struct SbsCustomExecutor *custom_executor = (struct SbsCustomExecutor*)executor;
    struct SbsCommandDriver *driver = NULL;

    if (flm_cstring_equals_n(custom_executor->base.env->type, "cmd", 3))
        driver = &CmdCommandDriver;
    else if (flm_cstring_equals_n(custom_executor->base.env->type, "bash", 4))
        driver = &BashCommandDriver;
    else if (flm_cstring_equals_n(custom_executor->base.env->type, "powershell", 10))
        driver = &PowershellCommandDriver;
    else
        return false;

    bool success = false;

    unsigned long hash = djb2_hash(command, strlen(command));

    char *driver_command = fl_cstring_vdup(driver->driver_command, command, hash);
    char *sentinel_driver_echo = fl_cstring_vdup(driver->sentinel_driver_echo, hash);
    char *sentinel_driver_result = fl_cstring_vdup(driver->sentinel_driver_result, hash);
    char *exit_code_format = fl_cstring_vdup(driver->exit_code_format, hash);

    if (fl_process_write_to_stdin(custom_executor->process, driver_command, strlen(driver_command)) == 0)
        goto CLEAN;

    #ifdef _WIN32
        printf("%s", command);
    #else
        printf("%s\n", command);
    #endif

    // 
    char *output = NULL;
    // Internal result will hold the actual result the command outputs
    char *result = NULL;
    do
    {
        char *tmp = fl_process_read_from_stdout(custom_executor->process);

        if (!tmp)
            continue;

        if (output == NULL)
        {
            output = tmp;
        }
        else
        {
            fl_cstring_append(&output, tmp);
            fl_cstring_free(tmp);
        }

        if ((result = fl_cstring_find(output, sentinel_driver_result)) != NULL)
            break;

    } while (fl_process_poll_stdout(custom_executor->process, 500, 20));

    int retval = -1;

    if (result != NULL)
    {
        #if defined(_WIN32) && __STDC_WANT_SECURE_LIB__
        success = sscanf_s(result, exit_code_format, &retval) > 0;
        #else
        success = sscanf(result, exit_code_format, &retval) > 0;
        #endif

        // Get the position where the executed command starts at the terminal's output        
        char *actual_output_start = fl_cstring_find(output, command);

        if (actual_output_start)
        {
            // The start of the command's output should start at the
            // output's strlen(command)-nth position
            actual_output_start += strlen(command);

            // In order to remove all the driver's intrinsics, we need to find the first
            // character after the actual command's output, and nullify it for display purposes. 
            // The position where the sentinel_driver_echo starts, is the last position
            // of the actual command's result, while the sentinel_driver_result contains
            // the first position of the driver echo's output and...
            char *driver_echo = fl_cstring_find(output, sentinel_driver_echo);
            char *driver_result = fl_cstring_find(output, sentinel_driver_result);

            // ...we need it because there's an issue with the bash driver when used with the 
            // WSL's bash where the driver's echo comes AFTER the same echo's result, something like:
            //  We send: 
            //      command
            //      echo __tmp__%lu__=$?
            //  We receive: 
            //      ~$ command
            //      command's output
            //      __tmp__%lu__=0
            //      ~$ echo __tmp__%lu__=$?
            // If that case happens, the previous '\0' would change the output to be:
            //      command's output
            //      __tmp__%lu__=0
            // Because of that we need to find the sentinel_driver_result, and use whichever pointer
            // is lesser than the other.
            char *actual_output_end = driver_echo < driver_result ? driver_echo : driver_result;

            if (actual_output_end)
            {
                size_t i=0;

                // The driver_result might start with '\n', let just keep it
                if (actual_output_end[0] == '\n')
                    i++;

                actual_output_end[i] = '\0';
            }

            printf("%s", actual_output_start);
        }
    }

    fl_cstring_free(output);
CLEAN:
    fl_cstring_free(driver_command);
    fl_cstring_free(sentinel_driver_echo);
    fl_cstring_free(sentinel_driver_result);
    fl_cstring_free(exit_code_format);

    return success && retval == 0;
}

void sbs_executor_free(SbsExecutor executor)
{
    if (executor->type == SBS_EXEC_CUSTOM)
    {
        FlProcess process = ((struct SbsCustomExecutor*)executor)->process;
        char **argv = fl_process_argv(process);
        if (argv)
            fl_free(argv);

        char **envp = fl_process_environ(process);
        if (envp)
            fl_free(envp);
        fl_process_free(process);
    }    

    fl_free(executor);
}
