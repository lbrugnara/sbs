#ifndef SBS_LANG_VARIABLE_H
#define SBS_LANG_VARIABLE_H

typedef struct SbsValueVariable {
    const char *namespace;
    const char *name;
} SbsValueVariable;

void sbs_value_variable_free(SbsValueVariable *variable);

#endif /* SBS_LANG_VARIABLE_H */
