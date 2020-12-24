#ifndef SBS_CLI_H

#define CLI_HEADER "sbs build system (%d.%d.%d)\n\n"

void sbs_cli_print_header(void);
void sbs_cli_print_message(const char *message, ...);
void sbs_cli_print_error(const char *message, ...);

#endif /* SBS_CLI_H */
