#include <iostream>
#include <fstream>
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
knapsack_result knapsack_recursive(const std::vector<item>& items, int capacity, int index, int current_weight, int current_value, vector<int> selected_items) {
    if (index == items.size()) {
        return {current_value, current_weight, selected_items};
    }

    // Caso onde não incluímos o item atual
    knapsack_result exclude = knapsack_recursive(items, capacity, index + 1, current_weight, current_value, selected_items);

    knapsack_result include = {0, 0, {}};
    if (current_weight + items[index].weight <= capacity) {
        selected_items.push_back(index);
        include = knapsack_recursive(items, capacity, index + 1, current_weight + items[index].weight, current_value + items[index].value, selected_items);
    }

    // Retornar a melhor solução entre incluir e não incluir o item atual
    if (include.max_value > exclude.max_value) {
        return include;
    } else {
        return exclude;
    }
}

// Função principal para chamar a recursiva e encontrar a melhor solução
tuple<int, int, vector<int>> knapsack(const std::vector<item>& items, int capacity) {
    vector<int> selected_items;
    knapsack_result result = knapsack_recursive(items, capacity, 0, 0, 0, selected_items);
    return make_tuple(result.max_value, result.final_weight, result.selected_items);
}

tuple<int,int,int> naive_knapsack(const std::vector<item>& items, int capacity) {
    int n = items.size();
    int max_value = 0;
    int items_selected = 0;
    int final_weight = 0;

    // Total possible combinations is 2^n
    int total_combinations = 1 << n;

    for (int i = 0; i < total_combinations; i++) {
        int current_weight = 0;
        int current_value = 0;

        for (int j = 0; j < n; j++) {
            // Check if the j-th item is included in the i-th combination
            if (i & (1 << j)) {
                current_weight += items[j].weight;
                current_value += items[j].value;
            }
        }

        // Check if the current combination is valid and update max_value
        if (current_weight <= capacity && current_value > max_value) {
            max_value = current_value;
            final_weight = current_weight;
            items_selected = i;
        }
    }

    return make_tuple(max_value, final_weight, items_selected);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << argv[1] << std::endl;
        return 1;
    }

    string line;
    getline(file, line);
    istringstream iss(line);
    int n, capacity;
    iss >> n >> capacity;

    // cout << "Knapsack size: " << capacity << " ";
    // cout << "Number of items: " << n << endl;
    vector<item> items;
    for (int i=0; i<n; i++) {
        getline(file, line);
        istringstream iss(line);
        item item;
        iss >> item.weight >> item.value;
        items.push_back(item);
    }

    // tuple<int, int, int> result = knapsack(items, capacity);
    // int max_value = get<0>(result);
    // int final_weight = get<1>(result);
    // int items_selected = get<2>(result);
    // cout << "Max value: " << max_value << endl;
    // cout << "Final weight: " << final_weight << endl;
    // cout << "Items selected: ";
    // for (int i=0; i<n; i++) {
    //     if (items_selected & (1 << i)) {
    //         cout << i << " ";
    //     }
    // }
    // cout << endl;


    // Medir o tempo de execução
    auto start = high_resolution_clock::now();

    tuple<int, int, vector<int>> result = knapsack(items, capacity);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    int max_value = get<0>(result);
    int final_weight = get<1>(result);
    vector<int> items_selected = get<2>(result);

    // cout << "Max value: " << max_value << endl;
    // cout << "Final weight: " << final_weight << endl;
    // cout << "Items selected: ";
    // for (int i : items_selected) {
    //     cout << i << " ";
    // }
    // cout << endl;

    file.close();

    string output_file_name = "./outputs/output" + string(argv[1]).substr(14);
    cout << "Output file: " << output_file_name << endl;
    std::ofstream output_file(output_file_name);
    if (output_file.is_open()) {
        output_file << "Execution time for "<< n << " items: " << duration.count() << " milliseconds" << endl;
        output_file << endl;
        output_file.close();
    } else {
        std::cerr << "Error: Could not open the output file " << argv[2] << std::endl;
        return 1;
    }
    return 0;
}