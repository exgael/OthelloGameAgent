enable_testing()

add_test(NAME MyTest COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test_agent.sh)
