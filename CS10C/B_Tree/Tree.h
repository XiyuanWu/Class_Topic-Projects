#ifndef __TREE_H
#define __TREE_H

#include "Node.h"
#include <iostream>
#include <string>

class Tree {

  private:
    Node *root;

    // Helper functions for insertion
    Node* insertInternal(Node* node, const string &value, string &newChildEntry); 
    Node* splitNode(Node* nodeToSplit, const string &insertValue, string &newChildEntry); 

    /*Helper functions for deletion*/
    // Step 1: Rotation
    void BTreeRemoveKey(Node* node, int keyIndex);
    void BTreeAddKeyAndChild(Node* node, const string& key, Node* child);

    void BTreeRotateLeft(Node* node);
    string BTreeGetParentKeyLeftOfChild(Node* parent, Node* child);
    void BTreeSetParentKeyLeftOfChild(Node* parent, Node* child, const string& key);

    void BTreeRotateRight(Node* node);
    string BTreeGetParentKeyRightOfChild(Node* parent, Node* child);
    void BTreeSetParentKeyRightOfChild(Node* parent, Node* child, const string& key); 

    // Step 2: Fuse
    void BTreeFuseRoot(); //Root case
    Node* BTreeFuse(Node* leftNode, Node* rightNode); //Non-root case
    void BTreeSetChild(Node* parent, Node* child, int childIndex);

    // Step 3: Merge
    Node* BTreeMerge(Node* node);
    Node* BTreeGetLeftSibling(Node* node);
    Node* BTreeGetRightSibling(Node* node);

    // Step 4: Utility functions for removal
    string BTreeGetMinKey(Node* node);
    Node* BTreeGetChild(Node* node, int childIndex);
    Node* BTreeNextNode(Node* node, const string& key);
    bool BTreeKeySwap(Node* node, const string& existing, const string& replacement);
    int BTreeGetKeyIndex(Node* node, const string& key);
    Node* BTreeFindSuccessor(Node* node, const string& key);
    Node* BTreeFindPredecessor(Node* node, const string& key);

    // Step 5: Actual remove algorithm
    bool BTreeRemove(Node* cur, const string& key);


    // Helper functions for searching
    Node* searchInternal(Node* node, const string &value) const; 

    // Helper functions for traversal 
    void preOrderInternal(Node* node) const;
    void inOrderInternal(Node* node) const;
    void postOrderInternal(Node* node) const;

    // Private helper function to clear the tree
    void clear(Node *node) {
        if (node) {
            clear(node->left);
            clear(node->middle);
            clear(node->right);
            delete node;
        }
    }


  public:
    // Inline constructor
    Tree(): root(nullptr) {}

    // Inline destructor
    ~Tree() {
        clear(root);
    }

    void insert(const string &value) {

        if (!root) {
          // Tree is empty, create a new node and make it the root
          root = new Node(value);
        } else {
          string newChildEntry; // This will hold the new value that might get created if a split occurs
          Node *newChild = insertInternal(root, value, newChildEntry);

          if (newChild) {
              // If newChild is not null, it means the root was split and we have a new value and a new node
              Node *newRoot = new Node(newChildEntry);
              newRoot->left = root;
              newRoot->middle = newChild;
              root->parent = newRoot;
              newChild->parent = newRoot;
              root = newRoot;
          }
        }
    }


    void remove(const string& key) {
        // if (!root) {
        //     return; // Tree is empty
        // }

        // // Special case: root is a leaf with one key
        // if (root->isLeaf() && root->isTwoNode() && root->small == key) {
        //     delete root;
        //     root = nullptr;
        //     return;
        // }

        // BTreeRemove(root, key); // Start the removal process from the root
        BTreeRemove(root, key);
    }


    
    bool search(const string &value) const {
        return searchInternal(root, value) != nullptr;
    }


    void preOrder() const   { preOrderInternal(root); }
    void inOrder() const    { inOrderInternal(root); }
    void postOrder() const  { postOrderInternal(root); }

};

#endif
