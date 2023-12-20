#include "IntList.h"
#include <iostream>

using namespace std;


IntList::IntList() {

    dummyHead = new IntNode(-1);
    dummyTail = new IntNode(-1);

    // Linking dummy head and tail nodes
    dummyHead -> next = dummyTail;
    dummyTail -> prev = dummyHead;
}

IntList::~IntList() {

    IntNode* current = dummyHead;
    IntNode* next = nullptr;

    while (current != nullptr) {
        next = current -> next;
        delete current;
        current = next;
    }
}

void IntList::push_front(int value) {

    IntNode* newNode = new IntNode(value);

    //Link new node
    newNode -> next = dummyHead -> next;
    newNode -> prev = dummyHead;

    //Update folowing node and dummy head
    dummyHead -> next -> prev = newNode;
    dummyHead -> next = newNode;

}

void IntList::pop_front() {

    if (empty()) {
        return;
    }

    IntNode* removeNode = dummyHead -> next;

    //Update the pointer
    dummyHead -> next = removeNode -> next;
    removeNode -> next -> prev = dummyHead;

    //delete node
    delete removeNode;
}

void IntList::push_back(int value) {

    IntNode* newNode = new IntNode(value);

    //Update new node
    newNode -> next = dummyTail;
    newNode -> prev = dummyTail -> prev;

    //Update folowing node and dummy head
    dummyTail -> prev -> next = newNode;
    dummyTail -> prev = newNode;
}

void IntList::pop_back() {

    if (empty()) {
        return;
    }
    
    IntNode* removeNode = dummyTail -> prev;

    //Update pointer
    dummyTail -> prev = removeNode -> prev;
    removeNode -> prev -> next = dummyTail;

    //delete removeNode
    delete removeNode;
}


bool IntList::empty() const {

    return dummyHead -> next == dummyTail;
}

ostream& operator<<(ostream&out, const IntList&rhs) {

    IntNode* current = rhs.dummyHead -> next;

    while (current != rhs.dummyTail) {
        out << current -> data;
        current = current -> next;

        if (current != rhs.dummyTail) {
            out << " ";
        }
    }
    return out;
}

void IntList::printReverse() const {

    IntNode* current = dummyTail -> prev;

    //Traverse through the list until dummyHead is reached
    while (current != dummyHead) {
        cout << current -> data;
        current = current -> prev;

        //Make sure it does NOT output a newline or space at the end
        if (current != dummyHead) {
            cout << " ";
        }
    }
}

