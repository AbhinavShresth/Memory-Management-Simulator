#pragma once
#include <cstddef>

struct Block {
    size_t start;
    size_t size;
    bool free;

    int id;

    Block* prev;
    Block* next;
};
