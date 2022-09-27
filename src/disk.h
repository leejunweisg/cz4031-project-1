//
// Created by leejunwei on 26/9/2022.
//

#ifndef DB_PROJECT_DISK_H
#define DB_PROJECT_DISK_H

#include <cstddef>
#include <string>
#include "dtypes.h"

class Disk {
private:
    size_t blockSize;
    size_t diskSize;
    size_t blockIdx;
    size_t recordIdx;
    unsigned char *pMemAddress;

    size_t maxRecordsPerBlock;
    size_t maxBlocksInDisk;

public:
    // constructor
    Disk(size_t aDiskSize, size_t aBlockSize);

    // functions
    Record *insertRecord(const std::string &tconst, unsigned char avgRating, unsigned int numVotes);

    Record *getRecord(size_t aBlockIdx, size_t aRecordIdx);

    void printRecord(Record *record);

    void printBlock(size_t aBlockIdx);

    // misc
    size_t getMaxRecordsPerBlock();

    size_t getMaxBlocksInDisk();

    size_t getBlocksUsed();
};

#endif //DB_PROJECT_DISK_H