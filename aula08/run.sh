#!/bin/bash

# Arquivo de saída
output_file="output.txt"

# Limpar o conteúdo do arquivo de saída, se já existir
> $output_file

# Lista de entradas
inputs=("input1.txt" "input2.txt" "input3.txt" "input4.txt")

# Caminho dos binários
swap_exec="./bin/swap"
hill_exec="./bin/hill"

# Função para encontrar o melhor valor em uma lista
find_best() {
    sort -n | tail -1
}

# Função para calcular a média
calculate_average() {
    values=("${!1}")
    total=0
    for value in "${values[@]}"; do
        total=$(echo "$total + $value" | bc)
    done
    avg=$(echo "scale=6; $total / ${#values[@]}" | bc)
    echo "$avg"
}

# Função para formatar a saída de cada algoritmo
print_algorithm_results() {
    algorithm_name=$1
    values=("${!2}") # Recebe o array de valores como referência
    times=("${!3}") # Recebe o array de tempos como referência
    best_value=$4
    avg_time=$5

    echo "---------------------------------------" >> $output_file
    echo "$algorithm_name results:" >> $output_file
    echo "---------------------------------------" >> $output_file

    for value in "${values[@]}"; do
        echo "$value" >> $output_file
    done

    echo "---------------------------------------" >> $output_file
    echo "$algorithm_name best value: $best_value" >> $output_file
    echo "$algorithm_name average execution time: $avg_time seconds" >> $output_file
    echo "" >> $output_file
}

# Rodar os algoritmos para cada entrada
for input in "${inputs[@]}"; do
    echo "*******************************************" >> $output_file
    echo "Results for input: $input" >> $output_file
    echo "*******************************************" >> $output_file
    input_path="./inputs/$input"

    # Rodar swap 10 vezes
    swap_values=()
    swap_times=()
    echo "Executing Swap Method..." >> $output_file
    for i in {1..10}; do
        start_time=$(date +%s.%N)
        result=$($swap_exec < "$input_path")
        end_time=$(date +%s.%N)

        exec_time=$(echo "$end_time - $start_time" | bc)
        swap_times+=($exec_time)

        echo "Run $i - $result (Execution time: $exec_time seconds)" >> $output_file
        swap_values+=($(echo "$result" | grep "Max value" | awk '{print $3}'))
    done
    best_swap=$(echo "${swap_values[@]}" | tr ' ' '\n' | find_best)
    avg_swap_time=$(calculate_average swap_times[@])
    print_algorithm_results "swap" swap_values[@] swap_times[@] "$best_swap" "$avg_swap_time"

    # Rodar hill 10 vezes
    hill_values=()
    hill_times=()
    echo "Executing Hill Climbing Method..." >> $output_file
    for i in {1..10}; do
        start_time=$(date +%s.%N)
        result=$($hill_exec < "$input_path")
        end_time=$(date +%s.%N)

        exec_time=$(echo "$end_time - $start_time" | bc)
        hill_times+=($exec_time)

        echo "Run $i - $result (Execution time: $exec_time seconds)" >> $output_file
        hill_values+=($(echo "$result" | grep "Max value" | awk '{print $3}'))
    done
    best_hill=$(echo "${hill_values[@]}" | tr ' ' '\n' | find_best)
    avg_hill_time=$(calculate_average hill_times[@])
    print_algorithm_results "hill" hill_values[@] hill_times[@] "$best_hill" "$avg_hill_time"

done

echo "Results saved in file $output_file"
