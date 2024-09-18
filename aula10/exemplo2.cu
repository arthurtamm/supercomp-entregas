#include <thrust/device_vector.h>              // Inclui a biblioteca para vetores na GPU
#include <thrust/host_vector.h>                // Inclui a biblioteca para vetores na CPU
#include <thrust/sequence.h>                   // Inclui a função para preencher vetores com uma sequência de números
#include <thrust/functional.h>                 // Inclui operações matemáticas padrão, como soma e multiplicação
#include <thrust/transform.h>                  // Inclui a função para transformar vetores com operações ponto a ponto
#include <thrust/iterator/constant_iterator.h> // Inclui o iterador constante necessário
#include <iostream>   

int main() {
    // Cria um vetor na GPU (device_vector) com 10 elementos, todos inicializados com 0
    thrust::device_vector<double> V1(10, 0);
    // Preenche V1 com uma sequência de números: {0, 1, 2, ..., 9}
    thrust::sequence(V1.begin(), V1.end());

    // Cria um vetor na GPU com 5 elementos, todos inicializados com 0
    thrust::device_vector<double> V2(5, 0);
    // Preenche os dois primeiros elementos de V2 com 5.5: {5.5, 5.5, 0, 0, 0}
    thrust::fill(V2.begin(), V2.begin() + 2, 5.5);
    // Preenche os elementos restantes de V2 com 10: {5.5, 5.5, 10, 10, 10}
    thrust::fill(V2.begin() + 2, V2.end(), 10);

    // Cria dois vetores na GPU com 10 elementos, para armazenar resultados das operações
    thrust::device_vector<double> V3(10); // Vetor para armazenar o resultado da soma
    thrust::device_vector<double> V4(10); // Vetor para armazenar o resultado da multiplicação

    // Aplica a operação de soma elemento por elemento entre V1 e V2 e armazena o resultado em V3
    // Como V2 tem menos elementos, o restante de V1 é somado com zeros (elemento padrão).
    // Resultado: V3 = {0+5.5, 1+5.5, 2+10, 3+10, 4+10, 5+0, ..., 9+0}
    thrust::transform(V1.begin(), V1.end(), V2.begin(), V3.begin(), thrust::plus<double>());

    // Aplica multiplicação de V1 com o valor constante 0.5
    // e armazenaria o resultado em V4.
    thrust::transform(V1.begin(), V1.end(), thrust::constant_iterator<double>(0.5), V4.begin(), thrust::multiplies<double>());

    // Imprime os elementos de V1
    printf("V1: ");
    for (thrust::device_vector<double>::iterator i = V1.begin(); i != V1.end(); i++) {
        std::cout << *i << " "; // Acessa e imprime cada elemento do vetor V1
    }
    printf("\n");

    // Imprime os elementos de V2
    printf("V2: ");
    for (thrust::device_vector<double>::iterator i = V2.begin(); i != V2.end(); i++) {
        std::cout << *i << " "; // Acessa e imprime cada elemento do vetor V2
    }
    printf("\n");

    // Imprime os elementos de V3
    printf("V3: ");
    for (thrust::device_vector<double>::iterator i = V3.begin(); i != V3.end(); i++) {
        std::cout << *i << " "; // Acessa e imprime cada elemento do vetor V3
    }
    printf("\n");

    // Imprime o vetor V4, que foi criado mas não utilizado.
     printf("V4: ");
    for (thrust::device_vector<double>::iterator i = V4.begin(); i != V4.end(); i++) {
         std::cout << *i << " ";
    }
    printf("\n");

    return 0;
}