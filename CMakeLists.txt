cmake_minimum_required(VERSION 3.6)
project(podracer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(.)

add_subdirectory(engine)
add_subdirectory(ai)
add_subdirectory(window)
