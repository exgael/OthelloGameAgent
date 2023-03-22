#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <ncurses.h>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <csignal>
#include <chrono>
#include <thread>

#include "lib/CLI11.hpp"


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


int pipefd_child2parent_X[2] = {0,0};
int pipefd_parent2child_X[2] = {0,0};

int pipefd_child2parent_O[2] = {0,0};
int pipefd_parent2child_O[2] = {0,0};

bool noGUI=false;

void signal_handler(int signum) {

    if(!noGUI) {
        endwin();
    }

    std::cout << "CTRL + C détecté !" << std::endl;

    if(pipefd_child2parent_X[0] != 0) {
      // Close the pipes
      close(pipefd_child2parent_X[1]);
      close(pipefd_parent2child_X[0]);
    }

    if(pipefd_child2parent_O[0] != 0) {
      // Close the pipes
      close(pipefd_child2parent_O[1]);
      close(pipefd_parent2child_O[0]);
    }

    exit(signum);
}

int main(int argc, char *argv[]) {
    std::signal(SIGINT, signal_handler);


    CLI::App app("Environnement pour le jeu Othello");


    std::string joueurX="";
    app.add_option("-X", joueurX, "Lien vers l'IA pour le joueur X");

    std::string joueurO="";
    app.add_option("-O", joueurO, "Lien vers l'IA pour le joueur O");


    bool slow=false;
    app.add_flag("--slow", slow, "Ajoutez un délai entre chaque coup de l'IA");


    app.add_flag("--noGUI", noGUI, "Désactiver l'affichage graphique");

    CLI11_PARSE(app, argc, argv);



    if(joueurX.size()) {
        // Create a pipe
        pipe(pipefd_child2parent_X);

        // Create a pipe
        pipe(pipefd_parent2child_X);

        // Fork the process
        pid_t pid = fork();

        if (pid == 0)
        {
            // Child process
            close(pipefd_child2parent_X[1]);
            close(pipefd_parent2child_X[0]);

            // Redirect cout to the write end of the pipe
            dup2(pipefd_parent2child_X[1], STDOUT_FILENO);
            dup2(pipefd_child2parent_X[0], STDIN_FILENO);

            std::string s = "\"";
            s += joueurX;
            s += "\" X";
            system(s.c_str());

            sleep(1);

            // Close the pipes
            close(pipefd_child2parent_X[0]);
            close(pipefd_parent2child_X[1]);
            return 0;
        } else {
            sleep(1);

            close(pipefd_child2parent_X[0]);
            close(pipefd_parent2child_X[1]);
        }
    }


    if(joueurO.size()) {
        // Create a pipe
        pipe(pipefd_child2parent_O);

        // Create a pipe
        pipe(pipefd_parent2child_O);

        // Fork the process
        pid_t pid = fork();

        if (pid == 0)
        {
            // Child process
            close(pipefd_child2parent_O[1]);
            close(pipefd_parent2child_O[0]);

            // Redirect cout to the write end of the pipe
            dup2(pipefd_parent2child_O[1], STDOUT_FILENO);
            dup2(pipefd_child2parent_O[0], STDIN_FILENO);

            std::string s = "\"";
            s += joueurO;
            s += "\" O";
            system(s.c_str());

            sleep(1);

            // Close the pipes
            close(pipefd_child2parent_O[0]);
            close(pipefd_parent2child_O[1]);

            return 0;
        } else     {
            sleep(1);

            close(pipefd_child2parent_O[0]);
            close(pipefd_parent2child_O[1]);
        }
    }

	// Initialiser ncurses
    if(!noGUI) {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        mousemask(ALL_MOUSE_EVENTS, NULL);
        curs_set(FALSE);
    }
    initBoard();
    if(!noGUI) {
        printBoard();
    }
    while (true) {
        int col;
        int row;
        std::string coups;
        char buffer[200];

        if((player == 'O') && joueurO.size()) {
            std::cout << "IA O de jouer..." << std::endl;
            char buffer[3];
            read(pipefd_parent2child_O[0], buffer, 3);
            row = buffer[0]-'0';
            col = buffer[1]-'0';
        } else if((player == 'X') && joueurX.size()) {
            std::cout << "IA X de jouer..." << std::endl;
            char buffer[3];
            read(pipefd_parent2child_X[0], buffer, 3);
            row = buffer[0]-'0';
            col = buffer[1]-'0';
        } else {
            std::cout << "Joueur humain de jouer..." << std::endl;
            while(true) {
                int c = getch();
                if (c == KEY_MOUSE) {
                    MEVENT event;
                    getmouse(&event);
                    row = event.y;
                    col = event.x / 2;
                    if (isValidMove(row, col, player)) {
                        break;
                    }
                }
            }
        }

        if (!isValidMove(row, col, player)) {
            std::cout << "Le coups \"" << coups << "\" du joueur " << player << " n'est pas valide." << std::endl;
            std::cout << "Le joueur " << player << " perd." << std::endl;

            assert(false);
            break;
        }

        auto lastPlayer = player;
        auto sendToIA = [&](char lastPlayer){
            if(lastPlayer == 'O') {
                if(joueurX.size()) {
                    buffer[0] = row + '0';
                    buffer[1] = col + '0';
                    buffer[2] = '\n';
                    write(pipefd_child2parent_X[1], buffer, 3);
                }
            } else {
                if(joueurO.size()) {
                    buffer[0] = row + '0';
                    buffer[1] = col + '0';
                    buffer[2] = '\n';
                    write(pipefd_child2parent_O[1], buffer, 3);
                }
            }
        };

        playMove(row, col);
        if(!switchPlayer()) {
            sendToIA(lastPlayer);
            break;
        }
        if(!noGUI) {
            printBoard();
        }
        if(slow) {
            sleep(1);
        }

        sendToIA(lastPlayer);
    }

    afficherFin();
    std::cout << "Fin partie !" << std::endl;
    getch();

    if(!noGUI) {
        endwin();
    }



    return 0;
}
