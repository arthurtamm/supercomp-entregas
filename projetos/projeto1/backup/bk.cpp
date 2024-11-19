#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <chrono>

using namespace std;

// Função Bron-Kerbosch com limite de profundidade
void bronKerbosch(const vector<vector<int>>& graph, set<int>& R, set<int>& P, set<int>& X, vector<int>& maxClique, int depth = 0) {
    if (P.empty() && X.empty()) {
        if (R.size() > maxClique.size()) {
            maxClique.assign(R.begin(), R.end());
        }
        return;
    }

    // Escolhe um pivô arbitrário
    int u = *P.begin();
    set<int> PwithoutN;

    for (int v : P) {
        if (find(graph[u].begin(), graph[u].end(), v) == graph[u].end()) {
            PwithoutN.insert(v);
        }
    }

    for (int v : PwithoutN) {
        R.insert(v);

        // Calcula P ∩ N(v) e X ∩ N(v)
        set<int> Pn, Xn;
        for (int w : P) {
            if (find(graph[v].begin(), graph[v].end(), w) != graph[v].end()) {
                Pn.insert(w);
            }
        }
        for (int w : X) {
            if (find(graph[v].begin(), graph[v].end(), w) != graph[v].end()) {
                Xn.insert(w);
            }
        }

        bronKerbosch(graph, R, Pn, Xn, maxClique, depth + 1);

        R.erase(v);
        P.erase(v);
        X.insert(v);
    }
}

// Função principal para executar Bron-Kerbosch
vector<int> bronKerboschMain(const vector<vector<int>>& graph) {
    vector<int> maxClique;
    set<int> R, P, X;

    // Inicializa P com todos os vértices
    for (int i = 0; i < graph.size(); ++i) {
        P.insert(i);
    }

    bronKerbosch(graph, R, P, X, maxClique);
    return maxClique;
}

// Função para ler o grafo como lista de adjacência
vector<vector<int>> readGraph(const string& fileName, int& nVertices) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo " << fileName << endl;
        exit(1);
    }

    int nEdges;
    file >> nVertices >> nEdges;

    vector<vector<int>> graph(nVertices);
    int u, v;
    while (file >> u >> v) {
        graph[u - 1].push_back(v - 1);
        graph[v - 1].push_back(u - 1); // Grafo não direcionado
    }

    return graph;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string fileName = argv[1];
    int nVertices;
    vector<vector<int>> graph = readGraph(fileName, nVertices);

    auto start = chrono::high_resolution_clock::now();
    vector<int> maxClique = bronKerboschMain(graph);
    auto end = chrono::high_resolution_clock::now();

    // Calcula o tempo de execução
    chrono::duration<double> duration = end - start;

    cout << "Clique máximo: ";
    for (int v : maxClique) {
        cout << (v + 1) << " "; // Ajuste para índices começando em 1
    }
    cout << endl;
    cout << "Tamanho do clique máximo: " << maxClique.size() << endl;
    cout << "Tempo de execução: " << duration.count() << " segundos" << endl;

    return 0;
}
