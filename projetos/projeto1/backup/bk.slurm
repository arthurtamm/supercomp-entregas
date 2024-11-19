#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=./output/bk.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec

./bk ./graphs/graph150.txt
