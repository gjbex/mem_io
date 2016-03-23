#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmd_utils.h"
#include "mem_io_cl_params.h"

#define PATH_LENGTH 1024
#define PASSWD_LENGTH 1024

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
        printf("Enter password: ");
        if (1 != scanf("%s", passwd))
            errx(EXIT_FAILURE, "reading password failed");
        if (params.verbose)
            fprintf(stderr, "password '%s' to file '%s'\n", passwd,
                    conf_path);
        FILE *conf_fp = fopen(conf_path, "w");
        if (conf_fp == NULL)
            err(EXIT_FAILURE, "can open file '%s'", conf_path);
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
