cmake_minimum_required(VERSION 3.12)
project(test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_ROOT}/Modules)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(include)

add_executable(test main.cpp)
