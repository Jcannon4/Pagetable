
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
//CREATE A NEW LEVEL CLASS, WE NEED TO DEFINE ITS DEPTH AS WELL AS THE NEXTLEVEL
//WITH A POINTER
Level::Level(int depth, Level **nextLevelPtr) {
    this->depth = depth;
    this->NextLevelPtr = nextLevelPtr;
}
//CREATE AN INSTANCE OF THE MAP CLASS, ITS A LEVEL BUT WITH A MAP POINTER INSTEAD
Level::Level(int depth, map *mapPointer) {
    this->depth = depth;
    this->mapPointer = mapPointer;
}
//DEFAULT CONSTRUCTOR
Level::~Level() = default;