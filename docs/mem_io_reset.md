If a computation was aborted while it was still in progress, a number of channels will still be open, as reported by `mem_io_status`.  This probably means that the data in thoese channels is incomplete, and that the computations that were supposed to produce it will have to be redone.

Since it is mostly impossible to resume an aborted computation at exactly the point were it was aborted, it is probably redone completely.  Hence the data in the open channels should be removed, and the channels restored to their original status (unused).

`mem_io_reset` will do this.
```
mem_io_start  -restart  -mem_io_id <mem_io_id>
mem_io_reset
mem_io_stop  -Mem_io_id>
```
where `<mem_io_id>` denotes the mem_io ID of the computation to be reset.

Note that to use `mem_io_reset`, mem_io must be running, so typically you will have to restart  it, as in the example above.

Also note that `mem_io_reset` will only remove information frmo the database that is most likely corrupt or incomplete, you may want to savve that nevertheless by using `mem_io_retrieve` first.
