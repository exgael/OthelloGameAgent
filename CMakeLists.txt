cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_compile_definitions(NDEBUG)


add_subdirectory(src/Agent)
add_subdirectory(src/BaseAgent)
add_subdirectory(src/codeEnv)







