#include <iostream>   // Inclui a biblioteca padrão de entrada e saída, usada para operações como std::cout.
#include <chrono>     // Inclui a biblioteca para medição de tempo, usada para medir a performance do código.
#include <vector>     // Inclui a biblioteca de vetores, usada para criar vetores de tamanho dinâmico.
#include <chrono>     // Inclui a biblioteca para medição de tempo, usada para medir a performance do código.

using namespace std;

int main() {
    auto start = std::chrono::high_resolution_clock::now();  // Marca o tempo de início da execução.
    int N = 1000;  // Define o tamanho da matriz como N x N, onde N é 1000.

    // Alocação dinâmica de memória para uma matriz N x N.
    // Primeiro, aloca um array de ponteiros, onde cada ponteiro irá apontar para uma linha da matriz.
    vector <vector<int>> matriz(N, vector<int>(N));

    // Aqui você pode inicializar e somar os elementos da matriz.
    // Por exemplo, você pode preencher a matriz com valores e calcular a soma total.
    int soma = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = 1;  // Preenche a matriz com a soma dos índices i e j.
            soma += matriz[i][j];  // Soma os elementos da matriz.
        }
    }

    // Imprime a soma total dos elementos da matriz.
    std::cout << "Soma total dos elementos da matriz: " << soma << std::endl;

    auto end = std::chrono::high_resolution_clock::now();  // Marca o tempo de término da execução.
    std::chrono::duration<double> elapsed_seconds = end - start;  // Calcula o tempo total de execução.
    std::cout << "Tempo total de execução: " << elapsed_seconds.count() << "s" << std::endl;  // Imprime o tempo total de execução.
    return 0;  // Retorna 0, indicando que o programa terminou com sucesso.
}