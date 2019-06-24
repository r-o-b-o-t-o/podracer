cmake_minimum_required(VERSION 3.6)
project(ai)


# Sources
set(
    sources_list

    src/main.cpp
)


# Includes
include_directories(include)


# Executable file
add_executable(ai ${sources_list})
set_target_properties(ai PROPERTIES OUTPUT_NAME "podracer-ai")
