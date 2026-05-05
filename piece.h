#pragma once
// ============================================================
//  piece.h  --  Base class + all derived piece classes
//  MEMBER 2: Piece class hierarchy, inheritance, polymorphism
// ============================================================

#ifndef PIECE_H
#define PIECE_H

#include <cstdlib>   // for abs()

// -------------------------------------------------------
// MEMBER 2: Abstract base class for all chess pieces
// -------------------------------------------------------
class Piece {
protected:
    bool isWhite;   // true = White piece, false = Black piece

public:
    Piece(bool white);

    // Returns true if piece is white
    bool getColor() const;

    // Pure virtual: each piece defines its own movement rules
    // MEMBER 2: isValidMove only checks movement SHAPE (geometry)
    // MEMBER 3: Board::movePiece handles path blocking, captures, check
    virtual bool isValidMove(int startRow, int startCol,
        int endRow, int endCol) const = 0;

    // Returns uppercase symbol; Board::display lowercases for black
    virtual char getSymbol() const = 0;

    virtual ~Piece();
};


// -------------------------------------------------------
// MEMBER 2: Pawn -- moves forward, captures diagonally
// -------------------------------------------------------
class Pawn : public Piece {
public:
    Pawn(bool white);

    // Forward 1 (or 2 from start row), diagonal capture handled by
    // Board::movePiece (Member 3) which checks destination occupancy
    bool isValidMove(int startRow, int startCol,
        int endRow, int endCol) const override;

    char getSymbol() const override;
};


// -------------------------------------------------------
// MEMBER 2: Rook -- moves in straight lines
// -------------------------------------------------------
class Rook : public Piece {
public:
    Rook(bool white);

    bool isValidMove(int startRow, int startCol,
        int endRow, int endCol) const override;

    char getSymbol() const override;
};


// -------------------------------------------------------
// MEMBER 2: Knight -- moves in an L-shape, jumps over pieces
// -------------------------------------------------------
class Knight : public Piece {
public:
    Knight(bool white);

    bool isValidMove(int startRow, int startCol,
        int endRow, int endCol) const override;

    char getSymbol() const override;
};


// -------------------------------------------------------
// MEMBER 2: Bishop -- moves diagonally
// -------------------------------------------------------
class Bishop : public Piece {
public:
    Bishop(bool white);

    bool isValidMove(int startRow, int startCol,
        int endRow, int endCol) const override;

    char getSymbol() const override;
};


// -------------------------------------------------------
// MEMBER 2: Queen -- moves straight or diagonally
// -------------------------------------------------------
class Queen : public Piece {
public:
    Queen(bool white);

    bool isValidMove(int startRow, int startCol,
        int endRow, int endCol) const override;

    char getSymbol() const override;
};


// -------------------------------------------------------
// MEMBER 2: King -- moves one square in any direction
// -------------------------------------------------------
class King : public Piece {
public:
    King(bool white);

    bool isValidMove(int startRow, int startCol,
        int endRow, int endCol) const override;

    char getSymbol() const override;
};

#endif // PIECE_H