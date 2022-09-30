#ifndef TREE_H
#define TREE_H

#include <vector>
#include "dtypes.h"

class Node {
public:
    bool isLeafNode;
    std::vector<int> keys;
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
    int maxIntChildNum;
    int maxLeafNodeNum;
    int nodesAccessedNum;
    Node *rootNode;

    void insertInternal(int x, Node **currentNode, Node **child);

    Node **findParentNode(Node *currentNode, Node *child);

    Node *firstLeftNode(Node *currentNode);

public:
    explicit Tree(int blockSize);

    Node *getRoot();

    int countNodes();

    int countHeight();

    int getMaxIntChildNum();

    int getMaxLeafNodeNum();

    int getNodesAccessedNum();

    void setNodesAccessedNum(int setNumber);

    void setRoot(Node *);

    void display(Node *currentNode);

    void displayCurrentNode(Node *currentNode);

    void seqDisplay(Node *currentNode);

    std::vector<Record *> *search(int key, bool printLeafNode);

    Node *searchNode(int key, bool printLeafNode);

    void insert(int key, Record *pRecord);

    void removeKey(int key);

    void removeInternal(int x, Node *currentNode, Node *child);

};


#endif
