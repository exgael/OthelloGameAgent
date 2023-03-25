#include "agent.h"


/*  CONSTANTES  */

constexpr std::size_t DIM = 8;

/*  VARIABLES  */
Board board;
Player active_side = 'X';
Player home_side;
Player opposing_side;
int verbosity;

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



void init_agent(Player home_side_symbol, int verbose) 
{
    home_side = home_side_symbol;
    opposing_side = home_side == 'X' ? 'O' : 'X';
    srand(time(NULL));
    initBoard();
    verbosity = verbose;
}

void initBoard() {   
    board.resize(8, std::vector<char>(8, '-'));
    board[3][3] = 'X';
    board[3][4] = 'O';
    board[4][3] = 'O';
    board[4][4] = 'X';
}


const std::array<Moves, 9> DIRECTIONS = {{
    {{0, 1}, {1, 0}, {1, 1}},                     // top-left corner
    {{0, -1}, {1, -1}, {1, 0}},                    // top-right corner
    {{0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}},    // top edge
    {{-1, 0}, {-1, 1}, {0, 1}},                    // left edge
    {{-1, -1}, {-1, 0}, {0, -1}},                  // right edge
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}}, // bottom edge
    {{-1, 0}, {-1, 1}, {0, 1}, {1, 0}, {1, 1}},    // bottom-left corner
    {{-1, -1}, {-1, 0}, {0, -1}, {1, -1}, {1, 0}}, // bottom-right corner
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}} // center
}};

const std::array<std::array<int, 8>, 8> DIRECTIONS_TABLE = {{
    {{0, 2, 2, 2, 2, 2, 2, 1}},
    {{3, 8, 8, 8, 8, 8, 8, 4}},
    {{3, 8, 8, 8, 8, 8, 8, 4}},
    {{3, 8, 8, 8, 8, 8, 8, 4}},
    {{3, 8, 8, 8, 8, 8, 8, 4}},
    {{3, 8, 8, 8, 8, 8, 8, 4}},
    {{3, 8, 8, 8, 8, 8, 8, 4}},
    {{6, 5, 5, 5, 5, 5, 5, 7}}
}};


bool is_valid_move(const Move &move, const Player player) 
{
    const Coordinate x = std::get<0>(move);
    const Coordinate y = std::get<1>(move);

    if (x >= DIM || y >= DIM || board[x][y] != '-') {
        return false;
    }
    
    const int index = DIRECTIONS_TABLE[x][y];
    const auto &directions = DIRECTIONS[index];
    
    for (const auto &dir : directions) {
        int i = std::get<0>(dir);
        int j = std::get<1>(dir);
        int k = 1;
        while (true) {
            const Coordinate new_x = x + i * k;
            const Coordinate new_y = y + j * k;
            if (new_x < 0 || new_x >= DIM || new_y < 0 || new_y >= DIM) break;
            if (board[new_x][new_y] == '-') break;
            if (board[new_x][new_y] == player) {
                if (k > 1) { return true; }
                break;
            }
            k++;
        }
    }
    return false;
}


Board play_move(const Move &move, const Player player) {
    Board prevBoard = board;
    int x = std::get<0>(move);
    int y = std::get<1>(move);
    board[x][y] = player;

    const int index = DIRECTIONS_TABLE[x][y];
    const auto &directions = DIRECTIONS[index];
    
    for (const auto &dir : directions) {
        int i = std::get<0>(dir);
        int j = std::get<1>(dir); 
        int k = 1;
        while (true) {
            int newX = x + i * k;
            int newY = y + j * k;
            if (newX < 0 || newX >= DIM || newY < 0 || newY >= DIM) break;
            if (board[newX][newY] == '-') break;
            if (board[newX][newY] == player) {
                if (k > 1) {
                    for(int k2 = k-1; k2 > 0; k2--) {
                        int newX = x + i * k2;
                        int newY = y + j * k2;
                        board[newX][newY] = player;
                    }
                    break;
                }
                break;
            }
            k++;
        }
    }
    return prevBoard;
}


bool switch_player() {
    char newPlayer = (active_side == 'X') ? 'O' : 'X';
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (is_valid_move({i, j}, newPlayer)) {
                active_side = newPlayer;  
                msglog(2, "Switch occured"); 
                return true;
            }
        }
    }
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (is_valid_move({i, j}, active_side)) {
                 msglog(2, "Active player remain");
                return true;
            }
        }
    }

    if ( verbosity ) {
        msglog(2, "No valid moves for either player");
    }

    return false;
}

int scoreX() {
    int X = 0;
    int O = 0;

    for(auto &line: board) {
        for(auto c: line) {
            X += (c == 'X');
            O += (c == 'O');
        }
    }

    return X-O;
}

Moves move_candidates() {
    Moves moves;

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if ( board[i][j] == '-' &&  is_valid_move({i, j}, active_side))
            {
                moves.push_back({i,j});
            }
        }
    }

    return moves;
}


void restore_board(const Board& prevBoard) {
    board = prevBoard;
}


bool board_full(Board board) {
    for(auto &line: board) {
        for(auto c: line) {
            if(c == '-') {
                return false;
            }
        }
    }
    return true;
}


Move search_next_move()
{
    int best_score = -999;
    Move best_move = {-1, -1};
    Moves moves = move_candidates();
    msglog(0, "DEBUG: %d", moves.size());

/*     for (const Move move : moves)
    {
        assert(active_side == home_side);
        Board orginal_board = play_move(move, home_side);

        switch_player();
        assert(active_side == opposing_side);

        int score = alphabeta(4, -100000, 100000, opposing_side);

        restore_board(orginal_board);
        active_side = home_side;

        if (score > best_score)
        {
            best_score = score;
            best_move = move;
        }
    } */

    return moves.at(rand()%moves.size());
}

int alphabeta(int depth, int alpha, int beta, Player curr_player) {
    if (depth == 0 || board_full(board)) {
        if ( verbosity ) {
            if (depth == 0 && board_full(board)) {
                msglog(3, "Zero depth and board full");
            }
            else if (depth == 0 ) {
                 msglog(3, "Zero depth.");
            } else {
                msglog(3, "Board full.");
            }

        }
        return scoreX(); // Evaluate the board state when reaching the specified depth or no moves left
    }

    if (curr_player == home_side) {
        int maxEval = -100000; // Set to a very small value

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                if (is_valid_move({i, j}, home_side)) {
                    msglog(3, "Alpha beta maximizing part.");
                    msglog(3, "%c move : %d %d", active_side, i, j);


                    std::vector<std::vector<char>> prevBoard = play_move({i, j}, home_side);

                    char who_played = home_side;
                    

                     msglog(3, "Alpha beta prepare minimizing. Player switch.");


                    bool can_switch = switch_player();

                    if (! can_switch) {
                        return scoreX();
                    }

 
                    msglog(3, "After switchPlayer(): active side is: %c", active_side);

                    // If the other player cannot play, keep the current player
                    if (who_played == active_side) {
                        return scoreX();
                    }
                   
                    if (active_side != opposing_side ) {
                        msglog(3, "activate side i0s %c, should be %c", active_side, opposing_side);

                        assert( active_side == opposing_side );
                    }
                    

                    int eval = alphabeta(depth - 1, alpha, beta, opposing_side);


                    msglog(3, "Move : %d %d, Eval : %d", i, j, eval);

                  
                    restore_board(prevBoard);

                    active_side = who_played;


                    maxEval = std::max(maxEval, eval);
                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return maxEval;
    } else {
        int minEval = 100000; // Set to a very large value

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                 if (is_valid_move({i, j}, opposing_side)) {
                    msglog(3, "Alpha beta maximizing part.");
                    msglog(3, "%c move : %d %d", active_side, i, j);


                    std::vector<std::vector<char>> prevBoard = play_move({i, j}, opposing_side);

                    char who_played = opposing_side;
                    

                     msglog(3, "Alpha beta prepare minimizing. Player switch.");


                    bool can_switch = switch_player();

                    if (! can_switch) {
                        return scoreX();
                    }

 
                    msglog(3, "After switchPlayer(): active side is: %c", active_side);

                    // If the other player cannot play, keep the current player
                    if (who_played == active_side) {
                        return scoreX();
                    }
                   
                    if (active_side != home_side ) {
                        msglog(3, "activate side i0s %c, should be %c", active_side, home_side);

                        assert( active_side == home_side );
                    }
                    

                    int eval = alphabeta(depth - 1, alpha, beta, home_side);


                    msglog(3, "Move : %d %d, Eval : %d", i, j, eval);

                  
                    restore_board(prevBoard);

                    active_side = who_played;


                    minEval = std::min(minEval, eval);
                    beta = std::min(beta, eval);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return minEval;
    }
}