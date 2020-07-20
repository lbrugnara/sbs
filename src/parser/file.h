#ifndef SBS_PARSER_FILE_H
#define SBS_PARSER_FILE_H

#include <fllib/containers/Hashtable.h>

typedef struct SbsFile {
    const char *filename;
    /* FlHashtable<char*, SbsSectionEnv*> */
    FlHashtable *envs;
    /* FlHashtable<char*, SbsAbstractSectionTarget*> */
    FlHashtable *targets;
    /* FlHashtable<char*, SbsSectionToolchain*> */
    FlHashtable *toolchains;
    /* FlHashtable<char*, SbsSectionConfig*> */
    FlHashtable *configurations;
    /* FlHashtable<char*, SbsSectionAction*> */
    FlHashtable *actions;
    /* FlHashtable<char*, SbsSectionPreset*> */
    FlHashtable *presets;
} SbsFile;

SbsFile* sbs_file_parse(const char *filename);
void sbs_file_free(SbsFile *build);

#endif /* SBS_PARSER_FILE_H */
