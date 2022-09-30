#include <iostream>
#include <queue>
#include "tree.h"

using namespace std;

Node *parentNode = nullptr;

Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

Node::Node() {
    this->isLeafNode = false;
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
    this->maxIntChildNum = n + 1;
    this->maxLeafNodeNum = n;
    this->rootNode = nullptr;

    cout << "Instantiating B+ Tree" << endl;
    cout << " -> Nodes bounded by block size of = " << blockSize << endl;
    cout << " -> Maximum number of keys in a node: n = " << n << endl;
    cout << " -> Internal node max pointers to other nodes = " << this->maxIntChildNum << endl;
    cout << " -> Leaf node max key-record pointers = " << this->maxLeafNodeNum << endl;
    cout << "===========================================" << endl;
}

int Tree::getMaxIntChildNum() {
    return maxIntChildNum;
}

int Tree::getMaxLeafNodeNum() {
    return maxLeafNodeNum;
}

int Tree::getNodesAccessedNum() {
    return nodesAccessedNum;
}

void Tree::setNodesAccessedNum(int setNumber) {
    nodesAccessedNum = setNumber;
}

Node *Tree::getRoot() {
    return this->rootNode;
}

void Tree::setRoot(Node *ptr) {
    this->rootNode = ptr;
}

Node *Tree::firstLeftNode(Node *currentNode) {
    if (currentNode->isLeafNode)
        return currentNode;
    for (int i = 0; i < currentNode->pointer.pNode.size(); i++)
        if (currentNode->pointer.pNode[i] != nullptr)
            return firstLeftNode(currentNode->pointer.pNode[i]);

    return nullptr;
}

Node **Tree::findParentNode(Node *currentNode, Node *child) {
    /*
		Finds parentNode using depth first traversal and ignores leaf nodes as they cannot be parents
		also ignores second last level because we will never find parentNode of a leaf node during insertion using this function
	*/

    if (currentNode->isLeafNode || currentNode->pointer.pNode[0]->isLeafNode)
        return nullptr;

    for (int i = 0; i < currentNode->pointer.pNode.size(); i++) {
        if (currentNode->pointer.pNode[i] == child) {
            parentNode = currentNode;
        } else {
            //Commenting To Remove vector out of bound Error:
            //new (&currentNode->pointer.pNode) std::vector<Node*>;
            Node *temCurrent = currentNode->pointer.pNode[i];
            findParentNode(temCurrent, child);
        }
    }

    return &parentNode;
}

int Tree::countNodes() {
    Node *currentNode = rootNode;
    int count = 0;

    if (currentNode == nullptr)
        return count;

    queue<Node *> q;
    q.push(currentNode);
    count++;

    while (!q.empty()) {
        auto qSize = q.size();
        for (int i = 0; i < qSize; i++) {
            Node *u = q.front();
            q.pop();

            if (!u->isLeafNode) {
                for (Node *v: u->pointer.pNode) {
                    q.push(v);
                    count++;
                }
            }
        }
    }
    return count;
}

int Tree::countHeight() {
    int heightOfTree = 0;

    if (rootNode == nullptr) {
        return 0;
    } else {
        // start the currentNode at the rootNode
        Node *currentNode = rootNode;

        // traverse to the leaf node
        while (!currentNode->isLeafNode) {
            heightOfTree++;
            currentNode = currentNode->pointer.pNode[0];
        }

        // count leaf nodes level
        heightOfTree++;
    }
    return heightOfTree;
}