#include "IntList.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    IntList myList;

    // Test: push_front
    myList.push_front(3);
    myList.push_front(2);
    myList.push_front(1);
    cout << "After push_front(3), push_front(2), push_front(1): " << myList << endl; // Should print: 1 2 3

    // Test: push_back
    myList.push_back(4);
    myList.push_back(5);
    cout << "After push_back(4), push_back(5): " << myList << endl; // Should print: 1 2 3 4 5

    // Test: pop_front
    myList.pop_front();
    cout << "After pop_front(): " << myList << endl; // Should print: 2 3 4 5

    // Test: pop_back
    myList.pop_back();
    cout << "After pop_back(): " << myList << endl; // Should print: 2 3 4

    // Test: printReverse
    cout << "Print in reverse: ";
    myList.printReverse(); // Should print: 4 3 2
    cout << endl;

    // Test: empty
    cout << "Is the list empty? " << (myList.empty() ? "Yes" : "No") << endl; // Should print: No

    return 0;
}