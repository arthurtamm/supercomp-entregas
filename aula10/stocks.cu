#include <thrust/device_vector.h>   // Inclui a biblioteca Thrust para vetores na GPU
#include <thrust/host_vector.h>     // Inclui a biblioteca Thrust para vetores na CPU
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <chrono>                   // Biblioteca para medir o tempo de execução do código
using namespace std;

int main() {
    int n = 2518;                   // Define o tamanho do vetor (2518 elementos)
    double value = 0.0;             // Variável auxiliar para armazenar o valor lido da entrada
    std::chrono::duration<double> diff; // Variável para armazenar a diferença de tempo calculada

    // Marca o início da medição do tempo de leitura dos dados
    auto leitura_i = std::chrono::steady_clock::now();

    // Cria um vetor na CPU (host_vector) com n elementos, todos inicializados com 0
    thrust::host_vector<double> host(n, 0);

    // Loop para ler n valores da entrada padrão e armazená-los no vetor host
    for (int i = 0; i < n; i++) {
        cin >> value;              // Lê um valor da entrada padrão
        host[i] = value;           // Armazena o valor lido no vetor host
    }

    // Marca o fim da medição do tempo de leitura dos dados
    auto leitura_f = std::chrono::steady_clock::now();

    // Calcula o tempo gasto na leitura dos dados
    diff = leitura_f - leitura_i;

    // Exibe o tempo de leitura em segundos
    cout << "Tempo de LEITURA (em segundos)  " << diff.count() << endl;

    // Marca o início da medição do tempo de cópia dos dados da CPU para a GPU
    auto copia_i = std::chrono::steady_clock::now();

    // Cria um vetor na GPU (device_vector) copiando os dados do vetor host da CPU
    thrust::device_vector<double> dev(host);

    // Marca o fim da medição do tempo de cópia dos dados
    auto copia_f = std::chrono::steady_clock::now();

    // Calcula o tempo gasto na cópia dos dados para a GPU
    diff = copia_f - copia_i;

    // Exibe o tempo de cópia em segundos
    cout << "Tempo de CÓPIA (em segundos)  " << diff.count() << endl;

    return 0;
}