/*=============================================================
  Menu.cpp  —  All menus, manual input mode, help screen
  CS 330: Operating Systems OEL  —  Spring 2026
=============================================================*/

#include "Menu.h"
#include <iostream>
#include <string>

using namespace std;

//--------------------------------------------------------------
// Algorithm names and their title colours
//--------------------------------------------------------------
static const char* ANAME[] = {"","FIRST-FIT","NEXT-FIT","BEST-FIT","WORST-FIT"};
static const int   ACOL[]  = { 0, CLR_GREEN,  CLR_CYAN,  CLR_YELLOW, CLR_MAGENTA};

//--------------------------------------------------------------
// Helper: one animated menu item
//--------------------------------------------------------------
static void menuItem(int n, const string& lbl,
                     int nc=CLR_YELLOW, int lc=CLR_WHITE){
    setColor(nc);
    printf("    [%d] ",n);
    typeWrite(lbl, lc, 7);
}

//--------------------------------------------------------------
// Draw the main menu
//--------------------------------------------------------------
static void drawMain(){
    cls();
    dblSep(CLR_MAGENTA);
    typeWrite("   CS330 OEL  |  Dynamic Memory Allocation Simulator",CLR_CYAN,9);
    typeWrite("   Spring 2026  |  BESE-30",CLR_DARK_CYAN,7);
    dblSep(CLR_MAGENTA);
    cout << "\n";
    printLn("  MAIN MENU",CLR_YELLOW);
    sep('-',72,CLR_DARK_YELLOW);

    menuItem(1,"Run Algorithm      (Auto full demo)",      CLR_GREEN,   CLR_WHITE);
    menuItem(2,"Manual Mode        (Enter your own alloc)",CLR_CYAN,    CLR_WHITE);
    menuItem(3,"Run ALL FOUR       (Full comparison run)", CLR_YELLOW,  CLR_WHITE);
    menuItem(4,"Comparison Table   (Analysis summary)",    CLR_MAGENTA, CLR_WHITE);
    menuItem(5,"Memory Layout      (Diagram view)",        CLR_BLUE,    CLR_WHITE);
    menuItem(6,"Help / About",                             CLR_DARK_CYAN,CLR_WHITE);
    menuItem(0,"Exit",                                     CLR_RED,     CLR_RED);

    cout << "\n";
    sep('-',72,CLR_DARK_CYAN);
    setColor(CLR_YELLOW); cout << "  Enter choice: "; resetColor();
}

//--------------------------------------------------------------
// Algorithm selector sub-menu
//--------------------------------------------------------------
int Menu::algoSelector(){
    cls();
    dblSep(CLR_CYAN);
    printLn("  SELECT ALGORITHM",CLR_CYAN);
    sep('-',72,CLR_DARK_CYAN);

    menuItem(1,"First-Fit   - scan start, pick first fitting block",CLR_GREEN,  CLR_WHITE);
    menuItem(2,"Next-Fit    - resume from last allocated position", CLR_CYAN,   CLR_WHITE);
    menuItem(3,"Best-Fit    - pick smallest sufficient block",      CLR_YELLOW, CLR_WHITE);
    menuItem(4,"Worst-Fit   - pick largest available block",        CLR_MAGENTA,CLR_WHITE);
    menuItem(0,"Back",                                              CLR_RED,    CLR_RED);

    sep('-',72,CLR_DARK_CYAN);
    setColor(CLR_YELLOW); cout << "  Enter choice: "; resetColor();

    int ch=0;
    cin >> ch;
    cin.ignore(10000,'\n');
    return ch;
}

//--------------------------------------------------------------
// Manual input mode
//--------------------------------------------------------------
void Menu::runManualMode(int algoChoice, const string& name){
    Pool pool  = makePool();
    int lastIdx = 0;

    cls();
    dblSep(ACOL[algoChoice]);
    setColor(ACOL[algoChoice]);
    cout << "  MANUAL MODE -- " << name << "\n";
    resetColor();
    dblSep(ACOL[algoChoice]);

    printPool(pool,"Initial Memory");
    printMemBar(pool);

    while(true){
        cout << "\n";
        sep('-',72,CLR_DARK_CYAN);
        setColor(CLR_YELLOW);  cout << "    [1] Allocate memory\n";
        setColor(CLR_MAGENTA); cout << "    [2] Deallocate memory\n";
        setColor(CLR_CYAN);    cout << "    [3] Show current pool\n";
        setColor(CLR_GREEN);   cout << "    [4] Fragmentation report\n";
        setColor(CLR_RED);     cout << "    [0] Back to main menu\n";
        resetColor();
        sep('-',72,CLR_DARK_CYAN);
        setColor(CLR_YELLOW); cout << "  Enter choice: "; resetColor();

        int ch=0;
        cin >> ch;
        cin.ignore(10000,'\n');

        if(ch==0) break;

        if(ch==1){
            int sz=0, pid=0;
            setColor(CLR_CYAN); cout << "  Size to allocate (KB) : "; resetColor();
            cin >> sz; cin.ignore(10000,'\n');
            setColor(CLR_CYAN); cout << "  Process ID (PID)      : "; resetColor();
            cin >> pid; cin.ignore(10000,'\n');

            switch(algoChoice){
                case 1: Algorithms::firstFit (pool,sz,pid);         break;
                case 2: Algorithms::nextFit  (pool,sz,pid,lastIdx); break;
                case 3: Algorithms::bestFit  (pool,sz,pid);         break;
                case 4: Algorithms::worstFit (pool,sz,pid);         break;
            }
            printPool(pool);
            printMemBar(pool);
        }
        else if(ch==2){
            int pid=0;
            setColor(CLR_CYAN); cout << "  PID to free : "; resetColor();
            cin >> pid; cin.ignore(10000,'\n');
            deallocate(pool,pid);
            printPool(pool,"After Dealloc");
            printMemBar(pool);
        }
        else if(ch==3){
            printPool(pool,"Current State");
            printMemBar(pool);
        }
        else if(ch==4){
            fragReport(pool);
        }
    }
}

//--------------------------------------------------------------
// Help screen
//--------------------------------------------------------------
void Menu::showHelp(){
    cls();
    dblSep(CLR_CYAN);
    typeWrite("  HELP / ABOUT",CLR_CYAN,12);
    dblSep(CLR_CYAN);

    const char* lines[]={
        "This simulator implements 4 dynamic memory allocation algorithms.",
        "It shows allocation, deallocation, block splitting,",
        "block merging (coalescing), fragmentation reports, stress tests.",
        "",
        "BLOCK SPLITTING : leftover > 20 KB -> block is split.",
        "BLOCK MERGING   : on dealloc, adjacent free blocks merge.",
        "STRESS TEST     : 10 random allocs (10-150 KB) auto-run.",
        "",
        "COLOUR KEY:",
        "  GREEN   = free blocks / successful operations",
        "  RED     = allocated blocks / failures",
        "  YELLOW  = headers / operations",
        "  BLUE    = merge events",
        "  CYAN    = info / table headings",
        "  MAGENTA = dealloc / frag report",
        nullptr
    };
    for(int i=0;lines[i];i++){
        liveResult(lines[i],CLR_WHITE);
        Sleep(55);
    }
    dblSep(CLR_CYAN);
    waitKey();
}

//--------------------------------------------------------------
// Main menu loop
//--------------------------------------------------------------
void Menu::showMainMenu(){
    srand((unsigned)time(nullptr));

    while(true){
        drawMain();

        int ch=0;
        cin >> ch;
        cin.ignore(10000,'\n');

        switch(ch){

            case 1:{
                int a=algoSelector();
                if(a>=1&&a<=4)
                    Simulation::run(a,ANAME[a],ACOL[a]);
                break;
            }

            case 2:{
                int a=algoSelector();
                if(a>=1&&a<=4)
                    runManualMode(a,ANAME[a]);
                break;
            }

            case 3:
                for(int a=1;a<=4;a++)
                    Simulation::run(a,ANAME[a],ACOL[a]);
                Simulation::comparativeTable();
                waitKey();
                break;

            case 4:
                Simulation::comparativeTable();
                waitKey();
                break;

            case 5:
                cls();
                printLayoutDiagram();
                waitKey();
                break;

            case 6:
                showHelp();
                break;

            case 0:
                cls();
                typeWrite("  Goodbye! -- CS330 OEL Simulator",CLR_CYAN,13);
                Sleep(700);
                return;

            default:
                setColor(CLR_RED);
                cout << "  Invalid choice. Please try again.\n";
                resetColor();
                Sleep(600);
                break;
        }
    }
}
