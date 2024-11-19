#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <omp.h>  // Inclui a biblioteca OpenMP

using namespace std;
using namespace std::chrono;

// Função recursiva de backtracking
void backtrack(const vector<vector<int>>& graph, vector<int>& currentClique, vector<int>& localMaxClique, const vector<int>& candidates) {
    if (candidates.empty()) {
        if (currentClique.size() > localMaxClique.size()) {
            localMaxClique = currentClique;
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
            backtrack(graph, currentClique, localMaxClique, newCandidates);

            // Remove o nó atual da clique
            currentClique.pop_back();
        }
    }
}

// Ordena os nós por grau e randomiza
vector<int> sortAndRandomizeNodes(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<pair<int, int>> degrees;  // (grau, nó)

    for (int i = 0; i < n; ++i) {
        int degree = count(graph[i].begin(), graph[i].end(), 1);
        degrees.push_back({degree, i});
    }

    // Ordena pelo grau
    sort(degrees.begin(), degrees.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first;
    });

    // Extrai índices e randomiza
    vector<int> sortedNodes;
    for (const auto& p : degrees) {
        sortedNodes.push_back(p.second);
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(sortedNodes.begin(), sortedNodes.end(), g);

    return sortedNodes;
}

// Função para encontrar o clique máximo utilizando OpenMP
vector<int> findMaxCliqueParallel(const vector<vector<int>>& graph, const vector<int>& sortedNodes) {
    vector<int> globalMaxClique;
    int nThreads = omp_get_max_threads();

    // Vetor para armazenar o clique máximo de cada thread
    vector<vector<int>> localMaxCliques(nThreads);

    #pragma omp parallel
    {
        int threadNum = omp_get_thread_num();
        vector<int>& localMaxClique = localMaxCliques[threadNum];
        vector<int> currentClique;

        // Cada thread processa um subconjunto dos nós iniciais
        #pragma omp for schedule(dynamic)
        for (size_t i = 0; i < sortedNodes.size(); ++i) {
            int v = sortedNodes[i];

            currentClique.push_back(v);

            // Cria candidatos para o nó atual
            vector<int> newCandidates;
            for (size_t j = i + 1; j < sortedNodes.size(); ++j) {
                int u = sortedNodes[j];
                if (graph[v][u] == 1) {
                    newCandidates.push_back(u);
                }
            }

            // Chama a função de backtracking para cada nó inicial
            backtrack(graph, currentClique, localMaxClique, newCandidates);

            currentClique.pop_back();
        }
    }

    // Combina os resultados das threads para encontrar o clique máximo global
    for (const auto& clique : localMaxCliques) {
        if (clique.size() > globalMaxClique.size()) {
            globalMaxClique = clique;
        }
    }

    return globalMaxClique;
}

// Função para ler o grafo
vector<vector<int>> readGraph(const string& fileName, int& nVertices) {
    ifstream file(fileName);
    int nEdges;
    file >> nVertices >> nEdges;

    vector<vector<int>> graph(nVertices, vector<int>(nVertices, 0));

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
        cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << endl;
        return 1;
    }

    string fileName = argv[1];

    int nVertices;

    auto start = high_resolution_clock::now();

    vector<vector<int>> graph = readGraph(fileName, nVertices);

    vector<int> sortedNodes = sortAndRandomizeNodes(graph);

    vector<int> maxClique = findMaxCliqueParallel(graph, sortedNodes);

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    cout << "Clique máximo: ";
    for (int v : maxClique) {
        cout << (v + 1) << " ";  // Ajusta para exibir índices a partir de 1
    }
    cout << endl;
    cout << "Tamanho do clique máximo encontrado: " << maxClique.size() << endl;
    cout << "Tempo de execução: " << duration.count() << " segundos" << endl;

    return 0;
}
