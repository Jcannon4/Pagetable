#include <cstdint>
#include <cstdio>
#include <vector>
//#include "level.h"
#ifndef PAGETABLE_H
#define PAGETABLE_H

using namespace std;
class Level;
class map;
class PageTable {
    public:
    PageTable(int levels, vector<int> levelsAry);
    uint32_t *bitmaskAry;
    uint32_t *shiftAry;
    uint32_t *entrycount;

    int hits = 0;
    //int misses = 0;
    int totalMemory = 0;
    uint32_t currentFrame = 0;
    int totalADDRS = 0;
    unsigned int levelCountTable;
    Level *rootNodeptr;

    void pageInsert(uint32_t logicalAddress);
    void pageInsert(Level *levelPtr, uint32_t logicalAddress);

    static uint32_t logicalToPage(uint32_t logical_address, uint32_t mask, uint32_t shift);
    
   bool pageLookup(uint32_t logical, uint32_t &frame);

    void logicalToPhysical(uint32_t logicalAddress, uint32_t &physical, int offset);

    void pageToFrame();
    void pageToFrame(FILE *dumpFile, uint32_t page);
    void pageToFrame(FILE *dumpFile, Level *levelPtr, uint32_t page);

   
};


#endif