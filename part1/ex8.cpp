#include <iostream>
#include <vector>
#include <climits>
#include <sstream>

int main() {
    std::cout << "Insert a 3x3 matrix. Enter the elements row by row, separated by space: " << std::endl;
    std::vector<std::vector<int>> matrix(3, std::vector<int>(3));

    for (int i = 0; i < 3; i++) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        for (int j = 0; j < 3; j++) {
            iss >> matrix[i][j];
        }
    }

    std::cout << "Matrix: " << std::endl;
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Sum of the main diagonal: " << std::endl;
    int sum = 0;
    for (int i=0; i<3; i++){
        sum += matrix[i][i];
    }
    std::cout << sum << std::endl;
}
