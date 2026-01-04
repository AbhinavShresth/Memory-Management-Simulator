#pragma once
#include <cstddef>

struct AllocatorStats {
    size_t total_memory = 0;

    size_t used_memory = 0;
    size_t free_memory = 0;

    size_t total_alloc_requests = 0;
    size_t failed_alloc_requests = 0;

    size_t internal_fragmentation_bytes = 0;
};
