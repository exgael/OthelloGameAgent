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