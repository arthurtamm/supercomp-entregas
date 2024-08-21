#include <iostream>

int main(){

    std::cout << "Enter a vector of 5 numbers: ";
    std::string vec;
    std::cin >> vec;

    int sum = 0;
    for (int i=0; i<5; i++){
        sum += vec[i] - '0';
    }
    std::cout << sum << std::endl;

}