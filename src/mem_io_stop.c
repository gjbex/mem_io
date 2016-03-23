#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem_io_utils.h"
#include "mem_io_cl_params.h"

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
    mem_io_shutdown(context);
    mem_io_disconnect(context);
    finalizeCL(&params);
    free(password);
    return EXIT_SUCCESS;
}
