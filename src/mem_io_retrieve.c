#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

#define BUFF_SIZE 1024
#define MAX_LENGTH 1024

void retrieve_channel(redisContext *context, char id[], int channel_id,
                      FILE *fp, Params *params, bool print_eol);

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
    if (0 <= params.channel_id) {
        if (params.verbose)
            fprintf(stderr, "# retrieving data from channel %d\n",
                    params.channel_id);
        if (params.channel_id >= nr_channels)
            errx(INVALID_CHANNEL_ERROR, "invalid channel ID %d",
                 params.channel_id);
        retrieve_channel(context, mem_io_id, params.channel_id,
                         stdout, &params, true);
    } else {
        FILE *fp = stdout;
        if (params.verbose)
            fprintf(stderr, "# retrieving data from %d channels\n",
                    nr_channels);
        int width = 1 + (int) log10(nr_channels);
        bool add_eol = !params.split;
        for (int channel_id = 0; channel_id < nr_channels;
                channel_id++) {
            if (params.split) {
                char file_name[MAX_LENGTH];
                snprintf(file_name, MAX_LENGTH, "data_%s_%0*d.dat",
                         mem_io_id, width, channel_id);
                fp = fopen(file_name, "w");
                if (fp == NULL) {
                    err(EXIT_FAILURE, "can not open file '%s' for write",
                        file_name);
                    continue;
                }
            }
            retrieve_channel(context, mem_io_id, channel_id, fp,
                             &params, add_eol);
            if (params.split) {
                fclose(fp);
            }
        }
    }
    mem_io_disconnect(context);
    free(mem_io_id);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

void retrieve_channel(redisContext *context, char id[], int channel_id,
                      FILE *fp, Params *params, bool print_eol) {
    if (params->print_id)
        fprintf(fp, "%d%s", channel_id, params->sep);
    char *status_key = mem_io_create_channel_status_key(id, channel_id);
    if (!mem_io_channel_status_is_set(context, status_key) ||
            mem_io_is_channel_open(context, status_key)) {
        warnx("retrieving from open channel %d, data may be incomplete",
              channel_id);
    }
    char *key = mem_io_create_key(id, channel_id);
    long nr_bytes = mem_io_retrieve(context, key, fp);
    if (nr_bytes == 0)
        warnx("no data in channel %0*d",
                CHANNEL_ID_WIDTH, channel_id);
    else if (print_eol)
        fprintf(fp, "\n");
    free(key);
    free(status_key);
}
