#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <tuple>
#include <ncurses.h>

// rdm id generated
int rdm_id; 

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

std::vector<std::vector<char>> simulate_move(
    std::vector<std::vector<char>>&board_passed, 
    std::tuple<int, int> move) {
    int x = std::get<0>(move);
    int y = std::get<1>(move);


    auto sim = board_passed;
    sim[x][y] = player;  // Placer la pièce du joueur actuel sur la case choisie

    // Vérifier les huit directions autour de la case
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;  // On ignore la case actuelle

            int k = 1;
            while (true) {
                int newX = x + i * k;
                int newY = y + j * k;
                if (newX < 0 || newX >= sim.size() || newY < 0 || newY >= sim[newX].size()) break;  // Sortir si on est en dehors du plateau
                if (sim[newX][newY] == '-') break;  // Sortir si la case est vide
                if (sim[newX][newY] == player) {
                    if (k > 1) {
                        for(int k2 = k-1; k2 > 0; k2--) {
                            int newX = x + i * k2;
                            int newY = y + j * k2;
                            sim[newX][newY] = player;
                        }
                        break;
                    }
                    break;
                }
                k++;
            }
        }
    }

    return sim;
}

void playMove(int x, int y) {
    std::cerr << "Id: " << rdm_id << " Player " << player << " joue en " << x << " " << y << std::endl;
    assert( isValidMove(x, y, player) );
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

std::vector< std::tuple<int, int> > move_candidates(
    std::vector<std::vector<char>>&simulated_board) {

    std::vector< std::tuple<int, int> > result;
    for (int i = 0; i < simulated_board.size(); i++) {
        for (int j = 0; j < simulated_board[i].size(); j++) {
            if (isValidMove(i, j, player)) {
                result.push_back({i,j});
            }
        }
    }

    return result;
}

int alpha_beta(
    std::vector<std::vector<char>>&simulated_board, 
    bool is_max, 
    int depth, 
    int alpha, 
    int beta) {

        // Stop searching if we have reached the maximum depth
        if (depth == 0) {
            return scoreX();
        }

        // Maximizer's turn
        if (is_max) {
            int best_score = -1000;

            auto moves = move_candidates(simulated_board);

            // try all moves

            for (auto move : moves) {
                auto new_board = simulate_move(simulated_board, move);
                int score = alpha_beta(new_board, false, depth - 1, alpha, beta);
                best_score = std::max(best_score, score);
                alpha = std::max(alpha, best_score);
                if (beta <= alpha) {
                    break;
                }
            }
            return best_score;
        }
        //  Minimizer's turn  
        else {
            int best_score = 1000;
            auto moves = move_candidates(simulated_board);
            for (auto move : moves) {
                auto new_board = simulate_move(simulated_board, move);
                int score = alpha_beta(new_board, true, depth - 1, alpha, beta);
                best_score = std::min(best_score, score);
                beta = std::min(beta, best_score);
                if (beta <= alpha) {
                    break;
                }
            }
            return best_score;
        }

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





// chose_move
std::tuple<int, int> choose_move(std::vector<std::vector<char>>&simulated_board) {
    int best_score = -1000;
    int depth = 10;
    std::tuple<int, int> best_move = {-1,-1};
    auto moves = move_candidates(simulated_board);
    for (auto move : moves) {
        auto new_board = simulate_move(simulated_board, move);
        int score = alpha_beta(new_board, false, depth -1, -1000, 1000);
        // print move and score
        std::cerr << "Move: " << std::get<0>(move) << " " << std::get<1>(move) << " Score: " << score << std::endl;
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }
    return best_move;
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

    // Initialisation with random value with 1 nano sec diff
    rdm_id = time(NULL);
    std::cerr << "Main launch with Id: " << rdm_id << std::endl;

    char moi = argv[1][0];

    srand(time(NULL));

    initBoard();
    while (true) {
        int row;
        int col;

        if(moi == player) {
          //  auto actionsPossible = listDesCoupsPossible();

            // TODO : CHOISIR LE COUPS A JOUER ////////////
            std::tie(row, col) = choose_move(board);  //actionsPossible[ rand()%actionsPossible.size() ];  // Choix aléatoire
            std::cout << row << col << '\n' << std::flush;
            //////////////////////////////////////////////

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
