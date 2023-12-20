#include <iostream>
#include "Tree.h"

using namespace std;


/*---------------Insert---------------*/

Node* Tree::splitNode(Node* nodeToSplit, const string &insertValue, string &newChildEntry) {

    // First, determine the median value and the values to keep in the nodeToSplit and the new node
    string lowValue, highValue, medianValue;
    if (insertValue < nodeToSplit->small) {
        lowValue = insertValue;
        medianValue = nodeToSplit->small;
        highValue = nodeToSplit->large;
    } else if (insertValue < nodeToSplit->large) {
        lowValue = nodeToSplit->small;
        medianValue = insertValue;
        highValue = nodeToSplit->large;
    } else {
        lowValue = nodeToSplit->small;
        medianValue = nodeToSplit->large;
        highValue = insertValue;
    } 


    // Create a new node to hold the highValue
    Node *newNode = new Node(highValue);

    // If the node being split is not a leaf, we need to redistribute its children
    if (!nodeToSplit->isLeaf()) {
        if (insertValue < nodeToSplit->small) {
            newNode->middle = nodeToSplit->right;
            newNode->left = nodeToSplit->middle;
            nodeToSplit->middle = nullptr; // This child is moving to the new node
        } else if (insertValue < nodeToSplit->large) {
            newNode->middle = nodeToSplit->right;
            newNode->left = nullptr; // The new value is for the new node
        } else {
            newNode->left = nodeToSplit->middle;
            newNode->middle = nodeToSplit->right;
            nodeToSplit->middle = nullptr; // This child is moving to the new node
        }
        nodeToSplit->right = nullptr; // This child is moving to the new node
    }

    // Update the values of the nodeToSplit to be the lowValue
    nodeToSplit->small = lowValue;
    nodeToSplit->large = "";

    // The medianValue gets pushed up to the parent
    newChildEntry = medianValue;

    return newNode;
}


Node* Tree::insertInternal(Node* node, const string &value, string &newChildEntry) {

    if (node->isLeaf()) {
        // If the node is a leaf, insert the value here
        if (node->isTwoNode()) {
            // If the node has only one value, insert the new value in the correct order
            if (value < node->small) {
                node->large = node->small;
                node->small = value;
            } else {
                node->large = value;
            }
            return nullptr; // No new child was created, return null
        } else {
            // The node is a three-node and needs to be split
            Node *newNode = splitNode(node, value, newChildEntry);
            return newNode; // Return the new child node created by the split
        }
    } else {
        // The node is not a leaf, so we need to find the correct child to insert into
        Node *childToInsert = nullptr;
        if (node->isTwoNode()) {
            if (value < node->small) {
                childToInsert = node->left;
            } else {
                childToInsert = node->middle;
            }
        } else {
            if (value < node->small) {
                childToInsert = node->left;
            } else if (value < node->large) {
                childToInsert = node->middle;
            } else {
                childToInsert = node->right;
            }
        }

        // Now insert into the correct child and handle any split that occurs
        Node *newChild = insertInternal(childToInsert, value, newChildEntry);
        
        if (newChild) {
            // If a new child is returned, it means the child was split and we need to handle it
            if (node->isTwoNode()) {
                // If the node has only one value, we can add the new value and child without splitting
                if (newChildEntry < node->small) {
                    node->large = node->small;
                    node->small = newChildEntry;
                    node->right = node->middle;
                    node->middle = newChild;
                } else {
                    node->large = newChildEntry;
                    node->right = newChild;
                }
                return nullptr; // The node did not need to be split
            } else {
                // The node is a three-node and needs to be split to add the new child
                Node* newNode = splitNode(node, newChildEntry, newChildEntry);
                return newNode;
            }
        }

        return nullptr; // No split occurred, return null
    }
}






















/*---------------Remove---------------*/


// Step 1: Rotation

void Tree::BTreeRemoveKey(Node* node, int keyIndex) {
    if (node == nullptr) {
        return; // Safety check
    }

    if (keyIndex == 0) {
        // Remove the first key (small key)
        node->small = node->large;
        node->large.clear();

        if (!node->isLeaf()) {
            node->left = node->middle;
            node->middle = node->right;
            node->right = nullptr;
        }
    } else if (keyIndex == 1) {
        // Remove the second key (large key)
        node->large.clear();

        if (!node->isLeaf()) {
            node->middle = node->right;
            node->right = nullptr;
        }
    }
    // No case for keyIndex == 2 as it's not applicable in a 2-3 tree
}


void Tree::BTreeAddKeyAndChild(Node* node, const string& key, Node* child) {
    if (node == nullptr) {
        return; // Safety check
    }

    // Adding key and child to a 2-node (which becomes a 3-node)
    if (node->isTwoNode()) {
        // As the new key is greater than all keys in the node, it becomes the large key.
        node->large = key;

        // The new child becomes the right child.
        node->right = child;
    }

    // Update parent pointer of the child
    if (child != nullptr) {
        child->parent = node;
    }
}



//Rotate Left
void Tree::BTreeRotateLeft(Node* node) {
    if (node == nullptr || node->parent == nullptr) {
        return; // Safety check
    }

    Node* leftSibling = BTreeGetLeftSibling(node);
    if (leftSibling == nullptr) {
        return; // No left sibling to rotate with
    }

    // Get the key from the parent that is left of the current node
    string keyForLeftSibling = BTreeGetParentKeyLeftOfChild(node->parent, node);

    // Add this key and the leftmost child of 'node' to the left sibling
    BTreeAddKeyAndChild(leftSibling, keyForLeftSibling, node->left);

    // Replace the parent's key with the leftmost key of 'node'
    BTreeSetParentKeyLeftOfChild(node->parent, node, node->small);

    // Remove the leftmost key from 'node'
    BTreeRemoveKey(node, 0);
}

string Tree::BTreeGetParentKeyLeftOfChild(Node* parent, Node* child) {
    if (parent == nullptr || child == nullptr) {
        return ""; // Safety check
    }

    // If child is the right child, return the large key of the parent
    // If child is the middle child, return the small key of the parent
    if (parent->right == child) {
        return parent->large;
    } else if (parent->middle == child) {
        return parent->small;
    }

    return ""; // If child is the left child or an invalid child, return empty string
}


void Tree::BTreeSetParentKeyLeftOfChild(Node* parent, Node* child, const string& key) {
    if (parent == nullptr || child == nullptr) {
        return; // Safety check
    }

    // If child is the right child, set the large key of the parent
    // If child is the middle child, set the small key of the parent
    if (parent->right == child) {
        parent->large = key;
    } else if (parent->middle == child) {
        parent->small = key;
    }
    // If child is the left child or an invalid child, no action is needed
}


//Rotate Right
void Tree::BTreeRotateRight(Node* node) {
    if (node == nullptr || node->parent == nullptr) {
        return; // Safety check
    }

    Node* rightSibling = BTreeGetRightSibling(node);
    if (rightSibling == nullptr) {
        return; // No right sibling to rotate with
    }

    // Get the key from the parent that is right of the current node
    string keyForRightSibling = BTreeGetParentKeyRightOfChild(node->parent, node);

    // Add this key and the rightmost child of 'node' to the right sibling
    BTreeAddKeyAndChild(rightSibling, keyForRightSibling, node->right);

    // Replace the parent's key with the rightmost key of 'node'
    BTreeSetParentKeyRightOfChild(node->parent, node, node->large);

    // Remove the rightmost key from 'node'
    BTreeRemoveKey(node, 1); // Assuming keyIndex 1 corresponds to the 'large' key
}

string Tree::BTreeGetParentKeyRightOfChild(Node* parent, Node* child) {
    if (parent == nullptr || child == nullptr) {
        return ""; // Safety check
    }

    // If child is the left child, return the small key of the parent
    // If child is the middle child, return the large key of the parent
    if (parent->left == child) {
        return parent->small;
    } else if (parent->middle == child) {
        return parent->large;
    }

    return ""; // If child is the right child or an invalid child, return empty string
}

void Tree::BTreeSetParentKeyRightOfChild(Node* parent, Node* child, const string& key) {
    if (parent == nullptr || child == nullptr) {
        return; // Safety check
    }

    // If child is the left child, set the small key of the parent
    // If child is the middle child, set the large key of the parent
    if (parent->left == child) {
        parent->small = key;
    } else if (parent->middle == child) {
        parent->large = key;
    }
    // If child is the right child or an invalid child, no action is needed
}



// Step 2: Fuse

//This function don't work expected
//Fusion of the root node is a special case that happens only when the root and the root's 2 children each have 1 key. 
//In this case, the 3 keys from the 3 nodes are combined into a single node that becomes the new root node.
void Tree::BTreeFuseRoot() {
    std::cout << "new fuseroot" << endl;
    if (!root || !root->isTwoNode()) {
        // If the root is null or not a 2-node, there's no need to fuse
        return;
    }
    cout << "new fuseroot 2" << endl;
    cout << root->left->small << ", ";// << root->right->small << endl;

    // Check if the root has children to fuse
    if (root->left && (root->middle || root->right)) {
        // Create a new node that will become the new root
        // Node *newRoot = new Node(root->left->small);

        // // Adjusting keys and children for the new root
        // // newRoot->small = root->small;  // Transfer the key from the old root
        // newRoot->left = root->left->left;
        // newRoot->middle = root->left->right;
        // newRoot->right = root->right;
        // newRoot->large = root->small;

        // // Update parent pointers
        // if (newRoot->left) newRoot->left->parent = newRoot;
        // if (newRoot->middle) newRoot->middle->parent = newRoot;
        // if (newRoot->right) newRoot->right->parent = newRoot;

        // // Delete the old root and update the root pointer
        // delete root;
        // root = newRoot;
        std::cout<< "new fuseroot" << endl;
        root->large = root->small;
        root->small = root->left->small;
        root->left = nullptr;
    }
}







// Node* Tree::BTreeFuse(Node* leftNode, Node* rightNode) {
//     // Check if the parent of the leftNode is the root and has only one key
//     Node* parent = leftNode->parent;
//     if (parent == root && root->isTwoNode()) {
//         BTreeFuseRoot();
//     }

//     // Since we are dealing with a 2-3 tree, we only have two keys max
//     // Retrieve the middle key from the parent for fusion
//     string middleKey = BTreeGetParentKeyLeftOfChild(parent, rightNode);

//     // Create the new fused node by combining keys and children
//     Node* fusedNode = new Node();
//     fusedNode->small = leftNode->small; // The left node's key
//     fusedNode->large = rightNode->small; // The right node's key

//     // Assign children from leftNode and rightNode to the new fused node
//     fusedNode->left = leftNode->left;
//     fusedNode->middle = leftNode->middle; // This is the right child in a 2-3 tree
//     fusedNode->right = rightNode->middle; // This is the right child of the rightNode

//     // Remove the parent's key that was between leftNode and rightNode and update the parent's child reference
//     int keyIndex = BTreeGetKeyIndex(parent, middleKey);
//     BTreeRemoveKey(parent, keyIndex);
//     BTreeSetChild(parent, fusedNode, keyIndex); // This function needs to handle updating the correct child reference in the parent

//     // Clean up the nodes that have been fused
//     delete leftNode;
//     delete rightNode;

//     // Set the parent pointers of the fusedNode's children
//     if (fusedNode->left) {
//         fusedNode->left->parent = fusedNode;
//     }
//     if (fusedNode->middle) {
//         fusedNode->middle->parent = fusedNode;
//     }
//     if (fusedNode->right) {
//         fusedNode->right->parent = fusedNode;
//     }

//     // Update the parent's child pointer to point to the new fused node
//     if (keyIndex == 0) {
//         parent->left = fusedNode;
//     } else if (keyIndex == 1) {
//         parent->middle = fusedNode; // In a 2-3 tree, there is only left and middle
//     }
//     fusedNode->parent = parent;

//     return fusedNode;
// }




void Tree::BTreeSetChild(Node* parent, Node* child, int childIndex) {
    if (parent == nullptr) {
        return; // Safety check for the parent
    }

    // Setting the child based on the childIndex
    switch (childIndex) {
        case 0:
            parent->left = child;
            break;
        case 1:
            parent->middle = child;
            break;
        case 2:
            parent->right = child;
            break;
        default:
            // Invalid child index
            return;
    }

    // Update the parent pointer of the child, if the child is not null
    if (child != nullptr) {
        child->parent = parent;
    }
}




// Step 3: Merge

Node* Tree::BTreeMerge(Node* node) {
    if (node == nullptr) {
        // Node is null, cannot be merged
        return nullptr;
    }
    
    // No merging operation needed if node is the root, as this will be handled in BTreeFuse
    if (node->parent == nullptr) {
        return node; // This return is only to exit the function, the root fusion logic will be in BTreeFuse
    }

    Node* leftSib = BTreeGetLeftSibling(node);
    Node* rightSib = BTreeGetRightSibling(node);

    // Perform a right rotation if the left sibling has two keys (is a three-node in a 2-3 tree)
    if (leftSib != nullptr && leftSib->isThreeNode()) {
        BTreeRotateRight(leftSib);
    } 
    // Perform a left rotation if the right sibling has two keys (is a three-node in a 2-3 tree)
    else if (rightSib != nullptr && rightSib->isThreeNode()) {
        BTreeRotateLeft(rightSib);
    } 
    // If neither sibling can spare a key, perform a fusion
    else {
        // If there's no left sibling, fuse with the right sibling
        if (leftSib == nullptr) {
            node = BTreeFuse(node, rightSib);
        }
        // Otherwise, fuse with the left sibling
        else {
            node = BTreeFuse(leftSib, node);
        }
    }
    return node;
}





Node* Tree::BTreeGetLeftSibling(Node* node) {
    if (node == nullptr || node->parent == nullptr) {
        // No left sibling if the node is null or the root
        return nullptr;
    }

    Node* parent = node->parent;

    // Check if the node is the right child of its parent
    if (parent->right == node) {
        return parent->middle; // Middle child is the left sibling
    }

    // Check if the node is the middle child of its parent
    if (parent->middle == node) {
        return parent->left; // Left child is the left sibling
    }

    // The node is the left child or an only child, so no left sibling
    return nullptr;
}


Node* Tree::BTreeGetRightSibling(Node* node) {
    if (node == nullptr || node->parent == nullptr) {
        // No right sibling if the node is null or the root
        return nullptr;
    }

    Node* parent = node->parent;

    // Check if the node is the left child of its parent
    if (parent->left == node) {
        return parent->middle; // Middle child is the right sibling
    }

    // Check if the node is the middle child of its parent
    if (parent->middle == node) {
        return parent->right; // Right child is the right sibling
    }

    // The node is the right child or an only child, so no right sibling
    return nullptr;
}




// Step 4: Utility functions for removal

string Tree::BTreeGetMinKey(Node* node) {
    if (node == nullptr) {
        // Handle case where the node is null
        return "";
    }

    Node* cur = node;
    while (cur->left != nullptr) {
        cur = cur->left;
    }
    return cur->small; // The 'small' field holds the key
}

Node* Tree::BTreeGetChild(Node* node, int childIndex) {
    if (node == nullptr) {
        // If the node is null, return null
        return nullptr;
    }

    switch (childIndex) {
        case 0:
            return node->left;
        case 1:
            return node->middle; // For a 2-3 tree, it's just 'middle', not 'middle1'
        case 2:
            return node->right;
        default:
            return nullptr; // Invalid child index
    }
}

Node* Tree::BTreeNextNode(Node* node, const string& key) {
    if (node == nullptr) {
        // If the node is null, return null
        return nullptr;
    }

    if (key < node->small) {
        return node->left;
    } else if (node->large.empty() || key < node->large) {
        return node->middle; // For a 2-3 tree, there is only one middle child
    } else {
        return node->right;
    }
}

bool Tree::BTreeKeySwap(Node* node, const string& existing, const string& replacement) {
    if (node == nullptr) {
        return false;
    }

    int keyIndex = BTreeGetKeyIndex(node, existing);
    if (keyIndex == -1) {
        Node* next = BTreeNextNode(node, existing);
        return BTreeKeySwap(next, existing, replacement);
    }

    if (keyIndex == 0) {
        node->small = replacement;
    } else if (keyIndex == 1) {
        node->large = replacement;
    }

    return true;
}

// Implementation of BTreeGetKeyIndex (assumed to be a function that finds the index of a key in a node)
int Tree::BTreeGetKeyIndex(Node* node, const string& key) {
    if (node == nullptr) {
        return -1;
    }
    if (node->small == key) {
        return 0;
    } else if (node->large == key) {
        return 1;
    }
    return -1;
}

Node* Tree::BTreeFindSuccessor(Node* node, const string& key) {
    if (node == nullptr) {
        return nullptr;
    }

    // Start with the right child if the key matches the large key of the node
    if (!node->large.empty() && key == node->large) {
        Node* cur = node->right;
        while (cur->left != nullptr) {
            cur = cur->left;
        }
        return cur;
    }

    // Otherwise, start with the middle child
    Node* cur = node->middle;
    while (cur->left != nullptr) {
        cur = cur->left;
    }
    return cur;
}

Node* Tree::BTreeFindPredecessor(Node* node, const string& key) {
    if (node == nullptr) {
        return nullptr;
    }

    // Start with the left child
    Node* cur = node->left;
    while (cur->right != nullptr) {
        cur = cur->right;
    }
    return cur;
}



// Step 5: Actual remove algorithm


bool Tree::BTreeRemove(Node* cur, const string& key) {
    // Special case: root is a leaf with one key
    if (root->isLeaf() && root->isTwoNode() && root->small == key) {
        delete root;
        root = nullptr;
        return true;
    }

    cur = root; // Start from the root
    while (cur != nullptr) {
        if (cur->isTwoNode() && cur != root) {
            cur = BTreeMerge(cur); // Regular merge for two-node non-root nodes
        }

        int keyIndex = BTreeGetKeyIndex(cur, key);
        if (keyIndex != -1) {
            if (cur->isLeaf()) {
                BTreeRemoveKey(cur, keyIndex);
                return true;
            } else {
                Node* tmpChild = BTreeGetChild(cur, keyIndex + 1);
                string tmpKey = BTreeGetMinKey(tmpChild);
                BTreeRemove(root, tmpKey); // Recursive call with the tree's root
                BTreeKeySwap(root, key, tmpKey); // Key swap referring to the root of the tree
                return true;
            }
        }

        cur = BTreeNextNode(cur, key); // Move to the next node in the search path
    }

    return false; // Key not found
}


//Addition test function


// bool Tree::BTreeRemove(Node* cur, const string& key) {

//     cout << "BTreeRemove called with key: " << key << endl;

//     // Special case: root is a leaf with one key
//     if (root->isLeaf() && root->isTwoNode() && root->small == key) {
//         cout << "Removing key from root (leaf with one key)" << endl;
//         delete root;
//         root = nullptr;
//         return true;
//     }

//     cur = root; // Start from the root
//     while (cur != nullptr) {
//         cout << "Current node key: " << (cur->isTwoNode() ? cur->small : cur->small + ", " + cur->large) << endl;

//         if (cur->isTwoNode() && cur != root) {
//             cout << "Performing merge on a two-node non-root node" << endl;
//             cur = BTreeMerge(cur); // Regular merge for two-node non-root nodes
//         }

//         int keyIndex = BTreeGetKeyIndex(cur, key);
//         if (keyIndex != -1) {
//             cout << "Key found at index: " << keyIndex << endl;
//             if (cur->isLeaf()) {
//                 cout << "Removing key from leaf node" << endl;
//                 BTreeRemoveKey(cur, keyIndex);
//                 return true;
//             } else {
//                 cout << "Removing key from internal node" << endl;
//                 Node* tmpChild = BTreeGetChild(cur, keyIndex + 1);
//                 string tmpKey = BTreeGetMinKey(tmpChild);
//                 BTreeRemove(root, tmpKey); // Recursive call with the tree's root
//                 BTreeKeySwap(root, key, tmpKey); // Key swap referring to the root of the tree
//                 return true;
//             }
//         }

//         cur = BTreeNextNode(cur, key); // Move to the next node in the search path
//         cout << "Moving to the next node in search path" << endl;
//     }

//     cout << "Key not found in the tree" << endl;
//     return false; // Key not found
// }


// Node* Tree::BTreeMerge(Node* node) {
//     cout << "BTreeMerge called on node with key(s): ";
//     if (node) {
//         cout << node->small;
//         if (!node->large.empty()) {
//             cout << ", " << node->large;
//         }
//     } else {
//         cout << "null";
//     }
//     cout << endl;

//     if (node == nullptr) {
//         cout << "Node is null, cannot be merged" << endl;
//         return nullptr;
//     }
    
//     if (node->parent == nullptr) {
//         cout << "Node is the root, no merging operation needed" << endl;
//         return node;
//     }

//     Node* leftSib = BTreeGetLeftSibling(node);
//     Node* rightSib = BTreeGetRightSibling(node);

//     if (leftSib != nullptr && leftSib->isThreeNode()) {
//         cout << "Performing a right rotation on left sibling with key(s): " << leftSib->small;
//         if (!leftSib->large.empty()) {
//             cout << ", " << leftSib->large;
//         }
//         cout << endl;
//         BTreeRotateRight(leftSib);
//     } else if (rightSib != nullptr && rightSib->isThreeNode()) {
//         cout << "Performing a left rotation on right sibling with key(s): " << rightSib->small;
//         if (!rightSib->large.empty()) {
//             cout << ", " << rightSib->large;
//         }
//         cout << endl;
//         BTreeRotateLeft(rightSib);
//     } else {
//         cout << "Performing a fusion" << endl;
//         if (leftSib == nullptr) {
//             cout << "Fusing with right sibling" << endl;
//             node = BTreeFuse(node, rightSib);
//         } else {
//             cout << "Fusing with left sibling" << endl;
//             node = BTreeFuse(leftSib, node);
//         }
//     }

//     cout << "BTreeMerge completed. Returning node with key(s): ";
//     if (node) {
//         cout << node->small;
//         if (!node->large.empty()) {
//             cout << ", " << node->large;
//         }
//     } else {
//         cout << "null";
//     }
//     cout << endl;

//     return node;
// }




Node* Tree::BTreeFuse(Node* leftNode, Node* rightNode) {
    cout << "BTreeFuse called with left node key: " << leftNode->small << " and right node key: " << rightNode->small << endl;

    Node* parent = leftNode->parent;
    cout << parent->small << "," << parent->large << "," << parent->left->small << parent->middle->small << endl;

    if (parent == root && root->isTwoNode()) {
        cout << "Root fusion needed. Calling BTreeFuseRoot." << endl;
        BTreeFuseRoot();
        return root->left;
    }
    //follow zybook logic but still proceed code below
    string middleKey = BTreeGetParentKeyLeftOfChild(parent, rightNode);
    cout << "Middle key for fusion: " << middleKey << endl;

    Node* fusedNode = new Node();
    fusedNode->small = leftNode->small; // The left node's key
    fusedNode->large = rightNode->small; // The right node's key

    fusedNode->left = leftNode->left;
    fusedNode->middle = leftNode->middle;
    fusedNode->right = rightNode->middle;

    int keyIndex = BTreeGetKeyIndex(parent, middleKey);
    BTreeRemoveKey(parent, keyIndex);
    BTreeSetChild(parent, fusedNode, keyIndex);

    delete leftNode;
    delete rightNode;

    if (fusedNode->left) {
        fusedNode->left->parent = fusedNode;
    }
    if (fusedNode->middle) {
        fusedNode->middle->parent = fusedNode;
    }
    if (fusedNode->right) {
        fusedNode->right->parent = fusedNode;
    }

    if (keyIndex == 0) {
        parent->left = fusedNode;
    } else if (keyIndex == 1) {
        parent->middle = fusedNode;
    }
    fusedNode->parent = parent;
    

    cout << "Fusion completed. New fused node keys: " << fusedNode->small;
    if (!fusedNode->large.empty()) {
        cout << ", " << fusedNode->large;
    }
    cout << endl;

    return fusedNode;
    
}







































/*---------------Search---------------*/


Node* Tree::searchInternal(Node* node, const string &value) const {

    if (!node) return nullptr; // Base case: value not found

    // Check if the current node contains the value
    if (node->small == value || (node->isThreeNode() && node->large == value)) {
        return node; // Value found
    } else if (value < node->small) {
        return searchInternal(node->left, value); // Search left subtree
    } else if (node->isTwoNode() || value < node->large) {
        return searchInternal(node->middle, value); // Search middle subtree
    } else {
        return searchInternal(node->right, value); // Search right subtree
    }
}


/*---------------Traversal---------------*/



// Pre order - small, left, large, middle, right
void Tree::preOrderInternal(Node* node) const {

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



// In order - left, small, middle, large, right
void Tree::inOrderInternal(Node* node) const {
    if (!node) return;

    if (!node->isThreeNode()) {
        // Visit left child
        inOrderInternal(node->left);
        // Visit node
        cout << node->small << ", ";
        // Visit middle child
        inOrderInternal(node->middle);

        if (node->isThreeNode()) {
            // Visit node again for the large value
            cout << node->large << ", ";
            // Visit right child
            inOrderInternal(node->right);
        }
    } else {
        // Node has two keys
        inOrderInternal(node->left);   // Visit the left child
        cout << node->small << ", ";  // Visit the smaller key
        inOrderInternal(node->middle); // Visit the middle child
        cout << node->large << ", ";  // Visit the larger key
        inOrderInternal(node->right);  // Visit the right child
    }
}

// Post order - left, middle, small, right, large
void Tree::postOrderInternal(Node* node) const {
    if (!node) return;

    if (!node->isThreeNode()) {
        // Node has only one key
        // Visit children
        postOrderInternal(node->left);
        postOrderInternal(node->middle);
        if (node->isThreeNode()) postOrderInternal(node->right);

        // Visit node
        cout << node->small;
        if (node->isThreeNode()) cout << ", " << node->large;
        cout << ", ";
    } else {
        // Node has two keys
        postOrderInternal(node->left);  // Visit the left child
        if (node->isThreeNode()) postOrderInternal(node->middle);  // Visit the middle child if it's a three-node
        cout << node->small << ", ";  // Visit the smaller key in the node
        postOrderInternal(node->right);  // Visit the right child, then the larger key if it's a three-node
        if (node->isThreeNode()) cout << node->large << ", ";  // Visit the larger key if it's a three-node

    }
}

