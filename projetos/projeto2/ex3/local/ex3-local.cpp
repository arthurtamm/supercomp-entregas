#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>

// Função para contar as proteínas inicializadas (códons "AUG")
int countProteins(const std::string &rna_sequence) {
    int prot_counts = 0;
    
    #pragma omp parallel
    {
        int local_counts = 0;

        // Itera sobre a sequência RNA em blocos de 3 nucleotídeos
        #pragma omp for nowait
        for (size_t i = 0; i <= rna_sequence.size() - 3; i++) {
            if (rna_sequence[i] == 'A' && rna_sequence[i + 1] == 'U' && rna_sequence[i + 2] == 'G') {
                local_counts++;
            }
        }

        #pragma omp critical
        {
            prot_counts += local_counts;
        }
    }
    return prot_counts;
}

// Função para ler um arquivo e retornar o conteúdo como uma string
std::string readFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string rna_sequence((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return rna_sequence;
}

int main() {
    // Testar com um único arquivo ou múltiplos arquivos sequencialmente
    int num_files = 1; // Altere para o número de arquivos desejado
    int total_counts = 0;

    for (int i = 1; i <= num_files; i++) {
        std::string file = "../data/RNA/chr" + std::to_string(i) + ".rna.fa";
        std::string rna_sequence = readFile(file);

        // Contagem de proteínas para o arquivo atual
        // int local_counts = countProteins(rna_sequence);
        // total_counts += local_counts;
        total_counts += countProteins(rna_sequence);

        // std::cout << "Arquivo: " << file << " - Proteínas inicializadas (AUG): " << total_counts << std::endl;
    }

    // Exibe a contagem total
    std::cout << "Contagem total de proteínas inicializadas: " << total_counts << std::endl;

    return 0;
}
