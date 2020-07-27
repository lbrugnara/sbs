#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target.h"
#include "../lang/command.h"

SbsTargetCompile* sbs_target_compile_new(const char *name)
{
    SbsTargetCompile *compile_target = fl_malloc(sizeof(SbsTargetCompile));

    compile_target->type = SBS_TARGET_COMPILE;
    compile_target->name = fl_cstring_dup(name);

    return compile_target;
}

SbsTargetArchive* sbs_target_archive_new(const char *name)
{
    SbsTargetArchive *archive_target = fl_malloc(sizeof(SbsTargetArchive));

    archive_target->type = SBS_TARGET_ARCHIVE;
    archive_target->name = fl_cstring_dup(name);

    return archive_target;
}

SbsTargetShared* sbs_target_shared_new(const char *name)
{
    SbsTargetShared *shared_target = fl_malloc(sizeof(SbsTargetShared));

    shared_target->type = SBS_TARGET_SHARED;
    shared_target->name = fl_cstring_dup(name);

    return shared_target;
}

SbsTargetExecutable* sbs_target_executable_new(const char *name)
{
    SbsTargetExecutable *executable_target = fl_malloc(sizeof(SbsTargetExecutable));

    executable_target->type = SBS_TARGET_EXECUTABLE;
    executable_target->name = fl_cstring_dup(name);

    return executable_target;
}

static void free_library(void *obj)
{
    if (!obj)
        return;

    SbsTargetLibrary *lib = (SbsTargetLibrary*)obj;

    if (lib->name)
        fl_cstring_free(lib->name);

    if (lib->path)
        fl_cstring_free(lib->path);
}

void sbs_target_free(SbsTarget *target)
{
    if (target->name)
        fl_cstring_free(target->name);

    if (target->actions.before)
        fl_array_free_each(target->actions.before, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (target->actions.after)
        fl_array_free_each(target->actions.after, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (target->output_dir)
        fl_cstring_free(target->output_dir);

    switch (target->type)
    {
        case SBS_TARGET_COMPILE:
        {
            SbsTargetCompile *compile = (SbsTargetCompile*)target;
            
            if (compile->sources)
                fl_array_free_each_pointer(compile->sources, (FlArrayFreeElementFunc) fl_cstring_free);

            if (compile->includes)
                fl_array_free_each_pointer(compile->includes, (FlArrayFreeElementFunc) fl_cstring_free);

            if (compile->defines)
                fl_array_free_each_pointer(compile->defines, (FlArrayFreeElementFunc) fl_cstring_free);

            break;
        }
        case SBS_TARGET_ARCHIVE:
        {
            SbsTargetArchive *archive = (SbsTargetArchive*)target;

            if (archive->objects)
                fl_array_free_each(archive->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (archive->output_name)
                fl_cstring_free(archive->output_name);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            SbsTargetShared *shared = (SbsTargetShared*)target;

            if (shared->objects)
                fl_array_free_each(shared->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (shared->output_name)
                fl_cstring_free(shared->output_name);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            SbsTargetExecutable *executable = (SbsTargetExecutable*)target;

            if (executable->objects)
                fl_array_free_each(executable->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (executable->libraries)
                fl_array_free_each(executable->libraries, free_library);

            if (executable->output_name)
                fl_cstring_free(executable->output_name);

            if (executable->defines)
                fl_array_free_each_pointer(executable->defines, (FlArrayFreeElementFunc) fl_cstring_free);

            break;
        }
    }

    fl_free(target);
}
