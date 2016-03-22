#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mem_io_errors.h"
#include "mem_io_utils.h"

char config_file_name[1024] = ".mem_io.conf";

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
    if (reply->len != 2 || strncmp(reply->str, "OK", reply->len))
        errx(AUTH_ERROR, "AUTH failed: %s", reply->str);
    freeReplyObject(reply);
}

void mem_io_set_nr_channels(redisContext *context, char id[],
                            int nr_channels) {
    char *key = mem_io_create_meta_key(id, "nr_channels");
    redisReply *reply = redisCommand(context, "SET %b %d",
                                     key, strlen(key), nr_channels);
    if (reply->len != 2 || strncmp(reply->str, "OK", reply->len))
        errx(AUTH_ERROR, "SET for '%s' failed: %s", key, reply->str);
    freeReplyObject(reply);
}

int mem_io_get_nr_channels(redisContext *context, char id[]) {
    char *key = mem_io_create_meta_key(id, "nr_channels");
    redisReply *reply = redisCommand(context, "GET %b", key, strlen(key));
    int value = atoi(reply->str);
    freeReplyObject(reply);
    return value;
}

void mem_io_push(redisContext *context, char key[],
                 char *value, int length) {
    redisReply *reply = redisCommand(context, "RPUSH %b %b",
                                     key, strlen(key),
                                     value, length);
    if (reply->len != 0) 
        errx(PUSH_ERROR, "RPUSH to '%s' failed", key);
    freeReplyObject(reply);
}

long mem_io_retrieve(redisContext *context, char key[], FILE *fp) {
    redisReply *reply = redisCommand(context, "LLEN %b",
                                     key, strlen(key));
    long long list_length = reply->integer;
    freeReplyObject(reply);
    long nr_bytes = 0;
    for (long long i = 0; i < list_length; i++) {
        reply = redisCommand(context, "LINDEX %b %ld", key, i);
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
        errx(ALLOC_ERROR, "can allocate key of length %d", key_length);
    snprintf(key,  key_length, "%s%s%0*d", id, qualifier,
             CHANNEL_ID_WIDTH, channel_id);
    return key;
}

char *mem_io_create_meta_key(char id[], char spec[]) {
    char qualifier[80] = ":meta:";
    int key_length = strlen(id) + strlen(qualifier) + strlen(spec) + 1;
    char *key = (char *) malloc(key_length*sizeof(char));
    if (key == NULL)
        errx(ALLOC_ERROR, "can allocate key of length %d", key_length);
    snprintf(key,  key_length, "%s%s%s", id, qualifier, spec);
    return key;
}
