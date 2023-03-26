#include "agent.h"
#include <stack>
#include <set>
#include <algorithm>
#include <unordered_map>


/*  LOG UTILITY  */

void msglog(const int log_level, const char* format, ...) {
    if (log_level > verbosity) {
        return;
    }

    char buf[512];
    va_list args;
    va_start(args, format);
    std::vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    std::cerr << buf << std::endl;
}


/*  STRUCT TO STORE MOVE SCORE  */

struct MoveHasher {
    std::size_t operator()(const Move &move) const {
        return std::hash<int>{}(std::get<0>(move)) ^ std::hash<int>{}(std::get<1>(move));
    }
};


/*  CONSTANTES  */


// Board dimentions
constexpr std::size_t DIM = 8;

// Used to determine move score in heuristic function
constexpr int position_values[8][8] = {
    {100, -20,  10,   5,   5,  10, -20, 100},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    { 10,  -2,   1,   1,   1,   1,  -2,  10},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    { 10,  -2,   1,   1,   1,   1,  -2,  10},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    {100, -20,  10,   5,   5,  10, -20, 100}
};

// cell shift
constexpr Move RIGHT =  { 0,  1};
constexpr Move LEFT  =  { 0, -1};
constexpr Move DOWN  =  { 1,  0};
constexpr Move UP    =  {-1,  0};
constexpr Move UL    =  {-1, -1};
constexpr Move UR    =  {-1,  1};
constexpr Move DL    =  { 1, -1};
constexpr Move DR    =  { 1,  1};

// Valid direction to find neighbors based on cell position on board
constexpr std::array<std::array<Move, 8>, 9> DIRECTIONS = {{
    {RIGHT, DOWN, DR},                          // top-left corner
    {LEFT, DOWN, DL},                           // top-right corner
    {RIGHT, LEFT, DOWN, DR, DL},                // top edge
    {RIGHT, DOWN, UP, DR, UR},                  // left edge
    {LEFT, DOWN, UP, UL, DL},                   // right edge
    {UP, RIGHT, LEFT, UL, UR},                  // bottom edge
    {UP, RIGHT, UR},                            // bottom-left corner
    {UP, LEFT, UL},                             // bottom-right corner
    {UP, DOWN, RIGHT, LEFT, UL, UR, DL, DR}     // center
}};

// Lookup table to link board cell to valid neighbors 
constexpr std::array<size_t, 64> DIRECTIONS_TABLE = {
    0, 2, 2, 2, 2, 2, 2, 1,
    3, 8, 8, 8, 8, 8, 8, 4,
    3, 8, 8, 8, 8, 8, 8, 4,
    3, 8, 8, 8, 8, 8, 8, 4,
    3, 8, 8, 8, 8, 8, 8, 4,
    3, 8, 8, 8, 8, 8, 8, 4,
    3, 8, 8, 8, 8, 8, 8, 4,
    6, 5, 5, 5, 5, 5, 5, 7
};

// The number of element at each index of DIRECTIONS
constexpr std::array<size_t, 9> DIRECTIONS_COUNT = {3, 3, 5, 5, 5, 5, 3, 3, 8};


/*  VARIABLES  */

Board board;
Player active_side = 'X';
Player home_side;
Player opposing_side;
int verbosity;


/*  FUNCTION IMPLEMENTATION  */

Player opponent(Player player) {
    return player == 'X' ? 'O' : 'X';
}


void init_agent(Player home_side_symbol, int verbose) 
{
    home_side = home_side_symbol;
    opposing_side = opponent(home_side);
    srand(time(NULL));
    initBoard();
    verbosity = verbose;
}


void initBoard() {   
    for (auto &row : board) {
        row.fill('-');
    }
    board[3][3] = 'X';
    board[3][4] = 'O';
    board[4][3] = 'O';
    board[4][4] = 'X';
}


bool is_valid_move(const Board &b, const Move &move, const Player player) 
{
    const Coordinate x = std::get<0>(move);
    const Coordinate y = std::get<1>(move);

    if (x >= DIM || y >= DIM || b[x][y] != '-') {
        return false;
    }
    
    const int index = DIRECTIONS_TABLE[x * 8 + y];
    const auto &directions = DIRECTIONS[index];
    size_t num_directions = DIRECTIONS_COUNT[index];
    
    for (size_t l = 0; l < num_directions; ++l) {
        const auto &dir = directions[l];
        int i = std::get<0>(dir);
        int j = std::get<1>(dir);
        int k = 1;
        while (true) {
            const Coordinate new_x = x + i * k;
            const Coordinate new_y = y + j * k;
            if (new_x < 0 
                || new_x >= DIM 
                || new_y < 0 
                || new_y >= DIM 
                || b[new_x][new_y] == '-') break;
                
            if (b[new_x][new_y] == player) {
                if (k > 1) { return true; }
                break;
            }
            k++;
        }
    }
    return false;
}


Board play_move(const Board &b, const Move &move, const Player player) {
    assert (is_valid_move(b, move, player) );
    Board new_board = b;
    int x = std::get<0>(move);
    int y = std::get<1>(move);
    new_board[x][y] = player;

    const int index = DIRECTIONS_TABLE[x * 8 + y];
    const auto &directions = DIRECTIONS[index];
    size_t num_directions = DIRECTIONS_COUNT[index];
    
    for (size_t l = 0; l < num_directions; ++l) {
        const auto &dir = directions[l];
        int i = std::get<0>(dir);
        int j = std::get<1>(dir); 
        int k = 1;
        while (true) {
            int newX = x + i * k;
            int newY = y + j * k;
            if (newX < 0 || newX >= DIM || newY < 0 || newY >= DIM) break;
            if (new_board[newX][newY] == '-') break;
            if (new_board[newX][newY] == player) {
                if (k > 1) {
                    for(int k2 = k-1; k2 > 0; k2--) {
                        int newX = x + i * k2;
                        int newY = y + j * k2;
                        new_board[newX][newY] = player;
                    }
                    break;
                }
                break;
            }
            k++;
        }
    }
    return new_board;
}


bool switch_player(const Board &b) {
    char newPlayer = (active_side == 'X') ? 'O' : 'X';
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (is_valid_move(b, {i, j}, newPlayer)) {
                active_side = newPlayer;  
                msglog(3, "Switch occured"); 
                return true;
            }
        }
    }
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (is_valid_move(b, {i, j}, active_side)) {
                msglog(3, "Active player remain");
                return true;
            }
        }
    }

    msglog(3, "No valid moves for either player");
    return false;
}

bool can_switch_player(const Board &b) {
    char newPlayer = (active_side == 'X') ? 'O' : 'X';
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (is_valid_move(b, {i, j}, newPlayer)) {
                msglog(3, "Switch occured"); 
                return true;
            }
        }
    }
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (is_valid_move(b, {i, j}, active_side)) {
                msglog(3, "Active player remain");
                return true;
            }
        }
    }

    msglog(3, "No valid moves for either player");
    return false;
}


bool board_full(const Board &b) {
    for(auto &line: b) {
        for(auto c: line) {
            if(c == '-') {
                return false;
            }
        }
    }
    return true;
}


Move search_next_move(int depth)
{
    // Call alphabeta and get the best move
    std::pair<int, Move> result = alphabeta(board, depth, -9999, 9999, home_side);

    // Extract the best move from the result
    Move best_move = result.second;

    // Log the score
    msglog(1, "Best move has a score of %d.", result.first);

    return best_move;
}


Moves move_candidates(const Board &b, Player player) {
    Moves candidates;

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (is_valid_move(b, {i, j}, player))
            {
                candidates.push_back({i,j});
            }
        }
    }
    return candidates;
}




int position_score(const Board &b, const Player player) {
    int score = 0;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (b[i][j] == player) {
                score += position_values[i][j];
            } else if (b[i][j] == opponent(player)) {
                score -= position_values[i][j];
            }
        }
    }
    return score;
}


int mobility_score(const Board &b, const Player player) {
    int player_mobility = move_candidates(b, player).size();
    int opponent_mobility = move_candidates(b, opponent(player)).size();

    // Avoid division by zero
    if (player_mobility + opponent_mobility == 0) {
        return 0;
    }

    int mobility_diff = player_mobility - opponent_mobility;
    float mobility_score = static_cast<float>(mobility_diff) / static_cast<float>(player_mobility + opponent_mobility);

    return static_cast<int>(100 * mobility_score);
}


int evaluate_board(const Board &b, const Player player) 
{
    float position_weight = 0.5;
    float mobility_weight = 0.5;

    int position_diff = position_score(b, player) - position_score(b, opponent(player));
    int mobility_diff = mobility_score(b, player) - mobility_score(b, opponent(player));

    return position_weight * position_diff + mobility_weight * mobility_diff;
}


bool compare_moves(
    const Move &move1, const Move &move2,
    const Board &b, 
    const Player player, 
    std::unordered_map<Move, int, MoveHasher> &move_scores) 
{
    if (move_scores.find(move1) == move_scores.end()) {
        Board board_after_move1 = play_move(b, move1, player);
        move_scores[move1] = evaluate_board(board_after_move1, player);
    }

    if (move_scores.find(move2) == move_scores.end()) {
        Board board_after_move2 = play_move(b, move2, player);
        move_scores[move2] = evaluate_board(board_after_move2, player);
    }

    return move_scores[move1] > move_scores[move2];
}

void sort_moves(Moves &moves, const Board &b, const Player player) {
    std::unordered_map<Move, int, MoveHasher> move_scores;
    std::sort(
        moves.begin(), moves.end(), 
        [&b, player, &move_scores](const Move &move1, const Move &move2) {
        return compare_moves(move1, move2, b, player, move_scores);
    });
}




std::pair<int, Move> alphabeta(Board &b, int depth, int alpha, int beta, Player player)
{
    if (depth == 0 || board_full(b))
    {
        return {evaluate_board(b, player), {-999, -999}};
    }

    Moves moves = move_candidates(b, player);
    sort_moves(moves, b, player);

    if (moves.empty())
    {
        if (can_switch_player(b))
        {
            return alphabeta(b, depth - 1, alpha, beta, opponent(player));
        }
        else
        {
            return {evaluate_board(b, player), {-1, -1}};
        }
    }

    Move best_move = {-1, -1};
    if (player == home_side)
    {
        int max_eval = -9999;
        int counter = 0;
        for (const Move &move : moves)
        {
            counter  += 1;
            Board new_board = play_move(b, move, player);
            std::pair<int, Move> result = alphabeta(new_board, depth - 1, alpha, beta, opponent(player));
            int eval = result.first;
            if (eval > max_eval)
            {
                max_eval = eval;
                best_move = move;
            }
            alpha = std::max(alpha, eval);
            if (beta <= alpha) 
            { 
                msglog(2, "Pruned at counter %d, total moves %d", counter, moves.size());
                break;
            }
        }
        return {max_eval, best_move};
    }
    else // player to minimize
    {
        int min_eval = 9999;
        int counter = 0;
        for (const Move &move : moves)
        {
            counter  += 1;
            Board new_board = play_move(b, move, player);
            std::pair<int, Move> result = alphabeta(new_board, depth - 1, alpha, beta, opponent(player));
            int eval = result.first;
            if (eval < min_eval)
            {
                min_eval = eval;
                best_move = move;
            }
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                msglog(2, "Pruned at counter %d, total moves %d", counter, moves.size());
                break;
            }
        }
        return {min_eval, best_move};
    }
}

