cmake_minimum_required(VERSION 4.2)

message("Begin generate CUDA. . . . . . . . . . . . . . . .")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE INCLUDES ${CUDA_SOURCE}/*.h)
file(GLOB_RECURSE SOURCES ${CUDA_SOURCE}/*.cpp)
source_group(TREE ${CUDA_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})

include_directories(${JSON_INCLUDES})
include_directories(${GLM_INCLUDES})
include_directories(${MYSTL_SOURCE})
include_directories(${ENGINE_SOURCE})

add_library(${CUDA_NAME} STATIC ${INCLUDES} ${SOURCES})

message("End generate CUDA. . . . . . . . . . . . . . . .")
