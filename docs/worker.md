When using mem_io together with [worker](http://worker.readtheocs.org/), one of course has to set the mem_io password once using `mem_io_set_password`.  Again, this has to be done only once, see [Setting your mem_io password](mem_io_set_password.md).

mem_io will be started from a worker prolog script, i.e.,
```
module load mem_io

mem_io_start  -nr_channels 300
```
Here, the number of mem_io channels should be adapted to fit your computations.

The actual work is specified in the worker batch file, and the standard output is redirected to the appropriate mem_io channel.
```
module load mem_io

some_program  "some_input_${id}"  |  mem_io_store  -channel_id ${id}
```
Note that the channel ID (i.e., the `$id` variable in this example) should be in the range 0 to number of channels - 1, inclusive.

The worker epilog file will retrieve the data, save it to a file `all_output`, and stop mem_io:
```
module load mem_io
mem_io_retrieve  >  all_ouput
mem_io_stop
```
