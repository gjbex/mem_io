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
    char *mem_io_id = mem_io_get_id(&params);
    mem_io_merge_params(mem_io_id, &params);
    if (params.verbose)
        dumpCL(stderr, "# ", &params);
    redisContext *context = mem_io_connect(params.host, params.port,
                                           params.timeout);
    mem_io_auth(context, params.password);
    int nr_channels = mem_io_get_nr_channels(context, mem_io_id);
    if (params.channel_id < 0 || params.channel_id >= nr_channels)
        errx(INVALID_CHANNEL_ERROR, "channel ID %d is invalid",
             params.channel_id);
    char *status_key = mem_io_create_channel_status_key(mem_io_id,
                                                        params.channel_id);
    if (!mem_io_channel_status_is_set(context, status_key) ||
            !mem_io_is_channel_open(context, status_key)) {
        mem_io_open_channel(context, status_key);
        char buffer[BUFF_SIZE];
        char *key = mem_io_create_key(mem_io_id, params.channel_id);
        for (;;) {
            int count = fread(buffer, sizeof(char), BUFF_SIZE, stdin);
            if (count == 0)
                break;
            mem_io_push(context, key, buffer, count);
        }
        mem_io_close_channel(context, status_key);
        free(key);
    } else {
        errx(OPEN_CHANNEL_ERROR, "writing to open channel %d",
             params.channel_id);
    }
    mem_io_disconnect(context);
    free(status_key);
    free(mem_io_id);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
