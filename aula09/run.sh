#!/bin/bash

# Arquivo de saída
output_file="output.txt"

# Limpar o conteúdo do arquivo de saída, se já existir
> $output_file

# Lista de entradas
inputs=("./inputs/input1.txt" "./inputs/input2.txt" "./inputs/input3.txt" "./inputs/input4.txt")

# Caminho dos binários
maxValue='./bin/maxValue'
minWeight='./bin/minWeight'
bestRatio='./bin/bestRatio'

# Loop para rodar os algoritmos para cada entrada
for input in "${inputs[@]}"; do
    echo "Processing $input" >> $output_file
    echo " " >> $output_file
    $maxValue < $input >> $output_file
    echo " " >> $output_file
    $minWeight < $input >> $output_file
    echo " " >> $output_file
    $bestRatio < $input >> $output_file
    echo "------------------------------------------------------------------------------------------------------------" >> $output_file
done