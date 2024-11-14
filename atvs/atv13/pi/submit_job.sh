#!/bin/bash
#SBATCH --job-name=omp_scheduler_test
#SBATCH --output=output.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=1024
#SBATCH --time=00:05:00

./pi_parallel_for
./pi_task