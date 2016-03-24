#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd_utils.h"
#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

void create_conf_file(char mem_io_id[], Params *params, char password[]);

int main(int argc, char *argv[]) {
    Params params;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose)
        dumpCL(stderr, "# ", &params);
    if (params.nr_channels <= 0)
        errx(INVALID_NR_CHANNELS, "invalid number of channels %d",
             params.nr_channels);
    char *mem_io_id = mem_io_get_id(&params);
    pid_t pid = fork();
    if (pid == -1)
        errx(FORK_ERROR, "fork failed");
    if (pid == 0) {
        char *cmd = cmd_alloc(params.redis_path);
        cmd_append_arg(cmd, params.redis_conf, false);
        cmd_append_int_option(cmd, "--port", params.port);
        if (params.verbose)
            fprintf(stderr, "executing '%s'\n", cmd);
        int err = system(cmd);
        cmd_free(cmd);
        if (err != 0)
            errx(REDIS_RUN_ERROR, "redis started ended with %d", err);
    } else {
        char *password = mem_io_get_password(&params);
        while (sleep(params.timeout) > 0);
        redisContext *context = mem_io_connect(params.host, params.port,
                                               params.timeout);
        mem_io_auth(context, password);
        mem_io_set_nr_channels(context, mem_io_id,
                               params.nr_channels);
        mem_io_disconnect(context);
        create_conf_file(mem_io_id, &params, password);
        free(password);
    }
    free(mem_io_id);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

#define MAX_LENGTH 1024

char *get_hostname(void) {
    char *hostname = (char *) malloc(MAX_LENGTH*sizeof(char));
    if (hostname == NULL)
        errx(ALLOC_ERROR, "can not allocate hostname");
    int exit_code = gethostname(hostname, MAX_LENGTH);
    if (exit_code != 0)
        errx(HOSTNAME_ERROR, "can not determine hostname");
    return hostname;
}

void create_conf_file(char mem_io_id[], Params *params, char password[]) {
    char *conf_name = mem_io_conf_name(mem_io_id);
    FILE *conf_fp = fopen(conf_name, "w");
    if (conf_fp == NULL)
        err(EXIT_FAILURE, "can not create '%s'", conf_name);
    char *hostname = get_hostname();
    fprintf(conf_fp, "host = '%s'\n", hostname);
    fprintf(conf_fp, "port = %d\n", params->port);
    fprintf(conf_fp, "password = '%s'\n", password);
    fclose(conf_fp);
    free(hostname);
    free(conf_name);
}
