In order to check the status of mem_io, you can use `mem_io_status`.  By default, it will provide a summary listing the number of channels, the number of channels that have not been used yet, the number of open, and closed channels.

This utitlity can be used to check on the progress of a running computation, for diagnostic purposes during a run, or the verify the status once the computation has finished.

Note that `mem_io_status` expects mem_io to be started, so if it is not, first used `mem_io_start  -restart  -mem_io_id <mem_io_id>` to start it.

If mem_io is running, simply check the  status using:
```
mem_io_status
```

To check the status of a running mem_io session on computes from a login node, simply use
```
mem_io_start  -mem_io_id <mem_io_id>
```
where `<mem_io_id>` denotes the mem_io ID of the running computation.

To check the status of a finished computation, use:
```
mem_io_start  -restart  -mem_io_id <mem_io_id>
mem_io_status  -mem_io_id <mem_io_id>
mem_io_stop  -Mem_io_id>
where `<mem_io_id>` denotes the mem_io ID of the running computation.
