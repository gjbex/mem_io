To stop mem_io, and properly shutting down the database, you simply run the `mem_io_stop` command.  This should of course only be run once when the job is completely finished.  Hence it is either in te job user epilogue, the worker epilog script, or at the very end of you PBS script.  It *never* occurs in a worker batch script.
```
mem_io_shutdown
```
