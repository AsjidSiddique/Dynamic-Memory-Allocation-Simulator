/*=============================================================
  main.cpp  Entry point
  CS 330: Operating Systems  |  Open-Ended Lab  |  Spring 2026
  Dynamic Memory Allocation Simulator

  HOW TO BUILD IN DEV-C++:

=============================================================*/

#include "Menu.h"

int main(){
    // Enable extended ASCII block chars in Windows console
    SetConsoleOutputCP(437);

    // Show animated splash banner
    printBanner();
    Sleep(350);

    // Hand control to the menu
    Menu::showMainMenu();

    return 0;
}
