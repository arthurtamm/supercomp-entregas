// #include <mpi.h>
#include <omp.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cctype>

char transcription(char base) {
    base = std::toupper(base);
    switch(base) {
        case 'A': return 'U';
        case 'T': return 'A';
        case 'C': return 'G';
        case 'G': return 'C';
        default: return 'N';
    } 
}

std::string readFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string dna_sequence((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return dna_sequence;
}

void writeFile(const std::string &filename, const std::string &rna_sequence) {
    std::ofstream output_file(filename);
    if (!output_file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << filename << std::endl;
        return;
    }
    output_file << rna_sequence;
    output_file.close();
}

int main(int argc, char** argv) {
    // MPI_Init(&argc, &argv);
    
    // int world_size, world_rank;
    // MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_rank = 0;
    int world_size = 1;

    // Arquivos de entrada (modifique os caminhos se necessário)
    // int num_files = 22;
    int num_files = 1;
    
    for (int i = 1; i <= num_files; i++) {
        if ((i-1) % world_size == world_rank) {
            std::string input_filename = "../data/DNA/chr" + std::to_string(i) + ".subst.fa";
            // std::string input_filename = "../data/chr2.subst.fa";
            std::string dna_sequence = readFile(input_filename);

            std::string rna_sequence(dna_sequence.size(), ' ');
            bool is_first_line = true;

            // Manipula a primeira linha fora do loop paralelo
            size_t pos = 0;
            while (pos < dna_sequence.size() && dna_sequence[pos] != '\n') {
                rna_sequence[pos] = dna_sequence[pos];
                pos++;
            }

            #pragma omp parallel for
            for (size_t j = pos; j < dna_sequence.size(); j++) {
                char base = dna_sequence[j];
                if (is_first_line) {
                    if (base == '\n') {
                        is_first_line = false;
                    }
                    rna_sequence[j] = base;
                } else if (base != '\n') {
                    rna_sequence[j] = transcription(base);
                } else {
                    rna_sequence[j] = '\n';
                }
                
            }

            std::string output_filename = "../data/RNA/chr" + std::to_string(i) + ".rna.fa";
            writeFile(output_filename, rna_sequence);

        }
    }

    // MPI_Finalize();
    return 0;
}
