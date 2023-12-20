#include <iostream>
#include "Node.h"
#include "BSTree.h"

using namespace std;



// Node* BSTree::insertHelper(Node* current, const string &key) {
    
//     if (!root) {
//         root = new Node(key);
//         return root;
//     }
//     // Base case: If current node is null, a new node is created and returned
//     if (!current) return new Node(key);

//     // If the key is the same as the current node's data, increment the count
//     if (key == current->getData()) {
//         current->incrementCount();
//     }
//     // If the key is smaller than the current node's data, insert in the left subtree
//     else if (key < current->getData()) {
//         current->setLeft(insertHelper(current->getLeft(), key));
//     } 
//     // If the key is larger than the current node's data, insert in the right subtree
//     else {
//         current->setRight(insertHelper(current->getRight(), key));
//     }
//     return current;
// }

void BSTree::insert(const string &newString) {
    // If the tree is empty, simply create a new root
    if (!root) {
        root = new Node(newString);
        return;
    }
    
    Node* current = root;
    
    while (current) {
        // If the key is the same as the current node's data, increment the count and return
        if (newString == current->getData()) {
            current->incrementCount();
            return;
        }
        // If the key is smaller than the current node's data, move to the left child or insert there
        else if (newString < current->getData()) {
            if (!current->getLeft()) {
                current->setLeft(new Node(newString));
                return;
            } else {
                current = current->getLeft();
            }
        }
        // If the key is larger than the current node's data, move to the right child or insert there
        else {
            if (!current->getRight()) {
                current->setRight(new Node(newString));
                return;
            } else {
                current = current->getRight();
            }
        }
    }
}



Node* BSTree::removeHelper(Node* node, const string &key) {
    if (!node) return nullptr;  // Node not found

    // If the key is the current node's data
    if (node->getData() == key) {
        // If the node's count is greater than 1, decrement the count
        if (node->getCount() > 1) {
            node->decrementCount();
            return node;
        }

        // Node with only one child or no child
        if (!node->getLeft() && !node->getRight()) {
            delete node;
            return nullptr;
        }

        // Node has left children, get the largest node in the left subtree
        if (node->getLeft()) {
            Node* temp = node->getLeft();
            while (temp->getRight()) {
                temp = temp->getRight();
            }
            node->setData(temp->getData());
            node->setCount(temp->getCount());
            temp->setCount(1);  // Reset count for the node to be deleted recursively next
            node->setLeft(removeHelper(node->getLeft(), temp->getData()));
        }

        // Node doesn't have left children, get the smallest node in the right subtree
        else {
            Node* temp = node->getRight();
            while (temp->getLeft()) {
                temp = temp->getLeft();
            }
            node->setData(temp->getData());
            node->setCount(temp->getCount());
            temp->setCount(1);  // Reset count for the node to be deleted recursively next
            node->setRight(removeHelper(node->getRight(), temp->getData()));
        }
    } 
    else if (node->getData() > key) {  // If the key to be deleted is smaller than the node's key
        node->setLeft(removeHelper(node->getLeft(), key));
    } 
    else {  // If the key to be deleted is larger than the node's key
        node->setRight(removeHelper(node->getRight(), key));
    }
    return node;
}



/*-----------------------------------------------------------------------------*/


bool BSTree::searchHelper(Node* node, const string &key) const {
    
    if (!node) return false;  // Base case: If node is null
    if (node->getData() == key) return true;
    if (key < node->getData()) return searchHelper(node->getLeft(), key);
    return searchHelper(node->getRight(), key);
}


int BSTree::heightHelper(Node* node, const string &key) const {
    
    if (!node) {
        return -1;  // The string does not exist in the tree.
    }
    if (key == node->getData()) {
        // Found the node with the string, now compute its height.
        return findHeight(node);  // Subtracting 1 as the height of a leaf node is 0.
    } else if (key < node->getData()) {
        return heightHelper(node->getLeft(), key);
    } else {
        return heightHelper(node->getRight(), key);
    }
}

int BSTree::findHeight(Node* node) const {
    
    if (!node) {
        return -1;
    }
    return 1 + max(findHeight(node->getLeft()), findHeight(node->getRight()));
}


/*------------------------------------------------------------------------------*/


string BSTree::findLargestFromNode(Node* node) const {
    
    if (!node) return "";  // If the subtree is empty, return an empty string

    Node* current = node;
    while (current->getRight()) {
        current = current->getRight();  // Traverse to the rightmost node
    }
    return current->getData();
}

string BSTree::findSmallestFromNode(Node* node) const {
    
    if (!node) return "";  // If the subtree is empty, return an empty string

    Node* current = node;
    while (current->getLeft()) {
        current = current->getLeft();  // Traverse to the leftmost node
    }
    return current->getData();
}



/*------------------------------------------------------------------------------*/


/*Print tree*/

void BSTree::preOrderHelper(Node* node) const {
    
    if (node) {
        cout << node->getData() << "(" << node->getCount() << "), ";
        preOrderHelper(node->getLeft());
        preOrderHelper(node->getRight());
    }
}

void BSTree::inOrderHelper(Node* node) const {
    
    if (node) {
        inOrderHelper(node->getLeft());
        cout << node->getData() << "(" << node->getCount() << "), ";
        inOrderHelper(node->getRight());
    }
}

void BSTree::postOrderHelper(Node* node) const {
    
    if (node) {
        postOrderHelper(node->getLeft());
        postOrderHelper(node->getRight());
        cout << node->getData() << "(" << node->getCount() << "), ";
    }
}




