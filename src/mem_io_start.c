#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

#define CMD_LEN 1024

int main(int argc, char *argv[]) {
    Params params;
    initCL(&params);
    parseFileCL(&params, config_file_name);
    parseCL(&params, &argc, &argv);
    if (params.verbose)
        dumpCL(stderr, "# ", &params);
    if (params.nr_channels <= 0)
        errx(INVALID_NR_CHANNELS, "invalid number of channels %d",
             params.nr_channels);
    pid_t pid = fork();
    if (pid == -1)
        errx(FORK_ERROR, "fork failed");
    if (pid == 0) {
        char cmd[CMD_LEN];
        snprintf(cmd, CMD_LEN, "%s --port %d %s",
                 params.redis_path, params.port, params.redis_conf);
        int err = system(params.redis_path);
        if (err != 0)
            errx(REDIS_RUN_ERROR, "redis started ended with %d", err);
    } else {
        while (sleep(params.timeout) > 0);
        redisContext *context = mem_io_connect(params.host, params.port,
                                               params.timeout);
        mem_io_auth(context, params.password);
        mem_io_set_nr_channels(context, params.mem_io_id,
                               params.nr_channels);
        mem_io_disconnect(context);
    }
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
