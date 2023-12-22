#ifndef AVLTREE_H
#define AVLTREE_H

#include "Node.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

class AVLTree {

  public:
    // Constructor
    AVLTree(): root(nullptr) {}

    // Destructor
    ~AVLTree() { clear(root); }

    // Required public member functions
    void insert(const string &data) { root = insertInternal(root, data); }
    int balanceFactor(Node*);
    void printBalanceFactors() { printBalanceFactors(root); cout << endl;}
    void visualizeTree(const string &);

  private:
    Node *root;

    // Helper functions
    Node* insertInternal(Node* node, const string &data);
    Node* rotateLeft(Node*);
    Node* rotateRight(Node*);
    Node* leftRightRotate(Node*);
    Node* rightLeftRotate(Node*);

    int getHeight(Node*);
    void printBalanceFactors(Node*);
    void visualizeTree(ofstream&, Node*);
    
    
    void clear(Node* node) { // helper for destructor
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

};

#endif // AVLTREE_H
