To store output, `mem_io_store` reads everything that is sent to its standard input, and stores it in the specified channel in te in-memory database.  It will retrieve all necessary information from the run specific mem_io configuration file.

A mem_io channel ID is an integer between 0 and nr_channels - 1, inclusive.

For example, to store the results of an individual task or worker work item that is a computation with R:
```
Rscript  my_script  $var1  $var2  |  mem_io_store  -channel_id $id
```
Here `$var1` and `$var2` would contain the input parameters for this task, and `$id` would be the mem_io channel ID.
