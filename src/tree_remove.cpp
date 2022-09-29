#include <iostream>
#include <cstring>
#include "tree.h"

using namespace std;

void Tree::removeKey(int x) {
    Node *root = getRoot();

    // check if the B+ tree is empty
    if (root == nullptr) {
        cout << "Unable to remove: The B+ tree is empty!" << endl;
        return;
    }

    Node *cursor = root;
    Node *parent;
    int leftSibling, rightSibling;

    // traverse until the leaf node
    while (!cursor->isLeaf) {
        for (int i = 0; i < cursor->keys.size(); i++) {
            parent = cursor;
            leftSibling = i - 1;  // left side of parent
            rightSibling = i + 1;  // right side of parent

            if (x < cursor->keys[i]) {
                cursor = cursor->pointer.pNode[i];
                break;
            }
            if (i == cursor->keys.size() - 1) {
                leftSibling = i;
                rightSibling = i + 2;
                cursor = cursor->pointer.pNode[i + 1];
                break;
            }
        }
    }

    // check if the key exist in the current leaf node
    int pos = 0;
    for (pos = 0; pos < cursor->keys.size(); pos++) {
        if (cursor->keys[pos] == x) {
            break;
        }
    }

    auto itr = lower_bound(cursor->keys.begin(), cursor->keys.end(), x);

    // if the iterator points to the last key, the key was not found
    if (itr == cursor->keys.end()) {
        cout << "Unable to remove: The key '" << x << "' was not found in the B+ Tree." << endl;
        return;
    }

    // erase the vector of Record pointers from the position
    cursor->pointer.pData.erase(cursor->pointer.pData.begin() + pos);

    // shift key-pointer pairs to fill up the gap
    for (int i = pos; i < cursor->keys.size() - 1; i++) {
        cursor->keys[i] = cursor->keys[i + 1];
        cursor->pointer.pData[i] = cursor->pointer.pData[i + 1];
    }

    // resize the vectors
    auto new_size = cursor->keys.size() - 1;
    cursor->keys.resize(new_size);
    cursor->pointer.pData.resize(new_size);

    // if the cursor is the root node and is empty, the tree is empty
    if (cursor == root && cursor->keys.empty()) {
        setRoot(nullptr);
    }

    cout << "Removed '" << x << "' from the B+ Tree successfully!" << endl;

    // return if the B+ tree is still balanced
    if (cursor->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2) {
        return;
    }

    // attempt to borrow a key from the left sibling if we have a left sibling
    if (leftSibling >= 0) {
        Node *leftNode = parent->pointer.pNode[leftSibling];

        // check if left sibling has extra key to lend
        if (leftNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {

            // transfer the largest key from the left Sibling
            auto maxIdx = leftNode->keys.size() - 1;
            cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
            cursor->pointer.pData.insert(cursor->pointer.pData.begin(), leftNode->pointer.pData[maxIdx]);

            // resize the left sibling node
            leftNode->keys.resize(maxIdx);
            leftNode->pointer.pData.resize(maxIdx);

            // update the parent node
            parent->keys[leftSibling] = cursor->keys[0];
            return;
        }
    }

    // attempt to borrow a key from the right sibling, if we have a right sibling
    if (rightSibling < parent->pointer.pNode.size()) {
        Node *rightNode = parent->pointer.pNode[rightSibling];

        // check if right sibling has extra key to lend
        if (rightNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {

            // transfer the smallest key from the right Sibling
            int minIdx = 0;
            cursor->keys.push_back(rightNode->keys[minIdx]);
            cursor->pointer.pData.push_back(rightNode->pointer.pData[minIdx]);

            // resize the right sibling node
            rightNode->keys.erase(rightNode->keys.begin());
            rightNode->pointer.pData.erase(rightNode->pointer.pData.begin());

            // update the parent node
            parent->keys[rightSibling - 1] = rightNode->keys[0];
            return;
        }
    }

    // merge and delete nodes
    // check if we have a left sibling
    if (leftSibling >= 0) {
        Node *leftNode = parent->pointer.pNode[leftSibling];

        // merge the two leaf nodes by transferring the key-pointer pairs
        for (int i = 0; i < cursor->keys.size(); i++) {
            leftNode->keys.push_back(cursor->keys[i]);
            leftNode->pointer.pData.push_back(cursor->pointer.pData[i]);
        }
        // update the pointer to the next leaf node
        leftNode->pNextLeaf = cursor->pNextLeaf;

        // delete the node
        removeInternal(parent->keys[leftSibling], parent, cursor);//delete parent Node Key

    } else if (rightSibling <= parent->keys.size()) {
        Node *rightNode = parent->pointer.pNode[rightSibling];

        // merge the two leaf nodes by transferring the key-pointer pairs
        for (int i = 0; i < rightNode->keys.size(); i++) {
            cursor->keys.push_back(rightNode->keys[i]);
            cursor->pointer.pData.push_back(rightNode->pointer.pData[i]);
        }
        // update the pointer to the next leaf node
        cursor->pNextLeaf = rightNode->pNextLeaf;

        // delete the node
        removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
        delete rightNode;
    }

}

void Tree::removeInternal(int x, Node *cursor, Node *child) {
    Node *root = getRoot();

    // check if the key to be deleted is in the root
    if (cursor == root) {
        if (cursor->keys.size() == 1) {
            // if only one key is left in the root and matches the child, set child as the root
            if (cursor->pointer.pNode[1] == child) {
                setRoot(cursor->pointer.pNode[0]);
                delete cursor;
                return;
            } else if (cursor->pointer.pNode[0] == child) {
                setRoot(cursor->pointer.pNode[1]);
                delete cursor;
                return;
            }
        }
    }

    // find position of key
    int pos;
    for (pos = 0; pos < cursor->keys.size(); pos++) {
        if (cursor->keys[pos] == x) {
            break;
        }
    }

    // shift the keys
    for (int i = pos; i < cursor->keys.size() - 1; i++) {
        cursor->keys[i] = cursor->keys[i + 1];
    }

    // reduce the size of the keys vector, effectively deleting the last element
    cursor->keys.resize(cursor->keys.size() - 1);

    // find position of the node
    for (pos = 0; pos < cursor->pointer.pNode.size(); pos++) {
        if (cursor->pointer.pNode[pos] == child) {
            break;
        }
    }

    // shift the node pointers
    for (int i = pos; i < cursor->pointer.pNode.size() - 1; i++) {
        cursor->pointer.pNode[i] = cursor->pointer.pNode[i + 1];
    }

    // reduce the size of the keys vector, effectively deleting the last element
    cursor->pointer.pNode.resize(cursor->pointer.pNode.size() - 1);

    // return if the B+ tree is still balanced
    if (cursor->keys.size() >= (getMaxIntChildLimit() + 1) / 2 - 1) {
        return;
    }

    if (cursor == root) {
        return;
    }

    Node **p1 = findParent(root, cursor);
    Node *parent = *p1;

    int leftSibling, rightSibling;

    // find the left and right siblings
    for (pos = 0; pos < parent->pointer.pNode.size(); pos++) {
        if (parent->pointer.pNode[pos] == cursor) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }

    // attempt to borrow a key from the left sibling if we have a left sibling
    if (leftSibling >= 0) {
        Node *leftNode = parent->pointer.pNode[leftSibling];

        // check if left sibling has extra key to lend
        if (leftNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2) {

            // transfer the key from left sibling through parent
            auto maxIdxKey = leftNode->keys.size() - 1;
            cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
            parent->keys[leftSibling] = leftNode->keys[maxIdxKey];

            auto maxIdxPtr = leftNode->pointer.pNode.size() - 1;
            cursor->pointer.pNode.insert(cursor->pointer.pNode.begin(), leftNode->pointer.pNode[maxIdxPtr]);

            // resize the left sibling node
            leftNode->keys.resize(maxIdxKey);
            leftNode->pointer.pData.resize(maxIdxPtr);

            return;
        }
    }

    // attempt to borrow a key from the right sibling if we have a right sibling
    if (rightSibling < parent->pointer.pNode.size()) {
        Node *rightNode = parent->pointer.pNode[rightSibling];

        // check if right sibling has extra key to lend
        if (rightNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2) {

            // transfer the key from right sibling through parent
            auto maxIdxKey = rightNode->keys.size() - 1;
            cursor->keys.push_back(parent->keys[pos]);
            parent->keys[pos] = rightNode->keys[0];
            rightNode->keys.erase(rightNode->keys.begin());

            // transfer the pointer from rightSibling to cursor
            cursor->pointer.pNode.push_back(rightNode->pointer.pNode[0]);
            cursor->pointer.pNode.erase(cursor->pointer.pNode.begin());

            return;
        }
    }

    // merge nodes
    if (leftSibling >= 0) {
        // leftNode + parent key + cursor
        Node *leftNode = parent->pointer.pNode[leftSibling];
        leftNode->keys.push_back(parent->keys[leftSibling]);

        for (int val: cursor->keys) {
            leftNode->keys.push_back(val);
        }

        for (auto & i : cursor->pointer.pNode) {
            leftNode->pointer.pNode.push_back(i);
            i = nullptr;
        }

        cursor->pointer.pNode.resize(0);
        cursor->keys.resize(0);

        removeInternal(parent->keys[leftSibling], parent, cursor);
    } else if (rightSibling < parent->pointer.pNode.size()) {

        //cursor + parentkey +rightNode
        Node *rightNode = parent->pointer.pNode[rightSibling];
        cursor->keys.push_back(parent->keys[rightSibling - 1]);

        for (int val: rightNode->keys) {
            cursor->keys.push_back(val);
        }

        for (auto & i : rightNode->pointer.pNode) {
            cursor->pointer.pNode
                    .push_back(i);
            i = nullptr;
        }

        rightNode->pointer.pNode.resize(0);
        rightNode->keys.resize(0);

        removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
    }
}