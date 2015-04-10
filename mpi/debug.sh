#!/bin/bash

mpirun -n 5 gdb -x gdb_commands -q --args a.out 5 ../vierhoek.txt
