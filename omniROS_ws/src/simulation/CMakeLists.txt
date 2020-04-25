cmake_minimum_required(VERSION 2.8.3)
project(simulation)


find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  gazebo_msgs
  std_srvs
)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(simulation src/simulation.cpp)
target_link_libraries(simulation ${catkin_LIBRARIES})
#add_dependencies(simulation)
