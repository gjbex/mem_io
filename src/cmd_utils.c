#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            snprintf(cmd, CMD_LENGTH, "%s '%s'", tmp, option);
        else
            snprintf(cmd, CMD_LENGTH, "%s %s", tmp, option);
    }
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
