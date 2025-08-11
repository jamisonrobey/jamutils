cmake_minimum_required(VERSION 3.30)
project(jamutils)
add_library(jamutils STATIC jamutils/FD.h jamutils/M_Map.h)
target_include_directories(jamutils PUBLIC ${PROJECT_SOURCE_DIR})
