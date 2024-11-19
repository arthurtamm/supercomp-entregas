#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=./output/ex.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec


./ex ./graphs/graph150.txt
