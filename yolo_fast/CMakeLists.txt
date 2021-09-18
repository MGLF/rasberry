cmake_minimum_required(VERSION 3.0.2)
project(yolo_fast)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  rospy
  sensor_msgs
  std_msgs
  geometry_msgs 
  roslib
  tf
)

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui objdetect)

add_message_files(DIRECTORY msg
   FILES
   position.msg
 )


## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
  
 )
catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime message_generation

)


###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
   src/include
   include/ncnn
   lib
)

add_executable(yolo_node src/demo.cpp src/yolo-fastestv2.cpp)
target_link_libraries(yolo_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
target_link_libraries(yolo_node ${CMAKE_SOURCE_DIR}/yolo_fast/lib/libncnn.a -lpthread -fopenmp)
add_dependencies(yolo_node ${${PROJECT_NAME}_EXPORTED_TARGETS})

