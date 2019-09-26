#ifndef SBS_PARSER_ACTION_H
#define SBS_PARSER_ACTION_H

#include "parser.h"

/*
 * Type: struct SbsActionNode
 * 	Represents the commands that belongs to an action section, including
 *  those that are within a for statement
 * 
 * ===== C =====
 * struct SbsActionNode
 * {
 * 		char **commands;
 * 		char **for_envs;
 * };
 * 
 */
struct SbsActionNode {
    struct SbsStringOrId *commands;
    char **for_envs;
};

/*
 * Type: struct SbsActionSection
 * 	Represents an action block. Keeps track of its content
 *  through the ordered *FlArray* property *nodes*
 * 
 * ===== C =====
 * struct SbsActionSection
 * {
 * 		const char *name;
 * 		struct SbsActionNode *nodes;
 * };
 * 
 */
struct SbsActionSection {
    const char *name;
    struct SbsActionNode *nodes;
};

/*
 * Type: struct SbsActionsNode
 * 	Represents an "actions" property that is an object that
 *  contains both a *before* and an *after* properties that can
 *  hold action identifiers or command strings
 * 
 * ===== C =====
 * struct SbsActionsNode
 * {
 * 		struct SbsStringOrId *before;
 * 		struct SbsStringOrId *after;
 * };
 * 
 */
struct SbsActionsNode {
    struct SbsStringOrId *before;
    struct SbsStringOrId *after;
};

struct SbsActionSection* sbs_action_section_parse(struct SbsParser *parser);
void sbs_action_section_free(struct SbsActionSection *action);
struct SbsActionsNode sbs_actions_node_parse(struct SbsParser *parser);
void sbs_actions_node_free(struct SbsActionsNode *actions);

#endif /* SBS_PARSER_ACTION_H */
