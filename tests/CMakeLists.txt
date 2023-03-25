enable_testing()


add_executable(test_agent
  test_agent.cpp
)

target_link_libraries(test_agent
  core_agent
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_agent)

configure_file(test_othello_game.sh.in test_othello_game.sh @ONLY)

add_test(NAME test_game_1
  COMMAND test_othello_game.sh 1
)
add_test(NAME test_game_2
  COMMAND test_othello_game.sh 2
)
add_test(NAME test_game_3
  COMMAND test_othello_game.sh 3
)
add_test(NAME test_game_4
  COMMAND test_othello_game.sh 4
)
add_test(NAME test_game_5
  COMMAND test_othello_game.sh 5
)
add_test(NAME test_game_6
  COMMAND test_othello_game.sh 6
)
add_test(NAME test_game_7
  COMMAND test_othello_game.sh 7
)
add_test(NAME test_game_8
  COMMAND test_othello_game.sh 8
)
add_test(NAME test_game_9
  COMMAND test_othello_game.sh 9
)
add_test(NAME test_game_10
  COMMAND test_othello_game.sh 10
)
add_test(NAME test_game_11
  COMMAND test_othello_game.sh 11
)
add_test(NAME test_game_12
  COMMAND test_othello_game.sh 12
)
add_test(NAME test_game_13
  COMMAND test_othello_game.sh 13
)
add_test(NAME test_game_14
  COMMAND test_othello_game.sh 14
)
add_test(NAME test_game_15
  COMMAND test_othello_game.sh 15
)
add_test(NAME test_game_16
  COMMAND test_othello_game.sh 16
)
add_test(NAME test_game_17
  COMMAND test_othello_game.sh 17
)