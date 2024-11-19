#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <mpi.h>

using namespace std;

const int MAX_DEPTH = 1000; // Limite de profundidade para recursão

// Função Bron-Kerbosch com limite de profundidade
void bronKerbosch(const vector<vector<int>>& graph, set<int>& R, set<int>& P, set<int>& X, vector<int>& maxClique, int depth = 0) {
    if (depth > MAX_DEPTH) {
        cerr << "Profundidade máxima atingida!" << endl;
        return;
    }

    if (P.empty() && X.empty()) {
        if (R.size() > maxClique.size()) {
            maxClique.assign(R.begin(), R.end());
        }
        return;
    }

    int u = *P.begin(); // Escolhe um pivô arbitrário
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

// Função para executar o Bron-Kerbosch usando MPI
vector<int> bronKerboschMPI(const vector<vector<int>>& graph) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = graph.size();
    vector<int> globalMaxClique, localMaxClique;

    // Divide os vértices entre os processos
    for (int v = rank; v < n; v += size) {
        set<int> R = {v};
        set<int> P, X;

        for (int u : graph[v]) {
            P.insert(u);
        }

        bronKerbosch(graph, R, P, X, localMaxClique);
    }

    // Reduz para encontrar o clique máximo global
    int localMaxSize = localMaxClique.size();
    int globalMaxSize;
    MPI_Allreduce(&localMaxSize, &globalMaxSize, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (localMaxSize == globalMaxSize) {
        globalMaxClique = localMaxClique;
    }

    return globalMaxClique;
}

// Função para ler o grafo como lista de adjacência
vector<vector<int>> readGraph(const string& fileName, int& nVertices) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<vector<int>> graph;

    if (rank == 0) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Erro ao abrir o arquivo " << fileName << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int nEdges;
        file >> nVertices >> nEdges;

        graph.resize(nVertices);
        int u, v;
        while (file >> u >> v) {
            graph[u - 1].push_back(v - 1);
            graph[v - 1].push_back(u - 1); // Grafo não direcionado
        }
    }

    // Broadcast do número de vértices
    MPI_Bcast(&nVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Enviar grafo como lista de adjacência
    if (rank != 0) {
        graph.resize(nVertices);
    }

    for (int i = 0; i < nVertices; ++i) {
        int listSize = (rank == 0) ? graph[i].size() : 0;
        MPI_Bcast(&listSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank != 0) {
            graph[i].resize(listSize);
        }

        MPI_Bcast(graph[i].data(), listSize, MPI_INT, 0, MPI_COMM_WORLD);
    }

    return graph;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    if (argc < 2) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0) {
            cerr << "Usage: " << argv[0] << " <filename>" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    string fileName = argv[1];
    int nVertices;
    vector<vector<int>> graph = readGraph(fileName, nVertices);

    double start = MPI_Wtime();
    vector<int> maxClique = bronKerboschMPI(graph);
    double end = MPI_Wtime();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        cout << "Clique máximo: ";
        for (int v : maxClique) {
            cout << (v + 1) << " ";
        }
        cout << endl;
        cout << "Tamanho do clique máximo encontrado: " << maxClique.size() << endl;
        cout << "Tempo de execução: " << (end - start) << " segundos" << endl;
    }

    MPI_Finalize();
    return 0;
}
