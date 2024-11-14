#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <algorithm>
#include <random> 

using namespace std;
using namespace std::chrono;

struct item {
    int id;
    int weight;
    int value;
};

// Função para gerar uma solução inicial com o método de embaralhamento
tuple<int, int, vector<int>> knapsack_shuffle_method(vector<item> items, int capacity) {
    random_device rd;
    mt19937 g(rd());
    shuffle(items.begin(), items.end(), g);

    int current_capacity = 0;
    vector<int> selected_items;
    int max_value = 0;
    int final_weight = 0;
    for (int i = 0; i < items.size(); i++) {
        if (items[i].weight + current_capacity <= capacity) {
            current_capacity += items[i].weight;
            selected_items.push_back(items[i].id);
            max_value += items[i].value;
            final_weight += items[i].weight;
        }
    }

    return make_tuple(max_value, final_weight, selected_items);
}

// Função para calcular o valor e o peso de uma solução binária (string de bits)
tuple<int, int> evaluate_solution(vector<int>& solution, vector<item>& items, int capacity) {
    int total_value = 0;
    int total_weight = 0;
    for (int i = 0; i < solution.size(); i++) {
        if (solution[i] == 1) {
            total_weight += items[i].weight;
            total_value += items[i].value;
        }
    }

    // Se a solução ultrapassar a capacidade, o valor é 0 (solução inválida)
    if (total_weight > capacity) {
        return make_tuple(0, total_weight);
    }
    return make_tuple(total_value, total_weight);
}

// Função para gerar vizinhos trocando um bit
vector<vector<int>> generate_neighbors(vector<int> solution) {
    vector<vector<int>> neighbors;
    for (int i = 0; i < solution.size(); i++) {
        vector<int> neighbor = solution;
        neighbor[i] = 1 - neighbor[i]; // Troca de 0 para 1 ou de 1 para 0
        neighbors.push_back(neighbor);
    }
    return neighbors;
}

// Função Hill Climbing para otimização
tuple<int, int, vector<int>> knapsack_hill_climbing_method(vector<item>& items, int capacity, vector<int> initial_solution) {
    vector<int> current_solution = initial_solution;
    int current_value, current_weight;
    tie(current_value, current_weight) = evaluate_solution(current_solution, items, capacity);

    bool improved = true;
    while (improved) {
        improved = false;
        vector<vector<int>> neighbors = generate_neighbors(current_solution);

        for (auto& neighbor : neighbors) {
            int neighbor_value, neighbor_weight;
            tie(neighbor_value, neighbor_weight) = evaluate_solution(neighbor, items, capacity);

            // Se o vizinho for melhor, atualiza a solução
            if (neighbor_value > current_value) {
                current_solution = neighbor;
                current_value = neighbor_value;
                current_weight = neighbor_weight;
                improved = true;
                break;
            }
        }
    }

    return make_tuple(current_value, current_weight, current_solution);
}

int main() {
    int n, capacity;

    // Ler a primeira linha para obter n e capacity via cin
    cin >> n >> capacity;

    vector<item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].id = i;
        cin >> items[i].weight >> items[i].value;
    }

    // Gerar a solução inicial com o método de embaralhamento
    auto start = high_resolution_clock::now();
    auto result_shuffle = knapsack_shuffle_method(items, capacity);

    // Criar uma string binária para a solução inicial
    vector<int> initial_solution(items.size(), 0);
    for (int id : get<2>(result_shuffle)) {
        initial_solution[id] = 1;
    }

    // Executar o Hill Climbing
    auto result_hill_climbing = knapsack_hill_climbing_method(items, capacity, initial_solution);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    // Printar os resultados do Hill Climbing
    cout << "Hill Climbing Method:" << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    cout << "Max value: " << get<0>(result_hill_climbing) << endl;
    cout << "Final weight: " << get<1>(result_hill_climbing) << endl;
    cout << "Items selected: ";
    // Para cada índice onde o valor da solução binária é 1, printar o índice do item selecionado
    for (int i = 0; i < get<2>(result_hill_climbing).size(); i++) {
        if (get<2>(result_hill_climbing)[i] == 1) {
            cout << i << " "; // Printar os índices dos itens selecionados
        }
    }
    cout << endl;

    return 0;
}
