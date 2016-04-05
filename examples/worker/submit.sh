#!/bin/bash -l

module load worker/1.6.4-intel-2015a

wsub -t 0-99 -prolog prolog.sh -batch job.pbs -epilog epilog.sh
