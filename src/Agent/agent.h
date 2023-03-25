#pragma once


#include <iostream>
#include <cstdlib>
#include <cstdio>  
#include <cassert>
#include <vector>
#include <tuple>
#include <cstdarg>
#include <set>
#include <array>

using Board = std::array<std::array<char, 8>, 8>; // 2D vector of char to represent the game 
using Player = char;  // Type used to represent the players
using Coordinate = std::size_t; // Type used to represent the coordinates on the board
using Move = std::tuple<Coordinate, Coordinate>; // Tuple type to represent a move on the board
using Moves = std::vector<Move>; // Set type to represent a collection of unique moves

extern Board board;
extern Player active_side;
extern Player home_side;
extern Player opposing_side;
extern int verbosity;
extern int no_valid_move;

constexpr std::size_t DIM = 8;


/**

Initializes the game board with the starting position.
The board is represented by a 8x8 vector of characters,
with each cell containing a '-' character to indicate an empty cell,
an 'X' character to indicate a cell occupied by the X player,
or an 'O' character to indicate a cell occupied by the O player.
The starting position has the four central cells occupied by two X and two O pieces.
*/
void initBoard();

/**
 * Determines if a given move is valid for the current game board and player.
 *
 * @param move The move to check, specified as a tuple of (x,y) coordinates.
 * @param player The current player making the move.
 *
 * @return True if the move is valid, false otherwise.
 *
 * A move is considered valid if it satisfies the following conditions:
 *   - The specified coordinates are within the bounds of the game board.
 *   - The specified position is empty.
 *   - The move captures at least one opposing piece in any of the eight directions from the move position.
 */
bool is_valid_move(const Move& move, const Player player);


Board play_move( const Move &move, const Player player );
bool switch_player();
int scoreX();
Moves move_candidates(Player player);
void restore_board(const Board& prevBoard );
bool board_full( Board board );
int alphabeta( int depth, int alpha, int beta, Player current_player );
void msglog(const int log_level, const char* format, ...);
void init_agent(Player home_side_symbol, int verbose=0);
Move search_next_move();