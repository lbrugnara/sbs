#include "commands.h"

#ifndef SBS_TESTS
int main(int argc, char **argv, char **env)
{
    return sbs_command_run(argc, argv, env);
}
#endif
