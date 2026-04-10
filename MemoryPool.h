#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

/*=============================================================
  MemoryPool.h    Block struct, pool management
  split / merge / dealloc / display / frag report
  CS 330: Operating Systems OEL  —  Spring 2026
=============================================================*/

#include "ui.h"
#include <vector>
#include <algorithm>
#include <climits>

//--------------------------------------------------------------
// Single memory block
//--------------------------------------------------------------
struct MemBlock {
    int  id;
    int  start;    // start address in KB
    int  size;     // size in KB
    bool isFree;
    int  pid;      // -1 when free
};

typedef vector<MemBlock> Pool;

//--------------------------------------------------------------
// Build the initial 10-block, 1024 KB pool
//--------------------------------------------------------------
inline Pool makePool(){
    Pool p;
    // id  start  size  free   pid
    p.push_back({0,    0,   80, true, -1});
    p.push_back({1,   80,  120, true, -1});
    p.push_back({2,  200,   50, true, -1});
    p.push_back({3,  250,  200, true, -1});
    p.push_back({4,  450,   30, true, -1});
    p.push_back({5,  480,  150, true, -1});
    p.push_back({6,  630,   60, true, -1});
    p.push_back({7,  690,  100, true, -1});
    p.push_back({8,  790,  184, true, -1});
    p.push_back({9,  974,   50, true, -1});
    // Total = 1024 KB
    return p;
}

//--------------------------------------------------------------
// Print the address-bar memory layout diagram
//--------------------------------------------------------------
inline void printLayoutDiagram(){
    dblSep(CLR_YELLOW);
    printLn("  INITIAL MEMORY LAYOUT  (10 blocks, Total = 1024 KB)",CLR_YELLOW);
    sep('-',72,CLR_DARK_YELLOW);

    setColor(CLR_CYAN);
    cout << "  Addresses (KB): ";
    int addr=0;
    Pool tmp=makePool();
    for(auto& b:tmp){ printf("%d--",addr); addr+=b.size; }
    printf("%d\n",addr);

    setColor(CLR_GREEN);
    cout << "  Block IDs     : ";
    for(int i=0;i<10;i++) printf("[BLK%-2d]",i);
    cout << "\n";

    setColor(CLR_WHITE);
    cout << "  Sizes (KB)    : ";
    for(auto& b:tmp) printf("[%-4d] ",b.size);
    cout << "\n";

    setColor(CLR_DARK_GREEN);
    cout << "  Status        : ";
    for(int i=0;i<10;i++) printf("[FREE]  ");
    cout << "\n";

    resetColor();
    dblSep(CLR_YELLOW);
}

//--------------------------------------------------------------
// Print pool table  (coloured rows)
//--------------------------------------------------------------
inline void printPool(const Pool& pool, const string& label=""){
    if(!label.empty()){
        cout << "\n";
        setColor(CLR_YELLOW); cout << "  [STATE] ";
        setColor(CLR_WHITE);  cout << label << "\n";
        resetColor();
    }
    sep('-',72,CLR_DARK_CYAN);
    setColor(CLR_CYAN);
    printf("  %-7s %-11s %-11s %-14s %-6s\n",
           "BlkID","Start(KB)","Size(KB)","Status","PID");
    resetColor();
    sep('-',72,CLR_DARK_CYAN);

    for(const auto& b:pool){
        setColor(b.isFree ? CLR_GREEN : CLR_RED);
        printf("  %-7d %-11d %-11d %-14s %-6s\n",
               b.id, b.start, b.size,
               b.isFree?"FREE":"ALLOCATED",
               b.isFree?"-":to_string(b.pid).c_str());
        resetColor();
    }
    sep('-',72,CLR_DARK_CYAN);
}

//--------------------------------------------------------------
// Visual memory bar  (block chars)
//--------------------------------------------------------------
inline void printMemBar(const Pool& pool){
    setColor(CLR_DARK_YELLOW);
    cout << "  MemMap:[";
    for(const auto& b:pool){
        int cells=max(1,b.size/10);
        setColor(b.isFree ? CLR_GREEN : CLR_RED);
        for(int i=0;i<cells;i++)
            cout << (char)(b.isFree ? 176 : 219);
        setColor(CLR_DARK_YELLOW);
        cout << '|';
    }
    cout << "] ";
    setColor(CLR_GREEN);  cout << (char)176 << "=free ";
    setColor(CLR_RED);    cout << (char)219 << "=used\n";
    resetColor();
}

//--------------------------------------------------------------
// Fragmentation report
//--------------------------------------------------------------
inline void fragReport(const Pool& pool){
    int totalFree=0, freeBlks=0, largest=0;
    for(const auto& b:pool){
        if(b.isFree){
            totalFree+=b.size;
            freeBlks++;
            if(b.size>largest) largest=b.size;
        }
    }
    sep('~',72,CLR_MAGENTA);
    printLn("  FRAGMENTATION REPORT",CLR_MAGENTA);
    sep('~',72,CLR_MAGENTA);
    setColor(CLR_CYAN);   printf("  Total Free Memory  : ");
    setColor(CLR_WHITE);  printf("%d KB\n",totalFree);
    setColor(CLR_CYAN);   printf("  Free Block Count   : ");
    setColor(CLR_WHITE);  printf("%d\n",freeBlks);
    setColor(CLR_CYAN);   printf("  Largest Free Block : ");
    setColor(CLR_WHITE);  printf("%d KB\n",largest);
    setColor(CLR_CYAN);   printf("  External Frag.     : ");
    if(freeBlks>1){ setColor(CLR_RED);   printf("YES  (scattered free blocks)\n"); }
    else           { setColor(CLR_GREEN); printf("NO   (memory is contiguous)\n");  }
    resetColor();
    sep('~',72,CLR_MAGENTA);
}

//--------------------------------------------------------------
// Block splitting  (called inside alloc if leftover > 20 KB)
//--------------------------------------------------------------
inline void splitBlock(Pool& pool, int idx, int reqSize){
    MemBlock& b = pool[idx];
    int leftover = b.size - reqSize;
    if(leftover > 20){
        int maxId=0;
        for(const auto& blk:pool) if(blk.id>maxId) maxId=blk.id;

        MemBlock nb;
        nb.id    = maxId+1;
        nb.start = b.start + reqSize;
        nb.size  = leftover;
        nb.isFree= true;
        nb.pid   = -1;

        b.size = reqSize;
        pool.insert(pool.begin()+idx+1, nb);

        setColor(CLR_DARK_YELLOW);
        printf("  [SPLIT] Blk%d split -> kept %dKB | new Blk%d(%dKB) returned to pool\n",
               b.id, reqSize, nb.id, leftover);
        resetColor();
    }
}

//--------------------------------------------------------------
// Block merging / coalescing
//--------------------------------------------------------------
inline void mergeBlocks(Pool& pool){
    bool merged=true;
    while(merged){
        merged=false;
        for(int i=0;i+1<(int)pool.size();i++){
            if(pool[i].isFree && pool[i+1].isFree){
                setColor(CLR_BLUE);
                printf("  [MERGE] Blk%d(%dKB) + Blk%d(%dKB) => Blk%d(%dKB)\n",
                       pool[i].id,   pool[i].size,
                       pool[i+1].id, pool[i+1].size,
                       pool[i].id,   pool[i].size+pool[i+1].size);
                resetColor();
                pool[i].size += pool[i+1].size;
                pool.erase(pool.begin()+i+1);
                merged=true;
                break;
            }
        }
    }
}

//--------------------------------------------------------------
// Deallocation
//--------------------------------------------------------------
inline bool deallocate(Pool& pool, int pid){
    for(auto& b:pool){
        if(!b.isFree && b.pid==pid){
            setColor(CLR_GREEN);
            printf("  [FREE ] Blk%d (%dKB) freed from PID %d\n",b.id,b.size,pid);
            resetColor();
            b.isFree=true;
            b.pid=-1;
            mergeBlocks(pool);
            return true;
        }
    }
    setColor(CLR_RED);
    printf("  [FREE ] FAILED — no block found for PID %d\n",pid);
    resetColor();
    return false;
}

#endif // MEMORY_POOL_H
