#include "agent.h"

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

    moi = argv[1][0];

    if ( verbose ) {
        std::cerr <<  "Je suis: "<<  moi << std::endl;
    }
    
    lui = moi == 'X' ? 'O' : 'X';

    srand(time(NULL));

    initBoard();

    verbose = 0;
    while (true) {
        int row;
        int col;

        if (moi == player) {
            auto actionsPossible = listDesCoupsPossible();

            int bestValue = -100000;
            std::tuple<int, int> bestMove;

            for (const auto& action : actionsPossible) {
                int row, col;
                std::tie(row, col) = action;

                std::vector<std::vector<char>> prevBoard = playMove(row, col, moi);

                char current_player = moi;
                switchPlayer();
                assert( player==lui);

                int value = alphabeta(4, -100000, 100000, lui); // Set the depth to 4 or any desired value
                restoreBoard(row, col, prevBoard);
                player = moi;

                if (value > bestValue) {
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
            playMove(row, col, player);
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
