#copied from http://gazebosim.org/tutorials/?tut=plugins_hello_world#PluginTypes
#Example of HelloWorld plugin

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(roscpp REQUIRED)
find_package(geometry_msgs REQUIRED)
include_directories(${roscpp_INCLUDE_DIRS})
include_directories(${geometry_msgs_INCLUDE_DIRS})


# Find Gazebo
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")



#link_directories(${GAZEBO_LIBRARY_DIRS})
#list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

#Added from http://gazebosim.org/tutorials?tut=plugins_model&cat=write_plugin
# Build our plugin
add_library(torque_plugin SHARED torque_plugin.cc)
target_link_libraries(torque_plugin ${GAZEBO_LIBRARIES} ${roscpp_LIBRARIES})
# Build the stand-alone test program
#add_executable(vel vel.cc)
