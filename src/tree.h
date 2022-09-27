#ifndef TREE_H
#define TREE_H

#include <vector>
#include "dtypes.h"

class Node {
public:
    bool isLeaf;
    std::vector<unsigned char> keys;
    Node *pNextLeaf;

    union ptr {
        std::vector<Node *> pNode;
        std::vector<std::vector<Record *>> pData;

        ptr();   // To remove the error !?
        ~ptr();  // To remove the error !?
    } pointer;

    friend class Tree;

public:
    Node();
};

class Tree {
private:
    int maxIntChildLimit;
    int maxLeafNodeLimit;
    Node *root;

    void insertInternal(int x, Node **cursor, Node **child);

    Node **findParent(Node *cursor, Node *child);

    Node *firstLeftNode(Node *cursor);

public:
    Tree(int blockSize);

    Node *getRoot();

    int getMaxIntChildLimit();

    int getMaxLeafNodeLimit();

    void setRoot(Node *);

    void display(Node *cursor);

    void seqDisplay(Node *cursor);

    std::vector<Record *> *search(int key);

    void insert(unsigned char key, Record *filePtr);

    void removeKey(int key);

    void removeInternal(int x, Node *cursor, Node *child);
};


#endif
