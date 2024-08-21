#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

int main() {
    std::cout << "Welcome to the stock control program!" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "--add-item item_name" << std::endl;
    std::cout << "--remove-item item_name" << std::endl;
    std::cout << "--list-items" << std::endl;
    std::cout << "--exit" << std::endl;
    std::cout << "---------------------------------"<< std::endl;

    std::string line;
    std::getline(std::cin, line);
    std::vector<std::string> stock;
    // std::cout << "line: " << line << std::endl;

    int index;
    while (line != "--exit"){
        index = -1;
        auto it = std::find(line.begin(), line.end(), ' ');
        std::string command;
        std::string item;

        if (it != line.end()){
            index = std::distance(line.begin(), it);
            command = line.substr(0,index);
            item = line.substr(index+1);
        } else {
            command = line;
        }

        // std::cout << "Command: " << command << std::endl;

        if (command == "--add-item") {
            if (!item.empty()) {
                std::cout << "Item \"" << item << "\" added to stock." << std::endl;
                stock.push_back(item);
            } else {
                std::cout << "No item specified for --add-item command." << std::endl;
            }
        }
        else if (command == "--remove-item"){
            auto it = std::find(stock.begin(), stock.end(), item);
            if (it != stock.end()) {
                stock.erase(it);
                std::cout << "Item \"" << item << "\" removed from stock." << std::endl;
            } else {
                std::cout << "Item \"" << item << "\" not found in stock." << std::endl;
            }
        }
        else if (command == "--list-items"){
            if (stock.empty()) {
                std::cout << "Stock is empty." << std::endl;
            } else {
                int i = 0;
                for (const auto& item : stock) {
                    std::cout << i << "-" << item << std::endl;
                    i++;
                }
            }
        } else {
            std::cout << "Invalid command!" << std::endl;
        }
        std::cout << "---------------------------------" << std::endl;
        std::getline(std::cin, line);

    }
    std::cout << "End" << std::endl;

    return 0;
}
