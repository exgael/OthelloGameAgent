#pragma once


#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <tuple>

using Board = std::vector<std::vector<char>>;

extern Board board;
extern char player;
extern char moi;
extern char lui;
extern int verbose;
extern int no_valid_move;

void initBoard();
bool isValidMove(int x, int y, char player);
std::vector<std::vector<char>> playMove(int x, int y, char current_player);
bool switchPlayer();
int scoreX();
std::vector< std::tuple<int, int> > listDesCoupsPossible();
void restoreBoard(int x, int y, const std::vector<std::vector<char>>& prevBoard);
bool boardFull(Board board);
bool hasValidMoves(char current_player);
int alphabeta(int depth, int alpha, int beta, char current_player);
