#include <iostream>
#include "tree.h"

using namespace std;

Node *parent = NULL;

Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

Node::Node() {
    this->isLeaf = false;
    this->pNextLeaf = NULL;
}


Tree::Tree(int blockSize) {
    /*
     *  | |       | |       | |       | |       | |
     *  p - Each pointer is 8 bytes (64-bit address)
     *  k - Each key is 1 byte (unsigned char)
     *  n - Number of key-pointer pairs
     *  Total size of one block = p + n(p+k)
     *  n = (block_size - p) / (p+k)
     */
    int n = (blockSize - 8) / (8 + 1);
    this->maxIntChildLimit = n + 1;
    this->maxLeafNodeLimit = n;
    this->root = nullptr;
    cout << "Instantiating B+ Tree" << endl;
    cout << " -> Nodes bounded by block size of = " << blockSize << endl;
    cout << " -> Maximum number of keys in a node: n = " << n << endl;
    cout << " -> Internal node max pointers to other nodes = " << this->maxIntChildLimit << endl;
    cout << " -> Leaf node max key-record pointers = " << this->maxLeafNodeLimit << endl;
    cout << "===========================================" << endl;
}

int Tree::getMaxIntChildLimit() {
    return maxIntChildLimit;
}

int Tree::getMaxLeafNodeLimit() {
    return maxLeafNodeLimit;
}

Node *Tree::getRoot() {
    return this->root;
}

void Tree::setRoot(Node *ptr) {
    this->root = ptr;
}

Node *Tree::firstLeftNode(Node *cursor) {
    if (cursor->isLeaf)
        return cursor;
    for (int i = 0; i < cursor->pointer.pNode.size(); i++)
        if (cursor->pointer.pNode[i] != NULL)
            return firstLeftNode(cursor->pointer.pNode[i]);

    return NULL;
}

Node **Tree::findParent(Node *cursor, Node *child) {
    /*
		Finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
		also ignores second last level because we will never find parent of a leaf node during insertion using this function
	*/

    if (cursor->isLeaf || cursor->pointer.pNode[0]->isLeaf)
        return NULL;

    for (int i = 0; i < cursor->pointer.pNode.size(); i++) {
        if (cursor->pointer.pNode[i] == child) {
            parent = cursor;
        } else {
            //Commenting To Remove vector out of bound Error:
            //new (&cursor->pointer.pNode) std::vector<Node*>;
            Node *tmpCursor = cursor->pointer.pNode[i];
            findParent(tmpCursor, child);
        }
    }

    return &parent;
}


