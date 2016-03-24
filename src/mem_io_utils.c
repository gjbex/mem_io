#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mem_io_errors.h"
#include "mem_io_utils.h"
#include "cmd_utils.h"

#define MAX_LENGTH 1024

redisContext *mem_io_connect(char host[], int port, int timeout_secs) {
    struct timeval timeout = {timeout_secs, 0};
    redisContext *context = redisConnectWithTimeout(host, port, timeout);
    if (context == NULL || context->err) {
        if (context != NULL)
            errx(context->err, "connection failed: %s", context->errstr);
        else
            errx(ALLOC_ERROR, "can't allocate context");
    }
    return context;
}

void mem_io_auth(redisContext *context, char password[]) {
    redisReply *reply = redisCommand(context, "AUTH %b",
                                     password, strlen(password));
    if (reply->type == REDIS_REPLY_ERROR)
        errx(AUTH_ERROR, "AUTH failed: %s", reply->str);
    freeReplyObject(reply);
}

void mem_io_set_nr_channels(redisContext *context, char id[],
                            int nr_channels) {
    char *key = mem_io_create_meta_key(id, "nr_channels");
    redisReply *reply = redisCommand(context, "SET %b %d",
                                     key, strlen(key), nr_channels);
    if (reply->type == REDIS_REPLY_ERROR)
        errx(SET_ERROR, "SET for '%s' failed: %s", key, reply->str);
    freeReplyObject(reply);
}

int mem_io_get_nr_channels(redisContext *context, char id[]) {
    char *key = mem_io_create_meta_key(id, "nr_channels");
    redisReply *reply = redisCommand(context, "GET %b", key, strlen(key));
    if (reply->type == REDIS_REPLY_ERROR)
        errx(GET_ERROR, "GET for '%s' failed: %s", key, reply->str);
    int value = atoi(reply->str);
    freeReplyObject(reply);
    return value;
}

void mem_io_push(redisContext *context, char key[],
                 char *value, int length) {
    redisReply *reply = redisCommand(context, "RPUSH %b %b",
                                     key, strlen(key),
                                     value, length);
    if (reply->type == REDIS_REPLY_ERROR)
        errx(RPUSH_ERROR, "RPUSH to '%s' failed: %s", key, reply->str);
    freeReplyObject(reply);
}

void mem_io_print_type(int type) {
    if (type == REDIS_REPLY_INTEGER)
        printf("integer\n");
    else if (type == REDIS_REPLY_STRING)
        printf("string\n");
    else if (type == REDIS_REPLY_ARRAY)
        printf("array\n");
    else if (type == REDIS_REPLY_ERROR)
        printf("error\n");
    else
        printf("unknown type\n");
}

long mem_io_retrieve(redisContext *context, char key[], FILE *fp) {
    redisReply *reply = redisCommand(context, "LLEN %b",
                                     key, strlen(key));
    if (reply->type == REDIS_REPLY_ERROR)
        errx(LLEN_ERROR, "LLEN for '%s' failed: %s", key, reply->str);
    long long list_length = reply->integer;
    freeReplyObject(reply);
    long nr_bytes = 0;
    for (long long i = 0; i < list_length; i++) {
        reply = redisCommand(context, "LINDEX %b %lld",
                             key, strlen(key), i);
        if (reply->type == REDIS_REPLY_ERROR)
            errx(LINDEX_ERROR, "LINDEX for '%s' at %lld failed: %s",
                 key, i, reply->str);
        fwrite(reply->str, sizeof(char), reply->len, fp);
        nr_bytes += reply->len;
        freeReplyObject(reply);
    }
    return nr_bytes;
}

void mem_io_shutdown(redisContext *context) {
    redisReply *reply = redisCommand(context, "SHUTDOWN");
    freeReplyObject(reply);
}

void mem_io_disconnect(redisContext *context) {
    redisFree(context);
}

char *mem_io_create_key(char id[], int channel_id) {
    char qualifier[80] = ":data:";
    int key_length = strlen(id) + strlen(qualifier) + CHANNEL_ID_WIDTH + 1;
    char *key = (char *) malloc(key_length*sizeof(char));
    if (key == NULL)
        errx(ALLOC_ERROR, "can int allocate key of length %d", key_length);
    snprintf(key,  key_length, "%s%s%0*d", id, qualifier,
             CHANNEL_ID_WIDTH, channel_id);
    return key;
}

char *mem_io_get_conf_path(char file_name[]) {
    char *home_dir = env_get_home_dir();
    int path_length = strlen(home_dir) + 1 + strlen(file_name) + 1;
    char *conf_path = (char *) malloc(path_length*sizeof(char));
    snprintf(conf_path, path_length, "%s/%s", home_dir, file_name);
    return conf_path;
}

char *mem_io_create_meta_key(char id[], char spec[]) {
    char qualifier[80] = ":meta:";
    int key_length = strlen(id) + strlen(qualifier) + strlen(spec) + 1;
    char *key = (char *) malloc(key_length*sizeof(char));
    if (key == NULL)
        errx(ALLOC_ERROR, "can not allocate key of length %d", key_length);
    snprintf(key,  key_length, "%s%s%s", id, qualifier, spec);
    return key;
}

char *mem_io_get_password(Params *params) {
    size_t password_length = strlen(params->password) + 1;
    if (password_length != 1) {
        char *tmp = (char *) malloc(password_length*sizeof(char));
        if (tmp == NULL)
            errx(ALLOC_ERROR, "can not allocate password string");
        strncpy(tmp, params->password, password_length);
        return tmp;
    } else {
        char *conf_path = mem_io_get_conf_path(params->mem_io_conf);
        if (file_exists(conf_path)) {
            char *tmp = (char *) malloc(MAX_LENGTH*sizeof(char));
            bool read_password = false;
            FILE *conf_fp = fopen(conf_path, "r");
            while (!feof(conf_fp)) {
                if (1 == fscanf(conf_fp, "password = '%s'", tmp)) {
                    // the final quote will be part of tmp, so overwrite it
                    tmp[strlen(tmp) - 1] = '\0';
                    read_password = true;
                    break;
                }
            }
            fclose(conf_fp);
            if (read_password)
                return tmp;
            else
                errx(AUTH_ERROR,
                        "no password set, you do not want to do this");
        } else {
            errx(AUTH_ERROR, "no password set, you do not want to do this");
        }
    }
}

#define DEFAULT "default"

char *mem_io_get_id(Params *params) {
    size_t id_length = strlen(params->mem_io_id) + 1;
    if (id_length != 1) {
        char *tmp = (char *) malloc(id_length*sizeof(char));
        if (tmp == NULL)
            errx(ALLOC_ERROR, "can not allocate mem_io ID string");
        strncpy(tmp, params->mem_io_id, id_length);
        return tmp;
    } else {
        char *job_id = env_get_job_id();
        if (job_id != NULL) {
            return job_id;
        } else {
            char *id = (char *) malloc((strlen(DEFAULT) + 1)*sizeof(char));
            if (id == NULL)
                errx(ALLOC_ERROR, "can allocate default mem_io ID");
            strcpy(id, DEFAULT);
            return id;
        }
    }
}

char *conf_name(char prefix[], char mem_io_id[], char ext[]) {
    int name_length = strlen(prefix) + strlen(mem_io_id) + strlen(ext) + 1;
    char *conf_name = (char *) malloc(name_length*sizeof(char));
    if (conf_name == NULL)
        errx(ALLOC_ERROR, "can not allocate configuration file name");
    snprintf(conf_name, name_length, "%s%s%s", prefix, mem_io_id, ext);
    return conf_name;
}

#define MEM_IO_CONF_PREFIX "mem_io_"
#define MEM_IO_CONF_EXT ".conf"

char *mem_io_conf_name(char mem_io_id[]) {
    return conf_name(MEM_IO_CONF_PREFIX, mem_io_id, MEM_IO_CONF_EXT);
}

#define REDIS_CONF_PREFIX "mem_io_"
#define REDIS_CONF_EXT ".conf"

char *redis_conf_name(char mem_io_id[]) {
    return conf_name(REDIS_CONF_PREFIX, mem_io_id, REDIS_CONF_EXT);
}
