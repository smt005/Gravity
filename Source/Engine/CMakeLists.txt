cmake_minimum_required(VERSION 4.2)

message("Begin generate Engine. . . . . . . . . . . . . . . .")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE INCLUDES ${ENGINE_SOURCE}/*.h)
file(GLOB_RECURSE SOURCES ${ENGINE_SOURCE}/*.cpp)
source_group(TREE ${ENGINE_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})

add_library(${ENGINE_NAME} STATIC ${INCLUDES} ${SOURCES})

message("End generate Engine. . . . . . . . . . . . . . . .")
