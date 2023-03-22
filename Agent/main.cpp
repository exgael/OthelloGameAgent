#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <tuple>
//#include <ncurses.h>


std::vector<std::vector<char>> board;
char player = 'X';

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

std::vector<std::vector<char>> playMove(int x, int y) {
    assert( isValidMove(x, y, player) );
    std::vector<std::vector<char>> prevBoard = board;
    board[x][y] = player;  // Placer la pièce du joueur actuel sur la case choisie

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
                return true;
            }
        }
    }
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (isValidMove(i, j, player)) {
                return true;
            }
        }
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

void undoMove(int x, int y, const std::vector<std::vector<char>>& prevBoard) {
    board = prevBoard;
}


int alphabeta(int depth, int alpha, int beta, char maximizingPlayer) {
    if (depth == 0 || !switchPlayer()) {
        return scoreX(); // Evaluate the board state when reaching the specified depth or no moves left
    }

    if (maximizingPlayer == 'X') {
        int maxEval = -100000; // Set to a very small value

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                if (isValidMove(i, j, player)) {
                    std::vector<std::vector<char>> prevBoard = playMove(i, j);
                    char prevPlayer = player;
                    switchPlayer();
                    int eval = alphabeta(depth - 1, alpha, beta, 'O');
                    // print move and eval
                    std::cerr << "Move: " << i << " " << j << " Eval: " << eval << std::endl;
                    undoMove(i, j, prevBoard);
                    player = prevPlayer;
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
                if (isValidMove(i, j, player)) {
                    std::vector<std::vector<char>> prevBoard = playMove(i, j);
                    char prevPlayer = player;
                    switchPlayer();
                    int eval = alphabeta(depth - 1, alpha, beta, 'X');
                    undoMove(i, j, prevBoard);
                    player = prevPlayer;
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


int main(int argc, char *argv[]) {

    if( argc != 2 ) {
        std::cerr << "Utilisation :" << std::endl;
        std::cerr << argv[0] << " [X|O]" << std::endl;
        return 0;
    }

    if((argv[1][0] != 'X') && (argv[1][0] != 'O')) {
        std::cerr << "Utilisation :" << std::endl;
        std::cerr << argv[0] << " [X|O]" << std::endl;
        return 0;
    }

    char moi = argv[1][0];

    srand(time(NULL));

    initBoard();
    while (true) {
        int row;
        int col;

        if (moi == player) {
            auto actionsPossible = listDesCoupsPossible();

            int bestValue = (moi == 'X') ? -100000 : 100000;
            std::tuple<int, int> bestMove;

            for (const auto& action : actionsPossible) {
                int row, col;
                std::tie(row, col) = action;
                std::vector<std::vector<char>> prevBoard = playMove(row, col);
                char prevPlayer = player;
                switchPlayer();
                int value = alphabeta(4, -100000, 100000, (moi == 'X') ? 'O' : 'X'); // Set the depth to 4 or any desired value
                undoMove(row, col, prevBoard);
                player = prevPlayer;

                if ((moi == 'X' && value > bestValue) || (moi == 'O' && value < bestValue)) {
                    bestValue = value;
                    bestMove = action;
                }
            }
            std::tie(row, col) = bestMove;
            std::cout << row << col << '\n' << std::flush;

        } else {
            std::string coups;
            std::cin >> coups;
            std::cerr << "coups: \"" << coups << "\"" << std::endl;
            row =  coups[0]-'0';
            col =  coups[1]-'0';
        }

        if (isValidMove(row, col, player)) {
            playMove(row, col);
            if(!switchPlayer()) {
                break;
            }
        } else {
            std::cerr << "ERREUR: coups " << row << " " << col << " est invalide !" << std::endl;
            return 0;
        }
    }

    return 0;
}
