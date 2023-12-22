#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>

using namespace std;

class Node {

  public:

    string data;
    Node *left;
    Node *right;
    int height;

    Node(const string &data) : data(data), left(nullptr), right(nullptr), height(1) {}
};

#endif // NODE_H
