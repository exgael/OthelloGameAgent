add_library(core_agent
  agent.cpp
)

add_executable(agent agent.cpp)
target_link_libraries(agent PRIVATE core_agent)