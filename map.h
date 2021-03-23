#ifndef MAP_H
#define MAP_H

#include <cstdint>

struct map {
    //int size ;

    uint32_t frame =0;
    bool isValid = false;

    ~map() = default;
};

#endif //MAP_H