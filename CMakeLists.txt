cmake_minimum_required (VERSION 3.8)
project(RayTracer)

set(CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Library)

include_directories(includes)
aux_source_directory(sources SRC)

add_subdirectory(glfw)
add_executable (RayTracer "main.cpp" ${SRC})

target_link_libraries(RayTracer PUBLIC glfw)
