#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cmd_utils.h"

char *cmd_alloc(char cmd[]) {
    char *tmp = (char *) malloc(CMD_LENGTH*sizeof(char));
    if (tmp == NULL)
        errx(EXIT_FAILURE, "can not allocate command buffer");
    strncpy(tmp, cmd, CMD_LENGTH);
    return tmp;
}

void cmd_append_option(char cmd[], char option[], char value[],
                       bool quote) {
    int length = strlen(cmd);
    if (option != NULL)
        length += 1 + strlen(option);
    if (value != NULL) {
        length += 1 + strlen(value);
        if (quote)
            length += 2;
    }
    if (length + 1 > CMD_LENGTH)
        errx(EXIT_FAILURE, "command buffer exceeded");
    char tmp[CMD_LENGTH];
    if (option != NULL ) {
        strncpy(tmp, cmd, CMD_LENGTH);
        snprintf(cmd, CMD_LENGTH, "%s %s", tmp, option);
    }
    if (value != NULL) {
        strncpy(tmp, cmd, CMD_LENGTH);
        if (quote)
            snprintf(cmd, CMD_LENGTH, "%s '%s'", tmp, value);
        else
            snprintf(cmd, CMD_LENGTH, "%s %s", tmp, value);
    }
}

void cmd_append_int_option(char cmd[], char option[], int value) {
    char tmp[40];
    snprintf(tmp, 40, "%d", value);
    cmd_append_option(cmd, option, tmp, false);
}

void cmd_append_arg(char cmd[], char arg[], bool quote) {
    cmd_append_option(cmd, NULL, arg, quote);
}

void cmd_append_flag(char cmd[], char flag[]) {
    cmd_append_option(cmd, flag, NULL, false);
}

void cmd_free(char *cmd) {
    free(cmd);
}

char *env_get_home_dir(void) {
    char var[80] = "HOME";
    char *home_dir = getenv(var);
    if (home_dir == NULL)
        errx(EXIT_FAILURE, "no %s environment variable set", var);
    return home_dir;
}

bool file_exists(char path[]) {
    struct stat file_stat;
    int exit_code = stat(path, &file_stat);
    return exit_code == 0;
}
