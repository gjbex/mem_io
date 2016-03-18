#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "mem_io_cl_params.h"
#include "mem_io_cl_params_aux.h"

#define MAX_CL_OPT_LEN 128
#define MAX_CL_LINE_LEN 1024

void initCL(Params *params) {
	int len;
	len = strlen("localhost");
	if (!(params->host = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate host field");
	strncpy(params->host, "localhost", len + 1);
	params->port = 6379;
	params->timeout = 2;
	len = strlen("foobared");
	if (!(params->password = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate password field");
	strncpy(params->password, "foobared", len + 1);
	len = strlen("default");
	if (!(params->mem_io_id = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate mem_io_id field");
	strncpy(params->mem_io_id, "default", len + 1);
	len = strlen("/usr/local/software/redis/3.0.7/bin/redis-server");
	if (!(params->redis_path = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate redis_path field");
	strncpy(params->redis_path, "/usr/local/software/redis/3.0.7/bin/redis-server", len + 1);
	len = strlen("/home/lucg5005/redis.conf");
	if (!(params->redis_conf = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate redis_conf field");
	strncpy(params->redis_conf, "/home/lucg5005/redis.conf", len + 1);
	params->channel_id = -1;
	params->nr_channels = -1;
	params->verbose = 0;
}

void parseCL(Params *params, int *argc, char **argv[]) {
	char *argv_str;
	int i = 1;
	while (i < *argc) {
		 if (!strncmp((*argv)[i], "-?", 3)) {
			printHelpCL(stderr);
			finalizeCL(params);
			exit(EXIT_SUCCESS);
		}
		if (!strncmp((*argv)[i], "-host", 6)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-host' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->host);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->host = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-port", 6)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-port' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->port = atoi(argv_str);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-timeout", 9)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-timeout' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->timeout = atoi(argv_str);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-password", 10)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-password' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->password);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->password = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-mem_io_id", 11)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-mem_io_id' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->mem_io_id);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->mem_io_id = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-redis_path", 12)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-redis_path' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->redis_path);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->redis_path = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-redis_conf", 12)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-redis_conf' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->redis_conf);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->redis_conf = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-channel_id", 12)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-channel_id' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->channel_id = atoi(argv_str);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-nr_channels", 13)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-nr_channels' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->nr_channels = atoi(argv_str);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-verbose", 9)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-verbose' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->verbose = atoi(argv_str);
			i++;
			continue;
		}
		break;
	}
	if (i > 1) {
		(*argv)[i - 1] = (*argv)[0];
		*argv = &((*argv)[i - 1]);
		*argc -= (i - 1);
	}
}

void parseFileCL(Params *params, char *fileName) {
	char line_str[MAX_CL_LINE_LEN];
	char argv_str[MAX_CL_LINE_LEN];
	FILE *fp;
	if (!(fp = fopen(fileName, "r"))) {
		fprintf(stderr, "### error: can not open file '%s'\n", fileName);
		exit(EXIT_CL_FILE_OPEN_FAIL);
	}
	while (fgets(line_str, MAX_CL_LINE_LEN, fp)) {
		if (isCommentCL(line_str)) continue;
		if (isEmptyLineCL(line_str)) continue;
		if (sscanf(line_str, "host = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-host' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->host);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->host = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->host);
			continue;
		}
		if (sscanf(line_str, "port = %[^\n]", argv_str) == 1) {
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-port' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->port = atoi(argv_str);
			continue;
		}
		if (sscanf(line_str, "timeout = %[^\n]", argv_str) == 1) {
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-timeout' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->timeout = atoi(argv_str);
			continue;
		}
		if (sscanf(line_str, "password = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-password' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->password);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->password = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->password);
			continue;
		}
		if (sscanf(line_str, "mem_io_id = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-mem_io_id' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->mem_io_id);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->mem_io_id = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->mem_io_id);
			continue;
		}
		if (sscanf(line_str, "redis_path = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-redis_path' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->redis_path);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->redis_path = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->redis_path);
			continue;
		}
		if (sscanf(line_str, "redis_conf = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-redis_conf' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->redis_conf);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->redis_conf = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->redis_conf);
			continue;
		}
		if (sscanf(line_str, "channel_id = %[^\n]", argv_str) == 1) {
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-channel_id' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->channel_id = atoi(argv_str);
			continue;
		}
		if (sscanf(line_str, "nr_channels = %[^\n]", argv_str) == 1) {
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-nr_channels' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->nr_channels = atoi(argv_str);
			continue;
		}
		if (sscanf(line_str, "verbose = %[^\n]", argv_str) == 1) {
			if (!isIntCL(argv_str, 0)) {
				fprintf(stderr, "### error: invalid value for option '-verbose' of type int\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			params->verbose = atoi(argv_str);
			continue;
		}
		fprintf(stderr, "### warning, line can not be parsed: '%s'\n", line_str);
	}
	fclose(fp);
}

void dumpCL(FILE *fp, char prefix[], Params *params) {
	fprintf(fp, "%shost = '%s'\n", prefix, params->host);
	fprintf(fp, "%sport = %d\n", prefix, params->port);
	fprintf(fp, "%stimeout = %d\n", prefix, params->timeout);
	fprintf(fp, "%spassword = '%s'\n", prefix, params->password);
	fprintf(fp, "%smem_io_id = '%s'\n", prefix, params->mem_io_id);
	fprintf(fp, "%sredis_path = '%s'\n", prefix, params->redis_path);
	fprintf(fp, "%sredis_conf = '%s'\n", prefix, params->redis_conf);
	fprintf(fp, "%schannel_id = %d\n", prefix, params->channel_id);
	fprintf(fp, "%snr_channels = %d\n", prefix, params->nr_channels);
	fprintf(fp, "%sverbose = %d\n", prefix, params->verbose);
}

void finalizeCL(Params *params) {
	free(params->host);
	free(params->password);
	free(params->mem_io_id);
	free(params->redis_path);
	free(params->redis_conf);
}

void printHelpCL(FILE *fp) {
	fprintf(fp, "  -host <string>\n  -port <integer>\n  -timeout <integer>\n  -password <string>\n  -mem_io_id <string>\n  -redis_path <string>\n  -redis_conf <string>\n  -channel_id <integer>\n  -nr_channels <integer>\n  -verbose <integer>\n  -?: print this message");
}