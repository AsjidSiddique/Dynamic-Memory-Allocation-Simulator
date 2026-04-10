#ifndef SIMULATION_H
#define SIMULATION_H

/*=============================================================
  Simulation.h  —  Auto demo + stress test + comparison table
  CS 330: Operating Systems OEL  —  Spring 2026
=============================================================*/

#include "Algorithms.h"
#include <string>

class Simulation {
public:
    // Run full automated scenario for one algorithm
    static void run(int algoChoice, const string& name, int titleCol);

    // 10 random allocations stress test
    static void stressTest(int algoChoice, const string& name, Pool pool);

    // Side-by-side comparison table
    static void comparativeTable();
};

#endif // SIMULATION_H
