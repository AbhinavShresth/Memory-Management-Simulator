#pragma once
#include <cstddef>

class Allocator {
public:
    virtual int malloc(size_t size) = 0;
    virtual void free(int id) = 0;

    virtual void dump() const = 0;
    virtual void stats() const = 0;

    virtual ~Allocator() = default;
};
