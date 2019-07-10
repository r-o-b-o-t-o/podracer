cmake_minimum_required(VERSION 3.6)
project(shared)


# Sources
set(
    sources_list

    src/Messaging.cpp
    src/Message.cpp
    src/Turn.cpp
    src/Settings.cpp
    src/Action.cpp
    src/Random.cpp
    src/Pod.cpp
    src/Physics.cpp
    src/Entity.cpp
)


# Includes
include_directories(include)


add_library(podracershared STATIC ${sources_list})
