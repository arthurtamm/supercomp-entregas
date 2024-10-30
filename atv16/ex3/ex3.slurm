#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=../output/ex3.txt
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:50:00
#SBATCH --partition=espec


mpirun -np 4 ./ex3