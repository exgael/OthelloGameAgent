add_library(core
  environment.cpp
)

add_executable(environment environment.cpp)
target_link_libraries(environment PRIVATE core ncurses)

target_include_directories(core PRIVATE lib)
target_include_directories(environment PRIVATE lib)