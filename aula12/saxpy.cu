#include <thrust/device_vector.h>   // Inclui a biblioteca Thrust para vetores na GPU
#include <thrust/host_vector.h>     // Inclui a biblioteca Thrust para vetores na CPU
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <thrust/sequence.h>   
using namespace std;

struct saxpy
{
    int a;    
    saxpy(int a_) : a(a_) {};  // Construtor que inicializa a constante 'a'

    __host__ __device__
    double operator()(const int& x, const int& y) const {
        return a * x + y;  // Operação que será aplicada a cada elemento de x e y
    }
};

int main() {

    thrust::device_vector<int> v(10);  // Cria um vetor na GPU com 10 elementos
    thrust::sequence(v.begin(), v.end(), 1);  // Preenche o vetor v com valores de 1 a 10

    thrust::device_vector<int> w(10);  // Cria um vetor na GPU com 10 elementos
    thrust::sequence(w.begin(), w.end(), 5);  // Preenche o vetor w com valores de 5 a 15

    thrust::device_vector<int> d_ret(10, 0);  // Cria um vetor na GPU com 10 elementos, todos inicializados com 0

    thrust::transform(v.begin(), v.end(), w.begin(), d_ret.begin(), saxpy(100));  // Executa a operação saxpy

    thrust::host_vector<int> h_ret(d_ret);  // Cria um vetor na CPU copiando os dados do vetor d_ret da GPU
    
    for (int i = 0; i < 10; i++) {
        cout << h_ret[i] << " ";  // Imprime o vetor h_ret
    }
    cout << endl;

    return 0;
}