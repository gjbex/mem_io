# To do

## Must do
1. Write a function that uses m4 to create a per run redis config file, and
    call that from mem_io_start.  This should fill out the dbfilename
    (based on job ID as possible), and the password.  The function should
    also ensure the correct file permissions since it contains a clear
    text password.
1. `mem_io_store`, `mem_io_retrieve`, and `mem_io_stop` should take into
    account run-specific mem_io configuration file.
1. Write documentation.

## Nice to have
1. Write an application to retrieve data from all channels, storing it in
    an individual file per channel.
