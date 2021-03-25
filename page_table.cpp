#include "stdlib.h"
#include "page_table.h"
#include "math.h"
#include "page_table.h"
#include "output_mode_helpers.h"
#include <vector>
#include "level.h"
#include "map.h"


using namespace std;

//CONSTRUCTOR TO CREATE OUR PAGETABLE
PageTable::PageTable(int levelCount, vector<int> levelAry){
    PageTable::levelCountTable = levelCount;
    //INITIALIZE OUR ARRAYS 
    bitmaskAry = new uint32_t [levelCount];
    entrycount = new uint32_t [levelCount];
    shiftAry = new uint32_t [levelCount];

    int ADDRS = 31;
    int shift = 0;
   
   //ITERATE THROUGH OUR LEVEL COUNT AND ALLOCATE DATA TO PAGETABLE ARRAYS 
    for (int i = 0; i < levelCount ; i++){
        entrycount[i] = pow(2, levelAry[i]); //2^4 OR 2^6 (WHICHEVER THAT LEVEL IS)
        shiftAry[i] = 32 - (shift+=levelAry[i]); //BITS WE NEED TO SHIT FOR THAT SPOT
       
        uint32_t tmp = 0x0;
        for (int j = 0; j < levelAry[i]; j++){
            tmp += pow(2, ADDRS);
            
            ADDRS--;
        }
        //GIVES US OUR BITMASK ARRAY FOR EACH LEVEL
        bitmaskAry[i] = tmp;
    }
    
   //EDGE CASE FOR A ONE LEVEL PAGETABLE CREATE A MAP OFF THE BAT
    if (levelCount == 1){ 
        rootNodeptr = new Level(0, new map[entrycount[0]]());
        totalMemory += sizeof(map) * entrycount[0];
    } else {
        //FOR PAGETABLES GREATER THAN LENGTH ONE CREATE A LEVEL
        rootNodeptr = new Level(0, false, this);
        totalMemory += sizeof(Level) * entrycount[0];
    }
    //KEEP TRACK OF OUR MEMORY ALLOCATION
    totalMemory += sizeof(Level);
    totalMemory += sizeof(PageTable);
}
void PageTable::pageInsert(uint32_t logicalAddress) {
    //SEPARATE PAGEINSERT FUNCTIONS, THIS OUR DRIVER INSERT
    pageInsert(rootNodeptr, logicalAddress);
}  
//THIS IS OUR IMPLICIT PAGETABLE FUNCTION THAT USES RECURSION AND NEEDS
//PARAMETERS NOT AVAILABLE IN THE DRIVER
void PageTable::pageInsert(Level *currentPointer, uint32_t logicalAddress) {
    
    int depth = currentPointer->depth;
    uint32_t page = logicalToPage(logicalAddress,
    PageTable::bitmaskAry[depth],
    PageTable::shiftAry[depth]);
    
    //CHECKING IF WE NEED A HIT OR MISS
    if(depth == levelCountTable-1){
        //HERE IS OUR HIT
        if(currentPointer->mapPointer[page].isLeaf){hits++; totalADDRS++;}
        else {
            //THIS IS OUR MISS, BUT JUST INCREASE TOTALADDRESS SINCE
            //THAT WILL ALLOW US TO SUBTRACT FROM HITS TO FIND MISS
            currentPointer->mapPointer[page].frame = currentFrame++;   
            currentPointer->mapPointer[page].isLeaf = true;
            totalADDRS++;}
        return;
    }
    //CREATE A NEW LEVEL OR MAP
    else if(currentPointer->NextLevelPtr[page] == nullptr){
        int minusCount = levelCountTable - 2;
        int size = PageTable::entrycount[depth + 1];
        if(depth == minusCount){
            currentPointer->NextLevelPtr[page] = new Level(depth+1, new map[size]());
        } else{
            currentPointer->NextLevelPtr[page] = new Level(depth+1, new Level *[size]());
        }
    }
    //RECURSIE CALL 
    pageInsert(currentPointer->NextLevelPtr[page], logicalAddress);
}

//USEFUL FUNCTION THAT ALLOWS US TO GIVE US A REFERENCE TO OUR FRAME GIVEN
//ANY LOGICAL ADDRESS
bool PageTable::pageLookup(uint32_t logical, uint32_t &frame) {
    Level *currentPointer = rootNodeptr;
   for(int i =0; i < levelCountTable; i++){
       //WEVE FOUND OUR CURRENT LOGICAL ADDRESS IN OUR PAGETABLE
       if(currentPointer == nullptr){return false;}
       uint32_t currentPG = logicalToPage(logical, bitmaskAry[i], shiftAry[i]);
    if(currentPointer->mapPointer){
        //IF WERE AT A MAP THEN ITS THEN END SO PASS OUR FRAME BY REFERENCE
        frame = currentPointer->mapPointer[currentPG].frame;
        return true;
    }
    //CHANGE OUR POINTER AND TELL COMPUTER WE DID NOT FIND OUR LOGICAL ADDRESS
    currentPointer = currentPointer->NextLevelPtr[currentPG];
   } return false;
}

//SHIFTER FUNCTION
uint32_t PageTable::logicalToPage(uint32_t logicalAddress, uint32_t mask, uint32_t shift) {
    return (logicalAddress & mask) >> shift;
}

void PageTable::logicalToPhysical(uint32_t logical, int offset){
    uint32_t frame;
    uint32_t offAddr = logical;
    uint32_t mask =  pow(2,32) - 1;
    int pageSize = pow(2, offset);
    //WHEN LOOKUP IS SUCCESSFUL DO OUR L2P CALCULATION
    if(pageLookup(logical, frame)){
        
        for(int i = 0; i < levelCountTable; i++){
            mask -= bitmaskAry[i];
        }
        
        offAddr = offAddr & mask;
        //FRAME * 2^OFFSET + OFFSET = OUR PHYSICAL ADDRESS
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
        //WHEN PAGELOOKUP IS SUCCESS THEN GRAB OUR LEVELS FOR EASIER PRINTING
        for(int i =0; i < levelCountTable ; i++){
            pageArray[i] = logicalToPage(logical, bitmaskAry[i], shiftAry[i]);
        }
        report_pagemap(logical, levelCountTable, pageArray, tmpFrame);
    }
}