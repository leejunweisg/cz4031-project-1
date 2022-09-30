#include <iostream>
#include <queue>
#include "tree.h"

using namespace std;

void Tree::display(Node *currentNode) {
    /*
		Depth First Display
    if (currentNode != NULL) {
        for (int i = 0; i < currentNode->keys.size(); i++)
            cout << currentNode->keys[i] << " ";
        cout << endl;
        if (currentNode->isLeafNode != true) {
            for (int i = 0; i < currentNode->ptr2TreeOrData.ptr2Tree.size(); i++)
                display(currentNode->ptr2TreeOrData.ptr2Tree[i]);
        }
    }
    */

    /*
        Level Order Display
    */
    if (currentNode == nullptr) return;
    queue<Node *> q;
    q.push(currentNode);

    while (!q.empty()) {
        auto sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node *u = q.front();
            q.pop();

            //printing keys in self
            for (int val: u->keys)
                cout << val << " ";

            cout << "|| ";//to seperate next adjacent nodes

            if (!u->isLeafNode) {
                for (Node *v: u->pointer.pNode) {
                    q.push(v);
                }
            }
        }
        cout << "_" << endl;
    }
}

void Tree::displayCurrentNode(Node *currentNode) {
    // return if node is null
    if (currentNode == nullptr) return;

    // print keys of the node
    cout << "{";
    for (auto i = 0; i < currentNode->keys.size(); i++) {
        cout << currentNode->keys.at(i);
        if (i != currentNode->keys.size() - 1)
            cout << ", ";
    }
    cout << "}" << endl;
}

void Tree::seqDisplay(Node *currentNode) {
    Node *firstLeft = firstLeftNode(currentNode);

    if (firstLeft == nullptr) {
        cout << "No Data in the Database yet!" << endl;
        return;
    }
    while (firstLeft != nullptr) {
        for (unsigned char key: firstLeft->keys) {
            cout << key << " ";
        }

        firstLeft = firstLeft->pNextLeaf;
    }
    cout << endl;
}
