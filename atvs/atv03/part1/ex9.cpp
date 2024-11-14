#include <iostream>
#include <vector>
#include <sstream>

int main() {
    std::cout << "Palindrome validator. Insert a string: " << std::endl;
    std::string str;
    std::getline(std::cin, str);

    bool is_palindrome = true;
    for (size_t i = 0; i < str.size() / 2 ; i++) {
        if (str[i] != str[str.size()-1-i]) {
            std::cout << "Not a palindrome!" << std::endl;
            is_palindrome = false;
            break;
        }
    }

    if(is_palindrome){
        std::cout << "Palindrome!" << std::endl;
    }

    return 0;
}
