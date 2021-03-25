#include "stdlib.h"
#include "page_table.h"
#include "math.h"
#include "page_table.h"
#include "output_mode_helpers.h"
#include <vector>
#include "level.h"
#include "map.h"


using namespace std;

PageTable::PageTable(int levelCount, vector<int> levelAry){
    PageTable::levelCountTable = levelCount;
    // printf("PAGTEABLE LEVEL COUNT: %i\n", levelCount);
    bitmaskAry = new uint32_t [levelCount];
    entrycount = new uint32_t [levelCount];
    shiftAry = new uint32_t [levelCount];

    int ADDRS = 31;
    int shift = 0;
    //uint32_t bitmasks[levelCount];
    
    for (int i = 0; i < levelCount ; i++){
        entrycount[i] = pow(2, levelAry[i]);
        shiftAry[i] = 32 - (shift+=levelAry[i]);
       // printf("SHIFT ARRAY : %i\n",shiftAry[i]);
        uint32_t tmp = 0x0;
        for (int j = 0; j < levelAry[i]; j++){
            tmp += pow(2, ADDRS);
            
            ADDRS--;
        }
        //bitmasks[i] = tmp;
        bitmaskAry[i] = tmp;
    }
    
   //report_bitmasks(levelCount, bitmasks);
    if (levelCount == 1){ 
        rootNodeptr = new Level(0, new map[entrycount[0]]());
        totalMemory += sizeof(map) * entrycount[0];
    } else {
        rootNodeptr = new Level(0, false, this);
        totalMemory += sizeof(Level) * entrycount[0];
    }
    totalMemory += sizeof(Level);
    totalMemory += sizeof(PageTable);
}
void PageTable::pageInsert(uint32_t logicalAddress) {
    
    pageInsert(rootNodeptr, logicalAddress);
}  
void PageTable::pageInsert(Level *currentPointer, uint32_t logicalAddress) {
    
    int depth = currentPointer->depth;
    uint32_t page = logicalToPage(logicalAddress,
    PageTable::bitmaskAry[depth],
    PageTable::shiftAry[depth]);
    
    if(depth == levelCountTable-1){
        if(currentPointer->mapPointer[page].isLeaf){hits++; totalADDRS++;}
        else {
            currentPointer->mapPointer[page].frame = currentFrame++;   
            currentPointer->mapPointer[page].isLeaf = true;
            totalADDRS++;}
        return;
    }
    else if(currentPointer->NextLevelPtr[page] == nullptr){
        int minusCount = levelCountTable - 2;
        int size = PageTable::entrycount[depth + 1];
        if(depth == minusCount){
            currentPointer->NextLevelPtr[page] = new Level(depth+1, new map[size]());
        } else{
            currentPointer->NextLevelPtr[page] = new Level(depth+1, new Level *[size]());
        }
    }
    //printf("Calling FRAME: %i\n", currentPointer->mapPointer[page].frame);
    pageInsert(currentPointer->NextLevelPtr[page], logicalAddress);
}


bool PageTable::pageLookup(uint32_t logical, uint32_t &frame) {
    Level *currentPointer = rootNodeptr;
    //printf("BEFORE COUNT: %i\n", levelCountTable);
   for(int i =0; i < levelCountTable; i++){
       //printf("COUNT: %i\n", levelCountTable);
       if(currentPointer == nullptr){return false;}
       uint32_t currentPG = logicalToPage(logical, bitmaskAry[i], shiftAry[i]);
    if(currentPointer->mapPointer){
        frame = currentPointer->mapPointer[currentPG].frame;
        return true;
    }
    currentPointer = currentPointer->NextLevelPtr[currentPG];
   } return false;
}

uint32_t PageTable::logicalToPage(uint32_t logicalAddress, uint32_t mask, uint32_t shift) {
    return (logicalAddress & mask) >> shift;
}
void PageTable::logicalToPhysical(uint32_t logical, int offset){
    uint32_t frame;
    uint32_t offAddr = logical;
    uint32_t mask =  pow(2,32) - 1;
    int pageSize = pow(2, offset);
    if(pageLookup(logical, frame)){
        
        for(int i = 0; i < levelCountTable; i++){
            mask -= bitmaskAry[i];
        }
        offAddr = offAddr & mask;
        unsigned int physicalADDRESS = (frame * pageSize) + offAddr;
        report_logical2physical(logical, physicalADDRESS);
    }
    return;
    
}

void PageTable::pageToFrame(uint32_t logical)
{
    uint32_t tmpFrame;
    uint32_t pageArray[levelCountTable];
    if(pageLookup(logical, tmpFrame)){
        for(int i =0; i < levelCountTable ; i++){
            pageArray[i] = logicalToPage(logical, bitmaskAry[i], shiftAry[i]);
        }
        report_pagemap(logical, levelCountTable, pageArray, tmpFrame);
    }
}