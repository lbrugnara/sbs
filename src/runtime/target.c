#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target.h"
#include "../lang/command.h"

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
