/*!
  \file
  \brief Functions for construction system calls, and retrieving specific
         environment variables.
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cmd_utils.h"

/*!
  \brief Create a new command string.
  \param cmd Command to execute using a system call.
  \return char* String representing the new command.
*/
char *cmd_alloc(char cmd[]) {
    char *tmp = (char *) malloc(CMD_LENGTH*sizeof(char));
    if (tmp == NULL)
        errx(EXIT_FAILURE, "can not allocate command buffer");
    strncpy(tmp, cmd, CMD_LENGTH);
    return tmp;
}

/*!
  \brief Append an option to a command.
  \param cmd Command to add the option to.
  \param option Name of the option.
  \param value Value of the option.
  \paarm quote If true, add quotes around option value.
*/
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

/*!
  \brief Add option with integer value to a command.
  \param cmd Command to add the option to.
  \param option Name of the option.
  \param value Value of the option.
*/
void cmd_append_int_option(char cmd[], char option[], int value) {
    char tmp[40];
    snprintf(tmp, 40, "%d", value);
    cmd_append_option(cmd, option, tmp, false);
}

/*!
  \brief Add argument to a command.
  \param cmd Command to add the option to.
  \param value Value of the argument.
  \paarm quote If true, add quotes around argument value.
*/
void cmd_append_arg(char cmd[], char arg[], bool quote) {
    cmd_append_option(cmd, NULL, arg, quote);
}

/*!
  \brief Add flag to a command.
  \param cmd Command to add the option to.
  \param flag Flag to add to the command.
*/
void cmd_append_flag(char cmd[], char flag[]) {
    cmd_append_option(cmd, flag, NULL, false);
}

/*!
  \brief Append a key/value option to a command, i.e., an option in the
         format -a b=c.
  \param cmd Command to add the option to.
  \param option Name of the option.
  \param key Key of the option.
  \param value Value of the option.
  \paarm quote If true, add quotes around option value.
*/
void cmd_append_key_value_option(char cmd[], char option[],
                                 char key[], char value[]) {
    int str_length = strlen(key) + 1 + 1 + strlen(value) + 1 + 1;
    char *key_value = (char *) malloc(str_length*sizeof(char));
    if (key_value == NULL)
        errx(EXIT_FAILURE, "can not allocated key/value string");
    snprintf(key_value, str_length, "%s='%s'", key, value);
    cmd_append_option(cmd, option, key_value, false);
    free(key_value);
}

/*!
  \brief Add standard output rediction.
  \param cmd Command to add the option to.
  \param file_name File name to direct stdout to.
*/
void cmd_redirect_stdout(char cmd[], char file_name[]) {
    cmd_append_option(cmd, ">", file_name, false);
}

/*!
  \brief Free the memory allocated for the command.
  \param cmd Command string to deallocate.
*/
void cmd_free(char *cmd) {
    free(cmd);
}

/*~
  \brief Retrieve the value of the HOME variable.
  \return char* Value of the HOME environment variable, or throw error if
                the latter is not set.
*/
char *env_get_home_dir(void) {
    char var[80] = "HOME";
    char *home_dir = getenv(var);
    if (home_dir == NULL)
        errx(EXIT_FAILURE, "no %s environment variable set", var);
    return home_dir;
}

/*~
  \brief Retrieve the value of the PBS_JOBID variable.
  \return char* Value of the PBS_JOBID environment variable, or NULL if the
                latter is not set.
*/
char *env_get_job_id(void) {
    char var[80] = "PBS_JOBID";
    char *job_id = getenv(var);
    if (job_id != NULL) {
        char *tmp = (char *) malloc((1 + strlen(job_id))*sizeof(char));
        if (tmp == NULL)
            errx(EXIT_FAILURE, "can not allocate job ID");
        strcpy(tmp, job_id);
        return tmp;
    } else {
        return NULL;
    }
}

/*!
  \brief test whether a file exists with the given file name.
  \param path File to test.
  \return bool True if the file exists, false otherwise.
*/
bool file_exists(char path[]) {
    struct stat file_stat;
    int exit_code = stat(path, &file_stat);
    return exit_code == 0;
}
