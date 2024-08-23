#include <iostream>
#include <vector>

class Vector {
public:
    Vector(int tamanho);
    ~Vector();
    void inicializa(int valor);
    int get(int index) const;
    void set(int index, int valor);
    void inserir(int index, int valor);
    void remover(int index);
    void imprime() const;

private:
    int* dados;
    int tam;
    int capacidade;
    void redimensiona(int novaCapacidade);

};

// Construtor
Vector::Vector(int tamanho) : tam(tamanho), capacidade(tamanho) {
    dados = new int[tamanho]; // Aloca memória para o array de inteiros
}

// Destrutor
Vector::~Vector() {
    delete[] dados; // Libera a memória alocada
}

void Vector::inicializa(int valor) {
    for (int i = 0; i < tam; i++) {
        dados[i] = valor;
    }
}

void Vector::imprime() const {
    for (int i = 0; i < tam; i++) {
        std::cout << dados[i] << " ";
    }
    std::cout << std::endl;
}


int main() {
    Vector vec(5);
    vec.inicializa(0);
    vec.imprime();

    return 0;
}