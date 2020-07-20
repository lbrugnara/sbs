#ifndef SBS_TYPES_H
#define SBS_TYPES_H

typedef struct SbsStringOrId {
    enum {
        SBS_STRING,
        SBS_IDENTIFIER
    } type;
    const char *value;
} SbsStringOrId;

typedef struct SbsVariable {
    const char *namespace;
    const char *name;
} SbsVariable;

#endif /* SBS_TYPES_H */
