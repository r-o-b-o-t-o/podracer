cmake_minimum_required(VERSION 3.6)
project(podracer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(engine)
add_subdirectory(ai)
add_subdirectory(window)
