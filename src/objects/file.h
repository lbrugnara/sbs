#ifndef SBS_FILE_H
#define SBS_FILE_H

#include <fllib.h>

struct SbsFile {
    const char *filename;
    /* FlHashtable<char*, struct SbsEnv*> */
    FlHashtable envs;
    /* FlHashtable<char*, struct SbsTarget*> */
    FlHashtable targets;
    /* FlHashtable<char*, struct SbsToolchainSection*> */
    FlHashtable toolchains;
    /* FlHashtable<char*, struct SbsConfigSection*> */
    FlHashtable configurations;
    /* FlHashtable<char*, struct SbsAction*> */
    FlHashtable actions;
    /* FlHashtable<char*, struct SbsPreset*> */
    FlHashtable presets;
};

struct SbsFile* sbs_file_parse(const char *filename);
void sbs_file_delete(struct SbsFile *build);

#endif /* SBS_FILE_H */
