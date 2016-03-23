#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

#define BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    Params params;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    char *password = mem_io_get_password(&params);
    if (params.verbose)
        dumpCL(stderr, "# ", &params);
    redisContext *context = mem_io_connect(params.host, params.port,
                                           params.timeout);
    mem_io_auth(context, password);
    int nr_channels = mem_io_get_nr_channels(context, params.mem_io_id);
    if (params.channel_id < 0 || params.channel_id >= nr_channels)
        errx(INVALID_CHANNEL_ERROR, "channel ID %d is invalid",
             params.channel_id);
    char buffer[BUFF_SIZE];
    char *key = mem_io_create_key(params.mem_io_id, params.channel_id);
    for (;;) {
        int count = fread(buffer, sizeof(char), BUFF_SIZE, stdin);
        if (count == 0)
            break;
        mem_io_push(context, key, buffer, count);
    }
    mem_io_disconnect(context);
    free(password);
    free(key);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
