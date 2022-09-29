#include "disk.h"
#include "tree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "dtypes.h"

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
void experiment4 (Tree* tree, Disk* disk,int key1 ,int key2) {
    cout << "======EXPERIMENT 4======" << endl;
        Node* result = tree->searchRange(key1);
        (*tree).displaySingleNode(result);

        int count=0;
        int i = 0;
        int total_average_rating=0;
        int idx=lower_bound(result->keys.begin(), result->keys.end(), key1) - result->keys.begin();
        vector<Record *> record;
//        for (int x=0;x<5;x++) { //e first 5 index nodes or data blocks
//            record = tree->searchRange(key1)->pointer.pData[idx+x];
//            disk->printBlock(disk->getBlockId(record.at(0)));
//        }

        while(result->keys.at(i+idx)<key2) {
            record = result->pointer.pData[idx+i];
            count++;
            total_average_rating+=record.at(0)->averageRating;
            //print records
            for (int i = 0; i < record.size(); i++){
                disk->printRecord(record.at(i));
            }
            i++;
            if(idx+i==21){
                //disk->printBlock(disk->getBlockId(record.at(0))); //print record range from key1 to key 2
                i=0;
                idx=0;
                result=result->pNextLeaf;
            }
        }

        cout << "Number of Index Nodes Accessed: " << count << endl;
        cout << "total average rating value: " << total_average_rating << endl;
        cout << "Average of Total AverageRating: " << total_average_rating/count/10.0 << endl;
}
void experiment5 (Tree* tree, Disk* disk) {
    cout << "======EXPERIMENT 5======" << endl;

    // tree->removeKey(1000);

    vector<Record*>* result = tree->search(1000, true);

    cout << "result size: " << result->size();

    for (int i = 0; i < result->size(); i++){
        disk->printRecord(result->at(i));
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
    // experiment3(&tree, &disk);

    // run experiment 4
    experiment4(&tree, &disk, 30000,40000);

    // run experiment 5
    // experiment5(&tree, &disk);



    cout << "End of program! " << endl;
}
