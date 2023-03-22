cmake_minimum_required(VERSION 3.10)

project(MyProject)

add_subdirectory(src/Agent)
add_subdirectory(src/BaseAgent)
add_subdirectory(src/codeEnv)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_custom_target(test
    COMMAND ${CMAKE_COMMAND} -E echo "Running test script..."
    COMMAND bash ${CMAKE_SOURCE_DIR}/tests/test.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running test script"
)
