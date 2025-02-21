cmake_minimum_required(VERSION 3.6)
project(window)

include(CMakeLists.local.txt OPTIONAL)
find_package(SFML 2.5 COMPONENTS graphics window system)

# Sources
set(
    sources_list

    src/main.cpp
    src/TextureLoader.cpp
    src/FontLoader.cpp
    src/Pod.cpp
    src/PodTooltip.cpp
    src/Wall.cpp
    src/Checkpoint.cpp
)


# Assets
set(assets_dir ${CMAKE_CURRENT_SOURCE_DIR}/../assets)
file(GLOB_RECURSE assets "${assets_dir}/*")


# Includes
include_directories(include ../shared/include)


# Executable file
add_executable(window ${sources_list})
set_target_properties(window PROPERTIES OUTPUT_NAME "podracer-window")


# Copy assets to target location
#foreach (file ${assets})
#    if (NOT IS_DIRECTORY ${file})
#        STRING(REGEX REPLACE "^${assets_dir}/" "" file_dest_name ${file})
#        configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/assets/${file_dest_name} COPYONLY)
#    endif ()
#endforeach ()#


# Shared classes
if (NOT TARGET podracershared)
    add_subdirectory(../shared shared)
endif ()
target_link_libraries(window podracershared)


# Libraries
target_link_libraries(window sfml-graphics sfml-window sfml-system)

if (WIN32)
    target_link_libraries(window opengl32)
    target_link_libraries(window comdlg32 ole32)
else ()
    target_link_libraries(window ${SFML_LIBRARIES} ${EXTRA_LIBS})
endif ()
