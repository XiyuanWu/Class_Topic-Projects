#ifndef __BSTREE_H__
#define __BSTREE_H__
#include "Node.h"


class BSTree {

  private:
   Node *root;
      
  public:
   /* Constructors */
   /* Default constructor */
   BSTree(): root(nullptr) {}
   ~BSTree() {
      if (root) {
         if (root->getLeft()) delete root->getLeft();
         if (root->getRight()) delete root->getRight();
         delete root;
      }
   }

   /* Mutators */
   /* Insert an item into the binary search tree. 
      Be sure to keep BST properties. 
      When an item is first inserted into the tree the count should be set to 1. 
      When adding a duplicate string (case sensitive), rather than adding another node, 
      the count variable should be incremented 
   */
   void insert(const string &newString);// {
   //    root = insertHelper(root, newString);
   // }



   /* Remove a specified string from the tree. 
      Be sure to maintain all bianry search tree properties. 
      If removing a node with a count greater than 1, just decrement the count, otherwise, 
      if the count is simply 1, remove the node. 
      You MUST follow the remove algorithm shown in the slides and discussed in class or else 
      your program will not pass the test functions. 
      When removing, 
            if removing a leaf node, simply remove the leaf. Otherwise, 
            if the node to remove has a left child, replace the node to remove with the largest 
            string value that is smaller than the current string to remove 
            (i.e. find the largest value in the left subtree of the node to remove). 
            If the node has no left child, replace the node to remove with the smallest value 
            larger than the current string to remove 
            (i.e. find the smallest value in the right subtree of the node to remove. 
   */
   void remove(const string &key) {
      root = removeHelper(root, key);
   }



   /* Accessors */
   /* Search for a string in the binary search tree. 
      It should return true if the string is in the tree, and false otherwise. 
   */
   bool search(const string &key) const {
      return searchHelper(root, key);
   }

   /* Find and return the largest value in the tree. Return an empty string if the tree is empty */
   string largest() const {
      return findLargestFromNode(root);
   }

   /* Find and return the smallest value in the tree. Return an emtpy string if the tree is empty */
   string smallest() const {
      return findSmallestFromNode(root);
   }

   /* Compute and return the height of a particular string in the tree. 
      The height of a leaf node is 0 (count the number of edges on the longest path). 
      Return -1 if the string does not exist. 
   */
   int height(const string& key) const {
      return heightHelper(root, key);
   }



   /* Printing */
   /* For all printing orders, each node should be displayed as follows:
      <string> (<count>)
      e.g. goodbye(1), Hello World(3)
      */
   void preOrder() const {
      preOrderHelper(root);
   }
   
   void postOrder() const {
      postOrderHelper(root);
   }

   void inOrder() const {
      inOrderHelper(root);
   }

  private:
   //Helper Functions

   bool searchHelper(Node* node, const string &key) const;

   Node* insertHelper(Node* current, const string &key);
   Node* removeHelper(Node* node, const string &key);

   int heightHelper(Node* node, const string &key) const;
   int findHeight(Node* node) const;

   string findLargestFromNode(Node* node) const;
   string findSmallestFromNode(Node* node) const;

   void preOrderHelper(Node* node) const;
   void inOrderHelper(Node* node) const;
   void postOrderHelper(Node* node) const;

};

#endif // __BSTREE_H__
