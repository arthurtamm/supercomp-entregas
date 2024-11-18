#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;


vector<int> findMaxClique(const vector<vector<int>> &graph, int nVertices){
    vector<int> maxClique;
    vector<int> candidates;

    for(int i = 0; i < nVertices; i++){
        candidates.push_back(i);
    }

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::ifstream file(fileName);

    int nVertices;

    vector<vector<int>> graph = readGraph(fileName, nVertices);

    vector<int> maxClique = findMaxClique(graph, nVertices);

    cout << "Clique máximo: ";
    for (int v : maxClique) {
        cout << (v + 1) << " ";  // Ajusta para exibir índices a partir de 1
    }
    cout << endl;

    return 0;
}