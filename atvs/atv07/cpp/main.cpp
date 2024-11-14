#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <chrono>
#include <algorithm>
#include <random> 

using namespace std;
using namespace std::chrono;

struct item {
    int weight;
    int value;
};

// Estrutura para armazenar o resultado da mochila
struct knapsack_result {
    int max_value;
    int final_weight;
    vector<int> selected_items;
};

// Função recursiva para resolver a mochila binária
knapsack_result knapsack_recursive(const vector<item>& items, int capacity, int index, int current_weight, int current_value, vector<int>& selected_items) {
    if (index == items.size()) {
        return {current_value, current_weight, selected_items};
    }

    // Caso onde não incluímos o item atual
    knapsack_result exclude = knapsack_recursive(items, capacity, index + 1, current_weight, current_value, selected_items);

    knapsack_result include = {0, 0, {}};
    if (current_weight + items[index].weight <= capacity) {
        selected_items.push_back(index);
        include = knapsack_recursive(items, capacity, index + 1, current_weight + items[index].weight, current_value + items[index].value, selected_items);
        selected_items.pop_back(); // Volta ao estado anterior
    }

    // Retornar a melhor solução entre incluir e não incluir o item atual
    if (include.max_value > exclude.max_value) {
        return include;
    } else {
        return exclude;
    }
}

// Função principal para chamar a recursiva e encontrar a melhor solução
tuple<int, int, vector<int>> knapsack_recursive_method(const vector<item>& items, int capacity) {
    vector<int> selected_items;
    knapsack_result result = knapsack_recursive(items, capacity, 0, 0, 0, selected_items);
    return make_tuple(result.max_value, result.final_weight, result.selected_items);
}

// Função para rodar o método de embaralhamento e preenchimento sequencial
tuple<int, int, vector<int>> knapsack_shuffle_method(vector<item> items, int capacity) {
    // Criar um gerador de números aleatórios
    random_device rd;  // Fonte de entropia
    mt19937 g(rd());   // Gerador Mersenne Twister inicializado com rd

    // Embaralhar o vetor
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



int main() {
    int n, capacity;

    // Ler a primeira linha para obter n e capacity via cin
    cin >> n >> capacity;

    vector<item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].id = i;
        cin >> items[i].weight >> items[i].value;
    }

    // Executar e medir o tempo do método recursivo
    auto start_recursive = high_resolution_clock::now();
    auto result_recursive = knapsack_recursive_method(items, capacity);
    auto end_recursive = high_resolution_clock::now();
    duration<double> duration_recursive = end_recursive - start_recursive;

    // Executar e medir o tempo do método de embaralhamento
    auto start_shuffle = high_resolution_clock::now();
    auto result_shuffle = knapsack_shuffle_method(items, capacity);
    auto end_shuffle = high_resolution_clock::now();
    duration<double> duration_shuffle = end_shuffle - start_shuffle;

    // Printar os resultados do método recursivo
    cout << "Recursive Method:" << endl;
    cout << "Execution time: " << duration_recursive.count() << " seconds" << endl;
    cout << "Max value: " << get<0>(result_recursive) << endl;
    cout << "Final weight: " << get<1>(result_recursive) << endl;
    cout << "Items selected: ";
    for (int i : get<2>(result_recursive)) {
        cout << i << " ";
    }
    cout << endl << endl;
    cout << "-----------------------------------------------------------------------" << endl;

    // Printar os resultados do método de embaralhamento
    cout << "Shuffle Method:" << endl;
    cout << "Execution time: " << duration_shuffle.count() << " seconds" << endl;
    cout << "Max value: " << get<0>(result_shuffle) << endl;
    cout << "Final weight: " << get<1>(result_shuffle) << endl;
    cout << "Items selected: ";
    for (int i : get<2>(result_shuffle)) {
        cout << i << " ";
    }
    cout << endl;
    cout << "-----------------------------------------------------------------------" << endl;

    return 0;
}