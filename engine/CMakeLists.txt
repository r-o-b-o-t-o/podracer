cmake_minimum_required(VERSION 3.6)
project(engine)


# Sources
set(
    sources_list

    src/main.cpp
    src/Engine.cpp
    src/Timer.cpp
)


# Includes
include_directories(include ../shared/include)


# Executable file
add_executable(engine ${sources_list})
set_target_properties(engine PROPERTIES OUTPUT_NAME "podracer-engine")


# Shared classes
if (NOT TARGET podracershared)
    add_subdirectory(../shared shared)
endif ()
target_link_libraries(engine podracershared)
