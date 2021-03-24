#ifndef TABLE_LEVEL_H
#define TABLE_LEVEL_H

#include "page_table.h"
#include "map.h"
class PageTable;
class Level {
    public:

    PageTable *PageTablePtr;
    Level **NextLevelPtr = nullptr;
    map *mapPointer = nullptr;
    int depth;
    bool leaf;
    Level(int depth, bool leaf, PageTable *);

    Level(int depth, map *mapPointer);

    Level(int depth, Level **nextLevelPtr);
    ~Level();
    
};

#endif