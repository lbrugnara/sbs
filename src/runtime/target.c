#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target.h"
#include "target-compile.h"
#include "target-archive.h"
#include "target-shared.h"
#include "target-executable.h"

void sbs_target_base_free_members(SbsTarget *target)
{
    if (target->name)
        fl_cstring_free(target->name);

    if (target->actions.before)
        fl_array_free_each_pointer(target->actions.before, (FlArrayFreeElementFunc) sbs_command_free);

    if (target->actions.after)
        fl_array_free_each_pointer(target->actions.after, (FlArrayFreeElementFunc) sbs_command_free);

    if (target->output_dir)
        fl_cstring_free(target->output_dir);

    if (target->flags)
        fl_array_free_each_pointer(target->flags, (FlArrayFreeElementFunc) sbs_expr_free_string);
}

void sbs_target_free(SbsTarget *target)
{
    switch (target->type)
    {
        case SBS_TARGET_COMPILE:
            sbs_target_compile_free((SbsTargetCompile*) target);
            break;

        case SBS_TARGET_ARCHIVE:
            sbs_target_archive_free((SbsTargetArchive*) target);
            break;

        case SBS_TARGET_SHARED:
            sbs_target_shared_free((SbsTargetShared*) target);
            break;

        case SBS_TARGET_EXECUTABLE:
            sbs_target_executable_free((SbsTargetExecutable*) target);
            break;
    }
}

void sbs_target_base_resolve_members(SbsTarget *extend, const SbsAbstractNodeTarget *source)
{
    extend->output_dir = sbs_cstring_set(extend->output_dir, source->output_dir);
    extend->actions.before = sbs_command_array_extend(extend->actions.before, source->actions.before);
    extend->actions.after = sbs_command_array_extend(extend->actions.after, source->actions.after);
    extend->flags = sbs_expr_extend_string_array(extend->flags, source->flags);
}

SbsTarget* sbs_target_resolve(SbsResolveContext *context, const char *target_name, const SbsTarget *parent)
{    
    const SbsAbstractSectionTarget *abs_target_section = fl_hashtable_get(context->file->targets, target_name);

    if (!abs_target_section)
        return NULL;

    switch (abs_target_section->type)
    {
        case SBS_TARGET_COMPILE:
            return (SbsTarget*) sbs_target_compile_resolve(context, (SbsSectionCompile*) abs_target_section, parent);

        case SBS_TARGET_ARCHIVE:
            return (SbsTarget*) sbs_target_archive_resolve(context, (SbsSectionArchive*) abs_target_section);

        case SBS_TARGET_SHARED:
            return (SbsTarget*) sbs_target_shared_resolve(context, (SbsSectionShared*) abs_target_section);

        case SBS_TARGET_EXECUTABLE:
            return (SbsTarget*) sbs_target_executable_resolve(context, (SbsSectionExecutable*) abs_target_section);

        default:
            break;
    }

    return NULL;
}
