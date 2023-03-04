#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <tuple>
#include <ncurses.h>


std::vector<std::vector<char>> board;
char player = 'X';

void initBoard() {   
    board.resize(8, std::vector<char>(8, '-'));
    board[3][3] = 'X';
    board[3][4] = 'O';
    board[4][3] = 'O';
    board[4][4] = 'X';
}

void printBoard() {
    clear();
    for(auto &line: board) {
        for(auto c: line) {
            printw("%c ", c);
        }
        printw("\n");
    }
    printw("Player: %c\n", player);
    refresh();
}


// print board in stderr
void printBoardDebug() {
    std::cerr << "Board:" << std::endl;
    for(auto &line: board) {
        for(auto c: line) {
            std::cerr << c << " ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Player: " << player << std::endl;
}


bool isValidMove(int x, int y, char player, std::vector<std::vector<char>> board_) {
    if (x < 0 || x >= board_.size() || y < 0 || y >= board_[x].size()) return false;  // Vérifier si la case est dans les limites du plateau de jeu
    if (board_[x][y] != '-') return false;  // Vérifier si la case est vide

    // Vérifier les huit directions autour de la case
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;  // On ignore la case actuelle
          
            int k = 1;
            while (true) {
                int newX = x + i * k;
                int newY = y + j * k;
                if (newX < 0 || newX >= board_.size() || newY < 0 || newY >= board_[newX].size()) break;  // Sortir si on est en dehors du plateau
                if (board_[newX][newY] == '-') break;  // Sortir si la case est vide
                if (board_[newX][newY] == player) {
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

void playMove(int x, int y,  std::vector<std::vector<char>> &board_ ) {
    assert( isValidMove(x, y, player, board_) );

    board_[x][y] = player;  // Placer la pièce du joueur actuel sur la case choisie

    // Vérifier les huit directions autour de la case
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;  // On ignore la case actuelle

            int k = 1;
            while (true) {
                int newX = x + i * k;
                int newY = y + j * k;
                if (newX < 0 || newX >= board_.size() || newY < 0 || newY >= board_[newX].size()) break;  // Sortir si on est en dehors du plateau
                if (board_[newX][newY] == '-') break;  // Sortir si la case est vide
                if (board_[newX][newY] == player) {
                    if (k > 1) {
                        for(int k2 = k-1; k2 > 0; k2--) {
                            int newX = x + i * k2;
                            int newY = y + j * k2;
                            board_[newX][newY] = player;
                        }
                        break;
                    }
                    break;
                }
                k++;
            }
        }
    }
}


// Change de joueur si l'autre joueur peut jouer, si non le joueur garde la main. Si personne ne peut jouer, retourne false
bool switchPlayer(std::vector<std::vector<char>> &board_) {
    char newPlayer = (player == 'X') ? 'O' : 'X';
    std::cerr << "Switching player from " << player << " to " << newPlayer << std::endl;
    for (int i = 0; i < board_.size(); i++) {
        for (int j = 0; j < board_[i].size(); j++) {
            if (isValidMove(i, j, newPlayer, board_)) {
                player = newPlayer;
                return true;
            }
        }
    }
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (isValidMove(i, j, player, board_)) {
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

void afficherFin() {
    clear();
    for(auto &line: board) {
        for(auto c: line) {
            printw("%c ", c);
        }
        printw("\n");
    }
    int s = scoreX();
    if(s > 0) {
        printw("Player X a gagner de %d points !\n", s);
    } else if(s < 0) {
        printw("Player O a gagner de %d points !\n", -s);
    } else {
        printw("Egalité !\n");
    }
    refresh();
}

std::vector< std::tuple<int, int> > listDesCoupsPossible(std::vector<std::vector<char>> board_) {

    std::vector< std::tuple<int, int> > result;

    for (int i = 0; i < board_.size(); i++) {
        for (int j = 0; j < board_[i].size(); j++) {
            if (isValidMove(i, j, player, board_)) {
                result.push_back({i,j});
            }
        }
    }

    return result;
}

bool boardFull() {
    for(auto &line: board) {
        for(auto c: line) {
            if(c == '-') {
                return false;
            }
        }
    }
    return true;
}




int alphabeta(int depth, int alpha, int beta, bool maximizingPlayer, std::vector<std::vector<char>> board_) {
    if(depth == 0 || boardFull()) {
        return scoreX();
    }

    // copy the board_

    if(maximizingPlayer) {
        int maxEval = -100000;
        auto possibleMoves = listDesCoupsPossible(board_);

        for(auto &move: possibleMoves) {
            int row, col;
            std::tie(row, col) = move;

            
            playMove(row, col, board_);
            int eval = alphabeta(depth - 1, alpha, beta, false, board_);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if(beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = 100000;
        auto possibleMoves = listDesCoupsPossible(board_);

        for(auto &move: possibleMoves) {
            int row, col;
            std::tie(row, col) = move;
            playMove(row, col, board_);
            int eval = alphabeta(depth - 1, alpha, beta, true, board_);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if(beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}


std::tuple<int, int> chooseMove() {
    int depth = 6; // Profondeur maximale de recherche

    auto possibleMoves = listDesCoupsPossible(board);
    int maxEval = -100000;
    int bestRow = -1;
    int bestCol = -1;

    for(auto &move: possibleMoves) {
        int row, col;
        std::cerr << "Trying move (" << row << ", " << col << ")" << std::endl;
        std::tie(row, col) = move;
        std::vector<std::vector<char>> board_copy = board;
        playMove(row, col, board_copy);
        
        int eval = alphabeta(depth - 1, -100000, 100000, false, board_copy);
        std::cerr << "Move (" << row << ", " << col << ") has score " << eval << std::endl;
        if(eval > maxEval) {
            maxEval = eval;
            bestRow = row;
            bestCol = col;
        }
    }
    std::cerr << "Best move is (" << bestRow << ", " << bestCol << ") with score " << maxEval << std::endl;
    return {bestRow, bestCol};
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

        if(moi == player) {
            // CHOISIR LE COUPS A JOUER ////////////
            std::tie(row, col) = chooseMove();
            std::cerr << "Joue " << row << " " << col << std::endl;
            std::cout << row << col << '\n' << std::flush;
            ////////////////////////////////////////

        } else {
            std::string coups;
            std::cin >> coups;
            std::cerr << "coups: \"" << coups << "\"" << std::endl;
            row =  coups[0]-'0';
            col =  coups[1]-'0';
        }

        if (isValidMove(row, col, player, board)) {
            playMove(row, col, board);
            if(!switchPlayer(board)) {
                break;
            }
        } else {
            std::cerr << "ERREUR: coups " << row << " " << col << " est invalide !" << std::endl;
            return 0;
        }
    }

    return 0;
}
