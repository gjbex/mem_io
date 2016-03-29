The first step is to set a password to protect access to the database.  The password will be stored in a file `.mem_io.conf` that is created in your home directory.  The permissions of this file will automatically be set to ensure that only you have read/write access to it.
```
$ mem_io_set_password
```
will prompt you for a password, create the configuration file in your home directory with the correct permissions, and store the password in it.

This has to be done only once, unless you wish to change your password.

Note  that if the `.mem_io.conf` file already exists, you can modify your password using the `--force` flag, i.e.,
```
$ mem_io_set_password  --force
```
