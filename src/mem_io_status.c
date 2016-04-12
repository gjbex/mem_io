#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

#define CHANNEL_UNUSED 0
#define CHANNEL_OPEN 1
#define CHANNEL_CLOSED 2

int get_channel_status(redisContext *context, char id[], int channel_id,
                       Params *params);

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
    if (params.verbose)
        fprintf(stderr, "# checking status for %d channels\n", nr_channels);
    int nr_unused = 0, nr_open = 0, nr_closed = 0;
    for (int channel_id = 0; channel_id < nr_channels; channel_id++) {
        int status = get_channel_status(context, mem_io_id, channel_id,
                                        &params);
        switch (status) {
            case CHANNEL_UNUSED:
                nr_unused++;
                break;
            case CHANNEL_CLOSED:
                nr_closed++;
                break;
            default:
                nr_open++;
        }
    }
    if (params.details)
        printf("\n");
    printf("summary:\n");
    printf("number of channels: %d\n", nr_channels);
    printf("  closed channels: %d\n", nr_closed);
    printf("  open channels: %d\n", nr_open);
    printf("  unused channels: %d\n", nr_unused);
    mem_io_disconnect(context);
    free(mem_io_id);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

int get_channel_status(redisContext *context, char id[], int channel_id,
                       Params *params) {
    if (params->details)
        printf("%d", channel_id);
    char *status_key = mem_io_create_channel_status_key(id, channel_id);
    if (!mem_io_channel_status_is_set(context, status_key)) {
        if (params->details)
            printf(": unused\n");
        return CHANNEL_UNUSED;
    } else {
        int channel_status;
        if (mem_io_is_channel_open(context, status_key)) {
            if (params->details)
                printf(": open");
            channel_status = CHANNEL_OPEN;
        } else {
            channel_status = CHANNEL_CLOSED;
        }
        if (params->details) {
            char *key = mem_io_create_key(id, channel_id);
            long nr_bytes = mem_io_channel_size(context, key);
            printf(": %ld\n", nr_bytes);
        }
        return channel_status;
    }
    free(status_key);
}
