#include <iostream>
#include <sstream>
#include <string>

#include "allocator/list_allocator.hpp"
#include "allocator/buddy_allocator.hpp"
#include "cache/cache_simulator.hpp"

FitStrategy parse_fit(const std::string& s) {
    if (s == "first") return FitStrategy::FirstFit;
    if (s == "best")  return FitStrategy::BestFit;
    if (s == "worst") return FitStrategy::WorstFit;
    return FitStrategy::FirstFit;
}

CachePolicy parse_cache_policy(const std::string& s) {
    if (s == "fifo") return CachePolicy::FIFO;
    if (s == "lru")  return CachePolicy::LRU;
    if (s == "lfu")  return CachePolicy::LFU;
    return CachePolicy::LRU;
}

int main() {
    std::cout << "Memory Simulator\n";

    Allocator* allocator = nullptr;
    size_t memory_size = 0;

    CacheSimulator* cache = nullptr;
    CachePolicy cache_policy = CachePolicy::LRU;

    std::string line;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            break;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "exit" || cmd == "quit") {
            break;
        }

        else if (cmd == "init") {
            std::string sub;
            ss >> sub;

            if (sub == "memory") {
                size_t size;
                ss >> size;

                delete allocator;
                allocator = nullptr;
                memory_size = size;

                std::cout << "Initialized memory of size " << size << "\n";
            }
            else if (sub == "cache") {
                delete cache;
                cache = new CacheSimulator(cache_policy);
                std::cout << "Cache initialized\n";
            }
            else {
                std::cout << "Invalid init command\n";
            }
        }

        else if (cmd == "set") {
            std::string sub, arg;
            ss >> sub >> arg;

            if (sub == "allocator") {
                if (memory_size == 0) {
                    std::cout << "Initialize memory first\n";
                    continue;
                }

                delete allocator;
                allocator = nullptr;

                if (arg == "first" || arg == "best" || arg == "worst") {
                    allocator = new ListAllocator(
                        memory_size,
                        parse_fit(arg)
                    );
                }
                else if (arg == "buddy") {
                    allocator = new BuddyAllocator(memory_size);
                }
                else {
                    std::cout << "Unknown allocator\n";
                    continue;
                }

                std::cout << "Allocator set to " << arg << "\n";
            }
            else if (sub == "policy") {
                cache_policy = parse_cache_policy(arg);
                delete cache;
                cache = new CacheSimulator(cache_policy);
                std::cout << "Cache policy set to " << arg << "\n";
            }
            else {
                std::cout << "Invalid set command\n";
            }
        }

        else if (cmd == "malloc") {
            if (!allocator) {
                std::cout << "Allocator not set\n";
                continue;
            }

            size_t size;
            ss >> size;

            int id = allocator->malloc(size);
            if (id < 0)
                std::cout << "Allocation failed\n";
            else
                std::cout << "Allocated block id=" << id << "\n";
        }

        else if (cmd == "free") {
            if (!allocator) {
                std::cout << "Allocator not set\n";
                continue;
            }

            int id;
            ss >> id;
            allocator->free(id);
            std::cout << "Block " << id << " freed\n";
        }

        else if (cmd == "dump") {
            std::string sub;
            ss >> sub;

            if (sub == "memory") {
                if (!allocator) {
                    std::cout << "Allocator not set\n";
                    continue;
                }
                allocator->dump();
            }
            else if (sub == "cache") {
                if (!cache) {
                    std::cout << "Cache not initialized\n";
                    continue;
                }
                cache->dump();
            }
            else {
                std::cout << "Invalid dump command\n";
            }
        }

        else if (cmd == "stats") {
            std::string sub;
            ss >> sub;

            if (sub == "memory") {
                if (!allocator) {
                    std::cout << "Allocator not set\n";
                    continue;
                }
                allocator->stats();
            }
            else if (sub == "cache") {
                if (!cache) {
                    std::cout << "Cache not initialized\n";
                    continue;
                }
                cache->stats();
            }
            else {
                std::cout << "Invalid stats command\n";
            }
        }

        else if (cmd == "access") {
            if (!cache) {
                std::cout << "Cache not initialized\n";
                continue;
            }

            size_t addr;
            ss >> addr;
            cache->access(addr);
        }

        else if (cmd == "enable") {
            if (!cache) {
                std::cout << "Cache not initialized\n";
                continue;
            }

            std::string what;
            ss >> what;

            if (what == "logs") {
                cache->enable_logs();
                std::cout << "Logs enabled\n";
            }
            else if (what == "filelog") {
                cache->enable_filelog();
                std::cout << "File logging enabled\n";
            }
            else {
                std::cout << "Unknown enable option\n";
            }
        }

        else if (cmd == "disable") {
            if (!cache) {
                std::cout << "Cache not initialized\n";
                continue;
            }

            std::string what;
            ss >> what;

            if (what == "logs") {
                cache->disable_logs();
                std::cout << "Logs disabled\n";
            }
            else if (what == "filelog") {
                cache->disable_filelog();
                std::cout << "File logging disabled\n";
            }
            else {
                std::cout << "Unknown disable option\n";
            }
        }

        else {
            std::cout << "Unknown command\n";
        }
    }

    delete allocator;
    delete cache;
    return 0;
}
