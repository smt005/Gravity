cmake_minimum_required(VERSION 4.2)

message("Begin generate Program. . . . . . . . . . . . . . . .")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE INCLUDES ${PROGRAM_SOURCE}/*.h)
file(GLOB_RECURSE SOURCES ${PROGRAM_SOURCE}/*.cpp)
source_group(TREE ${PROGRAM_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})

include_directories(${MYSTL_SOURCE})
include_directories(${ENGINE_SOURCE})

add_library(${PROGRAM_NAME} STATIC ${INCLUDES} ${SOURCES})

message("End generate Program. . . . . . . . . . . . . . . .")
