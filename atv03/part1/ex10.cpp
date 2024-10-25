#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

int main() {
    cout << "Welcome to the stock control program!" << endl;
    cout << "Commands:" << endl;
    cout << "--add-item item_name" << endl;
    cout << "--remove-item item_name" << endl;
    cout << "--list-items" << endl;
    cout << "--exit" << endl;
    cout << "---------------------------------"<< endl;

    string line;
    getline(cin, line);
    vector<string> stock;

    int index;
    while (line != "--exit"){
        index = -1;
        auto it = find(line.begin(), line.end(), ' ');
        string command;
        string item;

        if (it != line.end()){
            index = distance(line.begin(), it);
            command = line.substr(0,index);
            item = line.substr(index+1);
        } else {
            command = line;
        }

        if (command == "--add-item") {
            if (!item.empty()) {
                cout << "Item \"" << item << "\" added to stock." << endl;
                stock.push_back(item);
            } else {
                cout << "No item specified for --add-item command." << endl;
            }
        }
        else if (command == "--remove-item"){
            auto it = find(stock.begin(), stock.end(), item);
            if (it != stock.end()) {
                stock.erase(it);
                cout << "Item \"" << item << "\" removed from stock." << endl;
            } else {
                cout << "Item \"" << item << "\" not found in stock." << endl;
            }
        }
        else if (command == "--list-items"){
            if (stock.empty()) {
                cout << "Stock is empty." << endl;
            } else {
                int i = 0;
                for (const auto& item : stock) {
                    cout << i << "-" << item << endl;
                    i++;
                }
            }
        } else {
            cout << "Invalid command!" << endl;
        }
        cout << "---------------------------------" << endl;
        getline(cin, line);

    }
    cout << "End" << endl;

    return 0;
}
