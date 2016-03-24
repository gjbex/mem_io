# To do
1. Use the PBS torque job id as mem_io_id unless the latter is specified
    explicitly.
1. Write a function that uses m4 to create a per run redis config file, and
    call that from mem_io_start.  This should fill out the dbfilename (based
    on job ID as possible), and the password.  The function should also ensure
    the correct file permissions since it contains a clear text password.
1. Write a function to determine hostname and dump it to disk in specifc
    file, and function to read that file to retrieve the jostname.
1. Write an application to retrieve data from all channels, storing it in
    an individual file per channel.
1. Write documentation.
