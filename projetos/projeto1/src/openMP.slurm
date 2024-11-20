#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=../output/openMP.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec

echo 75 vertices
./openMP ./graphs/graph75.txt
echo

echo 100 vertices
./openMP ./graphs/graph100.txt
echo

echo 125 vertices
./openMP ./graphs/graph125.txt
echo

echo 150 vertices
./openMP ./graphs/graph150.txt
echo

echo 200 vertices
./openMP ./graphs/graph200.txt


