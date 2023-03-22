cmake_minimum_required(VERSION 3.10)

project(MyProject)

add_subdirectory(src/Agent)
add_subdirectory(src/BaseAgent)
add_subdirectory(src/codeEnv)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build)

include(CTest)

