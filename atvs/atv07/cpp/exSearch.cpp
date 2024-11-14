#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <chrono>

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
tuple<int, int, vector<int>> knapsack(const vector<item>& items, int capacity) {
    vector<int> selected_items;
    knapsack_result result = knapsack_recursive(items, capacity, 0, 0, 0, selected_items);
    return make_tuple(result.max_value, result.final_weight, result.selected_items);
}

int main() {
    int n, capacity;

    // Ler a primeira linha para obter n e capacity via cin
    cin >> n >> capacity;

    vector<item> items(n);
    for (int i = 0; i < n; i++) {
        cin >> items[i].weight >> items[i].value;
    }

    // Medir o tempo de execução
    auto start = high_resolution_clock::now();
    auto result = knapsack(items, capacity);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    int max_value = get<0>(result);
    int final_weight = get<1>(result);
    vector<int> items_selected = get<2>(result);

    cout << "Execution time for " << n << " items: " << duration.count() << " seconds" << endl;
    cout << "Max value: " << max_value << endl;
    cout << "Final weight: " << final_weight << endl;
    cout << "Items selected: ";
    for (int i : items_selected) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}