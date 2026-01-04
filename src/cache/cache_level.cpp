#include "cache/cache_level.hpp"
#include <iostream>
#include <algorithm>

CacheLevel::CacheLevel(size_t cap, size_t hit, CachePolicy pol)
    : capacity(cap),
      hit_time(hit),
      policy(pol),
      time_counter(0) {}

bool CacheLevel::access(size_t block_id) {
    time_counter++;

    auto it = blocks.find(block_id);
    if (it == blocks.end()) {
        return false;
    }

    CacheBlock& blk = it->second;
    blk.last_used = time_counter;
    blk.freq++;

    if (policy == CachePolicy::LRU) {
        order.remove(block_id);
        order.push_back(block_id);
    }

    return true;
}

void CacheLevel::insert(size_t block_id) {
    time_counter++;

    if (blocks.count(block_id)) {
        blocks[block_id].last_used = time_counter;
        blocks[block_id].freq++;
        return;
    }

    if (blocks.size() >= capacity) {
        size_t victim;

        if (policy == CachePolicy::FIFO || policy == CachePolicy::LRU) {
            victim = order.front();
            order.pop_front();
        } else {
            victim = std::min_element(
                blocks.begin(), blocks.end(),
                [](const auto& a, const auto& b) {
                    if (a.second.freq == b.second.freq)
                        return a.second.last_used < b.second.last_used;
                    return a.second.freq < b.second.freq;
                }
            )->first;

            order.remove(victim);
        }

        blocks.erase(victim);
    }

    CacheBlock blk;
    blk.block_id = block_id;
    blk.freq = 1;
    blk.last_used = time_counter;

    blocks[block_id] = blk;
    order.push_back(block_id);
}

void CacheLevel::dump(const std::string& name) const {
    std::cout << name << " Cache:\n";

    if (blocks.empty()) {
        std::cout << "  [empty]\n";
        return;
    }

    for (const auto& id : order) {
        const auto& blk = blocks.at(id);
        std::cout << "  block=" << blk.block_id
                  << " freq=" << blk.freq
                  << " last_used=" << blk.last_used
                  << "\n";
    }
}

size_t CacheLevel::get_hit_time() const {
    return hit_time;
}
