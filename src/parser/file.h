#ifndef SBS_PARSER_FILE_H
#define SBS_PARSER_FILE_H

#include <fllib.h>

typedef struct {
    const char *filename;
    /* FlHashtable<char*, SbsEnvSection*> */
    FlHashtable *envs;
    /* FlHashtable<char*, SbsTargetSection*> */
    FlHashtable *targets;
    /* FlHashtable<char*, SbsToolchainSection*> */
    FlHashtable *toolchains;
    /* FlHashtable<char*, SbsConfigSection*> */
    FlHashtable *configurations;
    /* FlHashtable<char*, SbsActionSection*> */
    FlHashtable *actions;
    /* FlHashtable<char*, SbsPresetSection*> */
    FlHashtable *presets;
} SbsFile;

SbsFile* sbs_file_parse(const char *filename);
void sbs_file_free(SbsFile *build);

#endif /* SBS_PARSER_FILE_H */
