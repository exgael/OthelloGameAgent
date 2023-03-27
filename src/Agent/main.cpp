#include "agent.h"

#include <fstream>
#include <regex>
#include <string>



void read_config_file() {
  std::ifstream file("../tests/config.txt");
  if (file.is_open()) {
    std::string line;
    while (getline(file, line)) {
      std::regex pattern("var1\\s*:\\s*([0-9]*\\.?[0-9]+)");
      std::smatch match;
      if (std::regex_search(line, match, pattern)) {
        weight = std::stod(match[1]);
        break; // stop searching after finding var1 value
      }
    }
    file.close();
  }
}


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

int get_score(Player player) {
    int score = 0;

    for(auto &line: board) {
        for(auto c: line) {
            score += (c == player);
        }
    }
    return score;
}

void print_score() {
    msglog(0, "X : %d", get_score('X'));
    msglog(0, "O : %d", get_score('O'));
}




void play_move_locally(const Move& move) 
{
    if (is_valid_move(board, move, active_side))
    {
        board = play_move(board, move, active_side);
    } 
    else 
    {
        msglog(1, "Failure!");
        msglog(1, "Current player : %c", active_side);
        exit(0);
    }
}




int main(int argc, char *argv[]) {
    char moi = parse_args(argc, argv);

    read_config_file();

    init_agent( 
        moi,
        1
    );

    while (true) {
        Move move;
        if (home_side == active_side) {
            move = search_next_move( 6 );
            broadcast_move(move);
        } else {    
            move = listen_for_broadcast();
        }

        play_move_locally(move);

        if(!switch_player(board)) {
            break;
        }
    }
    if (get_score(home_side) > get_score(opposing_side)) {
        msglog(0, "Won!");
    } else {
        msglog(0, "Lost!");
    }

    print_score();

    return 0;
}