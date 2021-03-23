#include "stdlib.h"
#include "page_table.h"
#include "math.h"
#include "page_table.h"
#include "output_mode_helpers.h"
#include <vector>
#include "level.h"

using namespace std;

PageTable::PageTable(int levelCount, vector<int> levelAry){
    PageTable::levelCountTable = levelCount;
    printf("PAGTEABLE LEVEL COUNT: %i\n", levelCount);
    bitmaskAry = new uint32_t [levelCount];
    entrycount = new uint32_t [levelCount];
    shiftAry = new uint32_t [levelCount];

    int ADDRS = 31;
    int shift = 0;
    uint32_t bitmasks[levelCount];
    
    for (int i = 0; i < levelCount ; i++){
        entrycount[i] = pow(2, levelAry[i]);
        shiftAry[i] = 32 - (shift+=levelAry[i]);
       // printf("SHIFT ARRAY : %i\n",shiftAry[i]);
        uint32_t tmp = 0x0;
        for (int j = 0; j < levelAry[i]; j++){
            tmp += pow(2, ADDRS);
            
            ADDRS--;
        }
        bitmasks[i] = tmp;
        bitmaskAry[i] = tmp;
    }
    //bitmaskAry = bitmasks;
    
    report_bitmasks(levelCount, bitmasks);
    if (levelCount == 1){
        printf("ONLY ONE LEVEL TODO MAP\n");
       rootNodeptr = new Level(0, new map[entrycount[0]]());
    } else {
        rootNodeptr = new Level(0, false, this);
    }
}

void PageTable::pageInsert(uint32_t logicalAddress) {
    // if(rootNodeptr == nullptr)
    // {
    //     rootNodeptr = new Level(0, 1 == levelCountTable, this);
    // }
    pageInsert(rootNodeptr, logicalAddress);
}
void PageTable::pageInsert(Level *levelPtr, uint32_t logicalAddress) {
    int depth = levelPtr->depth;
    uint32_t page = logicalToPage(logicalAddress, PageTable::bitmaskAry[depth],
                                  PageTable::shiftAry[depth]);

    // // Leaf node
    if (depth == levelCountTable - 1) {
       if (levelPtr->mapPtr[page].isValid)
         hits++;
        else {
            levelPtr->mapPtr[page].frame = currentFrame++;
            levelPtr->mapPtr[page].isValid = true;
            misses++;
        }
        return;
     } // Inner node
    else if (levelPtr->NextLevelPtr[page] == nullptr) {
        int nextEntries = PageTable::entrycount[depth + 1];
         if (depth == levelCountTable - 2) {
            levelPtr->NextLevelPtr[page] = new Level(depth + 1,
                                                      new map[nextEntries]());
           totalMemory += sizeof(map) * nextEntries;
        } else {
             levelPtr->NextLevelPtr[page] = new Level(depth + 1,
                                                      new Level *[nextEntries]());
            totalMemory += sizeof(Level) * nextEntries;
        }
         totalMemory += sizeof(Level);
    }
     pageInsert(levelPtr->NextLevelPtr[page], logicalAddress);
}

bool PageTable::pageLookup(uint32_t logicalAddress, uint32_t &frame) {
    
    for (int i = 0; i < levelCountTable; i++) {
       uint32_t page = logicalToPage(logicalAddress, bitmaskAry[i], shiftAry[i]);
    }
    return false;
}

uint32_t PageTable::logicalToPage(uint32_t logicalAddress, uint32_t mask,
                                  uint32_t shift) {
    return (logicalAddress & mask) >> shift;
}