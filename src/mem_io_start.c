#include <err.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cmd_utils.h"
#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

void create_mem_io_conf_file(char mem_io_id[], Params *params,
                             char password[]);
void create_redis_conf_file(char mem_io_id[], Params *params,
                            char password[]);

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
    char *password = mem_io_get_password(&params);
    create_redis_conf_file(mem_io_id, &params, password);
    pid_t pid = fork();
    if (pid == -1)
        errx(FORK_ERROR, "fork failed");
    if (pid == 0) {
        char *cmd = cmd_alloc(params.redis_path);
        char *conf_name = redis_conf_name(mem_io_id);
        cmd_append_arg(cmd, conf_name, false);
        cmd_append_int_option(cmd, "--port", params.port);
        if (params.verbose)
            fprintf(stderr, "executing '%s'\n", cmd);
        int exit_code = system(cmd);
        if (exit_code != 0)
            errx(REDIS_RUN_ERROR, "redis startup ended with %d", exit_code);
        char *db_name = redis_db_name(mem_io_id);
        if (0 != chmod(db_name, S_IRUSR | S_IWUSR))
            err(EXIT_FAILURE, "can not set file permissions on '%s'",
                db_name);
        cmd_free(cmd);
        free(conf_name);
        free(db_name);
    } else {
        while (sleep(params.timeout) > 0);
        redisContext *context = mem_io_connect(params.host, params.port,
                                               params.timeout);
        mem_io_auth(context, password);
        mem_io_set_nr_channels(context, mem_io_id,
                               params.nr_channels);
        mem_io_disconnect(context);
        create_mem_io_conf_file(mem_io_id, &params, password);
    }
    free(mem_io_id);
    free(password);
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

void create_mem_io_conf_file(char mem_io_id[], Params *params,
                             char password[]) {
    char *conf_name = mem_io_conf_name(mem_io_id);
    FILE *conf_fp = fopen(conf_name, "w");
    if (conf_fp == NULL)
        err(EXIT_FAILURE, "can not create '%s'", conf_name);
    if (0 != chmod(conf_name, S_IRUSR | S_IWUSR))
        err(EXIT_FAILURE, "can not set file permissions on '%s'",
            conf_name);
    char *hostname = get_hostname();
    fprintf(conf_fp, "host = '%s'\n", hostname);
    fprintf(conf_fp, "port = %d\n", params->port);
    fprintf(conf_fp, "password = '%s'\n", password);
    fclose(conf_fp);
    free(hostname);
    free(conf_name);
}

char *get_redis_conf_m4_path(Params *params) {
    char exec_path[MAX_LENGTH];
    ssize_t nr_bytes = readlink("/proc/self/exe", exec_path, MAX_LENGTH);
    if (nr_bytes < 0)
        err(EXIT_FAILURE, "can not determine executable path");
    char *dir = dirname(exec_path);
    int str_len = strlen(dir) + 1 + strlen(params->redis_conf_path) +
        1 + strlen(params->redis_conf_m4) + 1;
    char *redis_m4 = (char *) malloc(str_len*sizeof(char));
    if (redis_m4 == NULL)
        errx(EXIT_FAILURE, "can not allocate redis m4 template path");
    snprintf(redis_m4, str_len, "%s/%s/%s", dir, params->redis_conf_path,
             params->redis_conf_m4);
    return redis_m4;
}

void create_redis_conf_file(char mem_io_id[], Params *params,
                            char password[]) {
    char *conf_name = redis_conf_name(mem_io_id);
    char *cmd = cmd_alloc(params->m4_path);
    cmd_append_key_value_option(cmd, "--define",
                                "REDIS_PASSWORD", password);
    char *db_name = redis_db_name(mem_io_id);
    cmd_append_key_value_option(cmd, "--define",
                                "REDIS_DBFILENAME", db_name);
    char *redis_conf_m4 = get_redis_conf_m4_path(params);
    cmd_append_arg(cmd, redis_conf_m4, false);
    cmd_redirect_stdout(cmd, conf_name);
    if (params->verbose)
        fprintf(stderr, "executing '%s'\n", cmd);
    int exit_code = system(cmd);
    if (exit_code != 0)
        errx(M4_RUN_ERROR, "redis started ended with %d", exit_code);
    if (0 != chmod(conf_name, S_IRUSR | S_IWUSR))
        err(EXIT_FAILURE, "can not set file permissions on '%s'",
            conf_name);
    cmd_free(cmd);
    free(conf_name);
    free(db_name);
}
