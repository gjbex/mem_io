#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include "cmd_utils.h"
#include "mem_io_cl_params.h"

#define PATH_LENGTH 1024
#define PASSWD_LENGTH 1024

void cli_get_passwd(char password[], int max_length);

int main(int argc, char *argv[]) {
    Params params;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose)
        dumpCL(stdout, "# ", &params);
    char *home_dir = env_get_home_dir();
    char conf_path[PATH_LENGTH];
    snprintf(conf_path, PATH_LENGTH, "%s/%s", home_dir, params.mem_io_conf);
    if (!file_exists(conf_path) || params.force) {
        char passwd[PASSWD_LENGTH];
        cli_get_passwd(passwd, PASSWD_LENGTH);
        if (params.verbose)
            fprintf(stderr, "password '%s' to file '%s'\n", passwd,
                    conf_path);
        FILE *conf_fp = fopen(conf_path, "w");
        if (conf_fp == NULL)
            err(EXIT_FAILURE, "can open file '%s'", conf_path);
        if (0 != chmod(conf_path, S_IRUSR | S_IWUSR))
            err(EXIT_FAILURE, "can set file permissions on '%s'",
                conf_path);
        fprintf(conf_fp, "password = '%s'", passwd);
        fclose(conf_fp);
    } else {
        errx(EXIT_FAILURE,
             "file '%s' already exists,\nuse -force to overwrite",
             conf_path);
    }
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

void cli_get_passwd(char password[], int max_length) {
    struct termios oflags, nflags;
    /* disabling echo */
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        err(EXIT_FAILURE, "can set terminal attributes");

    printf("Enter password: ");
    char *result = fgets(password, max_length, stdin);
    if (result == NULL)
        err(EXIT_FAILURE, "could not read password from stdin");
    password[strlen(password) - 1] = '\0';
    /* restore terminal */
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        err(EXIT_FAILURE, "can restore terminal attributes");
}

