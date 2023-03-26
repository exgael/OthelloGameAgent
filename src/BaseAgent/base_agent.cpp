#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <tuple>
//#include <ncurses.h>


using Coordinate = std::size_t; // Type used to represent the coordinates on the board
using Move = std::tuple<Coordinate, Coordinate>; // Tuple type to represent a move on the board
using Moves = std::vector<Move>; // Set type to represent a collection of unique moves

/*  CONSTANTES  */

constexpr Move RIGHT = {0, 1};
constexpr Move LEFT = {0, -1};
constexpr Move DOWN = {1, 0};
constexpr Move UP = {-1, 0};
constexpr Move UL = {-1, -1};
constexpr Move UR = {-1, 1};
constexpr Move DL = {1, -1};
constexpr Move DR = {1, 1};


// Valid direction to find neighbors
const std::array<Moves, 9> DIRECTIONS = {{
    {RIGHT, DOWN, DR},                           // top-left corner
    {LEFT, DOWN, DL},                            // top-right corner
    {RIGHT, LEFT, DOWN, DR, DL},               // top edge
    {RIGHT, DOWN, UP, DR, UR},                 // left edge
    {LEFT, DOWN, UP, UL, DL},                  // right edge
    {UP, RIGHT, LEFT, UL, UR},                 // bottom edge
    {UP, RIGHT, UR},                             // bottom-left corner
    {UP, LEFT, UL},                              // bottom-right corner
    {UP, DOWN, RIGHT, LEFT, UL, UR, DL, DR} // center
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

std::vector<std::vector<char>> board;
char player = 'X';

void initBoard() {   
    board.resize(8, std::vector<char>(8, '-'));
    board[3][3] = 'X';
    board[3][4] = 'O';
    board[4][3] = 'O';
    board[4][4] = 'X';
}
constexpr std::size_t DIM = 8;

bool isValidMove(int x, int y, char player) {
    if (x < 0 || x >= DIM || y < 0 || y >= DIM || board[x][y] != '-') {
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

void playMove(int x, int y) {
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
            auto actionsPossible = listDesCoupsPossible();

            if (actionsPossible.size() ==0 ) {
                std::cerr << "OPPONENT SKIP TURN!" << std::endl;
            }

            // TODO : CHOISIR LE COUPS A JOUER ////////////
            std::tie(row, col) = actionsPossible[ rand()%actionsPossible.size() ];  // Choix aléatoire
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
