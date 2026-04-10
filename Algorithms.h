#ifndef ALGORITHMS_H
#define ALGORITHMS_H

/*=============================================================
  Algorithms.h   Four dynamic memory allocation algorithms
  CS 330: Operating Systems OEL  —  Spring 2026
=============================================================*/

#include "MemoryPool.h"

class Algorithms {
public:
    // 1. First-Fit  scan from start, pick first block that fits
    static bool firstFit(Pool& pool, int size, int pid);

    // 2. Next-Fit   resume scan from last allocated position
    static bool nextFit(Pool& pool, int size, int pid, int& lastIdx);

    // 3. Best-Fit   pick smallest block that is still large enough
    static bool bestFit(Pool& pool, int size, int pid);

    // 4. Worst-Fit  pick the largest available block
    static bool worstFit(Pool& pool, int size, int pid);
};

#endif // ALGORITHMS_H
