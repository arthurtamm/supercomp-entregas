#!/bin/bash

#SBATCH --ntasks=1
#SBATCH --output=./txt/exSearch.txt
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec

echo "Exhaustive Search for the Knapsack Problem"
echo " "

# Exhaustive Search

echo "================ Exhaustive Search ================"

echo "Running Exhaustive Search with input1.txt:"
./bin/exSearch < ./inputs/input1.txt
echo " "

echo "Running Exhaustive Search with input2.txt:"
./bin/exSearch < ./inputs/input2.txt
echo " "

echo "Running Exhaustive Search with input3.txt:"
./bin/exSearch < ./inputs/input3.txt
echo " "

echo "Running Exhaustive Search with input4.txt:"
./bin/exSearch < ./inputs/input4.txt
echo " "

echo "================ End of Exhaustive Search Executions ================"
