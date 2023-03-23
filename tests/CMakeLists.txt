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

configure_file(test_othello_game.sh.in test_othello_game.sh)

add_test(NAME test_othello_game
  COMMAND test_othello_game.sh 1
)