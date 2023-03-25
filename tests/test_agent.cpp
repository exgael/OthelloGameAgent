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
    ASSERT_TRUE(is_valid_move({2, 4}, 'X'));
    ASSERT_FALSE(is_valid_move({4, 1}, 'X'));
    ASSERT_FALSE(is_valid_move({3, 3}, 'X'));
    ASSERT_FALSE(is_valid_move({3, 3}, 'O'));
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
    bool can_switch_to_O = switch_player();
    ASSERT_TRUE(can_switch_to_O);
    ASSERT_EQ(active_side, 'O');

    active_side = 'O';
    bool can_switch_to_X = switch_player();
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
    bool can_switch_to_X = switch_player();
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
            EXPECT_FALSE(is_valid_move(move, player));
        }
    }
}

TEST(IS_VALID_MOVE, FullBoard_NoValidMoves) {}
TEST(IS_VALID_MOVE, NoValidMovesForCurrentPlayer) {}
TEST(IS_VALID_MOVE, MultipleRowsColumnsFlipped) {}
TEST(IS_VALID_MOVE, MoveOutOfBounds) {}
TEST(IS_VALID_MOVE, MoveOnOccupiedSpot) {}
TEST(IS_VALID_MOVE, MoveDoesNotFlipAnyPieces) {}
TEST(IS_VALID_MOVE, MoveFlipsMultiplePiecesInOneDirection) {}

// Define your main function to run all tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}