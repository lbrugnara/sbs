#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-compile.h"
#include "target-executable.h"
#include "../lang/command.h"
#include "../utils.h"
#include "../lang/action.h"
#include "../lang/command.h"
#include "../lang/target.h"

SbsTargetCompile* sbs_target_compile_new(const char *name)
{
    SbsTargetCompile *compile_target = fl_malloc(sizeof(SbsTargetCompile));

    compile_target->base.type = SBS_TARGET_COMPILE;
    compile_target->base.name = fl_cstring_dup(name);

    return compile_target;
}

void sbs_target_compile_free(SbsTargetCompile *target)
{
    sbs_target_base_free_members((SbsTarget*) target);

    if (target->sources)
        fl_array_free_each_pointer(target->sources, (FlArrayFreeElementFunc) fl_cstring_free);

    if (target->includes)
        fl_array_free_each_pointer(target->includes, (FlArrayFreeElementFunc) fl_cstring_free);

    if (target->defines)
        fl_array_free_each_pointer(target->defines, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(target);
}

SbsTargetCompile* sbs_target_compile_resolve(SbsResolveContext *context, const SbsSectionCompile *compile_section, const SbsTarget *parent)
{
    SbsTargetCompile *compile_target = sbs_target_compile_new(compile_section->base.name);

    for (size_t i = 0; i < fl_array_length(compile_section->entries); i++)
    {
        const SbsNodeCompile *compile_entry = compile_section->entries[i];

        if (compile_entry->base.condition && !sbs_eval_expr_to_bool(context->evalctx, compile_entry->base.condition))
            continue;

        sbs_target_base_resolve_members((SbsTarget*) compile_target, (const SbsAbstractNodeTarget*) compile_entry);

        compile_target->includes = sbs_cstring_array_extend(compile_target->includes, compile_entry->includes);
        compile_target->sources = sbs_cstring_array_extend(compile_target->sources, compile_entry->sources);
        compile_target->defines = sbs_cstring_array_extend(compile_target->defines, compile_entry->defines);

        if (parent == NULL)
            continue;

        if (parent->type == SBS_TARGET_EXECUTABLE)
        {
            SbsTargetExecutable *executable = (SbsTargetExecutable*) parent;
            compile_target->defines = sbs_cstring_array_extend(compile_target->defines, executable->defines);
        }
    }

    return compile_target;
}
