#include <iostream>
#include <vector>
#include <random>

#include "allocator/list_allocator.hpp"
#include "allocator/buddy_allocator.hpp"

static const size_t MEMORY_SIZE = 1024;
static const int OPERATIONS = 1000;
static const int RUNS = 10;
static const size_t MAX_ALLOC_SIZE = 64;

int main() {
    std::mt19937 rng(42);

    auto run_list = [&](const std::string& name, FitStrategy s) {
        double frag = 0, util = 0, fail = 0;

        for (int i = 0; i < RUNS; i++) {
            ListAllocator alloc(MEMORY_SIZE, s);
            std::vector<int> active;

            for (int j = 0; j < OPERATIONS; j++) {
                if (active.empty() || (rng() % 100) < 70) {
                    int id = alloc.malloc((rng() % MAX_ALLOC_SIZE) + 1);
                    if (id >= 0) active.push_back(id);
                } else {
                    int idx = rng() % active.size();
                    alloc.free(active[idx]);
                    active.erase(active.begin() + idx);
                }
            }

            frag += alloc.compute_external_fragmentation();
            util += (double)alloc.get_used_memory() / MEMORY_SIZE * 100.0;
            fail += alloc.get_total_requests() == 0 ? 0.0 :
                    (double)alloc.get_failed_requests() /
                    alloc.get_total_requests() * 100.0;
        }

        std::cout << "Strategy: " << name << "\n";
        std::cout << "Avg External Fragmentation: " << frag / RUNS << "%\n";
        std::cout << "Avg Utilization: " << util / RUNS << "%\n";
        std::cout << "Avg Failure Rate: " << fail / RUNS << "%\n\n";
    };

    auto run_buddy = [&]() {
        double frag = 0, util = 0, fail = 0, internal = 0;

        for (int i = 0; i < RUNS; i++) {
            BuddyAllocator alloc(MEMORY_SIZE);
            std::vector<int> active;

            for (int j = 0; j < OPERATIONS; j++) {
                if (active.empty() || (rng() % 100) < 70) {
                    int id = alloc.malloc((rng() % MAX_ALLOC_SIZE) + 1);
                    if (id >= 0) active.push_back(id);
                } else {
                    int idx = rng() % active.size();
                    alloc.free(active[idx]);
                    active.erase(active.begin() + idx);
                }
            }

            frag += alloc.external_fragmentation();
            util += alloc.utilization();
            fail += alloc.failure_rate();
            internal += alloc.internal_fragmentation();
        }

        std::cout << "Strategy: Buddy\n";
        std::cout << "Avg External Fragmentation: " << frag / RUNS << "%\n";
        std::cout << "Avg Internal Fragmentation: " << internal / RUNS << "%\n";
        std::cout << "Avg Utilization: " << util / RUNS << "%\n";
        std::cout << "Avg Failure Rate: " << fail / RUNS << "%\n\n";
    };

    run_list("First Fit", FitStrategy::FirstFit);
    run_list("Best Fit", FitStrategy::BestFit);
    run_list("Worst Fit", FitStrategy::WorstFit);
    run_buddy();

    return 0;
}
