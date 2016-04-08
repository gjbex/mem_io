To retrieve the generated output from the database, `mem_io_retrieve` can be used.  It will print the collected output to standard output so that it can be redirected to a file.
Optionally, you can specify the specific channel ID to retrieve the data from.  This can be useful when the output was in some binary format, rather than ASCII text, so that concatenating the output of all channels would probably not result in a correct file.

This should of course only be run once when the job is completely finished.  Hence it is either in te job user epilogue, the worker epilog script, or at the very end of you PBS script.  It *never* occurs in a worker batch script.

To retrieve all results, and saving them into a text file `data.txt`, use:
```
mem_io_retrieve  >  data.txt
```

Sometimes it is useful to store the data of the individual channels in separate files, e.g., when the channels contain binary data which should not be combined into a single file.  For this purpose, `mem_io_retrieve` supports the `-split` flag.
```
mem_io_retrieve  -split
```
This will create as many files as there are channels, one for each, named `data_<mem_io_id>.dat`.  For convenience, `<mem_io_id>` is padded with zeroes, so that file names are trivally sorted lexicographically.
