cmake_minimum_required(VERSION 4.2)

message("Begin generate MyStl. . . . . . . . . . . . . . . .")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE INCLUDES ${MYSTL_SOURCE}/*.h)
file(GLOB_RECURSE SOURCES ${MYSTL_SOURCE}/*.cpp)
source_group(TREE ${MYSTL_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})
#source_group(TREE ${MYSTL_SOURCE} TREE PREFIX "" FILES ${INCLUDES})

add_library(${MYSTL_NAME} STATIC ${INCLUDES} ${SOURCES})
#add_library(${MYSTL_NAME} INTERFACE ${INCLUDES})

message("End generate MyStl. . . . . . . . . . . . . . . .")
