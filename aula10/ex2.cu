#include <thrust/device_vector.h>   // Inclui a biblioteca Thrust para vetores na GPU
#include <thrust/host_vector.h>     // Inclui a biblioteca Thrust para vetores na CPU
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <chrono>                   // Biblioteca para medir o tempo de execução do código
#include <thrust/extrema.h> 
using namespace std;

int main() {
    int n = 3112;                   // Define o tamanho do vetor (2518 elementos)
    double value = 0.0;             // Variável auxiliar para armazenar o valor lido da entrada

    // Cria um vetor na CPU (host_vector) com n elementos, todos inicializados com 0
    thrust::host_vector<double> host(n, 0);

    // Loop para ler n valores da entrada padrão e armazená-los no vetor host
    for (int i = 0; i < n; i++) {
        cin >> value;              // Lê um valor da entrada padrão
        host[i] = value;           // Armazena o valor lido no vetor host
    }

    // Cria um vetor na GPU (device_vector) copiando os dados do vetor host da CPU
    thrust::device_vector<double> dev(host);

    double sum10 = thrust::reduce(dev.begin(), dev.end(), 0, thrust::plus<double>());
    double average10 = sum10 / n;
    std::cout << "Average last 10 years: " << average10 << std::endl;

    double sum1 = thrust::reduce(dev.end()-365, dev.end(), 0, thrust::plus<double>());
    double average1 = sum1 / 365;
    std::cout << "Average last 1 year: " << average1 << std::endl;

    double min = *thrust::min_element(dev.begin(), dev.end());
    std::cout << "Minimum value 10 years: " << min << std::endl;

    double max = *thrust::max_element(dev.begin(), dev.end());
    std::cout << "Maximum value 10 years: " << max << std::endl;

    double min1 = *thrust::min_element(dev.end()-365, dev.end());
    std::cout << "Minimum value last year: " << min1 << std::endl;

    double max1 = *thrust::max_element(dev.end()-365, dev.end());
    std::cout << "Maximum value last year: " << max1 << std::endl;


    return 0;
}