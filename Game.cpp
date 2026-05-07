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

    while (true) {
        // MEMBER 1: Show the current board state
        board.display();

        cout << (whiteTurn ? FG_WHITE_P " White" : FG_BLACK_P " Black")
            << RESET FG_CYAN " to move" RESET ": ";
        string input;
        getline(cin, input);

        // MEMBER 1: Quit command
        if (input == "quit" || input == "q") {
            cout << FG_CYAN "Thanks for playing!\n" RESET;
            return false;
        }

        // MEMBER 1: Input format validation
        if (!isValidInput(input)) {
            cout << "Invalid format. Use: e2 e4  (column a-h, row 1-8)\n";
            continue;
        }

        // MEMBER 1: Parse "e2 e4" -> board indices
        int fr, fc, tr, tc;
        parseInput(input, fr, fc, tr, tc);

        // MEMBER 1: Check a piece exists at source
        Piece* p = board.getPiece(fr, fc);
        if (!p) {
            cout << "No piece at that square.\n";
            continue;
        }

        // MEMBER 1: Check the piece belongs to the current player
        if (p->getColor() != whiteTurn) {
            cout << "That is not your piece.\n";
            continue;
        }

        // MEMBER 3: Board::movePiece enforces all move-validation rules:
        //   bounds, geometry, path clear, no friendly-fire, no self-check
        if (!board.movePiece(fr, fc, tr, tc)) {
            cout << "Illegal move. Try again.\n";
            continue;
        }

        // MEMBER 1: Switch turns after a successful move
        whiteTurn = !whiteTurn;

        // MEMBER 3 (shared): Check for checkmate / stalemate / draw
        if (checkGameOver())
            return askPlayAgain();
    }
}

bool Game::askPlayAgain() const {
    string ans;

    while (true) {
        cout << FG_CYAN BOLD "\n  Play again? " RESET
            << FG_GREEN "[y]" RESET "/" FG_RED "[n]" RESET ": ";

        getline(cin, ans);

        if (ans == "y" || ans == "Y")
            return true;
        else if (ans == "n" || ans == "N")
            return false;
        else
            cout << "Invalid input. Please enter only y/Y or n/N.\n";
    }
}
