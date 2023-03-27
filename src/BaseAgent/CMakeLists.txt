add_library(core_base_agent
  base_agent.cpp
)

add_executable(base_agent base_agent.cpp)
target_link_libraries(base_agent PRIVATE core_base_agent)