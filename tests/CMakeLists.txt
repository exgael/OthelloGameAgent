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


configure_file(benchmark.sh benchmark.sh @ONLY)
configure_file(train.sh train.sh @ONLY)
configure_file(config.txt config.txt @ONLY)

#add_test(NAME benchmark
#  COMMAND benchmark.sh 1
#)