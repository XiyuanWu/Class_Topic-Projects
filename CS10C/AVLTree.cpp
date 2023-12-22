#include "AVLTree.h"
#include <string>

using namespace std;

/*---------------Insert---------------*/

// Helper function for insert
Node* AVLTree::insertInternal(Node* node, const string &data) {

    // If tree are empty
    if (node == nullptr) {
        return new Node(data);
    }

    if (data < node->data) {
        node->left = insertInternal(node->left, data);
    } else if (data > node->data) {
        node->right = insertInternal(node->right, data);
    } else {
        // duplicate data is not inserted in the tree
        return node;
    }

    // Update the height of this ancestor node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Get the balance factor to check whether this node became unbalanced
    int balance = balanceFactor(node);

    // If the node becomes unbalanced, there are 4 cases

    // Left Left Case
    if (balance > 1 && data < node->left->data)
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && data > node->right->data)
        return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && data > node->left->data) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && data < node->right->data) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // return the (unchanged) node pointer
    return node;
}



/*---------------Rotate---------------*/

Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    // Return new root
    return y;
}

Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    // Return new root
    return x;
}

Node* AVLTree::leftRightRotate(Node* z) {
    z->left = rotateLeft(z->left);
    return rotateRight(z);
}

Node* AVLTree::rightLeftRotate(Node* z) {
    z->right = rotateRight(z->right);
    return rotateLeft(z);
}





/*---------------BalanceFactor---------------*/

// Function to return the balance factor of a node
int AVLTree::balanceFactor(Node* node) {

    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

// Function to get the height of a node
int AVLTree::getHeight(Node* node) {

    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// Helper function to print balance factors in in-order traversal
void AVLTree::printBalanceFactors(Node *node) {
    if (node != nullptr) {
        printBalanceFactors(node->left); // Visit left subtree

        // Print the current node's data and balance factor
        cout << node->data << "(" << balanceFactor(node) << "), ";

        printBalanceFactors(node->right); // Visit right subtree
    }
}


/*---------------VisualizeTree---------------*/

void AVLTree::visualizeTree(const string &outputFilename) {

    ofstream outFS(outputFilename.c_str());
    if(!outFS.is_open()){
        cout<<"Error"<<endl;
        return;
    }
    outFS<<"digraph G {"<<endl;
    visualizeTree(outFS,root);
    outFS<<"}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0,outputFilename.size()-4)+".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}

void AVLTree::visualizeTree(ofstream & outFS, Node *n) {

    if(n){
        if(n->left){
            visualizeTree(outFS,n->left);
            outFS<<n->data <<" -> " <<n->left->data<<";"<<endl;    
        }

        if(n->right){
            visualizeTree(outFS,n->right);
            outFS<<n->data <<" -> " <<n->right->data<<";"<<endl;    
        }
    }
}