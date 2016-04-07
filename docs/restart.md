When a cmputation was interrupted before `mem_io_retrieve` was finished, or you prefer to retrieve information at some later point, mem_io can in fact be restarted.

Suppose that the mem_io ID for the computation was `param_est`, then we can restart using
```
mem_io_start  -restart  -mem_io_id param_est
```
The database named `redis_param_est.rdb` will be used, as well as the configuration files `redis_param_est.conf` and `mem_io_param_est.conf`.

Now it is possible to retrieve informaiton using
```
mem_io_retrieve  -mem_io_id param_est
```

Note that if `mem_io_store` processes were prematurely ended, for each channel that was in use at the time, a warning will be generated since information might be lost.

If you restart a database for which all channels were closed properly, it is possible to insert additional data in it when it is restarted.  However, the number of channels will remain the same, so it is only possible to add information to existing channels, not create new ones upon restart.
