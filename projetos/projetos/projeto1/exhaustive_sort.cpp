#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;


vector<int> findMaxClique(const vector<vector<int>> &graph, const vector<int>& sortedNodes){
    vector<int> maxClique;
    vector<int> candidates = sortedNodes;

    while(!candidates.empty()){
        int v = candidates.back();
        candidates.pop_back();

        bool canAdd = true;

        for(int u : maxClique){
            if(graph[u][v] == 0){
                canAdd = false;
                break;
            }
        }

        if (canAdd){
            maxClique.push_back(v);
            vector<int> newCandidates;

            for(int u : candidates){
                bool adjacentToAll = true;

                for(int c : maxClique){
                    if(graph[u][c] == 0){
                        adjacentToAll = false;
                        break;
                    }
                }
                
                if(adjacentToAll){
                    newCandidates.push_back(u);
                }
            }

            candidates = newCandidates;
        }
    }

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

// Função para ordenar os nós por grau de adjacência em ordem decrescente
vector<int> sortNodesByDegree(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<pair<int, int>> degrees;  // (grau, nó)

    // Calcula o grau de cada nó
    for (int i = 0; i < n; ++i) {
        int degree = 0;
        for (int j = 0; j < n; ++j) {
            if (graph[i][j] == 1) {
                degree++;
            }
        }
        degrees.push_back({degree, i});
    }

    // Ordena pelo grau de forma decrescente
    sort(degrees.begin(), degrees.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first;
    });

    // Extrai apenas os índices dos nós
    vector<int> sortedNodes;
    for (const auto& p : degrees) {
        sortedNodes.push_back(p.second);
    }

    return sortedNodes;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::ifstream file(fileName);

    int nVertices;

    vector<vector<int>> graph = readGraph(fileName, nVertices);

    // Ordena os nós por grau de adjacência
    vector<int> sortedNodes = sortNodesByDegree(graph);

    vector<int> maxClique = findMaxClique(graph, sortedNodes);

    cout << "Clique máximo: ";
    for (int v : maxClique) {
        cout << (v + 1) << " ";  // Ajusta para exibir índices a partir de 1
    }
    cout << endl;

    return 0;
}