# Memory Management Simulator

A comprehensive C++ simulation framework for studying and analyzing memory management techniques, including dynamic memory allocators and multi-level cache systems with various replacement policies.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Project Structure](#project-structure)
- [Building the Project](#building-the-project)
- [Usage](#usage)
- [Testing](#testing)
- [Architecture](#architecture)
- [Performance Metrics](#performance-metrics)

## Overview

This project provides an interactive command-line simulator for exploring memory management concepts. It implements multiple memory allocation strategies and cache simulation with configurable policies, making it an excellent educational tool for understanding how operating systems and computer architectures manage memory.

## Features

### Memory Allocators

- **List Allocator** with three fit strategies:
  - First Fit
  - Best Fit
  - Worst Fit
- **Buddy Allocator** for power-of-two allocations
- Memory fragmentation analysis
- Real-time memory state visualization
- Allocation statistics tracking

### Cache Simulator

- **Multi-level cache hierarchy** (L1, L2, L3)
- **Multiple replacement policies**:
  - FIFO (First In, First Out)
  - LRU (Least Recently Used)
  - LFU (Least Frequently Used)
- Cache hit/miss tracking
- Performance metrics (hit rates, access times)
- Detailed logging capabilities
- File-based logging support

### Interactive CLI

- Command-line interface for real-time interaction
- Memory and cache state inspection
- Statistics and performance metrics
- Configurable allocator and cache policies

## Technologies Used

- **Language**: C++17
- **Compiler**: GCC (G++)
- **Build System**: Make
- **Standard Library**: STL containers (`std::list`, `std::unordered_map`)
- **Platform**: Cross-platform (Windows/Linux/macOS)

## Project Structure

```
Memory-2/
├── include/                    # Header files
│   ├── allocator/
│   │   ├── allocator.hpp       # Base allocator interface
│   │   ├── list_allocator.hpp  # List-based allocator
│   │   ├── buddy_allocator.hpp # Buddy system allocator
│   │   ├── block.hpp           # Memory block structure
│   │   └── allocator_stats.hpp # Statistics tracking
│   └── cache/
│       ├── cache_level.hpp     # Single cache level implementation
│       └── cache_simulator.hpp # Multi-level cache simulator
├── src/                        # Source files
│   ├── main.cpp                # CLI entry point
│   ├── allocator/
│   │   ├── list_allocator.cpp
│   │   └── buddy_allocator.cpp
│   └── cache/
│       ├── cache_level.cpp
│       └── cache_simulator.cpp
├── tests/                      # Test suites
│   ├── allocator_tests.cpp
│   ├── cache_tests.cpp
│   ├── random_test.cpp
│   └── cache_random_test.cpp
├── Images/                     # Documentation images
├── Makefile                    # Build configuration
├── Report.pdf                  # Project documentation
└── README.md                   # This file
```

## Building the Project

### Prerequisites

- GCC compiler with C++17 support
- Make utility

### Build Commands

```bash
# Build the main simulator
make

# Build and run allocator tests
make test

# Build and run cache tests
make cache_test

# Run random allocation tests
make random

# Run random cache tests
make cache_random

# Clean build artifacts
make clean
```

The main executable will be generated as `memsim`.

## Usage

### Starting the Simulator

```bash
./memsim
```

### Available Commands

#### Memory Management

```bash
# Initialize memory pool
init memory <size>

# Set allocator type
set allocator <type>    # type: first, best, worst, buddy

# Allocate memory block
malloc <size>

# Free memory block
free <id>

# Display memory state
dump memory

# Show allocation statistics
stats memory
```

#### Cache Simulation

```bash
# Initialize cache
init cache

# Set cache replacement policy
set policy <policy>      # policy: fifo, lru, lfu

# Simulate memory access
access <address>

# Display cache state
dump cache

# Show cache statistics
stats cache

# Enable/disable logging
enable logs
disable logs

# Enable/disable file logging
enable filelog
disable filelog
```

## Testing

The project includes comprehensive test suites:

- **Unit Tests**: Test individual components (allocators, cache levels)
- **Random Tests**: Stress testing with random allocation patterns
- **Cache Tests**: Validate cache behavior and policies

Run tests using the Makefile targets:

```bash
make test          # Allocator tests
make cache_test    # Cache tests
make random        # Random allocator tests
make cache_random  # Random cache tests
```

## Architecture

### Allocator Design

- **Interface-based**: Base `Allocator` class with virtual methods
- **Strategy Pattern**: Different fit strategies for list allocator
- **Block Management**: Linked list of memory blocks with coalescing
- **Statistics**: Track fragmentation, allocation success rates

### Cache Design

- **Hierarchical**: Three-level cache (L1, L2, L3)
- **Policy Pattern**: Pluggable replacement policies
- **Performance Tracking**: Hit/miss rates, access times, cycle counting
- **Logging**: Optional detailed access logging

## Performance Metrics

### Allocator Metrics

- Total memory usage
- External fragmentation percentage
- Allocation success rate
- Number of allocated blocks

### Cache Metrics

- Overall hit rate
- Per-level hit rates (L1, L2, L3)
- Average access time
- Memory access count
- Total cycles

## Documentation

For detailed implementation details, design decisions, and analysis, refer to `Report.pdf` included in this repository.
