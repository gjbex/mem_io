#!/bin/bash -l

cd ${PBS_O_WORKDIR}

../../bin/mem_io_start -nr_channels 100 -verbose
