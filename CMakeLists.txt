cmake_minimum_required (VERSION 3.8)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Library)

find_package(OpenGL REQUIRED)

message("-- Current environment: Windows")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	
if(DEFINED ENV{VULKAN_SDK})
set(Vulkan_DIR $ENV{VULKAN_SDK})
else()
message(FATAL_ERROR "VULKAN_SDK environment variable is not set.")
endif()

set(Vulkan_LIBRARY_DIRS "${Vulkan_DIR}/Lib")
set(Vulkan_INCLUDE_DIRS "${Vulkan_DIR}/Include")
	
include_directories(${Vulkan_INCLUDE_DIRS})
link_directories(${Vulkan_LIBRARY_DIRS} ${UD_LOGGER_DIR}/lib/Windows/${CMAKE_BUILD_TYPE})


include_directories(includes includes/imgui Platform)
aux_source_directory(sources SRC)
aux_source_directory(Platform PLATFORM_SRC)

add_executable (RayTracer "main.cpp" ${SRC} ${PLATFORM_SRC})

target_link_libraries(RayTracer PUBLIC "vulkan-1.lib" OpenGL::GL)