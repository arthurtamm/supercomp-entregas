#include <iostream>
#include <vector>
#include <climits>

int main() {
    std::cout << "Enter a vector of numbers: ";

    
    std::string vec;
    std::cin >> vec;
    std::cout << "Initial order: ";
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    int ordered_vec[vec.size()];
    int aux;
    for (int i = vec.size()-1; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            if (vec[j] > vec[j+1]) {
                aux = vec[j];
                vec[j] = vec[j+1];
                vec[j+1] = aux;
            }
        }
    }

    std::cout << "Ordered vector: ";
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
