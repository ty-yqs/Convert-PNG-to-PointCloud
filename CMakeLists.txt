cmake_minimum_required(VERSION 3.10)
project(depth2ply LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(depth2ply
  src/main.cpp
  src/depth_to_cloud.cpp
  src/ply_writer.cpp
)

target_link_libraries(depth2ply ${OpenCV_LIBS})
