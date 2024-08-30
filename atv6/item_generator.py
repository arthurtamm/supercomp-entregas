import random
import sys

def generate_knapsack_file(n, max_weight, max_value, capacity, filename):
    with open(filename, 'w') as file:
        # Escreve a primeira linha com o número de itens e a capacidade da mochila
        file.write(f"{n} {capacity}\n")
        
        # Gera os itens com peso e valor aleatórios
        for _ in range(n):
            weight = random.randint(1, max_weight)
            value = random.randint(1, max_value)
            file.write(f"{weight} {value}\n")

if __name__ == "__main__":
    # Número de itens
    n = 36
    # n = int(sys.argv[1])
    
    # Peso máximo dos itens
    max_weight = 10
    
    # Valor máximo dos itens
    max_value = 100
    
    # Capacidade da mochila
    capacity = 50
    
    # Nome do arquivo de saída
    filename = f"./inputs/input{n}.txt"
    generate_knapsack_file(n, max_weight, max_value, capacity, filename)
    
    # Gera o arquivo
    # for i in range(9, 39, 3):
    #     filename = f"./inputs/input{i}.txt"
    #     generate_knapsack_file(i, max_weight, max_value, capacity, filename)
    
    print(f"Arquivo {filename} gerado com sucesso.")
