cmake_minimum_required(VERSION 4.2)

message("Begin generate. . . . . . . . . . . . . . . .")

set(PROJECT_NAME Gravity)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_PLATFORM_DIR "${SOURCE_DIR}/Engine/Platforms/Win")
set(PROJECT_MAIN_FILE "${PROJECT_PLATFORM_DIR}/main.cpp")
set(PROJECT_DEFINE_FILE "${PROJECT_PLATFORM_DIR}/Defines.h")

# Game
set(PROGRAM_NAME Program)
set(PROGRAM_SOURCE "${SOURCE_DIR}/${PROGRAM_NAME}")

# Engine
set(ENGINE_NAME Engine)
set(ENGINE_DIR "${SOURCE_DIR}/${ENGINE_NAME}")
set(ENGINE_SOURCE "${ENGINE_DIR}/Source")

# ThirdParty
# MyStl
set(MYSTL_NAME MyStl)
set(MYSTL_DIR "${ENGINE_DIR}/ThirdParty/${MYSTL_NAME}")
set(MYSTL_SOURCE "${MYSTL_DIR}/Source")

# GLFW
set(GLFW_NAME glfw)
set(GLFW_DIR "${ENGINE_DIR}/ThirdParty/${GLFW_NAME}")
set(GLFW_INCLUDES "${GLFW_DIR}/include")
set(GLFW_SOURCE "${GLFW_DIR}/src")
set(GLFW_LIB "${PROJECT_DIR}/../build/Win/Engine/ThirdParty/glfw/src")

# GLAD
set(GLAD_NAME glad)
set(GLAD_DIR "${ENGINE_DIR}/ThirdParty/${GLAD_NAME}")
set(GLAD_INCLUDES "${GLAD_DIR}/include")
set(GLAD_SOURCE "${GLAD_DIR}/src")
set(GLAD_LIB "${PROJECT_DIR}/../lib")

# JSON (nlohmann/json)
set(JSON_NAME json)
set(JSON_DIR "${ENGINE_DIR}/ThirdParty/${JSON_NAME}")
set(JSON_INCLUDES "${JSON_DIR}/include")

# GLM
set(GLM_NAME glm)
set(GLM_DIR "${ENGINE_DIR}/ThirdParty/${GLM_NAME}")
set(GLM_INCLUDES "${GLM_DIR}")

# ImGui
set(IMGUI_NAME imgui)
set(IMGUI_DIR "${ENGINE_DIR}/ThirdParty/${IMGUI_NAME}")
set(IMGUI_INCLUDES "${IMGUI_DIR}")

# FreeImage (prebuilt: Dist/x32, Dist/x64) — arch set after project()
set(FREEIMAGE_NAME FreeImage)
set(FREEIMAGE_DIR "${ENGINE_DIR}/ThirdParty/${FREEIMAGE_NAME}")

set(CUDA_NAME CudaWrapper)
set(CUDA_DIR "${ENGINE_DIR}/${CUDA_NAME}")
set(CUDA_SOURCE "${CUDA_DIR}/Source")

# properties
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOURCE_DIR}/../Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOURCE_DIR}/../Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOURCE_DIR}/../Bin)

# Project
project(${PROJECT_NAME} LANGUAGES CXX)
add_executable(${PROJECT_NAME} ${PROJECT_MAIN_FILE} ${PROJECT_DEFINE_FILE})
include_directories(${PROJECT_PLATFORM_DIR})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Keep MSVC runtime consistent across all subprojects/targets.
# Mismatched runtime settings can surface as unresolved externals in MSVC STL
# (e.g. __std_find_not_ch_1) depending on which libraries/objects get linked.
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# CUDA (optional)
option(GRAVITY_ENABLE_CUDA "Build Gravity with CUDA acceleration" ON)
#set(GRAVITY_ENABLE_CUDA OFF)
set(GRAVITY_HAS_CUDA OFF)
if(GRAVITY_ENABLE_CUDA)
	include(CheckLanguage)
    check_language(CUDA)
    
	if(CMAKE_CUDA_COMPILER)
        enable_language(CUDA)
        set(GRAVITY_HAS_CUDA ON)
		message("CUDA enabled")		
    else()
        message(WARNING "CUDA requested but no CUDA compiler found; building without CUDA.")
    endif()
endif()

# FreeImage: choose arch after project() so CMAKE_SIZEOF_VOID_P is set
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(FREEIMAGE_DIST_ARCH "x64")
else()
    set(FREEIMAGE_DIST_ARCH "x32")
endif()
set(FREEIMAGE_INCLUDES "${FREEIMAGE_DIR}/Dist/${FREEIMAGE_DIST_ARCH}")
set(FREEIMAGE_LIB_DIR "${FREEIMAGE_DIR}/Dist/${FREEIMAGE_DIST_ARCH}")

# ThirdParty

# glm
include_directories(${GLM_INCLUDES})

# glfw
include_directories(${GLFW_INCLUDES})
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "Use MSVC runtime library DLL" FORCE)
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} ${GLFW_NAME})
link_directories(${GLFW_LIB})

# glad
include_directories(${GLAD_INCLUDES})
add_subdirectory(${GLAD_DIR})
target_link_libraries(${PROJECT_NAME} ${GLAD_NAME})
link_directories(${GLAD_LIB})

# json (nlohmann/json)
include_directories(${JSON_INCLUDES})
add_subdirectory(${JSON_DIR} EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

# ImGui
include_directories(${IMGUI_INCLUDES})

# FreeImage
include_directories(${FREEIMAGE_INCLUDES})
target_link_libraries(${PROJECT_NAME} "${FREEIMAGE_LIB_DIR}/FreeImage.lib")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FREEIMAGE_LIB_DIR}/FreeImage.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying FreeImage.dll to output directory"
)

# MyStl
include_directories(${MYSTL_SOURCE})
add_subdirectory(${MYSTL_DIR})
target_link_libraries(${PROJECT_NAME} ${MYSTL_NAME})
link_directories(${MYSTL_SOURCE})

include_directories(${CUDA_SOURCE})
add_subdirectory(${CUDA_DIR})
target_link_libraries(${PROJECT_NAME} ${CUDA_NAME})
link_directories(${CUDA_SOURCE})

# Engine
include_directories(${ENGINE_SOURCE})
add_subdirectory(${ENGINE_DIR})
target_link_libraries(${PROJECT_NAME} ${ENGINE_NAME})
link_directories(${ENGINE_SOURCE})

# Game
include_directories(${PROGRAM_SOURCE})
add_subdirectory(${PROGRAM_SOURCE})
target_link_libraries(${PROJECT_NAME} ${PROGRAM_NAME})
link_directories(${PROGRAM_SOURCE})

message("End generate. . . . . . . . . . . . . . . .")
