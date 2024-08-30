#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

int main() {
    std::string inputFileName;
    std::cout << "Enter the name of the file to be analyzed: ";
    std::cin >> inputFileName;

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Could not open the file " << inputFileName << std::endl;
        return 1;
    }

    int totalLines = 0;
    int totalWords = 0;
    std::map<std::string, int> wordFrequency;

    std::string line;
    while (std::getline(inputFile, line)) {
        totalLines++;
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            totalWords++;
            wordFrequency[word]++;
        }
    }
    inputFile.close();

    std::string mostFrequentWord;
    int maxFrequency = 0;
    for (const auto& pair : wordFrequency) {
        if (pair.second > maxFrequency) {
            mostFrequentWord = pair.first;
            maxFrequency = pair.second;
        }
    }

    double averageWordsPerLine = (totalLines > 0) ? static_cast<double>(totalWords) / totalLines : 0.0;

    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Could not open the output file." << std::endl;
        return 1;
    }

    outputFile << "Total number of lines: " << totalLines << std::endl;
    outputFile << "Total number of words: " << totalWords << std::endl;
    outputFile << "Average number of words per line: " << averageWordsPerLine << std::endl;
    outputFile << "Most frequent word: " << mostFrequentWord << " (appeared " << maxFrequency << " times)" << std::endl;

    outputFile.close();

    std::cout << "Statistics have been written to output.txt" << std::endl;

    return 0;
}
