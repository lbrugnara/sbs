#ifndef SBS_LANG_ACTION_H
#define SBS_LANG_ACTION_H

#include "parser.h"
#include "expr.h"
#include "command.h"

/*
 * Type: SbsNodeAction
 * 	Represents the commands that belongs to an action section, including
 *  those that are within a for statement
 * 
 * ===== C =====
 * typedef struct
 * {
 * 		char **commands;
 * 		SbsStmtConditional *condition;
 * } SbsNodeAction;
 * 
 */
typedef struct SbsNodeAction {
    SbsCommand **commands;
    SbsExpression *condition;
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
 * 		SbsCommand *before;
 * 		SbsCommand *after;
 * } SbsPropertyActions;
 * 
 */
typedef struct SbsPropertyActions {
    SbsCommand **before;
    SbsCommand **after;
} SbsPropertyActions;

SbsSectionAction* sbs_section_action_parse(SbsParser *parser);
SbsPropertyActions sbs_property_actions_parse(SbsParser *parser);
SbsSectionAction* sbs_section_action_new(const struct FlSlice *name);
SbsNodeAction* sbs_section_action_add_node(SbsSectionAction *action_section);
void sbs_section_action_free(SbsSectionAction *action_section);
void sbs_property_actions_free(SbsPropertyActions *actions);

#endif /* SBS_LANG_ACTION_H */
