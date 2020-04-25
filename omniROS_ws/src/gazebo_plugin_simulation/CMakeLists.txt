#copied from http://gazebosim.org/tutorials/?tut=plugins_hello_world#PluginTypes
#Example of HelloWorld plugin
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(roscpp REQUIRED)
find_package(std_msgs REQUIRED)
include_directories(${roscpp_INCLUDE_DIRS})
include_directories(${std_msgs_INCLUDE_DIRS})

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")


add_library(simulation_plugin SHARED simulation_plugin.cc)
target_link_libraries(simulation_plugin ${GAZEBO_LIBRARIES})
