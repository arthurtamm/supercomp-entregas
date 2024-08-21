#include <iostream>
#include <vector>
#include <climits>

int main() {
    std::cout << "Enter a vector of 10 numbers: ";
    // std::vector<int> vec(10);
    
    // for (int i = 0; i < 10; i++) {
    //     std::cin >> vec[i];
    //     // Verifica se a entrada foi válida
    //     if(std::cin.fail()) {
    //         std::cerr << "Invalid input. Please enter only numbers." << std::endl;
    //         return 1;
    //     }
    // }

    // int greatest = INT_MIN;
    // for (int i = 0; i < vec.size(); i++) {
    //     if (vec[i] > greatest) {
    //         greatest = vec[i];
    //     }
    // }
    
    std::string vec;
    std::cin >> vec;

    int greatest = INT_MIN;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] - '0' > greatest) {
            greatest = vec[i] - '0';
        }
    }
    std::cout << "The greatest number is: " << greatest << std::endl;

    return 0;
}
