#ifndef CMD_UTILS_HDR
#define CMD_UTILS_HDR

#define CMD_LENGTH 65536

char *cmd_alloc(char cmd[]);
void cmd_append_option(char cmd[], char option[], char value[],
                       bool quote);
void cmd_append_arg(char cmd[], char arg[], bool quote);
void cmd_append_flag(char cmd[], char flag[]);
void cmd_free(char *cmd);

#endif
