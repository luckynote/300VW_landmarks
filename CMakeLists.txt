cmake_minimum_required(VERSION 3.12)
project(300VW_landmark)

set(CMAKE_CXX_STANDARD 14)
find_package( OpenCV REQUIRED )

add_executable(300VW_landmark main.cpp)
target_link_libraries(300VW_landmark ${OpenCV_LIBS})