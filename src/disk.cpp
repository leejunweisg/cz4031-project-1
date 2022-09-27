#include "disk.h"

#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

Disk::Disk(size_t aDiskSize, size_t aBlockSize) {

    // set disk size and block size
    diskSize = aDiskSize;
    blockSize = aBlockSize;

    // allocate memory on the heap
    pMemAddress = new unsigned char[diskSize]();

    // calculate maxes
    maxRecordsPerBlock = std::floor(blockSize / sizeof(Record));
    maxBlocksInDisk = std::floor(diskSize / blockSize);

    // initialize indexes to 0
    blockIdx = 0;
    recordIdx = 0;
    cout << "Instantiating Disk" << endl;
    cout << " -> Disk Size: " << aDiskSize << " bytes" << endl;
    cout << " -> Block Size: " << aBlockSize << " bytes" << endl;
    cout << " -> Max Records Per Block: " << maxRecordsPerBlock << endl;
    cout << " -> Max Blocks in Disk: " << maxBlocksInDisk << endl;
    cout << "===========================================" << endl;

}

Record *Disk::insertRecord(const std::string &tconst, unsigned char avgRating, unsigned int numVotes) {
    /*
        * Inserts a record at the next available memory location pointed by blockIdx and recordIdx
        *
        * Returns:
        * -> If successful, the pointer to the inserted record (useful for building b+ tree) is returned
        * -> If disk is full, return nullptr.
        */

    // if disk is full, return nullptr
    if (blockIdx >= maxBlocksInDisk) {
        return nullptr;
    }

    // get pointer to the new record
    Record *new_record = getRecord(blockIdx, recordIdx);

    // set values into the  new record
    strncpy(new_record->tconst, tconst.c_str(), sizeof(new_record->tconst) - 1);
    new_record->numVotes = numVotes;
    new_record->averageRating = avgRating;

    // increment recordIdx
    recordIdx++;

    // if the current block is full after this insertion, move to next block and reset recordIdx to 0
    if (recordIdx == maxRecordsPerBlock) {
        blockIdx++;
        recordIdx = 0;
    }

    // return a pointer to the inserted record
    return new_record;
}


Record *Disk::getRecord(size_t aBlockIdx, size_t aRecordIdx) {
    /*
     * Returns a pointer to a record!
     *
     * The offset consists of two parts:
     * -> blockIdx x BLOCK_SIZE: offset to the start of a specified block
     * -> recordIdx x recordSize: offset to the start of a record in a block
     */
    size_t offset = (aBlockIdx * blockSize) + (aRecordIdx * sizeof(Record));
    return reinterpret_cast<Record *>(pMemAddress + offset);
}

void Disk::printRecord(Record *record) {
    printf("%s / %.1f / %u\n", record->tconst, (float) record->averageRating / 10, record->numVotes);
}

void Disk::printBlock(size_t aBlockIdx) {
    std::cout << "Contents of the block (blockIdx=" << aBlockIdx << ") :" << std::endl;

    // print record one by one in the block
    for (int i = 0; i < maxRecordsPerBlock; i++) {
        printRecord(getRecord(aBlockIdx, i));
    }
}

// misc
size_t Disk::getMaxRecordsPerBlock() {
    return maxRecordsPerBlock;
}

size_t Disk::getMaxBlocksInDisk() {
    return maxBlocksInDisk;
}

size_t Disk::getBlocksUsed() {
    return blockIdx + 1;
}