
#include "level.h"
#include "page_table.h"



Level::Level(int depth, bool isLeaf, PageTable *pagetablepointer){
    Level::depth = depth;
    Level::leaf = isLeaf;
    Level::PageTablePtr = pagetablepointer;
    Level::mapPointer = nullptr;
    if (leaf){
        Level::NextLevelPtr = nullptr;
    } else {
        int currentLevelSize = Level::PageTablePtr->entrycount[depth];
        Level::NextLevelPtr = new Level *[currentLevelSize];
        for (int i = 0; i < currentLevelSize; i++){
            NextLevelPtr[i] = nullptr;
        }
    }
}
Level::Level(int depth, Level **nextLevelPtr) {
    this->depth = depth;
    this->NextLevelPtr = nextLevelPtr;
}
Level::Level(int depth, map *mapPointer) {
    this->depth = depth;
    this->mapPointer = mapPointer;
}

Level::~Level() = default;