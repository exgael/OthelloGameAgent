#include "agent.h"

Player parse_args(int argc, char *argv[]) {
    if( argc != 2 ) {
        std::cerr << "Agent back Utilisation :" << std::endl;
        std::cerr << argv[0] << " [X|O]" << std::endl;
        exit(0);
    }

    if((argv[1][0] != 'X') && (argv[1][0] != 'O')) {
        std::cerr << "Agent back Utilisation :" << std::endl;
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


void print_board()
{
    for (int i = 0; i < 8; i++) {
        char row[18] = {'\0'};  // initialize row to null characters
        for (int j = 0; j < 8; j++) {
            row[j * 2] = board[i][j];  // set the character at the appropriate position in the row array
            row[j * 2 + 1] = '|';      // set the separator character
        }
        msglog(0, "%s", row);  // print the row using the msglog() function
    }
}


void play_move_locally(const Move& move) 
{
    if (is_valid_move(board, move, active_side))
    {
        board = play_move(board, move, active_side);
    } 
    else 
    {
        msglog(1, "Agent back Failure!");
        msglog(1, "Agent back Current player : %c", active_side);
        exit(0);
    }
}




int main(int argc, char *argv[]) {
    char moi = parse_args(argc, argv);

    init_agent( 
        moi,
        1
    );

    while (true) {
        Move move;
        if (home_side == active_side) {
            move = search_next_move( 8 );
            broadcast_move(move);
        } else {    
            move = listen_for_broadcast();
        }

        play_move_locally(move);

        if(!switch_player(board)) {
            break;
        }
    }

    return 0;
}