#pragma once
#include<bits/stdc++.h>
#include <cstddef>
#include <unordered_map>
#include <list>
#include <string>

enum class CachePolicy {
    FIFO,
    LRU,
    LFU
};

struct CacheBlock {
    size_t block_id;
    size_t freq;
    size_t last_used;
};

class CacheLevel {
private:
    size_t capacity;
    size_t hit_time;
    CachePolicy policy;

    size_t time_counter;

    std::list<size_t> order;
    std::unordered_map<size_t, CacheBlock> blocks;

public:
    CacheLevel(size_t cap, size_t hit, CachePolicy pol);

    bool access(size_t block_id);

    void insert(size_t block_id);

    void dump(const std::string& name) const;

    size_t get_hit_time() const;
};
