#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-archive.h"
#include "../lang/command.h"
#include "../utils.h"
#include "../lang/action.h"
#include "../lang/command.h"
#include "../lang/target.h"

SbsTargetArchive* sbs_target_archive_new(const char *name)
{
    SbsTargetArchive *archive_target = fl_malloc(sizeof(SbsTargetArchive));

    archive_target->base.type = SBS_TARGET_ARCHIVE;
    archive_target->base.name = fl_cstring_dup(name);

    return archive_target;
}

void sbs_target_archive_free(SbsTargetArchive *target)
{
    sbs_target_base_free_members((SbsTarget*) target);

    if (target->objects)
        fl_array_free_each_pointer(target->objects, (FlArrayFreeElementFunc) sbs_source_free);

    if (target->output_name)
        fl_cstring_free(target->output_name);

    fl_free(target);
}

SbsTargetArchive* sbs_target_archive_resolve(SbsResolveContext *context, const SbsSectionArchive *archive_section)
{
    SbsTargetArchive *archive_target = sbs_target_archive_new(archive_section->base.name);

    for (size_t i = 0; i < fl_array_length(archive_section->entries); i++)
    {
        const SbsNodeArchive *archive_entry = archive_section->entries[i];

        if (archive_entry->base.condition && !sbs_expression_eval_bool(context->evalctx, archive_entry->base.condition->expr))
            continue;

        sbs_target_base_resolve_members((SbsTarget*) archive_target, (const SbsAbstractNodeTarget*) archive_entry);

        archive_target->output_name = sbs_cstring_set(archive_target->output_name, archive_entry->output_name);
        archive_target->objects = sbs_source_array_extend_from_value_source(archive_target->objects, archive_entry->objects);
    }

    return archive_target;
}
