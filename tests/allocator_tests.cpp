#include <iostream>
#include <cassert>
#include "allocator/list_allocator.hpp"

void test_first_fit_basic() {
    ListAllocator alloc(64, FitStrategy::FirstFit);

    int a = alloc.malloc(16);
    int b = alloc.malloc(16);
    assert(a > 0 && b > 0);

    alloc.free(a);

    int c = alloc.malloc(8);
    assert(c > 0);
}

void test_best_fit() {
    ListAllocator alloc(64, FitStrategy::BestFit);

    int a = alloc.malloc(8);
    int b = alloc.malloc(16);
    int c = alloc.malloc(8);

    alloc.free(b);
    alloc.free(a);

    int d = alloc.malloc(7);
    assert(d > 0);
}

void test_worst_fit() {
    ListAllocator alloc(64, FitStrategy::WorstFit);

    int a = alloc.malloc(8);
    int b = alloc.malloc(8);
    int c = alloc.malloc(8);

    alloc.free(b);
    alloc.free(a);

    int d = alloc.malloc(4);
    assert(d > 0);
}

void test_coalescing() {
    ListAllocator alloc(64, FitStrategy::FirstFit);

    int a = alloc.malloc(16);
    int b = alloc.malloc(16);

    alloc.free(a);
    alloc.free(b);

    int c = alloc.malloc(32);
    assert(c > 0);
}

int main() {
    test_first_fit_basic();
    test_best_fit();
    test_worst_fit();
    test_coalescing();

    std::cout << "[PASS] All allocator tests\n";
    return 0;
}