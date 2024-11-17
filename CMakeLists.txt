#Top-level
cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

project(gpxSpeedEditor VERSION 1.0)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.4 REQUIRED)
find_package(pugixml REQUIRED)

add_subdirectory(libs)
add_subdirectory(src)