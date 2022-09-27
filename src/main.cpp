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
    cout << "EXPERIMENT 1 AND 2" << endl;

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
}

int main() {
    // initialize a b+ tree
    Tree tree = Tree(blockSize);

    // initialize a disk of 100MB
    Disk disk = Disk((100 * 1000 * 1000), blockSize);

    // run experiment 1 and 2
    experiment12(&tree, &disk);

    // run experiment 2


    cout << "End of program! " << endl;
}