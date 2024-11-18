#include <mpi.h>
#include <omp.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cctype>

int countProteins(const std::string &rna_sequence) {
    int prot_counts = 0;
    
    #pragma omp parallel
    {
        int local_counts = 0;
        #pragma omp for nowait
        for (size_t i = 0; i < rna_sequence.size(); i++) {
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
    int global_counts = 0;
    int total_local_counts = 0;
    
    for (int i = 1; i <= num_files; i++) {
        if ((i-1) % world_size == world_rank) {
            std::string file = "../data/RNA/chr" + std::to_string(i) + ".rna.fa";
            std::string rna_sequence = readFile(file);
            total_local_counts += countProteins(rna_sequence);
            std::cout << "Processo " << std::to_string(i) << " encontrou " << std::to_string(total_local_counts) << " proteínas inizializadas (AUG)" << std::endl;
        }
    }


    MPI_Reduce(&total_local_counts, &global_counts, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double total_time = end_time - start_time;

    if (world_rank == 0) {
        std::cout << std::endl;
        std::cout << "Contagem de proteínas inicializadas:" << global_counts << std::endl;
        std::cout << "Tempo total de execução: " << total_time << "s" << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
