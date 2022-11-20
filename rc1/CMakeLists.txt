# cmake needs this line
cmake_minimum_required(VERSION 2.8)
 
# Define project name
project(rc1)

add_executable(
	rc1 
	main.cpp 
        src/serial_port.cpp
	src/camera.cpp
	src/process.cpp
	src/visualization.cpp
)
 
# Kinect DK相机
find_package(k4a REQUIRED)# 关键。因为后面的target_link_libraries中用到了k4a::k4a，所以在这儿加了一行，否则会显示未定义声明
# Find OpenCV
find_package(OpenCV REQUIRED)
 
# Add includes
include_directories( ${CMAKE_CURRENT_LIST_DIR} ) # 包含当前目录下我的头文件
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(include)
 
# Declare the executable target built from your sources
 
# Link your application with other libraries
target_link_libraries(rc1 k4a::k4a ${OpenCV_LIBS})
 
 
# Enable C++11
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#lh
#2022/11/19
