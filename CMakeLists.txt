cmake_minimum_required(VERSION 2.8)

project(ROS_Bag_D435)

SET("OpenCV_DIR" "/home/jin/Third_Party_Packages/opencv-2.4.13/build")
  find_package(OpenCV 2.4 REQUIRED)
include_directories(
  ${OpenCV_INCLUDE_DIRS}
)

message("OPENCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS}")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosbag
  std_msgs
  message_filters
  cv_bridge
  image_transport
  tf
)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs tf
  DEPENDS std_msgs opencv 
)

SET( CMAKE_CXX_COMPILER "g++")
SET( CMAKE_BUILD_TYPE Release )
SET( CMAKE_CXX_FLAGS "-O3 -std=c++11" )


include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(ROS_Bag_D435 src/RGB_IMU_bag.cpp)
target_link_libraries(ROS_Bag_D435 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
