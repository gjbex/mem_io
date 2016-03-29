For each job, the first thing to do is to start mem_io.  This is done using the `mem_io_start` command.  This will:
* create a run-specific mem_io configuration file,
* create a run-specific redis configuration file, and
* start the redis database in the background.

Each mem_io run is identified by a mem_io ID, which is intended to be unique.  it can either be specified on the command line using the `-mem_io_id` option, taken for the `PBS_JOBID` environment variable, or default to `default` (which is of course not unique, and will overwrite data from from previous runs with that mem_io ID).

The mem_io configuration file will be created in the current working directory and its name has the following pattern `mem_io_<mem_io_id>.conf`.  Since this file contains the password, care is taken that it is creaed with the correct file permissions.

The redis configuration file will be created in the current working directory and its name has the following pattern `redis_<mem_io_id>.conf`.  Since this file contains the password, care is taken that it is creaed with the correct file permissions.

Upon starting mem_io, you have to specify the number of I/O channels to use.  These are independent storage entries in the database, and each store operation will append information to a specific channel, identified by a channel ID.  In practice, the number of channels is typically equal to the number of files that would have been produced by the job if mem_io were not used.

Hence, starting mem_io with, e.g., 20 channels would be accomplished by:
```
mem_io_start  -nr_channels 20
```

The resulting on-disk database file will named according to the following pattern: `mem_io_<mem_io_id>.rdb`.

Note that mem_io is only started once per job, either in the PBS user prologue file, the worker prolog script, or at the start of your PBS job script.  Hence this command *should never* occor in a worker batch script!
