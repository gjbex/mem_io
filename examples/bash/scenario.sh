#!/bin/bash

export PATH="../../bin:${PATH}"

# define the number of channels to use
nr_channels=5

# start mem_io
mem_io_start -nr_channels ${nr_channels}

# for each channel, write 10 values
for channel_id in `seq 0 $(( ${nr_channels} - 1 ))`
do
    for value in `seq ${channel_id} $(( ${channel_id} + 10 ))`
    do
        echo -n "${value} " | mem_io_store -channel_id ${channel_id}
    done
done

# retrieve all information and dump to file
mem_io_retrieve -print_id -sep ': ' > data.txt

# stop mem_io
mem_io_stop
