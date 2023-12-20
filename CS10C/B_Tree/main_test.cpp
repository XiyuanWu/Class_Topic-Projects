#include "Tree.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;

bool isNumeric(const string& s);

void printOrders(Tree *tree) {
    cout << "Preorder = ";
    tree->preOrder();
    cout << endl;

    cout << "Inorder = ";
    tree->inOrder();
    cout << endl;

    cout << "Postorder = ";
    tree->postOrder();
    cout << endl;
}

int menu() {
    int choice = 0;
    cout << endl << "Enter menu choice: ";
    cout << endl;
    cout 
      << "1. Insert" << endl
      << "2. Remove" << endl
      << "3. Print" << endl
      << "4. Search" << endl
      << "5. Quit" << endl;
    //cin >> choice;
    
    // fix buffer just in case non-numeric choice entered
    // also gets rid of newline character
    cin.clear();
    cin.ignore(256, '\n');
    return choice;
}

int main(int argc, char* argv[]) {
    Tree tree;
    int choice = 0;
    string entry;
    
    for (int i = 1; i < argc; ++i) {
        if (isNumeric(argv[i])) {
            choice = atoi(argv[i]);

            // The next argument is expected to be a movie title if the choice is 1, 2, or 4.
            if ((choice == 1 || choice == 2 || choice == 4) && i + 1 < argc && !isNumeric(argv[i + 1])) {
                entry = argv[++i];
                replace(entry.begin(), entry.end(), '_', ' '); // Replace underscores with spaces.
            }
            
            // Process the choice.
            if (choice == 1) {
                cout << "Inserting movie title: " << entry << endl;
                tree.insert(entry); // Assume already defined.
            } else if (choice == 2) {
                cout << "Removing movie title: " << entry << endl;
                tree.remove(entry); // Assume already defined.
            } else if (choice == 3) {
                printOrders(&tree); // Assume already defined.
            } else if (choice == 4) {
                cout << "Searching for movie title: " << entry << endl;
                if (tree.search(entry)) { // Assume already defined.
                    cout << "Found" << endl;
                } else {
                    cout << "Not Found" << endl;
                }
            } else if (choice == 5) {
                cout << "Quitting program." << endl;
                break; // Exit the loop and end the program.
            } else {
                cerr << "Invalid choice: " << choice << endl;
                return 1; // Exit if the choice is not valid.
            }
        } else {
            cerr << "Expected numeric choice, but got: " << argv[i] << endl;
            return 1;
        }
    }

    return 0; // End of the program.
}

// A helper function to check if a string is numeric
bool isNumeric(const string& s) {
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

    char * p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
}

/*Test case*/

//g++ Node.cpp main_test.cpp Tree.cpp -Wall -Werror -Wuninitialized -o a.out
//g++ B_Tree/Node.cpp main_test.cpp Tree.cpp -Wall -Werror -Wuninitialized -o a.out


//Case 1
// ./a.out 1 Die_Hard 1 Terminator 1 Kill_Bill_Volume_1 3 5

//Case 2
// ./a.out 1 Die_Hard 1 Terminator 1 Kill_Bill_Volume_1 1 Pirates_of_the_Caribbean 1 The_Lord_of_the_Rings 1 Star_Wars 1 Gladiator 3 5



//Case 4
// ./a.out 1 Die_Hard 1 Terminator 1 Kill_Bill_Volume_1 3 2 Kill_Bill_Volume_1 3 5
//Case 6
// ./a.out 1 Die_Hard 1 Terminator 1 Kill_Bill_Volume_1 3 2 Die_Hard 3 5

/*

1 for insert, 3 for print tree(pre, in, post) and 5 for exit program
Pass input and run the code: 1 Die_Hard 1 Terminator 1 Kill_Bill_Volume_1 1 Pirates_of_the_Caribbean 1 The_Lord_of_the_Rings 1 Star_Wars 1 Gladiator 3 5
What's the tree look like?

EXAMPLE 2: given input:
Die_Hard, Terminator, Kill_Bill_Volume_1, Pirates_of_the_Caribbean, The_Lord_of_the_Rings, Star_Wars, Gladiator
specific logic is
Preorder = Kill Bill Volume 1, Die Hard, Gladiator, Terminator, Pirates of the Caribbean, Star Wars, The Lord of the Rings,
Inorder = Die Hard, Gladiator, Kill Bill Volume 1, Pirates of the Caribbean, Star Wars, Terminator, The Lord of the Rings,
Postorder = Die Hard, Gladiator, Pirates of the Caribbean, Star Wars, Kill Bill Volume 1, The Lord of the Rings, Terminator,







Zybook traverl tree algorthm
1. Pre order - small, left, large, middle, right
Visit smaller root keys first, and visit left child; Next visit larger keys in root node, and then visit
middle child, and then right child.
2. In order - left, small, middle, large, right
Visit left child first, and small key in root node, and visit middle child, larger keys in node, and right child.
3. Post order - left, middle, small, large, right
Visit left child first, and middle child, and smaller keys in root, and larger keys in node, and right child. 





*/