#ifndef MAP_H
#define MAP_H

#include <cstdint>

struct map {
    //int size ;

    uint32_t frame=0;
    bool isValid = false;
    ~map() = default;
    map *pageLookup(PageTable *ptable, unsigned int logical);
};

#endif //MAP_H