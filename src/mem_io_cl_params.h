#ifndef MEM_IO_CL_PARAMS_HDR
#define MEM_IO_CL_PARAMS_HDR

#include <stdbool.h>
#include <stdio.h>

typedef struct {
	char * host;
	int port;
	int timeout;
	char * password;
	char * mem_io_id;
	char * redis_path;
	char * m4_path;
	char * redis_conf_path;
	char * redis_conf_m4;
	char * global_conf;
	char * mem_io_conf;
	char * domain_name;
	int channel_id;
	int nr_channels;
	bool verbose;
	bool force;
	bool restart;
	bool random;
} Params;

void initCL(Params *params);
void parseCL(Params *params, int *argc, char **argv[]);
void parseFileCL(Params *params, char *fileName);
void dumpCL(FILE *fp, char prefix[], Params *params);
void finalizeCL(Params *params);
void printHelpCL(FILE *fp);

#endif