project(RayTraingLauncher)

cmake_minimum_required (VERSION 3.8)
set(CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(./sources SRC)
include_directories(includes)

add_executable (RayTraingLauncher "main.cpp" ${SRC})