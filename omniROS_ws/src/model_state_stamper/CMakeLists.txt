cmake_minimum_required(VERSION 2.8.3)
project(model_state_stamper)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  geometry_msgs
)

add_message_files(
FILES
ModelStatesStamped.msg
)

generate_messages(
  DEPENDENCIES
  geometry_msgs
  std_msgs
)

catkin_package(
CATKIN_DEPENDS message_runtime
)


include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(model_state_stamper src/model_state_stamping_node.cpp)
target_link_libraries(model_state_stamper ${catkin_LIBRARIES})
add_dependencies(model_state_stamper ${catkin_EXPORTED_TARGETS})
