#ifndef SBS_TYPES_H
#define SBS_TYPES_H

struct SbsStringOrId {
    enum {
        SBS_STRING,
        SBS_IDENTIFIER
    } type;
    const char *value;
};

struct SbsActionCommand {
    char **commands;
    char **for_envs;
};

struct SbsAction {
    const char *name;
    struct SbsActionCommand *commands;
};

struct SbsActions {
    struct SbsStringOrId *before;
    struct SbsStringOrId *after;
};

struct SbsEnv {
    const char *name;
    const char *type;
    const char *terminal;
    char **args;
    char **variables;
    struct SbsActions actions;
};

struct SbsToolchain {
    const char *name;
    char **extends;
    char **for_envs;
    const char *compiler;
    const char *linker;
    const char *archiver;
};

struct SbsConfigCompile {
    const char *extension;
    char **flags;
    char **defines;
};

struct SbsConfigArchive {
    const char *extension;
    char **flags;
};

struct SbsConfigShared {
    const char *extension;
    char **flags;
};

struct SbsConfigExecutable {
    const char *extension;
    char **flags;
};

struct SbsConfiguration {
    const char *name;
    char **extends;
    char **for_toolchains;
    struct SbsConfigCompile compile;
    struct SbsConfigArchive archive;
    struct SbsConfigShared shared;
    struct SbsConfigExecutable executable;
};

enum SbsTargetType {
    SBS_TARGET_COMPILE,
    SBS_TARGET_ARCHIVE,
    SBS_TARGET_SHARED,
    SBS_TARGET_EXECUTABLE,
};

struct SbsTarget {
    enum SbsTargetType type;
    const char *name;
    struct SbsActions actions;
    const char *output_dir;
};

struct SbsTargetCompile {
    struct SbsTarget base;
    const char *include_flag;
    char **includes;
    char **sources;
};

struct SbsTargetArchive {
    struct SbsTarget base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetShared {
    struct SbsTarget base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetExecutable {
    struct SbsTarget base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsPreset {
    const char *name;
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    struct SbsActions actions;
};

#endif /* SBS_TYPES_H */
