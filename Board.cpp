// ============================================================
//  Board.cpp  --  Chess board implementation
//  MEMBER 1: Board setup, display, piece access
//  MEMBER 3: Move validation, path checking, check/checkmate/
//            stalemate, draw by insufficient material
// ============================================================

#include "Board.h"
#include <iostream>
#include <cctype>    // tolower

// ===== COLORS (FINAL FIX) =====

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

using namespace std;


// ==============================================================
//  MEMBER 1 -- Board construction / destruction
// ==============================================================

Board::Board() {
    // Null-initialise every cell before placing pieces
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            board[r][c] = nullptr;

    initializeBoard();
}

Board::~Board() {
    // Free every piece to prevent memory leaks
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            delete board[r][c];
            board[r][c] = nullptr;
        }
    }
}


// ==============================================================
//  MEMBER 1 -- initializeBoard
//  Place all 32 pieces in their standard starting positions.
//  Fixed bugs from original:
//    - Rook spelling was "Rock" -> corrected to Rook
//    - Queen/King column positions were swapped -> fixed
//    - Pawn loops used wrong loop variable -> fixed (c < 8)
// ==============================================================
void Board::initializeBoard() {
    // --- Row 0: Black back rank ---
    board[0][0] = new Rook(false);
    board[0][1] = new Knight(false);
    board[0][2] = new Bishop(false);
    board[0][3] = new Queen(false);   // Queen on d-file (col 3)
    board[0][4] = new King(false);    // King on e-file (col 4)
    board[0][5] = new Bishop(false);
    board[0][6] = new Knight(false);
    board[0][7] = new Rook(false);

    // --- Row 1: Black pawns ---
    for (int c = 0; c < 8; c++)      // Fixed: was "c < 7" and incremented r
        board[1][c] = new Pawn(false);

    // --- Rows 2-5: Empty ---
    // (already nullptr from constructor)

    // --- Row 6: White pawns ---
    for (int c = 0; c < 8; c++)      // Fixed: was "c < 7" and incremented r
        board[6][c] = new Pawn(true);

    // --- Row 7: White back rank ---
    board[7][0] = new Rook(true);
    board[7][1] = new Knight(true);
    board[7][2] = new Bishop(true);
    board[7][3] = new Queen(true);   // Queen on d-file (col 3)
    board[7][4] = new King(true);    // King on e-file (col 4)
    board[7][5] = new Bishop(true);
    board[7][6] = new Knight(true);
    board[7][7] = new Rook(true);
}


// ==============================================================
//  MEMBER 1 -- display
//  Print the board with column letters and row numbers.
//  Uppercase = White piece, lowercase = Black piece, '.' = empty
//  Uses plain ASCII '+', '-', '|' borders to avoid Unicode issues.
// ==============================================================
void Board::display() const {
    cout << "\n";
    cout << FG_CYAN BOLD "   a  b  c  d  e  f  g  h\n" RESET;
   

    for (int r = 0; r < 8; r++) {
        cout << FG_CYAN BOLD << (8 - r) << " " RESET;

        for (int c = 0; c < 8; c++) {
            // Checkerboard: light square when (r+c) is even
            bool lightSquare = (r + c) % 2 == 0;
            string bg = lightSquare ? BG_LIGHT : BG_DARK;

            if (board[r][c] == nullptr) {
                cout << bg << "   " << RESET;
            }
            else {
                char sym = board[r][c]->getSymbol();
                bool pieceIsWhite = board[r][c]->getColor();
                string fg = pieceIsWhite ? FG_WHITE_P : FG_BLACK_P;
                if (!pieceIsWhite) sym = (char)tolower(sym);
                cout << bg << fg << " " << sym << " " << RESET;
            }
        }

        cout << FG_CYAN BOLD " " << (8 - r) << "\n" RESET;

      
    }

   
    cout << FG_CYAN BOLD "   a  b  c  d  e  f  g  h\n\n" RESET;
}

// ==============================================================
//  MEMBER 1 -- Accessors
// ==============================================================
Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row > 7 || col < 0 || col > 7) return nullptr;
    return board[row][col];
}

void Board::setPiece(int row, int col, Piece* p) {
    if (row < 0 || row > 7 || col < 0 || col > 7) return;
    board[row][col] = p;
}


// ==============================================================
//  MEMBER 3 -- isPathClear
//  Walk every square between source and destination.
//  Returns true if no piece blocks the path.
//  Knights are exempt (they jump).
// ==============================================================
bool Board::isPathClear(int fr, int fc, int tr, int tc) const {
    Piece* mover = board[fr][fc];

    // Knights jump over pieces -- path is always clear
    if (mover) {
        char sym = mover->getSymbol();
        if (sym == 'N' || sym == 'n') return true;
    }

    // Determine step direction along each axis
    int rowStep = 0, colStep = 0;
    if (tr > fr) rowStep = 1;
    else if (tr < fr) rowStep = -1;

    if (tc > fc) colStep = 1;
    else if (tc < fc) colStep = -1;

    // Walk from one step past source up to (but NOT including) destination
    int r = fr + rowStep;
    int c = fc + colStep;

    while (r != tr || c != tc) {
        if (board[r][c] != nullptr)
            return false;   // A piece is blocking the path
        r += rowStep;
        c += colStep;
    }
    return true;
}


// ==============================================================
//  MEMBER 3 -- findKing
//  Scan the board for the King of the given color.
//  Stores position in kingRow/kingCol; returns false if not found.
// ==============================================================
bool Board::findKing(bool white, int& kingRow, int& kingCol) const {
    char target = white ? 'K' : 'k';
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board[r][c] && board[r][c]->getSymbol() == target) {
                kingRow = r;
                kingCol = c;
                return true;
            }
        }
    }
    return false;  // King not on board (should not happen in a legal game)
}


// ==============================================================
//  MEMBER 3 -- isInCheck
//  Returns true if the given color's King can be captured by
//  any enemy piece on the very next move.
// ==============================================================
bool Board::isInCheck(bool white) const {
    int kingRow, kingCol;
    if (!findKing(white, kingRow, kingCol)) return false;

    // Scan every enemy piece and see if it threatens the King's square
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* p = board[r][c];
            if (!p || p->getColor() == white) continue;  // skip empty / own pieces

            // Polymorphism: each piece checks its own movement rules
            if (p->isValidMove(r, c, kingRow, kingCol) &&
                isPathClear(r, c, kingRow, kingCol)) {
                return true;  // King is under attack
            }
        }
    }
    return false;
}


// ==============================================================
//  MEMBER 3 -- simulateMove
//  Temporarily apply a move on the board, test whether the
//  given color's King is still in check, then undo the move.
//  Returns true if after the move the King is NOT in check
//  (i.e. the move is safe).
// ==============================================================
bool Board::simulateMove(int fr, int fc, int tr, int tc, bool white) const {
    // Save board state
    Piece* moving = board[fr][fc];
    Piece* captured = board[tr][tc];

    // Apply move temporarily (cast away const for simulation)
    Board* self = const_cast<Board*>(this);
    self->board[tr][tc] = moving;
    self->board[fr][fc] = nullptr;

    bool safe = !isInCheck(white);

    // Undo the move
    self->board[fr][fc] = moving;
    self->board[tr][tc] = captured;

    return safe;
}


// ==============================================================
//  MEMBER 3 -- isLegalMove
//  A fully legal move must:
//    1. Have a piece at source
//    2. Pass the piece's own movement geometry check
//    3. Have a clear path (except Knights)
//    4. Not capture a friendly piece
//    5. Not leave the mover's own King in check
//  This is the central rule-enforcement function.
// ==============================================================
bool Board::isLegalMove(int fr, int fc, int tr, int tc) const {
    // Bounds check
    if (fr < 0 || fr > 7 || fc < 0 || fc > 7) return false;
    if (tr < 0 || tr > 7 || tc < 0 || tc > 7) return false;
    if (fr == tr && fc == tc) return false;   // must actually move

    Piece* moving = board[fr][fc];
    if (!moving) return false;

    bool white = moving->getColor();

    // Cannot capture own piece
    Piece* target = board[tr][tc];
    if (target && target->getColor() == white) return false;

    // Pawn diagonal capture: only legal if an enemy is actually there
    char sym = moving->getSymbol();
    if (sym == 'P' || sym == 'p') {
        int dc = tc - fc;
        if (dc != 0 && !target) return false; // diagonal with no enemy
        if (dc == 0 && target)  return false; // forward into occupied square
    }

    // Piece geometry check (Member 2's logic via polymorphism)
    if (!moving->isValidMove(fr, fc, tr, tc)) return false;

    // Path must be clear (Member 3)
    if (!isPathClear(fr, fc, tr, tc)) return false;

    // Move must not leave own King in check (Member 3)
    if (!simulateMove(fr, fc, tr, tc, white)) return false;

    return true;
}


// ==============================================================
//  MEMBER 3 -- hasAnyLegalMove
//  Try every possible move for the given color.
//  Returns true the moment one legal move is found.
// ==============================================================
bool Board::hasAnyLegalMove(bool white) const {
    for (int fr = 0; fr < 8; fr++) {
        for (int fc = 0; fc < 8; fc++) {
            Piece* p = board[fr][fc];
            if (!p || p->getColor() != white) continue;

            // Try every destination square
            for (int tr = 0; tr < 8; tr++) {
                for (int tc = 0; tc < 8; tc++) {
                    if (isLegalMove(fr, fc, tr, tc))
                        return true;
                }
            }
        }
    }
    return false;   // No legal move found for this color
}


// ==============================================================
//  MEMBER 3 -- movePiece  (public)
//  Execute a move if it passes all legality checks.
//  Returns true on success, false if the move is illegal.
// ==============================================================
bool Board::movePiece(int fr, int fc, int tr, int tc) {
    if (!isLegalMove(fr, fc, tr, tc))
        return false;

    // Capture: delete enemy piece if present
    delete board[tr][tc];

    // Move the piece pointer
    board[tr][tc] = board[fr][fc];
    board[fr][fc] = nullptr;

    return true;
}


// ==============================================================
//  MEMBER 3 -- isCheckmate
//  Checkmate = in check AND no legal move exists
// ==============================================================
bool Board::isCheckmate(bool white) const {
    if (!isInCheck(white)) return false;     // Must be in check first
    return !hasAnyLegalMove(white);          // And have no escape
}


// ==============================================================
//  MEMBER 3 -- isStalemate
//  Stalemate = NOT in check AND no legal move exists -> draw
// ==============================================================
bool Board::isStalemate(bool white) const {
    if (isInCheck(white)) return false;      // Must NOT be in check
    return !hasAnyLegalMove(white);          // But must have no move
}


// ==============================================================
//  MEMBER 3 -- isInsufficientMaterial
//  Draw condition: only Kings remain on the board.
//  (Extended versions would also catch K+B vs K, K+N vs K, etc.)
// ==============================================================
bool Board::isInsufficientMaterial() const {
    int pieceCount = 0;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board[r][c] != nullptr)
                pieceCount++;
        }
    }
    // Only two Kings left -- neither side can deliver checkmate
    return (pieceCount == 2);
}