#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=./output/bk_mpi.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096M
#SBATCH --time=00:05:00
#SBATCH --partition=espec

echo 100 vertices
echo " "
mpirun -np 4 ./bk_mpi ./graphs/graph100.txt
echo

echo 150 vertices
echo " "
mpirun -np 4 ./bk_mpi ./graphs/graph150.txt
