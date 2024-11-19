#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <numeric>

using namespace std;
using namespace std::chrono;

void backtrack(const vector<vector<int>>& graph, vector<int>& currentClique, vector<int>& maxClique, vector<int>& candidates) {
    if (candidates.empty()) {
        if (currentClique.size() > maxClique.size()) {
            maxClique = currentClique;
        }
        return;
    }

    for (size_t i = 0; i < candidates.size(); ++i) {
        int v = candidates[i];

        // Verifica se v pode ser adicionado à clique atual
        bool canAdd = true;
        for (int u : currentClique) {
            if (graph[u][v] == 0) {
                canAdd = false;
                break;
            }
        }

        if (canAdd) {
            currentClique.push_back(v);

            // Cria novos candidatos
            vector<int> newCandidates;
            for (size_t j = i + 1; j < candidates.size(); ++j) {
                int u = candidates[j];
                bool adjacentToAll = true;

                for (int c : currentClique) {
                    if (graph[u][c] == 0) {
                        adjacentToAll = false;
                        break;
                    }
                }

                if (adjacentToAll) {
                    newCandidates.push_back(u);
                }
            }

            // Chama a recursão
            backtrack(graph, currentClique, maxClique, newCandidates);

            // Remove o nó atual da clique
            currentClique.pop_back();
        }
    }
}

vector<int> findMaxClique(const vector<vector<int>> &graph, int nVertices) {
    vector<int> maxClique;
    vector<int> candidates(nVertices);
    iota(candidates.begin(), candidates.end(), 0);  // Inicializa candidatos com [0, 1, ..., nVertices-1]

    vector<int> currentClique;
    backtrack(graph, currentClique, maxClique, candidates);

    return maxClique;
}

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> readGraph(const std::string& fileName, int& nVertices) {
    ifstream file(fileName);
    int nEdges;
    file >> nVertices >> nEdges;

    std::vector<std::vector<int>> graph(nVertices, std::vector<int>(nVertices, 0));

    for (int i = 0; i < nEdges; ++i) {
        int u, v;
        file >> u >> v;
        graph[u - 1][v - 1] = 1;
        graph[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    file.close();

    return graph;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];

    int nVertices;
    auto start = high_resolution_clock::now();

    vector<vector<int>> graph = readGraph(fileName, nVertices);

    vector<int> maxClique = findMaxClique(graph, nVertices);

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    int size_maxClique = 0;
    cout << "Clique máximo: ";
    for (int v : maxClique) {
        cout << (v + 1) << " ";  // Ajusta para exibir índices a partir de 1
        size_maxClique ++;
    }
    cout << endl;
    cout << "Temanho do clique máximo encontrado: " << size_maxClique << endl;
    cout << "Tempo de execução: " << duration.count() << " seconds" << endl;

    return 0;
}
