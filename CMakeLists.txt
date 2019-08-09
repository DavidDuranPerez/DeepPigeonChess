# Distributed under the MIT License (See accompanying file /LICENSE )

# CMake build : global project

cmake_minimum_required (VERSION 3.3)

project (DeepPigeonChess)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

add_subdirectory (src)
