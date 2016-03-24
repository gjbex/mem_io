#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

#define BUFF_SIZE 1024

void retrieve_channel(redisContext *context, char id[], int channel_id);

int main(int argc, char *argv[]) {
    Params params;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    if (params.verbose)
        dumpCL(stderr, "# ", &params);
    char *password = mem_io_get_password(&params);
    char *mem_io_id = mem_io_get_id(&params);
    redisContext *context = mem_io_connect(params.host, params.port,
                                           params.timeout);
    mem_io_auth(context, password);
    int nr_channels = mem_io_get_nr_channels(context, mem_io_id);
    if (0 <= params.channel_id) {
        if (params.channel_id >= nr_channels)
            errx(INVALID_CHANNEL_ERROR, "invalid channel ID %d",
                 params.channel_id);
        retrieve_channel(context, mem_io_id, params.channel_id);
    } else {
        for (int channel_id = 0; channel_id < nr_channels;
                channel_id++) {
            retrieve_channel(context, mem_io_id, channel_id);
        }
    }
    mem_io_disconnect(context);
    free(password);
    free(mem_io_id);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

void retrieve_channel(redisContext *context, char id[], int channel_id) {
    char *key = mem_io_create_key(id, channel_id);
    long nr_bytes = mem_io_retrieve(context, key, stdout);
    if (nr_bytes == 0)
        warnx("no data in channel %0*d",
                CHANNEL_ID_WIDTH, channel_id);
    free(key);
}
