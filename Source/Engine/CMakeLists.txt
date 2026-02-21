cmake_minimum_required(VERSION 4.2)

message("Begin generate Engine. . . . . . . . . . . . . . . .")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE INCLUDES ${ENGINE_SOURCE}/*.h)
file(GLOB_RECURSE SOURCES ${ENGINE_SOURCE}/*.cpp)
source_group(TREE ${ENGINE_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})

# ImGui sources (build without modifying the imgui directory itself)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

list(APPEND SOURCES ${IMGUI_SOURCES})
include_directories(${IMGUI_INCLUDES})

include_directories(${MYSTL_SOURCE})
include_directories(${GLFW_INCLUDES})
include_directories(${GLAD_INCLUDES})
include_directories(${JSON_INCLUDES})

add_library(${ENGINE_NAME} STATIC ${INCLUDES} ${SOURCES})

message("End generate Engine. . . . . . . . . . . . . . . .")
