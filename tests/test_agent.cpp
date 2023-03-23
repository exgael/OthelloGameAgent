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
    ASSERT_TRUE(isValidMove(2, 4, 'X'));
    ASSERT_FALSE(isValidMove(4, 1, 'X'));
    ASSERT_FALSE(isValidMove(3, 3, 'X'));
    ASSERT_FALSE(isValidMove(3, 3, 'O'));
}

// Test the boardFull function
TEST(AgentTest, BoardFull) {
    initBoard();
    ASSERT_FALSE(boardFull(board));
    
    // Fill the board with 'X'
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = 'X';
        }
    }
    ASSERT_TRUE(boardFull(board));
}

// Test the hasValidMoves function
TEST(AgentTest, HasValidMoves) {
    initBoard();
    ASSERT_TRUE(hasValidMoves('X'));
    ASSERT_TRUE(hasValidMoves('O'));

    // Fill the board with 'X'
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = 'X';
        }
    }
    ASSERT_FALSE(hasValidMoves('X'));
    ASSERT_FALSE(hasValidMoves('O'));
}

TEST(SwitchPlayerTest, BasicSwitch) {
    initBoard();

    player = 'X';
    bool can_switch_to_O = switchPlayer();
    ASSERT_TRUE(can_switch_to_O);
    ASSERT_EQ(player, 'O');

    player = 'O';
    bool can_switch_to_X = switchPlayer();
    ASSERT_TRUE(can_switch_to_X);
    ASSERT_EQ(player, 'X');
}

TEST(SwitchPlayerTest, NoValidMoves) {
    initBoard();

    // Fill the board with 'X'
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = 'X';
        }
    }
    player = 'O';
    bool can_switch_to_X = switchPlayer();
    ASSERT_FALSE(can_switch_to_X);
}

// Define your main function to run all tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}