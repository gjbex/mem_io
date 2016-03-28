#ifndef __MEM_IO_UTILS_HDR__
#define __MEM_IO_UTILS_HDR__

#include <hiredis/hiredis.h>

#include "mem_io_errors.h"
#include "mem_io_cl_params.h"

#define CHANNEL_ID_WIDTH 9
#define DEFAULT_MEM_IO "default"

redisContext *mem_io_connect(char host[], int port, int timeout_secs);
void mem_io_auth(redisContext *context, char password[]);
void mem_io_set_nr_channels(redisContext *context, char id[],
                            int nr_channels);
int mem_io_get_nr_channels(redisContext *context, char id[]);
void mem_io_push(redisContext *context, char key[],
                 char *value, int length);
long mem_io_retrieve(redisContext *context, char key[], FILE *fp);
void mem_io_shutdown(redisContext *context);
void mem_io_disconnect(redisContext *context);
char *mem_io_create_key(char id[], int channel_id);
char *mem_io_create_meta_key(char id[], char spec[]);
char *mem_io_get_password(Params *params);
char *mem_io_get_id(Params *params);
char *mem_io_conf_name(char mem_io_id[]);
char *redis_conf_name(char mem_io_id[]);
char *redis_db_name(char mem_io_id[]);

#endif
