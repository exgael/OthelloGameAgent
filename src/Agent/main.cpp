#include "agent.h"

Player parse_args(int argc, char *argv[]) {
    if( argc != 2 ) {
        std::cerr << "Utilisation :" << std::endl;
        std::cerr << argv[0] << " [X|O]" << std::endl;
        exit(0);
    }

    if((argv[1][0] != 'X') && (argv[1][0] != 'O')) {
        std::cerr << "Utilisation :" << std::endl;
        std::cerr << argv[0] << " [X|O]" << std::endl;
         exit(0);
    }

    return argv[1][0];
}


void broadcast_move(const Move& move) {
    int row, col;
    std::tie(row, col) = move;
    std::cout << row << col << '\n' << std::flush;
}


Move listen_for_broadcast()
{
    std::string coups;
    std::cin >> coups;
    msglog(0, "coups: \"%d%d\"", coups.at(0) - '0', coups.at(1) - '0' );
    return {coups.at(0) - '0', coups.at(1) - '0'};
}


void play_move_locally(const Move& move) 
{
    if (is_valid_move(board, move, active_side))
    {
       board = play_move(board, move, active_side);
    } 
    else {
        msglog(1, "Failure!");
        msglog(1, "Current player : %c", active_side);
        exit(0);
        }
}


int main(int argc, char *argv[]) {

    msglog(1, "Agent start...");
    char moi = parse_args(argc, argv);

    msglog(1, "Ana : %c", moi);

    init_agent( 
        moi,
        1
    );

    msglog(1, "Agent initialized.");

    while (true) {
        Move move;
        if (home_side == active_side) {
            move = search_next_move( 6 );

            msglog(1, "Broadcasting move...");
            broadcast_move(move);

            msglog(1, "Agent play (%d, %d) on local board",
            std::get<0>(move),
            std::get<1>(move)
            );
        } else {    
            msglog(1, "Agent waiting for opponent...");
            move = listen_for_broadcast();

            msglog(1, "Agent replicate (%d, %d) on local board",
                std::get<0>(move),
                std::get<1>(move)
            );
        }

        play_move_locally(move);

        if(!switch_player(board)) {
            break;
        }
    }
    if (evaluate_board(board, home_side) > 0) {
        msglog(0, "Won!");
    } else {
        msglog(0, "Lost!");
    }
    msglog(0, "Home : %d  vs Opponent : %d", 
        evaluate_board(board, home_side), 
        evaluate_board(board, opposing_side)
    );

    return 0;
}
