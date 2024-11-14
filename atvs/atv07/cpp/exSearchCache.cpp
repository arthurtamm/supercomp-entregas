#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct item {
    int weight;
    int value;
};

struct knapsack_result {
    int max_value;
    int final_weight;
    vector<int> selected_items;
};

// Função recursiva para resolver a mochila binária com memoization
knapsack_result knapsack_recursive(const vector<item>& items, int capacity, int index, vector<vector<knapsack_result>>& cache) {
    if (index == items.size() || capacity == 0) {
        return {0, 0, {}};
    }

    if (cache[index][capacity].max_value != -1) {
        return cache[index][capacity];
    }

    // Caso onde não incluímos o item atual
    knapsack_result exclude = knapsack_recursive(items, capacity, index + 1, cache);

    knapsack_result include = {0, 0, {}};
    if (items[index].weight <= capacity) {
        include = knapsack_recursive(items, capacity - items[index].weight, index + 1, cache);
        include.max_value += items[index].value;
        include.final_weight += items[index].weight;
        include.selected_items.push_back(index);
    }

    knapsack_result result;
    if (include.max_value > exclude.max_value) {
        result = include;
    } else {
        result = exclude;
    }

    cache[index][capacity] = result;

    return result;
}

// Função principal para chamar a recursiva e encontrar a melhor solução
tuple<int, int, vector<int>> knapsack(const vector<item>& items, int capacity) {
    vector<vector<knapsack_result>> cache(items.size(), vector<knapsack_result>(capacity + 1, {-1, 0, {}}));

    knapsack_result result = knapsack_recursive(items, capacity, 0, cache);

    return make_tuple(result.max_value, result.final_weight, result.selected_items);
}

int main() {
    int n, capacity;

    cin >> n >> capacity;

    vector<item> items(n);
    for (int i = 0; i < n; i++) {
        cin >> items[i].weight >> items[i].value;
    }

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
