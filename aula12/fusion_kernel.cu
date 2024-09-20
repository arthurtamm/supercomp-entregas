#include <thrust/device_vector.h>   // Inclui a biblioteca Thrust para vetores na GPU
#include <thrust/host_vector.h>     // Inclui a biblioteca Thrust para vetores na CPU
#include <thrust/transform.h>       // Inclui a biblioteca Thrust para transformações
#include <thrust/random.h>          // Inclui a biblioteca Thrust para números aleatórios
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <chrono>  
#include <thrust/sequence.h>
#include <thrust/transform_reduce.h>
using namespace std;
using namespace std::chrono;

// Functor para gerar números aleatórios
struct random_number_generator
{
    unsigned int seed;

    random_number_generator(unsigned int seed_) : seed(seed_) {}

    __host__ __device__
    float operator()(const unsigned int n) const {
        thrust::default_random_engine rng(seed);
        thrust::uniform_real_distribution<float> dist(0.0f, 1.0f);
        rng.discard(n);
        return dist(rng);
    }
};

struct variance_op
{
    float mean;
    variance_op(float mean_) : mean(mean_) {}  // Construtor que armazena a média

    __host__ __device__
    float operator()(const float& x) const {
        float diff = x - mean;  // Diferença em relação à média
        return diff * diff;  // Elevar ao quadrado a diferença
    }
};

float calculate_variance(const thrust::device_vector<float>& d_vec, float mean) {
    return thrust::transform_reduce(d_vec.begin(), d_vec.end(), variance_op(mean), 0.0f, thrust::plus<float>()) / d_vec.size();
}

// Cálculo da variância em etapas separadas (transform seguido de reduce)
float calculate_variance_separate(const thrust::device_vector<float>& d_vec, float mean) {
    thrust::device_vector<float> diff_squared(d_vec.size());

    // Etapa 1: Aplicar transform para calcular a diferença ao quadrado de cada elemento
    thrust::transform(d_vec.begin(), d_vec.end(), diff_squared.begin(), variance_op(mean));

    // Etapa 2: Aplicar reduce para somar todas as diferenças ao quadrado
    float sum_diff_squared = thrust::reduce(diff_squared.begin(), diff_squared.end(), 0.0f, thrust::plus<float>());

    // Dividir pela quantidade de elementos para obter a variância
    return sum_diff_squared / d_vec.size();
}

int main() {

    thrust::host_vector<float> h_vec(1000*1000); // Vetor com 1 milhão de elementos

    // Gerar números aleatórios no vetor host
    thrust::transform(thrust::counting_iterator<unsigned int>(0),
                      thrust::counting_iterator<unsigned int>(h_vec.size()),
                      h_vec.begin(),
                      random_number_generator(1234));

    // Copiar o vetor host para o vetor device
    thrust::device_vector<float> d_vec = h_vec;

    // Calcular a média dos valores
    float mean = thrust::reduce(d_vec.begin(), d_vec.end(), 0.0f, thrust::plus<float>()) / d_vec.size();

    // Implementação em duas etapas separadas
    auto start = high_resolution_clock::now();
    
    // Calcular a variância em duas etapas
    float variance_separate = calculate_variance_separate(d_vec, mean);
    auto end = high_resolution_clock::now();
    duration<double> duration_separate = end - start;

    cout << "Two steps method" << endl;
    cout << "Variance: " << variance_separate << endl;
    cout << "Execution time: " << duration_separate.count() << " seconds" << endl;

    cout << "---------------------------------" << endl;
    
    // Implementação com fusion kernel (transform_reduce)
    auto start_2 = high_resolution_clock::now();
    // Calcular a média dos valores
    float mean_2 = thrust::reduce(d_vec.begin(), d_vec.end(), 0.0f, thrust::plus<float>()) / d_vec.size();

    // Calcular a variância
    float variance_2 = calculate_variance(d_vec, mean_2);
    auto end_2 = high_resolution_clock::now();
    duration<double> duration_2 = end_2 - start_2;

    cout << "Fusion kernel method" << endl;
    cout << "Variance: " << variance_2 << endl;
    cout << "Execution time: " << duration_2.count() << " seconds" << endl;


    return 0;
}