// ============================================================
//  Game.cpp  --  Game class implementation
//  MEMBER 1: Game loop, turn switching, input parsing
//  MEMBER 3 (shared): check/checkmate/stalemate/draw detection
//                     called after every successful move
// ============================================================

#include "Game.h"
#include <iostream>
#include <string>
using namespace std;


// ==============================================================
//  MEMBER 1 -- Constructor
// ==============================================================
Game::Game() : whiteTurn(true) {}    // White always moves first


// ==============================================================
//  MEMBER 1 -- isValidInput
//  Accepts moves in algebraic notation: "e2 e4"
//  Format: [a-h][1-8] [a-h][1-8]  (length must be exactly 5)
// ==============================================================
bool Game::isValidInput(const string& in) const {
    if (in.size() != 5)      return false;
    if (in[2] != ' ')        return false;
    if (in[0] < 'a' || in[0] > 'h') return false;
    if (in[3] < 'a' || in[3] > 'h') return false;
    if (in[1] < '1' || in[1] > '8') return false;
    if (in[4] < '1' || in[4] > '8') return false;
    return true;
}


// ==============================================================
//  MEMBER 1 -- parseInput
//  Convert "e2 e4" into board row/column indices.
//  Columns: 'a'=0 -> 'h'=7
//  Rows:    '8'=0 -> '1'=7  (board is stored top-to-bottom)
// ==============================================================
void Game::parseInput(const string& in,
    int& fr, int& fc,
    int& tr, int& tc) const {
    fc = in[0] - 'a';
    fr = 8 - (in[1] - '0');
    tc = in[3] - 'a';
    tr = 8 - (in[4] - '0');
}


// ==============================================================
//  MEMBER 3 (shared) -- checkGameOver
//  Called after every successful move to detect:
//    1. Checkmate  -- current player is in check and has no move
//    2. Stalemate  -- current player not in check but has no move
//    3. Draw (insufficient material) -- only Kings remain
//  Returns true if the game should end.
// ==============================================================
bool Game::checkGameOver() const {
    // whiteTurn now holds the NEXT player's color (already switched)

    // --- Draw: insufficient material ---
    if (board.isInsufficientMaterial()) {
        cout << FG_YELLOW BOLD "\n====================================\n"
            << "  DRAW - Insufficient material!  \n"
            << "====================================\n" RESET;
        return true;
    }

    // --- Checkmate ---
    if (board.isCheckmate(whiteTurn)) {
        cout << FG_GREEN BOLD "\n====================================\n"
            << "  CHECKMATE!  "
            << (whiteTurn ? "Black" : "White") << " wins!\n"
            << "====================================\n" RESET;
        return true;
    }

    // --- Stalemate ---
    if (board.isStalemate(whiteTurn)) {
        cout << FG_YELLOW BOLD "\n====================================\n"
            << "  STALEMATE - It's a DRAW!       \n"
            << "====================================\n" RESET;
        return true;
    }

    // --- Check (game continues but warn the player) ---
    if (board.isInCheck(whiteTurn)) {
        cout << FG_RED BOLD
            << (whiteTurn ? "White" : "Black")
            << " is in CHECK!\n" RESET;
    }

    return false;   // Game continues
}


// ==============================================================
//  MEMBER 1 -- run
//  Main game loop:
//    1. Display board
//    2. Read and validate input
//    3. Attempt the move (Board enforces all rules -- Member 3)
//    4. Check for end conditions (Member 3 shared)
//    5. Switch turns
// ==============================================================
bool Game::run() {
    cout << BG_MENU FG_YELLOW BOLD;
    cout << "\n============================================\n";
    cout << "          Welcome to C++ Chess!           \n";
    cout << "============================================\n";
    cout << RESET;
    cout << FG_CYAN "  Enter moves as: " FG_GREEN BOLD "e2 e4"
        << RESET FG_CYAN "  (from -> to)\n";
    cout << FG_CYAN "  Type " FG_RED BOLD "'quit'" RESET FG_CYAN " to exit.\n\n" RESET;

  bool Game::run() {
    cout << BG_MENU FG_YELLOW BOLD;
    cout << "\n============================================\n";
    cout << "          Welcome to C++ Chess!           \n";
    cout << "============================================\n";
    cout << RESET;
    cout << FG_CYAN "  Enter moves as: " FG_GREEN BOLD "e2 e4"
        << RESET FG_CYAN "  (from -> to)\n";
    cout << FG_CYAN "  Type " FG_RED BOLD "'quit'" RESET FG_CYAN " to exit.\n\n" RESET;

    // Display once before the loop starts
    board.display();

    while (true) {
        // REMOVED: board.display() from here

        cout << (whiteTurn ? FG_WHITE_P " White" : FG_BLACK_P " Black")
            << RESET FG_CYAN " to move" RESET ": ";
        string input;
        getline(cin, input);

        if (input == "quit" || input == "q") {
            cout << FG_CYAN "Thanks for playing!\n" RESET;
            return false;
        }

        if (!isValidInput(input)) {
            cout << "Invalid format. Use: e2 e4  (column a-h, row 1-8)\n";
            continue;   // re-prompt, NO board redraw
        }

        int fr, fc, tr, tc;
        parseInput(input, fr, fc, tr, tc);

        Piece* p = board.getPiece(fr, fc);
        if (!p) {
            cout << "No piece at that square.\n";
            continue;   // re-prompt, NO board redraw
        }

        if (p->getColor() != whiteTurn) {
            cout << "That is not your piece.\n";
            continue;   // re-prompt, NO board redraw
        }

        if (!board.movePiece(fr, fc, tr, tc)) {
            cout << "Illegal move. Try again.\n";
            continue;   // re-prompt, NO board redraw
        }

        // Move succeeded -- switch turns, THEN redraw
        whiteTurn = !whiteTurn;

        // Redraw only after a valid move
        board.display();

        if (checkGameOver())
            return askPlayAgain();
    }
}
bool Game::askPlayAgain() const {
    cout << FG_CYAN BOLD "\n  Play again? " RESET
        << FG_GREEN "[y]" RESET "/" FG_RED "[n]" RESET ": ";
    string ans;
    getline(cin, ans);
    return (!ans.empty() && (ans[0] == 'y' || ans[0] == 'Y'));
}
