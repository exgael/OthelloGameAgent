enable_testing()

add_test(NAME test_1 COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test_agent.sh 1)
add_test(NAME test_2 COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test_agent.sh 2)
add_test(NAME test_3 COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test_agent.sh 3)
add_test(NAME test_4 COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test_agent.sh 4)
add_test(NAME test_5 COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test_agent.sh 5)
add_test(NAME test_6 COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test_agent.sh 6)