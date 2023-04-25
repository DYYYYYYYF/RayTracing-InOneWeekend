cmake_minimum_required(VERSION 3.2.0)
project(RayTracing)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

aux_source_directory(./src DIR_SRCS)
aux_source_directory(./include DIR_INC)

add_executable(RayTracing main.cpp ${DIR_INC} ${DIR_SRCS})

include_directories(./include)
