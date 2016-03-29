To retrieve the generated output from the database, `mem_io_retrieve` can be used.  It will print the collected output to standard output so that it can be redirected to a file.
Optionally, you can specify the specific channel ID to retrieve the data from.  This can be useful when the output was in some binary format, rather than ASCII text, so that concatenating the output of all channels would probably not result in a correct file.

This should of course only be run once when the job is completely finished.  Hence it is either in te job user epilogue, the worker epilog script, or at the very end of you PBS script.  It *never* occurs in a worker batch script.

To retrieve all results, and saving them into a text file `data.txt`, use:
```
mem_io_retrieve  >  data.txt
```
