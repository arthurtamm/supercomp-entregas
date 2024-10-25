#include <iostream>

int main(){

    int n;
    std::cout << "Enter a number: ";
    std::cin >> n;

    if (n % 2 == 0) {
        std::cout << "Even" << std::endl;
        return 0;
    } else {
        std::cout << "Odd" << std::endl;
        return 0;
    }

}