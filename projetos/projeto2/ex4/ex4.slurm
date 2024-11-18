#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=../output/ex4.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec


mpirun -np 4 ./ex4
