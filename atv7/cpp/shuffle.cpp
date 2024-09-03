#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
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

    // Executar e medir o tempo do método de embaralhamento
    auto start_shuffle = high_resolution_clock::now();
    auto result_shuffle = knapsack_shuffle_method(items, capacity);
    auto end_shuffle = high_resolution_clock::now();
    duration<double> duration_shuffle = end_shuffle - start_shuffle;

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
    
    return 0;
}