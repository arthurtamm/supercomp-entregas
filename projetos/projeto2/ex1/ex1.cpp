#include <mpi.h>
#include <omp.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cctype>

std::map<char, int> countBases(const std::string &dna_sequence) {
    std::map<char, int> base_counts = {{'A', 0}, {'C', 0}, {'G', 0}, {'T', 0}};
    
    #pragma omp parallel
    {
        std::map<char, int> local_counts = {{'A', 0}, {'C', 0}, {'G', 0}, {'T', 0}};
        #pragma omp for nowait
        for (size_t i = 0; i < dna_sequence.size(); i++) {
            char base = std::toupper(dna_sequence[i]);
            if (local_counts.find(base) != local_counts.end()) {
                local_counts[base]++;
            }
        }
        #pragma omp critical
        {
            for (const auto &pair : local_counts) {
                base_counts[pair.first] += pair.second;
            }
        }
    }
    return base_counts;
}

std::string readFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string dna_sequence((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return dna_sequence;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int num_files = 22;
    std::map<char, int> global_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};
    std::map<char, int> total_local_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};
    
    for (int i = 1; i <= num_files; i++) {
        if ((i-1) % world_size == world_rank) {
            std::string file = "../data/DNA/chr" + std::to_string(i) + ".subst.fa";
            std::string dna_sequence = readFile(file);
            std::map<char, int> local_counts = countBases(dna_sequence);

            // Redução dos resultados locais para o nó principal
            for (auto &pair : local_counts) {
                total_local_counts[pair.first] += pair.second;
            }
        }
    }

    for (const auto &pair : total_local_counts) {
        MPI_Reduce(&pair.second, &global_counts[pair.first], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    double end_time = MPI_Wtime();
    double total_time = end_time - start_time;

    if (world_rank == 0) {
        std::cout << "Frequência das bases nitrogenadas no DNA:" << std::endl;
        for (const auto &pair : global_counts) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Tempo total de execução: " << total_time << "s" << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
