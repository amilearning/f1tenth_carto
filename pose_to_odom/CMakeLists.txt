cmake_minimum_required(VERSION 3.10)
project(pose_to_odom)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_STANDARD 14)

find_package(catkin REQUIRED COMPONENTS
        roscpp
        rospy
        std_msgs
        sensor_msgs
        geometry_msgs
        nav_msgs
        cartographer_ros_msgs
)

catkin_package(
        CATKIN_DEPENDS
        std_msgs
        geometry_msgs
        sensor_msgs
        nav_msgs
        cartographer_ros_msgs
)
link_libraries( ${catkin_LIBRARIES})

include_directories(${catkin_INCLUDE_DIRS} src)

# add_executable(poes_to_odom src/poseToOdom.cpp)

add_executable(carto_init_pose src/cartoInitPose.cpp)

add_executable(scan_filter src/scanFilter.cpp)
target_link_libraries(scan_filter ${catkin_LIBRARIES})
# add_executable(carto_velocity src/cartoVelocity.cpp)

# add_executable(track_velocity src/trackVelocity.cpp)
