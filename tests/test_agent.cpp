#include <gtest/gtest.h>
#include "agent.h"

// Test the initBoard function
TEST(AgentTest, InitBoard) {
    initBoard();
    ASSERT_EQ(board[3][3], 'X');
    ASSERT_EQ(board[3][4], 'O');
    ASSERT_EQ(board[4][3], 'O');
    ASSERT_EQ(board[4][4], 'X');
}

// Test the isValidMove function
TEST(AgentTest, IsValidMove) {
    initBoard();
    ASSERT_TRUE(is_valid_move(board, {2, 4}, 'X'));
    ASSERT_FALSE(is_valid_move(board, {4, 1}, 'X'));
    ASSERT_FALSE(is_valid_move(board, {3, 3}, 'X'));
    ASSERT_FALSE(is_valid_move(board, {3, 3}, 'O'));
}

// Test the boardFull function
TEST(AgentTest, BoardFull) {
    initBoard();
    ASSERT_FALSE(board_full(board));
    
    // Fill the board with 'X'
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = 'X';
        }
    }
    ASSERT_TRUE(board_full(board));
}



TEST(SwitchPlayerTest, BasicSwitch) {
    initBoard();

    active_side = 'X';
    bool can_switch_to_O = switch_player(board);
    ASSERT_TRUE(can_switch_to_O);
    ASSERT_EQ(active_side, 'O');

    active_side = 'O';
    bool can_switch_to_X = switch_player(board);
    ASSERT_TRUE(can_switch_to_X);
    ASSERT_EQ(active_side, 'X');
}

TEST(SwitchPlayerTest, NoValidMoves) {
    initBoard();

    // Fill the board with 'X'
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = 'X';
        }
    }
    active_side = 'O';
    bool can_switch_to_X = switch_player(board);
    ASSERT_FALSE(can_switch_to_X);
}

TEST(IS_VALID_MOVE, EmptyBoard_NoValidMoves) {
    initBoard();
    board[3][3] = '-';
    board[3][4] = '-';
    board[4][3] = '-';
    board[4][4] = '-';
    Player player = 'O';
    for(int i = 0; i < 8; i++ ) {
        for(int j = 0; j < 8; j++ ) {
            Move move = {i, j};
            EXPECT_FALSE(is_valid_move(board, move, player));
        }
    }
}

TEST(IS_VALID_MOVE, FullBoard_NoValidMoves) {
    initBoard();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = 'X';
        }
    }

    for(int i = 0; i < 8; i++ ) {
        for(int j = 0; j < 8; j++ ) {
            Move move = {i, j};
            Player player = 'O';
            EXPECT_FALSE(is_valid_move(board, move, player));
            player = 'X';
            EXPECT_FALSE(is_valid_move(board, move, player));
        }
    }
}

TEST(IS_VALID_MOVE, MoveOutOfBounds) {
    initBoard();

    Move move = {-1, -1};
    Player player = 'O';
    EXPECT_FALSE(is_valid_move(board, move, player));
}


TEST(IS_VALID_MOVE, MoveDoesNotFlipAnyPieces) {
    initBoard();

    Move move = {2, 3};
    Player player = 'X';
    EXPECT_FALSE(is_valid_move(board, {2, 3}, player));
    EXPECT_FALSE(is_valid_move(board, {3, 2}, player));
    player = 'O';
    EXPECT_FALSE(is_valid_move(board, {2, 4}, player));
    EXPECT_FALSE(is_valid_move(board, {4, 2}, player));
    EXPECT_FALSE(is_valid_move(board, {5, 5}, player));
    EXPECT_FALSE(is_valid_move(board, {6, 4}, player));
}

TEST(PLAY_MOVE, GameTest1) {
    initBoard();

    Move move;
    Player player;

    // turn 1
    player = 'X';
    move = {5, 3};
    EXPECT_TRUE(is_valid_move(board, move, player));
    board = play_move(board, move, player);
    assert(board[3][3] == 'X');
    assert(board[4][3] == 'X');
    assert(board[5][3] == 'X');

    player = 'O';
    move = {5, 2};
    EXPECT_TRUE(is_valid_move(board, move, player));
    board = play_move(board, move, player);
    assert(board[5][2] == 'O');
    assert(board[4][3] == 'O');
    assert(board[3][4] == 'O');

    // turn 2
    player = 'X';
    auto moves = move_candidates(board, player);
    EXPECT_TRUE(moves.size() == 4);

    move = {5, 1};
    EXPECT_TRUE(is_valid_move(board, move, player));
    board = play_move(board, move, player);
    assert(board[5][1] == 'X');
    assert(board[5][2] == 'X');
    assert(board[5][3] == 'X');

    player = 'O';
    move = {4, 5};
    EXPECT_TRUE(is_valid_move(board, move, player));
    board = play_move(board, move, player);
    assert(board[4][5] == 'O');
    assert(board[4][4] == 'O');
    assert(board[4][3] == 'O');

}

// Define your main function to run all tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}