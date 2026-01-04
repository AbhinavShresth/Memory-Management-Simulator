#include "allocator/list_allocator.hpp"
#include <iostream>
#include <iomanip>

ListAllocator::ListAllocator(size_t memory_size, FitStrategy strat)
    : head(nullptr),
      total_memory(memory_size),
      next_id(1),
      strategy(strat) {

    head = new Block{
        0,
        memory_size,
        true,
        -1,
        nullptr,
        nullptr
    };

    stats_data.total_memory = memory_size;
}

ListAllocator::~ListAllocator() {
    Block* curr = head;
    while (curr) {
        Block* next = curr->next;
        delete curr;
        curr = next;
    }
}

Block* ListAllocator::find_free_block(size_t size) {
    Block* curr = head;
    Block* best = nullptr;

    while (curr) {
        if (curr->free && curr->size >= size) {
            if (strategy == FitStrategy::FirstFit)
                return curr;

            if (strategy == FitStrategy::BestFit) {
                if (!best || curr->size < best->size)
                    best = curr;
            }

            if (strategy == FitStrategy::WorstFit) {
                if (!best || curr->size > best->size)
                    best = curr;
            }
        }
        curr = curr->next;
    }
    return best;
}

void ListAllocator::split_block(Block* block, size_t size) {
    if (block->size == size)
        return;

    Block* remainder = new Block{
        block->start + size,
        block->size - size,
        true,
        -1,
        block,
        block->next
    };

    if (block->next)
        block->next->prev = remainder;

    block->next = remainder;
    block->size = size;
}

void ListAllocator::coalesce(Block* block) {
    if (block->next && block->next->free) {
        Block* next = block->next;
        block->size += next->size;
        block->next = next->next;
        if (next->next)
            next->next->prev = block;
        delete next;
    }

    if (block->prev && block->prev->free) {
        Block* prev = block->prev;
        prev->size += block->size;
        prev->next = block->next;
        if (block->next)
            block->next->prev = prev;
        delete block;
    }
}

Block* ListAllocator::find_block_by_id(int id) {
    Block* curr = head;
    while (curr) {
        if (!curr->free && curr->id == id)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

int ListAllocator::malloc(size_t size) {
    stats_data.total_alloc_requests++;

    if (size == 0)
        return -1;

    Block* block = find_free_block(size);
    if (!block) {
        stats_data.failed_alloc_requests++;
        return -1;
    }

    split_block(block, size);

    block->free = false;
    block->id = next_id++;

    stats_data.used_memory += block->size;
    stats_data.free_memory = total_memory - stats_data.used_memory;

    return block->id;
}

void ListAllocator::free(int id) {
    Block* block = find_block_by_id(id);
    if (!block)
        return;

    block->free = true;
    block->id = -1;

    stats_data.used_memory -= block->size;
    stats_data.free_memory = total_memory - stats_data.used_memory;

    coalesce(block);
}

void ListAllocator::dump() const {
    Block* curr = head;
    while (curr) {
        size_t end = curr->start + curr->size - 1;
        std::cout << "[0x"
                  << std::hex << std::setw(4) << std::setfill('0') << curr->start
                  << " - 0x"
                  << std::hex << std::setw(4) << end
                  << "] ";

        if (curr->free)
            std::cout << "FREE\n";
        else
            std::cout << "USED (id=" << std::dec << curr->id << ")\n";

        curr = curr->next;
    }
}

void ListAllocator::stats() const {
    std::cout << std::dec;

    double utilization =
        (double)stats_data.used_memory /
        (double)stats_data.total_memory * 100.0;

    size_t total_free = 0;
    size_t largest_free = 0;

    Block* curr = head;
    while (curr) {
        if (curr->free) {
            total_free += curr->size;
            if (curr->size > largest_free)
                largest_free = curr->size;
        }
        curr = curr->next;
    }

    double external_fragmentation = compute_external_fragmentation();

    std::cout << "Total memory: " << stats_data.total_memory << "\n";
    std::cout << "Used memory: " << stats_data.used_memory << "\n";
    std::cout << "Free memory: " << total_free << "\n";
    std::cout << "Memory utilization: " << utilization << "%\n";
    std::cout << "External fragmentation: "
              << external_fragmentation << "%\n";

    if (stats_data.total_alloc_requests > 0) {
        double fail_rate =
            (double)stats_data.failed_alloc_requests /
            (double)stats_data.total_alloc_requests * 100.0;

        std::cout << "Allocation failure rate: "
                  << fail_rate << "%\n";
    }
}

double ListAllocator::compute_external_fragmentation() const {
    size_t total_free = 0;
    size_t largest_free = 0;

    Block* curr = head;
    while (curr) {
        if (curr->free) {
            total_free += curr->size;
            if (curr->size > largest_free)
                largest_free = curr->size;
        }
        curr = curr->next;
    }

    if (total_free == 0) return 0.0;

    return (double)(total_free - largest_free) /
           (double)total_free * 100.0;
}
