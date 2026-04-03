cmake_minimum_required(VERSION 4.2)

message("Begin generate CUDA. . . . . . . . . . . . . . . .")

file(GLOB_RECURSE INCLUDES ${CUDA_SOURCE}/*.h)

if(GRAVITY_HAS_CUDA)
	set(CMAKE_CUDA_ARCHITECTURES native)

	file(GLOB_RECURSE SOURCES  ${CUDA_SOURCE}/*.cu)
	source_group(TREE ${CUDA_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})
	
	add_library(${CUDA_NAME} STATIC ${INCLUDES} ${SOURCES})
	find_package(CUDAToolkit REQUIRED)
	target_link_libraries(${CUDA_NAME} PUBLIC CUDA::cudart)
		
	set_target_properties(${CUDA_NAME} PROPERTIES
		CUDA_STANDARD 20
		CUDA_STANDARD_REQUIRED ON
		CUDA_SEPARABLE_COMPILATION ON
		CUDA_RESOLVE_DEVICE_SYMBOLS ON)
else()
	set(CMAKE_CXX_STANDARD 23)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	file(GLOB_RECURSE SOURCES ${CUDA_SOURCE}/*.cpp)
	source_group(TREE ${CUDA_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})
	add_library(${CUDA_NAME} STATIC ${INCLUDES})
endif()

message("End generate CUDA. . . . . . . . . . . . . . . .")
