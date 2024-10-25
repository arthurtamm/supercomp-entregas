#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <algorithm>
#include <random>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct item {
    int id;
    int weight;
    int value;
};

bool compareByValue(const item &a, const item &b) {
    return a.value > b.value;
}

// 
tuple<int, int, vector<int>> knapsack_max_value_method(vector<item> items, int capacity) {
    int current_capacity = 0;
    vector<int> selected_items;
    int max_value = 0;
    int final_weight = 0;
    sort(items.begin(), items.end(), compareByValue);

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

    // Gerar a solução inicial com o método de embaralhamento
    auto start = high_resolution_clock::now();
    auto result = knapsack_max_value_method(items, capacity);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    // Printar os resultados do Max Value Method
    cout << "Max Value Method:" << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    cout << "Max value: " << get<0>(result) << endl;
    cout << "Final weight: " << get<1>(result) << endl;
    cout << "Items selected: ";
    // Para cada índice onde o valor da solução binária é 1, printar o índice do item selecionado
    for (int id : get<2>(result)) {
        cout << id << " ";  // Printar os IDs dos itens selecionados
    }
    cout << endl;

    return 0;
}
