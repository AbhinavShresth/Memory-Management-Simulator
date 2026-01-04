#pragma once

#include <cstddef>
#include <string>
#include <fstream>

#include "cache/cache_level.hpp"

class CacheSimulator {
private:
    CacheLevel l1;
    CacheLevel l2;
    CacheLevel l3;

    static constexpr size_t BLOCK_SIZE = 16;
    static constexpr size_t MEMORY_PENALTY = 100;

    bool logs_enabled;
    bool filelog_enabled;
    std::ofstream logfile;

    size_t total_accesses;

    size_t l1_hits, l1_misses;
    size_t l2_hits, l2_misses;
    size_t l3_hits, l3_misses;
    size_t memory_accesses;

    size_t total_cycles;

    size_t address_to_block(size_t address) const;
    void log(const std::string& msg);

public:
    CacheSimulator(CachePolicy policy);
    ~CacheSimulator();

    void enable_logs();
    void disable_logs();

    void enable_filelog();
    void disable_filelog();

    void access(size_t address);

    void dump() const;
    void stats() const;

    double get_overall_hit_rate() const;
    double get_avg_access_time() const;
    double get_l1_hit_rate() const;
    double get_l2_hit_rate() const;
    double get_l3_hit_rate() const;
    size_t get_memory_accesses() const;
    size_t get_total_accesses() const;
};
