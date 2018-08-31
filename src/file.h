#ifndef SBS_FILE_H
#define SBS_FILE_H

struct SbsFile {
    const char *filename;
    FlHashtable /* <char*, struct SbsEnv*> */ envs;
    FlHashtable /* <char*, struct SbsTarget*> */ targets;
    FlHashtable /* <char*, struct SbsToolchain*> */ toolchains;
    FlHashtable /* <char*, struct SbsConfiguration*> */ configurations;
    FlHashtable /* <char*, struct SbsAction*> */ actions;
    FlHashtable /* <char*, struct SbsPreset*> */ presets;
};

struct SbsFile* sbs_file_parse(const char *filename);
void sbs_file_delete(struct SbsFile *build);

#endif /* SBS_FILE_H */
