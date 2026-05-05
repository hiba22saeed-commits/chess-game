#pragma once
// ============================================================
//  Board.h  --  Chess board class declaration
//  MEMBER 1: Board structure, display, piece placement, turn flow
//  MEMBER 3: Move validation, check/checkmate/stalemate/draw
// ============================================================

#ifndef BOARD_H
#define BOARD_H

#include "piece.h"


class Board {
private:
    // MEMBER 1: 8x8 grid -- composition (Board contains Pieces)
    Piece* board[8][8];

    // -------------------------------------------------------
    // MEMBER 3: Private helpers for move validation & game rules
    // -------------------------------------------------------

    // Returns true if no piece stands between (fr,fc) and (tr,tc)
    // Knights are exempt -- they jump over pieces
    bool isPathClear(int fr, int fc, int tr, int tc) const;

    // Simulates a move on the internal board, calls callback, then undoes it
    // Used by isInCheck, isCheckmate, isStalemate without allocating memory
    bool simulateMove(int fr, int fc, int tr, int tc, bool white) const;

    // Finds the King of the given color; returns false if not found
    bool findKing(bool white, int& kingRow, int& kingCol) const;

    // Returns true if the given color has at least one legal move available
    bool hasAnyLegalMove(bool white) const;

    // Returns true if the given color can legally move piece from (fr,fc)
    // to (tr,tc) -- geometry + path + does not leave own king in check
    bool isLegalMove(int fr, int fc, int tr, int tc) const;

public:
    // MEMBER 1: Constructor / Destructor
    Board();
    ~Board();

    // MEMBER 1: Place all pieces in starting positions
    void initializeBoard();

    // MEMBER 1: Print the board to console
    void display() const;

    // MEMBER 1: Accessors used by Game
    Piece* getPiece(int row, int col) const;
    void   setPiece(int row, int col, Piece* p);

    // -------------------------------------------------------
    // MEMBER 3: Public move execution -- enforces ALL rules
    // Returns true if the move was legal and was executed
    // -------------------------------------------------------
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    // MEMBER 3: Returns true if the given color's King is in check
    bool isInCheck(bool white) const;

    // MEMBER 3: Returns true if the given color is in checkmate
    //   (in check AND has no legal move)
    bool isCheckmate(bool white) const;

    // MEMBER 3: Returns true if the given color is in stalemate
    //   (NOT in check but has no legal move)
    bool isStalemate(bool white) const;

    // MEMBER 3: Returns true if only the two Kings remain (dead position)
    bool isInsufficientMaterial() const;
};

#endif // BOARD_H