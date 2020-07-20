#ifndef SBS_PARSER_ACTION_H
#define SBS_PARSER_ACTION_H

#include "helpers.h"
#include "parser.h"
#include "for.h"

/*
 * Type: SbsNodeAction
 * 	Represents the commands that belongs to an action section, including
 *  those that are within a for statement
 * 
 * ===== C =====
 * typedef struct
 * {
 * 		char **commands;
 * 		SbsSectionFor *for_clause;
 * } SbsNodeAction;
 * 
 */
typedef struct SbsNodeAction {
    SbsStringOrId *commands;
    SbsSectionFor *for_clause;
} SbsNodeAction;

/*
 * Type: SbsSectionAction
 * 	Represents an action block. Keeps track of its content
 *  through the ordered *FlArray* property *nodes*
 * 
 * ===== C =====
 * typedef struct
 * {
 * 		const char *name;
 * 		SbsNodeAction *nodes;
 * } SbsSectionAction;
 * 
 */
typedef struct SbsSectionAction {
    const char *name;
    SbsNodeAction **nodes;
} SbsSectionAction;

/*
 * Type: SbsPropertyActions
 * 	Represents an "actions" property that is an object that
 *  contains both a *before* and an *after* properties that can
 *  hold action identifiers or command strings
 * 
 * ===== C =====
 * typedef struct
 * {
 * 		SbsStringOrId *before;
 * 		SbsStringOrId *after;
 * } SbsPropertyActions;
 * 
 */
typedef struct SbsPropertyActions {
    SbsStringOrId *before;
    SbsStringOrId *after;
} SbsPropertyActions;

SbsSectionAction* sbs_section_action_parse(SbsParser *parser);
void sbs_section_action_free(SbsSectionAction *action);
SbsPropertyActions sbs_property_actions_parse(SbsParser *parser);
void sbs_property_actions_free(SbsPropertyActions *actions);

#endif /* SBS_PARSER_ACTION_H */
