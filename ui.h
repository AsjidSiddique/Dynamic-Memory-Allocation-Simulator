#ifndef UI_H
#define UI_H

/*=============================================================
  ui.h  —  Console colours, animations, fancy printing
  Uses windows.h (Dev-C++ / MinGW / Windows only)
  CS 330: Operating Systems OEL  —  Spring 2026
=============================================================*/

#include <windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace std;

//--------------------------------------------------------------
// Colour constants  (Windows console colour codes)
//--------------------------------------------------------------
#define CLR_BLACK        0
#define CLR_DARK_BLUE    1
#define CLR_DARK_GREEN   2
#define CLR_DARK_CYAN    3
#define CLR_DARK_RED     4
#define CLR_DARK_MAGENTA 5
#define CLR_DARK_YELLOW  6
#define CLR_GREY         7
#define CLR_DARK_GREY    8
#define CLR_BLUE         9
#define CLR_GREEN        10
#define CLR_CYAN         11
#define CLR_RED          12
#define CLR_MAGENTA      13
#define CLR_YELLOW       14
#define CLR_WHITE        15
#define CLR_RESET        7

//--------------------------------------------------------------
// Set / reset console colour
//--------------------------------------------------------------
inline void setColor(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(WORD)c);
}
inline void resetColor(){ setColor(CLR_RESET); }

//--------------------------------------------------------------
// Coloured print helpers
//--------------------------------------------------------------
inline void printC(const string& s, int c){
    setColor(c); cout << s; resetColor();
}
inline void printLn(const string& s, int c){
    setColor(c); cout << s << "\n"; resetColor();
}

//--------------------------------------------------------------
// Typewriter animation — prints one char at a time
//--------------------------------------------------------------
inline void typeWrite(const string& s, int c, int ms=16){
    setColor(c);
    for(char ch : s){
        cout << ch;
        cout.flush();
        Sleep(ms);
    }
    cout << "\n";
    resetColor();
}

//--------------------------------------------------------------
// Spinner  (shows while "processing")
//--------------------------------------------------------------
inline void spinner(const string& msg, int steps=14){
    const char fr[]={'|','/','|','\\','-','/'};
    setColor(CLR_YELLOW);
    for(int i=0;i<steps;i++){
        printf("\r  %c  %s   ", fr[i%6], msg.c_str());
        fflush(stdout);
        Sleep(70);
    }
    printf("\r  * %s  [DONE]   \n", msg.c_str());
    resetColor();
}

//--------------------------------------------------------------
// Progress bar
//--------------------------------------------------------------
inline void progressBar(const string& lbl, int total=24){
    setColor(CLR_CYAN);
    cout << "\n  " << lbl << "\n  [";
    for(int i=0;i<=total;i++){
        printf("\r  [");
        setColor(CLR_GREEN);
        for(int j=0;j<i;j++)     printf("%c",(char)219);
        setColor(CLR_DARK_GREY);
        for(int j=i;j<total;j++) printf("%c",(char)176);
        setColor(CLR_CYAN);
        printf("] %3d%%  ", i*100/total);
        fflush(stdout);
        Sleep(35);
    }
    cout << "\n";
    resetColor();
}

//--------------------------------------------------------------
// Separator lines
//--------------------------------------------------------------
inline void sep(char c='-', int w=72, int col=CLR_DARK_CYAN){
    setColor(col);
    cout << "  ";
    for(int i=0;i<w;i++) cout<<c;
    cout << "\n";
    resetColor();
}
inline void dblSep(int col=CLR_CYAN){ sep('=',72,col); }

//--------------------------------------------------------------
// Clear screen
//--------------------------------------------------------------
inline void cls(){ system("cls"); }

//--------------------------------------------------------------
// Wait for ENTER
//--------------------------------------------------------------
inline void waitKey(const string& msg="  Press ENTER to continue..."){
    setColor(CLR_DARK_YELLOW);
    cout << "\n" << msg;
    resetColor();
    // flush leftover newlines then wait
    cin.clear();
    cin.sync();
    cin.get();
}

//--------------------------------------------------------------
// Live result line  (typewriter, coloured)
//--------------------------------------------------------------
inline void liveResult(const string& s, int col=CLR_GREEN){
    typeWrite("  >> "+s, col, 9);
}

//--------------------------------------------------------------
// Animated splash banner
//--------------------------------------------------------------
inline void printBanner(){
    cls();
    Sleep(100);
    dblSep(CLR_MAGENTA);
    typeWrite("   CS 330 : Operating Systems  |  Open-Ended Lab  |  Spring 2026",CLR_CYAN,10);
    typeWrite("   DYNAMIC MEMORY ALLOCATION SIMULATOR",CLR_YELLOW,14);
    typeWrite("   First-Fit  |  Next-Fit  |  Best-Fit  |  Worst-Fit",CLR_GREEN,10);
    dblSep(CLR_MAGENTA);
    Sleep(250);
}

#endif // UI_H
