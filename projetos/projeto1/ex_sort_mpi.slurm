#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=./output/ex_sort_mpi.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=12M
#SBATCH --time=00:10:00
#SBATCH --partition=espec

echo 75 vertices
mpirun -np 4 ./ex_sort_mpi ./graphs/graph75.txt
echo

echo 100 vertices
mpirun -np 4 ./ex_sort_mpi ./graphs/graph100.txt
echo

echo 125 vertices
mpirun -np 4 ./ex_sort_mpi ./graphs/graph125.txt
echo

echo 150 vertices
mpirun -np 4 ./ex_sort_mpi ./graphs/graph150.txt
echo

echo 200 vertices
mpirun -np 4 ./ex_sort_mpi ./graphs/graph200.txt

