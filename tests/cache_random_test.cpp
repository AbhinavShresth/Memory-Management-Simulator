#include <iostream>
#include <random>
#include "cache/cache_simulator.hpp"

static const int OPERATIONS = 1000;
static const int RUNS = 10;
static const size_t MAX_ADDRESS = 1024;

int main() {
    std::mt19937 rng(42);

    auto run_policy = [&](const std::string& name, CachePolicy policy) {
        double overall_hit_rate = 0, avg_access_time = 0;
        double l1_hit_rate = 0, l2_hit_rate = 0, l3_hit_rate = 0;
        double memory_accesses = 0;

        for (int i = 0; i < RUNS; i++) {
            CacheSimulator cache(policy);
            
            for (int j = 0; j < OPERATIONS; j++) {
                size_t address = rng() % MAX_ADDRESS;
                cache.access(address);
            }

            overall_hit_rate += cache.get_overall_hit_rate();
            avg_access_time += cache.get_avg_access_time();
            l1_hit_rate += cache.get_l1_hit_rate();
            l2_hit_rate += cache.get_l2_hit_rate();
            l3_hit_rate += cache.get_l3_hit_rate();
            memory_accesses += cache.get_memory_accesses();
        }

        std::cout << "Policy: " << name << "\n";
        std::cout << "Avg Overall Hit Rate: " << overall_hit_rate / RUNS << "%\n";
        std::cout << "Avg Access Time: " << avg_access_time / RUNS << " cycles\n";
        std::cout << "Avg L1 Hit Rate: " << l1_hit_rate / RUNS << "%\n";
        std::cout << "Avg L2 Hit Rate: " << l2_hit_rate / RUNS << "%\n";
        std::cout << "Avg L3 Hit Rate: " << l3_hit_rate / RUNS << "%\n";
        std::cout << "Avg Memory Accesses: " << memory_accesses / RUNS << "\n\n";
    };

    run_policy("FIFO", CachePolicy::FIFO);
    run_policy("LRU", CachePolicy::LRU);
    run_policy("LFU", CachePolicy::LFU);

    return 0;
}

