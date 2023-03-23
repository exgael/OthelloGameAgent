#include "agent.h"

using Board = std::vector<std::vector<char>>;

Board board;
char player = 'X';
char moi;
char lui;
int verbose = 0;
int no_valid_move = 0;

void initBoard() {   
    board.resize(8, std::vector<char>(8, '-'));
    board[3][3] = 'X';
    board[3][4] = 'O';
    board[4][3] = 'O';
    board[4][4] = 'X';
}


bool isValidMove(int x, int y, char player) {
    if (x < 0 || x >= board.size() || y < 0 || y >= board[x].size()) return false;  // Vérifier si la case est dans les limites du plateau de jeu
    if (board[x][y] != '-') return false;  // Vérifier si la case est vide

    // Vérifier les huit directions autour de la case
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;  // On ignore la case actuelle
          
            int k = 1;
            while (true) {
                int newX = x + i * k;
                int newY = y + j * k;
                if (newX < 0 || newX >= board.size() || newY < 0 || newY >= board[newX].size()) break;  // Sortir si on est en dehors du plateau
                if (board[newX][newY] == '-') break;  // Sortir si la case est vide
                if (board[newX][newY] == player) {
                    if (k > 1) {
                        return true; // Si on a trouvé une pièce du joueur actuel après avoir trouvé une pièce de l'autre joueur, alors le coup est valide
                    }
                    break;
                }
                k++;
            }
        }
    }
  
    return false;
}

std::vector<std::vector<char>> playMove(int x, int y, char current_player) {
    assert( isValidMove(x, y, current_player) );
    std::vector<std::vector<char>> prevBoard = board;
    board[x][y] = current_player;  // Placer la pièce du joueur actuel sur la case choisie

    // Vérifier les huit directions autour de la case
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;  // On ignore la case actuelle

            int k = 1;
            while (true) {
                int newX = x + i * k;
                int newY = y + j * k;
                if (newX < 0 || newX >= board.size() || newY < 0 || newY >= board[newX].size()) break;  // Sortir si on est en dehors du plateau
                if (board[newX][newY] == '-') break;  // Sortir si la case est vide
                if (board[newX][newY] == current_player) {
                    if (k > 1) {
                        for(int k2 = k-1; k2 > 0; k2--) {
                            int newX = x + i * k2;
                            int newY = y + j * k2;
                            board[newX][newY] = current_player;
                        }
                        break;
                    }
                    break;
                }
                k++;
            }
        }
    }
    return prevBoard;
}


// Change de joueur si l'autre joueur peut jouer, si non le joueur garde la main. Si personne ne peut jouer, retourne false
bool switchPlayer() {
    char newPlayer = (player == 'X') ? 'O' : 'X';
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (isValidMove(i, j, newPlayer)) {
                player = newPlayer;

                if ( verbose ) {
                    std::cerr << "Switched player to: " << player << std::endl;
                }
                
                return true;
            }
        }
    }
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (isValidMove(i, j, player)) {

                if ( verbose ) {
                    std::cerr << "Player remains: " << player << std::endl;
                }

                return true;
            }
        }
    }

    if ( verbose ) {
        std::cerr << "No valid moves for either player" << std::endl;
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

std::vector< std::tuple<int, int> > listDesCoupsPossible() {

    std::vector< std::tuple<int, int> > result;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (isValidMove(i, j, player)) {
                result.push_back({i,j});
            }
        }
    }

    return result;
}

void restoreBoard(int x, int y, const std::vector<std::vector<char>>& prevBoard) {
    board = prevBoard;
}

bool boardFull(Board board) {
    for(auto &line: board) {
        for(auto c: line) {
            if(c == '-') {
                return false;
            }
        }
    }
    return true;
}

bool hasValidMoves(char current_player) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (isValidMove(i, j, current_player)) {
                return true;
            }
        }
    }
    return false;
}


int alphabeta(int depth, int alpha, int beta, char current_player) {
    if (depth == 0 || boardFull(board)) {
        if ( verbose ) {
            if (depth == 0 && boardFull(board)) {
                 std::cerr << "Zero depth and board full"  << std::endl;
            }
            else if (depth == 0 ) {
                 std::cerr << "Zero depth."  << std::endl;
            } else {
                std::cerr << "Board full."  << std::endl;
            }

        }
        return scoreX(); // Evaluate the board state when reaching the specified depth or no moves left
    }

    if (current_player == moi) {
        int maxEval = -100000; // Set to a very small value

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                if (isValidMove(i, j, moi)) {
                    if ( verbose ) {
                        std::cerr << "Alpha beta maximizing part."  << std::endl;
                        std::cerr << "Moi :" << player << " plays : " << i << "," << j << std::endl;
                    }

                    std::vector<std::vector<char>> prevBoard = playMove(i, j, moi);

                    char who_played = moi;
                    
                    if ( verbose ) {
                        std::cerr << "Alpha beta prepare minimizing. Player switch."  << std::endl;
                    }

                    bool can_switch = switchPlayer();

                    if (! can_switch) {
                        no_valid_move = 1;
                        return scoreX();
                    }

                    if ( verbose ) {
                        std::cerr << "After switchPlayer(): player is: " << player << std::endl;
                    }
                    // If the other player cannot play, keep the current player
                    if (who_played == player) {
                        return scoreX();
                    }
                   
                    if (player != lui ) {
                        if ( verbose ) {
                            std::cerr << "player is: " << player << ", should be: "  << lui << std::endl;
                        }
                        assert( player == lui );
                    }

                    if ( verbose ) {
                        std::cerr << "player is: " << player << std::endl;
                    }
                    

                    int eval = alphabeta(depth - 1, alpha, beta, lui);
                    // print move and eval

                    if ( verbose ) {
                        std::cerr << "Move: " << i << " " << j << " Eval: " << eval << std::endl;
                    }
                  
                    restoreBoard(i, j, prevBoard);

                    player = who_played;


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
                if (isValidMove(i, j, lui)) {

                    if ( verbose ) {
                        std::cerr << "Alpha beta minimizing part."  << std::endl;
                        std::cerr << "Adversary :" << player << " plays : " << i << "," << j << std::endl;
                    }

                    std::vector<std::vector<char>> prevBoard = playMove(i, j, lui);
                    
                    char who_played = lui;
                             
                    if ( verbose ) {
                        std::cerr << "Alpha beta prepare minimizing. Player switch."  << std::endl;
                    }

                    bool can_switch = switchPlayer();

                    if (! can_switch) {
                        no_valid_move = 1;
                        return scoreX();
                    }

                    if ( verbose ) {
                        std::cerr << "After switchPlayer(): player is: " << player << std::endl;
                    }

                    // If the other player cannot play, keep the current player
                    if (who_played == player) {
                        if ( verbose ) {
                        std::cerr << "Cannot switch player."  << std::endl;
                        }
                        return scoreX();
                    }

                    
                    if ( player != moi ) {

                        if (verbose ) {
                             std::cerr << "player is: " << player << ", should be: "  << moi << std::endl;
                        }
                       
                        assert( player == moi );
                    }

                    if (verbose ) {
                        std::cerr << "player is: " << player << std::endl;
                    }
                    
                    int eval = alphabeta(depth - 1, alpha, beta, moi);
                    restoreBoard(i, j, prevBoard);

                    player = who_played;


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