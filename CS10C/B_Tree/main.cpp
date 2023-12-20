#include <iostream>
#include <string>

using namespace std;

class Node {
public:
    string small, large;
    Node *left, *middle, *right, *parent;

    Node(string key) : small(key), large(""), left(nullptr), middle(nullptr), right(nullptr), parent(nullptr) {}

    bool isTwoNode() const {
        return large.empty();
    }

    bool isThreeNode() const {
        return !large.empty();
    }
};

class Tree {
public:
    Node *root;

    Tree() : root(nullptr) {}

    void BTreeFuseRoot() {
        if (!root || !root->isTwoNode()) {
        // If the root is null or not a 2-node, there's no need to fuse
        return;
    }

    // Check if the root has children to fuse
    if (root->left && root->right) {
        // Create a new node that will become the new root
        Node *newRoot = new Node(root->left->small);

        // Adjusting keys and children for the new root
        newRoot->left = root->left->left;
        newRoot->middle = root->left->right;
        newRoot->right = root->right;
        newRoot->large = root->small;
        
        // Update parent pointers
        if (newRoot->left) newRoot->left->parent = newRoot;
        if (newRoot->middle) newRoot->middle->parent = newRoot;
        if (newRoot->right) newRoot->right->parent = newRoot;

        // Delete the old root and update the root pointer
        delete root;
        root = newRoot;
    }
    }

    void preOrderInternal(Node* node) const {

        if (!node) return;

        if (!node->isThreeNode()) {
            // Node has only one key
            cout << node->small;
            if (node->isThreeNode()) cout << ", " << node->large;
            cout << ", ";
            // Visit children
            preOrderInternal(node->left);
            preOrderInternal(node->middle);
            if (node->isThreeNode()) preOrderInternal(node->right);
        } else {
            // Node has two keys
            cout << node->small << ", ";  // Visit the smaller key
            preOrderInternal(node->left);  // Visit the left child
            cout << node->large << ", ";  // Visit the larger key
            preOrderInternal(node->middle); // Visit the middle child
            preOrderInternal(node->right); // Visit the right child
        }
    }

    void preOrder() const {
        preOrderInternal(root);
    }
};

int main() {
    // Create a tree and nodes
    Tree tree;
    tree.root = new Node("B");
    tree.root->left = new Node("A");
    tree.root->right = new Node("C");

    // Set parent pointers
    tree.root->left->parent = tree.root;
    tree.root->right->parent = tree.root;

    // Perform fusion
    tree.BTreeFuseRoot();

    // Print tree
    tree.preOrder();
    cout << endl;

    return 0;
}


/*

After insert, tree like this:

   B
  / \
 A   C

After fuse, tree like this:

  A, B, C

And output is:

A, B, C,

And we are able to start remove, not cover in this test main.cpp.


*/