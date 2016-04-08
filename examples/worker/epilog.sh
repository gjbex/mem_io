#!/bin/bash -l

cd ${PBS_O_WORKDIR}

../../bin/mem_io_retrieve -verbose > data.txt
../../bin/mem_io_stop
