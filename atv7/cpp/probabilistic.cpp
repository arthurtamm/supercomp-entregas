#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

struct item {
    int id;
    int weight;
    int value;
};

tuple<int, int, vector<int>> knapsack_probabilistic_method(vector<item> items, int capacity) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    int current_weight = 0;
    int max_value = 0;
    vector<int> selected_items;
    const double threshold = 0.5; // Valor limiar definido na tarefa

    for (const auto& itm : items) {
        double probability = dis(gen);
        if (probability > threshold && current_weight + itm.weight <= capacity) {
            current_weight += itm.weight;
            max_value += itm.value;
            selected_items.push_back(itm.id);
        }
    }

    return make_tuple(max_value, current_weight, selected_items);
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

    // Executar e medir o tempo do método probabilístico
    auto start = high_resolution_clock::now();
    auto result = knapsack_probabilistic_method(items, capacity);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    int max_value = get<0>(result);
    int final_weight = get<1>(result);
    vector<int> items_selected = get<2>(result);

    cout << "Probabilistic Heuristic Method:" << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    cout << "Max value: " << max_value << endl;
    cout << "Final weight: " << final_weight << endl;
    cout << "Items selected: ";
    for (int i : items_selected) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
