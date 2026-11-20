# Virtual Memory + CPU Simulator

## Overview

This project simulates several operating system concepts involving virtual memory, CPU execution, processes, threads, and page replacement.

The simulator includes:

- Two-level page tables
- Translation Lookaside Buffer (TLB)
- Configurable TLB size
- LRU TLB replacement
- Advanced Clock page replacement
- Referenced and modified bits
- Page fault handling
- Multiple processes
- Multiple threads
- CPU execution tracking
- Context switch tracking
- Thread switch tracking
- Memory access logging
- Physical frame tracking

## Project Files

- `main.cpp` - Main simulator and memory access processing
- `Process.cpp` / `Process.h` - Process and thread implementation
- `CPU.cpp` / `CPU.h` - CPU execution and statistics
- `Scheduler.cpp` / `Scheduler.h` - Process/thread scheduling support
- `TLB.cpp` / `TLB.h` - TLB implementation with LRU replacement
- `TwoLevelPageTable.cpp` / `TwoLevelPageTable.h` - Two-level page table
- `Clock.cpp` / `Clock.h` - Advanced Clock page replacement algorithm
- `input/trace.txt` - Memory access trace
- `output.log` - Memory translation results
- `Makefile` - Build commands

## Compile

Compile the simulator using:

```bash
make clean
make
