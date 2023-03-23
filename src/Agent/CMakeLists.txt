add_library(core_agent
  agent.cpp
)

# Enable test file.cpp to use agent.h
target_include_directories(core_agent PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(agent agent.cpp)
target_link_libraries(agent PRIVATE core_agent)