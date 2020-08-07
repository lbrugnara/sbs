#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target.h"
#include "target-compile.h"
#include "target-archive.h"
#include "target-shared.h"
#include "target-executable.h"
#include "../utils.h"

void sbs_node_target_free_members(SbsAbstractNodeTarget *node)
{
    sbs_property_actions_free(&node->actions);

    if (node->condition)
        sbs_expression_free(node->condition);

    if (node->output_dir)
        fl_cstring_free(node->output_dir);
}

void sbs_section_target_free_members(SbsAbstractSectionTarget *section)
{
    if (section->name)
        fl_cstring_free(section->name);
}

void sbs_section_target_free(SbsAbstractSectionTarget *section)
{
    switch (section->type)
    {
        case SBS_SECTION_TARGET_COMPILE:
            sbs_section_compile_free((SbsSectionCompile*) section);
            break;
        case SBS_SECTION_TARGET_ARCHIVE:
            sbs_section_archive_free((SbsSectionArchive*) section);
            break;
        case SBS_SECTION_TARGET_SHARED:
            sbs_section_shared_free((SbsSectionShared*) section);
            break;
        case SBS_SECTION_TARGET_EXECUTABLE:
            sbs_section_executable_free((SbsSectionExecutable*) section);
            break;
    }
}
