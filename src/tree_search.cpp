#include <iostream>
#include <algorithm>
#include "dtypes.h"
#include "tree.h"

using namespace std;

vector<Record*>* Tree::search(int key) {
    /*
     * Searches the B+ tree for a key and returns the corresponding pointer to a vector of Record pointers.
     * If the key is not found in the tree, a nullptr is returned.
     */

    // check if the B+ tree is empty
    if (root == nullptr) {
        return nullptr;
    } else {
        // start the cursor at the root node
        Node *cursor = root;

        // traverse to the leaf node
        while (!cursor->isLeaf) {
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->pointer.pNode[idx];
        }

        // binary search of the keys in the leaf node
        int idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

        // key not found
        if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
            return nullptr;
        }

        // return the corresponding pointer of the key
        return &cursor->pointer.pData[idx];
    }
}
