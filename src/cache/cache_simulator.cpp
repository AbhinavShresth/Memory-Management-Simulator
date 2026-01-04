#include "cache/cache_simulator.hpp"
#include <iostream>
#include <sstream>

CacheSimulator::CacheSimulator(CachePolicy policy)
    : l1(4, 1, policy),
      l2(8, 5, policy),
      l3(16, 20, policy),
      logs_enabled(false),
      filelog_enabled(false),
      total_accesses(0),
      l1_hits(0), l1_misses(0),
      l2_hits(0), l2_misses(0),
      l3_hits(0), l3_misses(0),
      memory_accesses(0),
      total_cycles(0) {}

CacheSimulator::~CacheSimulator() {
    if (logfile.is_open())
        logfile.close();
}

void CacheSimulator::enable_logs() { logs_enabled = true; }
void CacheSimulator::disable_logs() { logs_enabled = false; }

void CacheSimulator::enable_filelog() {
    if (!filelog_enabled) {
        logfile.open("cache_log.txt");
        filelog_enabled = true;
    }
}

void CacheSimulator::disable_filelog() {
    if (filelog_enabled) {
        logfile.close();
        filelog_enabled = false;
    }
}

void CacheSimulator::log(const std::string& msg) {
    if (logs_enabled) std::cout << msg << "\n";
    if (filelog_enabled && logfile.is_open()) logfile << msg << "\n";
}

size_t CacheSimulator::address_to_block(size_t address) const {
    return address / BLOCK_SIZE;
}

void CacheSimulator::access(size_t address) {
    total_accesses++;
    size_t block = address_to_block(address);
    size_t access_cycles = 0;

    std::ostringstream oss;
    oss << "ACCESS " << address << " (block " << block << "):";
    log(oss.str());

    if (l1.access(block)) {
        l1_hits++;
        access_cycles = l1.get_hit_time();
        total_cycles += access_cycles;
        log("  L1 HIT");
        log("  Access time: " + std::to_string(access_cycles) + " cycles");
        return;
    }
    l1_misses++;
    log("  L1 MISS");

    if (l2.access(block)) {
        l2_hits++;
        access_cycles = l1.get_hit_time() + l2.get_hit_time();
        total_cycles += access_cycles;
        log("  L2 HIT");
        l1.insert(block);
        log("  Loaded into L1");
        log("  Access time: " + std::to_string(access_cycles) + " cycles");
        return;
    }
    l2_misses++;
    log("  L2 MISS");

    if (l3.access(block)) {
        l3_hits++;
        access_cycles =
            l1.get_hit_time() + l2.get_hit_time() + l3.get_hit_time();
        total_cycles += access_cycles;
        log("  L3 HIT");
        l2.insert(block);
        l1.insert(block);
        log("  Loaded into L2");
        log("  Loaded into L1");
        log("  Access time: " + std::to_string(access_cycles) + " cycles");
        return;
    }
    l3_misses++;
    log("  L3 MISS");

    memory_accesses++;
    access_cycles =
        l1.get_hit_time() +
        l2.get_hit_time() +
        l3.get_hit_time() +
        MEMORY_PENALTY;

    total_cycles += access_cycles;

    log("  MAIN MEMORY ACCESS");

    l3.insert(block);
    l2.insert(block);
    l1.insert(block);

    log("  Loaded into L3");
    log("  Loaded into L2");
    log("  Loaded into L1");
    log("  Access time: " + std::to_string(access_cycles) + " cycles");
}

void CacheSimulator::dump() const {
    l1.dump("L1");
    l2.dump("L2");
    l3.dump("L3");
}

void CacheSimulator::stats() const {
    std::cout << "Total accesses: " << total_accesses << "\n\n";

    std::cout << "L1 hits: " << l1_hits
              << "  misses: " << l1_misses
              << "  hit rate: "
              << (total_accesses ? (double)l1_hits / total_accesses * 100 : 0)
              << "%\n";

    std::cout << "L2 hits: " << l2_hits
              << "  misses: " << l2_misses
              << "  hit rate: "
              << (total_accesses ? (double)l2_hits / total_accesses * 100 : 0)
              << "%\n";

    std::cout << "L3 hits: " << l3_hits
              << "  misses: " << l3_misses
              << "  hit rate: "
              << (total_accesses ? (double)l3_hits / total_accesses * 100 : 0)
              << "%\n";

    std::cout << "Memory accesses: " << memory_accesses << "\n\n";

    double overall_hit_rate =
        total_accesses == 0 ? 0.0 :
        (double)(l1_hits + l2_hits + l3_hits) /
        total_accesses * 100.0;

    double avg_time =
        total_accesses == 0 ? 0.0 :
        (double)total_cycles / total_accesses;

    std::cout << "Overall hit rate: " << overall_hit_rate << "%\n";
    std::cout << "Average access time: " << avg_time << " cycles\n\n";

    std::cout << "Miss penalties:\n";
    std::cout << "  L1 -> L2: " << l2.get_hit_time() << " cycles\n";
    std::cout << "  L2 -> L3: " << l3.get_hit_time() << " cycles\n";
    std::cout << "  L3 -> Memory: " << MEMORY_PENALTY << " cycles\n";
}

double CacheSimulator::get_overall_hit_rate() const {
    return total_accesses == 0 ? 0.0 :
           (double)(l1_hits + l2_hits + l3_hits) / total_accesses * 100.0;
}

double CacheSimulator::get_avg_access_time() const {
    return total_accesses == 0 ? 0.0 :
           (double)total_cycles / total_accesses;
}

double CacheSimulator::get_l1_hit_rate() const {
    return total_accesses == 0 ? 0.0 :
           (double)l1_hits / total_accesses * 100.0;
}

double CacheSimulator::get_l2_hit_rate() const {
    return total_accesses == 0 ? 0.0 :
           (double)l2_hits / total_accesses * 100.0;
}

double CacheSimulator::get_l3_hit_rate() const {
    return total_accesses == 0 ? 0.0 :
           (double)l3_hits / total_accesses * 100.0;
}

size_t CacheSimulator::get_memory_accesses() const {
    return memory_accesses;
}

size_t CacheSimulator::get_total_accesses() const {
    return total_accesses;
}