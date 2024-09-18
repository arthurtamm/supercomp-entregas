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

// Função para substituir itens e melhorar a solução
tuple<int, int, vector<int>> knapsack_swap_method(vector<item>& items, int capacity, vector<int> selected_items) {
    bool improved = true;
    int current_capacity = 0;
    int max_value = 0;
    
    // Calcular capacidade e valor atuais
    for (int id : selected_items) {
        current_capacity += items[id].weight;
        max_value += items[id].value;
    }

    // Loop até não encontrar mais melhorias
    while (improved) {
        improved = false;
        vector<int> new_selected_items = selected_items;

        // Verificar se há melhorias possíveis substituindo itens
        for (int i = 0; i < items.size(); i++) {
            if (find(selected_items.begin(), selected_items.end(), i) == selected_items.end()) {
                for (int j = 0; j < selected_items.size(); j++) {
                    int id_j = selected_items[j];
                    int temp_capacity = current_capacity - items[id_j].weight + items[i].weight;
                    int temp_value = max_value - items[id_j].value + items[i].value;

                    if (temp_capacity <= capacity && temp_value > max_value) {
                        // Realiza a troca
                        new_selected_items[j] = items[i].id;
                        current_capacity = temp_capacity;
                        max_value = temp_value;
                        improved = true;
                        break;
                    }
                }
            }
            if (improved) break;
        }

        if (improved) {
            selected_items = new_selected_items;
        }
    }

    // Recalcular o peso final
    int final_weight = 0;
    for (int id : selected_items) {
        final_weight += items[id].weight;
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
    auto start = high_resolution_clock::now();
    auto result_shuffle = knapsack_shuffle_method(items, capacity);
    auto result_swap = knapsack_swap_method(items, capacity, get<2>(result_shuffle));
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    // Printar os resultados do método de substituição de itens
    cout << "Swap Method:" << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    cout << "Max value: " << get<0>(result_swap) << endl;
    cout << "Final weight: " << get<1>(result_swap) << endl;
    cout << "Items selected: ";
    for (int i : get<2>(result_swap)) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
