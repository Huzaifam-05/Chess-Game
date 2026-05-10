#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <windows.h>
using namespace std;

// Utility
void enableANSI();

// Constants
const int rows = 8;
const int cols = 8;

// Enum for colours
enum Color
{
    WHITE, BLACK
};

// Forward Declaration
class Board;

class Piece
{
    Color color;
    char  symbol;

public:
    Piece(Color c, char s);
    virtual ~Piece();

    char  getSymbol() const;
    Color getColor()  const;

    virtual bool valid(int s_row, int s_col, int d_row, int d_col, Board* board) = 0;
};

class Pawn : public Piece
{
public:
    Pawn(Color c);
    bool valid(int s_row, int s_col, int d_row, int d_col, Board* board) override;
};

class Rook : public Piece
{
public:
    Rook(Color c);
    bool valid(int s_row, int s_col, int d_row, int d_col, Board* board) override;
};

class Knight : public Piece
{
public:
    Knight(Color c);
    bool valid(int s_row, int s_col, int d_row, int d_col, Board* board) override;
};

class Bishop : public Piece
{
public:
    Bishop(Color c);
    bool valid(int s_row, int s_col, int d_row, int d_col, Board* board) override;
};

class Queen : public Piece
{
public:
    Queen(Color c);
    bool valid(int s_row, int s_col, int d_row, int d_col, Board* board) override;
};

class King : public Piece
{
public:
    King(Color c);
    bool valid(int s_row, int s_col, int d_row, int d_col, Board* board) override;
};

class Board
{
    Piece* grid[rows][cols];
    char   whiteCaptured[32];
    char   blackCaptured[32];
    int    whiteCapturedCount;
    int    blackCapturedCount;

public:
    Board();
    ~Board();

    void    initializeEmpty();
    void    setupPiece();

    Piece* getPiece(int r, int c) const;
    void    setPiece(int r, int c, Piece* p);

    bool    validMove(string s);
    Color   getOpponent(Color c);

    bool    isMoveSafe(int s_row, int s_col, int d_row, int d_col, Piece* p);
    void    findKing(Color c, int& kRow, int& kCol);
    bool    isUnderAttack(int row, int col, Color attacker);
    bool    hasValidMoves(Color player);

    void    display(int dangerRow = -1, int dangerCol = -1);
    void    displayCaptured();
    const char* getUnicodePiece(char symbol);

    void    start();
    void    startScreen();
};