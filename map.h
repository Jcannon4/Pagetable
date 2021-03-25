#ifndef TABLE_MAP_H
#define TABLE_MAP_H

struct map {
    //KEEP TRACK OF FRAME AND IF IT IS THE LEAF (LAST IN LEVEL)
    uint32_t frame=0;
    bool isLeaf = false;
    ~map() = default;
    map *pageLookup(PageTable *ptable, unsigned int logical);
};

#endif