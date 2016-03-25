#ifndef CMD_UTILS_HDR
#define CMD_UTILS_HDR

#include <stdbool.h>

#define CMD_LENGTH 65536

char *cmd_alloc(char cmd[]);
void cmd_append_option(char cmd[], char option[], char value[],
                       bool quote);
void cmd_append_key_value_option(char cmd[], char option[],
                                 char key[], char value[]);
void cmd_append_int_option(char cmd[], char option[], int value);
void cmd_append_arg(char cmd[], char arg[], bool quote);
void cmd_append_flag(char cmd[], char flag[]);
void cmd_redirect_stdout(char cmd[], char file_name[]);
void cmd_free(char *cmd);

char *env_get_home_dir(void);
char *env_get_job_id(void);

bool file_exists(char path[]);

#endif
