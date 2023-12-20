#ifndef __NODE_H__
#define __NODE_H__

#include <string>
using namespace std;

class Node {
    private:
        string data;
        int count;
        Node* left;
        Node* right;

    public:
        // Constructor
        Node(const string &data) : data(data), count(1), left(nullptr), right(nullptr) {}

        // Accessor 
        string getData() const { return data; }
        int getCount() const { return count; }
        Node* getLeft() const { return left; }
        Node* getRight() const { return right; }

        // Mutator 
        void setData(const string &data) { this->data = data; }
        void setCount(int count) { this->count = count; }
        void incrementCount() { count++; }
        void decrementCount() { if (count > 0) count--; }
        void setLeft(Node* left) { this->left = left; }
        void setRight(Node* right) { this->right = right; }
};

#endif // __NODE_H__
