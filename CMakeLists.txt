cmake_minimum_required(VERSION 3.30)
project(jamutils)
add_library(jamutils INTERFACE jamutils/FD.h jamutils/M_Map.h)
target_include_directories(jamutils INTERFACE ${PROJECT_SOURCE_DIR})
