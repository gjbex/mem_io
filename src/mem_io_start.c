#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd_utils.h"
#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

int main(int argc, char *argv[]) {
    Params params;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    char *password = mem_io_get_password(&params);
    if (params.verbose)
        dumpCL(stderr, "# ", &params);
    if (params.nr_channels <= 0)
        errx(INVALID_NR_CHANNELS, "invalid number of channels %d",
             params.nr_channels);
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
        while (sleep(params.timeout) > 0);
        redisContext *context = mem_io_connect(params.host, params.port,
                                               params.timeout);
        mem_io_auth(context, password);
        mem_io_set_nr_channels(context, params.mem_io_id,
                               params.nr_channels);
        mem_io_disconnect(context);
    }
    free(password);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
