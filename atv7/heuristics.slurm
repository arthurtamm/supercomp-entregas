#!/bin/bash

#SBATCH --ntasks=1
#SBATCH --output=./txt/heuristics.txt
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec

echo "Comparison of Shuffle Heuristic and Probabilistic Heuristic for the Knapsack Problem"
echo " "

# Shuffle Heuristic - 5 Runs
echo "================ Shuffle Heuristic ================"

echo "---------------- input1.txt -----------------------------"
for i in {1..5}
do
  echo "Run $i with input1.txt:"
  ./bin/shuffle < ./inputs/input1.txt
  echo " "
done

echo "---------------- input2.txt -----------------------------"
for i in {1..5}
do
  echo "Run $i with input2.txt:"
  ./bin/shuffle < ./inputs/input2.txt
  echo " "
done

echo "---------------- input3.txt -----------------------------"
for i in {1..5}
do
  echo "Run $i with input3.txt:"
  ./bin/shuffle < ./inputs/input3.txt
  echo " "
done

echo "---------------- input4.txt -----------------------------"
for i in {1..5}
do
  echo "Run $i with input4.txt:"
  ./bin/shuffle < ./inputs/input4.txt
  echo " "
done

# Probabilistic Heuristic - 5 Runs
echo "================ Probabilistic Heuristic ================"

echo "---------------- input1.txt -----------------------------"
for i in {1..5}
do
  echo "Run $i with input1.txt:"
  ./bin/probabilistic < ./inputs/input1.txt
  echo " "
done

echo "---------------- input2.txt -----------------------------"
for i in {1..5}
do
  echo "Run $i with input2.txt:"
  ./bin/probabilistic < ./inputs/input2.txt
  echo " "
done

echo "---------------- input3.txt -----------------------------"
for i in {1..5}
do
  echo "Run $i with input3.txt:"
  ./bin/probabilistic < ./inputs/input3.txt
  echo " "
done

echo "---------------- input4.txt -----------------------------"
for i in {1..5}
do
  echo "Run $i with input4.txt:"
  ./bin/probabilistic < ./inputs/input4.txt
  echo " "
done

echo "================ End of Heuristic Executions ================"
