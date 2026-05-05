// ============================================================
//  piece.cpp  --  Implementation of all chess piece classes
//  MEMBER 2: Movement rules for each piece (geometry only)
//  MEMBER 3 (shared contribution): Pawn diagonal capture logic
// ============================================================

#include "piece.h"
#include <cstdlib>   // abs()

// -------------------------------------------------------
// MEMBER 2: Base class Piece
// -------------------------------------------------------
Piece::Piece(bool white) : isWhite(white) {}

bool Piece::getColor() const {
    return isWhite;
}

Piece::~Piece() {}


// -------------------------------------------------------
// MEMBER 2: Pawn movement (forward)
// MEMBER 3 (shared): Added diagonal capture geometry so
//   Board::movePiece can correctly allow captures
// -------------------------------------------------------
Pawn::Pawn(bool white) : Piece(white) {}

bool Pawn::isValidMove(int sr, int sc, int er, int ec) const {
    int dc = ec - sc;
    int dcAbs = (dc < 0) ? -dc : dc;

    if (isWhite) {
        // --- Forward move (no column change) ---
        if (dc == 0) {
            if (sr - 1 == er) return true;          // one step forward
            if (sr == 6 && sr - 2 == er) return true; // two steps from start
        }
        // --- MEMBER 3 (shared): Diagonal capture geometry ---
        // Board::movePiece verifies an enemy piece is actually there
        if (sr - 1 == er && dcAbs == 1) return true;
    }
    else {
        // --- Forward move for black ---
        if (dc == 0) {
            if (sr + 1 == er) return true;
            if (sr == 1 && sr + 2 == er) return true;
        }
        // --- MEMBER 3 (shared): Diagonal capture geometry ---
        if (sr + 1 == er && dcAbs == 1) return true;
    }
    return false;
}

char Pawn::getSymbol() const {
    return isWhite ? 'P' : 'p';
}


// -------------------------------------------------------
// MEMBER 2: Rook -- same row OR same column
// -------------------------------------------------------
Rook::Rook(bool white) : Piece(white) {}

bool Rook::isValidMove(int sr, int sc, int er, int ec) const {
    if (sr == er && sc == ec) return false; // must actually move
    return (sr == er || sc == ec);
}

char Rook::getSymbol() const {
    return isWhite ? 'R' : 'r';
}


// -------------------------------------------------------
// MEMBER 2: Knight -- L-shape (2+1 squares)
// -------------------------------------------------------
Knight::Knight(bool white) : Piece(white) {}

bool Knight::isValidMove(int sr, int sc, int er, int ec) const {
    int dr = sr - er;
    int dc = sc - ec;
    if (dr < 0) dr = -dr;
    if (dc < 0) dc = -dc;
    return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
}

char Knight::getSymbol() const {
    return isWhite ? 'N' : 'n';
}


// -------------------------------------------------------
// MEMBER 2: Bishop -- equal row and column distance
// -------------------------------------------------------
Bishop::Bishop(bool white) : Piece(white) {}

bool Bishop::isValidMove(int sr, int sc, int er, int ec) const {
    int dr = sr - er;
    int dc = sc - ec;
    if (dr < 0) dr = -dr;
    if (dc < 0) dc = -dc;
    if (dr == 0) return false; // must actually move
    return dr == dc;
}

char Bishop::getSymbol() const {
    return isWhite ? 'B' : 'b';
}


// -------------------------------------------------------
// MEMBER 2: Queen -- straight line OR diagonal
// -------------------------------------------------------
Queen::Queen(bool white) : Piece(white) {}

bool Queen::isValidMove(int sr, int sc, int er, int ec) const {
    if (sr == er && sc == ec) return false;
    int dr = sr - er;
    int dc = sc - ec;
    if (dr < 0) dr = -dr;
    if (dc < 0) dc = -dc;
    return (sr == er || sc == ec || dr == dc);
}

char Queen::getSymbol() const {
    return isWhite ? 'Q' : 'q';
}


// -------------------------------------------------------
// MEMBER 2: King -- one square in any direction
// -------------------------------------------------------
King::King(bool white) : Piece(white) {}

bool King::isValidMove(int sr, int sc, int er, int ec) const {
    if (sr == er && sc == ec) return false;
    int dr = sr - er;
    int dc = sc - ec;
    if (dr < 0) dr = -dr;
    if (dc < 0) dc = -dc;
    return (dr <= 1 && dc <= 1);
}

char King::getSymbol() const {
    return isWhite ? 'K' : 'k';
}