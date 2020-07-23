#ifndef SBS_PARSER_ACTION_H
#define SBS_PARSER_ACTION_H

#include "../action.h"
#include "parser.h"

SbsSectionAction* sbs_section_action_parse(SbsParser *parser);
SbsPropertyActions sbs_property_actions_parse(SbsParser *parser);

#endif /* SBS_PARSER_ACTION_H */
