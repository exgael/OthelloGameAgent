#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdio>  
#include <cassert>
#include <vector>
#include <tuple>
#include <cstdarg>
#include <array>


// 2D array of char to represent the game
using Board         = std::array<std::array<char, 8>, 8>;

// Type used to represent the players
using Player        = char; 

// Type used to represent the coordinates on the board
using Coordinate    = std::size_t; 

// Tuple type to represent a move on the board
using Move          = std::tuple<Coordinate, Coordinate>;

// Set type to represent a collection of unique moves
using Moves         = std::vector<Move>; 


extern Board        board;         // The game board represented as a 2D grid.
extern Player       active_side;   // The active player's symbol ('X' or 'O').
extern Player       home_side;     // The home player's symbol ('X' or 'O').
extern Player       opposing_side; // The opposing player's symbol ('X' or 'O').
extern int          verbosity;     // The verbosity level for logging and debugging.


void msglog( const int log_level, const char* format, ... );

/**
 * @brief Initializes the game agent with the specified player symbol and verbosity level.
 * 
 * This function sets up the game agent by initializing the home and opposing side symbols,
 * seeding the random number generator, initializing the game board, and setting the verbosity level.
 *
 * @param home_side_symbol The symbol for the agent's side ('X' or 'O').
 * @param verbose The verbosity level for logging and debugging purposes.
 */
void init_agent( Player home_side_symbol, int verbose=0 );

/**
Initializes the game board with the starting position.
The board is represented by a 8x8 array of characters,
with each cell containing a '-' character to indicate an empty cell,
an 'X' character to indicate a cell occupied by the X player,
or an 'O' character to indicate a cell occupied by the O player.
The starting position has the four central cells occupied by two X and two O pieces.
*/
void initBoard(  );

/**
 * @brief Returns the opponent of the given player.
 * 
 * This function returns the opposing player based on the input player.
 * If the input player is 'X', the function returns 'O', and vice versa.
 *
 * @param player The input player ('X' or 'O').
 * @return Player The opponent of the input player.
 */
Player opponent( Player player );

/**
 * @brief Determines if the active player should be switched or remains the same.
 * 
 * The function checks if the new player has any valid moves. 
 *   - If yes, the active player is switched.
 *   - If the new player has no valid moves, the function checks if the current active player has any valid moves.
 *   - If neither player has any valid moves, the game is considered over.
 *
 * @param b The board state.
 * @return true If the active player is switched or remains the same due to valid moves available.
 * @return false If there are no valid moves for either player, indicating the game is over.
 */
bool switch_player( const Board &b );

Move search_next_move( int depth );

std::pair<int, Move> alphabeta(Board &b, int depth, int alpha, int beta, Player current_player );

/**
 * @brief Determines if a given move is valid for the passed game board and player.
 * 
 * A move is considered valid if it satisfies the following conditions:
 * - The specified coordinates are within the bounds of the game board.
 * - The specified position is empty.
 * - The move captures at least one opposing piece in any of the eight directions from the move position.
 *
 * @param b The board state.
 * @param move The move coordinates to be checked for validity.
 * @param player The player making the move ('X' or 'O').
 * @return true If the move is valid.
 * @return false If the move is invalid.
 */
bool is_valid_move( const Board &b, const Move &move, const Player player );

/**
 * @brief Generates a list of valid moves for the given player on the current board state.
 * 
 * The function iterates through all the positions on the board and checks for valid moves
 * for the specified player. It collects all valid moves and returns them as a vector.
 *
 * @param b The current board state.
 * @param player The player for whom valid moves are to be determined ('X' or 'O').
 * @return Moves A list of valid moves for the specified player on the current board state.
 */
Moves move_candidates( const Board &b, Player player );

/**
 * @brief Plays a move on the board and updates the board state accordingly.
 * 
 * The function assumes the move is valid and asserts the validity before proceeding.
 * It updates the board by placing the player's symbol at the specified move position and
 * capturing any opponent's pieces in line with the move in any of the eight directions.
 *
 * @param b The current board state.
 * @param move The move coordinates to be played.
 * @param player The player making the move ('X' or 'O').
 * @return Board The updated board state after playing the move.
 */
Board play_move( const Board &b, const Move &move, const Player player );

/**
 * @brief Checks if the game board is full (i.e., no empty cells left).
 * 
 * Iterates through the board cells to determine if any cells are empty.
 * If an empty cell is found, the board is considered not full, and the function returns false.
 * If no empty cells are found, the board is considered full, and the function returns true.
 *
 * @param b The current board state.
 * @return true If the board is full.
 * @return false If there are empty cells in the board.
 */
bool board_full( const Board &b );


int evaluate_board(const Board &b, const Player player);