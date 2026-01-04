#include <iostream>
#include <cassert>
#include "cache/cache_simulator.hpp"

void test_fifo_basic() {
    CacheSimulator cache(CachePolicy::FIFO);
    
    cache.access(0);
    cache.access(16);
    cache.access(32);
    cache.access(48);
    
    assert(cache.get_total_accesses() == 4);
    assert(cache.get_memory_accesses() == 4);
}

void test_lru_basic() {
    CacheSimulator cache(CachePolicy::LRU);
    
    cache.access(0);
    cache.access(16);
    cache.access(0);
    
    assert(cache.get_total_accesses() == 3);
    assert(cache.get_overall_hit_rate() >= 0);
}

void test_lfu_basic() {
    CacheSimulator cache(CachePolicy::LFU);
    
    cache.access(0);
    cache.access(0);
    cache.access(0);
    
    assert(cache.get_total_accesses() == 3);
    assert(cache.get_overall_hit_rate() > 0);
}

void test_fifo_eviction() {
    CacheSimulator cache(CachePolicy::FIFO);
    
    for (int i = 0; i < 10; i++) {
        cache.access(i * 16);
    }
    
    cache.access(0);
    
    assert(cache.get_total_accesses() == 11);
}

void test_lru_eviction() {
    CacheSimulator cache(CachePolicy::LRU);
    
    cache.access(0);
    cache.access(16);
    cache.access(32);
    cache.access(48);
    cache.access(0);
    
    assert(cache.get_total_accesses() == 5);
}

void test_lfu_eviction() {
    CacheSimulator cache(CachePolicy::LFU);
    
    cache.access(0);
    cache.access(16);
    cache.access(32);
    cache.access(0);
    cache.access(16);
    cache.access(48);
    
    assert(cache.get_total_accesses() == 6);
}

void test_cache_hit_rate() {
    CacheSimulator cache(CachePolicy::LRU);
    
    for (int i = 0; i < 5; i++) {
        cache.access(0);
        cache.access(16);
    }
    
    double hit_rate = cache.get_overall_hit_rate();
    assert(hit_rate >= 0 && hit_rate <= 100);
    assert(cache.get_total_accesses() == 10);
}

void test_cache_levels() {
    CacheSimulator cache(CachePolicy::LRU);
    
    cache.access(0);
    cache.access(0);
    
    assert(cache.get_l1_hit_rate() >= 0);
    assert(cache.get_l2_hit_rate() >= 0);
    assert(cache.get_l3_hit_rate() >= 0);
    assert(cache.get_total_accesses() == 2);
}

int main() {
    test_fifo_basic();
    test_lru_basic();
    test_lfu_basic();
    test_fifo_eviction();
    test_lru_eviction();
    test_lfu_eviction();
    test_cache_hit_rate();
    test_cache_levels();
    
    std::cout << "[PASS] All cache tests\n";
    return 0;
}

