#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=./output/ex_sort.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec

./ex_sort ./graphs/graph150.txt
