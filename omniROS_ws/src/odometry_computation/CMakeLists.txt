cmake_minimum_required(VERSION 2.8.3)
project(odometry_computation)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  nav_msgs
  geometry_msgs
  tf
  
)



catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(odometry_computation_node src/odometry_computation.cpp)
target_link_libraries(odometry_computation_node ${catkin_LIBRARIES})
#add_dependencies(odometry_computation_node)
