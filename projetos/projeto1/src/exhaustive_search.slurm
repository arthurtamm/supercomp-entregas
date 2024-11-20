#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --output=../output/exhaustive_search.txt
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec


echo 75 vertices
./exhaustive_search ./graphs/graph75.txt
echo

echo 100 vertices
./exhaustive_search ./graphs/graph100.txt
echo

echo 125 vertices
./exhaustive_search ./graphs/graph125.txt
echo

echo 150 vertices
./exhaustive_search ./graphs/graph150.txt
echo

echo 200 vertices
./exhaustive_search ./graphs/graph200.txt
