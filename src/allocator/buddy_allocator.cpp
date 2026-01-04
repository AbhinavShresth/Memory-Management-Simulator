#include "allocator/buddy_allocator.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

static bool is_power_of_two(size_t x) {
    return x && !(x & (x - 1));
}

BuddyAllocator::BuddyAllocator(size_t memory_size)
    : total_memory(memory_size), next_id(1) {

    if (!is_power_of_two(memory_size)) {
        throw std::runtime_error("Buddy allocator requires power-of-two memory");
    }

    max_order = static_cast<size_t>(std::log2(memory_size));
    free_lists.resize(max_order + 1);

    free_lists[max_order].push_back(0);

    stats_data.total_memory = memory_size;
}

size_t BuddyAllocator::size_to_order(size_t size) const {
    size_t order = 0;
    size_t block = 1;
    while (block < size) {
        block <<= 1;
        order++;
    }
    return order;
}

size_t BuddyAllocator::buddy_of(size_t addr, size_t order) const {
    return addr ^ (1ULL << order);
}

int BuddyAllocator::malloc(size_t size) {
    stats_data.total_alloc_requests++;

    if (size == 0)
        return -1;

    size_t order = size_to_order(size);
    size_t curr = order;

    while (curr <= max_order && free_lists[curr].empty())
        curr++;

    if (curr > max_order) {
        stats_data.failed_alloc_requests++;
        return -1;
    }

    size_t addr = free_lists[curr].front();
    free_lists[curr].pop_front();

    while (curr > order) {
        curr--;
        size_t buddy = addr + (1ULL << curr);
        free_lists[curr].push_back(buddy);
    }

    allocated[next_id] = {addr, order, size};

    size_t actual_size = (1ULL << order);
    stats_data.used_memory += actual_size;
    stats_data.internal_fragmentation_bytes += (actual_size - size);

    return next_id++;
}

void BuddyAllocator::free(int id) {
    auto it = allocated.find(id);
    if (it == allocated.end())
        return;

    size_t addr = it->second.addr;
    size_t order = it->second.order;
    size_t req   = it->second.requested;

    stats_data.used_memory -= (1ULL << order);
    stats_data.internal_fragmentation_bytes -= ((1ULL << order) - req);

    allocated.erase(it);

    while (order < max_order) {
        size_t buddy = buddy_of(addr, order);
        auto& list = free_lists[order];

        auto pos = std::find(list.begin(), list.end(), buddy);
        if (pos == list.end())
            break;

        list.erase(pos);
        addr = std::min(addr, buddy);
        order++;
    }

    free_lists[order].push_back(addr);
}

void BuddyAllocator::dump() const {
    std::cout << "Buddy Free Lists:\n";
    for (size_t i = 0; i <= max_order; i++) {
        if (!free_lists[i].empty()) {
            std::cout << "  size " << (1ULL << i) << ": ";
            for (auto addr : free_lists[i])
                std::cout << addr << " ";
            std::cout << "\n";
        }
    }

    std::cout << "Allocated Blocks:\n";
    for (const auto& it : allocated) {
        int id = it.first;
        const auto& b = it.second;

        std::cout << "  id=" << id
                  << " addr=" << b.addr
                  << " size=" << (1ULL << b.order)
                  << " requested=" << b.requested
                  << "\n";
    }
}

double BuddyAllocator::external_fragmentation() const {
    size_t total_free = 0;
    size_t largest = 0;

    for (size_t i = 0; i <= max_order; i++) {
        size_t block_size = (1ULL << i);
        size_t count = free_lists[i].size();

        if (count > 0) {
            total_free += block_size * count;
            largest = std::max(largest, block_size);
        }
    }

    if (total_free == 0)
        return 0.0;

    return (double)(total_free - largest) / total_free * 100.0;
}

double BuddyAllocator::internal_fragmentation() const {
    if (stats_data.used_memory == 0)
        return 0.0;

    return (double)stats_data.internal_fragmentation_bytes /
           stats_data.used_memory * 100.0;
}

double BuddyAllocator::utilization() const {
    return (double)stats_data.used_memory /
           stats_data.total_memory * 100.0;
}

double BuddyAllocator::failure_rate() const {
    if (stats_data.total_alloc_requests == 0)
        return 0.0;

    return (double)stats_data.failed_alloc_requests /
           stats_data.total_alloc_requests * 100.0;
}

void BuddyAllocator::stats() const {
    std::cout << "Total memory: " << stats_data.total_memory << "\n";
    std::cout << "Used memory: " << stats_data.used_memory << "\n";
    std::cout << "Memory utilization: " << utilization() << "%\n";
    std::cout << "External fragmentation: "
              << external_fragmentation() << "%\n";
    std::cout << "Internal fragmentation: "
              << internal_fragmentation() << "%\n";
    std::cout << "Allocation failure rate: "
              << failure_rate() << "%\n";
}
