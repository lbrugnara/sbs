#define _XOPEN_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/os/Process.h>
#include "executor.h"

extern char** environ;

enum SbsExecutorType {
    SBS_EXEC_SYSTEM,
    SBS_EXEC_CUSTOM
};

struct SbsExecutor {
    enum SbsExecutorType type;
    SbsEnv *env;
    bool script_mode;
};

typedef struct {
    SbsExecutor base;
    FlProcess *process;
} SbsCustomExecutor;

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

static const char* get_shell_command(SbsExecutor *executor)
{
    if (executor->env->shell_command != NULL)
        return executor->env->shell_command;

    switch (executor->env->shell_type)
    {
        case SBS_SHELL_BASH:
            return "bash";

        case SBS_SHELL_CMD:
            return "cmd";

        case SBS_SHELL_POWERSHELL:
            return "powershell";

        default: break;
    }

    return NULL;
}

SbsExecutor* sbs_executor_new(SbsEnv *environment)
{
    if (environment->shell_type == SBS_SHELL_UNK)
        return NULL;

    enum SbsExecutorType type = environment->shell_type == SBS_SHELL_SYSTEM ? SBS_EXEC_SYSTEM : SBS_EXEC_CUSTOM;

    SbsExecutor *executor = NULL;

    if (type == SBS_EXEC_CUSTOM)
    {
        SbsCustomExecutor *cexec = fl_malloc(sizeof(SbsCustomExecutor));
        cexec->base.env = environment;
        
        char **envp = NULL;
        if (cexec->base.env->variables)
        {
            // Make room for the current environment variables
            size_t env_req_length = 0;
            char **curenv = environ;
            while (*curenv++)
                env_req_length++;            

            // Make room for the custom environment variables
            env_req_length += fl_array_length(cexec->base.env->variables);

            // Make room for the last NULL
            env_req_length++;

            envp = fl_malloc(sizeof(char*) * env_req_length);

            size_t i=0;
            curenv = environ;
            while(*curenv)
                envp[i++] = *curenv++;

            for (size_t j=0; j < fl_array_length(cexec->base.env->variables); j++)
                envp[i++] = cexec->base.env->variables[j];

            envp[i] = NULL;
        }

        bool is_bash = cexec->base.env->shell_type == SBS_SHELL_BASH;
        char **argv = NULL;
        if (cexec->base.env->shell_args || is_bash)
        {

            size_t env_args_length = cexec->base.env->shell_args ? fl_array_length(cexec->base.env->shell_args) : 0;
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
                    argv[i++] = cexec->base.env->shell_args[j];

            argv[i] = NULL;
        }

        const char *shell_command = get_shell_command((SbsExecutor*) cexec);

        if (shell_command == NULL)
            return NULL;

        cexec->process = fl_process_create(shell_command, argv, envp, fl_process_pipe_new(), fl_process_pipe_new(), NULL);

        if (!cexec->process)
            return NULL;

        executor = (SbsExecutor*)cexec;
    }
    else
    {
        executor = fl_malloc(sizeof(SbsExecutor));
        executor->env = environment;

        if (executor->env->variables)
        {
            for (size_t i=0; i < fl_array_length(executor->env->variables); i++)
            {
                #ifdef _WIN32
                _putenv(executor->env->variables[i]);
                #else
                putenv(executor->env->variables[i]);
                #endif
            }
        }
    }

    executor->type = type;

    return executor;
}

void sbs_executor_enable_script_mode(SbsExecutor *executor)
{
    executor->script_mode = true;
}

bool sbs_executor_is_script_mode(SbsExecutor *executor)
{
    return executor->script_mode;
}

static unsigned long djb2_hash(const char *key, size_t size)
{
    unsigned long hash = 5381;
    FlByte c;

    for (size_t i=0; i < size; i++)
        hash = ((hash << 5) + hash) + key[i];
    return hash;
}

static struct SbsCommandDriver* get_shell_driver(const SbsExecutor *executor)
{
    if (executor->env->shell_type == SBS_SHELL_BASH)
        return &BashCommandDriver;
    
    if (executor->env->shell_type == SBS_SHELL_CMD)
        return &CmdCommandDriver;

    if (executor->env->shell_type == SBS_SHELL_POWERSHELL)
        return &PowershellCommandDriver;
    
    return NULL;
}

bool sbs_executor_run_command(const SbsExecutor *executor, const char *command)
{
    if (executor->script_mode)
    {
        printf("%s\n", command);
        return true;
    }

    if (executor->type == SBS_EXEC_SYSTEM)
    {
        printf("%s\n", command);
        return system(command) == 0;
    }

    struct SbsCommandDriver *driver = get_shell_driver(executor);

    if (driver == NULL)
        return false;

    // SBS_EXEC_CUSTOM
    SbsCustomExecutor *custom_executor = (SbsCustomExecutor*) executor;

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

    } while (fl_process_poll_stdout(custom_executor->process, 500, 10));

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
    else if (output != NULL)
    {
        output = fl_cstring_replace_realloc(output, sentinel_driver_echo, "");
        printf("%s", output);
    }

    fl_cstring_free(output);
CLEAN:
    fl_cstring_free(driver_command);
    fl_cstring_free(sentinel_driver_echo);
    fl_cstring_free(sentinel_driver_result);
    fl_cstring_free(exit_code_format);

    return success && retval == 0;
}

void sbs_executor_free(SbsExecutor *executor)
{
    if (executor->type == SBS_EXEC_CUSTOM)
    {
        FlProcess *process = ((SbsCustomExecutor*)executor)->process;
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
