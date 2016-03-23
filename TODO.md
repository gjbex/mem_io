# To do
1. Write function to retrieve password from .mem_io.conf if it is not given on
    the command line.  Use this function in all applications that require
    authentication.
1. Write a function to retrieve the PBS torque job ID from an environment
    variable.
1. Use the PBS torque job id as mem_io_id unless the latter is specified
    explicitly.
1. Write a function that uses m4 to create a per run redis config file, and
    call that from mem_io_start.  This should fill out the dbfilename (based
    on job ID as possible), and the password.  The function should also ensure
    the correct file permissions since it contains a clear text password.
1. Write an application to retrieve data from all channels, storing it in
    an individual file per channel.
1. Write documentation.
