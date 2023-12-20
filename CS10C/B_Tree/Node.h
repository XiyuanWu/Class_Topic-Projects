#ifndef __NODE_H
#define __NODE_H

#include <string>

using namespace std;


class Node {
    friend class Tree;

  private:
    string small;
    string large;

    Node *left;
    Node *middle;
    Node *right;
    Node *parent;
    

  public:
    // Default constructor
    Node(): small(""), large(""), left(nullptr), middle(nullptr), right(nullptr), parent(nullptr) {}

    // Parameterized constructor for a node with one initial value
    explicit Node(const string &value) : small(value), large(""), left(nullptr), middle(nullptr), right(nullptr), parent(nullptr) {}

    // Check if the node is a leaf (no children)
    bool isLeaf() const {
        return left == nullptr && middle == nullptr && right == nullptr;
    }

    // Check if the node is a two-node (one value, two children)
    bool isTwoNode() const {
        return !small.empty() && large.empty();
    }

    // Check if the node is a three-node (two values, three children)
    bool isThreeNode() const {
        return !small.empty() && !large.empty();
    }

};

#endif



/*
The destructor doesn't need to do anything special because the Node class doesn't manage memory that isn't managed elsewhere. The tree should handle node deletions.
Implementing the Rule of Three can be complex for tree nodes because of the ownership relations between nodes
*/
