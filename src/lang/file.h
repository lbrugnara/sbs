#ifndef SBS_LANG_FILE_H
#define SBS_LANG_FILE_H

#include <fllib/containers/Hashtable.h>

typedef struct SbsFile {
    const char *filename;
    // FlHashtable<char*, SbsSectionEnv*>
    FlHashtable *envs;
    // FlHashtable<char*, SbsAbstractSectionTarget*>
    FlHashtable *targets;
    // FlHashtable<char*, SbsSectionToolchain*>
    FlHashtable *toolchains;
    // FlHashtable<char*, SbsSectionConfig*>
    FlHashtable *configurations;
    // FlHashtable<char*, SbsSectionAction*>
    FlHashtable *actions;
    // FlHashtable<char*, SbsSectionPreset*>
    FlHashtable *presets;
    // FlHashtable<char*, SbsNodeVariableDefinition*>
    FlHashtable *variables;
} SbsFile;

SbsFile* sbs_file_new(const char *filename);
void sbs_file_free(SbsFile *file);

#endif /* SBS_LANG_FILE_H */
