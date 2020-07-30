#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "configuration.h"

SbsConfiguration* sbs_config_new(const char *name)
{
    SbsConfiguration *configuration = fl_malloc(sizeof(SbsConfiguration));

    configuration->name = fl_cstring_dup(name);

    return configuration;
}

void sbs_config_free(SbsConfiguration *config)
{
    fl_cstring_free(config->name);

    if (config->compile.flags)
        fl_array_free_each_pointer(config->compile.flags, (FlArrayFreeElementFunc) sbs_string_free);

    if (config->compile.extension)
        fl_cstring_free(config->compile.extension);


    if (config->archive.flags)
        fl_array_free_each_pointer(config->archive.flags, (FlArrayFreeElementFunc) fl_cstring_free);

    if (config->archive.extension)
        fl_cstring_free(config->archive.extension);


    if (config->shared.flags)
        fl_array_free_each_pointer(config->shared.flags, (FlArrayFreeElementFunc) fl_cstring_free);
    
    if (config->shared.extension)
        fl_cstring_free(config->shared.extension);


    if (config->executable.flags)
        fl_array_free_each_pointer(config->executable.flags, (FlArrayFreeElementFunc) fl_cstring_free);

    if (config->executable.extension)
        fl_cstring_free(config->executable.extension);

    fl_free(config);
}
