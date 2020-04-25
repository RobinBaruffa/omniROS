# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "model_state_stamper: 1 messages, 0 services")

set(MSG_I_FLAGS "-Imodel_state_stamper:/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(model_state_stamper_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg" NAME_WE)
add_custom_target(_model_state_stamper_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "model_state_stamper" "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg" "geometry_msgs/Twist:std_msgs/Header:geometry_msgs/Quaternion:geometry_msgs/Vector3:geometry_msgs/Point:geometry_msgs/Pose"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(model_state_stamper
  "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/model_state_stamper
)

### Generating Services

### Generating Module File
_generate_module_cpp(model_state_stamper
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/model_state_stamper
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(model_state_stamper_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(model_state_stamper_generate_messages model_state_stamper_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg" NAME_WE)
add_dependencies(model_state_stamper_generate_messages_cpp _model_state_stamper_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(model_state_stamper_gencpp)
add_dependencies(model_state_stamper_gencpp model_state_stamper_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS model_state_stamper_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(model_state_stamper
  "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/model_state_stamper
)

### Generating Services

### Generating Module File
_generate_module_eus(model_state_stamper
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/model_state_stamper
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(model_state_stamper_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(model_state_stamper_generate_messages model_state_stamper_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg" NAME_WE)
add_dependencies(model_state_stamper_generate_messages_eus _model_state_stamper_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(model_state_stamper_geneus)
add_dependencies(model_state_stamper_geneus model_state_stamper_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS model_state_stamper_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(model_state_stamper
  "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/model_state_stamper
)

### Generating Services

### Generating Module File
_generate_module_lisp(model_state_stamper
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/model_state_stamper
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(model_state_stamper_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(model_state_stamper_generate_messages model_state_stamper_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg" NAME_WE)
add_dependencies(model_state_stamper_generate_messages_lisp _model_state_stamper_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(model_state_stamper_genlisp)
add_dependencies(model_state_stamper_genlisp model_state_stamper_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS model_state_stamper_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(model_state_stamper
  "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/model_state_stamper
)

### Generating Services

### Generating Module File
_generate_module_nodejs(model_state_stamper
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/model_state_stamper
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(model_state_stamper_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(model_state_stamper_generate_messages model_state_stamper_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg" NAME_WE)
add_dependencies(model_state_stamper_generate_messages_nodejs _model_state_stamper_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(model_state_stamper_gennodejs)
add_dependencies(model_state_stamper_gennodejs model_state_stamper_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS model_state_stamper_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(model_state_stamper
  "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/model_state_stamper
)

### Generating Services

### Generating Module File
_generate_module_py(model_state_stamper
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/model_state_stamper
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(model_state_stamper_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(model_state_stamper_generate_messages model_state_stamper_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/user/omniROS/omniROS_ws/src/model_state_stamper/msg/ModelStatesStamped.msg" NAME_WE)
add_dependencies(model_state_stamper_generate_messages_py _model_state_stamper_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(model_state_stamper_genpy)
add_dependencies(model_state_stamper_genpy model_state_stamper_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS model_state_stamper_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/model_state_stamper)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/model_state_stamper
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(model_state_stamper_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(model_state_stamper_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/model_state_stamper)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/model_state_stamper
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(model_state_stamper_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(model_state_stamper_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/model_state_stamper)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/model_state_stamper
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(model_state_stamper_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(model_state_stamper_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/model_state_stamper)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/model_state_stamper
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(model_state_stamper_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(model_state_stamper_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/model_state_stamper)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/model_state_stamper\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/model_state_stamper
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(model_state_stamper_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(model_state_stamper_generate_messages_py std_msgs_generate_messages_py)
endif()
