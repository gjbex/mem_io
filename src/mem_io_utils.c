/*!
  \file
  \brief Implementation of utility functions used throughout the mem_io
         project.
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mem_io_errors.h"
#include "mem_io_utils.h"
#include "cmd_utils.h"

#define MAX_LENGTH 1024

/*!
  \brief Create a connection to the redis database, represented by a
         redisContext.
  \param host Name of the host the database is running on.
  \param port Number of the port the database is listening on.
  \param timeout Maximum timeout for the connection to be established.
  \return redisContext The database context that is used to interact
          with the database.
*/
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

/*!
  \brief Authenticate to the database, required at the start of a session
  \param context Rdis database context to work in.
  \param password Password to authenticate.
*/
void mem_io_auth(redisContext *context, char password[]) {
    redisReply *reply = redisCommand(context, "AUTH %b",
                                     password, strlen(password));
    if (reply->type == REDIS_REPLY_ERROR)
        errx(AUTH_ERROR, "AUTH failed: %s", reply->str);
    freeReplyObject(reply);
}

/*!
  \brief Set the number of channels to use in this session, should be
         called only when a new database is created.
  \param context Redis database context to work in.
  \param mem_io_id mem_io identifier string to use.
  \param nr_channels The number of channels that should be available.
*/
void mem_io_set_nr_channels(redisContext *context, char mem_io_id[],
                            int nr_channels) {
    char *key = mem_io_create_meta_key(mem_io_id, "nr_channels");
    redisReply *reply = redisCommand(context, "SET %b %d",
                                     key, strlen(key), nr_channels);
    if (reply->type == REDIS_REPLY_ERROR)
        errx(SET_ERROR, "SET for '%s' failed: %s", key, reply->str);
    freeReplyObject(reply);
    free(key);
}

/*!
  \brief Get the number of channels used in this session.
  \param context Redis database context to work in.
  \param mem_io_id mem_io identifier string to use.
  \return int The number of channels that should be available.
*/
int mem_io_get_nr_channels(redisContext *context, char mem_io_id[]) {
    char *key = mem_io_create_meta_key(mem_io_id, "nr_channels");
    redisReply *reply = redisCommand(context, "GET %b", key, strlen(key));
    if (reply->type == REDIS_REPLY_ERROR)
        errx(GET_ERROR, "GET for '%s' failed: %s", key, reply->str);
    int value = atoi(reply->str);
    freeReplyObject(reply);
    free(key);
    return value;
}

/*!
  \brief Push a new value to a channel.
  \param context Redis database context to work in.
  \param key Redis database key identifying the channel to be pushed to.
  \param value Byte sequence to be pushed to the channel.
  \param length Number of bytes to push.
*/
void mem_io_push(redisContext *context, char key[],
                 char *value, int length) {
    redisReply *reply = redisCommand(context, "RPUSH %b %b",
                                     key, strlen(key),
                                     value, length);
    if (reply->type == REDIS_REPLY_ERROR)
        errx(RPUSH_ERROR, "RPUSH to '%s' failed: %s", key, reply->str);
    freeReplyObject(reply);
}

/*!
  \brief Retrieve the data written to a channel, and write it all to
         the given output stream.
  \param context Redis database context to work in.
  \param key Redis database key identifying the channel to be retrieved.
  \param fp File pointer to write the data to, is supposed to be open
            for writing.
*/
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

/*!
  \brief Modify the specified channel's status.
  \param context Redis database context to work in.
  \param key Redis key to store status information in.
  \param status Status string to set, either 'open' or 'closed'
*/
void mem_io_set_channel_status(redisContext *context, char key[],
                               char status[]) {
    redisReply *reply = redisCommand(context, "SET %b %b",
                                     key, strlen(key),
                                     status, strlen(status) + 1);
    if (reply->type == REDIS_REPLY_ERROR)
        errx(SET_ERROR, "SET for '%s' failed: %s", key, reply->str);
    freeReplyObject(reply);
}

/*!
  \brief Open the specified channel.
  \param context Redis database context to work in.
  \param key Redis key to set 'open' value in.
*/
void mem_io_open_channel(redisContext *context, char key[]) {
    mem_io_set_channel_status(context, key, "open");
}

/*!
  \brief Close the specified channel.
  \param context Redis database context to work in.
  \param key Redis key to set 'close' value in.
*/
void mem_io_close_channel(redisContext *context, char key[]) {
    mem_io_set_channel_status(context, key, "closed");
}

/*!
  \brief Check whether channel status information has been set for the
         given key
  \param context Redis database context to work in.
  \param key Redis key to check for.
 */
bool mem_io_channel_status_is_set(redisContext *context, char key[]) {
    redisReply *reply = redisCommand(context, "GET %b",
                                     key, strlen(key));
    bool result = !(reply->type == REDIS_REPLY_ERROR);
    freeReplyObject(reply);
    return result;
}

/*!
  \brief Check whether channel status information has been set for the
         given key
  \param context Redis database context to work in.
  \param key Redis key to check for.
 */
bool mem_io_is_channel_open(redisContext *context, char key[]) {
    if (!mem_io_channel_status_is_set(context, key))
        return false;
    redisReply *reply = redisCommand(context, "GET %b",
                                     key, strlen(key));
    if (reply->type == REDIS_REPLY_ERROR)
        errx(GET_ERROR, "GET for '%s' failed: %s", key, reply->str);
    bool result = (0 == strncmp(reply->str, "open", strlen("open") + 1));
    freeReplyObject(reply);
    return result;
}

/*!
  \brief Stop the redis database by performing a shutdown.
  \param context Redis database context to work in.
*/
void mem_io_shutdown(redisContext *context) {
    redisReply *reply = redisCommand(context, "SHUTDOWN");
    freeReplyObject(reply);
}

/*!
  \brief Destroy the database context
  \param context Redis database context to work in.
*/
void mem_io_disconnect(redisContext *context) {
    redisFree(context);
}

/*!
  \brief Create a redis database key for the given channel ID.
  \param mem_io_id mem_io identifier string to use.
  \param channel_id Number of the channel to be used, between 0 and
                    nr_channels - 1, inclusive.
  \return char* The key to access the given channel ID.
*/
char *mem_io_create_key(char mem_io_id[], int channel_id) {
    char qualifier[80] = ":data:";
    int key_length = strlen(mem_io_id) + strlen(qualifier) +
        CHANNEL_ID_WIDTH + 1;
    char *key = (char *) malloc(key_length*sizeof(char));
    if (key == NULL)
        errx(ALLOC_ERROR, "can int allocate key of length %d", key_length);
    snprintf(key,  key_length, "%s%s%0*d", mem_io_id, qualifier,
             CHANNEL_ID_WIDTH, channel_id);
    return key;
}

/*!
  \brief Create a redis database key for storing meta information.
  \param mem_io_id mem_io identifier string to use.
  \param spec Name of the meta-information specifier to generate the key
              for.
  \return char* The key to access the given meta-information specifier.
*/
char *mem_io_create_meta_key(char mem_io_id[], char spec[]) {
    char qualifier[80] = ":meta:";
    int key_length = strlen(mem_io_id) + strlen(qualifier) +
        strlen(spec) + 1;
    char *key = (char *) malloc(key_length*sizeof(char));
    if (key == NULL)
        errx(ALLOC_ERROR, "can not allocate key of length %d", key_length);
    snprintf(key,  key_length, "%s%s%s", mem_io_id, qualifier, spec);
    return key;
}

/*!
  \brief Create a redis database key for storing status information about
         the given channel.
  \param mem_io_id mem_io identifier string to use.
  \param channel_id Channel ID to store the status of.
              for.
  \return char* The key to access the given channel's status information.
*/
char *mem_io_create_channel_status_key(char mem_io_id[], int channel_id) {
    char qualifier[80] = ":meta:status:";
    int key_length = strlen(mem_io_id) + strlen(qualifier) +
        CHANNEL_ID_WIDTH + 1;
    char *key = (char *) malloc(key_length*sizeof(char));
    if (key == NULL)
        errx(ALLOC_ERROR, "can not allocate key of length %d", key_length);
    snprintf(key,  key_length, "%s%s%0*d", mem_io_id, qualifier,
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
        if (params->verbose)
            fprintf(stderr, "# reading config file '%s'\n", conf_path);
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

/*!
  \brief Determine the mem_io ID.
  \param params Command line and configuration file parameters.
  \return char* String containing the relevant mem_io ID.

  If the mem_io_id is set via the command line, or a configuration file,
  use that.  If not, retrieve the job ID, and use that (only available
  when using mem_io in the context of a job under control of a resource
  manager or scheduler, e.g., PBS torque).  If not, use the default name.
*/
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
            id_length = strlen(DEFAULT_MEM_IO) + 1;
            char *id = (char *) malloc(id_length*sizeof(char));
            if (id == NULL)
                errx(ALLOC_ERROR, "can not allocate default mem_io ID");
            strcpy(id, DEFAULT_MEM_IO);
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

/*!
  \brief Generate the name of the mem_io configuration name based on
         the mem_io ID.
  \param mem_io_id mem_io ID string to use.
  \return char* Name of the mem_io configuration file relevant for the
                given mem_io ID.
*/
char *mem_io_conf_name(char mem_io_id[]) {
    return conf_name(MEM_IO_CONF_PREFIX, mem_io_id, MEM_IO_CONF_EXT);
}

/*!
  \brief Generate the name of the global mem_io configuration file
  \return char* Name of the global mem_io configuration file
*/

#define REDIS_PREFIX "redis_"
#define REDIS_CONF_EXT ".conf"

/*!
  \brief Generate the name of the redis configuration name based on
         the mem_io ID.
  \param mem_io_id mem_io ID string to use.
  \return char* Name of the redis configuration file relevant for the
                given mem_io ID.
*/
char *redis_conf_name(char mem_io_id[]) {
    return conf_name(REDIS_PREFIX, mem_io_id, REDIS_CONF_EXT);
}

#define REDIS_DB_EXT ".rdb"

/*!
  \brief Generate the name of the redis database file name based on
         the mem_io ID.
  \param mem_io_id mem_io ID string to use.
  \return char* Name of the redis database file relevant for the
                given mem_io ID.
*/
char *redis_db_name(char mem_io_id[]) {
    return conf_name(REDIS_PREFIX, mem_io_id, REDIS_DB_EXT);
}

/*!
  \brief Merge parameters specified on the command line with those in
         the session mem_io configuration file
  \param mem_io_id mem_io ID to use.
  \param params Parameters passed via the command line
*/
void mem_io_merge_params(char mem_io_id[], Params *params) {
    char *conf_name = mem_io_conf_name(mem_io_id);
    if (file_exists(conf_name)) {
        Params conf_params;
        initCL(&conf_params);
        parseFileCL(&conf_params, conf_name);
        if (strlen(params->password) == 0) {
            free(params->password);
            int str_len = strlen(conf_params.password) + 1;
            params->password = (char *) malloc(str_len*sizeof(char));
            if (params->password == NULL)
                errx(ALLOC_ERROR, "can not allocate password string");
            strncpy(params->password, conf_params.password, str_len);
        }
        if (strlen(params->host) == 0) {
            free(params->host);
            int str_len = strlen(conf_params.host) + 1;
            params->host = (char *) malloc(str_len*sizeof(char));
            if (params->host == NULL)
                errx(ALLOC_ERROR, "can not allocate host string");
            strncpy(params->host, conf_params.host, str_len);
        }
        if (params->port == REDIS_DEFAULT_PORT) {
            params->port = conf_params.port;
        }
        finalizeCL(&conf_params);
    }
    free(conf_name);
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
