#include <iostream>
#include <queue>
#include "tree.h"

using namespace std;

void Tree::display(Node *cursor) {
    /*
		Depth First Display
    if (cursor != NULL) {
        for (int i = 0; i < cursor->keys.size(); i++)
            cout << cursor->keys[i] << " ";
        cout << endl;
        if (cursor->isLeaf != true) {
            for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
                display(cursor->ptr2TreeOrData.ptr2Tree[i]);
        }
    }
    */

    /*
        Level Order Display
    */
    if (cursor == nullptr) return;
    queue<Node *> q;
    q.push(cursor);

    while (!q.empty()) {
        auto sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node *u = q.front();
            q.pop();

            //printing keys in self
            for (int val: u->keys)
                cout << val << " ";

            cout << "|| ";//to seperate next adjacent nodes

            if (!u->isLeaf) {
                for (Node *v: u->pointer.pNode) {
                    q.push(v);
                }
            }
        }
        cout << "_" << endl;
    }
}

void Tree::seqDisplay(Node *cursor) {
    Node *firstLeft = firstLeftNode(cursor);

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
