## Without mem_io

Consider the following script that would perform parallel, indepedent computations on a compute node that has a total of 20 cores:
```
nr_cores=20
for id in `seq 0 $(( ${nr_cores} - 1 ));
do
    some_program  "some_input_${id}"  >  "some_output_${i}" &
done
wait

for id in `seq 0 $(( ${nr_cores} - 1 ));
do
    cat  "some_output_${i}"  >>  "all_output"
done
```
Each of the, in this case, 20 instances of the `some_program` application will perform writes on standard output, which is redirected to individual files.  When the computations have been performed in parallel, the resulting individual output files are aggregated into a single file.

Needless to say that such an approach will lead to many I/O, and meta data operations.  This particular case is pretty mild when compared to more massively parallelized computation using the worker framework or nitro (Adaptive Computing).

## With mem_io

mem_io can be of help here, with a very minor modification to the workflow, and none to the application `some_program`.  Consider:
```
nr_cores=20

mem_io_start  -nr_channels ${nr_cores}

for id in `seq 0 $(( ${nr_cores} - 1 ));
do
    some_program  "some_input_${id}"  |  mem_io_store  -channel_id ${id}
done
wait

mem_io_retrieve > all_ouput
mem_io_stop
```
The `mem_io_start` command prepares a configuration file, and starts an
in-memory database that will be used to store the I/O.  For details, see [Starting mem_io](mem_io_start).

Rather than redirecting the standard output of processes to a file, it is piped through `mem_io_store` which will deal with the file I/O on behalf of the processes.  For details, see [Writing data to mem_io](mem_io_store.md).

When all computations are done, the output is retrieved from the database by the `mem_io_retrieve` command, and can be redirected to a file, `all_output` in this example.  For details, see [Retrieving data from mem_io](mem_io_retrieve.md).

Lastly, the in-memory will be shut down cleanly by `mem_io_stop`.  For detals, see [Stopping mem_io](mem_io_stop).

Note that `mem_io_store` is agnostic about the nature of the processes' output, which may be text or binary.  However, for binary output, a simple concatenation of individual output will probably not result in a valid aggregated output file, so the output needs to be retrieved and redirected on a per channel basis using `mem_io_retrieve` `-channel_id` option.
