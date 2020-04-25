cmake_minimum_required(VERSION 2.8.3)
project(fitness)


find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  gazebo_msgs
  std_msgs
  roscpp
  total_torque
  model_state_stamper
  message_filters

)
catkin_package(
  CATKIN_DEPENDS message_filters roscpp total_torque model_state_stamper
)
catkin_package()


include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(fitness src/fitness.cpp)
target_link_libraries(fitness ${catkin_LIBRARIES})
add_dependencies(model_state_stamper ${catkin_EXPORTED_TARGETS})
add_dependencies(total_torque ${catkin_EXPORTED_TARGETS})
#Changes the build order , same as add_executable on the other package
