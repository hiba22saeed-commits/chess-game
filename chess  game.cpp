// ============================================================
//  main.cpp  --  Entry point
//  ALL MEMBERS: Integration point -- creates and runs the game
// ============================================================

#include "Game.h"
#include <iostream>
using namespace std;

int main() {
    bool again = true;
    while (again) {
        Game game;        // fresh board every time
        again = game.run();
    }
    cout << FG_CYAN BOLD "Goodbye! Thanks for playing C++ Chess.\n" RESET;
    return 0;
}