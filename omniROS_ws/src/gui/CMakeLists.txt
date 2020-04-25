cmake_minimum_required(VERSION 2.8.3)
project(gui)

find_package(catkin REQUIRED COMPONENTS
    rospy
    std_msgs
    #message_generation
)

################################################
## Declare ROS messages, services and actions ##
################################################

#add_message_files(
#    FILES
#    Vmessage.msg
#)

#add_service_files(
#    FILES
#    SetTimer.srv
#    Delay.srv
#)

#generate_messages(
#    DEPENDENCIES
#    std_msgs
#)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

###################################
## catkin specific configuration ##
###################################

catkin_package(
    CATKIN_DEPENDS message_runtime
)

###########
## Build ##
###########

include_directories(
    ${catkin_INCLUDE_DIRS}
)

#############
## Install ##
#############


#############
## Testing ##
#############
