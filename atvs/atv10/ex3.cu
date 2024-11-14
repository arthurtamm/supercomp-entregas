#include <thrust/device_vector.h>   // Inclui a biblioteca Thrust para vetores na GPU
#include <thrust/host_vector.h>     // Inclui a biblioteca Thrust para vetores na CPU
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <sstream>                  // Biblioteca para manipulação de strings
using namespace std;

int main() {
    int n = 3520;  // Define o tamanho do vetor (3520 elementos, representando dias de negociação)
    double valueAAPL = 0.0, valueMSFT = 0.0;  // Variáveis auxiliares para armazenar os valores lidos

    // Cria vetores na CPU (host_vector) para as ações da Apple e Microsoft
    thrust::host_vector<double> AAPL(n, 0);
    thrust::host_vector<double> MSFT(n, 0);

    string line;
    int i = 0;

    // Loop para ler n linhas da entrada padrão (arquivo redirecionado com '<') e armazenar nos vetores AAPL e MSFT
    while (getline(cin, line) && i < n) {
        stringstream ss(line);   // Cria um stringstream a partir da linha lida
        string value;

        // Lê o valor da ação da Apple (antes da vírgula)
        getline(ss, value, ',');
        valueAAPL = stod(value);  // Converte a string para double
        AAPL[i] = valueAAPL;      // Armazena no vetor AAPL

        // Lê o valor da ação da Microsoft (depois da vírgula)
        getline(ss, value, ',');
        valueMSFT = stod(value);  // Converte a string para double
        MSFT[i] = valueMSFT;      // Armazena no vetor MSFT

        i++;
    }

    thrust::device_vector<double> devAAPL(AAPL);  // Cria um vetor na GPU para a ação da Apple
    thrust::device_vector<double> devMSFT(MSFT);  // Cria um vetor na GPU para a ação da Microsoft
    thrust::device_vector<double> diff(n, 0);  // Cria um vetor na GPU para armazenar o resultado

    thrust::transform(devAAPL.begin(), devAAPL.end(), devMSFT.begin(), diff.begin(), thrust::minus<double>());  // Calcula a diferença entre os vetores AAPL e MSFT

    double sum = thrust::reduce(diff.begin(), diff.end(), 0, thrust::plus<double>());  // Calcula a soma dos valores absolutos da diferença
    
    double avg = sum / n;  // Calcula a média dos valores absolutos da diferença

    cout << "Average difference: " << avg << endl;  // Exibe a média dos valores absolutos da diferença
    if (avg < 0) {
        cout << "MSFT stock is worth more than AAPL stock." << endl;  // Exibe a mensagem se a média for negativa
    }
    else {
        cout << "AAPL stock is worth more than MSFT stock." << endl;  // Exibe a mensagem se a média for positiva
    }

    return 0;
}
