#include "disk.h"
#include "tree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <assert.h>

using namespace std;

const int blockSize = 500;  // block size in bytes

void experiment12(Tree *tree, Disk *disk) {
    /*
     * Combines experiments 1 and 2:
     *  -> Insert record into disk
     *  -> Build B+ tree with numVotes attribute
     */
    cout << "EXPERIMENT 1 & 2" << endl;

    // declare pointer to hold newly created records
    Record *new_record = nullptr;

    // read and parse the data file from "../data/data.tsv"
    cout << "Inserting records from the data file into disk and building index..." << endl;
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

    cout << " -> No of records processed: " << count << endl;

    // print experiment 1 outputs
    cout << " -> No of blocks used: " << (*disk).getBlocksUsed() << " blocks" << endl;
    cout << " -> Size of the database (blocks used x blockSize): " << (*disk).getBlocksUsed() * blockSize << " bytes"
         << endl;

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

    cout << "===========================================" << endl;
}

void experiment3(Tree *tree, Disk *disk) {
    cout << "EXPERIMENT 3" << endl;

    // retrieve records with numVotes = 500
    cout << " -> Index Nodes accessed: " << endl;
    vector<Record *> *result = tree->search(500, true);

    // print number of index nodes accessed during the search
    cout << " -> No of Index nodes accessed: " << tree->getNumIndexNodesAccessed() << endl;

    // store all block numbers of the records into a vector
    vector<size_t> blockIDList;
    for (Record *record: (*result)) {
        blockIDList.push_back(disk->getBlockId(record));
    }

    // remove duplicates by inserting blockIDs into a set
    set<size_t> s;
    for (size_t blkID: blockIDList) {
        s.insert(blkID);
    }

    // print content of first 5 data blocks accessed
    cout << " -> Data Blocks accessed: " << endl;
    for (int i = 0; i < 5 && i < blockIDList.size(); i++) {
        disk->printBlock(blockIDList.at(i));
    }

    // print number of data blocks accessed
    cout << " -> No of Data blocks accessed: " << blockIDList.size() << endl;
    cout << " -> No of unique Data blocks accessed: " << s.size() << endl;

    // compute average of "averageRating"
    unsigned int total = 0;
    for (Record *record: (*result)) {
        total += record->averageRating;
    }
    cout << " -> Average of averageRating: " << ((float) total / 10) / result->size() << endl;

    // reset number of index nodes accessed
    tree->setNumIndexNodesAccessed(0);

    cout << "===========================================" << endl;
}

void experiment5(Tree *tree, Disk *disk) {
    cout << "EXPERIMENT 5" << endl;

    // original number of nodes in the tree
    int numNodes = tree->countNodes();

    tree->removeKey(1000);

    // current number of nodes after removal of key=100
    int numUpdatedNodes = tree->countNodes();

    cout << " -> No of times that a node is deleted (or two nodes are merged): " << numNodes - numUpdatedNodes << endl;
    cout << " -> No of nodes in the updated B+ tree: " << numUpdatedNodes << endl;
    cout << " -> Height of the updated B+ tree: " << tree->countDepth() << endl;
    cout << " -> Content of root node: ";
    tree->displaySingleNode(tree->getRoot());
    cout << " -> Content of root's first child node: ";
    tree->displaySingleNode(tree->getRoot()->pointer.pNode[0]);

    // verify that the key has been deleted
    vector<Record *> *result = tree->search(1000, false);
    assert(result == nullptr);

    // reset number of index nodes accessed
    tree->setNumIndexNodesAccessed(0);

    cout << "===========================================" << endl;
}

int main() {
    // instantiate a disk of 100MB
    Disk disk = Disk((100 * 1000 * 1000), blockSize);

    // instantiate an empty b+ tree
    Tree tree = Tree(blockSize);

    // run experiment 1 and 2
    experiment12(&tree, &disk);

    // run experiment 3
    experiment3(&tree, &disk);

    // run experiment 5
    experiment5(&tree, &disk);


    cout << "End of program! " << endl;
}
