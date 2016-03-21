#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

int main(int argc, char *argv[]) {
    Params params;
    initCL(&params);
    parseFileCL(&params, config_file_name);
    parseCL(&params, &argc, &argv);
    if (params.verbose)
        dumpCL(stderr, "# ", &params);
    redisContext *context = mem_io_connect(params.host, params.port,
                                           params.timeout);
    mem_io_auth(context, params.password);
    mem_io_shutdown(context);
    mem_io_disconnect(context);
    finalizeCL(&params);
    return EXIT_SUCCESS;
}

