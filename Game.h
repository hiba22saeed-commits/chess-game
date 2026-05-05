#pragma once
// ============================================================
//  Game.h  --  Game class declaration
//  MEMBER 1: Game loop, turn management, input handling
//  MEMBER 3 (shared): Check/checkmate/stalemate/draw detection
// ============================================================

#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <string>

// ANSI color codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"

// Board colors
#define BG_DARK     "\033[48;5;94m"   // dark brown square
#define BG_LIGHT    "\033[48;5;222m"  // light cream square
#define BG_MENU     "\033[48;5;235m"  // dark gray for menu bg

// Piece colors
#define FG_WHITE_P  "\033[97;1m"      // bright white + bold
#define FG_BLACK_P  "\033[30;1m"      // bold black

// Text colors for menu / messages
#define FG_YELLOW   "\033[93m"
#define FG_GREEN    "\033[92m"
#define FG_RED      "\033[91m"
#define FG_CYAN     "\033[96m"
#define FG_MAGENTA  "\033[95m"

class Game {
private:
    // MEMBER 1: Composition -- Game contains a Board
    Board board;

    // MEMBER 1: Track whose turn it is (true = White's turn)
    bool whiteTurn;     // Fixed: was "WhiteTurn" (capital W) -- caused linker mismatch

    // MEMBER 1: Input helpers
    bool isValidInput(const std::string& in) const;
    void parseInput(const std::string& in, int& fr, int& fc, int& tr, int& tc) const;

    // MEMBER 3 (shared): Print appropriate game-over message and return true
    //   if the game has ended (checkmate / stalemate / draw)
    bool checkGameOver() const;

public:
    Game();

    // MEMBER 1: Main game loop
    bool run();
    bool askPlayAgain() const;
};

#endif // GAME_H