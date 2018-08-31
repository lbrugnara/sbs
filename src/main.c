#include <stdio.h>
#include <fllib.h>
#include "commands.h"

int main(int argc, char **argv, char **env)
{
    return sbs_command_run(argc, argv, env);
}
