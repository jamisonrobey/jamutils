cmake_minimum_required(VERSION 3.5...4.0)
project(jamutils)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRCS "jamutils/*.h" "jamutils/dummy.cpp")
add_library(jamutils STATIC ${SRCS})

target_include_directories(jamutils PUBLIC ${PROJECT_SOURCE_DIR})
