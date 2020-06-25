#ifndef SBS_PARSER_ACTION_H
#define SBS_PARSER_ACTION_H

#include "common.h"
#include "parser.h"

/*
 * Type: SbsActionNode
 * 	Represents the commands that belongs to an action section, including
 *  those that are within a for statement
 * 
 * ===== C =====
 * typedef struct
 * {
 * 		char **commands;
 * 		char **for_envs;
 * } SbsActionNode;
 * 
 */
typedef struct {
    SbsStringOrId *commands;
    char **for_envs;
} SbsActionNode;

/*
 * Type: SbsActionSection
 * 	Represents an action block. Keeps track of its content
 *  through the ordered *FlArray* property *nodes*
 * 
 * ===== C =====
 * typedef struct
 * {
 * 		const char *name;
 * 		SbsActionNode *nodes;
 * } SbsActionSection;
 * 
 */
typedef struct {
    const char *name;
    SbsActionNode *nodes;
} SbsActionSection;

/*
 * Type: SbsActionsNode
 * 	Represents an "actions" property that is an object that
 *  contains both a *before* and an *after* properties that can
 *  hold action identifiers or command strings
 * 
 * ===== C =====
 * typedef struct
 * {
 * 		SbsStringOrId *before;
 * 		SbsStringOrId *after;
 * } SbsActionsNode;
 * 
 */
typedef struct {
    SbsStringOrId *before;
    SbsStringOrId *after;
} SbsActionsNode;

SbsActionSection* sbs_action_section_parse(SbsParser *parser);
void sbs_action_section_free(SbsActionSection *action);
SbsActionsNode sbs_actions_node_parse(SbsParser *parser);
void sbs_actions_node_free(SbsActionsNode *actions);

#endif /* SBS_PARSER_ACTION_H */
