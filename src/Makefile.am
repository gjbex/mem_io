bin_PROGRAMS = mem_io_start mem_io_store mem_io_retrieve mem_io_status \
               mem_io_reset mem_io_stop mem_io_set_password

mem_io_start_SOURCES = mem_io_utils.c mem_io_utils.h \
                       mem_io_cl_params_aux.c mem_io_cl_params_aux.h \
                       mem_io_cl_params.c mem_io_cl_params.h \
                       cmd_utils.c cmd_utils.h \
                       mem_io_start.c
mem_io_start_CFLAGS = -std=c99 -Wall  -D_POSIX_SOURCE -D_BSD_SOURCE
mem_io_start_LDFLAGS = -static
mem_io_start_LDADD = ${hiredis_LIBS}

mem_io_store_SOURCES = mem_io_utils.c mem_io_utils.h \
                       mem_io_cl_params_aux.c mem_io_cl_params_aux.h \
                       mem_io_cl_params.c mem_io_cl_params.h \
                       cmd_utils.c cmd_utils.h \
                       mem_io_store.c
mem_io_store_CFLAGS = -std=c99 -Wall  -D_POSIX_SOURCE -D_BSD_SOURCE
mem_io_store_LDFLAGS = -static
mem_io_store_LDADD = ${hiredis_LIBS}

mem_io_retrieve_SOURCES = mem_io_utils.c mem_io_utils.h \
                          mem_io_cl_params_aux.c mem_io_cl_params_aux.h \
                          mem_io_cl_params.c mem_io_cl_params.h \
                          cmd_utils.c cmd_utils.h \
                          mem_io_retrieve.c
mem_io_retrieve_CFLAGS = -std=c99 -Wall  -D_POSIX_SOURCE -D_BSD_SOURCE
mem_io_retrieve_LDFLAGS = -static
mem_io_retrieve_LDADD = ${hiredis_LIBS}

mem_io_status_SOURCES = mem_io_utils.c mem_io_utils.h \
                        mem_io_cl_params_aux.c mem_io_cl_params_aux.h \
                        mem_io_cl_params.c mem_io_cl_params.h \
                        cmd_utils.c cmd_utils.h \
                        mem_io_status.c
mem_io_status_CFLAGS = -std=c99 -Wall  -D_POSIX_SOURCE -D_BSD_SOURCE
mem_io_status_LDFLAGS = -static
mem_io_status_LDADD = ${hiredis_LIBS}

mem_io_reset_SOURCES = mem_io_utils.c mem_io_utils.h \
                       mem_io_cl_params_aux.c mem_io_cl_params_aux.h \
                       mem_io_cl_params.c mem_io_cl_params.h \
                       cmd_utils.c cmd_utils.h \
                       mem_io_reset.c
mem_io_reset_CFLAGS = -std=c99 -Wall  -D_POSIX_SOURCE -D_BSD_SOURCE
mem_io_reset_LDFLAGS = -static
mem_io_reset_LDADD = ${hiredis_LIBS}

mem_io_stop_SOURCES = mem_io_utils.c mem_io_utils.h \
                      mem_io_cl_params_aux.c mem_io_cl_params_aux.h \
                      mem_io_cl_params.c mem_io_cl_params.h \
                      cmd_utils.c cmd_utils.h \
                      mem_io_stop.c
mem_io_stop_CFLAGS = -std=c99 -Wall  -D_POSIX_SOURCE -D_BSD_SOURCE
mem_io_stop_LDFLAGS = -static
mem_io_stop_LDADD = ${hiredis_LIBS}

mem_io_set_password_SOURCES = mem_io_utils.c mem_io_utils.h \
                      mem_io_cl_params_aux.c mem_io_cl_params_aux.h \
                      mem_io_cl_params.c mem_io_cl_params.h \
                      cmd_utils.c cmd_utils.h \
                      mem_io_set_password.c
mem_io_set_password_CFLAGS = -std=c99 -Wall  -D_POSIX_SOURCE -D_BSD_SOURCE
mem_io_set_password_LDFLAGS = -static
mem_io_set_password_LDADD = ${hiredis_LIBS}

sharedir = $(prefix)/share/memio
dist_share_DATA = mem_io_global.conf redis.conf.m4
