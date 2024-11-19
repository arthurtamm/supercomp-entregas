#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=./output/ex_sort_omp.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec

echo 75 vertices
./ex_sort_omp ./graphs/graph75.txt
echo

echo 100 vertices
./ex_sort_omp ./graphs/graph100.txt
echo

echo 125 vertices
./ex_sort_omp ./graphs/graph125.txt
echo

echo 150 vertices
./ex_sort_omp ./graphs/graph150.txt
echo

echo 200 vertices
./ex_sort_omp ./graphs/graph200.txt


