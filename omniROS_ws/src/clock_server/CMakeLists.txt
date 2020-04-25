cmake_minimum_required(VERSION 2.8.3)
project(clock_server)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosgraph_msgs
)



catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(clock_server_node src/clock_server.cpp)
target_link_libraries(clock_server_node ${catkin_LIBRARIES})
#add_dependencies(clock_server_node)
