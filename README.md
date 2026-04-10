# Dynamic Memory Allocation Simulator

A console-based C++ simulator that implements and compares all four classical dynamic memory allocation algorithms — **First-Fit**, **Next-Fit**, **Best-Fit**, and **Worst-Fit** — on an independent 1024 KB memory pool per algorithm. Built with Dev-C++ for Windows console.

![Language](https://img.shields.io/badge/Language-C%2B%2B11-blue?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey?style=flat-square)
![IDE](https://img.shields.io/badge/IDE-Dev--C%2B%2B-orange?style=flat-square)
![Compiler](https://img.shields.io/badge/Compiler-MinGW%20GCC-green?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-teal?style=flat-square)

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Memory Pool Layout](#memory-pool-layout)
- [Algorithms](#algorithms)
- [Project Structure](#project-structure)
- [Build and Run](#build-and-run)
- [How It Works](#how-it-works)
- [Sample Output](#sample-output)
- [Comparative Analysis](#comparative-analysis)
- [Author](#author)

---

## Overview

This project simulates how an operating system manages physical memory at runtime. A fixed pool of 1024 KB is divided into 10 free blocks of varying sizes. Each of the four allocation algorithms receives its own independent copy of this pool and processes the same set of allocation and deallocation requests — making it possible to observe exactly how different strategies affect fragmentation, utilization, and failure rates under identical workloads.

The simulator also includes automatic **block splitting** when allocated blocks are significantly larger than requested, and **block coalescing** (merging) when adjacent free blocks appear after a deallocation.

---

## Features

- Four allocation algorithms — each running on an independent pool copy
- Block splitting — if the leftover after an allocation exceeds 20 KB, it is split off into a new free block
- Block coalescing — on every deallocation, adjacent free blocks are automatically merged into one
- Intentional failure simulation — one allocation request is always too large to satisfy, demonstrating error handling
- Fragmentation report after each algorithm's run — shows total free memory, free block count, largest free block, and external fragmentation status
- Visual ASCII memory bar — shows free vs allocated blocks at a glance using block characters
- Stress test — 10 random allocation requests (sizes 10–150 KB) expose divergence between algorithms under sustained load
- Colour-coded console output using the Windows Console API — green for free/success, red for allocated/failure, yellow for splits, blue for merges

---

## Memory Pool Layout

The simulator initialises the following 10-block pool at startup. Each algorithm receives a fresh copy.

```
Address (KB):  0    80   200  250  450  480  630  690  790  974  1024
               |    |    |    |    |    |    |    |    |    |    |
Blocks:      [BLK0][BLK1][BLK2][BLK3][BLK4][BLK5][BLK6][BLK7][BLK8][BLK9]
Sizes (KB):  [ 80 ][ 120][ 50 ][ 200][ 30 ][ 150][ 60 ][ 100][ 184][ 50 ]
Status:      [FREE][FREE][FREE][FREE][FREE][FREE][FREE][FREE][FREE][FREE]

Total = 1024 KB
```

| Block | Start (KB) | Size (KB) | Status |
|-------|-----------|-----------|--------|
| BLK 0 | 0 | 80 | FREE |
| BLK 1 | 80 | 120 | FREE |
| BLK 2 | 200 | 50 | FREE |
| BLK 3 | 250 | 200 | FREE |
| BLK 4 | 450 | 30 | FREE |
| BLK 5 | 480 | 150 | FREE |
| BLK 6 | 630 | 60 | FREE |
| BLK 7 | 690 | 100 | FREE |
| BLK 8 | 790 | 184 | FREE |
| BLK 9 | 974 | 50 | FREE |

---

## Algorithms

### First-Fit
Scans from the beginning of the pool and allocates the **first block** that is large enough. It is the fastest algorithm because the scan stops as soon as a fit is found. The downside is that the front of the pool fills up quickly, leaving the start heavily fragmented while large blocks at the end remain untouched.

### Next-Fit
Works like First-Fit but instead of restarting the scan from the beginning every time, it **resumes from where the last allocation ended** (circular scan). This spreads allocations more evenly across the entire pool and avoids repeatedly fragmenting the same region at the start.

### Best-Fit
Scans the **entire pool** and picks the block with the **smallest leftover** after the allocation — the tightest fit. This minimises wasted space per allocation. However, it is the slowest algorithm (full scan every time) and over time produces many tiny fragments that are too small to be useful.

### Worst-Fit
Scans the entire pool and picks the **largest available block**. The large leftover is returned to the pool as a new block and remains large enough to satisfy future requests. This delays the creation of unusable tiny fragments, but rapidly exhausts large blocks, causing failures sooner under heavy load.

---

## Project Structure

```
MemAllocOEL_FINAL/
│
├── main.cpp              # Entry point — sets console codepage, shows banner, launches menu
├── Menu.h                # Menu class declaration
├── Menu.cpp              # Interactive main menu — choose algorithm, run simulation, exit
├── Algorithms.h          # Algorithm class declaration — firstFit, nextFit, bestFit, worstFit
├── Algorithms.cpp        # Algorithm implementations with split support
├── MemoryPool.h          # MemBlock struct, Pool typedef, makePool(), printPool(),
│                         # printMemBar(), fragReport(), splitBlock(), mergeBlocks(), deallocate()
├── Simulation.h          # Simulation class declaration
├── Simulation.cpp        # Orchestrates allocation loop, deallocations, and stress test
├── ui.h                  # Windows console colour macros, separator helpers, printBanner()
├── MemAllocOEL.dev       # Dev-C++ project file
├── MemAllocOEL.exe       # Compiled executable (Windows)
└── Makefile.win          # MinGW makefile (auto-generated by Dev-C++)
```

---

## Build and Run

### Option 1 — Dev-C++ (recommended)

1. Open `MemAllocOEL.dev` in **Dev-C++ 5.11** (Orwell or Bloodshed edition)
2. Press **F9** to compile and run immediately
   — or press **Ctrl + F9** to compile only, then double-click `MemAllocOEL.exe`

### Option 2 — Command line with MinGW

```bash
g++ -o MemAllocOEL main.cpp Menu.cpp Algorithms.cpp Simulation.cpp -std=c++11 -lwinmm
MemAllocOEL.exe
```

### Option 3 — Using the included Makefile

```bash
mingw32-make -f Makefile.win
MemAllocOEL.exe
```

### Requirements

| Requirement | Version |
|-------------|---------|
| Operating System | Windows 10 / 11 |
| IDE | Dev-C++ 5.11 (Orwell / Bloodshed) |
| Compiler | MinGW GCC 4.8 or later |
| C++ Standard | C++11 or later |

> **Note:** The colour output and `Sleep()` calls use the Windows Console API (`<windows.h>`). The program will not compile on Linux or macOS without removing those dependencies.

---

## How It Works

When the program starts:

1. An animated splash banner is displayed using Windows console colour codes
2. The main menu asks you to select an algorithm (1–4) or exit
3. The selected algorithm runs through the following sequence on its own pool copy:

**Allocation phase (5 requests):**

| Request | PID | Size | Expected Result |
|---------|-----|------|-----------------|
| 1 | 101 | 75 KB | SUCCESS — block split if leftover > 20 KB |
| 2 | 102 | 110 KB | SUCCESS — block split if leftover > 20 KB |
| 3 | 103 | 45 KB | SUCCESS |
| 4 | 104 | 180 KB | SUCCESS — block split if leftover > 20 KB |
| 5 | 105 | 500 KB | **FAIL** — no single block is large enough |

**Deallocation phase (2 requests):**
- PID 102 is freed → adjacent blocks checked and merged if free
- PID 101 is freed → adjacent blocks checked and merged if free

**Fragmentation report:**
After deallocation, the simulator prints total free memory, number of free blocks, largest free block, and whether external fragmentation is present.

**Stress test (10 random requests):**
Ten allocation requests with random sizes between 10 KB and 150 KB are issued. This reveals how each algorithm behaves under sustained load and at what point it begins to fail.

You can return to the main menu and run a different algorithm to compare results directly.

---

## Sample Output

```
══════════════════════════════════════════════════════════════════════════
  ALGORITHM : First-Fit
══════════════════════════════════════════════════════════════════════════

  BlkID   Start(KB)   Size(KB)   Status         PID
  -------------------------------------------------------
  0       0           80         FREE           -
  1       80          120        FREE           -
  ...

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ALLOCATE : PID 101  requests 75 KB
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  [SPLIT]  Blk0 split -> kept 75KB | new Blk10(5KB) returned to pool
  [FIRST-FIT ] PID 101  ( 75KB) -> Blk0

  MemMap:[████████|░░░░░░░░░░░░|░░░░░|...] ░=free █=used

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  FRAGMENTATION REPORT
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Total Free Memory  : 584 KB
  Free Block Count   : 7
  Largest Free Block : 200 KB
  External Frag.     : YES  (scattered free blocks)
```

---

## Comparative Analysis

| Criterion | First-Fit | Next-Fit | Best-Fit | Worst-Fit |
|-----------|-----------|----------|----------|-----------|
| Allocation speed | Fastest | Fast | Slowest | Slow |
| Internal fragmentation | Moderate | Moderate | Minimal | Highest |
| External fragmentation | High (early pool) | Distributed | High (tiny gaps) | Low initially |
| Memory utilization | Moderate | Good | Good | Poor long-term |
| Stress test endurance | Fails ~request 6 | Best endurance | Fails ~request 7 | Fails ~request 5 |
| Best suited for | General purpose | Long-running servers | Memory-constrained | Large initial requests |

**Key takeaway:** No single algorithm is universally optimal. The best choice depends on the workload — First-Fit for speed, Next-Fit for balance, Best-Fit for tight memory budgets, Worst-Fit when large contiguous leftovers matter early on.

---

## Author

**Asjid Siddique**
(https://github.com/AsjidSiddique/Dynamic-Memory-Allocation-Simulator)

---

*Contributions and issues are welcome. Feel free to fork and extend with additional algorithms, a GUI frontend, or a Linux-compatible build.*
