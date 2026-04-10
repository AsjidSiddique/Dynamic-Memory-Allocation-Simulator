#ifndef MENU_H
#define MENU_H

/*=============================================================
  Menu.h  —  Animated main menu and sub-menus
  CS 330: Operating Systems OEL  —  Spring 2026
=============================================================*/

#include "Simulation.h"

class Menu {
public:
    static void showMainMenu();   // main loop — call this from main()
private:
    static int  algoSelector();
    static void runManualMode(int algoChoice, const string& name);
    static void showHelp();
};

#endif // MENU_H
