#ifndef LEVEL_H
#define LEVEL_H

#include "page_table.h"
#include "map.h"
class PageTable;
class Level {
    public: 
    int depth;
    //bool leaf;
    PageTable *PageTablePtr;
    Level **NextLevelPtr = nullptr;
    map *mapPtr = nullptr;
   
    Level(int depth, bool leaf, PageTable *);

    explicit Level(int depth, map *mapPtr);

    
    explicit Level(int depth, Level **nextLevelPtr);
    ~Level();
    
};

#endif