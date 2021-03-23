#ifndef LEVEL_H
#define LEVEL_h

#include "page_table.h"
class PageTable;
class Level {
    private : 
    int depth;
    bool leaf;
    PageTable *PageTablePtr;
    Level **NextLevelPtr;

    public :
        Level(int depth, bool isLeaf, PageTable *PageTablePtr);
};

#endif