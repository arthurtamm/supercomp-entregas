#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=./output/ex2.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec


echo 75 vertices
./ex2 ./graphs/graph75.txt
echo

echo 100 vertices
./ex2 ./graphs/graph100.txt
echo

echo 125 vertices
./ex2 ./graphs/graph125.txt
echo

echo 150 vertices
./ex2 ./graphs/graph150.txt
echo

echo 200 vertices
./ex2 ./graphs/graph200.txt
