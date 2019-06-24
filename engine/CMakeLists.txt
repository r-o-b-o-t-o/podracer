cmake_minimum_required(VERSION 3.6)
project(engine)


# Sources
set(
    sources_list

    src/main.cpp
)


# Includes
include_directories(include)


# Executable file
add_executable(engine ${sources_list})
set_target_properties(engine PROPERTIES OUTPUT_NAME "podracer-engine")
