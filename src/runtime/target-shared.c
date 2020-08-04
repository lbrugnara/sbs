#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-shared.h"
#include "../lang/command.h"
#include "../utils.h"
#include "../lang/action.h"
#include "../lang/command.h"
#include "../lang/target.h"

SbsTargetShared* sbs_target_shared_new(const char *name)
{
    SbsTargetShared *shared_target = fl_malloc(sizeof(SbsTargetShared));

    shared_target->base.type = SBS_TARGET_SHARED;
    shared_target->base.name = fl_cstring_dup(name);

    return shared_target;
}

void sbs_target_shared_free(SbsTargetShared *target)
{
    sbs_target_base_free_members((SbsTarget*) target);

    if (target->objects)
        fl_array_free_each(target->objects, (FlArrayFreeElementFunc) sbs_value_source_free);

    if (target->output_name)
        fl_cstring_free(target->output_name);

    fl_free(target);
}

SbsTargetShared* sbs_target_shared_resolve(SbsResolveContext *context, const SbsSectionShared *shared_section)
{
    SbsTargetShared *shared_target = sbs_target_shared_new(shared_section->base.name);

    for (size_t i = 0; i < fl_array_length(shared_section->entries); i++)
    {
        const SbsNodeShared *shared_entry = shared_section->entries[i];

        if (shared_entry->base.condition && !sbs_expression_eval_bool(context->symbols, shared_entry->base.condition->expr))
            continue;

        sbs_target_base_resolve_members((SbsTarget*) shared_target, (const SbsAbstractNodeTarget*) shared_entry);

        shared_target->output_name = sbs_cstring_set(shared_target->output_name, shared_entry->output_name);
        shared_target->objects = sbs_value_source_array_extend(shared_target->objects, shared_entry->objects);
    }

    return shared_target;
}
