CC = gcc
CFLAGS = -O2 -g -Wall -std=c99
CPPFLAGS = `pkg-config --cflags hiredis`
LDFLAGS = `pkg-config --libs hiredis`

APPL = mem_io_start mem_io_store mem_io_retrieve
COMMON_OBJS = mem_io_utils.o mem_io_cl_params.o mem_io_cl_params_aux.o
MEM_IO_START = $(COMMON_OBJS) mem_io_start.o
MEM_IO_STORE_OBJS = $(COMMON_OBJS) mem_io_store.o
MEM_IO_RETRIEVE_OBJS = $(COMMON_OBJS) mem_io_retrieve.o

all: $(APPL)

mem_io_start: $(MEM_IO_START)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

mem_io_store: $(MEM_IO_STORE_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

mem_io_retrieve: $(MEM_IO_RETRIEVE_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f $(APPL) *.o

weave:
	weave -l C -d params_mem_io.txt -b mem_io_cl_params

weave_clean:
	rm -f mem_io_cl_params.[ch] mem_io_cl_params_aux.[ch]
