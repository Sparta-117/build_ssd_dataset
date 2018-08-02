cmake_minimum_required(VERSION 2.8.3)
project(build_dataset)

## Compile as C++11, supported in ROS Kinetic and newer
 add_compile_options(-std=c++11)

############### dependencies ######################
#Opencv
find_package(OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIR})
# Eigen
include_directories( "/usr/include/eigen3" )
include_directories(${EIGEN_INCLUDE_DIRS})
#ros
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  rospy
  sensor_msgs
  std_msgs
)
catkin_package()
include_directories(${catkin_INCLUDE_DIRS})

#self file
include_directories(../../devel/include)
include_directories(include/build_dataset)
include_directories( ${PROJECT_SOURCE_DIR}/include )

############### execute ######################
#file(GLOB build_dataset_source_files "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB build_dataset_source_files src/main.cpp src/rgbAndGroundTruth.cpp
    src/mergeBackGround.cpp src/formXML.cpp src/tinyxml2.cpp)

file(GLOB formConfigFiles_source_files src/formConfigFiles.cpp src/tinyxml2.cpp)

add_executable(build_dataset ${build_dataset_source_files})
add_executable(form_configfils ${formConfigFiles_source_files})

target_link_libraries(build_dataset
   ${catkin_LIBRARIES}
   ${OpenCV_LIBS}
   ${Eigen_LIBRARIES}
 )

target_link_libraries(form_configfils
   ${catkin_LIBRARIES}
   ${OpenCV_LIBS}
   ${Eigen_LIBRARIES}
 )


