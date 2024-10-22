#!/bin/bash

# Função para rodar um programa 10 vezes e calcular médias
run_experiment() {
    program=$1  # Nome do programa a ser executado
    echo "Running $program 10 times..."

    total_time=0
    total_pi=0

    for i in {1..10}; do
        # Executa o programa e captura as saídas de tempo e pi
        output=$("./$program")
        pi=$(echo "$output" | grep "pi =" | awk '{print $3}')
        time=$(echo "$output" | grep "Time:" | awk '{print $2}')

        # Acumula os resultados para a média
        total_pi=$(echo "$total_pi + $pi" | bc -l)
        total_time=$(echo "$total_time + $time" | bc -l)
    done

    # Calcula as médias
    avg_pi=$(echo "$total_pi / 10" | bc -l)
    avg_time=$(echo "$total_time / 10" | bc -l)

    # Exibe os resultados
    echo "Average pi for $program: $avg_pi"
    echo "Average time for $program: $avg_time seconds"
    echo "------------------------------------"
}

# Compilar os programas (caso necessário)
g++ -o pi_serial pi_serial.cpp -fopenmp
g++ -o pi_parallel1 pi_parallel1.cpp -fopenmp
g++ -o pi_parallel2 pi_parallel2.cpp -fopenmp

# Rodar os experimentos
run_experiment "pi_serial"
run_experiment "pi_parallel1"
run_experiment "pi_parallel2"
