#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

void backtrack(const vector<vector<int>>& graph, vector<int>& currentClique, vector<int>& maxClique,
               const vector<int>& candidates, int rank, int size, bool isRoot) {
    if (candidates.empty()) {
        if (currentClique.size() > maxClique.size()) {
            maxClique = currentClique;
        }
        return;
    }

    for (size_t i = 0; i < candidates.size(); ++i) {
        if (isRoot && (i % size != rank)) {
            continue;  // Cada processo processa apenas seus candidatos designados no nível raiz
        }

        int v = candidates[i];

        // Verifica se v pode ser adicionado ao clique atual
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
            backtrack(graph, currentClique, maxClique, newCandidates, rank, size, false);

            // Remove o nó atual do clique
            currentClique.pop_back();
        }
    }
}

vector<int> sortNodes(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<pair<int, int>> degrees;

    for (int i = 0; i < n; ++i) {
        int degree = count(graph[i].begin(), graph[i].end(), 1);
        degrees.push_back({degree, i});
    }

    // Ordena pelo grau
    sort(degrees.begin(), degrees.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first;
    });

    vector<int> sortedNodes;
    for (const auto& p : degrees) {
        sortedNodes.push_back(p.second);
    }

    return sortedNodes;
}

vector<int> findMaxCliqueMPI(const vector<vector<int>>& graph, const vector<int>& sortedNodes, int rank, int size) {
    vector<int> localMaxClique;
    vector<int> currentClique;
    vector<int> candidates = sortedNodes;

    backtrack(graph, currentClique, localMaxClique, candidates, rank, size, true);

    // Encontra o tamanho do clique máximo global e o rank do processo que o encontrou
    int localMaxSize = localMaxClique.size();
    struct {
        int size;
        int rank;
    } localData = {localMaxSize, rank}, globalData;

    MPI_Allreduce(&localData, &globalData, 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);

    // O processo com o clique máximo global envia os dados para os outros
    vector<int> maxClique;
    if (rank == globalData.rank) {
        maxClique = localMaxClique;
    }

    // Broadcast do tamanho do clique máximo
    int cliqueSize = maxClique.size();
    MPI_Bcast(&cliqueSize, 1, MPI_INT, globalData.rank, MPI_COMM_WORLD);

    // Broadcast dos dados do clique máximo
    maxClique.resize(cliqueSize);
    MPI_Bcast(maxClique.data(), cliqueSize, MPI_INT, globalData.rank, MPI_COMM_WORLD);

    return maxClique;
}

vector<vector<int>> readGraph(const string& fileName, int& nVertices) {
    ifstream file(fileName);
    int nEdges;
    file >> nVertices >> nEdges;

    vector<vector<int>> graph(nVertices, vector<int>(nVertices, 0));

    for (int i = 0; i < nEdges; ++i) {
        int u, v;
        file >> u >> v;
        graph[u - 1][v - 1] = 1;
        graph[v - 1][u - 1] = 1; // Grafo não direcionado
    }

    file.close();
    return graph;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << endl;
        MPI_Finalize();
        return 1;
    }

    string fileName = argv[1];

    int nVertices, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto start = high_resolution_clock::now();

    // Todos os processos leem o grafo
    vector<vector<int>> graph = readGraph(fileName, nVertices);

    vector<int> sortedNodes = sortNodes(graph);
    vector<int> maxClique = findMaxCliqueMPI(graph, sortedNodes, rank, size);

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    if (rank == 0) {
        cout << "Clique máximo: ";
        for (int v : maxClique) {
            cout << (v + 1) << " ";
        }
        cout << endl;
        cout << "Tamanho do clique máximo encontrado: " << maxClique.size() << endl;
        cout << "Tempo de execução: " << duration.count() << " segundos" << endl;
    }

    MPI_Finalize();
    return 0;
}
