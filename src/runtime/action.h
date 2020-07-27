#ifndef SBS_RUNTIME_ACTION_H
#define SBS_RUNTIME_ACTION_H

typedef struct SbsAction {
    char *name;
    char **commands;
} SbsAction;

SbsAction* sbs_action_new(const char *name);
void sbs_action_free(SbsAction *action);
void sbs_action_free_all(SbsAction **actions);
void sbs_action_add_command(SbsAction *action, char *cmd);
void sbs_action_copy(SbsAction **dest, const SbsAction **src);

#endif /* SBS_RUNTIME_ACTION_H */
