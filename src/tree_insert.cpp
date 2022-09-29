#include <iostream>
#include "dtypes.h"
#include "tree.h"

using namespace std;

void Tree::insert(int key, Record *filePtr) {  //in Leaf Node
    /*
		1. If the node has an empty space, insert the key/reference pair into the node.
		2. If the node is already full, split it into two nodes, distributing the keys
		evenly between the two nodes. If the node is a leaf, take a copy of the minimum
		value in the second of these two nodes and repeat this insertion algorithm to
		insert it into the parent node. If the node is a non-leaf, exclude the middle
		value during the split and repeat this insertion algorithm to insert this excluded
		value into the parent node.
	*/

    // search the tree for the key
    vector<Record *> *result = search(key, false);

    // if the key exists, simply add the current Record pointer to the existing vector and return
    if (result != nullptr) {
        (*result).push_back(filePtr);
        return;
    }

    // check if the B+ tree is empty
    if (root == nullptr) {
        // the tree is empty, create new root node and store the first Record pointer
        root = new Node;
        root->isLeaf = true;
        root->keys.push_back(key);
        new(&root->pointer.pData) vector<vector<Record *>>;
        root->pointer.pData.push_back(vector<Record *>{filePtr});
        return;
    } else {
        // the tree is not empty, need to look for a slot for the current record
        Node *cursor = root;
        Node *parent = nullptr;

        // traverse until the leaf node
        while (!cursor->isLeaf) {
            parent = cursor;
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->pointer.pNode[idx];
        }

        // check if the current node at the cursor has space for another key-pointer pair
        if (cursor->keys.size() < maxLeafNodeLimit) {
            // the current node is not full, so we have to find the correct position to insert it
            int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

            // temporarily append the key-pointer pair to the vectors to expand its size
            cursor->keys.push_back(key);
            cursor->pointer.pData.push_back(vector<Record *>{filePtr});

            if (idx != cursor->keys.size() - 1) {
                // shift the existing records
                for (auto j = cursor->keys.size() - 1;
                     j > idx; j--) {  // shifting the position for keys and datapointer
                    cursor->keys[j] = cursor->keys[j - 1];
                    cursor->pointer.pData[j] = cursor->pointer.pData[j - 1];
                }

                // finally, insert the key-pointer pair into the node
                cursor->keys[idx] = key;
                cursor->pointer.pData[idx] = vector<Record *>{filePtr};
            }
        } else {
            // the current node is full, we have to split the node
            vector<int> virtualNode(cursor->keys);
            vector<vector<Record *>> virtualDataNode(cursor->pointer.pData);

            // find the correct position to insert it
            int i = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

            // temporarily append the key-pointer pair to the vectors to expand its size
            virtualNode.push_back(key);
            virtualDataNode.push_back(vector<Record *>{filePtr});

            if (i != virtualNode.size() - 1) {
                // shift the existing records
                for (auto j = virtualNode.size() - 1; j > i; j--) {
                    virtualNode[j] = virtualNode[j - 1];
                    virtualDataNode[j] = virtualDataNode[j - 1];
                }

                // finally, insert the key-pointer pair into the node
                virtualNode[i] = key;
                virtualDataNode[i] = vector<Record *>{filePtr};
            }

            // create new leaf node
            Node *newLeaf = new Node;
            newLeaf->isLeaf = true;
            new(&newLeaf->pointer.pData) vector<Record *>;

            // swap pNextLeaf pointers
            Node *temp = cursor->pNextLeaf;
            cursor->pNextLeaf = newLeaf;
            newLeaf->pNextLeaf = temp;

            // resize and copy key-pointer pairs into the old node
            cursor->keys.resize((maxLeafNodeLimit) / 2 + 1);
            cursor->pointer.pData.reserve((maxLeafNodeLimit) / 2 + 1);
            for (i = 0; i <= (maxLeafNodeLimit) / 2; i++) {
                cursor->keys[i] = virtualNode[i];
                cursor->pointer.pData[i] = virtualDataNode[i];
            }

            // copy key-pointer pairs into the newly created node
            for (i = (maxLeafNodeLimit) / 2 + 1; i < virtualNode.size(); i++) {
                newLeaf->keys.push_back(virtualNode[i]);
                newLeaf->pointer.pData.push_back(virtualDataNode[i]);
            }

            // if cursor points to root, create a new node
            if (cursor == root) {
                Node *newRoot = new Node;
                newRoot->keys.push_back(newLeaf->keys[0]);
                new(&newRoot->pointer.pNode) vector<Node *>;
                newRoot->pointer.pNode.push_back(cursor);
                newRoot->pointer.pNode.push_back(newLeaf);
                root = newRoot;
            } else {
                // insert new key into the parent node
                insertInternal(newLeaf->keys[0], &parent, &newLeaf);
            }
        }
    }
}

void Tree::insertInternal(int x, Node **cursor, Node **child) {  //in Internal Nodes
    // check if the current node is full
    if ((*cursor)->keys.size() < maxIntChildLimit - 1) {
        // the current node is not full, so we have to find the correct position to insert it
        int idx = upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();

        // temporarily append the key-pointer pair to the vectors to expand its size
        (*cursor)->keys.push_back(x);
        (*cursor)->pointer.pNode.push_back(*child);

        // shift the existing key-pointer pairs
        if (idx != (*cursor)->keys.size() - 1) {
            for (auto j = (*cursor)->keys.size() - 1; j > idx; j--) {
                (*cursor)->keys[j] = (*cursor)->keys[j - 1];
            }

            for (auto j = (*cursor)->pointer.pNode.size() - 1; j > (idx + 1); j--) {
                (*cursor)->pointer.pNode[j] = (*cursor)->pointer.pNode[j - 1];
            }

            // finally, insert the key-pointer pair into the node
            (*cursor)->keys[idx] = x;
            (*cursor)->pointer.pNode[idx + 1] = *child;
        }
    } else {  //splitting
        // the current node is full, we have to split the node
        vector<int> virtualKeyNode((*cursor)->keys);
        vector<Node *> virtualTreePtrNode((*cursor)->pointer.pNode);

        // find the correct position to insert it
        int idx = upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) -
                  (*cursor)->keys.begin();

        // temporarily append the key-pointer pair to the vectors to expand its size
        virtualKeyNode.push_back(x);
        virtualTreePtrNode.push_back(*child);

        // shift the existing key-pointer pairs
        if (idx != virtualKeyNode.size() - 1) {
            for (auto j = virtualKeyNode.size() - 1; j > idx; j--) {
                virtualKeyNode[j] = virtualKeyNode[j - 1];
            }

            for (auto j = virtualTreePtrNode.size() - 1; j > (idx + 1); j--) {
                virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
            }

            // finally, insert the key-pointer pair into the node
            virtualKeyNode[idx] = x;
            virtualTreePtrNode[idx + 1] = *child;
        }

        int partitionKey;  // middle element excluded
        partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)];  // split is right-biased
        auto partitionIdx = (virtualKeyNode.size() / 2);

        // resize and copy key-pointer pairs into the old node
        (*cursor)->keys.resize(partitionIdx);
        (*cursor)->pointer.pNode.resize(partitionIdx + 1);
        (*cursor)->pointer.pNode.reserve(partitionIdx + 1);
        for (int i = 0; i < partitionIdx; i++) {
            (*cursor)->keys[i] = virtualKeyNode[i];
        }

        for (int i = 0; i < partitionIdx + 1; i++) {
            (*cursor)->pointer.pNode[i] = virtualTreePtrNode[i];
        }

        Node *newInternalNode = new Node;
        new(&newInternalNode->pointer.pNode) std::vector<Node *>;

        // copy key-pointer pairs into the newly created node
        for (auto i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
            newInternalNode->keys.push_back(virtualKeyNode[i]);
        }

        for (auto i = partitionIdx + 1;
             i < virtualTreePtrNode.size(); i++) {  // because only key is excluded not the pointer
            newInternalNode->pointer.pNode.push_back(virtualTreePtrNode[i]);
        }

        // if cursor points to root, create a new node
        if ((*cursor) == root) {
            Node *newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new(&newRoot->pointer.pNode) std::vector<Node *>;
            newRoot->pointer.pNode.push_back(*cursor);
            newRoot->pointer.pNode.push_back(newInternalNode);
            root = newRoot;
        } else {
            insertInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
        }
    }
}

