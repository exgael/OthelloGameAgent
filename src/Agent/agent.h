#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>
#include <tuple>
#include <cstdarg>
#include <array>
#include <unordered_map>

// 2D array of char to represent the game
using Board = std::array<std::array<char, 8>, 8>;

// Type used to represent the players
using Player = char;

// Type used to represent the coordinates on the board
using Coordinate = std::size_t;

// Tuple type to represent a move on the board
using Move = std::tuple<Coordinate, Coordinate>;

// Set type to represent a collection of unique moves
using Moves = std::vector<Move>;

extern Board board;          // The game board represented as a 2D grid.
extern Player active_side;   // The active player's symbol ('X' or 'O').
extern Player home_side;     // The home player's symbol ('X' or 'O').
extern Player opposing_side; // The opposing player's symbol ('X' or 'O').
extern int verbosity;        // The verbosity level for logging and debugging.


/*  STRUCT TO HASH MOVE  */

struct hash_move
{
    std::size_t operator()(const Move &move) const
    {
        // combine both value via XOR
        return std::hash<int>{}(std::get<0>(move)) ^ std::hash<int>{}(std::get<1>(move));
    }
};

void msglog(const int log_level, const char *format, ...);

/**
 * @brief Initializes the game agent with the specified player symbol and verbosity level.
 * @param home_side_symbol The symbol for the agent's side ('X' or 'O').
 * @param verbose The verbosity level for logging and debugging purposes.
 */
void init_agent(Player home_side_symbol, int verbose = 0);

/**
 * @brief Init the game board with the starting position.
 */
void initBoard();

/**
 * @brief Returns the opponent of the given player.
 * @param player The input player ('X' or 'O').
 * @return Player The opponent of the input player.
 */
Player opponent(Player player);

/**
 * @brief Determines if the active player should be switched or remains the same.
 * @param b The board state.
 * @return true If the active player is switched or remains the same due to valid moves available.
 * @return false If there are no valid moves for either player, indicating the game is over.
 */
bool switch_player(const Board &b);

/**
 * @brief Search the best move to play for the current player using the alphabeta algorithm with a given depth.
 * @param depth The depth to explore the game tree.
 * @return The best move found.
 */
Move search_next_move(int depth);

/**
 * @brief function compares two moves by their score.
 * @param move1 The first move to be compared.
 * @param move2 The second move to be compared.
 * @param b The board.
 * @param player The player
 * @param move_scores The unordered_map of previously calculated move scores.
 * @return Returns true if move1 has a higher score than move2, false otherwise.
 */
bool compare_moves(
    const Move &move1, const Move &move2,
    const Board &b,
    const Player player,
    std::unordered_map<Move, int, hash_move> &move_scores);

/**
 * @brief Sorts a vector of moves based on their scores in the given board state and for the specified player.
 * @param moves A reference to a vector of moves to be sorted.
 * @param b The current board state.
 * @param player The player for whom the scores are calculated.
 */
void sort_moves(Moves &moves, const Board &b, const Player player);

/**
 * @brief Applies the alpha-beta pruning algorithm to recursively evaluate the best move for the given player up to the specified depth.
 * @param b The current board state.
 * @param depth The search depth.
 * @param alpha The best evaluation score found so far for the maximizing player.
 * @param beta The best evaluation score found so far for the minimizing player.
 * @param player The current player for whom to find the best move.
 * @return A pair with the evaluation score of the best move found and the move itself.
 */
std::pair<int, Move> alphabeta(Board &b, int depth, int alpha, int beta, Player current_player);

/**
 * @brief Determines if a given move is valid for the passed game board and player.
 * @param b The board state.
 * @param move The move coordinates to be checked for validity.
 * @param player The player making the move ('X' or 'O').
 * @return true If the move is valid.
 * @return false If the move is invalid.
 */
bool is_valid_move(const Board &b, const Move &move, const Player player);

/**
 * @brief Eavluate a list of valid moves for the given player on the current board state.
 * @param b The given board state.
 * @param player The player for whom valid moves are to be determined ('X' or 'O').
 * @return Moves A list of valid moves for the specified player on the current board state.
 */
Moves move_candidates(const Board &b, Player player);

/**
 * @brief Plays a move on the board and updates the board state accordingly.
 * @param b The current board state.
 * @param move The move coordinates to be played.
 * @param player The player making the move ('X' or 'O').
 * @return Board The updated board state after playing the move.
 */
Board play_move(const Board &b, const Move &move, const Player player);

/**
 * @brief Checks if the game board is full (i.e., no empty cells left).
 * @param b The current board state.
 * @return true If the board is full.
 * @return false If there are empty cells in the board.
 */
bool board_full(const Board &b);

/**
 * @brief function calculates a score for a given board position based on the position weight and mobility weight.
 * @param b: the board object to evaluate
 * @param player: the player for whom to evaluate the score
 * @return an integer score for the given board position
 */
int evaluate_board(const Board &b, const Player player);