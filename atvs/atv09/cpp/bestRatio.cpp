#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct item {
    int id;
    int weight;
    int value;
};

// Função de comparação para ordenar pela melhor relação valor/peso (value/weight)
bool compareByRatio(const item &a, const item &b) {
    double ratioA = static_cast<double>(a.value) / a.weight;
    double ratioB = static_cast<double>(b.value) / b.weight;
    return ratioA > ratioB;
}

// Heurística de melhor ratio (value/weight)
tuple<int, int, vector<int>> knapsack_best_ratio_method(vector<item> items, int capacity) {
    int current_capacity = 0;
    vector<int> selected_items;
    int max_value = 0;
    int final_weight = 0;
    
    // Ordenar os itens pela razão valor/peso
    sort(items.begin(), items.end(), compareByRatio);

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

    // Gerar a solução inicial com a heurística de melhor ratio
    auto start = high_resolution_clock::now();
    auto result = knapsack_best_ratio_method(items, capacity);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    // Printar os resultados do método de melhor ratio
    cout << "Best Ratio Method:" << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    cout << "Max value: " << get<0>(result) << endl;
    cout << "Final weight: " << get<1>(result) << endl;
    cout << "Items selected: ";
    
    // Printar os IDs dos itens selecionados
    for (int id : get<2>(result)) {
        cout << id << " ";
    }
    cout << endl;

    return 0;
}
