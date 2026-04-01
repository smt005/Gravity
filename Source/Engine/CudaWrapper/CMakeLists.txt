cmake_minimum_required(VERSION 4.2)

message("Begin generate CUDA. . . . . . . . . . . . . . . .")

if(NOT GRAVITY_HAS_CUDA)
    message(STATUS "CUDA is disabled; skipping CudaWrapper target.")
    message("End generate CUDA. . . . . . . . . . . . . . . .")
    return()
endif()

# архитектура должна задаваться до создания target
set(CMAKE_CUDA_ARCHITECTURES native)

file(GLOB_RECURSE INCLUDES ${CUDA_SOURCE}/*.h)
file(GLOB_RECURSE SOURCES  ${CUDA_SOURCE}/*.cu)

# This CUDA file is only used to force device-link on the final executable.
list(REMOVE_ITEM SOURCES "${CUDA_SOURCE}/GravityCudaLink.cu")

source_group(TREE ${CUDA_SOURCE} PREFIX "" FILES ${INCLUDES} ${SOURCES})

add_library(${CUDA_NAME} STATIC
    ${INCLUDES}
    ${SOURCES}
)

target_include_directories(${CUDA_NAME}
    PUBLIC
        ${JSON_INCLUDES}
        ${GLM_INCLUDES}
        ${MYSTL_SOURCE}
        ${ENGINE_SOURCE}
)

set_target_properties(${CUDA_NAME} PROPERTIES
    CUDA_STANDARD 20
    CUDA_STANDARD_REQUIRED ON
    CUDA_SEPARABLE_COMPILATION ON
)

message("End generate CUDA. . . . . . . . . . . . . . . .")