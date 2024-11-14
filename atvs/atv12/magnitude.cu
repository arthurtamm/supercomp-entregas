#include <thrust/device_vector.h>   // Inclui a biblioteca Thrust para vetores na GPU
#include <thrust/host_vector.h>     // Inclui a biblioteca Thrust para vetores na CPU
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <chrono>  
#include <thrust/sequence.h>
#include <thrust/transform_reduce.h>
using namespace std;
using namespace std::chrono;

struct square
{
    __host__ __device__
    float operator()(const float& x) const {
        return x * x;  // Função de elevação ao quadrado
    }
};

float magnitude(thrust::device_vector<float>& v) {
    float sum_of_squares = thrust::transform_reduce(v.begin(), v.end(), square(), 0.0f, thrust::plus<float>());
    return std::sqrt(sum_of_squares);
}

float magnitude_host(thrust::host_vector<float>& v) {
    float sum_of_squares = thrust::transform_reduce(v.begin(), v.end(), square(), 0.0f, thrust::plus<float>());
    return std::sqrt(sum_of_squares);
}

int main() {

    thrust::host_vector<float> v(20000);

    thrust::fill(v.begin(), v.end(), 2);

    auto start_cpu = high_resolution_clock::now();
    float mag_v = magnitude_host(v);
    auto end_cpu = high_resolution_clock::now();
    duration<double> duration_cpu = end_cpu - start_cpu;

    cout << "Thrust CPU" << endl;
    cout << "Magnitude: " << mag_v << endl;
    cout << "Execution time: " << duration_cpu.count() << "seconds" << endl;

    cout << "---------------------------------" << endl;
    
    thrust::device_vector<float> d_v(v);
    auto start_gpu = high_resolution_clock::now();
    float mag_d_v = magnitude(d_v);
    auto end_gpu = high_resolution_clock::now();
    duration<double> duration_gpu = end_gpu - start_gpu;

    cout << "Thrust GPU" << endl;
    cout << "Magnitude: " << mag_d_v << endl;
    cout << "Execution time: " << duration_gpu.count() << "seconds" << endl;
    


    return 0;
}