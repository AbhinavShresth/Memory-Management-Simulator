#pragma once

#include "allocator.hpp"
#include "block.hpp"
#include "allocator_stats.hpp"

enum class FitStrategy {
    FirstFit,
    BestFit,
    WorstFit
};

class ListAllocator : public Allocator {
private:
    Block* head;
    size_t total_memory;
    int next_id;
    FitStrategy strategy;

    AllocatorStats stats_data;

    Block* find_free_block(size_t size);
    void split_block(Block* block, size_t size);
    void coalesce(Block* block);
    Block* find_block_by_id(int id);

public:
    ListAllocator(size_t memory_size, FitStrategy strat);
    ~ListAllocator();

    int malloc(size_t size) override;
    void free(int id) override;

    void dump() const override;
    void stats() const override;

    size_t get_used_memory() const { return stats_data.used_memory; }
    size_t get_total_requests() const { return stats_data.total_alloc_requests; }
    size_t get_failed_requests() const { return stats_data.failed_alloc_requests; }

    double compute_external_fragmentation() const;
};
