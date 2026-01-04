#pragma once

#include "allocator.hpp"
#include "allocator_stats.hpp"

#include <vector>
#include <list>
#include <unordered_map>

class BuddyAllocator : public Allocator {
private:
    size_t total_memory;
    size_t max_order;

    std::vector<std::list<size_t>> free_lists;

    struct AllocInfo {
        size_t addr;
        size_t order;
        size_t requested;
    };

    std::unordered_map<int, AllocInfo> allocated;
    int next_id;

    AllocatorStats stats_data;

    size_t size_to_order(size_t size) const;
    size_t buddy_of(size_t addr, size_t order) const;

public:
    BuddyAllocator(size_t memory_size);
    ~BuddyAllocator() override = default;

    int malloc(size_t size) override;
    void free(int id) override;

    void dump() const override;
    void stats() const override;

    double external_fragmentation() const;
    double internal_fragmentation() const;
    double utilization() const;
    double failure_rate() const;
};
