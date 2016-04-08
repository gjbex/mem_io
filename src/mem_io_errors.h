#ifndef __MEM_IO_ERRORS__
#define __MEM_IO_ERRORS__


/* redis start errors */
#define FORK_ERROR 12
#define REDIS_RUN_ERROR 13
#define M4_RUN_ERROR 14

/* application logic errors */
#define ALLOC_ERROR 20
#define INVALID_CHANNEL_ERROR 21
#define INVALID_NR_CHANNELS 22
#define HOSTNAME_ERROR 23
#define OPEN_CHANNEL_ERROR 24

/* redis command errors */
#define AUTH_ERROR 51
#define RPUSH_ERROR 52
#define LINDEX_ERROR 53
#define SET_ERROR 54
#define GET_ERROR 55
#define LLEN_ERROR 56
#define SHUTDOWN_ERROR 57

#endif
