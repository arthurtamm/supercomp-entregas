#include <thrust/device_vector.h>   // Inclui a biblioteca Thrust para vetores na GPU
#include <thrust/host_vector.h>     // Inclui a biblioteca Thrust para vetores na CPU
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <chrono>                   // Biblioteca para medir o tempo de execução do código
#include <thrust/extrema.h> 
#include <thrust/count.h>
#include <thrust/replace.h>
#include <thrust/functional.h> 
using namespace std;

struct isPositive
{
    typedef double argument_type;   // Necessário para o uso com thrust::not1
    __host__ __device__
    bool operator()(const double x) const
    {
        return x > 0.0;
    }
};

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
    thrust::device_vector<double> stocks(host);
    thrust::device_vector<double> d_ret(n, 0);
    thrust::transform(stocks.begin()+1, stocks.end(), stocks.begin(), d_ret.begin(), thrust::minus<double>());

    // int result = thrust::count_if(vec.begin(), vec.end(), is_odd());
    int n_positive_ret = thrust::count_if(d_ret.begin(), d_ret.end(), isPositive());

    thrust::replace_if(d_ret.begin(), d_ret.end(), thrust::not1(isPositive()), 0.0);

    double sum = thrust::reduce(d_ret.begin(), d_ret.end(), 0.0, thrust::plus<double>());
    double pos_avg = sum / n_positive_ret;

    std::cout << "Positive average return: " << pos_avg << std::endl;

    return 0;
}