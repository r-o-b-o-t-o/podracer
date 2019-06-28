cmake_minimum_required(VERSION 3.6)
project(shared)


# Sources
set(
    sources_list

    src/Messaging.cpp
)


# Includes
include_directories(include)


add_library(podracershared STATIC ${sources_list})
