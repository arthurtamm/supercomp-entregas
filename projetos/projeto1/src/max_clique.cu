#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/functional.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Função para ler o grafo do arquivo
vector<vector<int>> readGraph(const string& fileName, int& nVertices) {
    ifstream file(fileName);

    if (!file) {
        cerr << "Error opening file: " << fileName << endl;
        exit(1);
    }

    int nEdges;
    file >> nVertices >> nEdges;

    vector<vector<int>> graph(nVertices, vector<int>(nVertices, 0));

    for (int i = 0; i < nEdges; ++i) {
        int u, v;
        file >> u >> v;
        graph[u - 1][v - 1] = 1;
        graph[v - 1][u - 1] = 1;  // Grafo não direcionado
    }

    file.close();
    return graph;
}

// Função para performar o algoritmo de Bron-Kerbosch
void bronKerbosch(vector<vector<int>>& graph, vector<int>& R, vector<int>& P, vector<int>& X, vector<int>& maxClique) {
    if (P.empty() && X.empty()) {
        // Encontrou um clique maximal
        if (R.size() > maxClique.size()) {
            maxClique = R;
        }
        return;
    }

    // Escolha do pivô: primeiro vértice de P ∪ X
    int u = -1;
    vector<int> union_PX = P;
    union_PX.insert(union_PX.end(), X.begin(), X.end());
    if (!union_PX.empty()) {
        u = union_PX[0];
    }

    // P \ N(u)
    vector<int> P_without_Nu;
    for (int v : P) {
        if (graph[u][v] == 0) {
            P_without_Nu.push_back(v);
        }
    }

    for (int v : P_without_Nu) {
        // R ∪ {v}
        R.push_back(v);

        // P ∩ N(v)
        vector<int> P_intersect_Nv;
        for (int w : P) {
            if (graph[v][w] == 1) {
                P_intersect_Nv.push_back(w);
            }
        }

        // X ∩ N(v)
        vector<int> X_intersect_Nv;
        for (int w : X) {
            if (graph[v][w] == 1) {
                X_intersect_Nv.push_back(w);
            }
        }

        // Chamada recursiva
        bronKerbosch(graph, R, P_intersect_Nv, X_intersect_Nv, maxClique);

        // R \ {v}
        R.pop_back();

        // P \ {v}
        P.erase(remove(P.begin(), P.end(), v), P.end());

        // X ∪ {v}
        X.push_back(v);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <graph_file>" << endl;
        return 1;
    }

    string fileName = argv[1];

    int nVertices;
    auto start = high_resolution_clock::now();

    vector<vector<int>> graph = readGraph(fileName, nVertices);

    vector<int> R;             // Clique atual
    vector<int> P(nVertices);  // Candidatos a entrar no clique
    vector<int> X;             // Vertices já processados

    // Inicialize P com todos os vértices
    for (int i = 0; i < nVertices; ++i) {
        P[i] = i;
    }

    vector<int> maxClique;

    // Rode o algoritmo de Bron-Kerbosch
    bronKerbosch(graph, R, P, X, maxClique);

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    cout << "Maximum Clique: ";
    for (int v : maxClique) {
        cout << (v + 1) << " ";  // Ajustando o índice para começar em 1
    }
    cout << endl;
    cout << "Size of Maximum Clique: " << maxClique.size() << endl;
    cout << "Execution Time: " << duration.count() << " seconds" << endl;

    return 0;
}
