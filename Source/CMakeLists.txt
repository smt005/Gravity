cmake_minimum_required(VERSION 4.2)

message("Begin generate. . . . . . . . . . . . . . . .")

set(PROJECT_NAME Gravity)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_MAIN_FILE "${SOURCE_DIR}/Engine/Platforms/Win/main.cpp")
	message("PROJECT_MAIN_FILE: ${PROJECT_MAIN_FILE}")

# Game
set(PROGRAM_NAME Program)
set(PROGRAM_SOURCE "${SOURCE_DIR}/Program")

# Engine
set(ENGINE_NAME Engine)
set(ENGINE_DIR "${SOURCE_DIR}/Engine")
set(ENGINE_SOURCE "${ENGINE_DIR}/Source")

# ThirdParty
# MyStl
set(MYSTL_NAME MyStl)
set(MYSTL_DIR "${ENGINE_DIR}/ThirdParty/MyStl")
set(MYSTL_SOURCE "${MYSTL_DIR}/Source")

# properties
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOURCE_DIR}/../Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOURCE_DIR}/../Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOURCE_DIR}/../Bin)

# Project
project(${PROJECT_NAME} LANGUAGES CXX)
add_executable(${PROJECT_NAME} ${PROJECT_MAIN_FILE})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Game
include_directories(${PROGRAM_SOURCE})
add_subdirectory(${PROGRAM_SOURCE})
target_link_libraries(${PROJECT_NAME} ${PROGRAM_NAME})
link_directories(${PROGRAM_SOURCE})

# Engine
include_directories(${ENGINE_SOURCE})
add_subdirectory(${ENGINE_DIR})
target_link_libraries(${PROJECT_NAME} ${ENGINE_NAME})
link_directories(${ENGINE_SOURCE})

# ThirdParty
# MyStl
include_directories(${MYSTL_SOURCE})
add_subdirectory(${MYSTL_DIR})
target_link_libraries(${PROJECT_NAME} ${MYSTL_NAME})
link_directories(${MYSTL_SOURCE})

message("End generate. . . . . . . . . . . . . . . .")
