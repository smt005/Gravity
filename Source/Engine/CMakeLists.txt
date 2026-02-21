cmake_minimum_required(VERSION 4.2)

message("Begin generate Engine. . . . . . . . . . . . . . . .")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE INCLUDES ${ENGINE_SOURCE}/*.h)
file(GLOB_RECURSE SOURCES ${ENGINE_SOURCE}/*.cpp)
source_group(TREE ${ENGINE_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})

include_directories(${MYSTL_SOURCE})
include_directories(${GLFW_INCLUDES})
include_directories(${GLAD_INCLUDES})
include_directories(${JSON_INCLUDES})

add_library(${ENGINE_NAME} STATIC ${INCLUDES} ${SOURCES})

message("End generate Engine. . . . . . . . . . . . . . . .")
