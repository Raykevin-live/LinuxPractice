#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ROW 20
#define COL 20

#define PLAYER1 1
#define PLAYER2 2

#define NEXT 0
#define PLAYER1_WIN 1
#define PLAYER2_WIN 2
#define DRAW		3

enum Dir
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	LEFT_UP,
	LEFT_DOWN,
	RIGHT_UP,
	RIGHT_DOWN
};

void Menu();
void Game();

int IsOver(int board[ROW][COL], int row, int col);
void ShowBoard(int board[ROW][COL], int row, int col);
int ChessCount(int board[ROW][COL], int row, int col, enum Dir d);
void Playermove(int board[ROW][COL], int row, int col, int player);
