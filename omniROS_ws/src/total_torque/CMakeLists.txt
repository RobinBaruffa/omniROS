cmake_minimum_required(VERSION 2.8.3)
project(total_torque)


find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  std_msgs
  roscpp
  message_generation

)
add_message_files(
 FILES
Float32Stamped.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
CATKIN_DEPENDS message_runtime
)



include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(total_torque src/totalTorque.cpp)
target_link_libraries(total_torque ${catkin_LIBRARIES})
add_dependencies(total_torque ${catkin_EXPORTED_TARGETS})
