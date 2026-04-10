/*=============================================================
  Algorithms.cpp    First-Fit, Next-Fit, Best-Fit, Worst-Fit
  CS 330: Operating Systems OEL    Spring 2026
=============================================================*/

#include "Algorithms.h"
#include <climits>

//--------------------------------------------------------------
// Helper: print success
//--------------------------------------------------------------
static void printOK(const char* name, int pid, int sz, int blkId,
                    const char* extra=""){
    setColor(CLR_GREEN);
    printf("  [%-10s] PID %-4d (%3dKB) -> Blk%-3d %s\n",
           name, pid, sz, blkId, extra);
    resetColor();
    Sleep(50);
}

//--------------------------------------------------------------
// Helper: print failure
//--------------------------------------------------------------
static void printFail(const char* name, int pid, int sz){
    setColor(CLR_RED);
    printf("  [%-10s] FAILED — PID %d needs %dKB, no block fits!\n",
           name, pid, sz);
    resetColor();
    Sleep(50);
}

//--------------------------------------------------------------
// 1. FIRST-FIT
//--------------------------------------------------------------
bool Algorithms::firstFit(Pool& pool, int size, int pid){
    for(int i=0;i<(int)pool.size();i++){
        if(pool[i].isFree && pool[i].size >= size){
            splitBlock(pool, i, size);
            pool[i].isFree=false;
            pool[i].pid=pid;
            printOK("FIRST-FIT", pid, size, pool[i].id);
            return true;
        }
    }
    printFail("FIRST-FIT", pid, size);
    return false;
}

//--------------------------------------------------------------
// 2. NEXT-FIT
//--------------------------------------------------------------
bool Algorithms::nextFit(Pool& pool, int size, int pid, int& lastIdx){
    int n=(int)pool.size();
    for(int c=0;c<n;c++){
        int i=(lastIdx+c)%n;
        if(pool[i].isFree && pool[i].size >= size){
            splitBlock(pool, i, size);
            pool[i].isFree=false;
            pool[i].pid=pid;
            lastIdx=(i+1)%(int)pool.size();
            char ex[32];
            snprintf(ex,sizeof(ex),"(next pos->%d)",lastIdx);
            printOK("NEXT-FIT", pid, size, pool[i].id, ex);
            return true;
        }
    }
    printFail("NEXT-FIT", pid, size);
    return false;
}

//--------------------------------------------------------------
// 3. BEST-FIT
//--------------------------------------------------------------
bool Algorithms::bestFit(Pool& pool, int size, int pid){
    int bestIdx=-1, bestWaste=INT_MAX;
    for(int i=0;i<(int)pool.size();i++){
        if(pool[i].isFree && pool[i].size >= size){
            int w=pool[i].size-size;
            if(w<bestWaste){ bestWaste=w; bestIdx=i; }
        }
    }
    if(bestIdx!=-1){
        splitBlock(pool, bestIdx, size);
        pool[bestIdx].isFree=false;
        pool[bestIdx].pid=pid;
        char ex[32];
        snprintf(ex,sizeof(ex),"(waste=%dKB)",bestWaste);
        printOK("BEST-FIT", pid, size, pool[bestIdx].id, ex);
        return true;
    }
    printFail("BEST-FIT", pid, size);
    return false;
}

//--------------------------------------------------------------
// 4. WORST-FIT
//--------------------------------------------------------------
bool Algorithms::worstFit(Pool& pool, int size, int pid){
    int worstIdx=-1, worstSz=-1;
    for(int i=0;i<(int)pool.size();i++){
        if(pool[i].isFree && pool[i].size >= size){
            if(pool[i].size > worstSz){ worstSz=pool[i].size; worstIdx=i; }
        }
    }
    if(worstIdx!=-1){
        splitBlock(pool, worstIdx, size);
        pool[worstIdx].isFree=false;
        pool[worstIdx].pid=pid;
        char ex[32];
        snprintf(ex,sizeof(ex),"(was %dKB)",worstSz);
        printOK("WORST-FIT", pid, size, pool[worstIdx].id, ex);
        return true;
    }
    printFail("WORST-FIT", pid, size);
    return false;
}
