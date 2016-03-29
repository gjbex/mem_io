#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

int main(int argc, char *argv[]) {
    Params params;
    initCL(&params);
    parseCL(&params, &argc, &argv);
    char *mem_io_id = mem_io_get_id(&params);
    mem_io_merge_params(mem_io_id, &params);
    if (params.verbose) {
        dumpCL(stderr, "# ", &params);
    }
    redisContext *context = mem_io_connect(params.host, params.port,
                                           params.timeout);
    mem_io_auth(context, params.password);
    mem_io_shutdown(context);
    mem_io_disconnect(context);
    free(mem_io_id);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}
