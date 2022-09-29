#include "disk.h"
#include "tree.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const int blockSize = 500;  // block size in bytes

void experiment12(Tree* tree, Disk* disk){
    /*
     * Combines experiments 1 and 2:
     *  -> Insert record into disk
     *  -> Build B+ tree with numVotes attribute
     */
    cout << "======EXPERIMENT 1 AND 2======" << endl;

    // declare pointer to hold newly created records
    Record *new_record = nullptr;

    // read and parse the data file from "../data/data.tsv"
    cout << "Inserting records from data file into the disk..." << endl;
    ifstream data_file("../data/data.tsv");
    string line;

    // skip first line
    getline(data_file, line);

    // insert records into the disk and index sequentially
    int count = 0;
    while (getline(data_file, line)) {
        // parse the line
        istringstream iss(line);
        string tconst, averageRating, numVotes;
        getline(iss, tconst, '\t');
        getline(iss, averageRating, '\t');
        getline(iss, numVotes, '\t');

        // insert into disk
        new_record = (*disk).insertRecord(tconst,
                                          (unsigned char) (stof(averageRating) * 10),
                                          (unsigned int) stoi(numVotes));
        // insert into tree
        (*tree).insert(new_record->numVotes, new_record);
        count++;
    }

    // print experiment 1 outputs
    cout << " -> No of records processed: " << count << endl;
    cout << " -> No of blocks used: " << (*disk).getBlocksUsed() << " blocks" << endl;
    cout << " -> Size of the database: " << (*disk).getBlocksUsed() * blockSize << " bytes" << endl;

    // print experiment 2 outputs
    cout << " -> Parameter N of the B+ Tree: " << (*tree).getMaxLeafNodeLimit() << endl;
    cout << " -> No of nodes in the B+ Tree: " << (*tree).countNodes() << endl;
    cout << " -> Height of the B+ Tree: " << (*tree).countDepth() << endl;
    cout << " -> Content of root node: ";
    (*tree).displaySingleNode((*tree).getRoot());
    cout << " -> Content of root's first child node: ";
    (*tree).displaySingleNode((*tree).getRoot()->pointer.pNode[0]);

    // reset number of index nodes accessed
    tree->setNumIndexNodesAccessed(0);
}

void experiment3 (Tree* tree, Disk* disk) {
    cout << "======EXPERIMENT 3======" << endl;

    cout << "Index Nodes accessed: " << endl;
    vector<Record*>* result = tree->search(500, true);

    int resultSize = result->size();
    int totalAveRating = 0;

    for (int i = 0; i < result->size(); i++){
        totalAveRating += result->at(i)->averageRating;
        // disk->printRecord(result->at(i));
    }

    int numIndexNodesAccessed = tree->getNumIndexNodesAccessed();

    cout << "Number of Index Nodes Accessed: " << numIndexNodesAccessed << endl;
    cout << "total ave rating: " << totalAveRating << endl;
    cout << "result size: " << resultSize << endl;
    cout << "Average of ave rating: " << totalAveRating/resultSize << endl;

    // print first 5 data blocks accessed
    for (int i = 0; i < 5; i++){
        disk->printBlock(disk->getBlockId(result->at(i)));
    }

    vector<int> blockIdList;

    // push block id of records into a list
    for (int i = 0; i < result->size(); i++){
        blockIdList.push_back(disk->getBlockId(result->at(i)));
    }

    cout << "Number of records accessed: " << result->size() << endl;
    cout << "Number of entries in blockIdList(before removing duplicate): " << blockIdList.size() << endl;

    // sort and remove duplicate blockId
    sort(blockIdList.begin(),blockIdList.end());
    blockIdList.erase(unique(blockIdList.begin(),blockIdList.end()),blockIdList.end());
    cout << "Number of data blocks accessed(unique): " << blockIdList.size() << endl;

    // reset number of index nodes accessed
    tree->setNumIndexNodesAccessed(0);
}

void experiment5 (Tree* tree, Disk* disk) {
    cout << "======EXPERIMENT 5======" << endl;

    // the original num of nodes in the tree
    int numNodes = tree->countNodes();

    tree->removeKey(1000);

    int numUpdatedNodes = tree->countNodes();

    vector<Record*>* result = tree->search(1000, true);

    cout << "Number of times that a node is deleted (or two nodes are merged): " << numNodes - numUpdatedNodes << endl;
    cout << "Number of nodes in the updated B+ tree: " << numUpdatedNodes << endl;
    cout << "Height of the updated B+ tree: " << tree->countDepth() << endl;
    cout << "root node of the updated B+ tree: " << endl;
    tree->displaySingleNode(tree->getRoot());
    cout << "first child node of the updated B+ tree: " << endl;
    tree->displaySingleNode(tree->getRoot()->pointer.pNode[0]);

    if (result != nullptr) {
        cout << "result size: " << result->size() << endl;

        for (int i = 0; i < result->size(); i++){
            disk->printRecord(result->at(i));
        }
    }

    // reset number of index nodes accessed
    tree->setNumIndexNodesAccessed(0);
}

int main() {
    // initialize a b+ tree
    Tree tree = Tree(blockSize);

    // initialize a disk of 100MB
    Disk disk = Disk((100 * 1000 * 1000), blockSize);

    // run experiment 1 and 2
    experiment12(&tree, &disk);

    // run experiment 3
    experiment3(&tree, &disk);

    // run experiment 5
    experiment5(&tree, &disk);



    cout << "End of program! " << endl;
}
