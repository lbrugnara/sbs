#ifndef SBS_OBJECT_ACTION_H
#define SBS_OBJECT_ACTION_H

#include "parser.h"

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

void sbs_action_map_init(FlHashtable *actions);
void sbs_action_free(struct SbsAction *action);
struct SbsAction* sbs_action_parse(struct SbsParser *parser);
struct SbsActions sbs_actions_parse(struct SbsParser *parser);

#endif /* SBS_OBJECT_ACTION_H */
