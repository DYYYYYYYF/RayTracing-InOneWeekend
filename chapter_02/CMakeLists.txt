cmake_minimum_required(VERSION 3.0.0)
project(Image)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./src DIR_SRCS)
include_directories(./include)

add_executable(Image main.cpp ${DIR_SRCS})
project(Image)
