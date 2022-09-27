#include <iostream>
#include <queue>
#include "tree.h"

using namespace std;

Node *parent = nullptr;

Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

Node::Node() {
    this->isLeaf = false;
    this->pNextLeaf = nullptr;
}


Tree::Tree(int blockSize) {
    /*
     *  | |       | |       | |       | |       | |
     *  p - Each pointer is 8 bytes (64-bit address)
     *  k - Each key is 4 byte (unsigned int)
     *  n - Number of key-pointer pairs
     *  Total size of one block = p + n(p+k)
     *  n = (block_size - p) / (p+k)
     *
     *  This is just a simulation, in practice, the size of each Node is more complicated.
     */
    int n = (blockSize - 8) / (8 + 4);
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
        if (cursor->pointer.pNode[i] != nullptr)
            return firstLeftNode(cursor->pointer.pNode[i]);

    return nullptr;
}

Node **Tree::findParent(Node *cursor, Node *child) {
    /*
		Finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
		also ignores second last level because we will never find parent of a leaf node during insertion using this function
	*/

    if (cursor->isLeaf || cursor->pointer.pNode[0]->isLeaf)
        return nullptr;

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

int Tree::countNodes() {
    Node *cursor = root;
    int count = 0;

    if (cursor == nullptr)
        return count;

    queue<Node *> q;
    q.push(cursor);
    count++;

    while (!q.empty()) {
        auto sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node *u = q.front();
            q.pop();

            if (!u->isLeaf) {
                for (Node *v: u->pointer.pNode) {
                    q.push(v);
                    count++;
                }
            }
        }
    }
    return count;
}

int Tree::countDepth() {
    // todo: need to implement for experiment 2
    return 1;
}