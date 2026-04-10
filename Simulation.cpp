/*=============================================================
  Simulation.cpp  —  Full simulation, stress test, comparison
  CS 330: Operating Systems OEL  —  Spring 2026
=============================================================*/

#include "Simulation.h"
#include <cstdlib>
#include <ctime>

//--------------------------------------------------------------
// Route allocation to correct algorithm
//--------------------------------------------------------------
static bool callAlloc(int choice, Pool& pool, int sz, int pid, int& li){
    switch(choice){
        case 1: return Algorithms::firstFit (pool,sz,pid);
        case 2: return Algorithms::nextFit  (pool,sz,pid,li);
        case 3: return Algorithms::bestFit  (pool,sz,pid);
        default:return Algorithms::worstFit (pool,sz,pid);
    }
}

//--------------------------------------------------------------
// Print algorithm section header
//--------------------------------------------------------------
static void algoHeader(const string& name, int col){
    cout << "\n";
    dblSep(col);
    setColor(col);
    cout << "  ALGORITHM : ";
    setColor(CLR_WHITE);
    cout << name << "\n";
    resetColor();
    dblSep(col);
    Sleep(250);
}

//--------------------------------------------------------------
// Simulation::run
//--------------------------------------------------------------
void Simulation::run(int algoChoice, const string& name, int titleCol){
    Pool pool  = makePool();
    int lastIdx = 0;

    algoHeader(name, titleCol);
    printPool(pool,"Initial Memory State");
    printMemBar(pool);
    waitKey();

    //-- Allocation requests (5 total; last one intentionally fails)
    struct Req{ int size; int pid; };
    Req reqs[]={
        {75, 101},
        {110,102},
        {45, 103},
        {180,104},
        {500,105}   // <-- INTENTIONAL FAILURE
    };

    for(auto& r : reqs){
        cout << "\n";
        sep('>',72,titleCol);
        setColor(titleCol);
        printf("  ALLOCATE : PID %-4d  requests %d KB\n",r.pid,r.size);
        sep('<',72,titleCol);

        callAlloc(algoChoice, pool, r.size, r.pid, lastIdx);
        Sleep(180);
        printPool(pool);
        printMemBar(pool);
        waitKey();
    }

    //-- Deallocation (2 requests)
    int dpids[]={102,101};
    for(int pid : dpids){
        cout << "\n";
        sep('>',72,CLR_MAGENTA);
        setColor(CLR_MAGENTA);
        printf("  DEALLOCATE : PID %d\n",pid);
        sep('<',72,CLR_MAGENTA);

        deallocate(pool,pid);
        Sleep(250);
        printPool(pool,"After Deallocation");
        printMemBar(pool);
        waitKey();
    }

    fragReport(pool);
    waitKey();

    stressTest(algoChoice, name, pool);
    waitKey();
}

//--------------------------------------------------------------
// Simulation::stressTest
//--------------------------------------------------------------
void Simulation::stressTest(int algoChoice, const string& name, Pool pool){
    dblSep(CLR_YELLOW);
    setColor(CLR_YELLOW);
    cout << "  STRESS TEST -- " << name
         << "  (10 random allocs, 10-150 KB)\n";
    resetColor();
    dblSep(CLR_YELLOW);

    progressBar("Running stress test...", 20);

    int lastIdx=0, ok=0, fail=0;
    for(int i=0;i<10;i++){
        int sz  = 10 + rand()%141;
        int pid = 200+i;

        setColor(CLR_DARK_YELLOW);
        printf("  [%2d/10] PID%d  size=%3dKB  ->  ",i+1,pid,sz);
        fflush(stdout);
        resetColor();

        bool res = callAlloc(algoChoice, pool, sz, pid, lastIdx);
        res ? ok++ : fail++;
        Sleep(60);
    }

    cout << "\n";
    setColor(CLR_GREEN); printf("  Succeeded : %d\n",ok);
    setColor(CLR_RED);   printf("  Failed    : %d\n",fail);
    resetColor();

    fragReport(pool);
    printMemBar(pool);
}

//--------------------------------------------------------------
// Simulation::comparativeTable
//--------------------------------------------------------------
void Simulation::comparativeTable(){
    cls();
    dblSep(CLR_CYAN);
    typeWrite("  COMPARATIVE ANALYSIS TABLE",CLR_CYAN,12);
    dblSep(CLR_CYAN);

    setColor(CLR_YELLOW);
    printf("  %-22s %-16s %-16s %-16s %-16s\n",
           "Criterion","First-Fit","Next-Fit","Best-Fit","Worst-Fit");
    resetColor();
    sep('-',72,CLR_DARK_CYAN);

    struct Row{
        const char* c;
        const char* ff;
        const char* nf;
        const char* bf;
        const char* wf;
        int col;
    };
    Row rows[]={
        {"Alloc Speed",    "Fast",      "Fast",      "Slower",    "Slower",    CLR_GREEN  },
        {"Internal Frag.", "Moderate",  "Moderate",  "Low",       "High",      CLR_CYAN   },
        {"External Frag.", "High",      "Moderate",  "High(tiny)","Low",       CLR_MAGENTA},
        {"Block Splits",   "Common",    "Common",    "Rare",      "Very Cmn",  CLR_YELLOW },
        {"Merge Frequency","Frequent",  "Frequent",  "Frequent",  "Less Freq", CLR_BLUE   },
        {"Memory Use",     "Good init", "Spread",    "Best",      "Wasteful",  CLR_GREEN  },
        {"Stress Result",  "Degrades",  "Spreads",   "Consistent","Fails Early",CLR_RED   },
        {"Best Use Case",  "General",   "Uniform sz","Mixed",     "Large Req", CLR_CYAN   },
    };

    for(auto& r : rows){
        setColor(r.col);
        printf("  %-22s %-16s %-16s %-16s %-16s\n",
               r.c,r.ff,r.nf,r.bf,r.wf);
        resetColor();
        Sleep(55);
    }

    dblSep(CLR_CYAN);
    typeWrite("\n  JUSTIFICATION SUMMARY",CLR_YELLOW,12);
    sep('-',72,CLR_DARK_YELLOW);

    const char* just[]={
        "First-Fit : Fastest (stops at 1st fit). Fragments pile at front.",
        "Next-Fit  : Spreads load evenly, avoids reusing same early blocks.",
        "Best-Fit  : Least waste/alloc. Leaves tiny unusable gaps over time.",
        "Worst-Fit : Uses largest block, good for future large requests.",
        "CONCLUSION: Best-Fit = best utilisation. First-Fit = best speed.",
        nullptr
    };
    for(int i=0;just[i];i++){
        liveResult(just[i],CLR_WHITE);
        Sleep(90);
    }
    dblSep(CLR_CYAN);
}
