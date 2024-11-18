#include <omp.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>


int codonToAminoAcid(const std::string &codon) {
    if (codon == "AUG") return 1; // Metionina (início)
    if (codon == "UGA" || codon == "UAA" || codon == "UAG") return -1; // STOP
    
    if (codon == "CCA" || codon == "CCG" || codon == "CCU" || codon == "CCC") return 2; // Prolina
    if (codon == "UCU" || codon == "UCA" || codon == "UCG" || codon == "UCC") return 3; // Serina
    if (codon == "CAG" || codon == "CAA") return 4; // Glutamina
    if (codon == "ACA" || codon == "ACC" || codon == "ACU" || codon == "ACG") return 5; // Treonina
    if (codon == "UGC" || codon == "UGU") return 6; // Cisteína
    if (codon == "GUG" || codon == "GUA" || codon == "GUC" || codon == "GUU") return 7; // Valina
    return 0;
}

std::vector<std::pair<size_t, std::vector<int>>> translateRNA(const std::string &rna_sequence) {
    std::vector<std::pair<size_t, std::vector<int>>> protein_sequences;

    #pragma omp parallel
    {
        std::vector<std::pair<size_t, std::vector<int>>> local_proteins;

        #pragma omp for
        for (size_t i = 0; i < rna_sequence.size() - 2; i++) {
            bool in_translation = false;
            std::vector<int> current_protein;
            size_t start_pos = 0;

            for (; i < rna_sequence.size() - 2; i++) {
                std::string codon = rna_sequence.substr(i, 3);

                if (!in_translation) {
                    if (codon == "AUG") {
                        in_translation = true;
                        start_pos = i;
                        current_protein.push_back(1);
                        i += 2;
                    }
                } else {
                    int amino_acid = codonToAminoAcid(codon);

                    if (amino_acid == -1) {
                        in_translation = false;
                        local_proteins.push_back({start_pos, current_protein});
                        current_protein.clear();
                    } else if (amino_acid != 0) {
                        current_protein.push_back(amino_acid);
                    }

                    i += 2;
                }
            }

            if (!current_protein.empty()) {
                local_proteins.push_back({start_pos, current_protein});
            }
        }

        #pragma omp critical
        {
            protein_sequences.insert(protein_sequences.end(), local_proteins.begin(), local_proteins.end());
        }
    }

    std::sort(protein_sequences.begin(), protein_sequences.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    return protein_sequences;
}

std::string readFile(const std::string &filename) {
    std::ifstream file(filename);
    std::string dna_sequence((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return dna_sequence;
}

void writeFile(const std::string &filename, const std::vector<std::vector<int>> &proteins) {
    std::ofstream output_file(filename);
    if (!output_file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << filename << std::endl;
        return;
    }

    for (const auto &protein : proteins) {
        for (int amino_acid : protein) {
            output_file << amino_acid << " ";
        }
        output_file << "\n";
    }

    output_file.close();
}

int main(int argc, char** argv) {
    double start_time = omp_get_wtime(); // Medir o tempo de execução com OpenMP

    // Substitua o caminho do arquivo por um arquivo válido para teste
    std::string input_filename = "../data/RNA/chr1.rna.fa";
    std::string output_filename = "../data/Proteins/chr1.protein.fa";

    // Lê o RNA de entrada
    std::string rna_sequence = readFile(input_filename);

    // Tradução do RNA em proteínas
    std::vector<std::pair<size_t, std::vector<int>>> protein_sequences = translateRNA(rna_sequence);

    // Extrai apenas as sequências de proteínas
    std::vector<std::vector<int>> proteins;
    for (const auto &protein_sequence : protein_sequences) {
        proteins.push_back(protein_sequence.second);
    }

    // Escreve as proteínas no arquivo de saída
    writeFile(output_filename, proteins);

    double end_time = omp_get_wtime();
    double total_time = end_time - start_time;

    std::cout << "Tempo total de execução: " << total_time << "s" << std::endl;

    return 0;
}
