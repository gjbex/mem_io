#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "mem_io_cl_params.h"
#include "mem_io_cl_params_aux.h"

#define MAX_CL_OPT_LEN 128
#define MAX_CL_LINE_LEN 1024

void initCL(Params *params) {
	int len;
	len = strlen("");
	if (!(params->host = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate host field");
	strncpy(params->host, "", len + 1);
	params->port = 6379;
	params->timeout = 2;
	len = strlen("");
	if (!(params->password = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate password field");
	strncpy(params->password, "", len + 1);
	len = strlen("");
	if (!(params->mem_io_id = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate mem_io_id field");
	strncpy(params->mem_io_id, "", len + 1);
	len = strlen("/usr/local/software/redis/3.0.7/bin/redis-server");
	if (!(params->redis_path = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate redis_path field");
	strncpy(params->redis_path, "/usr/local/software/redis/3.0.7/bin/redis-server", len + 1);
	len = strlen("/usr/bin/m4");
	if (!(params->m4_path = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate m4_path field");
	strncpy(params->m4_path, "/usr/bin/m4", len + 1);
	len = strlen("../share/mem_io");
	if (!(params->redis_conf_path = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate redis_conf_path field");
	strncpy(params->redis_conf_path, "../share/mem_io", len + 1);
	len = strlen("redis.conf.m4");
	if (!(params->redis_conf_m4 = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate redis_conf_m4 field");
	strncpy(params->redis_conf_m4, "redis.conf.m4", len + 1);
	len = strlen("mem_io_global.conf");
	if (!(params->global_conf = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate global_conf field");
	strncpy(params->global_conf, "mem_io_global.conf", len + 1);
	len = strlen(".mem_io.conf");
	if (!(params->mem_io_conf = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate mem_io_conf field");
	strncpy(params->mem_io_conf, ".mem_io.conf", len + 1);
	len = strlen("");
	if (!(params->domain_name = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate domain_name field");
	strncpy(params->domain_name, "", len + 1);
	params->channel_id = -1;
	params->nr_channels = -1;
	params->verbose = false;
	params->force = false;
	params->restart = false;
	params->random = false;
	params->print_id = false;
	len = strlen(" ");
	if (!(params->sep = (char *) calloc(len + 1, sizeof(char))))
		errx(EXIT_CL_ALLOC_FAIL, "can not allocate sep field");
	strncpy(params->sep, " ", len + 1);
	params->split = false;
	params->details = false;
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
		if (!strncmp((*argv)[i], "-m4_path", 9)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-m4_path' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->m4_path);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->m4_path = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-redis_conf_path", 17)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-redis_conf_path' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->redis_conf_path);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->redis_conf_path = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-redis_conf_m4", 15)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-redis_conf_m4' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->redis_conf_m4);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->redis_conf_m4 = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-global_conf", 13)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-global_conf' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->global_conf);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->global_conf = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-mem_io_conf", 13)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-mem_io_conf' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->mem_io_conf);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->mem_io_conf = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-domain_name", 13)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-domain_name' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->domain_name);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->domain_name = strncpy(tmp, argv_str, len + 1);
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
			params->verbose = true;
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-force", 7)) {
			params->force = true;
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-restart", 9)) {
			params->restart = true;
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-random", 8)) {
			params->random = true;
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-print_id", 10)) {
			params->print_id = true;
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-sep", 5)) {
			shiftCL(&i, *argc, *argv);
			argv_str = (*argv)[i];
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-sep' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->sep);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->sep = strncpy(tmp, argv_str, len + 1);
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-split", 7)) {
			params->split = true;
			i++;
			continue;
		}
		if (!strncmp((*argv)[i], "-details", 9)) {
			params->details = true;
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
		if (sscanf(line_str, "m4_path = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-m4_path' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->m4_path);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->m4_path = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->m4_path);
			continue;
		}
		if (sscanf(line_str, "redis_conf_path = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-redis_conf_path' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->redis_conf_path);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->redis_conf_path = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->redis_conf_path);
			continue;
		}
		if (sscanf(line_str, "redis_conf_m4 = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-redis_conf_m4' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->redis_conf_m4);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->redis_conf_m4 = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->redis_conf_m4);
			continue;
		}
		if (sscanf(line_str, "global_conf = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-global_conf' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->global_conf);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->global_conf = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->global_conf);
			continue;
		}
		if (sscanf(line_str, "mem_io_conf = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-mem_io_conf' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->mem_io_conf);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->mem_io_conf = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->mem_io_conf);
			continue;
		}
		if (sscanf(line_str, "domain_name = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-domain_name' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->domain_name);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->domain_name = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->domain_name);
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
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-verbose' of type bool\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			if (!strncmp("false", argv_str, 6)) {
				params->verbose = false;
			} else if (!strncmp("true", argv_str, 5)) {
				params->verbose = true;
			} else {
				params->verbose = atoi(argv_str);
			}
			continue;
		}
		if (sscanf(line_str, "force = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-force' of type bool\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			if (!strncmp("false", argv_str, 6)) {
				params->force = false;
			} else if (!strncmp("true", argv_str, 5)) {
				params->force = true;
			} else {
				params->force = atoi(argv_str);
			}
			continue;
		}
		if (sscanf(line_str, "restart = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-restart' of type bool\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			if (!strncmp("false", argv_str, 6)) {
				params->restart = false;
			} else if (!strncmp("true", argv_str, 5)) {
				params->restart = true;
			} else {
				params->restart = atoi(argv_str);
			}
			continue;
		}
		if (sscanf(line_str, "random = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-random' of type bool\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			if (!strncmp("false", argv_str, 6)) {
				params->random = false;
			} else if (!strncmp("true", argv_str, 5)) {
				params->random = true;
			} else {
				params->random = atoi(argv_str);
			}
			continue;
		}
		if (sscanf(line_str, "print_id = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-print_id' of type bool\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			if (!strncmp("false", argv_str, 6)) {
				params->print_id = false;
			} else if (!strncmp("true", argv_str, 5)) {
				params->print_id = true;
			} else {
				params->print_id = atoi(argv_str);
			}
			continue;
		}
		if (sscanf(line_str, "sep = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-sep' of type char *\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			char *tmp;
			int len = strlen(argv_str);
			free(params->sep);
			if (!(tmp = (char *) calloc(len + 1, sizeof(char))))
				errx(EXIT_CL_ALLOC_FAIL, "can not allocate char* field");
			params->sep = strncpy(tmp, argv_str, len + 1);
			stripQuotesCL(params->sep);
			continue;
		}
		if (sscanf(line_str, "split = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-split' of type bool\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			if (!strncmp("false", argv_str, 6)) {
				params->split = false;
			} else if (!strncmp("true", argv_str, 5)) {
				params->split = true;
			} else {
				params->split = atoi(argv_str);
			}
			continue;
		}
		if (sscanf(line_str, "details = %[^\n]", argv_str) == 1) {
			if (!1) {
				fprintf(stderr, "### error: invalid value for option '-details' of type bool\n");
				exit(EXIT_CL_INVALID_VALUE);
			}
			if (!strncmp("false", argv_str, 6)) {
				params->details = false;
			} else if (!strncmp("true", argv_str, 5)) {
				params->details = true;
			} else {
				params->details = atoi(argv_str);
			}
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
	fprintf(fp, "%sm4_path = '%s'\n", prefix, params->m4_path);
	fprintf(fp, "%sredis_conf_path = '%s'\n", prefix, params->redis_conf_path);
	fprintf(fp, "%sredis_conf_m4 = '%s'\n", prefix, params->redis_conf_m4);
	fprintf(fp, "%sglobal_conf = '%s'\n", prefix, params->global_conf);
	fprintf(fp, "%smem_io_conf = '%s'\n", prefix, params->mem_io_conf);
	fprintf(fp, "%sdomain_name = '%s'\n", prefix, params->domain_name);
	fprintf(fp, "%schannel_id = %d\n", prefix, params->channel_id);
	fprintf(fp, "%snr_channels = %d\n", prefix, params->nr_channels);
	fprintf(fp, "%sverbose = %d\n", prefix, params->verbose);
	fprintf(fp, "%sforce = %d\n", prefix, params->force);
	fprintf(fp, "%srestart = %d\n", prefix, params->restart);
	fprintf(fp, "%srandom = %d\n", prefix, params->random);
	fprintf(fp, "%sprint_id = %d\n", prefix, params->print_id);
	fprintf(fp, "%ssep = '%s'\n", prefix, params->sep);
	fprintf(fp, "%ssplit = %d\n", prefix, params->split);
	fprintf(fp, "%sdetails = %d\n", prefix, params->details);
}

void finalizeCL(Params *params) {
	free(params->host);
	free(params->password);
	free(params->mem_io_id);
	free(params->redis_path);
	free(params->m4_path);
	free(params->redis_conf_path);
	free(params->redis_conf_m4);
	free(params->global_conf);
	free(params->mem_io_conf);
	free(params->domain_name);
	free(params->sep);
}

void printHelpCL(FILE *fp) {
	fprintf(fp, "  -host <string>\n  -port <integer>\n  -timeout <integer>\n  -password <string>\n  -mem_io_id <string>\n  -redis_path <string>\n  -m4_path <string>\n  -redis_conf_path <string>\n  -redis_conf_m4 <string>\n  -global_conf <string>\n  -mem_io_conf <string>\n  -domain_name <string>\n  -channel_id <integer>\n  -nr_channels <integer>\n  -verbose\n  -force\n  -restart\n  -random\n  -print_id\n  -sep <string>\n  -split\n  -details\n  -?: print this message");
}