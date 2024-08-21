#include <iostream>

int main(){

    int n1, n2;
    char op;
    std::cout << "Enter two numbers and an operator: ";
    std::cin >> n1 >> n2 >> op;

    if (op == '+') {
        std::cout << n1 + n2 << std::endl;
        return 0;
    } else if (op == '-') {
        std::cout << n1 - n2 << std::endl;
        return 0;
    } else if (op == '*') {
        std::cout << n1 * n2 << std::endl;
        return 0;
    } else if (op == '/') {
        std::cout << n1 / n2 << std::endl;
        return 0;
    } else {
        std::cout << "Invalid operator" << std::endl;
        return 1;
    }

}