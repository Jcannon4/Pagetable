#include <cstdint>
#include <cstdio>
#include <vector>

#ifndef PAGETABLE_H
#define PAGETABLE_H

using namespace std;

class PageTable {
    public:
    PageTable(int levels,vector<int> levelsAry);
    uint32_t *bitmaskAry;
    uint32_t *shiftAry;
    uint32_t *entrycount;

    unsigned int levelCount;
    private :
    //pageInsert(int val, int val2);
    int *rootNodeptr;
};


#endif