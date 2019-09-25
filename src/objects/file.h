#ifndef SBS_OBJECT_FILE_H
#define SBS_OBJECT_FILE_H

#include <fllib.h>

struct SbsFile {
    const char *filename;
    /* FlHashtable<char*, struct SbsEnv*> */
    FlHashtable envs;
    /* FlHashtable<char*, struct SbsTargetSection*> */
    FlHashtable targets;
    /* FlHashtable<char*, struct SbsToolchainSection*> */
    FlHashtable toolchains;
    /* FlHashtable<char*, struct SbsConfigSection*> */
    FlHashtable configurations;
    /* FlHashtable<char*, struct SbsActionSection*> */
    FlHashtable actions;
    /* FlHashtable<char*, struct SbsPreset*> */
    FlHashtable presets;
};

struct SbsFile* sbs_file_parse(const char *filename);
void sbs_file_free(struct SbsFile *build);

#endif /* SBS_OBJECT_FILE_H */
