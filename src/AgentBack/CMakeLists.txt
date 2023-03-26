add_library(core_agent_back
  agent_back.cpp
)

# Enable test_file.cpp to use agent.h
target_include_directories(core_agent_back PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(agent_back main.cpp)
target_link_libraries(agent_back PRIVATE core_agent_back)
