#include "disk.h"
#include "tree.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const int blockSize = 500;  // block size in bytes

void experiment1(Tree* tree, Disk* disk){
    cout << "EXPERIMENT 1" << endl;

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
        (*tree).insert(new_record->averageRating, new_record);
        count++;
    }

    // print experiment outputs
    cout << " -> No of records processed: " << count << endl;
    cout << " -> No of blocks used: " << (*disk).getBlocksUsed() << " blocks" << endl;
    cout << " -> Size of the database: " << (*disk).getBlocksUsed() * blockSize << " bytes" << endl;
}

int main() {
    // initialize a b+ tree
    Tree tree = Tree(blockSize);

    // initialize a disk of 100MB
    Disk disk = Disk((100 * 1000 * 1000), blockSize);

    // run experiment 1
    experiment1(&tree, &disk);


    cout << "End of program! " << endl;
}