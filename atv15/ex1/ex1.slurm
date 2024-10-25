#!/bin/bash

#SBATCH --ntasks=2
#SBATCH --output=../output/ex1.txt
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:50:00
#SBATCH --partition=espec


mpirun -np 2 ./ex1